#pragma once
#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

class computer :public player {
public:
	computer() :player() {
		m_name = "computer";
		m_face.loadImage("/faces/computer.jpg");
		m_ishuman = false;
		m_hand.loadModel("robot_hand.obj");
		m_hand.setScale(1.6, 1.6, 1.6);
		m_bowl.loadModel("bowl.obj");
		m_bowl.setScale(0.5, 0.5, 0.5);
		m_chess_stack.loadModel("chess.obj");
		m_chess_stack.setScale(0.45, 0.45, 0.45);
		m_wins = 0;
		m_loses = 0;
	}
	void getxy(int & x, int & y);
};

#endif