/*!
 *  @brief     常量汇总
 *  @details   定义了软件运行时所需的常量信息，包括(图片、控件ID等)
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constant {

/*日期*/
const char Date_Simple[] = "yyyy-MM-dd";

/*文件路径*/
const char AppShortName[] = "Rim";
const char ApplicationName[] = "ViewFrame";
const char Version[] = "1.1.0";

/*************************************************<配置文件>******************************************************/

/*系统设置*/
const char SYSTEM_SETTING[] = "System";
const char SYSTEM_DEFAULT_SCHEMES[] = "defaultSchemes";
const char SYSTEM_USER_SCHEMES_NAME[] = "userSchemesName";
const char SYSTEM_LOCALE[] = "locale";
const char SYSTEM_STYLE[] = "stylesheet";

//日志模块
const char LOG_LOG[] = "log";
const char LOG_LOG_RECORDFILE[] = "log2File";
const char LOG_LOG_LEVEL[] = "logLevel";

//数据库模块
const char DB_SETTING[] = "Database";
const char DB_TYPE[] = "Dbtype";
const char DB_HOST[] = "Hostname";
const char DB_DATABASE_NAME[] = "DatabaseName";
const char DB_USERNAME[] = "UserName";
const char DB_PASSWORD[] = "Password";
const char DB_PORT[] = "Port";

/*************************************************</配置文件>******************************************************/


const char SELECTED_LINE_COLOR[] = "rgb(34, 175, 75)";              /*!< 选中行的背景颜色 */

//插件
const char PLUGIN_TASK_CONTROL[] = "Plugin.TaskControl";
const char PLUGIN_HEALTH_MANAGER[] = "Plugin.HealthManager";
const char PLUGIN_DATA_DISPLAY[] = "Plugin.DataDisplay";
const char PLUGIN_DATA_VIEW[] = "Plugin.DataView";
//数据显示模块插件集合
const char PLUGIN_RADIATION_SOURCE_TABLE[] = "Plugin.DataDisplay.RadiationSourceTable";
const char PLUGIN_ALL_PLUS_TABLE[] = "Plugin.DataDisplay.AllPluseTable";
const char PLUGIN_MF_ACQUISTION_TABLE[] = "Plugin.DataDisplay.MFAcquistionTable";
const char PLUGIN_RADIA_SOURCE_MAP[] = "Plugin.DataDisplay.RadiaSourceMap";
const char PLUGIN_ALL_PLUS_GRAPHICS[] = "Plugin.DataDisplay.AllPluseGraphics";
const char PLUGIN_MF_QUISITION_GRAPHICS[] = "Plugin.DataDisplay.MFAcquisitionGraphics";
const char PLUGIN_SPECTRUM_GRAPHICS[] = "Plugin.DataDisplay.SpectrumGraphics";

//菜单栏
const char MENU_BAR[] = "MenuBar";

//菜单栏分组
const char G_PROGRAM[]  = "Group.Program";
const char G_SETTING[]  = "Group.Setting";
const char DATA_VIEW[] = "Setting.DataView";
const char G_HELP[]     = "Group.Help";
const char DEFAULT_GROUP[] = "Default.Group";

const char RADIASOURCETABLE_SETTING[] = "Setting.RadiaSourceTableSetting";        //数据源列表信息
const char ALLPLUSETABLE_SETTING[] = "Setting.AllPluseTableSetting";              //全脉冲列表信息
const char MFACQUISITIONTABLE_SETTING[] = "Setting.MFAcquisitionTableSetting";    //中频数据列表信息
const char RADIASOURCEMAP_SETTING[] = "Setting.RadiaSourceMapSetting";            //数据源地图信息
const char ALLPLUSEGRAPHICS_SETTING[] = "Setting.AllPluseGraphicsSetting";        //全脉冲图形信息
const char MFACQUISITIONGRAPHICS_SETTING[] = "Setting.MFAcquisitionGraphicsSetting";//中频数据图形信息
const char SPECTRUMGRAPHICS_SETTING[] = "Setting.SpectrumGraphicsSetting";        //频谱数据图形信息

/*!<
菜单栏中action的Id命名由两部分组成,格式如“A.B”，
其中A代表插件名，B代表Action名。
请务必遵守，否则会影响快捷键(ShortcutSettings)的处理
*/

//G_PROGRAM 分组
const char PROGRAM_EXIT[] = "Core.Exit";

//Setting 分组
const char TOP_HINT[] = "Core.TopHint";
const char FULL_SCREEN[] = "Core.FullScreen";
const char CUSTOM_STYLE[] = "Core.CustomStyle";
const char SYSTEM_LANGUAGE[] = "Core.SystemLanguage";
const char SYSTEM_SHORTCUT[] = "Core.ShortCut";

//G_HELP 分组
const char TEC_SUPPORT[] = "Core.TecSupport";
const char ABOUT_PROGRAM[] = "Core.About";

//右键菜单
const char MENU_ROUTE_TABLE_SERVER[] = "Menu.Route.Table.Server";             //路由表中服务器右键菜单

const char ACTION_ROUTE_SERVER_NEW[] = "Action_Route_Server_New";             //新建分组
const char ACTION_ROUTE_SERVER_DELETE[] = "Action_Route_Server_Delete";       //删除分组

const char MENU_ROUTE_TABLE_CLIENT[] = "Menu.Route.Table.Client";             //路由表中客户端右键菜单

const char ACTION_ROUTE_CLIENT_NEW[] = "Action_Route_Client_New";             //新建分组
const char ACTION_ROUTE_CLIENT_MOVE[] = "Action_Route_Client_Move";           //移动分组
const char ACTION_ROUTE_CLIENT_DELETE[] = "Action_Route_Clinet_Delete";       //删除分组

}

#endif // CONSTANTS_H
