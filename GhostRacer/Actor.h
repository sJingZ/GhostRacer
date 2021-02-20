#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class GameWorld;
const int ALIVE = 1;
const int DEAD = 0;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class GhostRacer;

class Actor: public GraphObject
{
public:
    Actor(int id, double x, double y, int dir,  double size, unsigned int graphD, int vS, int hS, int sta, int h, bool coll, GhostRacer* gr);
    virtual ~Actor(){return;}
    virtual void doSomething() = 0;
    int getVSpeed() const {return vSpeed;}
    int getHSpeed() const {return hSpeed;}
    void setState(int newState){state = newState;}
    int getState(){return state;}
    void setHit(int damage){hit -= damage;}
    void setVSpeed(int new_speed) {vSpeed = new_speed;}
    void setHSpeed(int new_hSpeed) {hSpeed = new_hSpeed;}
    GhostRacer* getGR(){return GhostR;}

private:
    int vSpeed;
    int hSpeed;
    int state;
    int hit;
    bool collAvoid;
    GhostRacer* GhostR;
};

class GhostRacer: public Actor
{
public:
    GhostRacer(int id, double x, double y, StudentWorld* ptr2sw);
    virtual void doSomething();
    void RacerMove();
    StudentWorld* getSW(){return sw;}
    void spin();
    
private:
    int holySpray;
    StudentWorld* sw;
    bool clc;
};

class Pedestrian: public Actor
{
public:
    Pedestrian(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    void PedMove();
private:
//    GhostRacer* GhostR;
    int MovePlan;
};

class BorderLine: public Actor
{
public:
    BorderLine(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
private:
//    GhostRacer* GhostR;
};

class OilSlick: public Actor
{
public:
    OilSlick(int id, double x, double y, double si, GhostRacer* ptr2gr);
    virtual void doSomething();
private:
};


#endif // ACTOR_H_
