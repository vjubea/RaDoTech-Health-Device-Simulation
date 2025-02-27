#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "profile.h"
#include "scanner.h"
#include "dbmanager.h"

class Model
{
    private:
        int profileID;
        QVector<Profile*> profiles;
        Profile* currentProfile;
        DBManager* dbManager;

    public:
        Model();

        Scanner* startScan();

        bool selectProfile(int index);
        QVector<Profile*> getAllProfiles();
        Profile* getProfileByID(int profileID);
        bool createProfile(QString& fname, QString& lname, float weight, float height, QString& bday);
        bool editCurProfile(QString& fname, QString& lname, float weight, float height, QString& bday);
        bool deleteCurrentProfile();

        bool getAllSnapshots(QVector<Snapshot*> &snaps);
        Profile* getCurProfile();
        QVector<QString> getProfileNames();
        DBManager* getDBM();
};

#endif // MODEL_H
