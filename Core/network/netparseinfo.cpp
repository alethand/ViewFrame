#include "netparseinfo.h"
#include <QDataStream>

namespace NetParse{

DataGenertor *DataGenertor::instance = NULL;


BitData::BitData(Datastruct::Data_Bit *defination){
    mType = T_BitData;
    pDef = defination;
}

ByteData::ByteData(const Datastruct::Data_Word *defination){
    pDef = defination;
    mType=T_ByteData;
    mCtlType = Cntl_EMPTY;
}

ByteData::ByteData(int len, CtlOperate type)
{
    mlenth = len;
    pDef = NULL;
    mType=T_ByteData;
    mCtlType = type;
}

ByteData::~ByteData()
{
    pDef = NULL;
}

BitData::~BitData()
{
    pDef = NULL;
}


short BitData::getLenth()
{
    if(0 == mlenth)
        mlenth = pDef->last;
    return mlenth;
}

short ByteData::getLenth()
{
    if(0 == mlenth)
        mlenth = pDef->bytes;
    return mlenth;
}

DomainGroup::DomainGroup(){
    mType=T_DomainGroup;
    isAllBitData = -1;
}

DomainGroup::~DomainGroup()
{
    QList<Elem*>::iterator ite =mElems.begin();
    while(ite != mElems.end()){
        delete *ite;
        (*ite) = NULL;
        ite++;
    }
}

void DomainGroup::write(QDataStream &stream)
{
    switch(isAllBitData) {
    case -1:   handleBitDatas(Check,NULL);
               this->write(stream);
               break;
    case 1: handleBitDatas(Write,&stream);
            break;
    case 0:
        for(int i=0;i < mElems.count();i++) {
            mElems.at(i)->write(stream);
        }
        break;
    default:break;
    }
}

void DomainGroup::read(QDataStream &stream)
{
    switch(isAllBitData) {
    case -1:   handleBitDatas(Check,NULL);
               this->read(stream);
               break;
    case 1: handleBitDatas(Read,&stream);
            break;
    case 0:
        for(int i=0;i < mElems.count();i++) {
            mElems.at(i)->read(stream);
        }
        break;
    default:break;
    }
}

short DomainGroup::getLenth()
{
   if(0 == mlenth) {
        unsigned short bytesLen=0;
        unsigned short bitsLen=0;
        for(int i=0;i < mElems.count();i++) {
            if(mElems.at(i)->mType == T_BitData)
                bitsLen += mElems.at(i)->getLenth();
            else
                bytesLen += mElems.at(i)->getLenth();
        }
        Q_ASSERT(0 == bitsLen %8);
         mlenth = bytesLen + bitsLen/8;
   }
   return mlenth;
}

/*!
 * \brief DomainGroup::handleBitDatas 处理位数据
 * \param operate = Check；检查所有数据是否都是位数据
 *        operate = Write
 *
 * \param stream
 * \return
 */
bool DomainGroup::handleBitDatas(DomainGroup::BitOperate operate, QDataStream *stream)
{
    BitData *pLast =dynamic_cast<BitData*>(mElems.back() );
    switch(operate){
    case Check:
            for(int i=0;i < mElems.count();i++) {
                if(mElems.at(i)->mType == T_BitData)
                   i++;
                else {
                    isAllBitData = 0;
                    return false;
                }
            }
            isAllBitData = 1;
            break;
    case Write:if(isAllBitData == 1&&NULL != stream){
            int len = (pLast->pDef->startPos +pLast->pDef->last )/8;
             QVariant memory;
             for(int i=0;i < mElems.count();i++) {
                 mElems.at(i)->write(memory,len);
                 }
            switch(len){
            case 1: {quint8 temp;
                    temp = memory.toChar().cell();
                    (*stream)<<temp;
                    }
                 break;
            case 2:
            case 4:{ quint16 temp;
                    temp = memory.toUInt();
                    (*stream)<<temp;
                    }
                 break;
            default:break;
            }
        }
            break;
    case Read:if(isAllBitData == 1 && NULL != stream){
            int len = (pLast->pDef->startPos +pLast->pDef->last )/8;
            QVariant memory;
            switch(len){
            case 1: {quint8 temp;
                    (*stream)>>temp;
                    memory = QVariant(temp);
                    }
                break;
            case 2: {quint16 temp;
                    (*stream)>>temp;
                    memory = QVariant(temp);
                    }
                break;
            case 4: {quint32 temp;
                    (*stream)>>temp;
                    memory = QVariant(temp);
                    }
                break;
            default:break;
            }
            for(int i=0;i < mElems.count();i++) {
                mElems.at(i)->write(memory,len);
                }
        }

            break;
    default:break;

    }
    return true;
}

Protocol::~Protocol()
{
    QList<Elem*>::iterator ite =mElems.begin();
    while(ite != mElems.end()){
        delete *ite;
        (*ite) = NULL;
        ite++;
    }
}

void Protocol::write(QDataStream &stream)
{
    QList<Elem*>::iterator ite =mElems.begin();
    while(ite != mElems.end()){
         (*ite)->write(stream);
        ite++;
    }

}

void Protocol::read(QDataStream &stream)
{
    QList<Elem*>::iterator ite =mElems.begin();
    while(ite != mElems.end()){
         (*ite)->read(stream);
        ite++;
    }
}

short Protocol::getLenth()
{
    if(0 == mlenth) {
         unsigned short bytesLen=0;
         unsigned short bitsLen=0;
         for(int i=0;i < mElems.count();i++) {
             if(mElems.at(i)->mType == T_BitData)
                 bitsLen += mElems.at(i)->getLenth();
             else
                 bytesLen += mElems.at(i)->getLenth();
         }
         Q_ASSERT(0 == bitsLen %8);
          mlenth = bytesLen + bitsLen/8;
    }
     return mlenth;
}

void  ByteData::write(QDataStream &stream)
{
    switch(getLenth()) {
    case 1:stream<<value.toChar();
          break;
    case 2:if(pDef != NULL && pDef->isSigned){
               qint16 temp;
               temp = value.toInt();
               stream<<temp;
           }
           else{
                quint16 temp;
                temp = value.toUInt();
                stream<<temp;
           }
            break;
    case 4:if(pDef != NULL) {
               if(pDef->type == Datastruct::ValueIntEdit){
                    if(pDef->isSigned){
                        qint32 temp;
                        temp = value.toInt();
                        stream<<temp;
                    }
                    else{
                        quint32 temp;
                        temp = value.toUInt();
                        stream<<temp;
                    }
                }
                else { //浮点型数据
                    float temp;
                    temp = value.toFloat();
                    stream<<temp;
                }
            }
            else {
                quint32 temp;
                temp = value.toUInt();
                stream<<temp;
            }
            break;
    case 8:if(pDef != NULL ){
               if(pDef->type == Datastruct::ValueIntEdit){
                    if(pDef->isSigned){
                        qint64 temp;
                        temp = value.toLongLong();
                        stream<<temp;
                    }
                    else{
                        quint64 temp;
                        temp = value.toULongLong();
                        stream<<temp;
                    }
                }
                else { //浮点型数据
                    double temp;
                    temp = value.toDouble();
                    stream<<temp;
                }
            }
            else{
                quint64 temp;
                temp = value.toULongLong();
                stream<<temp;
            }
        break;
    default:break;
    }
}

void  ByteData::read(QDataStream &stream)
{
    switch(getLenth()) {
    case 1: quint8 temp;
            stream>>temp;
            value = QVariant(temp);
          break;
    case 2:if(pDef != NULL && pDef->isSigned){
               qint16 temp;
               stream>>temp;
               value = QVariant(temp);
           }
           else{
               quint16 temp;
               stream>>temp;
               value = QVariant(temp);
           }
            break;
    case 4:if(pDef != NULL) {
                if(pDef->type == Datastruct::ValueIntEdit){
                    if(pDef->isSigned){
                        qint32 temp;
                        stream>>temp;
                        value = QVariant(temp);
                    }
                    else{
                        quint32 temp;
                        stream>>temp;
                        value = QVariant(temp);
                    }
                }
                else { //浮点型数据
                    float temp =0;
                    stream>>temp;
                    value = QVariant(temp);
                }
            }
            else{
                quint32 temp;
                stream>>temp;
                value = QVariant(temp);
            }
            break;
    case 8:if(pDef != NULL) {
                if(pDef->type == Datastruct::ValueIntEdit){
                    if(pDef->isSigned){
                        qint64 temp;
                        stream>>temp;
                        value = QVariant(temp);
                    }
                    else{
                        quint64 temp;
                        stream>>temp;
                        value = QVariant(temp);
                    }
                }
                else { //浮点型数据
                    double temp =0;
                    stream>>temp;
                    value = QVariant(temp);
                }
            }
            else{
                quint64 temp;
                stream>>temp;
                value = QVariant(temp);
            }
            break;
    default:break;
    }
}

QString ByteData::getValue(bool isFilter)
{
    QString str = value.toString();
    if(pDef !=NULL&&!pDef->name.isEmpty() ){
         str =  QString::number(value.toInt()*pDef->precision);
        if(isFilter &&!pDef->visible)//过滤但不可见
            return QString();
    }
    else
        return QString();

    return str;
}

void BitData::write(QVariant &data,int len)
{
    char temp = value;
    if(pDef != NULL ) {
        switch(pDef->last){
        case 1: temp = temp&0x01;
                break;
        case 2: temp = temp&0x03;
                break;
        case 3: temp = temp&0x07;
                break;
        case 4: temp = temp&0x0F;
                break;
        case 8: temp = temp&0xFF;
                break;
        }
    }

    switch(len) {
    case 1:{
            char memory = data.toChar().cell();
            if(pDef != NULL)
                memory |=(temp<<pDef->startPos);

            data = QVariant(memory);
            }
             break;
    case 2:
    case 4:{
            quint16 memory = data.toUInt();
            memory |=(temp<<pDef->startPos);
            data = QVariant(memory);
            }
            break;
    default:break;
    }
}

void BitData::read(QVariant &data,int len)
{

    char temp = 0x00;
    switch(len) {
    case 1:temp = data.toChar().cell();
           break;
    case 2:
    case 4:temp = data.toUInt()>>((pDef->startPos/8)*8);
           break;

    }
    if(pDef != NULL) {
        int startPos = pDef->startPos%8;    //位数据默认在一个字节之中，所有只取该取的数据
        temp = temp>>startPos;
        switch(pDef->last){
        case 1: temp = temp&0x01;
                break;
        case 2: temp = temp&0x03;
                break;
        case 3: temp = temp&0x07;
                break;
        case 4: temp = temp&0x0F;
                break;
        case 8: temp = temp&0xFF;
                break;
        }
    }
    value  = temp;
}

QString BitData::getValue(bool isFilter)
{
    QString str(value);
    if(pDef != NULL){
        if(!pDef->list.isEmpty())
            str = pDef->list.at((int)value);
        else
            return QString();

        if(isFilter && !pDef->visible)  //过滤但不可见
            return QString();

    }
    else
        return QString();

    return str;

}

QSharedPointer<Protocol> DataGenertor::startParse(ProtocolArray &array,Datastruct::BaseProtocol *protocol)
{
    if(NULL == instance)
        instance = new DataGenertor();

    QDataStream stream(array.data);
    QSharedPointer<Protocol> newData;
    ByteData *pByte = NULL;
    DomainGroup *pDomain = NULL;
    if(protocol->startLen > 0){
        if(protocol->startLen <=8) {
            pByte = new ByteData(protocol->startLen,Cntl_EMPTY);
            pByte->read(stream);
            newData.data()->mElems.append(pByte);
        }
        else{
            pDomain = new DomainGroup();
            for(int i=0;i < protocol->startLen;i++) {
                pDomain->mElems.append(new BitData());
                pDomain->mElems.back()->read(stream);
            }
            newData.data()->mElems.append(pDomain);
        }

    }else{
        return QSharedPointer<Protocol>();
    }

    int cycleTimes = 0;
    cycleTimes = instance->getRepeatTimes(stream,protocol,newData.data());
    for(int i=0;i < cycleTimes;i++) {
        pDomain = new DomainGroup();
        instance->parsedDomainGroup(stream,protocol,pDomain);
        newData.data()->mElems.append(pDomain);
    }

    return newData;
}


bool DataGenertor::parsedSignalProtocol(const QDataStream &stream, SingleProtocol *protocol, Protocol *result)
{
    //单层协议循环次数默认为1
        for(int j=0;j<protocol->fields.size();j++)//单层协议字段个数
        {
            Data_Word *fieldData =const_cast<Data_Word*>(&(protocol->fields.at(j)));

            //1.根据字段中repeat节点来获取多个值
            int repeatTime = 0;
            if(fieldData->repeat <= 1){
                repeatTime = 1;
            }else{
                repeatTime = fieldData->repeat;
            }
            for(int k = 0;k<repeatTime;k++){
                //2`.判断是为位操作还是字节操作
                if(fieldData->bitOperator){
                   // Datastruct::FieldValues values = getBitsData(array, fieldData.type, posPointer,fieldData.bytes,isError);
                }else{
                    if(fieldData->bytes <=8) {
                        ByteData *pByte = new ByteData(fieldData);
                        pByte->read(const_cast<QDataStream&>(stream));
                        result->mElems.append(pByte);
                    }
                    else{
                        DomainGroup *pDomain = new DomainGroup();
                        for(int i=0;i < fieldData->bytes;i++) {
                            pDomain->mElems.append(new BitData());
                            pDomain->mElems.back()->read(const_cast<QDataStream&>(stream));
                        }
                        result->mElems.append(pDomain);
                    }
                }
            }
       }
    return true;
}

bool DataGenertor::parsedDomainGroup(const QDataStream &stream, BaseProtocol *protocol, DomainGroup *result)
{
    for(int j = 0; j < protocol->contents.size();j++){//协议循环体中单层协议个数
        Datastruct::SingleProtocol signalProtcol = protocol->contents.at(j);
        if(signalProtcol.isProtocol) {
            int cycleTimes = 0;
            cycleTimes = getRepeatTimes(stream,protocol,result);
            for(int i=0;i < cycleTimes;i++) {
                Protocol * innerResult = new Protocol();
                if(!parsedSignalProtocol(stream,&signalProtcol,innerResult)){
                    delete innerResult;
                    return false;
                }
                result->mElems.append(innerResult);
            }
        }
        else {//解析单个字段
            ByteData *pByte = new ByteData(&(protocol->contents.at(j).fields.first()));
            pByte->read(const_cast<QDataStream&>(stream));
            result->mElems.append(pByte);
        }

    }
    return true;
}

int DataGenertor::getRepeatTimes(const QDataStream &stream, SingleProtocol *protocol,Elem *result)
{
    //确定循环次数
    ByteData *pByte = NULL;
    int cycleTimes =1;
    if(protocol->memoryBytes ==2 ||protocol->memoryBytes ==4) {
        pByte = new ByteData(protocol->memoryBytes,Cntl_COUNT);
        pByte->read(const_cast <QDataStream&>(stream));
        switch(result->mType) {
        case T_Protocol:
                dynamic_cast<Protocol*>(result)->mElems.append(pByte);
                break;
        case T_DomainGroup:
                dynamic_cast<DomainGroup*>(result)->mElems.append(pByte);
                break;
        default:break;
        }
        cycleTimes = pByte->value.toInt();
    }
    else if(protocol->count > 0){
         cycleTimes = protocol->count;
    }
    return cycleTimes;
}

int DataGenertor::getRepeatTimes(const QDataStream &stream, BaseProtocol *protocol, Elem *result)
{
    //确定循环次数
    ByteData *pByte = NULL;
    int cycleTimes =1;
    if(protocol->memoryBytes ==2 ||protocol->memoryBytes ==4) {
        pByte = new ByteData(protocol->memoryBytes,Cntl_COUNT);
        pByte->read(const_cast <QDataStream&>(stream));
        switch(result->mType) {
        case T_Protocol:
                dynamic_cast<Protocol*>(result)->mElems.append(pByte);
                break;
        case T_DomainGroup:
                dynamic_cast<DomainGroup*>(result)->mElems.append(pByte);
                break;
        default:break;
        }
        cycleTimes = pByte->value.toInt();
    }
    else if(protocol->count > 0){
         cycleTimes = protocol->count;
    }
    return cycleTimes;
}

/*!
 * \brief getData_InPos 获取协议中指定计数值的数据
 * \param protocol
 * \param count
 * \return
 */
Elem *getData_InPos(Protocol *protocol,int curCnt,int allCnt)
{
    QList<Elem*>::iterator ite = protocol->mElems.begin();
    while(ite != protocol->mElems.end()){
        switch((*ite)->mType){
        case T_Protocol:{
             Elem* data = getData_InPos(dynamic_cast<Protocol*>(*ite),curCnt,allCnt);
             if(data != NULL)
                 return data;
             else
                 break;
            }
            break;
        case T_DomainGroup:{
            Elem* data = getData_InPos(dynamic_cast<Protocol*>(*ite),curCnt,allCnt);
                if(data != NULL)
                    return data;
                else
                    break;
               }
               break;
        case T_ByteData:
                        if(! (dynamic_cast<ByteData*>(*ite)->getValue().isEmpty()))
                            curCnt++;
                        if(curCnt == allCnt)
                            return *ite;
            break;
        case T_BitData:
                        if(! (dynamic_cast<BitData*>(*ite)->getValue().isEmpty()))
                            curCnt++;
                        if(curCnt == allCnt)
                            return *ite;
            break;
        default:break;
        }

        ite++;
    }
    return NULL;
}










}
