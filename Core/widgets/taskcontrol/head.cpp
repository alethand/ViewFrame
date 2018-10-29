#include "head.h"

namespace TaskControlModel {

QDataStream & operator<<(QDataStream & stream,const BandControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<info.originFrequency<<info.stopFrequency<<info.frequencyStopping;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, BandControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    stream>>info.originFrequency>>info.stopFrequency>>info.frequencyStopping;
    return stream;
}


QDataStream & operator<<(QDataStream & stream,const StateControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<(int)info.workmode<<(int)info.workCycles<<info.workCyclesNumber<<(int)info.workingBandwidth<<(int)info.antennaSelection1
         <<(int)info.antennaSelection2;
    stream<<info.attenuationCode1<<info.attenuationCode2<<info.attenuationCode3<<info.attenuationCode4<<info.attenuationCode5
            <<info.attenuationCode6<<info.attenuationCode7<<info.attenuationCode8<<info.attenuationCode9<<info.attenuationCode10;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, StateControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    int workmode = 0;
    int workCycles = 0;

    stream >> workmode >> workCycles;

    info.workmode = workmode;
    info.workCycles = workCycles;

    stream>>info.workCyclesNumber;

    int workingBandwidth = 0,  antennaSelection1 = 0,antennaSelection2 = 0;

    stream >> workingBandwidth >> antennaSelection1 >> antennaSelection2;

    info.workingBandwidth = workingBandwidth;
    info.antennaSelection1 = antennaSelection1;
    info.antennaSelection2 = antennaSelection2;

    stream>>info.attenuationCode1>>info.attenuationCode2>>info.attenuationCode3>>info.attenuationCode4>>info.attenuationCode5
            >>info.attenuationCode6>>info.attenuationCode7>>info.attenuationCode8>>info.attenuationCode9>>info.attenuationCode10;

    return stream;
}


QDataStream & operator<<(QDataStream & stream,const GatherControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<(int)info.gatherWay<<(int)info.gatherType<<(int)info.sendingPause<<info.extractSwitch;
    stream<<info.gatherTime<<info.gatherImpulseNumber<<info.gatherFrequencyMin<<info.gatherFrequencyMax<<info.gatherPulseyMin
            <<info.gatherPulseyMax<<info.gatherPowerMin<<info.gatherPowerMax<<(int)info.gatherTacticsSwitch;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, GatherControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    int gatherWay= 0,gatherType = 0,sendingPause = 0;
    stream >> gatherWay >> gatherType >> sendingPause;
    info.gatherWay = gatherWay;
    info.gatherType = gatherType;
    info.sendingPause = sendingPause;
    stream>>info.extractSwitch>>info.gatherTime>>info.gatherImpulseNumber>>info.gatherFrequencyMin>>info.gatherFrequencyMax
            >>info.gatherPulseyMin>>info.gatherPulseyMax>>info.gatherPowerMin>>info.gatherPowerMax;

    int gatherTacticsSwitch = 0;
    stream >> gatherTacticsSwitch;
    info.gatherTacticsSwitch = gatherTacticsSwitch;

    return stream;
}

QDataStream & operator<<(QDataStream & stream,const SelfCheckControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<(int)info.workmode<<(int)info.workCycles<<info.workCyclesNumber<<(int)info.workingBandwidth;
    stream<<info.originFrequency<<info.stopFrequency<<info.frequencyStopping<<info.attenuationCode1<<info.attenuationCode2
            <<info.attenuationCode3<<info.attenuationCode4<<info.attenuationCode5<<info.attenuationCode6
           <<info.attenuationCode7<<info.attenuationCode8<<info.attenuationCode9<<info.attenuationCode10;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, SelfCheckControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    int workmode = 0,workCycles = 0;
    stream >> workmode >> workCycles;

    info.workmode = workmode;
    info.workCycles = workCycles;

    int workingBandwidth = 0;
    stream>>info.workCyclesNumber;

    stream >> workingBandwidth;
    info.workingBandwidth = workingBandwidth;

    stream>>info.originFrequency>>info.stopFrequency>>info.frequencyStopping>>info.attenuationCode1>>info.attenuationCode2
           >>info.attenuationCode3>>info.attenuationCode4>>info.attenuationCode5>>info.attenuationCode6
          >>info.attenuationCode7>>info.attenuationCode8>>info.attenuationCode9>>info.attenuationCode10;

    return stream;
}

QDataStream & operator<<(QDataStream & stream,const InstrumentControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<info.instrumentType<<info.instrumentModel<<info.instrumentCommType<<QString(info.IPAddress)<<info.networkPort
           <<info.GPIBPort<<info.signalCarrierFrequency<<info.signalRepetitionPeriod<<info.signalPulseWidth
            <<info.signalPower<<info.radioFrequencySwitchControl<<info.intrapulseSwitchControl<<info.ImpulseSwitchControl
             <<info.centreFrequency<<info.displayBandwidth<<info.IFBandwidth<<info.videoBandwidth
              <<info.scanTime<<info.scanPoints<<info.ifReadspectrumData<<info.signalBase
               <<info.powerCalibrationControl<<info.immediatePowerCalibration;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, InstrumentControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    stream>>info.instrumentType>>info.instrumentModel>>info.instrumentCommType;
    QString ip = QString(info.IPAddress);
    memset(info.IPAddress,0,sizeof(info.IPAddress));
    memcpy(info.IPAddress,ip.toLocal8Bit().data(),ip.toLocal8Bit().size());

    stream>>info.networkPort>>info.GPIBPort>>info.signalCarrierFrequency>>info.signalRepetitionPeriod>>info.signalPulseWidth
            >>info.signalPower>>info.radioFrequencySwitchControl>>info.intrapulseSwitchControl>>info.ImpulseSwitchControl
             >>info.centreFrequency>>info.displayBandwidth>>info.IFBandwidth>>info.videoBandwidth
              >>info.scanTime>>info.scanPoints>>info.ifReadspectrumData>>info.signalBase
               >>info.powerCalibrationControl>>info.immediatePowerCalibration;
    return stream;
}

QDataStream & operator<<(QDataStream & stream,const TurntableControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<info.type<<info.position<<info.speed<<info.directionRotation<<info.zeroPosSetting;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, TurntableControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    stream>>info.type>>info.position>>info.speed>>info.directionRotation>>info.zeroPosSetting;

    return stream;
}

QDataStream & operator<<(QDataStream & stream,const PlayBackControl & info)
{
    stream<<info.userChecked<<(int)info.taskType<<info.parameter<<info.excuteTime<<info.lastTime<<(int)info.dstate;
    stream<<QString(info.path)<<info.type<<info.originPos<<info.speed;
    return stream;
}

QDataStream & operator>>(QDataStream & stream, PlayBackControl & info)
{
    stream>>info.userChecked;
    int type = 0;
    stream >> type;
    info.taskType = static_cast<Type>(type);

    stream>>info.parameter>>info.excuteTime>>info.lastTime;

    int state = 0;
    stream >> state;
    info.dstate = static_cast<DistuributeState>(state);

    QString path;
    stream >> path;
    memset(info.path,0,sizeof(info.path));
    memcpy(info.path,path.toLocal8Bit().data(),path.toLocal8Bit().size());

    stream>>info.type>>info.originPos>>info.speed;

    return stream;
}

} //namespace TaskControlModel
