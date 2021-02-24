#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;

const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::decHumanPed(){
    PedDied = true;
}

void StudentWorld::addSpray(Actor *s){
    actors.push_back(s);
}

int StudentWorld::init()
{
    PedDied = false;
    souls2save = getLevel()*2 + 5;
    gr = new GhostRacer(IID_GHOST_RACER, 128, 32, this);
    bonus = 5000;

    // add border line
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int j=0;j<N;j++){
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE,j * SPRITE_HEIGHT, gr));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE,j * SPRITE_HEIGHT, gr));
    }
    int M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    for (int j=0;j<M;j++){
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT),gr));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3,j * (4*SPRITE_HEIGHT), gr));
    }
    lastWhite_y = actors[actors.size()-1]->getY();
    
//    cout << "at first, the actor size is " << actors.size() << endl;
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
            
            if (PedDied ||
                gr->getHit() <= 0){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            else if (souls2save <= 0){
                increaseScore(0);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
        it++;
    }
    
    
    // remove dead actors
    it = actors.begin();
    for (;it != actors.end();){
        if ((*it)->getState() == DEAD){
            delete *it;
            it = actors.erase(it);
            // if gr destroyed...
            // if gr completed level...
        }
        else{
            it++;
        }
    }
    
    // add new actors
    // Border line
    int L = getLevel();
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - lastWhite_y;
    if (delta_y >= SPRITE_HEIGHT){
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, gr));
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, gr));
    }
    if (delta_y >= 4*SPRITE_HEIGHT){
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, gr));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, gr));
        lastWhite_y = actors[actors.size()-1]->getY();
    }
    
    // Human Ped
    int ChanceHumanPed =  max(200 - L * 10, 30);
//    int randPed = 0;
    int randPed = randInt(0, ChanceHumanPed);
    if (randPed == 0){
        int randX = randInt(0, VIEW_WIDTH);
        actors.push_back(new HumanPedestrian(IID_HUMAN_PED, randX, VIEW_HEIGHT, gr));
    }
    
    // Zoobie Peds
    int ChanceZombiePed = max(100 - L*10, 20);
    int randZomPed = randInt(0, ChanceZombiePed);
    if(randZomPed == 0){
        int randX = randInt(0, VIEW_WIDTH);
        actors.push_back(new ZoomPedestrian(IID_ZOMBIE_PED, randX, VIEW_HEIGHT, gr));
    }
    
    // Oil Slicks
    int ChanceOilSlick = max(150-L*10, 40);
    int randOil = randInt(0, ChanceOilSlick);
//    int randOil = 0;
    if (randOil == 0){
        int randX = randInt(LEFT_EDGE, RIGHT_EDGE);
        int randSize = randInt(2, 5);
        actors.push_back(new OilSlick(IID_OIL_SLICK, randX, VIEW_HEIGHT,randSize, gr));
    }
    
    // Holy water
    int ChanceOfHolyWater = 100 + 10*L;
    int randHoly = randInt(0, ChanceOfHolyWater);
//    int randHoly = 0;
    if (randHoly == 0){
        int randX = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new HolyWater(IID_HOLY_WATER_GOODIE, randX, VIEW_HEIGHT, gr));
    }
    
    // Souls
    int ChanceOfLostSouls = 100;
    int randSouls = randInt(0, ChanceOfLostSouls);
    if (randSouls == 0){
        int randX = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new Soul(IID_SOUL_GOODIE, randX, VIEW_HEIGHT, gr));
    }

    // update text
    int score = getScore();
    int souls = getSouls();
    int livesLeft = getLives();
    int health = gr->getHit();
    int sprayAmt = gr->getHoly();
    int currBonus = getBonus();
    ostringstream oss;  // oss is a name of our choosing.
    oss.setf(ios::fixed);
    oss.precision(0);
    oss << "Score: " << score << " Lvl: " << L << " Souls2Save: " << souls << " Lives: " << livesLeft << " Health: " << health << " Sprays: " << sprayAmt << " Bonus: " << currBonus;
    string gameStatBar = oss.str();
    setGameStatText(gameStatBar);
    
    bonus--;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (gr){
        delete gr;
    }
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end()){
        delete *it;
        it = actors.erase(it);
    }
}
