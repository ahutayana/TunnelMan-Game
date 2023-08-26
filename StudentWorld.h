#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual ~StudentWorld();

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	bool deleteEarth(int x, int y);

	bool inRadius(int randX, int randY);

	int getTunnelManX();
	int getTunnelManY();


	void updateDisplayText();

	void setHealth(int health);
	int getHealth();

	void setWater(int water);
	int getWater();

	void setGold(int gold);
	int getGold();

	void setOil(int oil);
	int getOil();

	void setSonar(int sonar);
	int getSonar();


	void setBoulders(int boulders);
	int getBoulders();

	void setTicks();
	int getTicks();

	void setTicks2();
	int getTicks2();

	void setTicks2Hard();
	int getTicks2Hard();

	void setProtesters();
	int getProtesters();

	void removeDeadGameObjects();
	void removeAllObjects();

	bool CheckEarthDown(int X, int Y);
	bool CheckEarthRight(int X, int Y);
	bool CheckEarthLeft(int X, int Y);
	bool CheckEarthUp(int X, int Y);

	bool checkEarthBelow(int X, int Y);
	bool checkEarthAbove(int X, int Y);
	bool checkEarthRightMore(int X, int Y);
	bool checkEarthLeftMore(int X, int Y);
	
	bool cantMove(int imageID, int moveX, int moveY);

	void addActor(Actor* actor);

	bool checkEarthWater(int x, int y);

	void sonarReveal();

	int getLives();
	void setLives(int lives);

	void callAnnoy(int imageID);
	void callTMAnnoy();

private:

	int m_health = 100;
	int m_water = 5;
	int m_gold = 0;
	int m_oil = 2;
	int m_sonar = 1;
	int m_boulders;
	int m_ticks;
	int m_tickDoSomething;
	int m_tickDoSomething2;
	int m_protesters;
	int m_lives = 3;

	std::string correctString(int status);

	TunnelMan* m_tunnelMan;
	Earth* m_earth[64][64];
	std::vector<Actor*> m_actor;
};
#endif