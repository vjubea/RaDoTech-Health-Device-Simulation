#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QVector>

#include "profile.h"
#include "snapshot.h"

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
 * - createHandReadings: adds a new hand reading to the database
 * - createLegReadings: adds a new leg reading to the database
 * - getHandReadings: retrieves a hand reading by ID and orientation
 * - getLegReadings: retrieves a leg reading by ID and orientation
 */

class DBManager {
public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm"; // not used yet
    static const QString DATABASE_PATH;

    DBManager();

    // Returns the autoincremented id of last insert operation
    int getLastInsertId(QSqlQuery& query);

    // Profile-related functions
    Profile* createProfile(const QString& fname, const QString& lname, float weight, float height, const QString& bday);
    bool deleteProfile(int id);
    bool getAllProfiles(QVector<Profile*>& profiles);
    bool updateProfile(Profile* prof, const QString& newFname, const QString& newLname, float newWeight, float newHeight, const QString& newBday);

    // Snapshot-related functions
    bool addSnapshotToHistory(const Snapshot& snapshot); // Create snapshot object, then pass as parameter
    bool getAllSnapshots(QVector<Snapshot*>& snaps); // Updates snaps to DB query result
    bool getSnapshotByUserAndDate(Snapshot& snap, int userID, const QString& timestamp); // Updates snap by DB query resuult

    // HandReadings and LegReadings-related functions
    bool createHandReadings(int& id, const char orientation, const QVector<int>& readings);
    bool createLegReadings(int& id, const char orientation, const QVector<int>& readings);
    QVector<int> getHReadingsByIdAndOrient(int id, const char orientation);
    QVector<int> getLReadingsByIdAndOrient(int id, const char orientation);

private:
    QSqlDatabase raDoTechDB;
    bool DBInit(); // Initializes the database if not already done
};

#endif // DBMANAGER_H
