/*!
 *  @brief     任务控制配置文件
 *  @details   1.定义任务控制xml文件格式，描述了控件、窗口布局的关系； \n
 *             2.用户可对xml文件中的字段进行增减，实现协议动态化； \n
 *  @author    yww
 *  @version   1.0
 *  @date      2018.10.30
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef LAYOUT_BYXML_H
#define LAYOUT_BYXML_H

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QListWidget>

#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>

#include <QString>
#include <QList>
#include <QByteArray>

#include "Base/util/fileutils.h"
#include "head.h"

namespace TaskControlModel
{
class  TaskLayoutParse : public RXmlParseMethod
{
public:
     TaskLayoutParse();
     ~TaskLayoutParse();
     virtual bool  startParse(QDomNode & node);

     Container * getContainer(){return windowContainer;}

private:
    void parseContainer(QDomNode &node,Container *element);
    void parseLayout(QDomNode &node,Container *element);
    void parseItems(QDomNode &node,Container *element);
    void parseItem(QDomNode &node,Container *element);

    Field* parseAttributes(QDomNode &node);              //  解析特性
    void parseType(QDomNode &node,PubHead *element);       //解析类型
    void generateWidget(Container *container);

    Layout parseLayout(QString  layoutText);

    void releaseContainer(Container * container);

private:
    Container * windowContainer;    /*!< 保存解析xml文件后的 */
    int filedIndex;                 /*!< 字段类型索引,默认从0开始，每次累加1 */
};

}
#endif
