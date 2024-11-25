#include "model.h"


Model::Model()
{
    //PROFILES ARE TO BE LOADED TO THE ARRAY FROM THE DATABASE
    // more specifically, query the database for ids, placing them in array ids profiles are created from there.
    int ids[5] = {-1};
    //feel free to change this to fit loading architecture of the database.

    for(int i =0; (i< 5 && ids[i] != -1); i++){
        profiles[i] = new Profile(ids[i]);
    }

    currentProfile = nullptr;

}

bool Model::selectProfile(int index){
    //Functions are to be operated all under the current profile, so this is a really important, if simple, function towards not overcomplicating the system.

    if(index < 0 || index >= 5){
        return false;
    }
    currentProfile = profiles[index];
    return true;

}


bool Model::createProfile(std::string fname, std::string lname, int weight, int height, int birthmonth, int birthyear, int birthday){
    // attempts to create profile. If no room for another, false. If creation fails, false. etc...
    // DATABASE STORAGE GOES HERE FIRST, SINCE PROFILE OBJECTS ARE CREATED OUT OF THE DATABASE
    // after a successful database entry has been created, create object in a free space in the profile array

    return true;

}

bool Model::deleteProfile(std::string fname, std::string lname){
    // delete profile from database, then the associated measurements, then delete the profile object in the profiles array.

    return false;
}



