#pragma once

#include "Observer.h"
#include "Observable.h"
#include "ScoreRenderer.h"
#include "LiveRenderer.h"
#include "GameCtrlInputComponent.h"
#include "GameMsgRenderer.h"
#include "BadgeTimer.h"

const int ASTEROIDS_TO_BADGE = 1;

class GameManager : public Container, public Observer, public Observable {
public:
	GameManager();
	GameManager(SDLGame* game);
	virtual ~GameManager();

	bool isGameOver() const;
	int getLives() const;
	bool isRunning() const;
	void setRunning(bool running);
	int getScore() const;
	void setBadge(bool b);
	bool getBadge();
	void receive(Message* msg);

private:

	bool gameOver_;
	int lives_;
	bool running_;
	int score_;
	bool badge_;

	int brokenAsteroids_;

	ScoreRenderer scoreRenderer_;
	LiveRenderer livesRenderer_;
	GameCtrlInputComponent gameCtrl_;
	GameMsgRenderer gameMsg_;
	BadgeTimer badgeTimer_;
};

