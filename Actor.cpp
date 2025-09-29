#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor (StudentWorld* sw, int imageID, int startX, int startY, int dir)
: GraphObject(imageID, startX, startY, dir), m_studentWorld(sw), m_isDead(false)
{
}

void Actor::doSomething() {
    if (isDead()) return;
}

StudentWorld* Actor:: swAddress() const{
    return m_studentWorld;
}

void Actor::setDeath() {
    m_isDead = true;
}

bool Actor::isDead() const{
    return m_isDead;
}

bool Actor::onPlayer (int x, int y) const {
    if (swAddress()->isAt(swAddress()->playerAddress(), x, y)) {
        return true;
    }
    return false;
}

//FLOOR
Floor :: Floor (StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_FLOOR, startX, startY, none)
{
}

//LADDER
Ladder :: Ladder (StudentWorld* sw, int startX, int startY)
:Actor(sw, IID_LADDER, startX, startY, none)
{
    setDirection(none);
}

//PLAYER
Player :: Player (StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_PLAYER, startX, startY), m_numBurps(0), m_JumpSeq(0), m_coolTime(0)
{
}

void Player :: doSomething () {
    Actor::doSomething();
    //if is currently jumping
    if (swAddress() -> isLadder(getX(), getY())) {
        m_JumpSeq = 0;
    }
    
    switch (m_JumpSeq) {
        case 1:
        case 2:
        case 3:
            int x;
            int y;
            getPositionInThisDirection(getDirection(), 1, x, y);
            if (!swAddress()->isFloor(x, y)) {
                moveTo(x, y);
            }
            
            m_JumpSeq ++;
            return;
        case 4:
            if (!swAddress()->isFloor(getX(), getY() - 1)) {
                moveTo(getX(), getY() - 1);
            }
            m_JumpSeq = 0;
            return;
            
        default:
            break;
    }
    //frozen
    if (m_coolTime > 0) {
        m_coolTime --;
        return;
    }
    
    //free fall
    if (swAddress()->freeFall(getX(), getY())) {
        moveTo(getX(), getY() - 1);
        return;
    }
    
    //press any key
    int key;
    if (swAddress()->getKey(key))
    {
        switch (key) {
            /*
             Left/Right Arrow Key Pressed:
            ■ If the Player is not already facing in this direction, change the Player's
            direction to the new direction.
            ■ Else if the Player is not blocked by a floor from moving in the specified
            direction, move the Player one square to the specified direction.
             */
            case KEY_PRESS_LEFT:
                if (getDirection() != left) {
                    setDirection(left);
                    break;
                }
                if (swAddress() -> isFloor(getX() - 1, getY())) break;
                else moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right) {
                    setDirection(right);
                    break;
                }
                if (swAddress() -> isFloor(getX() + 1, getY())) break;
                else moveTo(getX() + 1, getY());
                break;
            /*
             Up/Down Arrow Key Pressed:
             ■ If the Player is in a position to climb (e.g., on a ladder) in the specified
             direction and there is no floor blocking movement, update the Player's
             position accordingly.
            */
            case KEY_PRESS_UP:
                if (swAddress()->isLadder(getX(), getY()) && !swAddress() -> isFloor(getX(), getY()+1)){
                    moveTo(getX(), getY() + 1);
                }
                break;
                
            case KEY_PRESS_DOWN:
                if (swAddress()->isLadder(getX(), getY()) && !swAddress() -> isFloor(getX(), getY()-1)){
                    moveTo(getX(), getY() - 1);
                }
                else if (swAddress()->isLadder(getX(), getY()-1)){
                    moveTo(getX(), getY() - 1);
                }
                break;
                
            case KEY_PRESS_SPACE: //jump
                if ((swAddress() -> isLadder(getX(), getY()-1) ||
                    swAddress() -> isFloor(getX(), getY()-1) ||
                    swAddress() -> isLadder(getX(), getY()))){
                    //playsound
                    swAddress() -> playSound(SOUND_JUMP);
                    
                    if (!swAddress() -> isFloor(getX(), getY() + 1)) {
                        //1st step of a new jump sequence
                        moveTo(getX(), getY() + 1);
                        m_JumpSeq ++;
                    }
                }
                break;
                
            case KEY_PRESS_TAB:
                if (m_numBurps > 0) {
                    swAddress()->playSound(SOUND_BURP);
                    int x = 0;
                    int y = 0;
                    getPositionInThisDirection(getDirection(), 1, x, y);
                    swAddress() -> addBurp(x, y, getDirection());
                    m_numBurps--;
                }
                break;
                
            default:
                break;
        }
    }
    
    
}

int Player::getBurps() const {
    return m_numBurps;
}

void Player::incBurps(int howmuch) {
    m_numBurps += howmuch;
}

void Player::setCoolTime(int howmuch) {
    m_coolTime += howmuch;
}

//BONEFIRE
Bonefire :: Bonefire (StudentWorld* sw, int startX, int startY)
: Actor(sw, IID_BONFIRE, startX, startY, none)
{
}

void Bonefire::doSomething() {
    increaseAnimationNumber();
    swAddress()->burnAt(getX(), getY());
}

//--------------------------------------------------------------
//GOODIE
Goodie::Goodie (StudentWorld* sw, int imageID, int startX, int startY)
: Actor(sw, imageID, startX, startY, none)
{
}

void Goodie::doSomething() {
    Actor::doSomething();
    
    if (onPlayer(getX(), getY())) {
        powerUp();
        setDeath();
        swAddress()->playSound(SOUND_GOT_GOODIE);
    }
}

    //EXTRA LIFE GOODIE
lifeGoodie::lifeGoodie (StudentWorld* sw, int startX, int startY)
: Goodie(sw, IID_EXTRA_LIFE_GOODIE, startX, startY)
{
}

void lifeGoodie::powerUp() { //+50 pts, extra life
    swAddress()->increaseScore(50);
    swAddress()->incLives();
}
    //GARLIC GOODIE
garlicGoodie::garlicGoodie (StudentWorld* sw, int startX, int startY)
: Goodie(sw, IID_GARLIC_GOODIE, startX, startY)
{
}

void garlicGoodie::powerUp () { //+25  pts, +5 burps
    swAddress()->increaseScore(25);
    swAddress()->playerAddress()->incBurps(5);
}

//----------------------------------------------------------------
//ENEMY
Enemy::Enemy (StudentWorld* sw, int imageID, int startX, int startY)
: Actor(sw, imageID, startX, startY), m_countDown(0)
{
    int dir = randInt(0, 1);
    switch (dir) {
        case 0:
            setDirection(right);
            break;
        case 1:
            setDirection(left);
            break;
        default:
            break;
    }
}

bool Enemy::attack() {
    if (onPlayer(getX(), getY())) {
        swAddress()->playerAddress()->setDeath();
        return true;
    }
    return false;
}

void Enemy::doSomething() {
    Actor::doSomething();
    if (attack()) return;
    
    enemyOnly();
    if (m_countDown % 10 == 0) { //every 10 ticks
        m_countDown = 0; //reset countdown
        specialMove();
        if (attack()) return;
    }
    m_countDown ++;
}

void Enemy::changeDir_or_move(int x, int y) {
    getPositionInThisDirection(getDirection(), 1, x, y);
    if (swAddress()->isFloor(x, y)
        || swAddress()->freeFall(x, y)){
        setDirection((getDirection() + 180) % 360);
    }
    else moveTo(x, y);
}

    //FIREBALL
Fireball :: Fireball (StudentWorld* sw, int startX, int startY)
: Enemy(sw, IID_FIREBALL, startX, startY), m_climbState(none)
{
}
void Fireball::specialMove() {
    //a
    if (swAddress()->isLadder(getX(), getY()) &&
        !swAddress()->isFloor(getX(), getY() + 1) &&
        m_climbState != down){
        int r = randInt(1,3);
        if (m_climbState == up || r == 1){
            m_climbState = up;
            moveTo(getX(), getY() + 1);
            return;
        }
    }
    if (swAddress()->isLadder(getX(), getY() - 1) && m_climbState != up){
        int r = randInt(1, 3);
        if (m_climbState == down || r == 1){
             m_climbState = down;
             moveTo(getX(), getY() - 1);
             return;
         }
    }
    else
    {
        int x = 0;
        int y = 0;
        if (m_climbState != none){
            getPositionInThisDirection(m_climbState, 1, x, y);
            if ((swAddress()->isFloor(x, y) ||
                 !swAddress()->isLadder(getX(), getY()))){
                m_climbState = none;
            }
        }
        
        changeDir_or_move(x, y);
    }
}


    //KOOPA
Koopa :: Koopa (StudentWorld* sw, int startX, int startY)
: Enemy(sw, IID_KOOPA, startX, startY), m_cooldownTimer(0)
{
}

bool Koopa::attack() {
    if (onPlayer(getX(), getY()) && m_cooldownTimer == 0) {
        m_cooldownTimer = 50;
        swAddress()->playerAddress()->setCoolTime(50);
        return true;
    }
    return false;
}

void Koopa::specialMove() {
    int x = 0;
    int y = 0;
    
    changeDir_or_move(x, y);
}

void Koopa::enemyOnly() {
    if (m_cooldownTimer > 0) {
        m_cooldownTimer --;
    }
}
    //BARREL
Barrel :: Barrel (StudentWorld* sw, int startX, int startY, int dir)
: Enemy(sw, IID_BARREL, startX, startY), m_falling(false)
{
    switch (dir) {
        case 0:
            setDirection(right);
            break;
        case 180:
            setDirection(left);
            break;
        default:
            break;
    }
}

void Barrel::specialMove() {
    int x = 0;
    int y = 0;
    
    getPositionInThisDirection(getDirection(), 1, x, y);
    if (swAddress()->isFloor(x, y)) {
        setDirection((getDirection() + 180) % 360);
    }
    else moveTo(x, y);
}

void Barrel::enemyOnly() {
    if (!swAddress()->isFloor(getX(), getY() - 1)) {
        moveTo(getX(), getY() - 1);
        m_falling = true;
    }
    if (m_falling && swAddress()->isFloor(getX(), getY() - 1)) {
        m_falling = false;
        setDirection((getDirection() + 180) % 360);
    }
}

//-----------------------------------------------------------
//BURP
Burp::Burp (StudentWorld* sw, int startX, int startY, int dir)
: Actor(sw, IID_BURP, startX, startY, dir), m_life(5)
{
}

void Burp::doSomething(){
    if (isDead()) return;
    m_life --;
    if (m_life == 0) {
        setDeath();
        return;
    }
    swAddress()->attackEnemy(getX(), getY());
}

//KONG

Kong::Kong (StudentWorld* sw, int startX, int startY, int dir)
: Actor(sw, IID_KONG, startX, startY, dir), m_fleeState(false), m_ticksElapsed(0)
{
}

int Kong::calDistance(int x, int y) {
    return std::sqrt(std::pow(getX() - x, 2) + std::pow(getY() - y, 2));
}

void Kong::doSomething() {
    if (isDead()) return;
    increaseAnimationNumber();
    if (calDistance(swAddress()->playerAddress()->getX(), swAddress()->playerAddress()->getY()) <= 2) m_fleeState = true;
    if (!m_fleeState && m_ticksElapsed == std::max(200 - 50 * swAddress()->getLevel(), 50)) {
        int x = 0;
        int y = 0;
        getPositionInThisDirection(getDirection(), 1, x, y);
        swAddress()->addBarrel(x, y, getDirection());
        m_ticksElapsed = 0;
    }
    
    if (m_ticksElapsed % 5 == 0) {
        if (m_fleeState) {
            if (getY() == VIEW_HEIGHT - 1) {
                setDeath();
                swAddress()->setFinish();
            }
            moveTo(getX(), getY() + 1);
        }
    }
    m_ticksElapsed ++;
}
