#include "profile.h"

Profile::Profile() : id(0), weight(0), height(0) {}

Profile::Profile(
    int id, const QString& fname, const QString& lname, float weight,
    float height, const QString& bday)
    :
    id(id), fname(fname), lname(lname), weight(weight), 
    height(height), bday(bday)
    {}

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
