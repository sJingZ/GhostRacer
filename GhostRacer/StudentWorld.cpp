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

////////////////// HELPER FUNCTIONS ////////////////////


void StudentWorld::decHumanPed(){
    PedDied = true;
}

void StudentWorld::addNewActor(Actor *act){
    actors.push_back(act);
}

std::vector<Actor *> StudentWorld::getActorsAffectedBySpray(){
    vector<Actor *> actorsCanBeAffected;
    vector<Actor *>::iterator ite;
    ite = actors.begin();
    while(ite != actors.end()){
        if ((*ite)->getIfAffectedBySpray()){
            actorsCanBeAffected.push_back(*ite);
//            (*ite)->printClass();
        }
        ite++;
    }
    return actorsCanBeAffected;
}

bool StudentWorld::isInSameLane(Actor* a, double leftOfLane){
    int rightOfLane = leftOfLane+ROAD_WIDTH/3; // compute the right edge from the left edge
    if (a->getCollAvoid() && // actor is collision-avoidance worthy
        a->getX() < rightOfLane && a->getX() > leftOfLane) // actor in the same lane
        {return true;}
    else
        {return false;}
}


Actor* StudentWorld::hasActorInFront(Actor* zomcab){
    Actor* closestActor = nullptr;
    
    vector<Actor *>::iterator it;
    it = actors.begin();
    int LeftOfZomLane = zomcab->getX()-ROAD_WIDTH/6; // left edge of zombie cab's lane
    
    while(it != actors.end()){
        if (isInSameLane(*it, LeftOfZomLane) &&
            (*it) != zomcab &&// and it is not the cab itself
            (*it) != gr && // and it is not the racer
            (*it)->getY() > zomcab->getY() && // the actor is in front of zombie cab
            (*it)->getY() < zomcab->getY()+96){ // but it is less than 96 pixels in front of zombie cab
            cout << "something in the same lane in front of zombie cab" << endl;
//            (*it)->printClass();
            return (*it);
        }
        it++;
    }
    return closestActor;
}

Actor* StudentWorld::hasActorAfter(Actor* zomcab){
    int leftOfZomCabLane = zomcab->getX()-ROAD_WIDTH/6;
    Actor* closestActor = nullptr;

    
    vector<Actor *>::iterator it;
    it = actors.begin();
//    int min_dist = VIEW_HEIGHT; // initialize the distance between actors
    
    while(it != actors.end()){
        if (isInSameLane(*it, leftOfZomCabLane) &&
            (*it) != zomcab && // and it is not the cab itself
            (*it) != gr && // and it is not the racer
            (*it)->getY() < zomcab->getY() && // the actor is after zombie cab
            (*it)->getY() > zomcab->getY()-96){ // but it is less than 96 pixels after zombie cab
            cout << "something in the same lane after zombie cab, and";
//            (*it)->printClass();
            return (*it);
        }
        it++;
    }
    return closestActor;
}

bool StudentWorld::hasActorAtBottomOfScreen(int leftOfLane){
    cout << "start find coll avoid actor at bottom" << endl;
    
    vector<Actor *>::iterator it;
    it = actors.begin();
    
    while (it != actors.end()){
        if (isInSameLane(*it, leftOfLane) &&
            (*it)->getY() <= (VIEW_HEIGHT / 3) // its Y cood is not greater than VIEW_HEIGHT / 3
            ){
            cout << "there is an actor before cab when cab is initiated" << endl;
//            (*it)->printClass();
//            if ((*it)->getY() < minDistanceToEdge){
//                minDistanceToEdge = (*it)->getY();
//                closestActor = *it;
            return true;
        }
        it++;
    }
    
    cout << "no actor at bottom" << endl;
    return false;
}

bool StudentWorld::hasActorAtTopOfScreen(int leftOfLane){
    cout << "start find coll avoid actor at top" << endl;
    
    
    // there is no need to check if ghost racer is in the lane since it always stays at the bottom
        
    vector<Actor *>::iterator it;
    it = actors.begin();
    
    while (it != actors.end()){
        if (isInSameLane(*it, leftOfLane) &&
            (*it)->getY() >= (VIEW_HEIGHT * 2/3) // its Y cood is less than (VIEW_HEIGHT * 2 / 3), meaning it is close to the top
            ){
            cout << "there is an actor after cab when cab is initiated" << endl;
//            (*it)->printClass();
            return true; // this lane is not a candidate lane
        }
        it++;
    }
    
    cout << "no actor at top" << endl;
    return false;
}


void StudentWorld::chooseLaneForZomCab(){
    
    cout << "start to choose a lane for zombie cab" << endl;
    int cur_lane = randInt(0, 2); // 0 is left, 1 is middle, 2 is right lane
    cout << "cur_lane is: " << cur_lane << endl;
    int randDelta = randInt(2, 4);
    switch (cur_lane) {
        case 0:{ // start from left lane
            for (int i=0; i<3; i++){
//                cout << "start left lane iter: " << i << endl;
                if(!hasActorAtBottomOfScreen(LEFT_EDGE + i*ROAD_WIDTH/3)){
                    // there is no such actor in candidate lane, or it has a Y greater than VIEW_HEIGHT/3
                    // found a spot for cab at bottom
//                    cout << "found a lane at bottom" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB, LEFT_EDGE + i*ROAD_WIDTH/3 + ROAD_WIDTH/6, SPRITE_HEIGHT/2, gr, gr->getVSpeed()+randDelta));
                    return;
                }
                
                if(!hasActorAtTopOfScreen(LEFT_EDGE + i*ROAD_WIDTH/3)){
                    // there is no such actor in candidate lane, or it has a Y less than VIEW_HEIGHT* 2/3
                    // found a spot for cab at top
//                    cout << "found a lane at top" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB, LEFT_EDGE + i*ROAD_WIDTH/3 + ROAD_WIDTH/6, VIEW_HEIGHT - SPRITE_HEIGHT/2, gr, gr->getVSpeed()-randDelta));
                    return;
                }
                
            }
            break;
        }
            
        case 1:{ // start from middle lane
            int lane[3] = {1,2,0}; // try middle first, then right, then left lane
            for (int i=0; i<3; i++){
//                cout << "start middle lane iter: " << i << endl;
                if(!hasActorAtBottomOfScreen(LEFT_EDGE + lane[i]*ROAD_WIDTH/3)){
                    // found a spot for cab at bottom
//                    cout << "found a lane at bottom" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB, LEFT_EDGE + lane[i]*ROAD_WIDTH/3 + ROAD_WIDTH/6, SPRITE_HEIGHT/2, gr, gr->getVSpeed()+randDelta));
                    return;
                }
                if(!hasActorAtTopOfScreen(LEFT_EDGE + lane[i]*ROAD_WIDTH/3)){
                    // found a spot for cab at top
//                    cout << "found a lane at top" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB, LEFT_EDGE + lane[i]*ROAD_WIDTH/3 + ROAD_WIDTH/6, VIEW_HEIGHT - SPRITE_HEIGHT / 2, gr, gr->getVSpeed()-randDelta));
                    return;
                }
            }
            break;
        }
            
        case 2:{ // start from right lane
            for (int i=2; i>=0; i--){
//                cout << "start right lane iter: " << i << endl;
                if(!hasActorAtBottomOfScreen(LEFT_EDGE + i*ROAD_WIDTH/3)){
                    // found a spot for cab at bottom
//                    cout << "found a lane at bottom" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB,LEFT_EDGE + i*ROAD_WIDTH/3 + ROAD_WIDTH/6, SPRITE_HEIGHT / 2, gr, gr->getVSpeed()+randDelta));
                    return;
                }
                if(!hasActorAtTopOfScreen(LEFT_EDGE + i*ROAD_WIDTH/3)){
                    // found a spot for cab at top
//                    cout << "found a lane at top" << endl;
                    actors.push_back(new ZomCab(IID_ZOMBIE_CAB,LEFT_EDGE + i*ROAD_WIDTH/3 + ROAD_WIDTH/6, VIEW_HEIGHT - SPRITE_HEIGHT / 2, gr, gr->getVSpeed()-randDelta));
                    return;
                }
            }
            break;
        }
        default:
            break;
    }
}


////////////////// INIT ////////////////////

int StudentWorld::init()
{
    PedDied = false;
    souls2save = getLevel()*2 + 5;
    gr = new GhostRacer(IID_GHOST_RACER, 128, 32, this);
    actors.push_back(gr);
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
    
    /////// ONLY USED FOR TESTING ZOMBIE CAB -- DELETE LATER /////////
//    actors.push_back(new forTestZomCab(IID_HUMAN_PED, 53 + ROAD_WIDTH/6 , 50, gr)); // put at left lane
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end()){
        if ((*it)->getState() == ALIVE){
            (*it)->doSomething();
            
            // check if racer is destroyed
            if (PedDied ||
                gr->getHit() <= 0){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            
            // check if racer has completed the level
            else if (souls2save <= 0){
                increaseScore(bonus);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
        it++;
    }
    
    
////////////////// REMOVE DEAD ACTORS ////////////////////
    it = actors.begin();
    for (;it != actors.end();){
        if ((*it)->getState() == DEAD){
            delete *it;
            it = actors.erase(it);
        }
        else{
            it++;
        }
    }
    
////////////////// ADD NEW ACTORS ////////////////////
    
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
    // int randPed = 0;
    int randPed = randInt(0, ChanceHumanPed);
    if (randPed == 0){
        int randHumanX = randInt(0, VIEW_WIDTH);
        actors.push_back(new HumanPedestrian(IID_HUMAN_PED, randHumanX, VIEW_HEIGHT, gr));
    }
    
    // Zoobie Peds
    int ChanceZombiePed = max(100 - L*10, 20);
    int randZomPed = randInt(0, ChanceZombiePed);
    if(randZomPed == 0){
        int randZoomX = randInt(0, VIEW_WIDTH);
        actors.push_back(new ZoomPedestrian(IID_ZOMBIE_PED, randZoomX, VIEW_HEIGHT, gr));
    }
    
    // Oil Slicks
    int ChanceOilSlick = max(150-L*10, 40);
    int randOil = randInt(0, ChanceOilSlick);
    // int randOil = 0;
    if (randOil == 0){
        int randOilX = randInt(LEFT_EDGE, RIGHT_EDGE);
        int randSize = randInt(2, 5);
        actors.push_back(new OilSlick(IID_OIL_SLICK, randOilX, VIEW_HEIGHT,randSize, gr));
    }
    
    // Holy water
    int ChanceOfHolyWater = 100 + 10*L;
    int randHoly = randInt(0, ChanceOfHolyWater);
    // int randHoly = 0;
    if (randHoly == 0){
        int randHolyX = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new HolyWater(IID_HOLY_WATER_GOODIE, randHolyX, VIEW_HEIGHT, gr));
    }
    
    // Souls
    int ChanceOfLostSouls = 100;
    int randSouls = randInt(0, ChanceOfLostSouls);
    if (randSouls == 0){
        int randSoulX = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new Soul(IID_SOUL_GOODIE, randSoulX, VIEW_HEIGHT, gr));
    }

    // Zombie cab
    int ChanceVehicle = max(100 - L*10, 20);
    int randZomCab = randInt(0, ChanceVehicle);
    if (randZomCab == 0){
        cout << "randzomcab = 0, so start to insert cab" << endl;
        chooseLaneForZomCab();
    }
    
////////////////// UPDATE TEXT ////////////////////
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
//    cout << "Tick ends //////////////////////////////////////////////" << endl;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end()){
        delete *it;
        it = actors.erase(it);
    }
}
