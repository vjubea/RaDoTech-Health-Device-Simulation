#include "snapshot.h"
#include "defs.h"

#include"dbmanager.h"

Snapshot::Snapshot() 
    : 
    profileID(0), bodyTemp(0.0f), 
    leftHandPressReadId(0), rightHandPressReadId(0),
    heartRate(0), sleepHrs(0), sleepMins(0),
    currWeight(0.0f), handReadingsID(0), footReadingsID(0)
    {}

Snapshot::Snapshot(
    int profileID, const QString& timestamp, float bodyTemp,
    int leftHandPressReadId, int rightHandPressReadId,
    int heartRate, int sleepHrs, int sleepMins, 
    float currWeight, const QString& notes,
    int handReadingsID, int footReadingsID)
    : 
    profileID(profileID), timestamp(timestamp), bodyTemp(bodyTemp),
    leftHandPressReadId(leftHandPressReadId), rightHandPressReadId(rightHandPressReadId), 
    heartRate(heartRate), sleepHrs(sleepHrs), sleepMins(sleepMins), 
    currWeight(currWeight), notes(notes),
    handReadingsID(handReadingsID), footReadingsID(footReadingsID)
    {}


bool Snapshot::operator==(const Snapshot& other) const {
    return getProfileID() == other.getProfileID() &&
           getTimestamp() == other.getTimestamp();
}

/***** Getters-and-Setters *****/

int Snapshot::getProfileID() const { return profileID; }
void Snapshot::setProfileID(int profileID) { this->profileID = profileID; }

QString Snapshot::getTimestamp() const { return timestamp; }
void Snapshot::setTimestamp(const QString& timestamp) { this->timestamp = timestamp; }

float Snapshot::getBodyTemp() const { return bodyTemp; }
void Snapshot::setBodyTemp(float bodyTemp) { this->bodyTemp = bodyTemp; }

int Snapshot::getLeftHandPressReadId() const { return leftHandPressReadId; }
void Snapshot::setLeftHandPressReadId(int leftHandPressReadId) { this->leftHandPressReadId = leftHandPressReadId; }

int Snapshot::getRightHandPressReadId() const { return rightHandPressReadId; }
void Snapshot::setRightHandPressReadId(int rightHandPressReadId) { this->rightHandPressReadId = rightHandPressReadId; }

int Snapshot::getHeartRate() const { return heartRate; }
void Snapshot::setHeartRate(int heartRate) { this->heartRate = heartRate; }

int Snapshot::getSleepHrs() const { return sleepHrs; }
void Snapshot::setSleepHrs(int sleepHrs) { this->sleepHrs = sleepHrs; }

int Snapshot::getSleepMins() const { return sleepMins; }
void Snapshot::setSleepMins(int sleepMins) { this->sleepMins = sleepMins; }

float Snapshot::getCurrWeight() const { return currWeight; }
void Snapshot::setCurrWeight(float currWeight) { this->currWeight = currWeight; }

QString Snapshot::getNotes() const { return notes; }
void Snapshot::setNotes(const QString& notes) { this->notes = notes; }

int Snapshot::getHandReadingsID() const { return handReadingsID; }
void Snapshot::setHandReadingsID(int handReadingsID) { this->handReadingsID = handReadingsID; }

int Snapshot::getFootReadingsID() const { return footReadingsID; }
void Snapshot::setFootReadingsID(int footReadingsID) { this->footReadingsID = footReadingsID; }

void Snapshot::setDBM(DBManager* d){dbm = d;}

QVector<QString> Snapshot::getOrganValues(){
    QVector<QString> organVals;
    QVector<int> readings = getRawReadings();
    for(int i=0; i<12; i+=2){
        float avg = (readings[i] + readings[i+1])/2;
        if(avg > HandGoodReadEnd[i]) organVals.append("Above Normal");
        else if(avg < HandGoodReadStart[i]) organVals.append("Below Normal");
        else organVals.append("Normal");
    }

    for(int i=12; i<24; i+=2){
        float avg = (readings[i] + readings[i+1])/2;
        if(avg > FootGoodReadEnd[i]) organVals.append("Above Normal");
        else if(avg < FootGoodReadStart[i]) organVals.append("Below Normal");
        else organVals.append("Normal");
    }

    return organVals;
}

QVector<int> Snapshot::getRawReadings(){
    QVector<int> hands =  dbm->getHandReadingsById(handReadingsID);
    QVector<int> feet = dbm->getFootReadingsById(footReadingsID);

    //hands.append(feet);
    return hands + feet;
}

QString Snapshot::getRecommendations(){
    return "This is a placeholder for a professional recommendation. It will tell you do do xyz and buy these supplements";
}
