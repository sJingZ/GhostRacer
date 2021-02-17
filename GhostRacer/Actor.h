#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
    Actor(int id, double x, double y, int di, int graphD, double size, int vS, int hS, char sta, int h, bool coll);
    virtual ~Actor();
    virtual void doSomething();
    int getVSpeed() const {return vSpeed;}
    int getHSpeed() const {return hSpeed;}
    void setState(char newState){state = newState;}

private:
    int direction;
    int graphDep;
    double size;
    int vSpeed;
    int hSpeed;
    char state;
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
    GhostRacer(int id, double x, double y);
private:
    int holySpray;
};

class Goodie: public Actor
{
public:
};

class BorderLine: public Actor
{
public:
    BorderLine(int id, double x, double y);
    virtual void doSomething();
    
};

#endif // ACTOR_H_
