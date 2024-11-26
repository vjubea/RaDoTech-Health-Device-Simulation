#include "profile.h"

Profile::Profile(int id)
{
    // PROFILE IS TO BE LOADED FROM DATABASE
    // no profile objects should be created unless they correspond to a known repository entry, so it *should* be safe to presume that an entry exists

}


Snapshot* getSnapshot(int id){
    //if snapshot is associated with the profile, and the id exists, get it. Else return nullptr.
    return new Snapshot(id);
}

Snapshot* getMostRecentSnapshot(){
    //find the most recent one from db.

    return getSnapshot(0);
}



std::string Profile::getFullName() {return (fname + " " + lname);}

std::string Profile::getFName() {return fname;}

std::string Profile::getLName() {return lname;}

int Profile::getWeight() {return weight;}

int Profile::getHeight() {return height;}

int* Profile::getDob() {return dob;}
