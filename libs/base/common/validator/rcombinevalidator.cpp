#include "rcombinevalidator.h"

#include <QDebug>

RCombineValidator::RCombineValidator()
{

}

RCombineValidator::~RCombineValidator()
{
    std::for_each(vlist.begin(),vlist.end(),[&](RValidator * valid){
        delete valid;
    });
    vlist.clear();
}

void RCombineValidator::addValidator(RValidator *v)
{
    vlist.push_back(v);
}

RAndCombineValidator::RAndCombineValidator():RCombineValidator()
{

}

RValid::State RAndCombineValidator::validate()
{
    RValid::State result = RValid::Invalid;

    for(int i = 0; i < vlist.size();i++){
        result = vlist.at(i)->validate();
        if(result == RValid::Invalid )
            break;
    }
    return result;
}

ROrCombineValidator::ROrCombineValidator():RCombineValidator()
{

}

RValid::State ROrCombineValidator::validate()
{
    RValid::State result = RValid::Invalid;

    for(int i = 0; i < vlist.size();i++){
        result = vlist.at(i)->validate();
        if(result == RValid::Acceptable )
            break;
    }
    return result;
}
