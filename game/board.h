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

enum CHESS_COLOR { NONE, WHITE, BLACK };

struct chess {
	CHESS_COLOR color = NONE;
	int num = 0;
};

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
	chess m_curr_color[array_size][array_size];
	int m_limit[2][array_size][array_size];	// m_limit[0][][] ��ʾ����������� m_limit[1][][] ��ʾ�����������
	int m_surround[dir_count][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };	 //�ٽ���
	node m_network[array_size][array_size][4];	//ÿ������һ���ڵ�
	node * m_head[array_size][array_size][4];// ����ĸ������������ɵ�˫�������ͷָ��
	int m_chess[chess_num * 2 + 1];// �����Ѿ������������ϵ����ӵ�λ�ã�0λ�ò���
	int m_chess_num = 0;	// ��¼�������̵�������
	int pre_chess = -1;	// ǰһ������
	int selected_chess = -1;	// ��ѡ���ƶ�������
	CHESS_COLOR selected_color = NONE;
	bool isSelected = false;
public:
	board();
	void init();
	void init_link();
	void init_line_head(int x, int y, int direction);
	bool feasible(CHESS_COLOR color, int x, int y);
	//�������Ӻ�����Χ8���㣨�����Լ���λ�ã�+1
	void limit_lay(CHESS_COLOR color, int x, int y);	
	//�������Ӻ�����Χ8���㣨�����Լ���λ�ã�-1
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
	//���壬p1���������꣬p2=Ϊǰ����;mode=0���壬1����
	bool back(bool mode, int p1, int p2);
	//��������
	bool lay(CHESS_COLOR color, int x, int y);
	//�ջ�����
	bool retract(CHESS_COLOR color, int x, int y);
};
#endif