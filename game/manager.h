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
	stack<int> m_before[2];// m_before[0] �洢��������У�m_before[1] : 0 ����  >=1 ����ǰ��λ������
	stack<int> m_success;//�洢�ɹ�����
	bool m_visited[array_size][array_size];// �����жϳɹ�����ʱ ����¼�������ĵ�
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
