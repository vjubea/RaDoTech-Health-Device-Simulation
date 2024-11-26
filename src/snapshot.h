#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include<string>

class Snapshot
{
    /*
    Snapshots represent singular, completed scans. This is where any algorithmic analysis on raw data should take place.
     */


public:
    Snapshot(int id);

    std::string getExpertRecomendation();
};

#endif // SNAPSHOT_H
