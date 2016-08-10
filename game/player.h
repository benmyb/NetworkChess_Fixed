#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "ofxAssimpModelLoader.h"
#include "board.h"

class player {
private:
	string m_name;
	bool our_side = true;
	bool is_human = true;
	ofxAssimpModelLoader m_hand;
	ofxAssimpModelLoader m_bowl;
};

#endif // !PLAYER_H
