#include "dbmanager.h"
#include "profile.h"
#include "snapshot.h"
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

const QString DBManager::DATABASE_PATH = "/database/raDoTech.db"; // Update with actual path

DBManager::DBManager()
{
    qInfo() << "Got here";

    // Initialize the database connection
    raDoTechDB = QSqlDatabase::addDatabase("QSQLITE");
    raDoTechDB.setDatabaseName("raDoTech.db");

    if (!raDoTechDB.open()) {
        qWarning() << "Database connection failed!";
    }
    DBInit();
}

bool DBManager::DBInit()
{
    QSqlQuery query;
    // Drop tables if exist
    query.exec("DROP TABLE IF EXISTS Profiles;");
    query.exec("DROP TABLE IF EXISTS Snapshots;");
    query.exec("DROP TABLE IF EXISTS HandBloodPressure;");
    query.exec("DROP TABLE IF EXISTS HandReadings;");
    query.exec("DROP TABLE IF EXISTS FootReadings;");

    query.exec("CREATE TABLE IF NOT EXISTS Profiles ("
                   "id INTEGER PRIMARY KEY, "
                   "fname TEXT NOT NULL, "
                   "lname TEXT NOT NULL, "
                   "weight REAL NOT NULL, "
                   "height REAL NOT NULL, "
                   "birthDay TEXT NOT NULL, "
                   "UNIQUE (fname, lname)"
               ");"
    );
    query.exec("CREATE TABLE IF NOT EXISTS Snapshots ("
                   "profileID INTEGER NOT NULL, "
                   "timestamp VARCHAR(16) NOT NULL, "
                   "bodyTemp REAL NOT NULL, "
                   "leftHandPressReadID INTEGER DEFAULT 0, "
                   "rightHandPressReadID INTEGER DEFAULT 0, "
                   "heartRate INTEGER NOT NULL, "
                   "sleepHrs INTEGER, "
                   "sleepMins INTEGER, "
                   "currWeight REAL NOT NULL, "
                   "notes TEXT, "
                   "handReadingsID INTEGER NOT NULL, "
                   "footReadingsID INTEGER NOT NULL, "
                   "PRIMARY KEY (profileID, timestamp), "
                   "FOREIGN KEY (profileID) REFERENCES Profiles(id) ON DELETE CASCADE"
               ");"
    );
    query.exec("CREATE TABLE IF NOT EXISTS HandBloodPressure("
                   "id INTEGER NOT NULL, "
                   "orientation CHAR(1) NOT NULL, -- “L” for left, “R” for right "
                   "systolic INTEGER NOT NULL, -- Systolic mm Hg "
                   "diastolic INTEGER NOT NULL, -- Diastolic mm Hg "
                   "PRIMARY KEY (id, orientation)"
               ");"
    );
    query.exec("CREATE TABLE IF NOT EXISTS HandReadings ("
                   "id INTEGER PRIMARY KEY, "
                   "LH1 INTEGER NOT NULL, "
                   "LH2 INTEGER NOT NULL, "
                   "LH3 INTEGER NOT NULL, "
                   "LH4 INTEGER NOT NULL, "
                   "LH5 INTEGER NOT NULL, "
                   "LH6 INTEGER NOT NULL, "
                   "RH1 INTEGER NOT NULL, "
                   "RH2 INTEGER NOT NULL, "
                   "RH3 INTEGER NOT NULL, "
                   "RH4 INTEGER NOT NULL, "
                   "RH5 INTEGER NOT NULL, "
                   "RH6 INTEGER NOT NULL, "
                   "FOREIGN KEY (id) REFERENCES Profiles(handReadingsID) ON DELETE CASCADE"
               ");"
    );
    query.exec("CREATE TABLE IF NOT EXISTS FootReadings ("
                   "id INTEGER PRIMARY KEY, "
                   "LF1 INTEGER NOT NULL, "
                   "LF2 INTEGER NOT NULL, "
                   "LF3 INTEGER NOT NULL, "
                   "LF4 INTEGER NOT NULL, "
                   "LF5 INTEGER NOT NULL, "
                   "LF6 INTEGER NOT NULL, "
                   "RF1 INTEGER NOT NULL, "
                   "RF2 INTEGER NOT NULL, "
                   "RF3 INTEGER NOT NULL, "
                   "RF4 INTEGER NOT NULL, "
                   "RF5 INTEGER NOT NULL, "
                   "RF6 INTEGER NOT NULL, "
                   "FOREIGN KEY (id) REFERENCES Profiles(footReadingsID) ON DELETE CASCADE"
               ");"
    );
    qInfo() <<"Initialized Database.";
    return true;
}

int DBManager::getLastInsertId(QSqlQuery& query) {
    int lastId = query.lastInsertId().toInt();
    if (lastId != 0)
        qWarning() << "Last inserted ID:" << lastId;
    else
        qWarning() << "Failed to retrieve last inserted ID";
    return lastId;
}

Profile* DBManager::createProfile(const QString& fname, const QString& lname,
                                  float weight, float height, const QString& bday)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Profiles (fname, lname, weight, height, birthDay) "
                  "VALUES (:fname, :lname, :weight, :height, :birthDay);");
    query.bindValue(":fname", fname);
    query.bindValue(":lname", lname);
    query.bindValue(":weight", weight);
    query.bindValue(":height", height);
    query.bindValue(":birthDay", bday);

    if (!query.exec()) {
        qWarning() << "Error creating profile: " << query.lastError().text();
        return nullptr;
    }
    int lastId = getLastInsertId(query);
    return new Profile(lastId, fname, lname, weight, height, bday, nullptr);
}

bool DBManager::deleteProfile(int id)
{
    QSqlQuery query;
    // Query cascades deletion to Snapshot table
    query.prepare("DELETE FROM Profiles WHERE id = :id;");
    query.bindValue(":id", id);

    if (query.exec()) {
        qWarning() << "Profile by ID:" << id << " and corresponding snapshots deleted successfully.";
        return true;
    }
    else {
        qWarning() << "Failed to delete profile:" << query.lastError().text();
        return false;
    }
}

bool DBManager::updateProfile(Profile* prof, const QString& newFname, const QString& newLname,
                              float newWeight, float newHeight, const QString& newBday)
{
    QSqlQuery query;
    query.prepare("UPDATE Profiles SET fname = :fname, lname = :lname, weight = :weight, "
                  "height = :height, birthDay = :birthDay "
                  "WHERE id = :id;");
    query.bindValue(":fname", newFname);
    query.bindValue(":lname", newLname);
    query.bindValue(":weight", newWeight);
    query.bindValue(":height", newHeight);
    query.bindValue(":birthDay", newBday);
    query.bindValue(":id", prof->getId());

    if (!query.exec()) {
        qWarning() << "ERR: Could not update Profile: " << query.lastError().text();
        return false;
    }
    prof->setFname(newFname);
    prof->setLname(newLname);
    prof->setWeight(newWeight);
    prof->setHeight(newHeight);
    prof->setBday(newBday);

    return true;
}

// Create HandReadings
bool DBManager::createHandReadings(int& lastId, const QVector<int>& readings)
{
    if (readings.size() != 12) {
        qWarning() << "Hand reading must have exactly 12 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO HandReadings (LH1, LH2, LH3, LH4, LH5, LH6, RH1, RH2, RH3, RH4, RH5, RH6) "
                  "VALUES (:LH1, :LH2, :LH3, :LH4, :LH5, :LH6, :RH1, :RH2, :RH3, :RH4, :RH5, :RH6);");

    query.bindValue(":LH1", readings.at(0));
    query.bindValue(":RH1", readings.at(1));
    query.bindValue(":LH2", readings.at(2));
    query.bindValue(":RH2", readings.at(3));
    query.bindValue(":LH3", readings.at(4));
    query.bindValue(":RH3", readings.at(5));
    query.bindValue(":LH4", readings.at(6));
    query.bindValue(":RH4", readings.at(7));
    query.bindValue(":LH5", readings.at(8));
    query.bindValue(":RH5", readings.at(9));
    query.bindValue(":LH6", readings.at(10));
    query.bindValue(":RH6", readings.at(11));

    if (!query.exec()) {
        qWarning() << "ERR: Could not insert hand readings: " << query.lastError().text();
        return false;
    }
    lastId = getLastInsertId(query);
    return true;
}
// Create FootReadings
bool DBManager::createFootReadings(int& lastId, const QVector<int>& readings)
{
    if (readings.size() != 12) {
        qWarning() << "Hand reading must have exactly 12 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO FootReadings (LF1, LF2, LF3, LF4, LF5, LF6, RF1, RF2, RF3, RF4, RF5, RF6) "
                  "VALUES (:LF1, :LF2, :LF3, :LF4, :LF5, :LF6, :RF1, :RF2, :RF3, :RF4, :RF5, :RF6);");

    query.bindValue(":LF1", readings.at(0));
    query.bindValue(":RF1", readings.at(1));
    query.bindValue(":LF2", readings.at(2));
    query.bindValue(":RF2", readings.at(3));
    query.bindValue(":LF3", readings.at(4));
    query.bindValue(":RF3", readings.at(5));
    query.bindValue(":LF4", readings.at(6));
    query.bindValue(":RF4", readings.at(7));
    query.bindValue(":LF5", readings.at(8));
    query.bindValue(":RF5", readings.at(9));
    query.bindValue(":LF6", readings.at(10));
    query.bindValue(":RF6", readings.at(11));

    if (!query.exec()) {
        qWarning() << "ERR: Could not insert foot readings: " << query.lastError().text();
        return false;
    }

    lastId = getLastInsertId(query);
    return true;
}

bool DBManager::getAllProfiles(QVector<Profile*>& profiles)
{
    // Create a query to retrieve all snapshots from the database
    QSqlQuery query("SELECT * FROM Profiles;");

    // Execute the query
    if (!query.exec()) {
        qWarning() << "ERR: Could not fetch profiles: " << query.lastError().text();
        return false;
    }
    // Iterate through the query result and populate the `snaps` vector
    while (query.next()) {
        // Create a new Snapshot object
        Profile* prof = new Profile();

        // Populate the Profile object with data from the query
        prof->setId(query.value("id").toInt());
        prof->setFname(query.value("fname").toString());
        prof->setLname(query.value("lname").toString());
        prof->setWeight(query.value("weight").toFloat());
        prof->setHeight(query.value("height").toFloat());
        prof->setBday(query.value("birthDay").toString());

        // Add the Snapshot object to the vector
        profiles.append(prof);
    }
    return true;
}

bool DBManager::getAllSnapshots(QVector<Snapshot*>& snaps)
{
    // Create a query to retrieve all snapshots from the database
    QSqlQuery query("SELECT * FROM Snapshots;");

    // Execute the query
    if (!query.exec()) {
        qWarning() << "ERR: Could not fetch Snapshots: " << query.lastError().text();
        return false;
    }

    // Iterate through the query result and populate the `snaps` vector
    while (query.next()) {
        // Create a new Snapshot object
        Snapshot* snap = new Snapshot();

        // Populate the Snapshot object with data from the query
        snap->setProfileID(query.value("profileID").toInt());
        snap->setTimestamp(query.value("timestamp").toString());
        snap->setBodyTemp(query.value("bodyTemp").toFloat());
        snap->setLeftHandPressReadId(0); // 0 would act as null
        snap->setRightHandPressReadId(0);
        snap->setHeartRate(query.value("heartRate").toInt());
        snap->setSleepHrs(query.value("sleepHrs").toInt());
        snap->setSleepMins(query.value("sleepMins").toInt());
        snap->setCurrWeight(query.value("currWeight").toFloat());
        snap->setNotes(query.value("notes").toString());
        snap->setHandReadingID(query.value("handReadingsID").toInt());
        snap->setFootReadingID(query.value("footReadingsID").toInt());
        snap->setDBM(this);

        // Add the Snapshot object to the vector
        snaps.append(snap);
    }
    return true;
}


bool DBManager::getAllSnapshotsOfUser(QVector<Snapshot*>& snaps, int userID)
{
    // Create a query to retrieve all snapshots from the database
    QSqlQuery query("SELECT * FROM Snapshots"
                    "WHERE profileID = :profileID;");

    query.bindValue(":profileID", userID);

    // Execute the query
    if (!query.exec()) {
        qWarning() << "ERR: Could not fetch Snapshots: " << query.lastError().text();
        return false;
    }

    // Iterate through the query result and populate the `snaps` vector
    while (query.next()) {
        // Create a new Snapshot object
        Snapshot* snap = new Snapshot();

        // Populate the Snapshot object with data from the query
        snap->setProfileID(query.value("profileID").toInt());
        snap->setTimestamp(query.value("timestamp").toString());
        snap->setBodyTemp(query.value("bodyTemp").toFloat());
        snap->setLeftHandPressReadId(0); // 0 would act as null
        snap->setRightHandPressReadId(0);
        snap->setHeartRate(query.value("heartRate").toInt());
        snap->setSleepHrs(query.value("sleepHrs").toInt());
        snap->setSleepMins(query.value("sleepMins").toInt());
        snap->setCurrWeight(query.value("currWeight").toFloat());
        snap->setNotes(query.value("notes").toString());
        snap->setHandReadingID(query.value("handReadingsID").toInt());
        snap->setFootReadingID(query.value("footReadingsID").toInt());

        // Add the Snapshot object to the vector
        snaps.append(snap);
    }
    return true;
}


bool DBManager::getSnapshotByUserAndTime(Snapshot& snap, int userID, const QString& timestamp)
{
    // Prepare the query to retrieve snapshot based on userID and timestamp (yyyy-MM-dd hh:mm)
    QSqlQuery query;
    query.prepare("SELECT * FROM Snapshots "
                  "WHERE profileID = :profileID AND timestamp = :timestamp;");

    query.bindValue(":profileID", userID);
    query.bindValue(":timestamp", timestamp);

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Error fetching snapshot: " << query.lastError().text();
        return false;  // Handle error appropriately
    }

    // If the snapshot exists, populate the Snapshot object
    if (query.next()) {
        snap.setProfileID(query.value("profileID").toInt());
        snap.setTimestamp(query.value("timestamp").toString());
        snap.setBodyTemp(query.value("bodyTemp").toFloat());
        snap.setLeftHandPressReadId(0);
        snap.setRightHandPressReadId(0);
        snap.setHeartRate(query.value("heartRate").toInt());
        snap.setSleepHrs(query.value("sleepHrs").toInt());
        snap.setSleepMins(query.value("sleepMins").toInt());
        snap.setCurrWeight(query.value("currWeight").toFloat());
        snap.setNotes(query.value("notes").toString());

        // Set the Hand and Leg readings
        snap.setHandReadingID(
            query.value("handReadingsID").isNull() ? -1 : query.value("handReadingsID").toInt()
            );
        snap.setFootReadingID(
            query.value("footReadingsID").isNull() ? -1 : query.value("footReadingsID").toInt()
            );
        return true;
    }
    return false;
}

QVector<int> DBManager::getHandReadingsById(int handReadingID)
{
    QVector<int> readings;

    // Prepare the query to fetch hand readings by handReadingsID
    QSqlQuery query;
    query.prepare("SELECT * FROM HandReadings WHERE id = :handReadingsID;");
    query.bindValue(":handReadingID", handReadingID);

    if (!query.exec()) {
        qWarning() << "Error fetching hand readings: " << query.lastError().text();
        return readings;  // Return an empty list on error
    }

    // If the readings exist, populate the list
    if (query.next()) {
        readings.push_back(query.value("LH1").toInt());
        readings.push_back(query.value("RH1").toInt());
        readings.push_back(query.value("LH2").toInt());
        readings.push_back(query.value("RH2").toInt());
        readings.push_back(query.value("LH3").toInt());
        readings.push_back(query.value("RH3").toInt());
        readings.push_back(query.value("LH4").toInt());
        readings.push_back(query.value("RH4").toInt());
        readings.push_back(query.value("LH5").toInt());
        readings.push_back(query.value("RH5").toInt());
        readings.push_back(query.value("LH6").toInt());
        readings.push_back(query.value("RH6").toInt());
    }
    return readings;
}
QVector<int> DBManager::getFootReadingsById(int footReadingsID)
{
    QVector<int> readings;

    // Prepare the query to fetch leg readings by footReadingsID
    QSqlQuery query;
    query.prepare("SELECT * FROM FootReadings WHERE id = :footReadingsID;");
    query.bindValue(":footReadingsID", footReadingsID);

    if (!query.exec()) {
        qWarning() << "Error fetching leg readings: " << query.lastError().text();
        return readings;  // Return an empty list on error
    }

    // If the readings exist, populate the list
    if (query.next()) {
        readings.push_back(query.value("LF1").toInt());
        readings.push_back(query.value("RF1").toInt());
        readings.push_back(query.value("LF2").toInt());
        readings.push_back(query.value("RF2").toInt());
        readings.push_back(query.value("LF3").toInt());
        readings.push_back(query.value("RF3").toInt());
        readings.push_back(query.value("LF4").toInt());
        readings.push_back(query.value("RF4").toInt());
        readings.push_back(query.value("LF5").toInt());
        readings.push_back(query.value("RF5").toInt());
        readings.push_back(query.value("LF6").toInt());
        readings.push_back(query.value("RF6").toInt());
    }
    return readings;
}

bool DBManager::addSnapshotToHistory(const Snapshot& snapshot)
{
    // Prepare the SQL query to insert a new snapshot
    QSqlQuery query;
    query.prepare("INSERT INTO Snapshots ("
                      "profileID, timestamp, bodyTemp, "
                      "heartRate, sleepHrs, sleepMins, currWeight, notes, "
                      "handReadingsID, footReadingsID"
                  ") VALUES ("
                      ":profileID, :timestamp, :bodyTemp, "
                      ":heartRate, :sleepHrs, :sleepMins, :currWeight, :notes, "
                      ":handReadingsID, :footReadingsID"
                  ");"
    );

    // Bind the values from the Snapshot object to the query parameters
    query.bindValue(":profileID", snapshot.getProfileID());
    query.bindValue(":timestamp", snapshot.getTimestamp());  // Full timestamp (yyyy-MM-dd hh:mm)
    query.bindValue(":bodyTemp", snapshot.getBodyTemp());
    query.bindValue(":heartRate", snapshot.getHeartRate());
    query.bindValue(":sleepHrs", snapshot.getSleepHrs());
    query.bindValue(":sleepMins", snapshot.getSleepMins());
    query.bindValue(":currWeight", snapshot.getCurrWeight());
    query.bindValue(":notes", snapshot.getNotes());
    query.bindValue(":handReadingsID", snapshot.getHandReadingID());
    query.bindValue(":footReadingsID", snapshot.getFootReadingID());

    // Execute the query and check if it was successful
    if (!query.exec()) {
        qWarning() << "Failed to add new Snapshot: " << query.lastError().text();
        return false;  // Return false if there was an error
    }
    // Successfully inserted the snapshot
    return true;
}
