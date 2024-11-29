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
    query.exec("DROP TABLE IF EXISTS LegReadings;");

    query.exec("CREATE TABLE IF NOT EXISTS Profiles ("
               "id INTEGER PRIMARY KEY, "
               "fname TEXT NOT NULL, "
               "lname TEXT NOT NULL, "
               "weight REAL NOT NULL, "
               "height REAL NOT NULL, "
               "birthDay TEXT NOT NULL, "
               "UNIQUE (fname, lname));"
    );
    query.exec("CREATE TABLE IF NOT EXISTS Snapshots ("
               "profileID INTEGER NOT NULL, "
               "timestamp VARCHAR(16) NOT NULL, --(yyyy-MM-dd hh:mm)"
               "bodyTemp REAL NOT NULL, "
               "leftHandPressReadID INTEGER DEFAULT 0, --can be null"
               "rightHandPressReadID INTEGER DEFAULT 0, --can be null"
               "heartRate INTEGER NOT NULL, "
               "sleepHrs INTEGER, --can be null"
               "sleepMins INTEGER, --can be null"
               "currWeight REAL NOT NULL, "
               "notes TEXT, "
               "handReadingsID INTEGER NOT NULL, "
               "legReadingsID INTEGER NOT NULL, "
               "PRIMARY KEY (profileID, timestamp), "
               "FOREIGN KEY (profileID) REFERENCES Profiles(id) ON DELETE CASCADE));"
    );
    //  To be added to Snapshot table creation in case blood pressure is handled:
    /*  "FOREIGN KEY (leftHandPressReadID) REFERENCES HandBloodPressure(id) "
        "    ON UPDATE CASCADE "
        "    ON DELETE SET NULL "
        "    CHECK (SELECT orientation FROM HandBloodPressure WHERE id = Snapshots.rightHandPressReadID) = 'L', "
        "FOREIGN KEY (rightHandPressReadID) REFERENCES HandBloodPressure(id) "
        "    ON UPDATE CASCADE "
        "    ON DELETE SET NULL "
        "    CHECK (SELECT orientation FROM HandBloodPressure WHERE id = Snapshots.rightHandPressReadID) = 'R');"
    */
    query.exec("CREATE TABLE IF NOT EXISTS HandBloodPressure("
               "id INTEGER NOT NULL, "
               "orientation CHAR(1) NOT NULL, -- “L” for left, “R” for right "
               "systolic INTEGER NOT NULL, -- Systolic mm Hg "
               "diastolic INTEGER NOT NULL, -- Diastolic mm Hg "
               "PRIMARY KEY (id, orientation), "
               "FOREIGN KEY (id) REFERENCES Profiles(legReadingsID) ON DELETE);"
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
               "FOREIGN KEY (id) REFERENCES Profiles(handReadingsID) ON DELETE);"
    );
    query.exec("CREATE TABLE IF NOT EXISTS LegReadings ("
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
               "FOREIGN KEY (id) REFERENCES Profiles(legReadingsID) ON DELETE);"
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
        return NULL;
    }
    int lastId = getLastInsertId(query);
    return new Profile(lastId, fname, lname, weight, height, bday);
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

// Create HandReading
bool DBManager::createHandReadings(int& lastId, const QVector<int>& readings)
{
    if (readings.size() != 12) {
        qWarning() << "Hand reading must have exactly 6 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO HandReadings (LH1, LH2, LH3, LH4, LH5, LH6, RH1, RH2, RH3, RH4, RH5, RH6) "
                  "VALUES (:LH1, :LH2, :LH3, :LH4, :LH5, :LH6, :RH1, :RH2, :RH3, :RH4, :RH5, :RH6);");

    query.bindValue(":LH1", readings[0]);
    query.bindValue(":RH1", readings[1]);
    query.bindValue(":LH2", readings[2]);
    query.bindValue(":RH2", readings[3]);
    query.bindValue(":LH3", readings[4]);
    query.bindValue(":RH3", readings[5]);
    query.bindValue(":LH4", readings[6]);
    query.bindValue(":RH4", readings[7]);
    query.bindValue(":LH5", readings[8]);
    query.bindValue(":RH5", readings[9]);
    query.bindValue(":LH6", readings[10]);
    query.bindValue(":RH6", readings[11]);

    if (!query.exec()) {
        qWarning() << "ERR: Could not insert hand readings: " << query.lastError().text();
        return false;
    }
    lastId = getLastInsertId(query);
    return true;
}
// Create LegReading
bool DBManager::createLegReadings(int& lastId, const QVector<int>& readings)
{
    if (readings.size() != 12) {
        qWarning() << "Hand reading must have exactly 6 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO LegReadings (LF1, LF2, LF3, LF4, LF5, LF6, RF1, RF2, RF3, RF4, RF5, RF6) "
                  "VALUES (:LF1, :LF2, :LF3, :LF4, :LF5, :LF6, :RF1, :RF2, :RF3, :RF4, :RF5, :RF6);");

    query.bindValue(":LF1", readings[0]);
    query.bindValue(":RF1", readings[1]);
    query.bindValue(":LF2", readings[2]);
    query.bindValue(":RF2", readings[3]);
    query.bindValue(":LF3", readings[4]);
    query.bindValue(":RF3", readings[5]);
    query.bindValue(":LF4", readings[6]);
    query.bindValue(":RF4", readings[7]);
    query.bindValue(":LF5", readings[8]);
    query.bindValue(":RF5", readings[9]);
    query.bindValue(":LF6", readings[10]);
    query.bindValue(":RF6", readings[11]);


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
        snap->setHandReadingID(query.value("handReadingID").toInt());
        snap->setLegReadingID(query.value("legReadingID").toInt());

        // Add the Snapshot object to the vector
        snaps.append(snap);
    }
    return true;
}

bool DBManager::getSnapshotByUserAndDate(Snapshot& snap, int userID, const QString& timestamp)
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
            query.value("handReadingID").isNull() ? -1 : query.value("handReadingID").toInt()
            );
        snap.setLegReadingID(
            query.value("legReadingID").isNull() ? -1 : query.value("legReadingID").toInt()
            );
        return true;
    }
    return false;
}

QVector<int> DBManager::getHandReadingsById(int handReadingID)
{
    QVector<int> readings;

    // Prepare the query to fetch hand readings by handReadingID
    QSqlQuery query;
    query.prepare("SELECT * FROM HandReadings WHERE id = :handReadingID;");
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
QVector<int> DBManager::getLegReadingsById(int legReadingID)
{
    QVector<int> readings;

    // Prepare the query to fetch leg readings by legReadingID
    QSqlQuery query;
    query.prepare("SELECT * FROM LegReadings WHERE id = :legReadingID;");
    query.bindValue(":legReadingID", legReadingID);

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
                  "profileID, timestamp, "
                  "bodyTemp, leftHandPressReadId, rightHandPressReadId, "
                  "heartRate, sleepHrs, sleepMins, currWeight, notes, "
                  "handReadingID, legReadingID) "
                  "VALUES ("
                    ":profileID, :timestamp, "
                    ":bodyTemp, :leftHandPressReadId, :rightHandPressReadId, "
                    ":heartRate, :sleepHrs, :sleepMins, :currWeight, :notes, "
                    ":handReadingID, :legReadingID);"
    );

    // Bind the values from the Snapshot object to the query parameters
    query.bindValue(":profileID", snapshot.getProfileID());
    query.bindValue(":timestamp", snapshot.getTimestamp());  // Full timestamp (yyyy-MM-dd hh:mm)
    query.bindValue(":bodyTemp", snapshot.getBodyTemp());
    query.bindValue(":leftHandPressReadId", snapshot.getLeftHandPressReadId());
    query.bindValue(":rightHandPressReadId", snapshot.getRightHandPressReadId());
    query.bindValue(":heartRate", snapshot.getHeartRate());
    query.bindValue(":sleepHrs", snapshot.getSleepHrs());
    query.bindValue(":sleepMins", snapshot.getSleepMins());
    query.bindValue(":currWeight", snapshot.getCurrWeight());
    query.bindValue(":notes", snapshot.getNotes());
    query.bindValue(":handReadingID", snapshot.getHandReadingID());
    query.bindValue(":legReadingID", snapshot.getLegReadingID());

    // Execute the query and check if it was successful
    if (!query.exec()) {
        qWarning() << "Failed to add new snapshot: " << query.lastError().text();
        return false;  // Return false if there was an error
    }
    // Successfully inserted the snapshot
    return true;
}
