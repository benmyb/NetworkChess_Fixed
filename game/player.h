#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "ofxAssimpModelLoader.h"
#include "board.h"

class player {
protected:
	string m_name;
	ofImage m_face;
	bool m_onTop = true;
	bool m_ishuman = true;
	bool m_isWhite = true;
	ofxAssimpModelLoader m_hand;
	ofxAssimpModelLoader m_bowl;
	ofxAssimpModelLoader m_chess_stack;
	unsigned m_wins = 0;
	unsigned m_loses = 0;	
public:
	player() {}
	void init();
	void name_reset(const string& name);
	void face_reset(const string& name);
	void hand_reset(const string& name);
	void bowl_reset(const string& name);
	void data_reset();

	void settop(bool top);
	void sethuman(bool human);
	void setwhite(bool white);

	const string& name()const { return m_name; }
	ofImage& face() { return m_face; }
	virtual void getxy(int & x, int & y) = 0;


};

#endif // !PLAYER_H
