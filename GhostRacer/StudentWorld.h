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
    void addNewActor(Actor* act);
    void setSouls2Save(int delta){souls2save += delta;}
    int getSouls(){return souls2save;}
    int getBonus(){return bonus;}
    std::vector<Actor *> getActorsAffectedBySpray();
    
    //// Functions for zombie cab ///////
    bool isInSameLane(Actor* a, double leftOfLane); // check if collision avoid actor in the same lane
    bool hasActorAtBottomOfScreen(int leftOfLane); // check if actor at bottom when creating cab
    bool hasActorAtTopOfScreen(int leftOfLane); // check if actor at top when creating cab
    Actor* hasActorInFront(Actor* zomcab); // check if actor in front of cab when cab running
    Actor* hasActorAfter(Actor* zomcab); // check if actor after cab when cab running
    void chooseLaneForZomCab(); // choose a lane and create a cab



    
private:
    GhostRacer* gr;
    std::vector<Actor *> actors;
    int lastWhite_y;
    bool PedDied;
    int souls2save;
    int bonus;
};

#endif // STUDENTWORLD_H_
