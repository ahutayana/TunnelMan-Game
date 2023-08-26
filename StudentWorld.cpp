#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{

}

StudentWorld::~StudentWorld()
{
}

int StudentWorld::init()
{
	int current_level_number = getLevel();

	int B = min(current_level_number / 2 + 2, 9);//boulder
	setBoulders(B);
	while (m_actor.size() < B)
	{
		int randX = rand() % 61;
		int randY = rand() % 37 + 20;
		if (m_actor.size() == 0)
		{
			if (randX <= 29 - 4 || randX >= 34)
			{
				m_actor.push_back(new Boulder(this, randX, randY));
			}
		}
		else if (m_actor.size() > 0)
		{
			if (inRadius(randX, randY) == false)
			{
				m_actor.push_back(new Boulder(this, randX, randY));
			}
		}
	}



	for (int i = 0; i < 64; ++i)//earth
	{
		for (int j = 0; j < 60; ++j)
		{
			if (i > 29 && i < 34 && j > 3 && j < 60)
			{
				break;
			}
			m_earth[i][j] = new Earth(this, i, j);
		}
	}
	for (int k = 0; k < m_actor.size(); ++k)
	{
		int boulderX = m_actor[k]->getX();
		int boulderY = m_actor[k]->getY();
		for (int i = boulderX; i <= boulderX + 3; ++i)
		{
			for (int j = boulderY; j <= boulderY + 3; ++j)
			{
				if (m_earth[i][j] != nullptr)
				{
					delete m_earth[i][j];
					m_earth[i][j] = nullptr;
				}
			}
		}
	}



	int L = min(2 + current_level_number, 21);//barrels
	setOil(L);
	while (m_actor.size() < B + L)
	{
		int randX = rand() % 61;
		int randY = rand() % 57;
		if (inRadius(randX, randY) == false)
		{
			m_actor.push_back(new Barrel(this, randX, randY));
		}
	}

	int G = max(5 - current_level_number / 2, 2);//gold
	setGold(0);
	while (m_actor.size() < B + L + G)
	{
		int randX = rand() % 61;
		int randY = rand() % 57;
		if (inRadius(randX, randY) == false)
		{
			m_actor.push_back(new Gold(false, true, this, randX, randY));
		}
	}



	m_tunnelMan = new TunnelMan(this);//tunnelman

	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::inRadius(int randX, int randY) {
	bool inRadius = false;
	for (int i = 0; i < m_actor.size(); ++i)
	{
		double distance = sqrt(pow(randX - m_actor[i]->getX(), 2) + pow(randY - m_actor[i]->getY(), 2));
		if (distance <= 6)
		{
			inRadius = true;
		}
		if (randX > 29 - 5 && randX < 35 && randY > 1)
		{
			inRadius = true;
		}
	}

	return inRadius;
}






int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	int current_lives = m_lives;
	int current_level_number = getLevel();
	int T = max(25, 200 - current_level_number);
	int P = min(15, 2 + int(current_level_number * 1.5));
	++m_ticks;
	++m_tickDoSomething;
	++m_tickDoSomething2;
	int probabilityOfHardcore = min(90, current_level_number * 10 + 30);
	int randNum = rand() % 100 + 1;
	if (getTicks() == 1 && getProtesters() == 0)
	{
	  if (randNum < probabilityOfHardcore) {
	      m_actor.push_back(new HardcoreProtester(this));
	  }
	  else {
	      m_actor.push_back(new Protester(this));
	  }
	  setTicks();
	  ++m_protesters;
}
	randNum = rand() % 100 + 1;
	if (getTicks() >= T && getProtesters() < P)
	{
	  if (randNum < probabilityOfHardcore)
	  {
	     m_actor.push_back(new HardcoreProtester(this));
	  }
	  else
	  {
	      m_actor.push_back(new Protester(this));
	  }
	  setTicks();
	  ++m_protesters;
	}
	
	int A = getLevel() * 25 + 300;
	int randChance = rand() % A + 1;
	if (randChance == 1) {
		int randChance2 = rand() % 5 + 1;
		if (randChance2 == 1) {
			m_actor.push_back(new Sonar(this));
		}
		else {
			int currentSize = m_actor.size();
			while (m_actor.size() == currentSize) {
				int randX = rand() % 61;
				int randY = rand() % 61;
				if (checkEarthWater(randX, randY) == false) {
					m_actor.push_back(new Water(this, randX, randY));
				}
			}
		}
	}
	
	m_tunnelMan->doSomething();
	int flag = 0;
	for (int j = 0; j < m_actor.size(); ++j)
	{
		if (m_actor[j]->getImage() == TID_PROTESTER && m_actor[j]->getStunned() == true)
		{
			flag = 1;
		}
		if (m_actor[j]->getImage() == TID_PROTESTER && m_actor[j]->getLeave() == true)
		{
			flag = 2;
		}
	}
	if (flag == 0)
	{
		int ticksToWaitBetweenMoves = max(0, 3 - current_level_number / 4);
		if (getTicks2() >= ticksToWaitBetweenMoves)
		{
			setTicks2();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_PROTESTER)
				{
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0)
					{
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}

	if (flag == 1)
	{
		int ticksToWaitBetweenMoves = max(50, 100 - current_level_number * 10);
		if (getTicks2() >= ticksToWaitBetweenMoves)
		{
			setTicks2();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_PROTESTER)
				{
					m_actor[j]->setStunned(false);
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0) {
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}

	if (flag == 2)
	{
		int ticksToWaitBetweenMoves = 0;
		if (getTicks2() >= ticksToWaitBetweenMoves)
		{
			setTicks2();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_PROTESTER)
				{
					m_actor[j]->setStunned(false);
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0) {
						playSound(SOUND_FINISHED_LEVEL);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}

	int otherFlag = 0;
	for (int j = 0; j < m_actor.size(); ++j)
	{
		if (m_actor[j]->getImage() == TID_HARD_CORE_PROTESTER && m_actor[j]->getStunned() == true)
		{
			flag = 1;
		}
		if (m_actor[j]->getImage() == TID_HARD_CORE_PROTESTER && m_actor[j]->getLeave() == true)
		{
			flag = 2;
		}
	}
	if (otherFlag == 0)
	{
		int ticksToWaitBetweenMove = max(0, 3 - current_level_number / 4);
		if (getTicks2Hard() >= ticksToWaitBetweenMove)
		{
			setTicks2Hard();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_HARD_CORE_PROTESTER)
				{
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0) {
						playSound(SOUND_FINISHED_LEVEL);
						setHealth(100);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}

	if (otherFlag == 1)
	{
		int ticksToWaitBetweenMove = max(50, 100 - current_level_number * 10);
		if (getTicks2Hard() >= ticksToWaitBetweenMove)
		{
			setTicks2Hard();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_HARD_CORE_PROTESTER)
				{
					m_actor[j]->setStunned(false);
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0) {
						playSound(SOUND_FINISHED_LEVEL);
						setHealth(100);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}

	if (otherFlag == 2)
	{
		int ticksToWaitBetweenMove = 0;
		if (getTicks2Hard() >= ticksToWaitBetweenMove)
		{
			setTicks2Hard();
			for (int j = 0; j < m_actor.size(); ++j)
			{
				if (m_actor[j]->getAlive() == true && m_actor[j]->getImage() == TID_HARD_CORE_PROTESTER)
				{
					m_actor[j]->setStunned(false);
					m_actor[j]->doSomething();
					if (m_tunnelMan->getAlive() == false) {
						return GWSTATUS_PLAYER_DIED;
					}
					if (getOil() == 0) {
						playSound(SOUND_FINISHED_LEVEL);
						setHealth(100);
						return GWSTATUS_FINISHED_LEVEL;
					}
				}
			}
		}
	}



	for (int i = 0; i < m_actor.size(); ++i) {
		if (m_actor[i]->getAlive() == true && m_actor[i]->getImage() != TID_PROTESTER && m_actor[i]->getImage() != TID_HARD_CORE_PROTESTER)
		{
			m_actor[i]->doSomething();
			if (m_tunnelMan->getAlive() == false)
			{
				return GWSTATUS_PLAYER_DIED;
			}
			if (getOil() == 0) {
				playSound(SOUND_FINISHED_LEVEL);
				setHealth(100);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	removeDeadGameObjects();

	if (m_lives < current_lives)
	{
		return GWSTATUS_PLAYER_DIED;
	}

	if (m_tunnelMan->getAlive() == false) {
		return GWSTATUS_PLAYER_DIED;
	}
	if (getOil() == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		setHealth(100);
		return GWSTATUS_FINISHED_LEVEL;
	}

	updateDisplayText();

	return GWSTATUS_CONTINUE_GAME;
}






void StudentWorld::cleanUp()
{
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 60; ++j)
		{
			delete m_earth[i][j];
			m_earth[i][j] = nullptr;
		}
	}

	removeAllObjects();
	delete m_tunnelMan;
	m_protesters = 0;
	m_ticks = 0;
	m_tickDoSomething = 0;
	m_tickDoSomething2 = 0;
}

void StudentWorld::removeAllObjects()
{
	for (int i = 0; i < m_actor.size(); ++i)
	{
		m_actor[i]->setGone();
	}
	m_actor.clear();
}

void StudentWorld::removeDeadGameObjects()
{
	vector<Actor*>::iterator ptr;
	ptr = m_actor.begin();
	while (ptr != m_actor.end())
	{
		if ((*ptr)->getAlive() == false)
		{
			delete* ptr;
			ptr = m_actor.erase(ptr);
		}
		else
		{
			++ptr;
		}
	}
}




bool StudentWorld::deleteEarth(int x, int y)
{
	bool flagSound = false;
	for (int i = x; i <= x + 3; ++i)
	{
		for (int j = y; j <= y + 3; ++j)
		{
			if (m_earth[i][j] != nullptr)
			{
				delete m_earth[i][j];
				m_earth[i][j] = nullptr;
				flagSound = true;
			}
		}
	}
	if (flagSound)
	{
		return true;
	}
	return false;
}





int StudentWorld::getTunnelManX()
{
	return m_tunnelMan->getX();
}

int StudentWorld::getTunnelManY()
{
	return m_tunnelMan->getY();
}






void StudentWorld::updateDisplayText()
{
	string levelS = correctString(getLevel());

	string healthS = to_string(getHealth()) + "%";
	if (healthS.size() < 4)
		healthS = " " + healthS;

	string waterS = correctString(getWater());
	string goldS = correctString(getGold());
	string oilS = correctString(getOil());
	string sonarS = correctString(getSonar());

	string score = to_string(getScore());
	while (score.length() < 6) {
		score = "0" + score;
	}

	setGameStatText(
		"Lvl:" + levelS +
		"  Lives: " + to_string(getLives()) +
		"  Hlth: " + healthS +
		"  Wtr: " + waterS +
		"  Gld: " + goldS +
		"  Oil Left: " + oilS +
		"  Sonar: " + sonarS +
		"  Scr: " + score
	);
}





std::string StudentWorld::correctString(int status)
{
	std::string temp = to_string(status);
	if (temp.size() < 2)
		temp = " " + temp;

	return temp;
}





bool StudentWorld::CheckEarthDown(int X, int Y)
{
	bool flag = false;
	for (int i = X; i < X + 4; ++i)
	{
		if (m_earth[i][Y - 1] != nullptr)
		{
			flag = true;
		}
	}
	return flag;
}

bool StudentWorld::CheckEarthRight(int X, int Y)
{
	bool flag = false;
	for (int i = Y; i < Y + 4; ++i)
	{
		if (m_earth[X + 5][i] != nullptr)
		{
			flag = true;
		}
	}
	return flag;
}

bool StudentWorld::CheckEarthLeft(int X, int Y)
{
	bool flag = false;
	for (int i = Y; i < Y + 4; ++i)
	{
		if (m_earth[X - 1][i] != nullptr)
		{
			flag = true;
		}
	}
	return flag;
}

bool StudentWorld::CheckEarthUp(int X, int Y)
{
	bool flag = false;
	for (int i = X; i < X + 4; ++i)
	{
		if (m_earth[i][Y + 5] != nullptr)
		{
			flag = true;
		}
	}
	return flag;
}

bool StudentWorld::checkEarthWater(int x, int y)
{
	bool flag = false;
	for (int i = x; i < x + 4; ++i)
	{
		for (int j = y; j < y + 4; ++j) {
			if (m_earth[i][j] != nullptr)
			{
				flag = true;
			}
		}
	}
	return flag;
}

bool StudentWorld::checkEarthBelow(int x, int y)
{
	bool flag = false;
	for (int i = x; i < x + 4; ++i)
	{
		for (int j = y; j > getTunnelManY(); --j) {
			if (m_earth[i][j] != nullptr)
			{
				flag = true;
			}
		}
	}
	return flag;
}

bool StudentWorld::checkEarthAbove(int x, int y)
{
	bool flag = false;
	for (int i = x; i < x + 4; ++i)
	{
		for (int j = y; j < getTunnelManY(); ++j) {
			if (m_earth[i][j] != nullptr)
			{
				flag = true;
			}
		}
	}
	return flag;
}

bool StudentWorld::checkEarthRightMore(int x, int y)
{
	bool flag = false;
	for (int i = y; i < y + 4; ++i)
	{
		for (int j = x; j < getTunnelManX(); ++j) {
			if (m_earth[j][i] != nullptr)
			{
				flag = true;
			}
		}
	}
	return flag;
}

bool StudentWorld::checkEarthLeftMore(int x, int y)
{
	bool flag = false;
	for (int i = y; i < y + 4; ++i)
	{
		for (int j = x; j > getTunnelManX(); --j) {
			if (m_earth[j][i] != nullptr)
			{
				flag = true;
			}
		}
	}
	return flag;
}

bool StudentWorld::cantMove(int imageID, int moveX, int moveY)
{
	for (int i = 0; i < m_actor.size(); ++i)
	{
		if (m_actor[i]->getImage() == imageID)
		{
			double distance = m_tunnelMan->getRadius(moveX, m_actor[i]->getX(), moveY, m_actor[i]->getY());
			if (distance <= 3.00)
			{
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::addActor(Actor* actor)
{
	m_actor.push_back(actor);
}

void StudentWorld::sonarReveal()
{
	for (int i = 0; i < m_actor.size(); ++i)
	{
		double distance = sqrt(pow(getTunnelManX() - m_actor[i]->getX(), 2) + pow(getTunnelManY() - m_actor[i]->getY(), 2));
		if (distance <= 12)
		{
			m_actor[i]->setVisible(true);
		}
	}
}








void StudentWorld::setHealth(int health)
{
	this->m_health = health;
}

int StudentWorld::getHealth()
{
	return this->m_health;
}

void StudentWorld::setWater(int water)
{
	this->m_water = water;
}

int StudentWorld::getWater()
{
	return this->m_water;
}

void StudentWorld::setGold(int gold)
{
	this->m_gold = gold;
}

int StudentWorld::getGold()
{
	return this->m_gold;
}

void StudentWorld::setOil(int oil)
{
	this->m_oil = oil;
}

int StudentWorld::getOil()
{
	return this->m_oil;
}

void StudentWorld::setSonar(int sonar)
{
	this->m_sonar = sonar;
}

int StudentWorld::getSonar()
{
	return this->m_sonar;
}

void StudentWorld::setBoulders(int boulders)
{
	this->m_boulders = boulders;
}

int StudentWorld::getBoulders()
{
	return this->m_boulders;
}

void StudentWorld::setTicks()
{
	m_ticks = 0;
}

int StudentWorld::getTicks()
{
	return m_ticks;
}

void StudentWorld::setTicks2()
{
	m_tickDoSomething = 0;
}

int StudentWorld::getTicks2()
{
	return m_tickDoSomething;
}

void StudentWorld::setTicks2Hard()
{
	m_tickDoSomething2 = 0;
}

int StudentWorld::getTicks2Hard()
{
	return m_tickDoSomething2;
}

void StudentWorld::setProtesters()
{
	m_protesters = 0;
}

int StudentWorld::getProtesters()
{
	return m_protesters;
}

int StudentWorld::getLives()
{
	return this->m_lives;
}

void StudentWorld::setLives(int lives)
{
	this->m_lives = lives;
}

void StudentWorld::callAnnoy(int imageID)
{
	for (int i = 0; i < m_actor.size(); ++i)
	{
		if (m_actor[i]->getImage() == imageID)
		{
			m_actor[i]->annoy();
		}
	}
}

void StudentWorld::callTMAnnoy()
{
	m_tunnelMan->annoy();
}
