#include "SVPNaviView.h"
#include "SVPNaviQML.h"
#include "appfw/ISVPAppFw.h"
#include "appfw/gui/ISVPAppFw_GUI_WS.h"

#include "SVPLog.h"
#include "launcherservice.h"

#include "ExtSspLink.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG         "ssplink|view"

SVPNaviView::SVPNaviView()
:   SVPAppFw_GUI_View()
{
    SVP_INFO_FUNC()

    LauncherService::init(m_qmlAppEngine);
    m_qmlAppEngine.loadData(SVP_NAVI_QML);
}

SVPNaviView::~SVPNaviView()
{

}

int32_t SVPNaviView::onInitialize()
{
    SVPAppFw_GUI_View::onInitialize();
    //================== 测试代码 begin =======================
    //InitFinish需在PcmInit之后才调用
    ExtSsp_InitFinish(SVPNaviView::onProcMsg);
	pthread_t m_tid;
	pthread_create(&m_tid, NULL, TestThread, (void*)this);
	//=================== 测试代码 end ========================

    return 0;
}

void* SVPNaviView::TestThread(void *arg)
{
	SVP_INFO("Language ID %d", ExtSsp_GetLanguageID());

	SVP_INFO("SVExt::MSG_TIMEFORMAT_CHANGE %d", ExtSsp_GetTimeFormat());

	SVP_INFO("SVExt::MSG_WIFICONNECT_CHANGE %d", ExtSsp_GetWifiConnected());

	SVP_INFO("PlayBeep(SkOperSound_Click)");
	//蓝牙状态、本地蓝牙地址、手机蓝牙地址
	SVP_INFO("SVExt::MSG_BTCONNECT_CHANGE %d", ExtSsp_GetBTConnectStatus());
	SVP_INFO("Local BT Addr %s", ExtSsp_GetLocalBTAddr());
	SVP_INFO("Mobile BT Addr %s", ExtSsp_GetMobileBTAddr());

	//播放短按按键音
	ExtSsp_PlayBeep(0x00);
	sleep(1);
	SVP_INFO("PlayBeep(SkOperSound_LongPress)");
	//播放长按按键音
	ExtSsp_PlayBeep(0x01);
	//返回主界面
	ExtSsp_SoftKey(SVExt::SOFTKEY_MENU);
	//启动SspLink 不同的HMI App
	ExtSsp_StartApp(73);

	//在RunStarted && LinkStarted才允许进入SspLink
	ExtSsp_SetRunStatus(SVExt::ExtSsp_Run_Started);
	ExtSsp_SetLinkStatus(SVExt::ExtSsp_Link_Started);
}


void SVPNaviView::onProcMsg(uint32_t evt_id, int argc, void *argv[], int *args, int fromID)
{
	switch (evt_id) {
	case SVExt::MSG_LANGUAGE_CHANGE: {
		SVP_INFO("Language ID %d", ExtSsp_GetLanguageID());
	}
		break;
	case SVExt::MSG_TIMEFORMAT_CHANGE: {
		SVP_INFO("SVExt::MSG_TIMEFORMAT_CHANGE %d", ExtSsp_GetTimeFormat());
	}
		break;
	case SVExt::MSG_WIFICONNECT_CHANGE: {
		SVP_INFO("SVExt::MSG_WIFICONNECT_CHANGE %d",
				ExtSsp_GetWifiConnected());
	}
		break;
	case SVExt::MSG_POWERLEVEL_CHANGE: {
		SVP_INFO("SVExt::MSG_POWERLEVEL_CHANGE %d", ExtSsp_GetPowerLevel());
	}
		break;
	case SVExt::MSG_BTCONNECT_CHANGE: {
		SVP_INFO("SVExt::MSG_BTCONNECT_CHANGE %d", ExtSsp_GetBTConnectStatus());
		SVP_INFO("Local BT Addr %s", ExtSsp_GetLocalBTAddr());
		SVP_INFO("Mobile BT Addr %s", ExtSsp_GetMobileBTAddr());
	}
		break;
	case SVExt::MSG_TOSSP_SHOW:
	{
		//东软根据自己的状态，决定显示哪个AppID
		ExtSsp_StartApp(73);
		//后台启动某个App
		int appid = 73;
		int param1 = 0;
		void* _a[] = {(void*)&appid, (void*)&param1};
		int   _s[] = {sizeof(appid), sizeof(param1)};
		ExtSsp_SendMsg(SVExt::MSG_SSPLINK_STARTAPP, EVT_ELEMENTS(_a), _a, _s);
	}
	break;
	case SVExt::MSG_TOSSP_HIDE:
	{
		//Desay App会置至前台
		//东软自我决策是否需做隐藏处理
		//也可在App的SVP_AppFw_OnActivation的回调处理
	}
	break;
	}
	//如果需要将事件抛送至主线程的onEvent函数处理，可调用：
	//ISVPAppFw::GetWS()->PostEvent(evt_id, argc, argv, args);
}
