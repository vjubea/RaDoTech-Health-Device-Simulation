#include "DBManager.h"
#include "profile.h"
#include "snapshot.h"
#include "handbloodpressure.h"
#include "handreadings.h"
#include "legreadings.h"

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

const QString DBManager::DATABASE_PATH = "path/to/database"; // Update with actual path

DBManager::DBManager() {
    // Initialize the database connection
    raDoTechDB = QSqlDatabase::addDatabase("QSQLITE");
    raDoTechDB.setDatabaseName(DATABASE_PATH);
    if (!raDoTechDB.open()) {
        qWarning() << "Database connection failed!";
    }
}

bool DBManager::DBInit() {
    // Ensures the database is initialized (create tables if they don't exist)
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Profiles ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "fname TEXT NOT NULL, "
               "lname TEXT NOT NULL, "
               "weight REAL NOT NULL, "
               "height REAL NOT NULL, "
               "birthDay TEXT NOT NULL, "
               "UNIQUE (fname, lname));"
    );
    query.exec("CREATE TABLE IF NOT EXISTS Snapshots ("
               "profileID INTEGER NOT NULL, "
               "timestamp TEXT NOT NULL, "
               "bodyTemp REAL NOT NULL, "
               "leftHandPressReadId INTEGER, "
               "rightHandPressReadId INTEGER, "
               "heartRate INTEGER NOT NULL, "
               "sleepHrs INTEGER NOT NULL, "
               "sleepMins INTEGER NOT NULL, "
               "currWeight REAL NOT NULL, "
               "notes TEXT, "
               "handReadingID INTEGER NOT NULL, "
               "legReadingID INTEGER NOT NULL, "
               "PRIMARY KEY (profileID, timestamp), "
               "FOREIGN KEY (profileID) REFERENCES Profiles(id) "
               "ON DELETE CASCADE);"
    );
    query.exec("CREATE TABLE IF NOT EXISTS HandBloodPressure("
               "id INTEGER NOT NULL AUTOINCREMENT, "
               "orientation CHAR(1) NOT NULL, -- “L” for left, “R” for right "
               "systolic INTEGER NOT NULL, -- Systolic mm Hg "
               "diastolic INTEGER NOT NULL, -- Diastolic mm Hg "
               "PRIMARY KEY (id, orientation));"
    );
    query.exec("CREATE TABLE IF NOT EXISTS HandReadings ("
               "id INTEGER NOT NULL AUTOINCREMENT, "
               "orientation CHAR(1) NOT NULL, "
               "H1 INTEGER NOT NULL, "
               "H2 INTEGER NOT NULL, "
               "H3 INTEGER NOT NULL, "
               "H4 INTEGER NOT NULL, "
               "H5 INTEGER NOT NULL, "
               "H6 INTEGER NOT NULL, "
               "PRIMARY KEY (id, orientation));"
    );
    query.exec("CREATE TABLE IF NOT EXISTS LegReadings ("
               "id INTEGER NOT NULL AUTOINCREMENT, "
               "orientation CHAR(1) NOT NULL, "
               "F1 INTEGER NOT NULL, "
               "F2 INTEGER NOT NULL, "
               "F3 INTEGER NOT NULL, "
               "F4 INTEGER NOT NULL, "
               "F5 INTEGER NOT NULL, "
               "F6 INTEGER NOT NULL, "
               "PRIMARY KEY (id, orientation));"
    );
    return true;
}

bool DBManager::createProfile(const QString& fname, const QString& lname, float weight, float height, const QString& bday) {
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
        return false;
    }

    return true;
}

bool DBManager::deleteProfile(const QString& fname, const QString& lname) {
    QSqlQuery query;
    query.prepare("DELETE FROM Profiles WHERE fname = :fname AND lname = :lname;");
    query.bindValue(":fname", fname);
    query.bindValue(":lname", lname);

    if (!query.exec()) {
        qWarning() << "Error deleting profile: " << query.lastError().text();
        return false;
    }

    return true;
}

bool DBManager::updateProfile(Profile* prof, const QString& newFname, const QString& newLname, float newWeight, float newHeight, const QString& newBday, const QString& newCountry) {
    QSqlQuery query;
    query.prepare("UPDATE Profiles SET fname = :fname, lname = :lname, weight = :weight, "
                  "height = :height, birthDay = :birthDay, country = :country "
                  "WHERE id = :id;");
    query.bindValue(":fname", newFname);
    query.bindValue(":lname", newLname);
    query.bindValue(":weight", newWeight);
    query.bindValue(":height", newHeight);
    query.bindValue(":birthDay", newBday);
    query.bindValue(":country", newCountry);
    query.bindValue(":id", prof->getId()); // Assuming Profile has a getId() method

    if (!query.exec()) {
        qWarning() << "Error updating profile: " << query.lastError().text();
        return false;
    }

    return true;
}
// Create HandReading
bool DBManager::createHandReading(int id, const QString& orientation, const QVector<int>& readings) {
    if (readings.size() != 6) {
        qWarning() << "Hand reading must have exactly 6 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO HandReadings (id, orientation, H1, H2, H3, H4, H5, H6) "
                  "VALUES (:id, :orientation, :H1, :H2, :H3, :H4, :H5, :H6);");
    query.bindValue(":id", id);
    query.bindValue(":orientation", orientation);
    query.bindValue(":H1", readings[0]);
    query.bindValue(":H2", readings[1]);
    query.bindValue(":H3", readings[2]);
    query.bindValue(":H4", readings[3]);
    query.bindValue(":H5", readings[4]);
    query.bindValue(":H6", readings[5]);

    if (!query.exec()) {
        qWarning() << "Error inserting hand reading: " << query.lastError().text();
        return false;
    }
    return true;
}
// Create LegReading
bool DBManager::createLegReading(int id, const QString& orientation, const QVector<int>& readings) {
    if (readings.size() != 6) {
        qWarning() << "Leg reading must have exactly 6 values!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO LegReadings(id, orientation, F1, F2, F3, F4, F5, F6) "
                  "VALUES (:id, :orientation, :F1, :F2, :F3, :F4, :F5, :F6);");
    query.bindValue(":id", id);
    query.bindValue(":orientation", orientation);
    query.bindValue(":F1", readings[0]);
    query.bindValue(":F2", readings[1]);
    query.bindValue(":F3", readings[2]);
    query.bindValue(":F4", readings[3]);
    query.bindValue(":F5", readings[4]);
    query.bindValue(":F6", readings[5]);

    if (!query.exec()) {
        qWarning() << "Error inserting foot reading: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::getAllSnapshots(QVector<Snapshot*>& snaps) {
    // Create a query to retrieve all snapshots from the database
    QSqlQuery query("SELECT * FROM Snapshots");

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Error fetching snapshots: " << query.lastError().text();
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
        snap->setLeftHandPressReadId(0); // To set to default NONE = 0
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

void DBManager::getSnapshotByUserAndDate(Snapshot& snap, int userID, const QString& date, int hour, int minute) {
    // Prepare the query to retrieve snapshot based on userID, date, hour, and minute
    QSqlQuery query;
    query.prepare("SELECT * FROM Snapshots "
                  "WHERE profileID = :profileID AND date = :date AND timestamp = :timestamp;");
    
    // Prepare the timestamp using the hour and minute (in the format HH:mm)
    QString timestamp = QString("%1 %2:%3")
        .arg(date)
        .arg(hour, 2, 10, QChar('0'))
        .arg(minute, 2, 10, QChar('0'));

    query.bindValue(":profileID", userID);
    query.bindValue(":date", date);
    query.bindValue(":timestamp", timestamp);

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Error fetching snapshot: " << query.lastError().text();
        return;  // Handle error appropriately
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
    }
}

QList<int> DBManager::getHandReadings(int handReadingID, char orientation) {
    QList<int> readings;

    // Prepare the query to fetch hand readings by handReadingID and orientation
    QSqlQuery query;
    query.prepare("SELECT * FROM HandReadings WHERE id = :handReadingID AND orientation = :orientation;");

    query.bindValue(":handReadingID", handReadingID);
    query.bindValue(":orientation", orientation);

    if (!query.exec()) {
        qWarning() << "Error fetching hand readings: " << query.lastError().text();
        return readings;  // Return an empty list on error
    }

    // If the readings exist, populate the list
    if (query.next()) {
        readings.append(query.value("H1").toInt());
        readings.append(query.value("H2").toInt());
        readings.append(query.value("H3").toInt());
        readings.append(query.value("H4").toInt());
        readings.append(query.value("H5").toInt());
        readings.append(query.value("H6").toInt());
    }

    return readings;
}
QList<int> DBManager::getLegReadings(int legReadingID, char orientation) {
    QList<int> readings;

    // Prepare the query to fetch leg readings by legReadingID and orientation
    QSqlQuery query;
    query.prepare("SELECT * FROM LegReadings WHERE id = :legReadingID AND orientation = :orientation;");

    query.bindValue(":legReadingID", legReadingID);
    query.bindValue(":orientation", orientation);

    if (!query.exec()) {
        qWarning() << "Error fetching leg readings: " << query.lastError().text();
        return readings;  // Return an empty list on error
    }

    // If the readings exist, populate the list
    if (query.next()) {
        readings.append(query.value("F1").toInt());
        readings.append(query.value("F2").toInt());
        readings.append(query.value("F3").toInt());
        readings.append(query.value("F4").toInt());
        readings.append(query.value("F5").toInt());
        readings.append(query.value("F6").toInt());
    }

    return readings;
}

bool DBManager::addNewSnapshot(const Snapshot& snapshot) {
    // Prepare the SQL query to insert a new snapshot
    QSqlQuery query;
    query.prepare("INSERT INTO Snapshots ("
                  "profileID, date, timestamp, "
                  "bodyTemp, leftHandPressReadId, rightHandPressReadId, "
                  "heartRate, sleepHrs, sleepMins, currWeight, notes, "
                  "handReadingID, legReadingID) "
                  "VALUES ("
                    ":profileID, :date, :timestamp, "
                    ":bodyTemp, :leftHandPressReadId, :rightHandPressReadId, "
                    ":heartRate, :sleepHrs, :sleepMins, :currWeight, :notes, "
                    ":handReadingID, :legReadingID);"
    );

    // Bind the values from the Snapshot object to the query parameters
    query.bindValue(":profileID", snapshot.getProfileID());
    query.bindValue(":date", snapshot.getTimestamp().left(10));  // Extract date part
    query.bindValue(":timestamp", snapshot.getTimestamp());  // Full timestamp (yyyy-MM-dd hh:mm)
    query.bindValue(":bodyTemp", snapshot.getBodyTemp());
    query.bindValue(":leftHandPressReadId", snapshot.getLeftHandPressReadId() == -1 ? QVariant::Null : snapshot.getLeftHandPressReadId());
    query.bindValue(":rightHandPressReadId", snapshot.getRightHandPressReadId() == -1 ? QVariant::Null : snapshot.getRightHandPressReadId());
    query.bindValue(":heartRate", snapshot.getHeartRate());
    query.bindValue(":sleepHrs", snapshot.getSleepHrs());
    query.bindValue(":sleepMins", snapshot.getSleepMins());
    query.bindValue(":currWeight", snapshot.getCurrWeight());
    query.bindValue(":notes", snapshot.getNotes());
    query.bindValue(":handReadingID", snapshot.getHandReadingID() == -1 ? QVariant::Null : snapshot.getHandReadingID());
    query.bindValue(":legReadingID", snapshot.getLegReadingID() == -1 ? QVariant::Null : snapshot.getLegReadingID());

    // Execute the query and check if it was successful
    if (!query.exec()) {
        qWarning() << "Failed to add new snapshot: " << query.lastError().text();
        return false;  // Return false if there was an error
    }

    // Successfully inserted the snapshot
    return true;
}