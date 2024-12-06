#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QVector>

#include "snapshot.h"

class DBManager;

class Profile {
public:
    // Constructors
    Profile();
    Profile(int id, const QString& fname, const QString& lname, float weight, float height, const QString& bday, DBManager* dbm = nullptr);
    ~Profile();

    QVector<Snapshot*> getSnapshots();
    void refreshSnapshots(DBManager* dbm);

    // Getters and Setters
    int getId() const;
    void setId(int id);
    
    QString getFname() const;
    void setFname(const QString& fname);

    QString getLname() const;
    void setLname(const QString& lname);

    QString getFullName() const;

    float getWeight() const;
    void setWeight(float weight);

    float getHeight() const;
    void setHeight(float height);

    QString getBday() const;
    void setBday(const QString& bday);

    void addSnap(Snapshot* snap);

private:
    int id;  // Profile ID
    QString fname;  // First name
    QString lname;  // Last name
    float weight;  // Weight in kg
    float height;  // Height in cm
    QString bday;  // Birthdate (mm-dd-yyyy)
    QVector<Snapshot*> snapshots;
};

#endif // PROFILE_H
