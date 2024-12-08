#include "scanner.h"
#include "profile.h"
#include "dbmanager.h"

#include <QRandomGenerator>
#include "defs.h"

Scanner::Scanner(Profile* profile, DBManager* dbm) : profile(profile), dbm(dbm)
{
    //initialize to -1s to indicate things not being ready yet.
    std::fill(std::begin(date), std::end(date), -1);
    std::fill(std::begin(time), std::end(time), -1);
    std::fill(std::begin(rHandBlood), std::end(rHandBlood), -1);
    std::fill(std::begin(lHandBlood), std::end(lHandBlood), -1);
    std::fill(std::begin(sleepTime), std::end(sleepTime), -1);
    heartRate = -1;
    rHandRead = new QVector<int>();
    lHandRead = new QVector<int>();
    rFootRead = new QVector<int>();
    lFootRead = new QVector<int>();
    weight = -1.0f;
    bodyTemp = -1.0f;
    notes = "";
    newSnap = nullptr;
}


void Scanner::registerReading(char side, char limb, int reading){
    QVector<int>* arr;
    if(side == 'L'){
        if(limb == 'H') {
            arr = lHandRead;
        }
        else {
            arr = lFootRead;
        }
    }
    else {
        if(limb == 'H') {
            arr = rHandRead;
        }
        else {
            arr = rFootRead;
        }
    }
    if(arr->size() < 6)
        arr->append(reading);
    else
        qWarning() << "Scanner l/r Hand/Foot Read array is at max capacity (6).";
}

void Scanner::registerBlood(char side, int dyst, int syst)
{
    int* arr;
    if(side == 'L'){arr = lHandBlood;}
    else {arr = rHandBlood;}
    arr[0] = dyst;
    arr[1] = syst;
}

void Scanner::registerWeight(float w){
    if(w == -1){
        w = profile->getWeight();
    }
    weight = w;
}

void Scanner::registerHeartRate(int h){heartRate = h;}

void Scanner::registerBodyTemp(float t){bodyTemp = t;}

void Scanner::registerDate(int m, int d, int y){date[0] = m; date[1] = d; date[2] = y;}

void Scanner::registerTime(int h, int m){time[0] = h; time[1] = m;}

void Scanner::registerSleepTime(int h, int m){sleepTime[0] = h; sleepTime[1] = m;}

void Scanner::registerNotes(QString n){notes = n;}


bool Scanner::finishScan(){
    registerWeight(weight);
    //check if any data invalid, return false if so
    if (heartRate == -1 || weight == -1.0f || bodyTemp == -1.0f) {
        qWarning()<<"Missing fields, please populate before finishing Snapshot.";
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (date[i] == -1) {
            qWarning() << "Missing date";
            return false;
        }
    }
    for (int i = 0; i < 2; ++i) {
        if (time[i] == -1) {
            qWarning() << "Missing time";
            return false;
        }
        if (sleepTime[i] == -1) {
            qWarning() << "Missing sleepTime";
            return false;
        }
    }
    if (rHandRead->size() != 6 || lHandRead->size() != 6 ||
        rFootRead->size() != 6 || lFootRead->size() != 6) {
            qWarning() << "Hand and Leg Readings are missing readings.";
            return false;
    }

    //register hand, foot, and blood pressure readings in db, set in object
    int hrID, lrID, bpID = 0;

    QVector<int> hands;
    QVector<int> feet;
    // Add hand and feet readings
    for(int i = 0; i < 6; i++) {
        hands.append(lHandRead->at(i));
        hands.append(rHandRead->at(i));
        feet.append(lFootRead->at(i));
        feet.append(rFootRead->at(i));
    }

    if(! dbm->createHandReadings(hrID, hands)) return false;
    if(! dbm->createFootReadings(lrID, feet)) return false;

    //create Snapshot object
    newSnap = new Snapshot();
    newSnap->setHandReadingsID(hrID);
    newSnap->setFootReadingsID(lrID);

    QString formattedTimeStamp = QString::asprintf("%04d-%02d-%02d %02d:%02d",
                                                  date[2], date[0], date[1],
                                                  time[0], time[1]);
    newSnap->setTimestamp(formattedTimeStamp);

    //register other values
    newSnap->setProfileID(profile->getId());
    newSnap->setBodyTemp(bodyTemp);
    newSnap->setLeftHandPressReadId(bpID);
    newSnap->setRightHandPressReadId(bpID);
    newSnap->setHeartRate(heartRate);
    newSnap->setSleepHrs(sleepTime[0]);
    newSnap->setSleepMins(sleepTime[1]);
    newSnap->setCurrWeight(weight);
    newSnap->setNotes(notes);

    qInfo() << "Attempting to Save Snapshot to the DB";
    if(dbm->addSnapshotToHistory(*newSnap)) {
        qInfo() << "ProfileID of new Snapshot:" << newSnap->getProfileID();
        return true;
    }
    else {
        qWarning() << "Deleting Incomplete Snapshot";
        delete newSnap;
        newSnap = nullptr;
        return false;
    }
}

Snapshot* Scanner::getFinishedSnap(){
    return newSnap;
}


void Scanner::genRandomSnap(){
    for(int i = 0; i< 6; i++){
        registerReading('L','H',genScanVal('H',i));
        registerReading('R','H',genScanVal('H',i));
        registerReading('L','F',genScanVal('F',i));
        registerReading('R','F',genScanVal('F',i));
    }

    registerWeight(10.0f);
    registerBlood('L', 0,0);
    registerBlood('R',0,0);
    registerBodyTemp(10.0);
    registerHeartRate(10);
    registerSleepTime(10,10);
    registerTime(QRandomGenerator::global()->bounded(1,24),QRandomGenerator::global()->bounded(0,59));
    registerDate(QRandomGenerator::global()->bounded(1,12),QRandomGenerator::global()->bounded(1,30),QRandomGenerator::global()->bounded(2000,2024));
    registerNotes("Normal scan");

    finishScan();
}

int Scanner::genScanVal(char limb, int index){
    int reading;
    if(limb == 'H') reading = QRandomGenerator::global()->bounded(HandGoodReadStart[index] - 3, HandGoodReadEnd[index] +3); // Generate a random reading in range
    else reading = reading = QRandomGenerator::global()->bounded(FootGoodReadStart[index] - 3, FootGoodReadEnd[index] +3);
    return reading;
}





