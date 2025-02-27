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
    QString bday = "yyyy-MM-dd hh:mm";
    createProfile(name, lname, 70, 150, bday); // returns Profile Object only if succeeds being added to DB.
    getAllProfiles();
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
    getAllProfiles();
    // Properly updates the profiles QVector, and DB
    dbManager->updateProfile(profiles.at(3), name, lname, 90, 150, bday);
    selectProfile(3);

    Scanner* scanner = startScan();
    for (int i=0; i<6; ++i) { // i from 1-12 is H, i from 13-24 is F. Odd i = L, Even i = R
        scanner->registerReading('L','H',10);
        scanner->registerReading('R','H',10);
        scanner->registerReading('L','F',10);
        scanner->registerReading('R','F',10);
    }
    scanner->registerHeartRate(10);
    scanner->registerSleepTime(7, 30);
    scanner->registerBodyTemp(36);
    scanner->registerDate(10, 4, 2024);
    scanner->registerTime(23, 59);
    scanner->finishScan(); // Saves snapshot to DB with all its required fields
    //gen three more scans
    delete scanner;
    scanner  = startScan();
    scanner->genRandomSnap();
    delete scanner;
    scanner  = startScan();
    scanner->genRandomSnap();
    delete scanner;
    scanner  = startScan();
    scanner->genRandomSnap();


    QVector<Snapshot*> snaps;
    dbManager->getAllSnapshots(snaps);
    qWarning() << (snaps.size());
    Snapshot* snap = snaps.at(0);
    qWarning() << "Here are the readings for ProfileID:" << snap->getProfileID();
    qWarning() << "LegReadID and HandReadID: " << snap->getFootReadingsID() << snap->getHandReadingsID();

    QVector<int> readings = dbManager->getFootReadingsById(snap->getFootReadingsID());
    QVector<int> hreadings = dbManager->getHandReadingsById(snap->getHandReadingsID());
    for (int i=0; i<6; ++i) {
        qWarning() <<"Foot Reading at ("<<i<<") is: "<<readings.at(i);
        //qWarning() <<"Hand Reading at ("<<i<<") is: "<<hreadings.at(i);
    }


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

QVector<Profile*> Model::getAllProfiles() {
    for(int i = 0; i < profiles.length(); i++) {
        delete profiles[i];
    }
    profiles.clear();
    dbManager->getAllProfiles(profiles);
    return profiles;
}

Profile* Model::getProfileByID(int profileID) {
    return dbManager->getProfileByID(profileID);
}

bool Model::getAllSnapshots(QVector<Snapshot*> &snaps) {
    return dbManager->getAllSnapshots(snaps);
}

bool Model::createProfile(QString& fname, QString& lname, float weight, float height, QString& birthday) {
    // attempts to create profile. If no room for another, false. If creation fails, false. etc...
    if (profiles.size() < 5){
        Profile* newProf = dbManager->createProfile(fname, lname, weight, height, birthday);
        if (newProf != nullptr) {
            return true;
        }
        qWarning() <<"ERR: Could not Create Profile. Database entry error.";
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


Profile* Model::getCurProfile(){
    return currentProfile;
}

QVector<QString> Model::getProfileNames(){
    QVector<QString> names;
    for (int i = 0;  i < profiles.size(); i++) {
        names.append(profiles[i]->getFullName());
    }
    return names;

}


DBManager* Model::getDBM() {return dbManager;}
