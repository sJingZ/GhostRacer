#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    gr = new GhostRacer(IID_GHOST_RACER, 128, 32, this);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    for (int j=0;j<N;j++){
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE,j * SPRITE_HEIGHT, gr));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE,j * SPRITE_HEIGHT, gr));
    }
    
    int M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int j=0;j<M;j++){
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT),gr));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT), gr));
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (gr->getState() == ALIVE){
        gr->doSomething();
    }
//    vector<Actor *>::iterator it;
//    it = actors.begin();
//    while (it != actors.end()){
//        if ((*it)->getState() == ALIVE){
//            (*it)->doSomething();
//            // if gr destroyed...
//            // if gr completed level...
//        }
//    }
    // remove dead actor
    // add new actors
    // update text
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
