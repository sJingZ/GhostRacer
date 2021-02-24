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
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setLastWhite(int delta_y){lastWhite_y+=delta_y;}
    void decHumanPed();
    void addSpray(Actor* s);
    void setSouls2Save(int delta){souls2save += delta;}
    int getSouls(){return souls2save;}
    int getBonus(){return bonus;}

private:
    GhostRacer* gr;
    std::vector<Actor *> actors;
    int lastWhite_y;
    bool PedDied;
    int souls2save;
    int bonus;
};

#endif // STUDENTWORLD_H_
