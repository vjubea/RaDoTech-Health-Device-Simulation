#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "profile.h"

class Model{

private:
    Profile* profiles[5];
    Profile* currentProfile;

public:
    Model();

    bool selectProfile(int index);

    bool createProfile(std::string fname = "NULL", std::string lname = "NULL", int weight = 40000, int height = 160, int birthmonth = 1, int birthyear = 1970, int birthday = 1);

    bool deleteProfile(std::string fname, std::string lname);

};

#endif // MODEL_H
