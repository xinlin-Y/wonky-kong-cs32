#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld ();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isAt (Actor* ptr, int x, int y) const;
    Player* playerAddress () const;
    bool isFloor (int x, int y) const;
    void burnAt (int x, int y) ;
    bool isLadder (int x, int y) const;
    bool freeFall (int x, int y) const;
    void setFinish ();
    void addBarrel (int x, int y, int dir);
    void addBurp (int x, int y, int dir);
    void attackEnemy (int x, int y);
private:
    void deleteDead ();
    void updateDisplayText ();
    std::string generate_stats(int score, int level, int livesLeft, int burps);
    std::vector <Actor*> m_actor;
    Player* m_player;
    bool m_finish;
    
};

#endif // STUDENTWORLD_H_
