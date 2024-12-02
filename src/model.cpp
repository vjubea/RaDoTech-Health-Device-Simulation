#include "model.h"
#include "dbmanager.h"

Model::Model() {
    //PROFILES ARE TO BE LOADED TO THE ARRAY FROM THE DATABASE
    //feel free to change this to fit loading architecture of the database.
    dbManager = new DBManager();
    currentProfile = nullptr;
    dbManager->getAllProfiles(profiles); // Populates profiles from the DB

    // Vlad's Test for Profile creation+deletion+updating
    QString name = "ADMIN";
    QString lname = "GO";
    QString bday = "yyyy/MM/dd";
    createProfile(name, lname, 70, 150, bday); // returns Profile Object only if succeeds being added to DB.
    selectProfile(0);
    deleteCurrentProfile();
    createProfile(name, lname, 70, 150, bday);
    name = "Vlad";
    createProfile(name, lname, 70, 150, bday);
    name = "Clad";
    createProfile(name, lname, 70, 150, bday);
    name = "Road";
    createProfile(name, lname, 70, 150, bday);
    name = "Toad";
    dbManager->updateProfile(profiles.at(3), name, lname, 90, 150, bday);
    selectProfile(3);

    /*
    -For scans, we create them in model, then use dbManager->createLeg/HandReadings()
        id& parameter passed used to save the auto-incremented id of the insertion, and the QVector<int>& will
        pass values by reference (for efficiency), using it for DB insertion.
    - We should use id& in the creation ofa Snapshot object that is then passed
        into the dbManager->addSnapshotToHistory(), where we
    */
}

Scanner* Model::startScan() {
    if(currentProfile == nullptr) {return nullptr;}

    return new Scanner(currentProfile, dbManager);
}

bool Model::selectProfile(int index) {
    //Functions are to be operated all under the current profile, so this is a really important, if simple, function towards not overcomplicating the system.
    if(index < 0 || index >= 5){
        return false;
    }
    currentProfile = profiles.at(index);
    qInfo() <<"Profile ID:"<<currentProfile->getId()<<" with Name:"<<currentProfile->getFname();
    return true;
}


bool Model::createProfile(QString& fname, QString& lname, float weight, float height, QString& birthday) {
    // attempts to create profile. If no room for another, false. If creation fails, false. etc...
    // DATABASE STORAGE GOES HERE FIRST, SINCE PROFILE OBJECTS ARE CREATED OUT OF THE DATABASE
    // after a successful database entry has been created, create object in a free space in the profile array
    if (profiles.size() < 5){
        Profile* newProf = dbManager->createProfile(fname, lname, weight, height, birthday);
        if (newProf != NULL) {
            profiles.push_back(newProf);
            return true;
        }
        qWarning() <<"ERR: Could not creat Profile. Database entry error.";
        return false;
    }
    qWarning() << "ERR: Could not Create Profile. Profiles exceed capacity.";
    return false;
}

bool Model::editCurProfile(QString &fname, QString &lname, float weight, float height, QString &bday){
    if(fname == "") fname = currentProfile->getFname();
    if(lname == "") lname = currentProfile->getLname();
    if(weight == -1.0f) weight = currentProfile->getWeight();
    if(height == -1.0f) height = currentProfile->getHeight();
    if(bday == "") bday = currentProfile->getBday();

    return dbManager->updateProfile(currentProfile, fname, lname, weight, height, bday);
}


bool Model::deleteCurrentProfile() {
    bool wasDeleted = false;
    for (int i = 0;  i < profiles.size(); i++) {
        if (profiles.at(i)->getFname() == currentProfile->getFname() &&
            profiles.at(i)->getLname() == currentProfile->getLname()) {

            /***
                Delete current profile from database, then the associated measurements,
                then delete the profile object in the profiles array.
            ***/
            dbManager->deleteProfile(currentProfile->getId());
            profiles.erase(profiles.begin() + i);
            currentProfile = nullptr;
            wasDeleted = true;
            break;
        }
    }
    return wasDeleted;
}



