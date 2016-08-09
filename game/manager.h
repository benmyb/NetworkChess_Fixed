#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "player.h"

class manager {
private:
	board m_board;
	int who_step = 1;
	bool white_is_first = true;
	CHESS_COLOR moving_color = NONE;
	stack<int> m_before[2];// m_before[0] 存储下棋的序列，m_before[1] : 0 下棋  >=1 移棋前的位置坐标
	stack<int> m_success;//存储成功网络
	bool m_visited[array_size][array_size];// 用于判断成功网络时 ，记录遍历过的点
public:
	manager();
	void restart();
	void step(int x, int y);
	void select(int x, int y);
	void move_step(int x, int y);
	void back(bool mode);
	bool network_dfs(int pos, int & total, CHESS_COLOR color);
	bool judge_success(CHESS_COLOR color);
};


#endif // !MANAGER_H
