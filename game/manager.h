#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "player.h"

class manager {
private:
	board m_board;
	int who_step = 0;
	bool white_is_first = true;
	CHESS_COLOR moving_color = NONE;
	stack<int> m_before[2];// m_before[0] �洢��������У�m_before[1] : 0 ����  >=1 ����ǰ��λ������
public:
	manager();
	void restart();
	void step(int x, int y);
	void select(int x, int y);
	//���嵽��λ�ã�����ܹ��ƶ��ͷ���1
	bool move_step(int x, int y);
	void move_back(int x, int y);

	void back(bool mode);
	bool judge_success(CHESS_COLOR color);
	void move_cancel();

	int* chess_on_board() { return m_board.m_chess; }
	unsigned long  available(CHESS_COLOR color);
	int getstep() { return who_step; }
	int getprechess() { return m_board.pre_chess; }
};


#endif // !MANAGER_H
