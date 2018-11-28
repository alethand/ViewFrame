#include "model_history.h"


namespace DataView{

Model_History::Model_History(DataManager *subject,bool openFilter)
    :DataView::Observer(subject,openFilter)
{

}

/*!
 * \brief RollRefresh::rowCount  滚动刷新--获取数据管理器的所有信息条数
 * \param parent
 * \return
 */
int Model_History::rowCount(const QModelIndex &parent) const
{
   return Observer::manager->getAllHistoryData()->count();
}

QVariant Model_History::getData(int row,int column) const
{
//    const ParsedResult * originInfo =
//        Observer::manager->getAllHistoryData()->at(row);

    for(int i=0;i < Observer::mNeedsDisplayData.count();i++)
        return QVariant();

}


}
