#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int id, double x, double y, int dir, double size, unsigned int graphD, int vS, int hS, int sta, int h, bool coll)
: GraphObject(id, x, y, dir, size, graphD)
{
    vSpeed = vS;
    hSpeed = hS;
    state = sta;
    hit = h;
    collAvoid = coll;
}

GhostRacer::GhostRacer(int id, double x, double y, StudentWorld* ptr2sw)
: Actor(id, x, y, 90, 4.0, 0, 0, 0, ALIVE, 100, true)
{
    sw = ptr2sw;
    holySpray = 10;
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
//    std::cout << delta_x << std::endl;
//    std::cout << cur_x << std::endl;
    moveTo(cur_x + delta_x, cur_y);
}


BorderLine::BorderLine(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 1, -4, 0, ALIVE, NULL, false)
{
    GhostR = ptr2gr;
}

void BorderLine::doSomething()
{
    int vert_speed = getVSpeed();
    vert_speed -= GhostR->getVSpeed();
    int horiz_speed = getHSpeed();
    int new_y = getY();
    new_y += vert_speed;
    int new_x = getX();
    new_x += horiz_speed;
    moveTo(new_x, new_y);
    if (new_x < 0 || new_y < 0 ||
        new_x > VIEW_WIDTH || new_y > VIEW_HEIGHT){
        setState(DEAD);
        return;
    }
}

Pedestrian::Pedestrian(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 0, -4, 0, ALIVE, 2, true)
{
    GhostR = ptr2gr;
}

void Pedestrian::doSomething()
{
    if (getState() != ALIVE){return;}
    if (overlap(this, GhostR)){
        
    }
}
