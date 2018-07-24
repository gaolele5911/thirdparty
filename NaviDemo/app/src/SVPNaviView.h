
#ifndef _SVP_NAVI_VIEW_H_
#define _SVP_NAVI_VIEW_H_

#include "appfw/gui/ui/SVPAppFw_GUI_View.h"
#include "ExtNavi.h"

class SVPNaviView : public SVPAppFw_GUI_View
{
public:
    SVPNaviView();

    virtual ~SVPNaviView();

protected:
    virtual int32_t onInitialize();

public:
   //virtual long OnMessage(int32_t msgid, int32_t wparam, long lpara);

   virtual void onEvent(int32_t evt_id, void** evt_data);

protected:
   //virtual int32_t onUpdateLanguage(uint32_t lang_id);

   // virtual long OnVKeyEvent(int32_t vkey_id, long vkey_value);

    // Self-update : Overload when needed
    virtual void onUpdate(uint32_t update_tick);

    // Self-render : Overload when needed
    virtual void onRender();

    static void onProcMsg(uint32_t evt_id, int argc, void *argv[], int *args, int fromID);
    static void* TestThread(void *arg);
    static void SatPositionFunc(const stSatPosition& val);
private:
    //家、公司、收藏列表、比例尺等级、地图模式，需在导航初始化完成时 & 状态有变化时，通知HUT
    void UpdateHomeIsFav(bool bIsFav);
    void UpdateOfficeIsFav(bool bIsFav);
    void UpdateFavList(const std::vector<std::string>& list);
    void UpdateZoomState(int value);
    void UpdateMapMode(int value);
    //收到语音指令：MSG_NAVASR_REMAINING_TIME 时，调用即可
    void UpdateRemainTime(int value);
    //收到语音指令：MSG_NAVASR_GET_MY_LOCATION 时， 调用即可
    void UpdateMyLocation(std::string province, std::string city, std::string district, std::string poiname);
    //更新道路类型：0 Unknown 1 HighWay 2 >70KPH 3 > 50KPH ...
    void UpdateCurrLoadType(int value);
    //更新导航限速: 0 No Display 1 SPL5 2 SPL10 3 SPL15 ...
    void UpdateSpeedLimit(int value);
    //更新导航限速状态: 0 不可用 1 可用 3 高速出入口
    void UpdateSpeedLimitStatus(int value);

};

#endif /* _SVP_NAVI_VIEW_H_ */
