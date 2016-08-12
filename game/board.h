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

const int board_size = 8;	// ��ִ�С
const int array_size = 10;	// ���̴�С,�������̴�С��ֹԽ��,����Խ���ж�
const int dir_count = 8;	// 8������ͨ
const int chess_num = 10;	// һ����������
const int chess_need_to_win = 6;//�ɹ�����������������
const int m_inf = 0x3f3f3f3f;	// ��������ֵ��������������ֵ
const int m_inf2 = 0x1f1f1f1f;	// ���õڶ�������ֵ����ʾ�������
//������ͨ����  ������ͨ���� 

enum CHESS_COLOR { WHITE, BLACK, NONE }; //%2==0���ӣ�%2==1����


struct node {
	node * m_pioneer = nullptr;
	node * m_next = nullptr;
	int m_num = -1;//��ʾ������ĵڼ������
	int m_index = -1;// ��ʾ�������������е�λ��
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
	// ==0 ��ʾû������,%2=0��ʾ���� %2=1��ʾ����,==m_inf����
	// >0 ��ʾ������ӷ�����m_chess���±�
	int m_curr_color[array_size][array_size];
	int m_limit[2][array_size][array_size];	// m_limit[0][][] ��ʾ����������� m_limit[1][][] ��ʾ�����������
	int m_surround[dir_count][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };	 //�ٽ���
	node m_network[array_size][array_size][4];	//ÿ������һ���ڵ�
	node * m_head[array_size][array_size][4];// ����ĸ������������ɵ�˫�������ͷָ��
	int m_chess[chess_num * 2 + 1];// �����Ѿ������������ϵ����ӵ�λ�ã�0λ�ò���
	int m_chess_num = 0;	// ��¼�������̵�������
	int pre_chess = -1;	// ǰһ������
	int selected_chess = -1;	// ��ѡ���ƶ�������
	stack<int> m_success;//�洢�ɹ�����
	bool m_visited[array_size][array_size];// �����жϳɹ�����ʱ ����¼�������ĵ�
	CHESS_COLOR selected_color = NONE;
	bool isSelected = false;
public:
	board();
	void init();
	void init_link();
	void init_line_head(int x, int y, int direction);
	bool feasible(CHESS_COLOR color, int x, int y);
	//�������Ӻ�����Χ8���㣬�Լ���λ��+m_inf2
	void limit_lay(CHESS_COLOR color, int x, int y);	
	//�������Ӻ�����Χ8���㣬�Լ���λ��-m_inf2
	void limit_retract(CHESS_COLOR color, int x, int y);
	bool judge_border(int x, int y) {	//�ж��Ƿ�Խ��
		return (x >= 1 && x <= board_size && y >= 1 && y <= board_size);
	}

	//��Ϸ�ٿ�
	void restart();
	//����
	bool step(CHESS_COLOR color, int x, int y);
	//�����һ����ѡ��Ҫ�ƶ������Ӳ�ɾ����
	bool move_select(int x, int y);
	//����ڶ���������λ������
	bool move_step(int x, int y);
	//�������������������
	bool move_cancel();
	//���壬p1���������꣬p2=Ϊǰ����;mode=0���壬1����
	bool back(bool mode, int p1, int p2);
	//��������
	bool lay(CHESS_COLOR color, int x, int y);
	//�ջ�����
	bool retract(CHESS_COLOR color, int x, int y);
	//��������
	bool network_dfs(int index, int & total, CHESS_COLOR color);
	//�жϳɹ������Ƿ����
	bool judge_success(int who_step, CHESS_COLOR color);

	stack<int>& success() { return m_success; }
	void success_clear() { while (!m_success.empty())m_success.pop(); }

	// ĳһλΪ1 ��ʾ�������壬�������� ��СΪ 0-63
	unsigned long  get_all_feasible(CHESS_COLOR color);

	

	

};
#endif