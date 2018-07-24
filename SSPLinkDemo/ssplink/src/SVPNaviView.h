#ifndef _SVP_NAVI_VIEW_H_
#define _SVP_NAVI_VIEW_H_

#include <QtQml/QQmlApplicationEngine>

#include "appfw/gui/ui/SVPAppFw_GUI_View.h"

class SVPNaviView : public SVPAppFw_GUI_View
{
public:
    SVPNaviView();
    virtual ~SVPNaviView();

protected:
    virtual int32_t onInitialize();
    static void onProcMsg(uint32_t evt_id, int argc, void *argv[], int *args, int fromID);
    static void* TestThread(void *arg);

private:
    QQmlApplicationEngine m_qmlAppEngine;
};

#endif /* _SVP_NAVI_VIEW_H_ */
