#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setLastWhite(int delta_y){lastWhite_y+=delta_y;}

private:
    GhostRacer* gr;
    std::vector<Actor *> actors;
    Actor* lastWhite;
    int lastWhite_y;
};

#endif // STUDENTWORLD_H_
