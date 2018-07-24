
#include "appfw/ISVPAppFw.h"
#include "SVPNaviView.h"
#include "app/SVPAppID.h"
#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG         "navi"

#define VID_NAVI_MAIN_VIEW       0

void SVP_AppFw_Config(SVPAppFw_Config& conf)
{
    SVP_INFO_FUNC();

    conf.appfw = "appfw-gui";
    conf.uifw = "uifw-gles";
    conf.appid = SVPApp::APPID_NAVIGATION;
    conf.startup_viewid = VID_NAVI_MAIN_VIEW;
    conf.res_h = 1920;
    conf.res_v = 720;
}

void SVP_AppFw_OnActivation(SVPAppFw_Activation activation, uint32_t param)
{
    SVP_INFO("SVP_AppFw_OnActivation() - 0x%08X, %u", activation, param);

    switch(activation)
    {
    case E_ACT_APP_FW_WS_READY:
        {
        }
        break;
    case E_ACT_APP_FW_DEINIT:
        {
        }
        break;
    case E_ACT_APP_FW_PRE_FG:  //界面准备显示
    case E_ACT_APP_FW_FG:      //界面正在显示
    case E_ACT_APP_FW_POST_FG: //界面己完成显示
        {
		}
		break;
	case E_ACT_APP_FW_BG:
		{
        }
        break;
    case E_ACT_APP_FW_MODE_UPDATE:
        {
        }
        break;
    }
}

bool SVP_AppFw_IsReady(SVPAppFw_ReadyCase ready_case)
{
    SVP_INFO("SVP_AppFw_IsReady() - %d", ready_case);

    return true;
}

SVPAppFw_View* SVP_AppFw_CreateView(int32_t viewid)
{
    SVP_INFO_FUNC();

    switch(viewid)
    {
    case VID_NAVI_MAIN_VIEW :
        return new SVPNaviView();
    default:
        return nullptr;
    }
}

void SVP_AppFw_DestroyView(SVPAppFw_View* view)
{
    SVP_INFO_FUNC();

    if (view)
        delete view;
}
