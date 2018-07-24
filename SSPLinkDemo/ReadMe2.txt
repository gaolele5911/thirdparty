1、相关头文件: ExtSspLink.h  KeySoundInterface.h  SVExtDef.h
   放置路径: /dist/release/svp/include/rn2_ext
2、相关库文件: libextssplink.so
   放置路径: /dist/release/svp/lib/rn2/app
3、在/storage/app/rn2.g5r2.ssplink/bin 目录下, 有链接文件ssplink, 指向SspLink可执行文件真实位置
4、SspLink App的启动:
   . /svp/etc/svp.env.setup.sh
   svp.util.app-run 8x default 1
   8x 待定

5、部分代码说明：
=================================================================
bool SVP_AppFw_IsReady(SVPAppFw_ReadyCase ready_case)
保持Demo状态，无须处理，直接return true;即可。

SVPAppFw_View* SVP_AppFw_CreateView(int32_t viewid)
保持Demo状态，无须处理

void SVP_AppFw_DestroyView(SVPAppFw_View* view)
保持Demo状态，无须处理

SVP_INFO_FUNC();
这一个宏，打印函数名的Log，无特别含义
SVP_INFO("%s()", __FUNCTION__);

void SVP_AppFw_OnActivation(SVPAppFw_Activation activation, uint32_t param)
在App不同的状态，这个函数会被调用，关注以下状态，其他状态无须关注。
如果此函数被阻塞，会导致整个机器界面短暂无法切换，请勿在此函数处理耗时操作
    E_ACT_APP_FW_PRE_FG  //App界面准备被切换至前台
    E_ACT_APP_FW_FG,     //App正在切换至前台
    E_ACT_APP_FW_POST_FG,//App己完成至前台

    E_ACT_APP_FW_PRE_BG,  //App界面准备被切换至后台
    E_ACT_APP_FW_BG,      //App正在切换至后台
    E_ACT_APP_FW_POST_BG, //App己完成至后台
=================================================================

6、Log机制说明
为了更好区分不同App打印的Log及排查问题，请在每个有需要打印Log的文件头，重定义LOG_TAG。
不同的cpp，使用不同的SVP_LOG_TAG，打印的Log会附带该标记。

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG         "vpanel|view"）
打印的Log如下，其中544是进程号，0xB1C9F450是线程号，[vpanel|view]是SVP_LOG_TAG, INFO: 是Log级别
(544|0xB1C9F450) [vpanel|view] INFO: MMICore_PostEvent:EvHVAC_UpdateRearAirView.
(544|0xB1C9F450) [vpanel|view] INFO: notifyHavcAirCleanerView
(544|0xB1C9F450) [vpanel|view] INFO: MMICore_PostEvent:EvHVAC_UpdateAirCleanerView.

普通Log使用SVP_INFO输出，示例如下:
SVP_INFO("SVP_AppFw_IsReady() - %d", ready_case);

警告Log使用SVP_WARN输出，示例如下:
SVP_WARN("Proxy NULL!");

错误Log使用SVP_ERROR输出，示例如下:
SVP_ERROR("Open /logo_os_queue fail!");

详细可查看SVPLog.h, 其中有定义, 代码相对简单.

7、manifest.xml
软件升级时，安装程序会读取此文件，形成ID、路径、安装包的对应关系，并存入App数据库。
这样使用以下指令，只需要指定ID，即可启动到对应路径的可执行文件。
svp.util.app-run 80 default 1
这个文件，供应商无须关注，Desay会配置好路径及ID。

8、pkg.sh
我们会利用这个脚本，将程序打包成升级包。
供应商无须关注，在软件调试时，可直接使用文件替换的方式进行调试。
如果HUT有大的变动时，我们会发完整的升级包，供应商作升级后再替换供应商自己的可执行程序。

9、SVPAppFw_Config赋值
以下参数请保持Demo状态不变，无须关注。Desay会统一分配App不同ID。
conf.appfw = "appfw-gui";
conf.uifw = "uifw-qt";  //指定是Qt还是OpenGL方案
conf.appid = SVPApp::APPID_NAVIGATION;
conf.startup_viewid = VID_NAVI_MAIN_VIEW;


10、viewID，如VID_NAVI_MAIN_VIEW
无须关注，保持Demo状态不变; 我们目前也只使用到一个View。

11、SVPAppFw_Activation及其他必须了解的功能
参见第5点，应该只有切换至前台或后台会被用到。

12、通讯协议及其他逻辑
待定，我方会提供.so库，东软可直接调用获取信息


13、字体文件请使用
/svp/share/font/ 字体

