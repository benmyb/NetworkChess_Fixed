#include "player.h"

void player::init() {
	m_name = "player";
	int index((rand() % 37) + 1);
	m_face.loadImage("/faces/human/" + ofToString(index) + ".jpg");
	m_bowl.loadModel("bowl.obj");
	m_chess_stack.loadModel("chess.obj");
	m_hand.loadModel("human_hand.stl");
	m_ishuman = true;
	m_onTop = false;
	m_wins = 0;
	m_loses = 0;
	m_isWhite = true;
}

void player::name_reset(const string & name) {
	m_name = name;
}

void player::face_reset(const string & name) {
	m_face.loadImage(name);
}

void player::hand_reset(const string & name) {
	m_hand.loadModel(name);
}

void player::bowl_reset(const string & name) {
	m_bowl.loadModel(name);
}

void player::data_reset() {
	m_wins = 0;
	m_loses = 0;
}

void player::settop(bool top) {
	m_onTop = top;
	if (m_hand.getRotationAngle(0) == 0)m_hand.setRotation(0, 180, 0, 0, 1);
	if (m_bowl.getRotationAngle(0) == 0)m_bowl.setRotation(0, 180, 0, 0, 1);
}

void player::sethuman(bool human) {
	m_ishuman = human;
	if (m_ishuman) {
		m_hand.loadModel("human_hand.stl");
		if (m_onTop) { 
			m_hand.setRotation(0, 180, 0, 0, 1);
			m_bowl.setRotation(0, 180, 0, 0, 1);
		}
	}
	else {
		m_hand.loadModel("robot_hand.stl");
		if (m_onTop) {
			m_hand.setRotation(0, 180, 0, 0, 1);
			m_bowl.setRotation(0, 180, 0, 0, 1);
		}
	}
}

void player::setwhite(bool white) {
	m_isWhite = white;
}
