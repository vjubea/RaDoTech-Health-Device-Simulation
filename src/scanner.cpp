#include "scanner.h"
#include "profile.h"


Scanner::Scanner(Profile* profile) : profile(profile)
{

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


void Scanner::registerWeight(int w){
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



int Scanner::finishScan(){
    // this functionshould place everything into the database as a scan. Values not held that are known of the profile should be retrieved as well
    // data in this object should then be cleaned up.


    //return ID of the scan that was just created in the db. This can be used in the GUI to immediately retrieve the scan and display it.
    return 0;
}
