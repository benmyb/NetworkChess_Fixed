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

const int board_size = 8;	// ��ִ�С
const int array_size = 10;	// ���̴�С,�������̴�С��ֹԽ��,����Խ���ж�
const int dir_count = 8;	// 8������ͨ
const int chess_num = 10;	// һ����������
const int chess_need_to_win = 6;//�ɹ�����������������
const int m_inf2 = 0x3f3f3f3f;	// ��������ֵ��������������ֵ
const int m_inf = 0x1f1f1f1f;	// ���õڶ�������ֵ����ʾ�������

const double double_inf = 1e9 + 10;
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




//  noted :::::::: �������£���������
class board {
private:
	// ==0 ��ʾû������,%2=0��ʾ���� %2=1��ʾ����,==m_inf����
	// >0 ��ʾ������ӷ�����m_chess���±�
	int m_curr_color[array_size][array_size];
	int m_limit[2][array_size][array_size];	// m_limit[0][][] ��ʾ����������� m_limit[1][][] ��ʾ�����������
	int m_surround[dir_count][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };	 //�ٽ���
	//node m_network[array_size][array_size][4];	//ÿ������һ���ڵ�
	node * m_head[array_size][array_size][4];// ����ĸ������������ɵ�˫�������ͷָ��


	int pre_chess = -1;	// ǰһ������
	int selected_chess = -1;	// ��ѡ���ƶ�������
	stack<int> m_success;//�洢�ɹ�����
	bool m_visited[array_size][array_size];// �����жϳɹ�����ʱ ����¼�������ĵ�
	// 1 ��ʾ���֣�0 ��ʾ����
	int selected_color = 1;// �����ƶ������ӵ���ɫ
	bool isSelected = false;


	// attention ::new added ��ʾ�����µ��ڼ����������жϸ�˭�£������廹������
	int who_step = 0;

	


	// white_is_first ��ô�õİ�������������

//	bool white_is_first = true;




	//CHESS_COLOR moving_color = NONE;
	stack<int> m_before[2];// m_before[0] �洢��������У�m_before[1] : 0 ����  >=1 ����ǰ��λ������





private:


	void init();
	void init_link();
	void init_line_head(int x, int y, int direction);
	bool feasible(int color, int x, int y);
	//�������Ӻ�����Χ8���㣬�Լ���λ��+m_inf2
	void limit_lay(int color, int x, int y);
	//�������Ӻ�����Χ8���㣬�Լ���λ��-m_inf2
	void limit_retract(int color, int x, int y);
	bool judge_border(int x, int y) {	//�ж��Ƿ�Խ��
		return (x >= 1 && x <= board_size && y >= 1 && y <= board_size);
	}

	// ��ճɹ�����
	void success_clear() { while (!m_success.empty())m_success.pop(); }
	bool step_inside(int x, int y);


	////���壬p1���������꣬p2=Ϊǰ����;mode=0���壬1����
	//bool back(bool mode, int p1, int p2);


	//��������
	bool lay(int color, int x, int y);
	//�ջ�����
	bool retract(int color, int x, int y);
	//��������
	bool network_dfs(int index, int & total, int color);





public:



	// ��ֻ����������ĺ���������������


	int m_chess[chess_num * 2 + 1];// �����Ѿ������������ϵ����ӵ�λ�ã�0λ�ò���
	int m_chess_num = 0;	// ��¼�������̵�������
	int m_chess_pos;// ��ʾӦ�÷���m_chess ���ĸ�λ��


	int GetColor(int x, int y)const { return m_curr_color[x][y]; }

	board();


	//��Ϸ�ٿ�
	void restart();


	// ע�⣺����Ҫ����ģʽ�жϺͲ����л�
	// �ж���Ϸ��ǰģʽ �� 0 ���� 1 ����
	bool get_mode();


	// ������һ���µ����ӣ�
	int get_prechess() {
		return pre_chess;

	}

	//����
	bool step(int x,int y);
	
	
	//����ע���жϵ�ǰ������ɫ
	// ���ڿ����ƶ����µ����ӵ���ɫ 1 ��ʾ�����壬0 ��ʾ������
	int moving_color();

	bool get_before_color();


	vector<bool> m_feasible;



	// ĳһλΪ1 ��ʾ�������壬�������� ��СΪ 0-63
	void cal_all_feasible();

	vector<bool> & get_all_feasible() {
		return m_feasible;
	}


	// �ж�����һ������
	bool getSelected() {
		return isSelected;
	}

	//�����һ����ѡ��Ҫ�ƶ������Ӳ�ɾ����
	// ѡһ��λ��
	bool move_select(int x, int y);
	//����ڶ���������λ������  ����1 ��ʾ�ɹ����壬 ����ʧ��
	bool move_step(int x, int y);
	//������������������� �Ż�����
	void move_cancel();

	// ����ɺ��ӣ� �ջ��ƶ���λ�õ����� ,���ص�����ڶ���
	void move_retract_new();

	// ���壺���봫������� �������� �� p1 ����λ�õ����ӣ� p2=0 ����ģʽ��p2!=0 ֮ǰλ�õ�����
	void back(int & p1,int & p2);

	


	//�жϳɹ������Ƿ����
	bool judge_success( int color);

	stack<int>& success() { return m_success; }



	//// ĳһλΪ1 ��ʾ�������壬�������� ��СΪ 0-63
	//unsigned long  get_all_feasible();

	// first ��ʾĳһλ��û�����ӣ�second ��ʾ��һλ�����Ǻ�ɫ��
	pair<unsigned long, unsigned long> get_current_state();


	// �򵥼���ÿ������Χ�Ŀɼ�����Ŀ 
	/*int  get_scores(int color);*/
	double get_scores_inside(int color);
	
	double get_scores(int color) {
		return get_scores_inside(color) - get_scores_inside((color + 1) % 2);
	}

	int who_turn() { return who_step; }

	int getselect() { return selected_chess; }

	node m_network[array_size][array_size][4];	//ÿ������һ���ڵ�


	void GetAheadBackStone(int index, int direction, int & index_ahead, int & index_back);
	bool WhetherToThr(int indexq, int index2);

};
#endif