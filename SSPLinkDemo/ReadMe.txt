SSPLink_Demo包含2个工程，
1. hmi是qml代码的工程，用于生成界面HMI相关的so库，libSSPLinkHMI.so
2. ssplink是在appfw（德赛内部框架）的基础上，集成libSSPLinkHMI.so，生成可执行文件。
ssplink仅仅是入口，启动方式是svp.util.app-run 72 default 1，  所有的逻辑代码都在hmi中
入口代码在SVPNaviView.cpp构造函数中
LauncherService::init(m_qmlAppEngine);   //调用qml工程中对应的c++类初始化，即qmlRegisterType(xxx)
m_qmlAppEngine.loadData(SVP_NAVI_QML); //加载qml页面数据   即"import SSPLink 1.0; Launcher { }"
目前给SSPLink分配的ID是    SURFACE_ID = 3001   APP_ID = 72                        // APPID_LINK
在ssplink代码SVPNaviApp.cpp中也要做对应的设置，如下：
void SVP_AppFw_Config(SVPAppFw_Config& conf)
{
    //...
    conf.startup_viewid = VID_SSPLINK_VIEW;    //Here
}

图层共有7层，SSPLink在3层（surface_id: 3xxx）, 车机的全局弹框，音量条都会在上面，可以遮盖ssplink
根据德赛的框架现状，建议SSPLink所有的逻辑代码都封装在so中，集成到一个进程ssplink中，内部的图层（镜像，截图，多个模块）
等逻辑都在内部调度。



编译方法：

1. qml库编译方法： 
cd hmi
qmake
make
生成的so库在SSPLink_Demo/Build/dist/arm/下面

2. ssplink 可执行文件的编译方法
cd ssplink/build/release
$CMAKE ../../
make
生成的bin文件在ssplink/build/release下面