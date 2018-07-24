/*
 * CPCMRecorder.cpp
 *
 *  Created on: 2013年11月15日
 *      Author: pffang
 */

#include "CPCMRecorder.h"

namespace VA {

#define DEBUG_DETAIL 0

void __PthreadSleep(pthread_cond_t * cond, pthread_mutex_t * mutex,
                    unsigned long int millisecond) {
    struct timespec timeToWait;
    struct timeval now;
    unsigned long int s, us;

    pthread_mutex_lock(mutex);
#if defined _WIN32
    now.tv_sec = 0;
    now.tv_usec = 0;
#elif defined __linux
    gettimeofday(&now, NULL);
#endif
    us = now.tv_usec + 1000UL * millisecond;
    s = us / 1000000UL;
    us = us - s * 1000000UL;

    timeToWait.tv_sec = now.tv_sec + s;
    timeToWait.tv_nsec = us * 1000UL;
    pthread_cond_timedwait(cond, mutex, &timeToWait);
    pthread_mutex_unlock(mutex);
}

CPCMRecorder::CPCMRecorder()
        : m_state(STREAM_CLOSED),
          m_pCallback(NULL),
          m_puserData(NULL),
          recordhandle(NULL),
          userBuffer(NULL),
          m_bufferFrames(0),
          m_xrun(false),
          can_pause(false),
          m_thread(0),
          thread_priority(0) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&runnable_cv, NULL);
}

CPCMRecorder::~CPCMRecorder() {
    if (STREAM_RUNNING == m_state || STREAM_PAUSED == m_state)
        StopStream();
    if (STREAM_STOPPED == m_state)
        CloseStream();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&runnable_cv);
}

void CPCMRecorder::OpenStream(StreamOptions options, unsigned int *bufferFrames,
                              PCMRecordCallback callback, void *userData) {
    __label__ OpenStreamUnlock;
    m_recordLock.Lock();
    cerr << "-->in Open" << endl;
    string errMessage;
    ssize_t bufferBytes = 0;
    int result = 0;
    snd_pcm_uframes_t periodSize = 0;
    unsigned int periods = 0;
    snd_pcm_sw_params_t *sw_params = NULL;
#ifdef SCHED_RR
    int min = 0;
    int max = 0;
#endif
    if (STREAM_CLOSED != m_state) {
        
        m_recordLock.Unlock();
        
        //errMessage = "stream already opened";
        //throw(PCMRecorderError(errMessage));在RCF线程抛出异常会导致app崩溃
        return;
    }
    int dir = 0;
    string device_name;

    pthread_mutex_lock(&mutex);
    m_options = options;
    m_pCallback = callback;
    m_puserData = userData;
    if (!m_pCallback) {
        errMessage = "callback can not be NULL";
        goto OpenStreamUnlock;
    }
#if DEBUG_DETAIL
    snd_output_t *out;
    snd_output_stdio_attach(&out, stderr, 0);
#endif

    if (-1 == m_options.alsa_device)
        device_name = "default";
    else if (-2 == m_options.alsa_device)
        device_name = m_options.alsa_device_name.c_str();
    else if (0 <= m_options.alsa_device) {
        char num[16];
        snprintf(num, 16, "%d", m_options.alsa_device);
        device_name = "plughw:";
        device_name.append(num);
    } else {
        errMessage = "alsa device number can not be negative";
        goto OpenStreamUnlock;
    }

    result = snd_pcm_open(&recordhandle, device_name.c_str(),
                          SND_PCM_STREAM_CAPTURE, 0 /*SND_PCM_ASYNC*/);
    if (result < 0) {
        errMessage = "can't open audio device(" + device_name + "): "
                + snd_strerror(result);
        goto OpenStreamUnlock;
    }

// Fill the parameter structure.
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_alloca(&hw_params);
    result = snd_pcm_hw_params_any(recordhandle, hw_params);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error getting parameters:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }
#if DEBUG_DETAIL
    cerr << "\n dump hardware params just after device open:\n" << endl;
    snd_pcm_hw_params_dump(hw_params, out);
#endif
    if (snd_pcm_hw_params_can_pause(hw_params)
            && snd_pcm_hw_params_can_resume(hw_params)) {
        can_pause = true;
    }
// Set access ... check user preference.
    result = snd_pcm_hw_params_set_access(recordhandle, hw_params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting access:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

    result = snd_pcm_hw_params_set_format(recordhandle, hw_params,
                                          m_options.format);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting format:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

// Set the sample rate.
    result = snd_pcm_hw_params_set_rate_near(recordhandle, hw_params,
                                             &(m_options.sampleRate), 0);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting sample rate:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

// Set the device channels.
    result = snd_pcm_hw_params_set_channels(recordhandle, hw_params,
                                            m_options.channels);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting channels:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

// Set the buffer (or period) size.
    periodSize = *bufferFrames;
    result = snd_pcm_hw_params_set_period_size_near(recordhandle, hw_params,
                                                    &periodSize, &dir);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting period size:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }
    *bufferFrames = periodSize;
    m_bufferFrames = *bufferFrames;

// Set the buffer number, which in ALSA is referred to as the "period".
    periods = 0;
    if (m_options.minimize_latency)
        periods = 2;
    else {
        if (m_options.numberOfBuffers > 0)
            periods = m_options.numberOfBuffers;
        if (periods < 2)
            periods = 4;  // a fairly safe default value
    }
    result = snd_pcm_hw_params_set_periods_near(recordhandle, hw_params,
                                                &periods, &dir);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error setting periods:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }
// Install the hardware configuration
    result = snd_pcm_hw_params(recordhandle, hw_params);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error installing hardware configuration:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

#if DEBUG_DETAIL
    cerr << "\ndump hardware params after installation:\n" << endl;
    snd_pcm_hw_params_dump(hw_params, out);
#endif

// Set the software configuration to fill buffers with zeros and prevent device stopping on xruns.
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(recordhandle, sw_params);
    snd_pcm_sw_params_set_start_threshold(recordhandle, sw_params,
                                          *bufferFrames);
    snd_pcm_sw_params_set_stop_threshold(recordhandle, sw_params, ULONG_MAX);
    snd_pcm_sw_params_set_silence_threshold(recordhandle, sw_params, 0);

// The following two settings were suggested by Theo Veenker
    snd_pcm_sw_params_set_avail_min(recordhandle, sw_params, *bufferFrames);
//snd_pcm_sw_params_set_xfer_align(playhandle, sw_params, 1);

// here are two options for a fix
//snd_pcm_sw_params_set_silence_size(playhandle, sw_params, ULONG_MAX);
    snd_pcm_uframes_t val;
    snd_pcm_sw_params_get_boundary(sw_params, &val);
    snd_pcm_sw_params_set_silence_size(recordhandle, sw_params, val);

    result = snd_pcm_sw_params(recordhandle, sw_params);
    if (result < 0) {
        snd_pcm_close(recordhandle);
        errMessage = "error installing software configuration:";
        errMessage += snd_strerror(result);
        goto OpenStreamUnlock;
    }

#if DEBUG_DETAIL
    cerr << "\n: dump software params after installation:\n" << endl;
    snd_pcm_sw_params_dump(sw_params, out);
#endif

    bufferBytes = snd_pcm_frames_to_bytes(recordhandle, m_bufferFrames);

    userBuffer = new char[bufferBytes];
    if (userBuffer == NULL) {
        snd_pcm_close(recordhandle);
        errMessage = "error allocating user buffer memory.";
        goto OpenStreamUnlock;
    }
    m_state = STREAM_STOPPED;

// Set the thread attributes for joinable and realtime scheduling
// priority (optional).  The higher priority will only take affect
// if the program is run as root or suid. Note, under Linux
// processes with CAP_SYS_NICE privilege, a user can change
// scheduling policy and priority (thus need not be root). See
// POSIX "capabilities".
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

#ifdef SCHED_RR // Undefined with some OSes (eg: NetBSD 1.6.x with GNU Pthread)
    min = sched_get_priority_min( SCHED_RR);
    max = sched_get_priority_max( SCHED_RR);
    if (thread_priority < min)
        thread_priority = min;
    else if (thread_priority > max)
        thread_priority = max;
#endif
    result = pthread_create(&m_thread, &attr, threadhandler, this);
    pthread_attr_destroy(&attr);
    if (result) {
        errMessage = "error creating callback thread!";
        goto OpenStreamUnlock;
    }
    OpenStreamUnlock: pthread_mutex_unlock(&mutex);
    cerr << "<--out Open" << endl;
    m_recordLock.Unlock();
        
    //if (!errMessage.empty())
    //    throw(PCMRecorderError(errMessage));
    return;
}

void CPCMRecorder::CloseStream() {
    m_recordLock.Lock();
    cerr << "-->in Close" << endl;
    if (STREAM_CLOSED == m_state) {
        cerr << "no open stream to close!" << endl;
        
        m_recordLock.Unlock();
        
        return;
    }
    if (STREAM_STOPPED != m_state) {
        StopStream();
    }
    m_state = STREAM_CLOSED;
	pthread_mutex_lock(&mutex);
    pthread_cond_signal(&runnable_cv);
	pthread_mutex_unlock(&mutex);
    pthread_join(m_thread, NULL);
    snd_pcm_close(recordhandle);

    if (userBuffer) {
        delete[] userBuffer;
        userBuffer = NULL;
    }
    cerr << "<--out Close" << endl;
        
    m_recordLock.Unlock();
}
void CPCMRecorder::StartStream() {
    m_recordLock.Lock();
    cerr << "-->in Start" << endl;
    if (STREAM_CLOSED == m_state) {
        cerr << "no open stream!" << endl;
        
        m_recordLock.Unlock();
        
        return;
    }
    if (STREAM_RUNNING == m_state || STREAM_PAUSED == m_state) {
        cerr << "the stream is already running!";
        
        m_recordLock.Unlock();
        
        return;
    }
	m_state = STREAM_RUNNING;
    pthread_mutex_lock(&mutex);
    int result = 0;
    snd_pcm_state_t state;
    result = snd_pcm_drop(recordhandle);  //Fix: recieved some stale data when first start but has been opened for some time,
    state = snd_pcm_state(recordhandle);
    if (state != SND_PCM_STATE_PREPARED) {
        result = snd_pcm_prepare(recordhandle);
        if (result < 0) {
            //cerr << "error preparing output: " << snd_strerror(result) << endl;
            cerr << "error preparing output: " << snd_strerror(result) << endl;
        }
    }
//    result = snd_pcm_drop(handle[1]);  //Fix: recieved some stale data when first start but has been opened for some time,
//    state = snd_pcm_state(handle[1]);
//    if (state != SND_PCM_STATE_PREPARED) {
//        result = snd_pcm_prepare(handle[1]);
//        if (result < 0) {
//            errorStream_
//                    << "RtApiAlsa::startStream: error preparing input pcm device, "
//                    << snd_strerror(result) << ".";
//            errorText_ = errorStream_.str();
//            goto unlock;
//        }
//    }
	pthread_cond_signal(&runnable_cv);
    pthread_mutex_unlock(&mutex);
    cerr << "<--out Start" << endl;
    m_recordLock.Unlock();
}

void CPCMRecorder::StopStream() {
    //m_recordLock.Lock();StopStream会在thread_working_proc调用，这里去掉避免死锁。
    
    cerr << "-->in Stop" << endl;
    if (STREAM_CLOSED == m_state) {
        cerr << "no open stream!" << endl;
        
        //m_recordLock.Unlock();
        
        return;
    }
    if (STREAM_STOPPED == m_state) {
        cerr << "the stream is already stopped!";
        
        //m_recordLock.Unlock();
        
        return;
    }
	m_state = STREAM_STOPPED;
	pthread_mutex_lock(&mutex);
    pthread_cond_signal(&runnable_cv);
    int result = 0;
    result = snd_pcm_drop(recordhandle);
    if (result < 0) {
        cerr << "error draining output pcm device: " << snd_strerror(result)
             << endl;
    }
    pthread_mutex_unlock(&mutex);
    cerr << "<--out Stop" << endl;

    //m_recordLock.Unlock();
}

void* CPCMRecorder::threadhandler(void * ptr) {
    CPCMRecorder *pThis = (CPCMRecorder *) ptr;

//#ifdef SCHED_RR // Undefined with some OSes (eg: NetBSD 1.6.x with GNU Pthread)
//    pthread_t tID = pthread_self();  // ID of this thread
//    sched_param prio = { pThis->thread_priority };  // scheduling priority of thread
//    pthread_setschedparam(tID, SCHED_RR, &prio);
//#endif
    pThis->thread_working_proc();
    pthread_exit( NULL);
}
void CPCMRecorder::thread_working_proc() {
    while (STREAM_CLOSED != m_state) {
        __label__ WorkingUnlock;
        pthread_testcancel();
        if (STREAM_RUNNING != m_state) {
            pthread_mutex_lock(&mutex);
            cerr << "in wait" << endl;
			if(STREAM_STOPPED == m_state) {
				pthread_cond_wait(&runnable_cv, &mutex);
			}
            cerr << "out wait" << endl;
            if (STREAM_CLOSED == m_state) {
                pthread_mutex_unlock(&mutex);
                return;
            }
            pthread_mutex_unlock(&mutex);
        }
        if (STREAM_CLOSED == m_state) {
            cerr << "the stream is closed ... this shouldn't happen!" << endl;
            return;
        }

        
        int result = 0;
        int doStopStream = 0;
        snd_pcm_sframes_t frames = m_bufferFrames;

        snd_pcm_state_t state;
        pthread_mutex_lock(&mutex);
        // The state might change while waiting on a mutex.
        if (STREAM_STOPPED == m_state || STREAM_PAUSED == m_state)
            goto WorkingUnlock;

        state = snd_pcm_state(recordhandle);
        if (SND_PCM_STATE_XRUN == state) {
            m_xrun = true;
            cerr << "preparing device after resume" << endl;
            result = snd_pcm_prepare(recordhandle);
            if (result < 0) {
                cerr << "error preparing device after resume: "
                     << snd_strerror(result) << endl;
            }
        } else if (SND_PCM_STATE_RUNNING != state
                && SND_PCM_STATE_PREPARED != state) {
            cerr << "error, current state is " << snd_pcm_state_name(state)
                 << ", " << snd_strerror(result) << endl;
        }
        frames = snd_pcm_readi(recordhandle, userBuffer, m_bufferFrames);
        if (frames < m_bufferFrames) {
            // Either an error or underrun occured.
            if (frames == -EPIPE) {
                state = snd_pcm_state(recordhandle);
                if (SND_PCM_STATE_XRUN == state) {
                    m_xrun = true;
                    result = snd_pcm_prepare(recordhandle);
                    if (result < 0) {
                        cerr << "error preparing device after underrun: "
                             << snd_strerror(result) << endl;
                    }
                } else {
                    cerr << "error, current state is "
                         << snd_pcm_state_name(state) << ", "
                         << snd_strerror(result) << endl;
                }
            } else {
                cerr << "audio write error, " << snd_strerror(result) << endl;
            }
            goto WorkingUnlock;
        }
	doStopStream = 0;
        doStopStream = m_pCallback(userBuffer, m_bufferFrames, m_xrun,
                                   m_puserData);
		m_xrun = false;
        if (doStopStream == -2) {
            pthread_mutex_unlock(&mutex);
            StopStream();  //立刻停止
            return;
        }
        // Check stream latency
        //    result = snd_pcm_delay(playhandle, &frames);
        //    if (result == 0 && frames > 0)
        //        stream_.latency[0] = frames;

        WorkingUnlock: pthread_mutex_unlock(&mutex);
    }
}

} /* namespace VA */
