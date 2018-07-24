#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ilm/ilm_control.h>
#include <ilm/ilm_types.h>


#include "General_StreamInput.h"
#include "ScreenSaverPlayer.h"
#include "SVPSingleton.h"

// #include <io.h>

#define ENABLE_WRITE_OUTPUT
#ifdef ENABLE_WRITE_OUTPUT
#define OUTPUT_FILE "tmpdump.yuv"
#endif

#define OFF_SET_X 0
#define OFF_SET_Y 0
#define DISP_WIDTH 1280
#define DISP_HEIGHT 720
#define WIDTH 1280
#define HEIGHT 720
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static int surface_id;

#define LOG(fmt, ...) \
        do { fprintf(stderr, "[%s][%d]" fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); } while (0)

ScreenPlayer* ScreenPlayer::m_pInstance = NULL;

int CIGetTime()
{
    struct timeval tv;
    gettimeofday(&tv, 0);   /* Get system time */
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static void PFN_InitLock_IMPL(void** ppLock)
{

}

static void PFN_DeleteLock_IMPL(void** ppLock)
{
}

static void PFN_Lock_IMPL(void* pLock)
{
}

static void PFN_UnLock_IMPL(void* pLock)
{
}

ScreenPlayer* ScreenPlayer::GetInstance() 
{
    if (!m_pInstance) {
        m_pInstance = new ScreenPlayer();
    }

    return m_pInstance;
}


ScreenPlayer::ScreenPlayer() :
m_renderActivity(this, &ScreenPlayer::thread_RenderFrame)
,m_videoShowActivity(this, &ScreenPlayer::runVideoShowThread)
,m_decoderActivity(this, &ScreenPlayer::runVideoDecodeThread)
,m_renderLoopEnd(0)
,m_bIsVidDecOpen(false)
{

}

ScreenPlayer::~ScreenPlayer()
{


}


int ScreenPlayer::OpenPlayfile(const char* filepath)
{
    SVP_INFO("OpenPlayfile");
    int render_frame;

    pInFile = fopen(filepath, "rb");
    if (NULL == pInFile)
    {
        LOG("[ERR] open input file %s", filepath);
        return -1;
    }
}

int ScreenPlayer::ClosePlayfile()
{
    if (pInFile)
    {
        fclose(pInFile);
        pInFile = NULL;
    }
}

void ScreenPlayer::initOpenOptions()
{
    OpenOptions = { 0 }; 
    //打开的视频流设置
    OpenOptions.DispQueue.pfnCreateLock = PFN_InitLock_IMPL;
    OpenOptions.DispQueue.pfnDestroyLock = PFN_DeleteLock_IMPL;
    OpenOptions.DispQueue.pfnLock = PFN_Lock_IMPL;
    OpenOptions.DispQueue.pfnUnlock = PFN_UnLock_IMPL;
    OpenOptions.s32Width = WIDTH;
    OpenOptions.s32Height = HEIGHT;
    OpenOptions.s32DispWidth = DISP_WIDTH;
    OpenOptions.s32DispHeight = DISP_HEIGHT;
    OpenOptions.s32OffsetX = OFF_SET_X;
    OpenOptions.s32OffsetY = OFF_SET_Y;
    OpenOptions.s32ScreenWidth = SCREEN_WIDTH;
    OpenOptions.s32ScreenHeight = SCREEN_HEIGHT;
}


void ScreenPlayer::resetVideoParams()
{
    DecOptions = { 0 };
    u32InpConsumed = 0;
    u32FramesDecoded = 0;
    u32NumberOfSkippedFrames = 0;
    FrmOptions = { 0 };
    Frame = { 0 };
    u32Status = 0;
    RenderOptions = { 0 };
    RenderOptions.u32ShowVideo = 1;
}

void ScreenPlayer::runVideoShowThread()
{
    SetDynimicScreenShow(true);
}


bool ScreenPlayer::SetDynimicScreenShow(bool bshow)
{
    bool res = false;
    surface_id = 20000000 + getpid();
	res = ilm_init();
    res = ilm_surfaceSetOpacity(3001, !bshow);
    ilm_commitChanges();
	res = ilm_surfaceSetOpacity(surface_id, bshow);
    ilm_commitChanges();
    return res;
}

void ScreenPlayer::thread_RenderFrame()
{

    VDEC_GetFrameOptions_t FrmOptions = {0};
    VDEC_Frame_t Frame = {0};
    VDEC_RenderFrameOptions_t RenderOptions = {0};

    CI_U64 u64StartTS;
    CI_U64 u64StartTime;

    RenderOptions.u32ShowVideo = 1;
 
    // SetDynimicScreenShow(true);
    while(!m_renderActivity.isStopped())
    {
        if(CI_SOK!=VDEC().getFrame(pDec, &FrmOptions, &Frame))
        {
            usleep(50000);
            if(m_renderLoopEnd == 0)
            {
                usleep(1*1000);
                continue;
            }
            break;
        }
            

        if(m_renderDisplayCount == 0)
        {
            u64StartTS = Frame.Pts.u64Timestamp;
            u64StartTime = CIGetTime();
        }

        m_renderDisplayCount ++;

        VDEC().renderFrame(pDec, &RenderOptions, &Frame);
        VDEC().releaseFrame(pDec, &Frame);
        m_videoShowActivity.start();
     
    }
}


int ScreenPlayer::OpenDynimicScreen(const char* StreamPath)
{
    if(m_bIsVidDecOpen)
    {
        return 0;
    }


    count = 0;
    BufLen = 0;
    initOpenOptions();

    resetVideoParams();

    int ret = VDEC().create(&pDev, NULL);

    OpenPlayfile(StreamPath);
    m_sDisplayStatus = true;

//初始化第一次解码数据
    ret = StreamInitBuffer(&StreamBuffer, &StreamCurPos, &BufLen);
    if (ret != 0)
    {
        printf("Fail to initialize stream buffer! \n");
        return 0;
    }

    StreamReloadBuffer(StreamBuffer, &StreamCurPos, &BufLen, pInFile);

	VDEC().open(pDev, &pDec, &OpenOptions);

	int check_AU = 1;
    VDEC().set(pDec, CI_VIDEC_PROPID_COMPLETEPIC, &check_AU, sizeof(check_AU));

    m_renderLoopEnd = 0;

    m_renderActivity.start();

    total_consume = 0;

    DecOptions.Pts.u8Valid = 1;
    DecOptions.Pts.u64Timestamp = 0;
    DecOptions.Pts.u32TimeScale = 1000;
    m_bIsVidDecOpen = true;
    m_decoderActivity.start();
    return 0;
}


void ScreenPlayer::runVideoDecodeThread()
{
    int ret;
    while(!m_renderActivity.isStopped() && m_sDisplayStatus)
    {
        if (!m_decoderMutex.tryLock(500)) 
        {
            return ;
        }

        int framelen = 0;
        framelen = BufLen - (StreamCurPos - StreamBuffer);
        if(framelen == 0)
        {
            usleep(50000);
        }


        ret = VDEC().decodeBuffer(pDec, &DecOptions, StreamCurPos, framelen, &u32InpConsumed, &u32FramesDecoded, &u32NumberOfSkippedFrames, &u32Status); 
        
        if (ret == CI_SOK)
        {
            count ++;
            DecOptions.Pts.u8Valid = 0;
        }

        framelen -= u32InpConsumed;
        StreamCurPos += u32InpConsumed;

        total_consume += u32InpConsumed;

        if (ret != CI_EOUTBUFTOOSMALL)
        {
            ret = StreamReloadBuffer(StreamBuffer, &StreamCurPos, &BufLen, pInFile);

            if (ret < 0)
            {
                count = 0;
                fseek(pInFile, 0, SEEK_SET);
            }
            DecOptions.Pts.u64Timestamp = m_renderDisplayCount * 25;
            DecOptions.Pts.u8Valid = 1;
        }

        m_decoderMutex.unlock();
//出错处理
        if ((CI_RESULT)CI_EOUTOFMEMORY == ret || (CI_RESULT)CI_EFAIL == ret)
        {
            break;
        }

    }
}

void ScreenPlayer::CloseDynimicScreen()
{
    Poco::Mutex::ScopedLock lck(m_decoderMutex);

    if(!m_bIsVidDecOpen)
    {
        SVP_ERROR(" Already CloseDynimicScreen ,Pls Don't close again!!!");  
        return;
    }

    m_sDisplayStatus = false;
    SetDynimicScreenShow(false);

     
    if(!m_pInstance)
    {
        SVP_ERROR(" CloseDynimicScreen failed,has not OpenDynimicScreen Success yet!");
    }
    else
    {
        m_renderLoopEnd = 1;
        m_decoderActivity.stop();
        m_renderActivity.stop();
        // m_videoShowActivity.stop();
        try {
            m_renderActivity.wait(2000);
            // m_videoShowActivity.wait(2000);
        } catch (Poco::Exception& e) {
            SVP_INFO("videoDecoderClose Poco::Exception %s", e.displayText().c_str());
        }

		VDEC().close(pDev, pDec);

        if (StreamBuffer)
        {
            StreamFreeBuffer(StreamBuffer);
            StreamBuffer = NULL;
        }

        ClosePlayfile();

        // VDEC().release(pDev);
		VDEC().release(pDev);

        m_bIsVidDecOpen = false;
    }

}



//--------------------------------------以下是刷黑帧的函数-----------------------------------------
CI_U8* ScreenPlayer::fillFrame(VDEC_Frame_t *pFrame, int width, int height) {
    CI_U8 *pBuf, *pYUV[3];
    int stride = (width + 15) & ~15;
    int size = stride * height;
    if (size == 0 || pFrame == NULL)
        return NULL;
    pBuf = (CI_U8*)malloc(size * 2 * sizeof(CI_U8*));
    if (pBuf == NULL)
        return NULL;

    pYUV[0] = pBuf;
    pYUV[1] = pYUV[0] + size;
    pYUV[2] = pYUV[1] + (size >> 2);

    //black buffer
    memset(pYUV[0], 0x10, size);
    memset(pYUV[1], 0x80, (size >> 2));
    memset(pYUV[2], 0x80, (size >> 2));

    //store frame
    pFrame->u32ChromaFormat = 2; //I420
    pFrame->apu8Frame[0] = pYUV[0];
    pFrame->apu8Frame[1] = pYUV[1];
    pFrame->apu8Frame[2] = pYUV[2];
    pFrame->au32Width[0] = width;
    pFrame->au32Stride[0] = stride;
    pFrame->au32Height[0] = height;
    pFrame->au32Width[1] = pFrame->au32Width[2] = pFrame->au32Width[0] >> 1;
    pFrame->au32Stride[1] = pFrame->au32Stride[2] = pFrame->au32Stride[0] >> 1;
    pFrame->au32Height[1] = pFrame->au32Height[2] = pFrame->au32Height[0] >> 1;

    return pBuf;
}

void ScreenPlayer::videoFillBlackFrame() {
    if (!m_bIsVidDecOpen) {
        return;
    }
    CI_U8 *pFillBuf = NULL;
    VDEC_Frame_t FillFrame = { 0 }, *pFillFrame = &FillFrame;
    pFillBuf = fillFrame(pFillFrame, DISP_WIDTH, DISP_HEIGHT);
    if (pFillBuf) {
        VDEC().renderFrame(pDec, &RenderOptions, &Frame);
    }

    if (pFillBuf)
        free(pFillBuf);
}

void ScreenPlayer::videoSetFlushFrame(bool bflush) {
    m_bNeedFillBlackFram = bflush;
}
