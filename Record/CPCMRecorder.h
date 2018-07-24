/*
 * CPCMRecorder.h
 * CPCMRecorder use ALSA
 *  Created on: 2013年11月15日
 *      Author: pffang
 */
#pragma once
#ifndef CPCMRECORDER_H_
#define CPCMRECORDER_H_
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <string>
#include <exception>
#include <sys/time.h>

#include "SVPMutex.h"

namespace VA {
using namespace std;

void __PthreadSleep(pthread_cond_t * cond, pthread_mutex_t * mutex,
                    unsigned long int millisecond);

//return -2 to stop
typedef int (*PCMRecordCallback)(void *Buffer, snd_pcm_sframes_t nFrames, bool xrun,
                               void *userData);

struct StreamOptions {
    int alsa_device; /*-2:use name, -1, use default, >=0:use plughw:x */
    snd_pcm_format_t format;
    unsigned int sampleRate;
    unsigned int channels;
    bool minimize_latency;
    unsigned int numberOfBuffers; /* Number of stream buffers. */
    string alsa_device_name;
    // Default constructor.
    StreamOptions()
            : alsa_device(-1),
              format(SND_PCM_FORMAT_S16_LE),
              sampleRate(16000),
              channels(1),
              minimize_latency(false),
              numberOfBuffers(0) {
    }
    // Copy constructor
    StreamOptions(const StreamOptions& src) {
        alsa_device = src.alsa_device;
        format = src.format;
        sampleRate = src.sampleRate;
        channels = src.channels;
        minimize_latency = src.minimize_latency;
        numberOfBuffers = src.numberOfBuffers;
        alsa_device_name = src.alsa_device_name;
    }
};

class PCMRecorderError : public exception
{
 public:
  //! The constructor.
    PCMRecorderError( const std::string& message) throw() : message(message){}
  //! The destructor.
  ~PCMRecorderError( void ) throw() {}
  //! Prints thrown error message to stderr.
  void printMessage( void ) const throw() { cerr << message << endl; }
  //! Returns the thrown error message string.
  const std::string& getMessage(void) const throw() { return message; }
  //! Returns the thrown error message as a c-style string.
  const char* what( void ) const throw() { return message.c_str(); }
 protected:
  std::string message;
};

class CPCMRecorder {
    enum StreamState {
        STREAM_CLOSED = 0,
        STREAM_STOPPED,
        STREAM_RUNNING,
        STREAM_PAUSED
    };

    volatile StreamState m_state;
    PCMRecordCallback m_pCallback;
    void* m_puserData;
    pthread_mutex_t mutex;
    pthread_cond_t runnable_cv;

    snd_pcm_t* recordhandle;
    StreamOptions m_options;
    char* userBuffer;
    snd_pcm_sframes_t m_bufferFrames;
    volatile bool m_xrun;
    bool can_pause;

    pthread_t m_thread;
    int thread_priority;

    static void *threadhandler(void * ptr);
    void thread_working_proc();
    
    SVPMutex m_recordLock;

 public:
    CPCMRecorder();
    ~CPCMRecorder();

    void OpenStream(StreamOptions options, unsigned int *bufferFrames,
                   PCMRecordCallback callback, void *userData = NULL);
    void CloseStream();
    void StartStream();
    void StopStream();
};

} /* namespace VA */

#endif /* CPCMRECORDER_H_ */
