#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);

	virtual ~Actor();

	virtual void doSomething() = 0;

	virtual void annoy() = 0;

	bool getAlive();

	bool getVisible();

	int getImage();

	int getTickAmount();
	int getTicks();

	int getNumMoves();

	void setAlive(bool alive);

	void setMVisible(bool visibility);


	void setGone();

	void setImage(int imageNum);

	void setTickAmount();
	void setTicks();

	void increaseTick();
	void increaseTicks();

	void setDefaultNumMoves();

	void resetNumMoves();

	void decNumMoves();

	bool getStunned();

	void setStunned(bool stunned);

	int getHealth();

	void setHealth(int health);

	void decHealth(int num);

	void setAnnoyed(bool annoyed);

	bool getAnnoyed();

	bool getLeave();

	void setLeave(bool leave);

	double getRadius(double actor1X, double actor2X, double actor1Y, double actor2Y);

	StudentWorld* getWorld() const
	{
		return m_world;
	}


private:
	StudentWorld* m_world;
	bool m_Alive;
	bool m_Visible;
	bool m_permanent;
	bool m_TunnelManPickup;
	int m_actorImage;
	int m_TickAmount;
	int m_ticks;
	bool m_stunned;
	int m_health;
	bool m_gotAnnoyed;
	bool m_leaveField;
	int m_numSquaresToMoveInCurrentDirection;
	bool m_discovered[60][60];
};

class Earth : public Actor
{
public:
	Earth(StudentWorld* world, int startX, int startY, int imageID = TID_EARTH, Direction startDirection = right, double size = 0.25, unsigned int depth = 3);

	virtual ~Earth();

	virtual void doSomething();

	virtual void annoy();

};


class TunnelMan : public Actor
{
public:
	TunnelMan(StudentWorld* world, int imageID = TID_PLAYER, int startX = 30, int startY = 60, Direction startDirection = right, double size = 1.0, unsigned int depth = 0);

	virtual ~TunnelMan();

	virtual void doSomething();

	virtual void annoy();

};

class Protester : public Actor
{
public:
	Protester(StudentWorld* world, int imageID = TID_PROTESTER, int startX = 60, int startY = 60, Direction startDirection = left, double size = 1.0, unsigned int depth = 0);

	virtual ~Protester();

	virtual void doSomething();

	virtual void annoy();

};

class HardcoreProtester : public Actor
{
public:
	HardcoreProtester(StudentWorld* world, int imageID = TID_HARD_CORE_PROTESTER, int startX = 60, int startY = 60, Direction startDirection = left, double size = 1.0, unsigned int depth = 0);

	virtual ~HardcoreProtester();

	virtual void doSomething();

	virtual void annoy();
};


class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int startX, int startY, int imageID = TID_BOULDER, Direction startDirection = down, double size = 1.0, unsigned int depth = 1);

	virtual ~Boulder();

	virtual void doSomething();

	virtual void annoy();

	bool getStable();

	bool getWaiting();

	bool getFalling();

	void setStable();

	void setWaiting();

	void setFalling();


private:
	bool m_Stable;
	bool m_Waiting;
	bool m_Falling;

};


class Barrel : public Actor
{
public:
	Barrel(StudentWorld* world, int startX, int startY, int imageID = TID_BARREL, Direction startDirection = right, double size = 1.0, unsigned int depth = 2);

	virtual ~Barrel();

	virtual void doSomething();

	virtual void annoy();

};


class Gold : public Actor
{
public:
	Gold(bool visible, bool tunnelManPickUp, StudentWorld* world, int startX, int startY, int imageID = TID_GOLD, Direction startDirection = right, double size = 1.0, unsigned int depth = 2);

	virtual ~Gold();

	virtual void doSomething();

	virtual void annoy();

private:

	int ticksToStare;
	int ticksRemaining;
	bool tunnelManPickUp;
};


class Sonar : public Actor
{
public:
	Sonar(StudentWorld* world, int startX = 0, int startY = 60, int imageID = TID_SONAR, Direction startDirection = right, double size = 1.0, unsigned int depth = 2);

	virtual ~Sonar();

	virtual void doSomething();

	virtual void annoy();

	void setTicksRemaining(int ticks);
	int getTicksRemaining();

private:
	int ticksRemaining;

};

class Squirt : public Actor
{
public:
	Squirt(StudentWorld* world, int startX, int startY, int imageID = TID_WATER_SPURT, Direction startDirection = right, double size = 1.0, unsigned int depth = 1);

	virtual ~Squirt();

	virtual void doSomething();

	int getSquaresTraveled();
	void setSquaresTraveled(int squares);

	virtual void annoy();

private:

	int squaresTraveled;
};

class Water : public Actor
{
public:
	Water(StudentWorld* world, int startX, int startY, int imageID = TID_WATER_POOL, Direction startDirection = right, double size = 1.0, unsigned int depth = 2);

	virtual ~Water();

	virtual void doSomething();

	virtual void annoy();

	void setTicksRemaining(int ticks);
	int getTicksRemaining();

private:
	int ticksRemaining;
};


#endif // ACTOR_H_
