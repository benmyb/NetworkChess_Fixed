#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include<limits>
using namespace std;

const int board_size = 8;	// 棋局大小
const int array_size = 10;	// 棋盘大小,扩大棋盘大小防止越界,减少越界判断
const int dir_count = 8;	// 8方向联通
const int chess_num = 10;	// 一方的棋子数
const int chess_need_to_win = 6;//成功网络中所需棋子数
const int m_inf2 = 0x3f3f3f3f;	// 设置无穷值，便于设置特殊值
const int m_inf = 0x1f1f1f1f;	// 设置第二个特殊值，表示特殊情况

const double double_inf = 1e9 + 10;
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




//  noted :::::::: 黑棋先下！！！！！
class board {
private:
	// ==0 表示没有棋子,%2=0表示白棋 %2=1表示黑棋,==m_inf禁区
	// >0 表示这个棋子放置在m_chess的下标
	int m_curr_color[array_size][array_size];
	int m_limit[2][array_size][array_size];	// m_limit[0][][] 表示白棋的限制域， m_limit[1][][] 表示黑棋的限制域
	int m_surround[dir_count][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };	 //临近域
	//node m_network[array_size][array_size][4];	//每个棋子一个节点
	node * m_head[array_size][array_size][4];// 存放四个方向的棋子组成的双向链表的头指针


	int pre_chess = -1;	// 前一颗棋子
	int selected_chess = -1;	// 被选择移动的棋子
	stack<int> m_success;//存储成功网络
	bool m_visited[array_size][array_size];// 用于判断成功网络时 ，记录遍历过的点
	// 1 表示先手，0 表示后手
	int selected_color = 1;// 现在移动的棋子的颜色
	bool isSelected = false;


	// attention ::new added 表示下棋下到第几步，用来判断该谁下，是下棋还是移棋
	int who_step = 0;

	


	// white_is_first 怎么用的啊啊啊？？？？

//	bool white_is_first = true;




	//CHESS_COLOR moving_color = NONE;
	stack<int> m_before[2];// m_before[0] 存储下棋的序列，m_before[1] : 0 下棋  >=1 移棋前的位置坐标





private:


	void init();
	void init_link();
	void init_line_head(int x, int y, int direction);
	bool feasible(int color, int x, int y);
	//放下棋子后在周围8个点，自己的位置+m_inf2
	void limit_lay(int color, int x, int y);
	//拿走棋子后在周围8个点，自己的位置-m_inf2
	void limit_retract(int color, int x, int y);
	bool judge_border(int x, int y) {	//判断是否越界
		return (x >= 1 && x <= board_size && y >= 1 && y <= board_size);
	}

	// 清空成功网络
	void success_clear() { while (!m_success.empty())m_success.pop(); }
	bool step_inside(int x, int y);


	////悔棋，p1：现在坐标，p2=为前坐标;mode=0下棋，1移棋
	//bool back(bool mode, int p1, int p2);


	//放下棋子
	bool lay(int color, int x, int y);
	//收回棋子
	bool retract(int color, int x, int y);
	//网络深搜
	bool network_dfs(int index, int & total, int color);





public:



	// 你只允许用下面的函数！！！！！！


	int m_chess[chess_num * 2 + 1];// 储存已经在下在棋盘上的棋子的位置，0位置不存
	int m_chess_num = 0;	// 记录放在棋盘的棋子数
	int m_chess_pos;// 表示应该放在m_chess 的哪个位置


	int GetColor(int x, int y)const { return m_curr_color[x][y]; }

	board();


	//游戏再开
	void restart();


	// 注意：界面要进行模式判断和操作切换
	// 判断游戏当前模式 ： 0 下棋 1 移棋
	bool get_mode();


	// 返回上一步下的棋子，
	int get_prechess() {
		return pre_chess;

	}

	//下棋
	bool step(int x,int y);
	
	
	//界面注意判断当前棋子颜色
	// 现在可以移动或下的棋子的颜色 1 表示先手棋，0 表示后手棋
	int moving_color();

	bool get_before_color();


	vector<bool> m_feasible;



	// 某一位为1 表示可以下棋，否则不能下 大小为 0-63
	void cal_all_feasible();

	vector<bool> & get_all_feasible() {
		return m_feasible;
	}


	// 判断拿着一个棋子
	bool getSelected() {
		return isSelected;
	}

	//移棋第一步：选择要移动的棋子并删除它
	// 选一个位置
	bool move_select(int x, int y);
	//移棋第二步：在新位置下棋  返回1 表示成功下棋， 否则失败
	bool move_step(int x, int y);
	//移棋第三步：不想移棋 放回棋子
	void move_cancel();

	// 界面可忽视： 收回移动新位置的棋子 ,返回到移棋第二步
	void move_retract_new();

	// 悔棋：无须传入参数： 传出参数 ： p1 现在位置的棋子， p2=0 下棋模式，p2!=0 之前位置的棋子
	void back(int & p1,int & p2);

	


	//判断成功网络是否存在
	bool judge_success( int color);

	stack<int>& success() { return m_success; }



	//// 某一位为1 表示可以下棋，否则不能下 大小为 0-63
	//unsigned long  get_all_feasible();

	// first 表示某一位有没有棋子，second 表示这一位棋子是黑色的
	pair<unsigned long, unsigned long> get_current_state();


	// 简单计算每个点周围的可见点数目 
	/*int  get_scores(int color);*/
	double get_scores_inside(int color);
	
	double get_scores(int color) {
		return get_scores_inside(color) - get_scores_inside((color + 1) % 2);
	}

	int who_turn() { return who_step; }

	int getselect() { return selected_chess; }

	node m_network[array_size][array_size][4];	//每个棋子一个节点


	void GetAheadBackStone(int index, int direction, int & index_ahead, int & index_back);
	bool WhetherToThr(int indexq, int index2);

};
#endif