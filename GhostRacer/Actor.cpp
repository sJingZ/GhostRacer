#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int id, double x, double y, int dir, double size, unsigned int graphD, int vS, int hS, int sta, int h, bool coll, GhostRacer* gr)
: GraphObject(id, x, y, dir, size, graphD)
{
    vSpeed = vS;
    hSpeed = hS;
    state = sta;
    hit = h;
    collAvoid = coll;
    GhostR = gr;
}

GhostRacer::GhostRacer(int id, double x, double y, StudentWorld* ptr2sw)
: Actor(id, x, y, 90, 4.0, 0, 0, NULL, ALIVE, 100, true, nullptr)
{
    sw = ptr2sw;
    holySpray = 10;
    clc = true;
}

void GhostRacer::doSomething()
{
    if (getState() != ALIVE){return;}
    int center_x = getX();
    int racer_direction = getDirection();
    if (center_x <= ROAD_CENTER - ROAD_WIDTH/2){
        if (racer_direction > 90){
            setHit(10);
            setDirection(82);
            sw->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
    if (center_x >= ROAD_CENTER + ROAD_WIDTH/2){
        if (racer_direction < 90){
            setHit(10);
            setDirection(98);
            sw->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    int ch;
    int speed = getVSpeed();
    if(sw->getKey(ch)){
        switch (ch) {
            case KEY_PRESS_LEFT:
                // move counterclockwise
                if (racer_direction < 114){
                    setDirection(racer_direction + 8);
                }
                break;
            case KEY_PRESS_RIGHT:
                if (racer_direction > 66){
                    setDirection(racer_direction - 8);
                }
                break;
            case KEY_PRESS_SPACE:
                // add spray
                break;
            case KEY_PRESS_UP:
                // speed up
                if (speed < 5){
                    setVSpeed(speed+1);
                }
                break;
            case KEY_PRESS_DOWN:
                // slow down
                if (speed > -1){
                    setVSpeed(speed-1);
                }
                break;
            default:
                break;
        }
    }
    RacerMove();
}

void GhostRacer::RacerMove()
{
    double max_shift_per_tick = 4.0;
    double direction = getDirection();
    double delta_x = max_shift_per_tick * cos(direction*3.14159/180);
    double cur_x = getX();
    double cur_y = getY();
    
    sw->setLastWhite(-4-getVSpeed());
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin(){
    int new_direction;
    int prev_dir = getDirection();
    if (clc)
    {
        do {
        new_direction = prev_dir + randInt(5, 20);
        } while (new_direction < 60 || new_direction > 120);
        setDirection(new_direction);
            
    }
}

BorderLine::BorderLine(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 1, -4, 0, ALIVE, NULL, false, ptr2gr)
{
//    GhostR = ptr2gr;
}

void UpdatePos(Actor* a)
{
    int vert_speed = a->getVSpeed() - a->getGR()->getVSpeed();
    int horiz_speed = a->getHSpeed();
    int new_y = a->getY() + vert_speed;
    int new_x = a->getX() + horiz_speed;
    a->moveTo(new_x, new_y);
}

bool isOutOfBoud(Actor *a){
    if (a->getX() < 0 || a->getY() < 0 ||
        a->getX() > VIEW_WIDTH || a->getY() > VIEW_HEIGHT)
    {
        a->setState(DEAD);
        return true;
    }
    else {return false;}
}

void BorderLine::doSomething()
{
    UpdatePos(this);
    if (isOutOfBoud(this))
        return;
}

bool overlap(Actor &a, Actor &b){
    unsigned int delta_x = a.getX() - b.getX();
    unsigned int delta_y = a.getY() - b.getY();
    unsigned int radius_sum = a.getRadius() + b.getRadius();
    if (delta_x < radius_sum*0.25 && delta_y < radius_sum * 0.6){
        return true;
    }
    else{return false;}
}

Pedestrian::Pedestrian(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 0, -4, 0, ALIVE, 2, true, ptr2gr)
{
    MovePlan = 0;
}

void Pedestrian::PedMove(){
    UpdatePos(this);
    if (isOutOfBoud(this)){
        return;
    }
}

void Pedestrian::doSomething()
{
    if (getState() != ALIVE){return;}
    if (overlap(*this, *getGR())){
        setState(DEAD);
        getGR()->setState(DEAD);
        StudentWorld* SW = getGR()->getSW();
        SW->decHumanPed();
        return;
    }
    PedMove();
    MovePlan--;
    if (MovePlan > 0){return;}
    else{
        int new_hS;
        do {
            new_hS = randInt(-3, 3);
        } while (new_hS == 0);
        setHSpeed(new_hS);
        MovePlan = randInt(4, 32);
        if (new_hS < 0){setDirection(180);}
        else{setDirection(0);}
    }
}


OilSlick::OilSlick(int id, double x, double y, double si, GhostRacer* ptr2gr)
:Actor(id, x, y, 0, si, 1, -4, 0, ALIVE, 0, false, ptr2gr)
{
}

void OilSlick::doSomething(){
    UpdatePos(this);
    if (isOutOfBoud(this)){
        return;
    }
    if(overlap(*this, *getGR())){
        getGR()->getSW()->playSound(SOUND_OIL_SLICK);
        getGR()->spin();
    }
}
