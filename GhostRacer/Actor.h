#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class GameWorld;
const int ALIVE = 1;
const int DEAD = 0;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
    Actor(int id, double x, double y, int di, int graphD, double size, int vS, int hS, char sta, int h, bool coll);
    virtual ~Actor(){return;}
    virtual void doSomething() = 0;
    int getVSpeed() const {return vSpeed;}
    int getHSpeed() const {return hSpeed;}
    void setState(int newState){state = newState;}
    int getState(){return state;}
    void setHit(int damage){hit -= damage;}
    void setVSpeed(int new_speed) {vSpeed = new_speed;}

private:
    int vSpeed;
    int hSpeed;
    int state;
    int hit;
    bool collAvoid;
};

class Pedestrian: public Actor
{
public:
};

class GhostRacer: public Actor
{
public:
    GhostRacer(int id, double x, double y, StudentWorld* ptr2sw);
    virtual void doSomething();
    void RacerMove();
private:
    int holySpray;
    StudentWorld* sw;
};

class Goodie: public Actor
{
public:
};

class BorderLine: public Actor
{
public:
    BorderLine(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
private:
    GhostRacer* GhostR;
};

#endif // ACTOR_H_
