#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

const int board_size = 8;	// 棋局大小
const int array_size = 10;	// 棋盘大小,扩大棋盘大小防止越界,减少越界判断
const int dir_count = 8;	// 8方向联通
const int chess_num = 10;	// 一方的棋子数
const int chess_need_to_win = 6;//成功网络中所需棋子数
const int m_inf = 0x3f3f3f3f;	// 设置无穷值，便于设置特殊值
const int m_inf2 = 0x1f1f1f1f;	// 设置第二个特殊值，表示特殊情况
//白棋连通上下  黑棋连通左右 

enum CHESS_COLOR { WHITE, BLACK, NONE }; //%2==0白子，%2==1黑子


struct node {
	node * m_pioneer = nullptr;
	node * m_next = nullptr;
	int m_num = -1;//表示是链表的第几个结点
	int m_index = -1;// 表示这个结点在棋盘中的位置
	node(int num) :m_num(num), m_next(nullptr), m_pioneer(nullptr) {};
	node() :m_pioneer(nullptr), m_next(nullptr), m_num(-1) {};
	void setZero() {
		m_pioneer = nullptr;
		m_next = nullptr;
	}
};

void data2real(int & x, int & y);	//Note:value changed!
int data2real(int index);
void real2data(int & x, int & y);	//Note:value changed!
int real2data(int index);
int data_xytoi(int x, int y);
int real_xytoi(int x, int y);
void data_itoxy(int i, int & x, int & y);
void real_itoxy(int i, int & x, int & y);

class board {
public:
	// ==0 表示没有棋子,%2=0表示白棋 %2=1表示黑棋,==m_inf禁区
	// >0 表示这个棋子放置在m_chess的下标
	int m_curr_color[array_size][array_size];
	int m_limit[2][array_size][array_size];	// m_limit[0][][] 表示白棋的限制域， m_limit[1][][] 表示黑棋的限制域
	int m_surround[dir_count][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };	 //临近域
	node m_network[array_size][array_size][4];	//每个棋子一个节点
	node * m_head[array_size][array_size][4];// 存放四个方向的棋子组成的双向链表的头指针
	int m_chess[chess_num * 2 + 1];// 储存已经在下在棋盘上的棋子的位置，0位置不存
	int m_chess_num = 0;	// 记录放在棋盘的棋子数
	int pre_chess = -1;	// 前一颗棋子
	int selected_chess = -1;	// 被选择移动的棋子
	stack<int> m_success;//存储成功网络
	bool m_visited[array_size][array_size];// 用于判断成功网络时 ，记录遍历过的点
	CHESS_COLOR selected_color = NONE;
	bool isSelected = false;
public:
	board();
	void init();
	void init_link();
	void init_line_head(int x, int y, int direction);
	bool feasible(CHESS_COLOR color, int x, int y);
	//放下棋子后在周围8个点，自己的位置+m_inf2
	void limit_lay(CHESS_COLOR color, int x, int y);	
	//拿走棋子后在周围8个点，自己的位置-m_inf2
	void limit_retract(CHESS_COLOR color, int x, int y);
	bool judge_border(int x, int y) {	//判断是否越界
		return (x >= 1 && x <= board_size && y >= 1 && y <= board_size);
	}

	//游戏再开
	void restart();
	//下棋
	bool step(CHESS_COLOR color, int x, int y);
	//移棋第一步：选择要移动的棋子并删除它
	bool move_select(int x, int y);
	//移棋第二步：在新位置下棋
	bool move_step(int x, int y);
	//移棋第三步：不想移棋
	bool move_cancel();
	//悔棋，p1：现在坐标，p2=为前坐标;mode=0下棋，1移棋
	bool back(bool mode, int p1, int p2);
	//放下棋子
	bool lay(CHESS_COLOR color, int x, int y);
	//收回棋子
	bool retract(CHESS_COLOR color, int x, int y);
	//网络深搜
	bool network_dfs(int index, int & total, CHESS_COLOR color);
	//判断成功网络是否存在
	bool judge_success(int who_step, CHESS_COLOR color);

	stack<int>& success() { return m_success; }
	void success_clear() { while (!m_success.empty())m_success.pop(); }

	// 某一位为1 表示可以下棋，否则不能下 大小为 0-63
	unsigned long  get_all_feasible(CHESS_COLOR color);

	

	

};
#endif