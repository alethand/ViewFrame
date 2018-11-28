#include "rvalidator.h"

#include <QRegExp>
#include <QDebug>

RStringValidator::RExpressMap RStringValidator::innerRegexps;

RStringValidator rv;

RStringValidator::RStringValidator():type(RValid::R_USER_DEFINE),RValidator()
{
    if(innerRegexps.size() == 0){
        innerRegexps.insert(RValid::R_FULL_SPACE,"\\s+");
        innerRegexps.insert(RValid::R_PHONE_NUM,"\\d{11}");
        innerRegexps.insert(RValid::R_MAIL,"^[0-9a-zA-Z]+@[0-9a-zA-Z]+(\\.[0-9a-zA-Z]+)+$");

        QString SingleIp_Reg = "(\\d|([1-9]\\d)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))";
        innerRegexps.insert(RValid::R_IP,QString("(%1\\.){3}%2").arg(SingleIp_Reg,SingleIp_Reg));
    }
}

RStringValidator::RStringValidator(QString input, RValid::RegularExpress re):
    input(input),type(re),RValidator()
{

}

RStringValidator::RStringValidator(QString input, QString regexp, RValid::RegularExpress re):
    input(input),userRegexp(regexp),type(re),RValidator()
{

}

void RStringValidator::setUserRegexp(QString text)
{
    this->userRegexp = text;
}

RValid::State RStringValidator::validate()
{
    QRegExp exp;

    if(type != RValid::R_USER_DEFINE){
        exp.setPattern(innerRegexps.value(type));
    }else{
        exp.setPattern(userRegexp);
    }

    if(exp.exactMatch(input))
        return RValid::Acceptable;
    else
        return RValid::Invalid;
}

void RStringValidator::setRegularExpress(RValid::RegularExpress tt)
{
    this->type = tt;
}
