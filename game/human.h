#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "../game/player.h"

class human :public player {
public:
	human() :player() {
		m_name = "player";
		int index((rand() % 37) + 1);
		m_face.loadImage("/faces/human/" + ofToString(index) + ".jpg");
		m_ishuman = true;
		m_hand.loadModel("human_hand.stl");
		m_hand.setScale(1.8, 1.6, 1.6);
		m_bowl.loadModel("bowl.obj");
		m_bowl.setScale(0.5, 0.5, 0.5);
		m_chess_stack.loadModel("chess.obj");
		m_chess_stack.setScale(0.45, 0.45, 0.45);
		m_wins = 0;
		m_loses = 0;
	}
	void getxy(int & x, int & y) {
		real2data(x, y);
	}
};

#endif