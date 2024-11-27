#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QVector>

#include "profile.h"
#include "snapshot.h"
#include "handbloodpressure.h"
#include "handreadings.h"
#include "legreadings.h"

/* Class Purpose: Manages interactions between application and database
 *
 * Data Members:
 * - QSqlDatabase raDoTechDB: the database object
 * - QString DATE_FORMAT: formats of dates going in/out the database
 * - QString DATABASE_PATH: path of the database
 *
 * Class Functions:
 * Profile-related functions
 * - createProfile: adds a new profile to the database
 * - deleteProfile: deletes a profile from the database by fname and lname
 * - updateProfile: updates a profile in the database with new details
 *
 * Snapshot-related functions
 * - getAllSnapshots: retrieves all snapshots for a profile
 * - getSnapshotByUserAndDate: retrieves a snapshot based on userID, date, and time
 *
 * HandReadings and LegReadings-related functions
 * - createHandReading: adds a new hand reading to the database
 * - createLegReading: adds a new leg reading to the database
 * - getHandReading: retrieves a hand reading by ID and orientation
 * - getLegReading: retrieves a leg reading by ID and orientation
 */

class DBManager {
public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();

    // Profile-related functions
    bool createProfile(const QString& fname, const QString& lname, float weight, float height, const QString& bday);
    bool deleteProfile(const QString& fname, const QString& lname);
    bool updateProfile(Profile* prof, const QString& newFname, const QString& newLname, float newWeight, float newHeight, const QString& newBday);

    // Snapshot-related functions
    bool addNewSnapshot(const Snapshot& snapshot);
    bool getAllSnapshots(QVector<Snapshot*>& snaps);
    bool getSnapshotByUserAndDate(Snapshot& snap, int userID, const QString& timestamp);

    // HandReadings and LegReadings-related functions
    bool createHandReading(int id, const QString& orientation, const QVector<int>& readings);
    bool createLegReading(int id, const QString& orientation, const QVector<int>& readings);
    bool getHandReading(HandReadings& handReading, int id, const QString& orientation);
    bool getLegReading(LegReadings& legReading, int id, const QString& orientation);

private:
    QSqlDatabase raDoTechDB;

    bool DBInit(); // Initializes the database if not already done
};

#endif // DBMANAGER_H