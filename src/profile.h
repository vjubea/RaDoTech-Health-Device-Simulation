#ifndef PROFILE_H
#define PROFILE_H

#include <QString>

class Profile {
public:
    // Constructors
    Profile();
    Profile(int id, const QString& fname, const QString& lname, float weight, float height, const QString& bday);

    // Getters and Setters
    int getId() const;
    void setId(int id);
    
    QString getFname() const;
    void setFname(const QString& fname);

    QString getLname() const;
    void setLname(const QString& lname);

    float getWeight() const;
    void setWeight(float weight);

    float getHeight() const;
    void setHeight(float height);

    QString getBday() const;
    void setBday(const QString& bday);

private:
    int id;  // Profile ID
    QString fname;  // First name
    QString lname;  // Last name
    float weight;  // Weight in kg
    float height;  // Height in cm
    QString bday;  // Birthdate (mm-dd-yyyy)
};

#endif // PROFILE_H
