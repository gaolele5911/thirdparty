#include "appfw/ISVPAppFw.h"
#include "SVPNaviView.h"
#include "app/SVPAppID.h"
#include "SVPLog.h"

#ifdef SVP_LOG_TAG
#undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG         "ssplink"

#define VID_SSPLINK_VIEW       0

void SVP_AppFw_Config(SVPAppFw_Config& conf)
{
    SVP_INFO_FUNC();

    conf.appfw = "appfw-gui";
    conf.uifw = "uifw-qt";
    conf.appid = SVPApp::APPID_LINK;
    conf.startup_viewid = VID_SSPLINK_VIEW;
}

void SVP_AppFw_OnActivation(SVPAppFw_Activation activation, uint32_t param)
{
    SVP_INFO("SVP_AppFw_OnActivation() - 0x%08X, %u", activation, param);
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
    case VID_SSPLINK_VIEW :
        return new SVPNaviView();
    default:
        return nullptr;
    }
}

void SVP_AppFw_DestroyView(SVPAppFw_View* view)
{
    SVP_INFO_FUNC();

    if (view)
    {
        delete view;
    }
}
