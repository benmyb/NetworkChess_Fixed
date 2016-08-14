#pragma once
#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"

typedef int mm;


struct point {
	int p1;
	int p2;
	double scores;

	point(int pp1, int pp2,double ss) {
		p1 = pp1;
		p2 = pp2;
		scores = ss;
	}
	bool operator<(const point & p1)const {
		return scores < p1.scores;

	}
};



class computer :public player {
	computer(){};


	int m_node_num = 0;

	priority_queue<point> m_possible;

	double alpha_beta(int depth, double alpha, double beta, int father, int color, int p1, int p2);

	void search_init() {
		m_node_num = 0;
		m_tree.clear();
		m_state.clear();
		m_scores.clear();
		m_current_state = m_manager->get_current_state();
	}
public:

	board *  m_manager;

	// �±�+1 ����� ��m_tree[] �ǽ��ĸ����
	vector<int> m_tree;
	// pair[0] ��ʾĳһλ�Ƿ������� pair[1] ��ʾĳһλ�������Ƿ���0��ɫ 1��ɫ
	map<int, pair<unsigned long, unsigned long>> m_state;
	map <int, double> m_scores;
	// pair[0] ��ʾĳһλ�Ƿ������� pair[1] ��ʾĳһλ�������Ƿ���0��ɫ 1��ɫ
	pair<unsigned long, unsigned long> m_current_state;
	

	int m_color;// �ҵ�������ʲô��ɫ�ģ�����

	int m_level;// �Ѷȵȼ�

	computer(board & ma) :player(),m_manager(&ma) {
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
	/*void getxy(int & x, int & y);

	void step(int & x, int & y, int & x1, int & y2,int mode);
*/
	// ��ʾ���������һ����������������壬p2=0��ʾ����  ����������
	// ���壺 p1��ʾ��ǰ�µ�λ��  ����: p1 ��ʾ��ǰλ�� p2 ��ʾ�ƶ�ǰ��λ��
	void step(int & p1,int & p2);



};

#endif