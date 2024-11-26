#ifndef SCANNER_H
#define SCANNER_H

#include<string>

//imports in .cpp to avoid circular imports in headers
class Profile;

class Scanner{
/*
Scanner is the class that will be used in the creation of snapshots.
The mainwindow will have direct access to the scanner object that is created on beginning a scan.
There should only ever be one scanner object at a time for this application's design - it's purpose is to be able to collect data then store it in the database on completion
*/

private:
    Profile* profile;

    int date[3]; // mm/dd/yyy
    int time[2]; // hh:mm

    float rHandRead[6];
    float lHandRead[6];
    float rFootRead[6];
    float lFootRead[6];
    int rHandBlood[2];
    int lHandBlood[2];
    int weight; //in grams
    int heartRate;
    float bodyTemp; //in Celcius
    int sleepTime[2]; // hh:mm
    std::string text;


public:
    Scanner(Profile* profile);

    void registerReading(char side, char limb, int point, float reading);
    void registerBlood(char side, int dyst, int syst);
    void registerWeight(int w);
    void registerHeartRate(int h);
    void registerBodyTemp(float t);
    void registerDate(int m, int d, int y);
    void registerTime(int h, int m);
    void registerSleepTime(int h, int m);

    int finishScan();
};

#endif // SCANNER_H
