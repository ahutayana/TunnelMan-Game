#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <cmath>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	m_world = world;
}

Actor::~Actor()
{

}

bool Actor::getAlive()
{
	return m_Alive;
}

bool Actor::getVisible()
{
	return m_Visible;
}

int Actor::getImage()
{
	return m_actorImage;
}

int Actor::getTickAmount()
{
	return m_TickAmount;
}

int Actor::getTicks()
{
	return m_ticks;
}

int Actor::getNumMoves()
{
	return m_numSquaresToMoveInCurrentDirection;
}

void Actor::setAlive(bool alive)
{
	m_Alive = alive;
}

void Actor::setMVisible(bool visibility)
{
	m_Visible = visibility;
}

void Actor::setGone()
{
	setAlive(false);
	setVisible(false);
	setMVisible(false);
}

void Actor::setImage(int imageNum)
{
	m_actorImage = imageNum;
}

void Actor::setTickAmount()
{
	m_TickAmount = 0;
}

void Actor::setTicks()
{
	m_ticks = 0;
}

void Actor::increaseTick()
{
	++m_TickAmount;
}

void Actor::increaseTicks()
{
	++m_ticks;
}

void Actor::setDefaultNumMoves()
{
	m_numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}

void Actor::resetNumMoves()
{
	m_numSquaresToMoveInCurrentDirection = 0;
}

void Actor::decNumMoves()
{
	--m_numSquaresToMoveInCurrentDirection;
}

bool Actor::getStunned()
{
	return m_stunned;
}

void Actor::setStunned(bool stunned)
{
	m_stunned = stunned;
}

int Actor::getHealth()
{
	return m_health;
}

void Actor::setHealth(int health)
{
	m_health = health;
}

void Actor::decHealth(int num)
{
	m_health -= num;
}

void Actor::setAnnoyed(bool annoyed)
{
	m_gotAnnoyed = annoyed;
}

bool Actor::getAnnoyed()
{
	return m_gotAnnoyed;
}

bool Actor::getLeave()
{
	return m_leaveField;
}

void Actor::setLeave(bool leave)
{
	m_leaveField = leave;
}

double Actor::getRadius(double actor1X, double actor2X, double actor1Y, double actor2Y)
{
	return sqrt(pow(actor1X - actor2X, 2) + pow(actor1Y - actor2Y, 2));
}

Earth::Earth(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setVisible(true);
}

Earth::~Earth()
{

}

void Earth::doSomething()
{
}

void Earth::annoy()
{

}





TunnelMan::TunnelMan(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);
	setHealth(10);
	setImage(imageID);
}

TunnelMan::~TunnelMan()
{

}

void TunnelMan::doSomething()
{
	if (getAlive() == false)
	{
		return;
	}
	if (getWorld()->deleteEarth(getX(), getY()))
	{
		getWorld()->playSound(SOUND_DIG);
	}
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			setGone();
			getWorld()->setLives(getWorld()->getLives() - 1);
			getWorld()->decLives();
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
			{
				setDirection(left);
				break;
			}
			if (getX() - 1 >= 0 && getWorld()->cantMove(TID_BOULDER, getX() - 1, getY()) == false)
			{
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
			{
				setDirection(right);
				break;
			}
			if (getX() + 1 <= 60 && getWorld()->cantMove(TID_BOULDER, getX() + 1, getY()) == false)
			{
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
			{
				setDirection(down);
				break;
			}
			if (getY() - 1 >= 0 && getWorld()->cantMove(TID_BOULDER, getX(), getY() - 1) == false)
			{
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
			{
				setDirection(up);
				break;
			}
			if (getY() + 1 <= 60 && getWorld()->cantMove(TID_BOULDER, getX(), getY() + 1) == false)
			{
				moveTo(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_SPACE:
			if (getWorld()->getWater() > 0) {
				getWorld()->setWater(getWorld()->getWater() - 1);
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				getWorld()->addActor(new Squirt(getWorld(), getX(), getY(), TID_WATER_SPURT, getDirection()));
			}
			break;
		case 'z':
		case 'Z':
			if (getWorld()->getSonar() > 0) {
				getWorld()->setSonar(getWorld()->getSonar() - 1);
				getWorld()->sonarReveal();
			}
			break;
		case KEY_PRESS_TAB:
			if (getWorld()->getGold() > 0) {
				getWorld()->setGold(getWorld()->getGold() - 1);
				getWorld()->addActor(new Gold(true, false, getWorld(), getX(), getY()));
			}
		}

		return;
	}
}

void TunnelMan::annoy()
{
	decHealth(2);
	getWorld()->setHealth(getWorld()->getHealth() - 20);
	if (getHealth() <= 0)
	{
		getWorld()->setHealth(100);
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		getWorld()->setLives(getWorld()->getLives() - 1);
		getWorld()->decLives();
		setGone();

	}
}



Protester::Protester(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);
	setLeave(false);
	setHealth(5);
	getNumMoves();
	setImage(imageID);
}

Protester::~Protester()
{
}

void Protester::doSomething()
{
	increaseTick();
	increaseTicks();
	int X = getX();
	int Y = getY();
	if (getAlive() == false)
	{
		return;
	}
	/*if (getHealth() == 0)
	{
		setLeave(true);
		if (X == 60 && Y == 60)
		{
			setGone();
		}
		else
		{
			setDiscovered(X, Y, false);
			if (getWorld()->CheckEarthUp(X, Y) == false)
			{

			}

		}
	}*/
	if (getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) <= 4)//yelling
	{
		if (X < getWorld()->getTunnelManX() && getDirection() == right && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;

		}
		else if (X > getWorld()->getTunnelManX() && getDirection() == left && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}
		else if (Y < getWorld()->getTunnelManY() && getDirection() == up && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}
		else if (Y > getWorld()->getTunnelManY() && getDirection() == down && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}

		if (X < getWorld()->getTunnelManX() && getDirection() == right && getTickAmount() < 15)
		{
			resetNumMoves();
			return;

		}
		else if (X > getWorld()->getTunnelManX() && getDirection() == left && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
		else if (Y < getWorld()->getTunnelManY() && getDirection() == up && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
		else if (Y > getWorld()->getTunnelManY() && getDirection() == down && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
	}
	if (Y == getWorld()->getTunnelManY() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && X < getWorld()->getTunnelManX() && getWorld()->checkEarthRightMore(X, Y) == false)
	{
		setDirection(right);
		moveTo(getX() + 1, getY());
		resetNumMoves();
		return;
	}
	if (Y == getWorld()->getTunnelManY() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && X > getWorld()->getTunnelManX() && getWorld()->checkEarthLeftMore(X, Y) == false)
	{
		setDirection(left);
		moveTo(getX() - 1, getY());
		resetNumMoves();
		return;
	}
	if (X == getWorld()->getTunnelManX() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && Y > getWorld()->getTunnelManY() && getWorld()->checkEarthBelow(X, Y) == false)
	{
		setDirection(down);
		moveTo(getX(), getY() - 1);
		resetNumMoves();
		return;
	}
	if (X == getWorld()->getTunnelManX() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && Y < getWorld()->getTunnelManY() && getWorld()->checkEarthAbove(X, Y) == false)
	{
		setDirection(up);
		moveTo(getX(), getY() + 1);
		resetNumMoves();
		return;
	}
	decNumMoves();
	bool flag = false;
	while (flag == false)
	{
		if (getNumMoves() <= 0)
		{
			int randDirection = rand() % 4;
			switch (randDirection)
			{
			case 0:
				if (getWorld()->CheckEarthUp(X, Y) == false)
				{
					flag = true;
					setDirection(up);
					setDefaultNumMoves();
				}
				break;
			case 1:
				if (getWorld()->CheckEarthDown(X, Y) == false)
				{
					flag = true;
					setDirection(down);
					setDefaultNumMoves();
				}
				break;
			case 2:
				if (getWorld()->CheckEarthRight(X, Y) == false)
				{
					flag = true;
					setDirection(right);
					setDefaultNumMoves();
				}
				break;
			case 3:
				if (getWorld()->CheckEarthLeft(X, Y) == false)
				{
					flag = true;
					setDirection(left);
					setDefaultNumMoves();
				}
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	if (flag == false)
	{
		if (getDirection() == left || getDirection() == right && getWorld()->CheckEarthUp(X, Y) == false || getWorld()->CheckEarthDown(X, Y) == false && getTicks() >= 200)
		{
			setTicks();
			if (getWorld()->CheckEarthUp(X, Y) == false && getWorld()->CheckEarthDown(X, Y) == false)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					setDirection(up);
					setDefaultNumMoves();
				}
				if (randChoice == 1)
				{
					setDirection(down);
					setDefaultNumMoves();
				}
			}
			else if (getWorld()->CheckEarthUp(X, Y) == false)
			{
				setDirection(up);
				setDefaultNumMoves();
			}
			else if (getWorld()->CheckEarthDown(X, Y) == false)
			{
				setDirection(down);
				setDefaultNumMoves();
			}
		}
		if (getDirection() == up || getDirection() == down && getWorld()->CheckEarthLeft(X, Y) == false || getWorld()->CheckEarthRight(X, Y) == false && getTicks() >= 200)
		{
			setTicks();
			if (getWorld()->CheckEarthRight(X, Y) == false && getWorld()->CheckEarthLeft(X, Y) == false)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					setDirection(right);
					setDefaultNumMoves();
				}
				if (randChoice == 1)
				{
					setDirection(left);
					setDefaultNumMoves();
				}
			}
			else if (getWorld()->CheckEarthRight(X, Y) == false)
			{
				setDirection(right);
				setDefaultNumMoves();
			}
			else if (getWorld()->CheckEarthLeft(X, Y) == false)
			{
				setDirection(left);
				setDefaultNumMoves();
			}
		}
	}
	Direction move = getDirection();
	if (move == up)
	{
		if (getWorld()->CheckEarthUp(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X, Y + 1);
	}
	if (move == down)
	{
		if (getWorld()->CheckEarthDown(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X, Y - 1);
	}
	if (move == right)
	{
		if (getWorld()->CheckEarthRight(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X + 1, Y);
	}
	if (move == left)
	{
		if (getWorld()->CheckEarthLeft(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X - 1, Y);
	}
}

void Protester::annoy()
{
	if (getLeave() == true)
	{
		return;
	}
	decHealth(2);
	if (getHealth() != 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		setStunned(true);
	}
	if (getHealth() <= 0)
	{
		setLeave(true);
		getWorld()->increaseScore(100);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);

	}
}


HardcoreProtester::HardcoreProtester(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);
	setLeave(false);
	setHealth(20);
	getNumMoves();
	setImage(imageID);
}

HardcoreProtester::~HardcoreProtester()
{
}

void HardcoreProtester::doSomething()
{
	increaseTick();
	increaseTicks();
	int X = getX();
	int Y = getY();
	if (getAlive() == false)
	{
		return;
	}
	/*if (getHealth() == 0)
	{
		setLeave(true);
		if (X == 60 && Y == 60)
		{
			setGone();
		}
		else
		{
			setDiscovered(X, Y, false);
			if (getWorld()->CheckEarthUp(X, Y) == false)
			{

			}

		}
	}*/
	if (getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) <= 4)//yelling
	{
		if (X < getWorld()->getTunnelManX() && getDirection() == right && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;

		}
		else if (X > getWorld()->getTunnelManX() && getDirection() == left && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}
		else if (Y < getWorld()->getTunnelManY() && getDirection() == up && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}
		else if (Y > getWorld()->getTunnelManY() && getDirection() == down && getTickAmount() >= 15)
		{
			setTickAmount();
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			setAnnoyed(true);
			getWorld()->callTMAnnoy();
			return;
		}

		if (X < getWorld()->getTunnelManX() && getDirection() == right && getTickAmount() < 15)
		{
			resetNumMoves();
			return;

		}
		else if (X > getWorld()->getTunnelManX() && getDirection() == left && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
		else if (Y < getWorld()->getTunnelManY() && getDirection() == up && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
		else if (Y > getWorld()->getTunnelManY() && getDirection() == down && getTickAmount() < 15)
		{
			resetNumMoves();
			return;
		}
	}
	if (Y == getWorld()->getTunnelManY() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && X < getWorld()->getTunnelManX() && getWorld()->checkEarthRightMore(X, Y) == false)
	{
		setDirection(right);
		moveTo(getX() + 1, getY());
		resetNumMoves();
		return;
	}
	if (Y == getWorld()->getTunnelManY() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && X > getWorld()->getTunnelManX() && getWorld()->checkEarthLeftMore(X, Y) == false)
	{
		setDirection(left);
		moveTo(getX() - 1, getY());
		resetNumMoves();
		return;
	}
	if (X == getWorld()->getTunnelManX() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && Y > getWorld()->getTunnelManY() && getWorld()->checkEarthBelow(X, Y) == false)
	{
		setDirection(down);
		moveTo(getX(), getY() - 1);
		resetNumMoves();
		return;
	}
	if (X == getWorld()->getTunnelManX() && getRadius(X, getWorld()->getTunnelManX(), Y, getWorld()->getTunnelManY()) > 4 && Y < getWorld()->getTunnelManY() && getWorld()->checkEarthAbove(X, Y) == false)
	{
		setDirection(up);
		moveTo(getX(), getY() + 1);
		resetNumMoves();
		return;
	}
	decNumMoves();
	bool flag = false;
	while (flag == false)
	{
		if (getNumMoves() <= 0)
		{
			int randDirection = rand() % 4;
			switch (randDirection)
			{
			case 0:
				if (getWorld()->CheckEarthUp(X, Y) == false)
				{
					flag = true;
					setDirection(up);
					setDefaultNumMoves();
				}
				break;
			case 1:
				if (getWorld()->CheckEarthDown(X, Y) == false)
				{
					flag = true;
					setDirection(down);
					setDefaultNumMoves();
				}
				break;
			case 2:
				if (getWorld()->CheckEarthRight(X, Y) == false)
				{
					flag = true;
					setDirection(right);
					setDefaultNumMoves();
				}
				break;
			case 3:
				if (getWorld()->CheckEarthLeft(X, Y) == false)
				{
					flag = true;
					setDirection(left);
					setDefaultNumMoves();
				}
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (flag == false)
	{
		if (getDirection() == left || getDirection() == right && getWorld()->CheckEarthUp(X, Y) == false || getWorld()->CheckEarthDown(X, Y) == false && getTicks() >= 200)
		{
			setTicks();
			if (getWorld()->CheckEarthUp(X, Y) == false && getWorld()->CheckEarthDown(X, Y) == false)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					setDirection(up);
					setDefaultNumMoves();
				}
				if (randChoice == 1)
				{
					setDirection(down);
					setDefaultNumMoves();
				}
			}
			else if (getWorld()->CheckEarthUp(X, Y) == false)
			{
				setDirection(up);
				setDefaultNumMoves();
			}
			else if (getWorld()->CheckEarthDown(X, Y) == false)
			{
				setDirection(down);
				setDefaultNumMoves();
			}
		}
		if (getDirection() == up || getDirection() == down && getWorld()->CheckEarthLeft(X, Y) == false || getWorld()->CheckEarthRight(X, Y) == false && getTicks() >= 200)
		{
			setTicks();
			if (getWorld()->CheckEarthRight(X, Y) == false && getWorld()->CheckEarthLeft(X, Y) == false)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					setDirection(right);
					setDefaultNumMoves();
				}
				if (randChoice == 1)
				{
					setDirection(left);
					setDefaultNumMoves();
				}
			}
			else if (getWorld()->CheckEarthRight(X, Y) == false)
			{
				setDirection(right);
				setDefaultNumMoves();
			}
			else if (getWorld()->CheckEarthLeft(X, Y) == false)
			{
				setDirection(left);
				setDefaultNumMoves();
			}
		}
	}
	Direction move = getDirection();
	if (move == up)
	{
		if (getWorld()->CheckEarthUp(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X, Y + 1);
	}
	if (move == down)
	{
		if (getWorld()->CheckEarthDown(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X, Y - 1);
	}
	if (move == right)
	{
		if (getWorld()->CheckEarthRight(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X + 1, Y);
	}
	if (move == left)
	{
		if (getWorld()->CheckEarthLeft(X, Y) == true)
		{
			resetNumMoves();
			return;
		}
		moveTo(X - 1, Y);
	}
}

void HardcoreProtester::annoy()
{
	if (getLeave() == true)
	{
		return;
	}
	decHealth(2);
	if (getHealth() != 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		setStunned(true);
	}
	if (getHealth() <= 0)
	{
		setLeave(true);
		getWorld()->increaseScore(250);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}
}



Boulder::Boulder(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);
	setStable();
	setImage(imageID);
}

Boulder::~Boulder()
{

}

void Boulder::doSomething()
{
	if (getAlive() == false)
	{
		return;
	}
	else
	{
		increaseTick();
		if (getStable() == true)
		{
			if (getWorld()->CheckEarthDown(getX(), getY()) == true)
			{
				return;
			}
			else
			{
				setWaiting();
				setTickAmount();
			}
		}
		else if (getWaiting() == true && getTickAmount() >= 30)
		{
			setFalling();
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		else if (getFalling() == true)
		{
			if (getY() - 1 == -1)
			{
				setGone();
			}
			if (getWorld()->CheckEarthDown(getX(), getY()) == true)
			{
				setGone();
			}
			moveTo(getX(), getY() - 1);
			double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
			if (distance <= 3.00)
			{
				getWorld()->setHealth(100);
				getWorld()->setLives(getWorld()->getLives() - 1);
				getWorld()->decLives();
				setGone();
			}
		}
	}
}

void Boulder::annoy()
{

}

bool Boulder::getStable()
{
	return m_Stable;
}

bool Boulder::getWaiting()
{
	return m_Waiting;
}

bool Boulder::getFalling()
{
	return m_Falling;
}

void Boulder::setStable()
{
	m_Stable = true;
	m_Waiting = false;
	m_Falling = false;
}

void Boulder::setWaiting()
{
	m_Stable = false;
	m_Waiting = true;
	m_Falling = false;
}

void Boulder::setFalling()
{
	m_Stable = false;
	m_Waiting = false;
	m_Falling = true;
}





Barrel::Barrel(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(false);
	setMVisible(false);
	setImage(imageID);
}

Barrel::~Barrel()
{

}

void Barrel::doSomething()
{
	if (getAlive() == false)
	{
		return;
	}
	else
	{
		if (getVisible() == false)
		{
			
			double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
			if (distance <= 4.00)
			{
				setVisible(true);
				setMVisible(true); 
				return;
			}
		}
		else
		{
			double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
			if (distance <= 3.00)
			{
				setGone();
				getWorld()->playSound(SOUND_FOUND_OIL);
				getWorld()->increaseScore(1000);
				getWorld()->setOil(getWorld()->getOil() - 1);
			}
		}
	}
}

void Barrel::annoy()
{

}









Gold::Gold(bool visible, bool TMPickUp, StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(visible);
	setMVisible(visible);

	int temp = 100 - getWorld()->getLevel() * 10;

	this->tunnelManPickUp = TMPickUp;

	if (TMPickUp == false) {
		this->ticksToStare = max(50, temp);
		this->ticksRemaining = 100;
	}
}

Gold::~Gold()
{

}

void Gold::doSomething()
{
	if (getAlive() == false) {
		return;
	}
	if (tunnelManPickUp == true) {
		double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
		if (distance <= 3.00) {
			setGone();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(10);
			getWorld()->setGold(getWorld()->getGold() + 1);
			return;
		}
		if (distance <= 4.00)
		{
			setVisible(true);
			setMVisible(true);
			return;
		}
	}
	if (tunnelManPickUp == false) {
		if (ticksRemaining > 0) {
			if (getWorld()->cantMove(TID_PROTESTER, getX(), getY()))
			{
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				getWorld()->increaseScore(25);
				setGone();
			}

			if (getWorld()->cantMove(TID_HARD_CORE_PROTESTER, getX(), getY()))
			{
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				getWorld()->increaseScore(50);
				setGone();
			}
			ticksToStare = ticksToStare - 1;
			ticksRemaining = ticksRemaining - 1;
			return;
		}
		else if (ticksRemaining <= 0) {
			setGone();
		}
	}
}

void Gold::annoy()
{

}


Sonar::Sonar(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);

	int temp = 300 - 10 * getWorld()->getLevel();
	setTicksRemaining(max(100, temp));
}

Sonar::~Sonar()
{
}

void Sonar::doSomething()
{
	if (getAlive() == false) {
		return;
	}
	if (getTicksRemaining() > 0) {
		setTicksRemaining(getTicksRemaining() - 1);
		double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
		if (distance <= 3.00)
		{
			setGone();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(75);
			getWorld()->setSonar(getWorld()->getSonar() + 1);
		}
	}
	else if (getTicksRemaining() <= 0) {
		setGone();
	}
}

void Sonar::annoy()
{

}

void Sonar::setTicksRemaining(int ticks)
{
	this->ticksRemaining = ticks;
}

int Sonar::getTicksRemaining()
{
	return this->ticksRemaining;
}

Squirt::Squirt(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);
	setSquaresTraveled(0);
}

Squirt::~Squirt() {

}

void Squirt::doSomething()
{
	//if squirt is in radius of 3.0 of one or more protestors, it will cause 2 points of annoyance and then immediately die
	if (getAlive() == false) {
		return;
	}
	if (getWorld()->cantMove(TID_PROTESTER, getX(), getY()) == true)
	{
		setAnnoyed(true);
		setGone();
		getWorld()->callAnnoy(TID_PROTESTER);
	}
	if (getWorld()->cantMove(TID_HARD_CORE_PROTESTER, getX(), getY()) == true)
	{
		setAnnoyed(true);
		setGone();
		getWorld()->callAnnoy(TID_HARD_CORE_PROTESTER);;
	}
	if (getSquaresTraveled() > 4) {
		setGone();
		return;
	}
	switch (getDirection()) {
	case left:
		if (getWorld()->cantMove(TID_BOULDER, getX() - 1, getY()) == false && getWorld()->CheckEarthLeft(getX(), getY()) == false) {
			moveTo(getX() - 1, getY());
			setSquaresTraveled(getSquaresTraveled() + 1);
		}
		else {
			setGone();
			return;
		}
		break;
	case right:
		if (getWorld()->cantMove(TID_BOULDER, getX() + 1, getY()) == false && getWorld()->CheckEarthRight(getX(), getY()) == false) {
			moveTo(getX() + 1, getY());
			setSquaresTraveled(getSquaresTraveled() + 1);
		}
		else {
			setGone();
			return;
		}
		break;
	case down:
		if (getWorld()->cantMove(TID_BOULDER, getX(), getY() - 1) == false && getWorld()->CheckEarthDown(getX(), getY()) == false) {
			moveTo(getX(), getY() - 1);
			setSquaresTraveled(getSquaresTraveled() + 1);
		}
		else {
			setGone();
			return;
		}
		break;
	case up:
		if (getWorld()->cantMove(TID_BOULDER, getX(), getY() + 1) == false && getWorld()->CheckEarthUp(getX(), getY()) == false) {
			moveTo(getX(), getY() + 1);
			setSquaresTraveled(getSquaresTraveled() + 1);
		}
		else {
			setGone();
			return;
		}
		break;
	}
}

void Squirt::annoy() {

}

int Squirt::getSquaresTraveled()
{
	return this->squaresTraveled;
}

void Squirt::setSquaresTraveled(int squares)
{
	this->squaresTraveled = squares;
}




Water::Water(StudentWorld* world, int startX, int startY, int imageID, Direction startDirection, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, startDirection, size, depth)
{
	setAlive(true);
	setVisible(true);
	setMVisible(true);

	int temp = 400 - 10 * getWorld()->getLevel();
	setTicksRemaining(max(100, temp));
}

Water::~Water()
{
}

void Water::setTicksRemaining(int ticks)
{
	this->ticksRemaining = ticks;
}

int Water::getTicksRemaining()
{
	return this->ticksRemaining;
}

void Water::doSomething() {
	if (getAlive() == false) {
		return;
	}
	if (getTicksRemaining() > 0) {
		setTicksRemaining(getTicksRemaining() - 1);
		double distance = getRadius(getX(), getWorld()->getTunnelManX(), getY(), getWorld()->getTunnelManY());
		if (distance <= 3.00)
		{
			setGone();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(100);
			getWorld()->setWater(getWorld()->getWater() + 5);
		}
	}
	else if (getTicksRemaining() <= 0) {
		setGone();
	}
}

void Water::annoy()
{
}
