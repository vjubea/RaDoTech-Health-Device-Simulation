#include "profile.h"
#include <iostream>

#include "dbmanager.h"

Profile::Profile() : id(0), weight(0), height(0) {}

Profile::Profile(
    int id, const QString& fname, const QString& lname, float weight,
    float height, const QString& bday, DBManager* dbm)
    :
    id(id), fname(fname), lname(lname), weight(weight), 
    height(height), bday(bday)
    {
        if(dbm != nullptr) {dbm->getAllSnapshotsOfUser(snapshots, id);}
    }

Profile::~Profile(){
    auto it = snapshots.begin();
    while (it != snapshots.end()){
        delete *it;
        it = snapshots.erase(it);
    }
}

QVector<Snapshot*> Profile::getSnapshots(){return snapshots;}

void Profile::refreshSnapshots(DBManager *dbm){
   // function reloads snapshots from the dbm, then adds only the new snapshots to the snapshots array

   QVector<Snapshot*> newsnaps;
   dbm->getAllSnapshotsOfUser(newsnaps, id);

   auto it = newsnaps.begin();
   while (it != newsnaps.end()) {
       Snapshot* newsnap = *it;
       if (std::any_of(snapshots.begin(), snapshots.end(),
                       [newsnap](Snapshot* snap) { return *snap == *newsnap; })) {
           delete newsnap;
           it = newsnaps.erase(it);
       } else {
           snapshots.append(newsnap);
           ++it;
       }
   }
}


int Profile::getId() const { return id; }
void Profile::setId(int id) { this->id = id; }

QString Profile::getFname() const { return fname; };
void Profile::setFname(const QString& fname) { this->fname = fname; }

QString Profile::getLname() const { return lname; }
void Profile::setLname(const QString& lname) { this->lname = lname; }

float Profile::getWeight() const { return weight; }
void Profile::setWeight(float weight) { this->weight = weight; }

float Profile::getHeight() const { return height; }
void Profile::setHeight(float height) { this->height = height; }

QString Profile::getBday() const { return bday; }
void Profile::setBday(const QString& bday) { this->bday = bday; }

void Profile::addSnap(Snapshot* snap){
    if(snap != nullptr) snapshots.append(snap);
}
