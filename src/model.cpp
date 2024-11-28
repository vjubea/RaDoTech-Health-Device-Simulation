#include "model.h"
#include "dbmanager.h"

Model::Model()
{
    //PROFILES ARE TO BE LOADED TO THE ARRAY FROM THE DATABASE
    //feel free to change this to fit loading architecture of the database.
    dbManager = new DBManager();
    profileID = 1;
    currentProfile = nullptr;

    // Vlad's Test for Profile creation+deletion
    QString name = "ADMIN";
    QString lname = "GO";
    QString bday = "yyyy/MM/dd";
    createProfile(name, lname, 70, 150, bday); // returns Profile Object only if succeeds being added to DB.
    selectProfile(0);
    qInfo() << currentProfile->getFname();
    deleteCurrentProfile();
}


Scanner* Model::startScan(){
    if(currentProfile == nullptr) {return nullptr;}

    return new Scanner(currentProfile);
}



bool Model::selectProfile(int index){
    //Functions are to be operated all under the current profile, so this is a really important, if simple, function towards not overcomplicating the system.

    if(index < 0 || index >= 5){
        return false;
    }
    currentProfile = profiles.at(index);
    return true;
}


bool Model::createProfile(QString& fname, QString& lname, float weight, float height, QString& birthday){
    // attempts to create profile. If no room for another, false. If creation fails, false. etc...
    // DATABASE STORAGE GOES HERE FIRST, SINCE PROFILE OBJECTS ARE CREATED OUT OF THE DATABASE
    // after a successful database entry has been created, create object in a free space in the profile array
    if (profiles.size() < 5){
        Profile* newProf = dbManager->createProfile(profileID++, fname, lname, weight, height, birthday);
        if (newProf != NULL) {
            profiles.push_back(newProf);
            return true;
        }
    }
    return false;
}

bool Model::deleteCurrentProfile() {
    bool wasDeleted = false;
    for (int i = 0;  i < profiles.size(); i++) {
        if (profiles.at(i)->getFname() == currentProfile->getFname() &&
            profiles.at(i)->getLname() == currentProfile->getLname()) {

            // delete current profile from database, then the associated measurements, then delete the profile object in the profiles array.
            dbManager->deleteProfile(currentProfile->getFname(), currentProfile->getLname());
            profiles.erase(profiles.begin() + i);
            currentProfile = nullptr;
            wasDeleted = true;
            break;
        }
    }
    return wasDeleted;
}



