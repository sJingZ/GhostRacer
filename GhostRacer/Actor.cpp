#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int id, double x, double y, int di, int graphD, double s, int vS, int hS, char sta, int h, bool coll)
: GraphObject(id, x, y)
{
    direction = di;
    graphDep = graphD;
    size = s;
    vSpeed = vS;
    hSpeed = hS;
    state = sta;
    hit = h;
    collAvoid = coll;
}

GhostRacer::GhostRacer(int id, double x, double y)
: Actor(id, x, y, 90, 0, 4.0, 0, 0, 'a', 100, true)
{
    holySpray = 10;
}


BorderLine::BorderLine(int id, double x, double y)
: Actor(id, x, y, 0, 1, 2.0, -4, 0, 'a', NULL, false)
{
    
}

void BorderLine::doSomething()
{
    int vert_speed = getVSpeed();
    int horiz_speed = getHSpeed();
    int new_y = getY();
    new_y += vert_speed;
    int new_x = getX();
    new_x += horiz_speed;
    moveTo(new_x, new_y);
    if (new_x < 0 || new_y < 0 ||
        new_x > VIEW_WIDTH || new_y > VIEW_HEIGHT){
        setState('d');
        return;
    }
}
