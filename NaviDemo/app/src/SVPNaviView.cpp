#include "SVPNaviView.h"

#include "appfw/ISVPAppFw.h"
#include "appfw/gui/ISVPAppFw_GUI_WS.h"
#include "PositionMgr.h"
#include "app/SVPAppLanguage.h"
#include "SVPApp.h"
#include "SVPPcmPlayer.h"
//#include "device/input/SVPInputType.h"

#include <GLES2/gl2.h>
#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG         "navi|view"

SVPNaviView* g_pView = NULL;

SVPNaviView::SVPNaviView()
: SVPAppFw_GUI_View()
{
	g_pView = this;
}


void* SVPNaviView::TestThread(void *arg)
{
	pthread_detach(pthread_self());
    //主动获取语言
    SVP_INFO("Language ID %d", ExtNavi_GetLanguageID());

    uint8_t mode = 0;
    uint8_t level = 0;
    ExtNavi_GetBkLightMode(mode, level);
    SVP_INFO("SVExt::MSG_BACKLIGHT_CHANGE %d, %d", mode, level);

    ExtNavi_SetNavVol(9);
    SVP_INFO("SVExt::MSG_NAVVOL_CHANGE %d", ExtNavi_GetNavVol());

    SVP_INFO("SVExt::MSG_TIMEFORMAT_CHANGE %d", ExtNavi_GetTimeFormat());

	SVP_INFO("SVExt::MSG_WIFICONNECT_CHANGE %d", ExtNavi_GetWifiConnected());

	bool bAllowPlay = ExtNavi_RequestAudioGuidancePlay();
	SVP_INFO("g_pExtNavi->RequestAudioGuidancePlay(%d)", bAllowPlay);
	if (bAllowPlay)
	{
		SVPSingleton<SVPPcmPlayer>::getInstance()->Play("/svp/share/sound/keyclicklong.wav");
	}
    SVP_INFO("g_pExtNavi->ReleaseAudioGuidancePlay() ");
    ExtNavi_ReleaseAudioGuidancePlay();

    SVP_INFO("g_pExtNavi->PlayBeep(SkOperSound_Click)");
    //播放短按按键音
    ExtNavi_PlayBeep(0x00);
    sleep(5);
    SVP_INFO("g_pExtNavi->PlayBeep(SkOperSound_LongPress)");
    //播放长按按键音
    ExtNavi_PlayBeep(0x01);
    //返回主界面
    ExtNavi_SoftKey(SVExt::SOFTKEY_MENU);
	return 0;
}

SVPNaviView::~SVPNaviView()
{

}

int32_t SVPNaviView::onInitialize()
{
    SVPAppFw_GUI_View::onInitialize();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable(GL_BLEND);

    glClearDepthf(1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //初始化 -- 如果耗时过长，可放到子线程执行
    //DR、GPS信息接收类
//    SVPSingleton<CPositionMgr>::getInstance();
    //或者， 后面四个函数请补全
    ExtNavi_SubscribeGPS(SatPositionFunc, NULL, NULL, NULL, NULL);
    //初始化TTS PCM播报
    int32_t channels = 1;
    uint32_t sampleRate = 16000;
    SVPSingleton<SVPPcmPlayer>::getInstance()->PcmInit(channels, sampleRate);

    //InitFinish需在PcmInit之后才调用
    ExtNavi_InitFinish(SVPNaviView::onProcMsg);

    //家、公司、收藏列表、比例尺等级、地图模式，需在导航初始化完成时 & 状态有变化时，通知HUT
    UpdateHomeIsFav(true);
    UpdateOfficeIsFav(false);
    std::vector<std::string> favlist;
    favlist.push_back("丽日商场");
    favlist.push_back("华贸广场");
    UpdateFavList(favlist);
    UpdateZoomState(SVExt::ExtNav_Zoom_1k);
    UpdateMapMode(SVExt::ExtNav_MapMode_CarHead);

	pthread_t m_tid;
	pthread_create(&m_tid, NULL, TestThread, (void*)this);

    return 0;
}

void SVPNaviView::onProcMsg(uint32_t evt_id, int argc, void *argv[], int *args, int fromID)
{
	SVP_INFO("SVPNaviView::onProcMsg,evt_id[%d],argc[%d]",evt_id,argc)
	switch (evt_id) {
	case SVExt::MSG_NAVVOL_CHANGE: {
		SVP_INFO("SVExt::MSG_NAVVOL_CHANGE %d", ExtNavi_GetNavVol());
	}
		break;
	case SVExt::MSG_LANGUAGE_CHANGE: {
		SVP_INFO("Language ID %d", ExtNavi_GetLanguageID());
	}
		break;
	case SVExt::MSG_BACKLIGHT_CHANGE: {
		uint8_t mode = 0;
		uint8_t level = 0;
		ExtNavi_GetBkLightMode(mode, level);
		SVP_INFO("SVExt::MSG_BACKLIGHT_CHANGE %d, %d", mode, level);
	}
		break;
	case SVExt::MSG_TIMEFORMAT_CHANGE: {
		SVP_INFO("SVExt::MSG_TIMEFORMAT_CHANGE %d", ExtNavi_GetTimeFormat());
	}
		break;
	case SVExt::MSG_WIFICONNECT_CHANGE: {
		SVP_INFO("SVExt::MSG_WIFICONNECT_CHANGE %d", ExtNavi_GetWifiConnected());
	}
		break;
	case SVExt::MSG_POWERLEVEL_CHANGE: {
		SVP_INFO("SVExt::MSG_POWERLEVEL_CHANGE %d", ExtNavi_GetPowerLevel());
	}
		break;
	case SVExt::MSG_STOP_GUIDANCEPLAY: {
		//如果当前处于导航播报状态，导航需立即停止导航TTS语音播报
	}
		break;
	case SVExt::MSG_NAVASR_GO_TO_POI: {
		//搜索 & 跳转至POI分类界面， 省 市 县/区 POI分类名
		if (argc == 4)
		{
			std::string province = (char*) argv[0];
			std::string city = (char*) argv[1];
			std::string district = (char*) argv[2];
			std::string poiname = (char*) argv[3];
			SVP_INFO("SVPNaviView::onProcMsg, province[%s],city[%s],district[%s],poiname[%s]",province.c_str(),city.c_str(),district.c_str(),poiname.c_str())
		}
	}
		break;
	case SVExt::MSG_NAVASR_GO_HOME: {
		//导航至家
	}
		break;
	case SVExt::MSG_NAVASR_GO_OFFICE: {
		//导航至公司
	}
		break;
	case SVExt::MSG_NAVASR_GO_FAV: {
		//导航至收藏夹
		if (argc == 1) {
			std::string favName = (char*)argv[0];
			//导航至收藏夹中名为favName的地点，如果找不到，则跳转至收藏夹界面
		}
	}
		break;
	case SVExt::MSG_NAVASR_OPEN_OVERSPEED_WARNING: {
		//打开超速提醒
	}
	break;
	case SVExt::MSG_NAVASR_CLOSE_OVERSPEED_WARNING: {
		//关闭超速提醒
	}
	break;
	case SVExt::MSG_NAVASR_OPEN_WATCHDOG: {
		//打开电子看门狗
	}
	break;
	case SVExt::MSG_NAVASR_CLOSE_WATCHDOG: {
		//关闭电子看门狗
	}
	break;
	case SVExt::MSG_NAVASR_ZOOM_IN: {
		//地图缩小
	}
	break;
	case SVExt::MSG_NAVASR_ZOOM_OUT: {
		//地图放大
	}
	break;
	case SVExt::MSG_NAVASR_MAP_MODE_HEAD_UP: {
		//车头向上
	}
	break;
	case SVExt::MSG_NAVASR_MAP_MODE_NORTH_UP: {
		//北向上
	}
	break;
	case SVExt::MSG_NAVASR_MAP_MODE_3D: {
		//3D视图
	}
	break;
	case SVExt::MSG_NAVASR_SWITCH_ROUTE: {
		//切换路线
	}
	break;
	case SVExt::MSG_NAVASR_GET_MY_LOCATION: {
		std::string province = "广东省";
		std::string city = "惠州市";
		std::string district = "惠城区";
		std::string poiname = "德赛西威汽车电子股份有限公司";
		g_pView->UpdateMyLocation(province, city, district, poiname);
	}
	break;
	case SVExt::MSG_NAVASR_SHOW_MY_LOCATION: {
		//跳转到导航界面显示当前位置
	}
	break;
	case SVExt::MSG_NAVASR_REMAINING_TIME: {
		//当前地点到目的地时间，如1小时42分30秒
		g_pView->UpdateRemainTime(1*60*60 + 42*60 + 30);
	}
	break;
	case SVExt::MSG_NAVASR_CANCELROUTE: {
		//取消路线规划导航
	}
	break;
	}
	//如果需要将事件抛送至主线程的onEvent函数处理，可调用：
	//ISVPAppFw::GetWS()->PostEvent(evt_id, argc, argv, args);
}
// Self-update
void SVPNaviView::onUpdate(uint32_t update_tick)
{
    SVPAppFw_GUI_View::onUpdate(update_tick);
}

// Self-render
void SVPNaviView::onRender()
{
    SVPAppFw_GUI_View::onRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    float r = (random() % 255) / 255.0f;
    float g = (random() % 255) / 255.0f;
    float b = (random() % 255) / 255.0f;

    //SVP_INFO("Render: r: %f g:%f, b: %f", r, g, b);
    glClearColor(r, g, b, 1.0f);
}

void SVPNaviView::onEvent(int32_t evt_id, void** evt_data)
{
	SVPAppFw_GUI_View::onEvent(evt_id, evt_data);
	SVP_INFO_FUNC();
	//触摸消息
    if (evt_id == EVTID_INPUT_TOUCH)
    {
    	int32_t touch_id    = ((int32_t*)(evt_data[0]))[0];
    	int32_t touch_state = ((int32_t*)(evt_data[1]))[0];
        switch(touch_state)
        {
        case 0:
            {
                SVP_INFO("INPUT_TOUCH UP: id: %d", touch_id);
            }
            break;
        case 1:
            {
            	int32_t touch_x    = ((int32_t*)(evt_data[2]))[0];
                int32_t touch_y    = ((int32_t*)(evt_data[3]))[0];
                SVP_INFO("INPUT_TOUCH DOWN: id: %d x: %d y: %d", touch_id, touch_x, touch_y);
            }
            break;
        case 2:
            {
            	int32_t touch_x    = ((int32_t*)(evt_data[2]))[0];
            	int32_t touch_y    = ((int32_t*)(evt_data[3]))[0];
                SVP_INFO("INPUT_TOUCH MOTION: id: %d x: %d y: %d", touch_id, touch_x, touch_y);
            }
            break;
        }
    }
}

void SVPNaviView::UpdateHomeIsFav(bool bIsFav)
{
	void* _a[] = { (void*) &bIsFav };
	int _s[] = { sizeof(bIsFav) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_HOME_IS_FAV, EVT_ELEMENTS(_a), _a, _s);
}


void SVPNaviView::UpdateOfficeIsFav(bool bIsFav)
{
	void* _a[] = { (void*) &bIsFav };
	int _s[] = { sizeof(bIsFav) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_OFFICE_IS_FAV, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateFavList(const std::vector<std::string>& favlist)
{
	int listsize = favlist.size();
	if (listsize > 0)
	{
		void* _a[listsize];
		int _s[listsize];
		for (int i = 0; i < listsize; i++)
		{
			_a[i] = (void*)favlist[i].c_str();
			_s[i] = favlist[i].length() + 1;
		}
		ExtNavi_SendMsg(SVExt::MSG_NAV_FAV_LIST, EVT_ELEMENTS(_a), _a, _s);
	}
	else
	{
		ExtNavi_SendMsg(SVExt::MSG_NAV_FAV_LIST, 0, 0, 0);
	}
}

void SVPNaviView::UpdateZoomState(int value)
{
	// >= ExtNav_Zoom_MIN && <= ExtNav_Zoom_Max
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_ZOOM_STATE, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateMapMode(int value)
{
	// >= ExtNav_MapMode_North && <= ExtNav_MapMode_3D
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_MAP_MODE, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateRemainTime(int value)
{
	//单位：秒  >= 0
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_REMAINING_TIME, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateMyLocation(std::string province, std::string city, std::string district, std::string poiname)
{
	void* _a[] = {(void*)province.c_str(), (void*)city.c_str(), (void*)district.c_str(), (void*)poiname.c_str()};
	int _s[] = {province.length() + 1, city.length() + 1, district.length() + 1, poiname.length() + 1};
	ExtNavi_SendMsg(SVExt::MSG_NAV_MY_LOCATION, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::SatPositionFunc(const stSatPosition& val)
{
	SVP_INFO("SatPositionFunc %f, %f, %d", val._lat, val._lon, val._used_sats);
}


void SVPNaviView::UpdateCurrLoadType(int value)
{
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_CURRLOADTYPE, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateSpeedLimit(int value)
{
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_SPEEDLIMIT, EVT_ELEMENTS(_a), _a, _s);
}

void SVPNaviView::UpdateSpeedLimitStatus(int value)
{
	void* _a[] = { (void*) &value };
	int _s[] = { sizeof(value) };
	ExtNavi_SendMsg(SVExt::MSG_NAV_SPEEDLIMITSTATUS, EVT_ELEMENTS(_a), _a, _s);
}
