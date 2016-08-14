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

	// 下标+1 结点编号 ，m_tree[] 是结点的父结点
	vector<int> m_tree;
	// pair[0] 表示某一位是否有棋子 pair[1] 表示某一位的棋子是否是0白色 1黑色
	map<int, pair<unsigned long, unsigned long>> m_state;
	map <int, double> m_scores;
	// pair[0] 表示某一位是否有棋子 pair[1] 表示某一位的棋子是否是0白色 1黑色
	pair<unsigned long, unsigned long> m_current_state;
	

	int m_color;// 我的棋子是什么颜色的？？？

	int m_level;// 难度等级

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
	// 表示计算机的下一步，包括下棋和移棋，p2=0表示下棋  否则是移棋
	// 下棋： p1表示当前下的位置  移棋: p1 表示当前位置 p2 表示移动前的位置
	void step(int & p1,int & p2);



};

#endif