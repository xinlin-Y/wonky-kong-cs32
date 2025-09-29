#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_player(nullptr), m_finish(false)
{
}

StudentWorld :: ~StudentWorld () {
    cleanUp();
}

int StudentWorld::init()
{
    ostringstream currFilename;
    currFilename.fill('0');
    currFilename <<"level" << setw(2) << getLevel() << ".txt";
    
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currFilename.str());
    if (result == Level::load_fail_file_not_found || getLevel() > MAX_LEVELS) {
        cerr << "Could not find level00.txt data file\n";
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level\n";
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry me = lev.getContentsOf(x,y);
                switch (me)
                {
                    case Level::floor:
                        m_actor.push_back(new Floor (this, x, y));
                        break;
                    case Level::player:
                        m_player = new Player (this, x, y);
                        break;
                    case Level::ladder:
                        m_actor.push_back(new Ladder (this, x, y));
                        break;
                    case Level::left_kong:
                        m_actor.push_back(new Kong (this, x, y, 180));
                        break;
                    case Level::right_kong:
                        m_actor.push_back(new Kong (this, x, y, 0));
                        break;
                    case Level::fireball:
                        m_actor.push_back(new Fireball (this, x, y));
                        break;
                    case Level::koopa:
                        m_actor.push_back(new Koopa (this, x, y));
                        break;
                    case Level::bonfire:
                        m_actor.push_back(new Bonefire (this, x, y));
                        break;
                    case Level::extra_life:
                        m_actor.push_back(new lifeGoodie (this, x, y));
                        break;
                    case Level::garlic:
                        m_actor.push_back(new garlicGoodie (this, x, y));
                        break;
                    case Level::empty:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    updateDisplayText ();
    
    m_player -> doSomething();
    for (Actor* cur : m_actor) {
        cur->doSomething();
    }
    
    deleteDead();
    
    if (m_player->isDead()) {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    
    //is complete current level
    if (m_finish) {
        m_finish = false;
        increaseScore(1000);
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (!m_actor.empty()) {
        delete m_actor.back();
        m_actor.pop_back();
    }
    
    delete m_player;
    m_player = nullptr;
}

void StudentWorld :: updateDisplayText () {
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    unsigned int burps = m_player -> getBurps();

    // Next, create a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 03 Burps: 08
    
    string s = generate_stats(score, level, livesLeft, burps);
    
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld :: generate_stats(int score, int level, int livesLeft, int burps) {
    ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(7) << score << " Level: " << setw(2) << level << " Lives: " << setw(2) << livesLeft << " Burps: " << setw(2) << burps << endl;
    return oss.str();
}

bool StudentWorld::isAt(Actor* ptr, int x, int y) const {
    if (ptr->getX() == x && ptr->getY() == y) {
        return true;
    }
    return false;
}

Player* StudentWorld::playerAddress () const {
    return m_player;
}

bool StudentWorld::isFloor (int x, int y) const {
    for (Actor* curr : m_actor) {
        if (curr -> isObstacle()) {
            if (isAt(curr, x, y)) {
                return true; // Floor exists, blocked
            }
        }
    }
        return false; // not Blocked otherwise
}

void StudentWorld::burnAt(int x, int y)  { //bonfire
    //for barrel
    for (Actor* curr : m_actor) {
        if (! curr -> isFireProof()) {
            if (isAt(curr, x, y)) {
                curr -> setDeath();
            }
        }
    }
    
    //for player
    if (isAt(m_player, x, y)) {
        m_player->setDeath();
    }
}

bool StudentWorld::isLadder(int x, int y) const{
    for (Actor* curr : m_actor) {
        if (curr -> isLadder()) {
            if (isAt(curr, x, y)) {
                return true; // ladder
            }
        }
    }
    return false; // not ladder otherwise
}

void StudentWorld::deleteDead() {
    vector<Actor*>::iterator it;
    it = m_actor.begin();
    while (it != m_actor.end()){
        if ((*it)->isDead()){
            delete (*it);
            it = m_actor.erase(it);
            continue;
        }
        it++;
    }
}

bool StudentWorld::freeFall(int x, int y) const{
    if (isFloor(x, y - 1) || isLadder(x, y) || isLadder(x, y - 1)) {
        return false;
    }
    return true;
}

void StudentWorld::setFinish() {
    m_finish = true;
}

void StudentWorld::addBarrel(int x, int y, int dir) {
    m_actor.push_back(new Barrel (this, x, y, dir));
}

void StudentWorld::addBurp(int x, int y, int dir) {
    m_actor.push_back(new Burp (this, x, y, dir));
}

void StudentWorld::attackEnemy (int x, int y) {
    for (Actor* curr : m_actor) {
        if (! curr -> isBurpProof()) {
            if (isAt(curr, x, y)) {
                curr -> setDeath();
                playSound(SOUND_ENEMY_DIE);
                increaseScore(100);
                
                int r = randInt(1, 3);
                if (r == 1) {
                    if (curr -> addGarlic()) m_actor.push_back(new garlicGoodie (this, x, y));
                    if (curr -> addLifeGoodie()) m_actor.push_back(new lifeGoodie (this, x, y));
                }
            }
        }
    }
}
