/*!
 *  @brief     通过xml文件产生静态布局
 *  @details
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STATICLAYOUTBYXML_H
#define STATICLAYOUTBYXML_H

#include "layoutset.h"

class StaticLayoutByXml : public LayoutAndDisplay
{
    Q_OBJECT
public:
      enum XmlNetCompareResult
      {
        XNCRDataContentError,                      //数据不同
        XNCROnlyWorkStateDif,                      //仅工作状态不同
        XNCROlySubMMapDif,                         //仅分机的映射不同，组件的排列相同
        XNCNOnlyComponentDif,                      //仅组件的排列不同
        XNCRNeedMAp,                              //大量的数据位置不同，需要映射操作
        XNCRCompleteSame,                          //完全相同
      };

      StaticLayoutByXml(QWidget* mparent);
      virtual ~StaticLayoutByXml();

      virtual bool startLayout(HealthData &data,bool ifUse);
      virtual void flushScreen(HealthData &data);

      virtual int findMatchWKIndex(int index);
      virtual QPair<int,int> findMatchSubMIndexPair(int subMIndex,int compIndex);

private:
      bool readXMLFile();
      void compareStringOrder(HealthData &data);

      XmlNetCompareResult mNetXmlResult;
      QMap<int,int> mMapWorkState;               ///工作状态映射表   统一为网络数据位置--xml位置
      QMap<int,int> mMapSubMachine;             ///分机映射表
      QList<QMap<int,int>> mMapComPonet;        ///n个组件映射表

      ParseXML_HealthStateLayout *mxmlData;
};

#endif // STATICLAYOUTBYXML_H
