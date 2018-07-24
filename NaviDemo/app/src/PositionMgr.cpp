#include "PositionMgr.h"
#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG  "navi|gps"

CPositionMgr::CPositionMgr()
: m_pProxy(nullptr)
{
	Init();
}

CPositionMgr::~CPositionMgr()
{
}

bool CPositionMgr::Init()
{
    SVP_INFO_FUNC_BEGIN();
    if(m_pProxy == nullptr) {
    	m_pProxy = svp::servicefw::buildProxy<v1::svp::positioning::PositioningProxy>(SVP_POSITIONING_INSTANCE, SVP_POSITIONING_LIBBASE);
    }
    if(m_pProxy)
    {
        if (!svp::servicefw::waitProxy(m_pProxy, 3000))
        {
        	SVP_ERROR("waitProxy error!");
        	return false;
        }
    }
    else
    {
    	SVP_ERROR("m_pProxy is null");
        return false;
    }
    sat_location.setVisible_sats(0);
    m_pProxy->getSatPositionAttribute().getChangedEvent().subscribe([&](const PositioningTypes::tSatPosition & val) {
        sat_location = val;
        SVP_INFO("Location - status: %d, visible: %d, tracked %d, used %d, lat %f, lon %f, timestamp %d",
        		sat_location.getStatus(), sat_location.getVisible_sats(), sat_location.getTracked_sats(),
				sat_location.getUsed_sats(), sat_location.getLat(), sat_location.getLon(),
				sat_location.getTimestamp());
    });

    m_pProxy->getSatChannelsAttribute().getChangedEvent().subscribe([&](const PositioningTypes::tSatChannels & val) {
        sat_channels = val;
        for(auto & x : sat_channels) {
        	SVP_INFO("Channels -ID: %d,  Signal: %f", x.getPrn(), x.getSnr());
        }
    });

    m_pProxy->getDrPositionAttribute().getChangedEvent().subscribe([&](const PositioningTypes::tDrPosition &val) {
        sat_dr_position = val;
        SVP_INFO("DrPosition - lat %f, lon %f", sat_dr_position.getLat(), sat_dr_position.getLon());
    });

    m_pProxy->getSatTimeAttribute().getChangedEvent().subscribe([&](const PositioningTypes::tSatTime &val) {
        sat_time = val;
        SVP_INFO("SatTime %d-%d-%d %d:%d:%d", sat_time.getYear(), sat_time.getMonth(), sat_time.getDay(),
        		sat_time.getHour(), sat_time.getMinute(), sat_time.getSecond());
    });

    m_pProxy->getDrSensorDataAttribute().getChangedEvent().subscribe([&](const PositioningTypes::tDrSensorData &val) {
    	sat_dr_sensor = val;
        SVP_INFO("DrSensor Acc: x %f y %f z %f, Gyro: x %f y %f z %f, dr_cal %d",
        		sat_dr_sensor.getAcc_x(), sat_dr_sensor.getAcc_y(), sat_dr_sensor.getAcc_z(),
				sat_dr_sensor.getGyro_x(), sat_dr_sensor.getGyro_y(), sat_dr_sensor.getGyro_z(),
				sat_dr_sensor.getDr_cal());
    });
    SVP_INFO_FUNC_END();
}
