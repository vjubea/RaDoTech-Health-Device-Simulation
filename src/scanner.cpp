#include "scanner.h"
#include "profile.h"
#include "dbmanager.h"

Scanner::Scanner(Profile* profile, DBManager* dbm) : profile(profile), dbm(dbm)
{
    //initialize to -1s to indicate things not being ready yet.
    std::fill(std::begin(date), std::end(date), -1);
    std::fill(std::begin(time), std::end(time), -1);
    std::fill(std::begin(rHandBlood), std::end(rHandBlood), -1);
    std::fill(std::begin(lHandBlood), std::end(lHandBlood), -1);
    std::fill(std::begin(sleepTime), std::end(sleepTime), -1);
    heartRate = -1;
    std::fill(std::begin(rHandRead), std::end(rHandRead), -1.0f);
    std::fill(std::begin(lHandRead), std::end(lHandRead), -1.0f);
    std::fill(std::begin(rFootRead), std::end(rFootRead), -1.0f);
    std::fill(std::begin(lFootRead), std::end(lFootRead), -1.0f);
    weight = -1.0f;
    bodyTemp = -1.0f;
    notes = "";
    newSnap = nullptr;
}


void Scanner::registerReading(char side, char limb, int point, float reading){
    float* arr;
    if(side == 'l'){
        if(limb == 'h') {arr = lHandRead;}
        else {arr = lFootRead;}
    }
    else {
        if(limb == 'h'){arr = rHandRead;}
        else {arr = rFootRead;}
    }
    arr[point] = reading;

}


void Scanner::registerBlood(char side, int dyst, int syst){
    int* arr;
    if(side == 'l'){arr = lHandBlood;}
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
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (date[i] == -1) return false;
    }
    for (int i = 0; i < 2; ++i) {
        if (time[i] == -1) return false;
        if (sleepTime[i] == -1) return false;
    }
    for (int i = 0; i < 6; ++i) {
        if (rHandRead[i] == -1.0f || lHandRead[i] == -1.0f || rFootRead[i] == -1.0f || lFootRead[i] == -1.0f) {
            return false;
        }
    }


    //create Snapshot object
    newSnap = new Snapshot();
    //register hand, foot, and blood pressure readings in db, set in object
    int hrID, lrID, bpID = 0;

    QVector<int> hands;
    QVector<int> feet;
    for(int i = 0; i < 6; i++){
        hands.append(lHandRead[i]);
        hands.append(rHandRead[i]);
        feet.append(lFootRead[i]);
        feet.append(rFootRead[i]);
    }
    if(! dbm->createHandReadings(hrID, hands)) return false;
    if(! dbm->createLegReadings(lrID, feet)) return false;
    QString formattedTimeStamp = QString::asprintf("%04d-%02d-%02d %02d:%02d",
                                                  date[2], date[0], date[1],
                                                  time[0], time[1]);


    //register other values
    newSnap->setProfileID(profile->getId());
    newSnap->setTimestamp(formattedTimeStamp);
    newSnap->setBodyTemp(bodyTemp);
    newSnap->setLeftHandPressReadId(bpID);
    newSnap->setRightHandPressReadId(bpID);
    newSnap->setHeartRate(heartRate);
    newSnap->setSleepHrs(sleepTime[0]);
    newSnap->setSleepMins(sleepTime[1]);
    newSnap->setCurrWeight(weight);
    newSnap->setNotes(notes);
    newSnap->setHandReadingID(hrID);
    newSnap->setLegReadingID(lrID);

    if(dbm->addSnapshotToHistory(*newSnap)){
        profile->addSnap(newSnap);
        return true;
    }

    else{
        delete newSnap;
        newSnap = nullptr;
        return false;
    }
}

Snapshot* Scanner::getFinishedSnap(){
    return newSnap;
}
