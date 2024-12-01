#include "snapshot.h"

Snapshot::Snapshot() 
    : 
    profileID(0), bodyTemp(0.0f), 
    leftHandPressReadId(0), rightHandPressReadId(0),
    heartRate(0), sleepHrs(0), sleepMins(0),
    currWeight(0.0f), handReadingID(0), legReadingID(0) 
    {}

Snapshot::Snapshot(
    int profileID, const QString& timestamp, float bodyTemp,
    int leftHandPressReadId, int rightHandPressReadId,
    int heartRate, int sleepHrs, int sleepMins, 
    float currWeight, const QString& notes,
    int handReadingID, int legReadingID)
    : 
    profileID(profileID), timestamp(timestamp), bodyTemp(bodyTemp),
    leftHandPressReadId(leftHandPressReadId), rightHandPressReadId(rightHandPressReadId), 
    heartRate(heartRate), sleepHrs(sleepHrs), sleepMins(sleepMins), 
    currWeight(currWeight), notes(notes),
    handReadingID(handReadingID), legReadingID(legReadingID)
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

int Snapshot::getHandReadingID() const { return handReadingID; }
void Snapshot::setHandReadingID(int handReadingID) { this->handReadingID = handReadingID; }

int Snapshot::getLegReadingID() const { return legReadingID; }
void Snapshot::setLegReadingID(int legReadingID) { this->legReadingID = legReadingID; }
