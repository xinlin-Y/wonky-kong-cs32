#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//ACTOR
class Actor: public GraphObject {
public:
    Actor (StudentWorld* sw, int imageID, int startX, int startY, int dir = 0);
    virtual ~Actor () {};
    virtual void doSomething ();
    virtual bool isObstacle () const {return false;} //floor turn true
    virtual bool isLadder () const {return false;} //ladder turn true
    virtual bool isFireProof () const {return true;} //player, barrel turn false
    virtual bool isBurpProof () const {return true;} //enemy turn false
    StudentWorld* swAddress () const;
    bool onPlayer (int x, int y) const;
    void setDeath ();
    bool isDead () const;
    virtual bool addLifeGoodie () const {return false;}
    virtual bool addGarlic () const {return false;}
    
private:
    StudentWorld* m_studentWorld;
    bool m_isDead;
};
//------------------------------------------------

//FLOOR
class Floor : public Actor {
public:
    Floor (StudentWorld* sw, int startX, int startY);
    //doSomething did nothing as in Actor
    virtual bool isObstacle () const {return true;}//floor turn true
};

//LADDER
class Ladder : public Actor {
public:
    Ladder (StudentWorld* sw, int startX, int startY);
    //doSomething did nothing as in Actor
    virtual bool isLadder () const {return true;} //ladder turn true
};

//PLAYER
class Player : public Actor {
public:
    Player (StudentWorld* sw, int startX, int startY);
    virtual void doSomething ();
    int getBurps () const;
    void incBurps (int howmuch);
    virtual bool isFireProof () const {return false;}//player, barrel turn false
    void setCoolTime (int howmuch);
private:
    int m_numBurps;
    int m_JumpSeq;
    int m_coolTime;
    
};

//BONEFIRE
class Bonefire : public Actor {
public:
    Bonefire (StudentWorld* sw, int startX, int startY);
    virtual void doSomething ();
};

//--------------------------------------------------------

//GOODIE
class Goodie : public Actor {
public:
    Goodie (StudentWorld* sw, int imageID, int startX, int startY);
    virtual void doSomething ();
    virtual void powerUp ()=0;
};

    //EXTRA LIFE GOODIE
class lifeGoodie : public Goodie {
public:
    lifeGoodie (StudentWorld* sw, int startX, int startY);
    virtual void powerUp (); //+50 pts, extra life
};

    //GARLIC GOODIE
class garlicGoodie : public Goodie {
public:
    garlicGoodie (StudentWorld* sw, int startX, int startY);
    virtual void powerUp (); //+25  pts, +5 burps
};

//------------------------------------------------------------
//ENEMY
class Enemy : public Actor {
public:
    Enemy (StudentWorld* sw, int imageID, int startX, int startY);
    virtual bool isBurpProof () const {return false;}
    virtual bool attack ();
    virtual void doSomething ();
    virtual void specialMove() = 0;
    virtual void enemyOnly () {} //for dec Timer
    void changeDir_or_move (int x, int y);
private:
    int m_countDown;
};

    //Fireball
class Fireball : public Enemy {
public:
    Fireball (StudentWorld* sw, int startX, int startY);
    virtual void specialMove();
    virtual bool addGarlic () const {return true;}
private:
    int m_climbState;
};

    //Koopa
class Koopa : public Enemy {
public:
    Koopa (StudentWorld* sw, int startX, int startY);
    virtual bool addLifeGoodie () const {return true;}
    virtual bool attack(); //freez
    virtual void specialMove ();
    virtual void enemyOnly ();
private:
    int m_cooldownTimer;
};


 //Barrel
class Barrel : public Enemy {
public:
    Barrel (StudentWorld* sw, int startX, int startY, int dir);
    virtual bool isFireProof () const {return false;}
    virtual void specialMove ();
    virtual void enemyOnly ();
private:
    bool m_falling;
};

//-----------------------------------------------------------
//BURP
class Burp : public Actor {
public:
    Burp (StudentWorld* sw, int startX, int startY, int dir);
    virtual void doSomething ();
private:
    int m_life;
};

//KONG
class Kong : public Actor {
public:
    Kong (StudentWorld* sw, int startX, int startY, int dir);
    virtual void doSomething ();
private:
    int calDistance (int x, int y);
    bool m_fleeState;
    int m_ticksElapsed;
};




#endif // ACTOR_H_
