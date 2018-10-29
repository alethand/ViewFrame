#ifndef TASKCNTLDISPLAY_PARSEXML_H
#define TASKCNTLDISPLAY_PARSEXML_H

#include "Base/util/fileutils.h"
/************界面显示***************/
struct TaskDisplay
{
  struct Type
  {
    bool ifNeedShow;        /*!< 是否显示*/
    short Column;           /*!< 显示列数*/
    double widthHint;       /*!< 默认宽度*/
    double heightHint;      /*!< 默认高度*/
  };

  Type bandCntl;           //频段控制
  Type stateCntl;          //状态控制
  Type gatherCntl;         //采集控制
  Type instrumentCntl;     //仪器控制
  Type selfCheckCntl;      //自检控制
  Type turntableCntl;      //转台控制
  Type playBackCntl;       //回放控制
};


/*!
 * \brief The TaskCntlDisplay_ParseXML class 任务控制显示 - 解析xml
 */
class  TaskCntlDisplay_ParseXML : public RXmlParseMethod
{
public:
    TaskCntlDisplay_ParseXML();
    //virtual ~TaskCntlDisplay_ParseXML();

    virtual bool  startParse(QDomNode & node);

    TaskDisplay data;

private:
    void parseCntlBranch(QDomElement & node,TaskDisplay::Type &content);
;
};
#endif // TASKCNTLDISPLAY_PARSEXML_H
