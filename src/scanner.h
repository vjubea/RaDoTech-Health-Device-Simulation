#ifndef SCANNER_H
#define SCANNER_H

#include<string>
#include <QString>
#include <QVector>
#include "snapshot.h"

//imports in .cpp to avoid circular imports in headers
class Profile;
class DBManager;

class Scanner{
/*
Scanner is the class that will be used in the creation of snapshots.
The mainwindow will have direct access to the scanner object that is created on beginning a scan.
There should only ever be one scanner object at a time for this application's design - it's purpose is to be able to collect data then store it in the database on completion
*/

    private:
        Profile* profile;
        Snapshot* newSnap;
        DBManager* dbm;

        int date[3]; // mm/dd/yyyy
        int time[2]; // hh:mm

        QVector<int>* rHandRead;
        QVector<int>* lHandRead;
        QVector<int>* rFootRead;
        QVector<int>* lFootRead;
        int rHandBlood[2];
        int lHandBlood[2];
        float weight; //in grams
        int heartRate;
        float bodyTemp; //in Celcius
        int sleepTime[2]; // hh:mm
        QString notes;
    

    public:
        Scanner(Profile* profile, DBManager* dbm);

        void registerReading(char side, char limb, int reading);
        void registerBlood(char side, int dyst, int syst);
        void registerWeight(float w);
        void registerHeartRate(int h);
        void registerBodyTemp(float t);
        void registerDate(int m, int d, int y);
        void registerTime(int h, int m);
        void registerSleepTime(int h, int m);
        void registerNotes(QString n);

        void genRandomSnap();
        int genScanVal(char limb, int index);

        bool finishScan();
        Snapshot* getFinishedSnap();
};

#endif // SCANNER_H
