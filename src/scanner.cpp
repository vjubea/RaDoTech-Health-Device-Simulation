#include "scanner.h"
#include "profile.h"


Scanner::Scanner(Profile* profile) : profile(profile)
{

}


void registerReading(char side, char limb, int point, float reading){

}


void registerBlood(char side, int dyst, int syst){

}


void registerWeight(int w){

}


int finishScan(){
    // this functionshould place everything into the database as a scan. Values not held that are known of the profile should be retrieved as well
    // data in this object should then be cleaned up.


    //return ID of the scan that was just created in the db.
    return 0;
}
