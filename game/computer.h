#pragma once
#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

typedef int mm;




class computer :public player {

	computer(){};

	
	
public:

	manager *  m_manager;

	// �±�+1 ����� ��m_tree[] �ǽ��ĸ����
	vector<int> m_tree;
	// pair[0] ��ʾĳһλ�Ƿ������� pair[1] ��ʾĳһλ�������Ƿ���0��ɫ 1��ɫ
	map<int, pair<unsigned long, unsigned long>> m_state;
	map <int, double> m_scores;

	pair<unsigned long, unsigned long> m_current_state;

	int m_node_num=0;
	computer(manager & ma) :player(),m_manager(&ma) {
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

	void step(int & x, int & y, int & x1, int & y2,int mode);


	double alpha_beta(int depth, double alpha, double beta,CHESS_COLOR color,int father,bool is_alpha);
	
	void search_init() {
		m_node_num = 0;
		m_tree.clear();
		m_state.clear();
		m_scores.clear();
		m_current_state = m_manager->get_state();
	}
};

#endif