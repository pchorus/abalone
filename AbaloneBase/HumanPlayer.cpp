#include "AbaloneBase.h"

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const CString& name, BoardField::Ball ball)
: Player(name, ball, Player::PLAYER_TYPE_HUMAN)
{
}

HumanPlayer::~HumanPlayer()
{
}
