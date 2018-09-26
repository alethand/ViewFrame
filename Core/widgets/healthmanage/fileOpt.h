/*!
 *  @brief     与健康管理相关的文件操作
 *  @details   xls：开机后对数据进行保存到xls文件中
 *             xml：从特定的xml文件中读取布局内容
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef FILEOPT_H
#define FILEOPT_H

#include "Base/util/fileutils.h"
#include "Base/util/xlsfilesaver.h"
#include "layoutset.h"
#include "healthstate_display.h"

namespace HealthState {
struct HealthData;
}
using namespace HealthState;
using HealthState::HealthData;

using namespace FileUtils;

//16进制转Rgb再转颜色

namespace HealthState {

int HexToOct(QString s);
QColor transformFrHexToRGB(QString strColor);

/*!
 * \brief 解析健康管理的xml文件
 */
class  ParseXML_HealthStateLayout:public ParseXMLMethod
{
public:
    ParseXML_HealthStateLayout(ParseXMLMethod *parent=0);
    virtual ~ParseXML_HealthStateLayout();


    HealthData  * mHealthData;       //这是狗屎编译器的问题，强制我使用指针

protected:
     virtual bool  concreteParse(QDomNode &node);

private:
    bool handleWorkStateInfo(QDomElement &node);        //处理工作状态信息
    bool handleSubMachineInfo(QDomElement &node);       //处理分机组件信息
};


/*!
 * \brief 对自定义的数据--健康管理状态的xls文件的生成方式
 * \attention  为了分别工作状态下的数据与颜色，设置为数值【颜色格式】
 */

class XlsHealthState:public TableXLSProxery
{
public:
    XlsHealthState(bool isVisible = false);
     void addData(HealthData &data);

protected:
     virtual void run();

private:
     void initXlsFirstLine();
     void writeData();

private:
     HealthData * mData;
     bool bisFirstReceive;
};

}

#endif // FILEOPT_H
