#pragma once

#include "Observer.h"
#include "Observable.h"
#include "ScoreRenderer.h"
#include "LiveRenderer.h"
#include "GameCtrlInputComponent.h"
#include "GameMsgRenderer.h"
#include "BadgeTimer.h"


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
	void receive(Message* msg);

private:

	ScoreRenderer scoreRenderer_;
	LiveRenderer livesRenderer_;
	GameCtrlInputComponent gameCtrl_;
	GameMsgRenderer gameMsg_;
	BadgeTimer badgeTimer_;
};

