#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QString>

class DBManager;

class Snapshot {
public:
    // Constructors
    Snapshot();
    Snapshot(int profileID, const QString& timestamp, float bodyTemp, int leftHandPressReadId, int rightHandPressReadId, 
             int heartRate, int sleepHrs, int sleepMins, float currWeight, const QString& notes, 
             int handReadingsID, int footReadingsID);

    bool operator==(const Snapshot& other) const;

    // Getters and Setters
    int getProfileID() const;
    void setProfileID(int profileID);

    QString getTimestamp() const;
    void setTimestamp(const QString& timestamp);

    float getBodyTemp() const;
    void setBodyTemp(float bodyTemp);

    int getLeftHandPressReadId() const;
    void setLeftHandPressReadId(int lhPressReadId);

    int getRightHandPressReadId() const;
    void setRightHandPressReadId(int rhPressReadId);

    int getHeartRate() const;
    void setHeartRate(int heartRate);

    int getSleepHrs() const;
    void setSleepHrs(int sleepHrs);

    int getSleepMins() const;
    void setSleepMins(int sleepMins);

    float getCurrWeight() const;
    void setCurrWeight(float currWeight);

    QString getNotes() const;
    void setNotes(const QString& notes);

    int getHandReadingsID() const;
    void setHandReadingsID(int handReadingsID);

    int getFootReadingsID() const;
    void setFootReadingsID(int footReadingsID);

    void setDBM(DBManager*);

    QVector<QString> getOrganValues();
    QVector<int> getRawReadings();
    QString getRecommendations();

private:
    int profileID;  // Foreign key linking to Profile
    QString timestamp;  // Date and time of the snapshot
    float bodyTemp;  // Body temperature in Celsius
    int leftHandPressReadId;  // Foreign key for left hand blood pressure reading
    int rightHandPressReadId;  // Foreign key for right hand blood pressure reading
    int heartRate;  // Heart rate in beats per minute
    int sleepHrs;  // Hours of sleep
    int sleepMins;  // Minutes of sleep
    float currWeight;  // Current weight in kg
    QString notes;  // Optional notes
    int handReadingsID;  // Foreign key for hand readings
    int footReadingsID;  // Foreign key for leg readings

    DBManager* dbm;
};

#endif // SNAPSHOT_H
