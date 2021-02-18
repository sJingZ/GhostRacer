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
    lastYellow = actors[actors.size()-1];
    int M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int j=0;j<M;j++){
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT),gr));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT), gr));
    }
    lastWhite = actors[actors.size()-1];
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (gr->getState() == ALIVE){
        gr->doSomething();
    }
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end()){
        if ((*it)->getState() == ALIVE){
            (*it)->doSomething();
            // if gr destroyed...
            // if gr completed level...
        }
        it++;
    }
    
    // remove dead actors
    while (it != actors.end()){
        if ((*it)->getState() == DEAD){
            it = actors.erase(it);
            // if gr destroyed...
            // if gr completed level...
        }
        else{
            it++;
        }
    }
    
    // add new actors
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - lastWhite->getY();
    if (delta_y >= SPRITE_HEIGHT){
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, gr));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, gr));
    }
    if (delta_y >= 4*SPRITE_HEIGHT){
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, gr));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, gr));
        lastWhite = actors[actors.size()-1];
    }
    // update text
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
