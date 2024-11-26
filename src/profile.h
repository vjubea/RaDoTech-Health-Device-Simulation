#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include "snapshot.h"


class Profile{

private:
    int id;
    std::string fname;
    std::string lname;
    int weight; // stored in GRAMS so we can keep it in an int.
    int height; // in CM
    int dob[3]; // date of birth as an array of 3 ints - mm/dd/yyyy


public:
    Profile(int id);

    //functions for scanning to go here... will create bases for them tomorrow!


    Snapshot* getSnapshot(int id);
    Snapshot* getMostRecentSnapshot();


    //getters
    std::string getFullName();
    std::string getFName();
    std::string getLName();
    int getWeight();
    int getHeight();
    int* getDob();


};

#endif // PROFILE_H
