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

//布局
const char LAY_SETTING[] = "Layout";
const char LAY_GEOMETRY[] = "geometry";
const char LAY_WINDOWSTATE[] = "windowState";


/*************************************************</配置文件>******************************************************/


/*************************************************<插件信息>******************************************************/
/*
 * 插件命名按照【<PLUGIN>.<插件所属模块>.<插件名>】
 * 设置插件所属模块便于系统将同一模块下的插件进行整合处理
*/
const char PLUGIN_CORE[] = "Plugin.Core.Core";
const char PLUGIN_TASK_CONTROL[] = "Plugin.TaskControl.TaskControl";
const char PLUGIN_HEALTH_MANAGER[] = "Plugin.HealthManager.HealthManager";
const char PLUGIN_RADIATION_SOURCE_TABLE[] = "Plugin.DataDisplay.RadiationSourceTable";
const char PLUGIN_ALL_PLUS_TABLE[] = "Plugin.DataDisplay.AllPluseTable";
const char PLUGIN_MF_ACQUISTION_TABLE[] = "Plugin.DataDisplay.MFAcquistionTable";
const char PLUGIN_RADIA_SOURCE_MAP[] = "Plugin.DataDisplay.RadiaSourceMap";
const char PLUGIN_ALL_PLUS_GRAPHICS[] = "Plugin.DataDisplay.AllPluseGraphics";
const char PLUGIN_MF_QUISITION_GRAPHICS[] = "Plugin.DataDisplay.MFAcquisitionGraphics";
const char PLUGIN_SPECTRUM_GRAPHICS[] = "Plugin.DataDisplay.SpectrumGraphics";

/*************************************************</插件信息>******************************************************/

/*************************************************<工具栏>******************************************************/
//菜单栏
const char MENU_BAR[] = "MenuBar";

/*
 * 工具栏命名按照【<Menu>.<Group>.<工具栏名>】
*/
const char MENU_PROGRAM[]  = "Menu.Group.Program";
const char MENU_VIEW[]  = "Menu.Group.View";
const char MENU_SETTING[]  = "Menu.Group.Setting";
const char MENU_HELP[]     = "Menu.Group.Help";
const char DEFAULT_GROUP[] = "Menu.Default.Group";

/*
 * 菜单栏中action的Id命名按照【<Action>.<插件模块名>.<工具名>】
 * 请务必遵守，否则会影响快捷键(ShortcutSettings)的处理
*/

//MENU_PROGRAM 分组
const char PROGRAM_EXIT[] = "Action.Core.Exit";

//MENU_VIEW 分组
const char VIEW_MANAGER[] = "Action.Core.ViewManager";
const char IMPORT_VIEW[] = "Action.Core.ImportView";
const char EXPORT_VIEW[] = "Action.Core.ExportView";

//MENU_SETTING 分组
const char TOP_HINT[] = "Action.Core.TopHint";
const char FULL_SCREEN[] = "Action.Core.FullScreen";
const char CUSTOM_STYLE[] = "Action.Core.CustomStyle";
const char SYSTEM_LANGUAGE[] = "Action.Core.SystemLanguage";
const char SYSTEM_SHORTCUT[] = "Action.Core.ShortCut";
const char SYSTEM_ScreenShot[] = "Action.Core.ScreenShot";

//MENU_HELP 分组
const char TEC_SUPPORT[] = "Action.Core.TecSupport";
const char ABOUT_PROGRAM[] = "Action.Core.About";

/*************************************************</工具栏>******************************************************/

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
