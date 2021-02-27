#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class GameWorld;
const int ALIVE = 1;
const int DEAD = 0;
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class GhostRacer;


//////////////// DELETE PRINT CLASS LATER /////////////////////////
class Actor: public GraphObject
{
public:
    Actor(int id, double x, double y, int dir, double size, unsigned int graphD, double vS, double hS, int sta, int h, bool coll, bool forSpray);
    virtual ~Actor(){return;}
    virtual void doSomething() = 0;
    double getVSpeed() const {return vSpeed;}
    double getHSpeed() const {return hSpeed;}
    void setDead(){state = DEAD;}
    int getState(){return state;}
    void setHit(int damage){hit -= damage;}
    void setVSpeed(double new_speed) {vSpeed = new_speed;}
    void setHSpeed(double new_hSpeed) {hSpeed = new_hSpeed;}
//    GhostRacer* getGR(){return GhostR;}
    int getHit(){return hit;}
    bool getIfAffectedBySpray(){return canBeAffectedBySpray;}
    virtual void affectedBySpray() = 0;
    bool getCollAvoid(){return collAvoid;}
//    virtual void printClass()=0;
    
private:
    double vSpeed;
    double hSpeed;
    int state;
    int hit;
    bool collAvoid;
    bool canBeAffectedBySpray;
//    GhostRacer* GhostR;
};

class GhostRacer: public Actor
{
public:
    GhostRacer(int id, double x, double y, StudentWorld* ptr2sw);
    virtual void doSomething();
    void RacerMove();
    StudentWorld* getSW(){return sw;}
    void spin();
    void setHoly(int delta){holySpray = holySpray + delta;}
    int getHoly(){return holySpray;}
    virtual void affectedBySpray(){};
    void isDamaged(int sound, int damage);
//    virtual void printClass(){std::cout << "this is a racer" << endl;}

private:
    int holySpray;
    StudentWorld* sw;
};

class Pedestrian: public Actor
{
public:
    Pedestrian(int id, double x, double y, int dir, double size, unsigned int graphD, double vS, double hS, int sta, int h, bool coll, int mvplan, GhostRacer* ptr2gr);
    virtual void doSomething() = 0;
    int getMovePlan(){return MovePlan;}
    GhostRacer* getGR(){return GhostR;}
    void PedMove();
    virtual void affectedBySpray() = 0;
//    virtual void printClass()=0;

private:
    GhostRacer* GhostR;
    int MovePlan;
};


class HumanPedestrian: public Pedestrian
{
public:
    HumanPedestrian(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    void HumanPedMove();
    virtual void affectedBySpray();
//    virtual void printClass(){cout << "this is a human ped" << endl;}

private:
//    int MovePlan;
};


class ZoomPedestrian: public Pedestrian
{
public:
    ZoomPedestrian(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    void ZoomPedMove();
    void isNearGR();
    virtual void affectedBySpray();
//    virtual void printClass(){cout << "this is a zombie ped" << endl;}


private:
    GhostRacer* GhostR;
//    int MovePlan;
    int tickBeforeGrunt;
};


class BorderLine: public Actor
{
public:
    BorderLine(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    virtual void affectedBySpray(){};
//    virtual void printClass(){cout << "this is a borderline" << endl;}

private:
    GhostRacer* GhostR;
};

class OilSlick: public Actor
{
public:
    OilSlick(int id, double x, double y, double si, GhostRacer* ptr2gr);
    virtual void doSomething();
    virtual void affectedBySpray(){};
//    virtual void printClass(){cout << "this is a oil slick" << endl;}

private:
    GhostRacer* GhostR;

};

class HolyWater: public Actor
{
public:
    HolyWater(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    virtual void affectedBySpray();
//    virtual void printClass(){cout << "this is a holy water" << endl;}
    
private:
    GhostRacer* GhostR;
};

class Spray: public Actor
{
public:
    Spray(int id, double x, double y, double dir, GhostRacer* ptr2gr);
    virtual void doSomething();
    void isActivated();
    void setActivation(){activated = true;}
    virtual void affectedBySpray(){};
//    virtual void printClass(){cout << "this is a spray" << endl;}
    
private:
    GhostRacer* GhostR;
    int max_dis;
    bool activated;
};


class Soul: public Actor
{
public:
    Soul(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    virtual void affectedBySpray(){};
//    virtual void printClass(){cout << "this is a soul" << endl;}

private:
    GhostRacer* GhostR;
};

class HealingGoodie: public Actor
{
public:
    HealingGoodie(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething();
    virtual void affectedBySpray();
//    virtual void printClass(){cout << "this is a healing goodie" << endl;}

private:
    GhostRacer* GhostR;
};

class ZomCab: public Actor
{
public:
    ZomCab(int id, double x, double y, GhostRacer* ptr2gr, double startSpeed);
    virtual void doSomething();
    virtual void affectedBySpray();
//    virtual void printClass(){cout << "this is a zombie car" << endl;}
    
private:
    GhostRacer* GhostR;
    bool hasHurtGR;
    int planLength;
};

class forTestZomCab: public Actor
{
public:
    forTestZomCab(int id, double x, double y, GhostRacer* ptr2gr);
    virtual void doSomething(){};
    virtual void affectedBySpray(){};
//    virtual void printClass(){cout << "this is a test figure for zombie car" << endl;}
    
private:
    GhostRacer* GhostR;
    bool hasHurtGR;
    int planLength;
};



#endif // ACTOR_H_
