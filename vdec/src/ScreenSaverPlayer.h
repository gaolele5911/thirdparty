
// #include "SVPType.h"
// #include <string.h>
#include "Poco/Activity.h"
#include "Poco/Mutex.h"
#include "SVPLog.h"
#include "IVDEC.h"
using namespace std;


class ScreenPlayer
{
public:
    static ScreenPlayer* GetInstance();
    //用于循环播放同一个H264视频流
    int OpenDynimicScreen(const char* StreamPath);

    //关闭当前播放视频流
    void CloseDynimicScreen();
private:
    void initOpenOptions();

    void resetVideoParams();

    int OpenPlayfile(const char* filepath);

    int ClosePlayfile();

    CI_U8* fillFrame(VDEC_Frame_t *pFrame, int width, int height);

    void videoFillBlackFrame();

    void videoSetFlushFrame(bool bflush);

    void runVideoShowThread();

    bool SetDynimicScreenShow(bool bshow);

    void runVideoDecodeThread();

private:
    ScreenPlayer();
    ~ScreenPlayer();
private:
    static ScreenPlayer* m_pInstance;
    void thread_RenderFrame();

    Poco::Activity<ScreenPlayer> m_renderActivity;
    Poco::Activity<ScreenPlayer> m_videoShowActivity;
    Poco::Activity<ScreenPlayer> m_decoderActivity;

    Poco::Mutex m_mutex;
    Poco::Mutex m_decoderMutex;

    FILE *pInFile = NULL;
    unsigned char *StreamBuffer = NULL; 
    unsigned char *StreamCurPos = NULL;

    bool m_bNeedFillBlackFram;
    bool m_bIsVidDecOpen;
    bool m_sDisplayStatus;
    bool m_userDisplayStatus;

    CI_VOID                     *pDev, *pDec;
    VDEC_DecodeOptions_t        DecOptions;
    CI_U32                      u32InpConsumed;
    CI_U32                      u32FramesDecoded;
    CI_U32                      u32NumberOfSkippedFrames;
    VDEC_GetFrameOptions_t      FrmOptions;
    VDEC_Frame_t                Frame;
    CI_U32                      u32Status;
    VDEC_RenderFrameOptions_t   RenderOptions;
    VDEC_Frame_t                FillFrame, *pFillFrame=&FillFrame;

    int m_renderLoopEnd;
    int m_renderDisplayCount;
    int count;
    int BufLen;
    int codec;
    int total_consume;
    int framelen;

    VDEC_OpenOptions_t OpenOptions; 
};



