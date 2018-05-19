#include "GameManager.h"



GameManager::GameManager()
	: Container(nullptr), Observer(), Observable(), 
	gameOver_(false), lives_(0), running_(false), score_(0), badge_(false), brokenAsteroids_(0)
{
}

GameManager::GameManager(SDLGame * game)
	: Container(game), Observer(), Observable(), 
	gameOver_(false), lives_(3), running_(true), score_(0), badge_(false), brokenAsteroids_(0)
{
	scoreRenderer_ = ScoreRenderer();
	livesRenderer_ = LiveRenderer(game_->getResources()->getImageTexture(Resources::Badges));
	gameCtrl_ = GameCtrlInputComponent();
	gameMsg_ = GameMsgRenderer();
	badgeTimer_ = BadgeTimer();
	
	addRenderComponent(&scoreRenderer_);
	addRenderComponent(&livesRenderer_);
	addInputComponent(&gameCtrl_);
	addRenderComponent(&gameMsg_);
	addPhysicsComponent(&badgeTimer_);
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

			if (brokenAsteroids_ == ASTEROIDS_TO_BADGE)
			{
				setBadge(true);
				badgeTimer_.start(3000);
				send(&Message(NORMAL_BADGE));
			}
			else if (brokenAsteroids_ == 2 * ASTEROIDS_TO_BADGE)
			{
				setBadge(true);
				badgeTimer_.start(3000);
				send(&Message(SUPER_BADGE));
			}
			else if (brokenAsteroids_ == 3 * ASTEROIDS_TO_BADGE)
			{
				setBadge(true);
				badgeTimer_.start(3000);
				send(&Message(MULTI_BADGE));
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
		case TAKE_BONUS:
		{
			lives_++;
			break;
		}
	}
}
