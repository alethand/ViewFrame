#include "taskcntldisplay_parsexml.h"
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QDebug>

TaskCntlDisplay_ParseXML::TaskCntlDisplay_ParseXML()
{

}

bool TaskCntlDisplay_ParseXML::startParse(QDomNode &node)
{
    QDomElement root = node.toElement();
    if(root.tagName() != "TaskControl")
    {
        qDebug()<<"TaskCntlDisplay_ParseXML::head's tag is Error";
        return false;
    }
    node = root.firstChild();
    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        if(element.tagName() == "BandCntl")
            parseCntlBranch(element,data.bandCntl);

        else  if(element.tagName() == "StateCntl")
              parseCntlBranch(element,data.stateCntl);



        else if(element.tagName() == "GatheCntl")
            parseCntlBranch(element,data.gatherCntl);


        else if(element.tagName() == "InstrumentCntl")
            parseCntlBranch(element,data.instrumentCntl);


        else if(element.tagName() == "SelfCheckCntl")
            parseCntlBranch(element,data.selfCheckCntl);


        else if(element.tagName() == "TurnTableCntl")
            parseCntlBranch(element,data.turntableCntl);


        else if(element.tagName() == "PlayBackCntl")
             parseCntlBranch(element,data.playBackCntl);

         node = node.nextSibling();

    }
}

void TaskCntlDisplay_ParseXML::parseCntlBranch(QDomElement &node,TaskDisplay::Type &content)
{
    QDomNodeList  settings = node.childNodes();
    QDomElement *element = NULL;
    for(int i=0;i < settings.count();i++)
    {
        element = &(settings.at(i).toElement() );
        if("show" == element->tagName())
        {
            if("true" == element->text())
                content.ifNeedShow = true;
            else
                content.ifNeedShow = false;
        }
        else  if("column" == element->tagName())
        {
            content.Column = element->text().toUShort();
        }
        else  if("height" == element->tagName())
        {
            content.heightHint = element->text().toUShort();
        }
        else  if("width" == element->tagName())
        {
            content.widthHint = element->text().toUShort();
        }

    }
}
