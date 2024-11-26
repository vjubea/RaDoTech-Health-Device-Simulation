#ifndef SCANNER_H
#define SCANNER_H

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

    float rHandRead[6];
    float lHandRead[6];
    float rFootRead[6];
    float lFootRead[6];
    int rHandBlood[2];
    int lHandBlood[2];
    int weight;
    //fill in other fields to register


public:
    Scanner(Profile* profile);

    void registerReading(char side, char limb, int point, float reading);
    void registerBlood(char side, int dyst, int syst);
    void registerWeight(int w);

    int finishScan();
};

#endif // SCANNER_H
