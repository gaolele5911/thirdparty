#ifndef __POSITIONMGR_H__
#define __POSITIONMGR_H__
#include "SVPType.h"
#include "SVPSingleton.h"
#include <CommonAPI/CommonAPI.hpp>
#include <v1/svp/positioning/PositioningProxy.hpp>
#include <SVPServiceHelper.h>

using namespace v1::svp::positioning;

class CPositionMgr
{
    SVP_SINGLETON_CLASS(CPositionMgr)
public:
    void SendCommand(int argc, char* argv[]);
protected:
    CPositionMgr();
    ~CPositionMgr();
private:
    bool Init();
    std::shared_ptr<v1::svp::positioning::PositioningProxy<> > m_pProxy;
    CommonAPI::CallStatus           m_callStatus = CommonAPI::CallStatus::SUCCESS;

    PositioningTypes::tSatPosition  sat_location;
    PositioningTypes::tSatChannels  sat_channels;
    PositioningTypes::tDrPosition   sat_dr_position;
    PositioningTypes::tSatTime      sat_time;
    PositioningTypes::tDrSensorData sat_dr_sensor;
};
#endif
