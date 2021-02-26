#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
using namespace std;

//////////////////////// ACTOR //////////////////////////////

Actor::Actor(int id, double x, double y, int dir, double size, unsigned int graphD, int vS, int hS, int sta, int h, bool coll, bool forSpray)
: GraphObject(id, x, y, dir, size, graphD)
{
    vSpeed = vS;
    hSpeed = hS;
    state = sta;
    hit = h;
    collAvoid = coll;
    canBeAffectedBySpray = forSpray;
}

//////////////////////// HELPER FUNCTIONS //////////////////////////////


void UpdatePos(Actor* a, GhostRacer* ptr2gr)
{
    int vert_speed = a->getVSpeed() - ptr2gr->getVSpeed();
    int horiz_speed = a->getHSpeed();
    int new_y = a->getY() + vert_speed;
    int new_x = a->getX() + horiz_speed;
    a->moveTo(new_x, new_y);
}

bool isOutOfBoud(Actor *a){
    if (a->getX() < 0 || a->getY() < 0 ||
        a->getX() > VIEW_WIDTH || a->getY() > VIEW_HEIGHT)
    {
        a->setDead();
        return true;
    }
    else {return false;}
}

bool overlap(Actor *a, Actor *b){
    int delta_x = (a->getX() - b->getX() >= 0) ? a->getX() - b->getX() : b->getX() - a->getX();
    int delta_y = (a->getY() - b->getY() >= 0) ? a->getY() - b->getY() : b->getY() - a->getY();
    int radius_sum = a->getRadius() + b->getRadius();
    if (delta_x < radius_sum*0.25 && delta_y < radius_sum * 0.6){
//        cout << "overlapped" << endl;
        return true;
    }
    else{return false;}
}


//////////////////////// GHOST RACER //////////////////////////////


GhostRacer::GhostRacer(int id, double x, double y, StudentWorld* ptr2sw)
: Actor(id, x, y, 90, 4.0, 0, 0, NULL, ALIVE, 100, true, false)
{
    sw = ptr2sw;
    holySpray = 10;
}

void GhostRacer::isDamaged(int sound, int damage){
    setHit(damage);
    if (getHit() > 0){
        getSW()->playSound(sound);
    }
    else{
        setDead();
        getSW()->playSound(SOUND_PLAYER_DIE);
    }
    
}


void GhostRacer::doSomething()
{
    if (getState() != ALIVE){return;}
    int center_x = getX();
    int racer_direction = getDirection();
    if (center_x <= ROAD_CENTER - ROAD_WIDTH/2){
        if (racer_direction > 90){
            isDamaged(SOUND_VEHICLE_CRASH, 10);
            setDirection(82);
//            setHit(10);
//            sw->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
    if (center_x >= ROAD_CENTER + ROAD_WIDTH/2){
        if (racer_direction < 90){
            isDamaged(SOUND_VEHICLE_CRASH, 10);
            setDirection(98);
//            setHit(10);
//            sw->playSound(SOUND_VEHICLE_CRASH);
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
            case KEY_PRESS_SPACE:{
                if (getHoly() > 0){
                    setHoly(-1);
                    double x = getX();
                    double y = getY();
                    double dir = getDirection();
                    Actor* s;
                    s = new Spray(IID_HOLY_WATER_PROJECTILE, x, y, dir, this);
                    getSW()->addNewActor(s);
                }
                break;
            }
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
//    cout << "direction is " << direction << endl;
    double delta_x = max_shift_per_tick * cos(direction*3.14159/180);
    double cur_x = getX();
    double cur_y = getY();
    
    sw->setLastWhite(-4-getVSpeed());
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin(){
    int new_direction;
    int prev_dir = getDirection();
    if (prev_dir <= 90)
    {
        do {
        new_direction = prev_dir + randInt(5, 20);
        } while (new_direction <= 60 || new_direction >= 120);
//        cout << "new direction is " << new_direction << endl;
        setDirection(new_direction);
    }
    else
    {
        do {
        new_direction = prev_dir - randInt(5, 20);
        } while (new_direction <= 60 || new_direction >= 120);
//        cout << "new direction is " << new_direction << endl;
        setDirection(new_direction);
    }
}

//////////////////////// BORDER LINE //////////////////////////////


BorderLine::BorderLine(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 2, -4, 0, ALIVE, NULL, false, false)
{
    GhostR = ptr2gr;
}



void BorderLine::doSomething()
{
    UpdatePos(this, GhostR);
        if (isOutOfBoud(this))
            return;
}

//////////////////////// HEALING GOODIE //////////////////////////////
HealingGoodie::HealingGoodie(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 1.0, 2, -4, 0, ALIVE, 0, false, true)
{
    GhostR = ptr2gr;
}

void HealingGoodie::doSomething(){
    UpdatePos(this, GhostR);
    if(isOutOfBoud(this)){return;}
    if(overlap(this, GhostR)){
        if (getHit() < 100){
            GhostR->setHit(-10); // get health level back by 10 points
        }
        setDead();
        GhostR->getSW()->playSound(SOUND_GOT_GOODIE);
        GhostR->getSW()->increaseScore(250);
    }
}

void HealingGoodie::affectedBySpray(){
    setDead();
}




//////////////////////// PED //////////////////////////////


Pedestrian::Pedestrian(int id, double x, double y, int dir, double size, unsigned int graphD, int vS, int hS, int sta, int h, bool coll, int mvplan, GhostRacer* ptr2gr)
: Actor(id, x, y, dir, size, graphD, vS, hS, sta, h, coll, true)
{
    MovePlan = mvplan;
    GhostR = ptr2gr;
}


void Pedestrian::PedMove(){
    UpdatePos(this, getGR());
    if (isOutOfBoud(this)){return;}
    if (MovePlan > 0){
        MovePlan--;
        return;
    }
    else{
        int new_hS;
        do {
            new_hS = randInt(-3, 3);
        } while (new_hS == 0);
        setHSpeed(new_hS);
        MovePlan = randInt(4, 32);
        if (new_hS < 0){setDirection(180);}
        else{setDirection(0);}
        return;
    }
}

//////////////////////// HUMAN PED //////////////////////////////

HumanPedestrian::HumanPedestrian(int id, double x, double y, GhostRacer* ptr2gr)
: Pedestrian(id, x, y, 0, 2.0, 0, -4, 0, ALIVE, 2, true, 0, ptr2gr)
{}

void HumanPedestrian::doSomething()
{
    if (getState() != ALIVE){return;}
    if (overlap(this, getGR())){
        getGR()->setDead();
        setDead();
        getGR()->getSW()->decHumanPed();
        return;
    }
    PedMove();
}

void HumanPedestrian::affectedBySpray(){
    setHSpeed(getHSpeed()*(-1));
    getDirection() == 180 ? setDirection(0) : setDirection(180);
    getGR()->getSW()->playSound(SOUND_PED_HURT);
}

//////////////////////// ZOOMBIE PED //////////////////////////////

ZoomPedestrian::ZoomPedestrian(int id, double x, double y, GhostRacer* ptr2gr)
: Pedestrian(id, x, y, 0, 3.0, 0, -4, 0, ALIVE, 2, true, 0, ptr2gr)
{
    tickBeforeGrunt = 20;
}

void ZoomPedestrian::isNearGR()
{
    int delta = getX();
    delta -= getGR()->getX();
    if (delta < -30 || delta > 30) {return;}
//    cout << "delta is " << delta << endl;
    setDirection(270);
    if (delta == 0)
    {setHSpeed(0);}
    else if (delta >= -30) // at left to GR
    {setHSpeed(1);}
    else if (delta <= 30)
    {setHSpeed(-1);}
    tickBeforeGrunt--;
    if (tickBeforeGrunt <= 0){
        getGR()->getSW()->playSound(SOUND_ZOMBIE_ATTACK);
        tickBeforeGrunt = 20;
    }
}

void ZoomPedestrian::affectedBySpray(){
    cout << "zoombie affected" << endl;
    setHit(1);
    // see if the zoombie has been killed by spray and is not killed by collision
    if (getHit() <= 0 && !overlap(this, getGR())){
        setDead();
        getGR()->getSW()->playSound(SOUND_PED_DIE);
        getGR()->getSW()->increaseScore(150);
        
        // add a new healing goodie
        int ChanceOfHealingGoodie = randInt(0, 5);
//        int ChanceOfHealingGoodie = 0;
        if (ChanceOfHealingGoodie == 0){
            Actor* healingGoodie = new HealingGoodie(IID_HEAL_GOODIE, getX(), getY(), getGR());
            getGR()->getSW()->addNewActor(healingGoodie);
        }
    }
}

void ZoomPedestrian::doSomething()
{
    if (getState() != ALIVE){return;}
    if (overlap(this, getGR())){
        getGR()->isDamaged(SOUND_PED_DIE, 5);
        setHit(2);
        setDead();
//        getGR()->setHit(5);
//        getGR()->getSW()->playSound(SOUND_PED_DIE);
        getGR()->getSW()->increaseScore(150);
        return;
    }
    
    // determine relative pos to racer
    isNearGR();
    PedMove();
}


//////////////////////// OIL SCLICK //////////////////////////////
OilSlick::OilSlick(int id, double x, double y, double si, GhostRacer* ptr2gr)
:Actor(id, x, y, 0, si, 1, -4, 0, ALIVE, 0, false, false)
{
    GhostR = ptr2gr;
}

void OilSlick::doSomething(){
    UpdatePos(this, GhostR);
    if (isOutOfBoud(this)){
        return;
    }
    if(overlap(this, GhostR)){
        GhostR->getSW()->playSound(SOUND_OIL_SLICK);
        GhostR->spin();
    }
}



//////////////////////// HOLY WATER //////////////////////////////
void HolyWater::affectedBySpray(){
    setDead();
}

HolyWater::HolyWater(int id, double x, double y, GhostRacer* ptr2gr)
:Actor(id, x, y, 90, 2, 2, -4, 0, ALIVE, 0, false, true)
{
    GhostR = ptr2gr;
}

void HolyWater::doSomething(){
    UpdatePos(this, GhostR);
    if (isOutOfBoud(this)){
        return;
    }
    if (overlap(this,GhostR)){
        GhostR->setHoly(10);
        setDead();
        GhostR->getSW()->playSound(SOUND_GOT_GOODIE);
        GhostR->getSW()->increaseScore(50);
    }
}

//////////////////////// SPRAY //////////////////////////////

Spray::Spray(int id, double x, double y, double dir, GhostRacer* ptr2gr)
:Actor(id, x, y, dir, 1.0, 1, 0, 0, ALIVE, 0, false, false)
{
    GhostR = ptr2gr;
    max_dis = 160;
    activated = false;
}

void Spray::isActivated(){
//    cout << "check activation" << endl;
    vector<Actor *> actors2Check = GhostR->getSW()->getActorsAffectedBySpray();
//    cout << "got actors 2 check" << endl;
    for (vector<Actor *>::iterator it = actors2Check.begin();
         it != actors2Check.end();
         it++){
        if(overlap(this, *it)){
            setDead();
            (*it)->affectedBySpray();
            return;
        }
    }
}

void Spray::doSomething(){
    if (getState() != ALIVE){return;}
    isActivated();
    moveForward(SPRITE_HEIGHT);
    if(isOutOfBoud(this)){
        return;
    }
    if (max_dis < 0){
        setDead();
        return;
    }
    max_dis -= SPRITE_HEIGHT;
}

//////////////////////// SOUL //////////////////////////////


Soul::Soul(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 4.0, 2, -4, 0, ALIVE, 0, false, false)
{
    GhostR = ptr2gr;
}

void Soul::doSomething(){
    UpdatePos(this, GhostR);
    if(isOutOfBoud(this)){
        return;
    }
    if(overlap(this, GhostR)){
        GhostR->getSW()->setSouls2Save(-1);
        setDead();
        GhostR->getSW()->playSound(SOUND_GOT_SOUL);
        GhostR->getSW()->increaseScore(100);
    }
//    cout << "new dir is " << getDirection() + 10 << endl;
    setDirection(getDirection() + 10);
}


//////////////////////// ZOOMBIE CAB //////////////////////////////
ZomCab::ZomCab(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 90, 4.0, 0, 0, 0, ALIVE, 3, true, true)
{
    GhostR = ptr2gr;
    hasHurtGR = false;
    planLength = 0;
}

void ZomCab::doSomething(){
    if(getState() != ALIVE){return;}
    if(!hasHurtGR && overlap(this, GhostR)){ // has not damaged the ghost racer and overlaps
        GhostR->isDamaged(SOUND_VEHICLE_CRASH, 20);
        int delta = getX() - GhostR->getX();
        if (delta <= 0){
            setHSpeed(-5);
            setDirection(60 - randInt(0, 20));
        }
        else{
            setHSpeed(5);
            setDirection(120 + randInt(0, 20));
        }
        hasHurtGR = true;
    }
    UpdatePos(this, GhostR);
    if (isOutOfBoud(this)){return;}
//    GhostR->getSW()->closestActorNearZomCab(this);
}



///////// THIS IS ONLY USED FOR TESTING ZOMBIE CAR DELETE LATER //////////////
forTestZomCab::forTestZomCab(int id, double x, double y, GhostRacer* ptr2gr)
: Actor(id, x, y, 0, 2.0, 0, -4, 0, ALIVE, 2, true, false)
{
    
}

