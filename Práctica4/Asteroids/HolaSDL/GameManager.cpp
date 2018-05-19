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

	livesRenderer_ = new LiveRenderer(game_->getResources()->getImageTexture(Resources::Badges));
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
	if (badgeTimer_ != nullptr)
		delete badgeTimer_;
	if (gameMsg_ != nullptr)
		delete gameMsg_;
	if (gameCtrl_ != nullptr)
		delete gameCtrl_;
	if (livesRenderer_ != nullptr)
		delete livesRenderer_;
	if (scoreRenderer_ != nullptr)
		delete scoreRenderer_;
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
	Message m = { running ? ROUND_START : ROUND_OVER };
	send(&m);
	if (gameOver_ && running)
		gameOver_ = false;
}

int GameManager::getScore() const
{
	return score_;
}

void GameManager::setBadge(bool b)
{
	badge_ = b;

	Message m = { b ? BADGE_ON : BADGE_OFF };
	send(&m);
}

bool GameManager::getBadge()
{
	return badge_;
}

void GameManager::receive(Message * msg)
{
	switch (msg->id_) {
		case ASTEROID_FIGHTER_COLLISION:
		{
			lives_--;
			brokenAsteroids_ = 0;
			setRunning(false);

			setBadge(false);

			if (lives_ == 0)
			{
				gameOver_ = true;
				send(&Message(GAME_OVER));
			}
			break;
		}

		case BULLET_ASTEROID_COLLISION:
		{
			score_ ++;
			brokenAsteroids_++;

			if (brokenAsteroids_ == 1)
			{
				setBadge(true);
				badgeTimer_->start(3000);
			}

			break;
		}

		case NO_MORE_ASTEROIDS:	
		{
			brokenAsteroids_ = 0;
			send(&Message(ROUND_OVER));

			setRunning(false);
			setBadge(false);

			game_->stop();

			break;
		}
		case ROUND_START:
		{
			brokenAsteroids_ = 0;
			setBadge(false);
			break;
		}
		case ROUND_OVER:
		{
			brokenAsteroids_ = 0;
			setBadge(false);
			break;
		}
	}
}
