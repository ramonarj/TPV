#include "GameManager.h"



GameManager::GameManager()
	: Container(nullptr), Observer(), Observable(), 
	gameOver_(false), lives_(0), running_(false), score_(0), badge_(false), brokenAsteroids_(0)
{
	scoreRenderer_ = nullptr;
	livesRenderer_ = nullptr;
	gameCtrl_ = nullptr;
	gameMsg_ = nullptr;
	badgeTimer_ = nullptr;
}

GameManager::GameManager(SDLGame * game)
	: Container(game), Observer(), Observable(), 
	gameOver_(false), lives_(3), running_(true), score_(0), badge_(false), brokenAsteroids_(0)
{
	scoreRenderer_ = new ScoreRenderer();
	livesRenderer_ = new LiveRenderer();
	gameCtrl_ = new GameCtrlInputComponent();
	gameMsg_ = new GameMsgRenderer();
	badgeTimer_ = new BadgeTimer();
	
	addRenderComponent(scoreRenderer_);
	addRenderComponent(livesRenderer_);
	addInputComponent(gameCtrl_);
	addRenderComponent(gameMsg_);
	addPhysicsComponent(badgeTimer_);
}


GameManager::~GameManager()
{
}

bool GameManager::isGameOver() const
{
	return gameOver_;
}

int GameManager::getLives() const
{
	return lives_;
}

bool GameManager::isRunning() const
{
	return running_;
}

void GameManager::setRunning(bool running)
{
	running_ = running;
}

int GameManager::getScore() const
{
	return score_;
}

void GameManager::setBadge(bool b)
{
	badge_ = b;
}

void GameManager::receive(Message * msg)
{
	switch (msg->id_) {
		case ASTEROID_FIGHTER_COLLISION:
		{
			lives_--;
			brokenAsteroids_ = 0;
			send(ROUND_OVER);

			if (badge_)
			{
				badge_ = false;
				send(BADGE_OFF);
			}

			if (lives_ < 0)
			{
				gameOver_ = true;
				send(GAME_OVER);
			}

			break;
		}

		case BULLET_ASTEROID_COLLISION:
		{
			score_ += ASTEROID_SCORE;

			brokenAsteroids_++;
			if (brokenAsteroids_ == 10)
			{
				badge_ = true;
				send(BADGE_ON);
				badgeTimer_->start(3000);
			}

			break;
		}

		case NO_MORE_ASTEROIDS:	
		{
			brokenAsteroids_ = 0;
			send(ROUND_OVER);

			running_ = false;

			if (badge_)
			{
				badge_ = false;
				send(BADGE_OFF);
			}

			break;
		}
	}
}
