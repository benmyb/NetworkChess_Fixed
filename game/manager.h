#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "player.h"


int change = 99;
class manager {
private:
	board m_board;
	int who_step = 0;
	bool white_is_first = true;
	CHESS_COLOR moving_color = NONE;
	stack<int> m_before[2];// m_before[0] 存储下棋的序列，m_before[1] : 0 下棋  >=1 移棋前的位置坐标
public:
	manager();
	void restart();
	void step(int x, int y);
	void select(int x, int y);
	//移棋到的位置，如果能够移动就返回1
	bool move_step(int x, int y);
	void move_back(int x, int y);

	void back(bool mode);
	bool judge_success(CHESS_COLOR color);
	void move_cancel();


};


#endif // !MANAGER_H
