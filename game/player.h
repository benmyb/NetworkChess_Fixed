#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "ofxAssimpModelLoader.h"
#include "board.h"

class player {
private:
	string m_name;
	ofImage m_face;
	bool m_onTop = true;
	bool m_ishuman = true;
	bool m_isWhite = true;
	ofxAssimpModelLoader m_hand;
	ofxAssimpModelLoader m_bowl;
	unsigned m_wins = 0;
	unsigned m_loses = 0;
	

public:
	//virtual void step()=0;


};

#endif // !PLAYER_H
