#include "board.h"

void data2real(int & x, int & y) {
	x -= 1;
	y -= 1;
}

int data2real(int index) {
	return (index % array_size) + (index / array_size) * board_size;
}

void real2data(int & x, int & y) {
	++x;
	++y;
}

int real2data(int index) {
	return (index % board_size) + (index / board_size) * array_size;
}

int data_xytoi(int x, int y) {
	return (x) + (y) * array_size;
}

int real_xytoi(int x, int y) {
	return (x) + (y) * board_size;
}

void data_itoxy(int i, int & x, int & y) {
	x = i % array_size;
	y = i / array_size;
}

void real_itoxy(int i, int & x, int & y) {
	x = i % board_size;
	y = i / board_size;
}

board::board() {
	memset(m_head, 0, sizeof(m_head));
	init();
	init_link();
}

void board::init() {
	memset(m_limit, 0, sizeof(m_limit));
	m_chess_num = 0;
	memset(m_curr_color, 0, sizeof(m_curr_color));
	// 设置特区
	for (int i(1); i <= board_size; ++i) {
		//黑棋不能走上下两行，白棋不能走左右两列
		//白棋连通上下 即y->(1,m_n) 黑棋连通左右  x->(1,m_n)
		m_limit[1][1][i] = m_limit[1][board_size][i] = m_limit[0][i][1] = m_limit[0][i][board_size] = m_inf;
	}

	m_curr_color[1][1] = m_curr_color[1][board_size] = m_curr_color[board_size][1] = m_curr_color[board_size][board_size] = m_inf;//设置禁区
	m_chess_num = 0;	// 记录放在棋盘的棋子数
	pre_chess = -1;	// 前一颗棋子
	selected_chess = -1;	// 被选择移动的棋子
	selected_color = NONE;
	isSelected = false;
}

void board::init_link() {
	//初始化棋盘每个棋子的结点
	memset(m_network, 0, sizeof(m_network));
	// 初始化每个结点对应在棋盘的位置
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int dir(0); dir < dir_count / 2; ++dir) {
				m_network[x][y][dir].m_index = x + y * array_size;
			}
		}
	}
	// 初始化棋盘每个位置的4个方向链表的头结点，时间复杂度O(m_size*m_size*4)
	// direction 表示链表的增加方向 x ,y 表示链表起始位置，也就是空头结点的位置，增加一个空的头结点，方便插入和删除结点
	// direction 具体如下 	int next_step[m_direction][2] = { {-1,1},{0,1},{1,1},{1,0},{-1,0},{-1,-1},{0,-1},{1,-1}};
	int x(0), y(0), direction(0);
	for (int i(0); i < array_size; ++i) {
		// init   y= i  0<x<=m_n
		y = i;
		direction = 3;
		init_line_head(x, y, direction);

		// init  　y-x=i  (1<=y<=m_n,1<=x<=m_n)
		direction = 2;
		init_line_head(x, y, direction);
	}
	y = 0;
	for (int i(0); i < array_size; ++i) {
		// init x+y=i; （1<=y<=m_n,1<=x<=m_n）
		x = i;
		direction = 0;
		init_line_head(x, y, direction);

		// init x=i; （1<=y<=m_n）
		direction = 1;
		init_line_head(x, y, direction);

		// init x-y=i; （1<=y<=m_n,1<=x<=m_n）
		direction = 2;
		init_line_head(x, y, direction);
	}
	x = board_size + 1;
	direction = 0;
	for (int i(0); i < array_size; ++i) {
		// init x+y=m_n+1+i   （1<=y<=m_n,1<=x<=m_n）
		y = i;
		init_line_head(x, y, direction);
	}
}

void board::init_line_head(int x, int y, int direction) {
	int count(0);	// 记录是第几个结点
					//由于棋盘扩大了，把周边没有用的结点作为一些链表的空头结点，增加结点的利用率，不用动态结点，方便初始化
	node* head = &m_network[x][y][direction];
	while (1) {
		//根据方向数组，计算链表的下一个结点的位置，
		x += m_surround[direction][0];
		y += m_surround[direction][1];
		if (judge_border(x, y)) {
			m_head[x][y][direction] = head;
			m_network[x][y][direction].m_num = ++count;
		}
		else break;
	}
}

bool board::feasible(CHESS_COLOR color, int x, int y) {
	if (!judge_border(x, y) || m_curr_color[x][y] || m_limit[color][x][y] >= m_inf)return false;

	//附近有2个棋子
	if (m_limit[color][x][y] >= 2)return 0;

	int count = 0;//记录周围有几个同色棋子

	int near_x, near_y;
	for (int i(0); i < dir_count; ++i)
	{
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		//if (m_situation[near_x][near_y] > 0 && m_situation[near_x][near_y] < m_inf&&m_situation[near_x][near_y] % 2 == color&&m_limit[color][near_x][near_y] == 2)return 0;

		// change by dyy at 8 7 10:36
		if (m_limit[color][near_x][near_y] == m_inf2 + 1)return false;
	}
	return true;
}

//放下棋子后在周围8个点（包括自己的位置）+1
void board::limit_lay(CHESS_COLOR color, int x, int y) {
	// 便于群的判断， 放下一个棋子，就在它的周围limit 数组+1 ，它自己的位置加上一个特殊值 m_inf2

	int near_x(0), near_y(0);
	for (int i(0); i < dir_count; ++i) {
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// 无须判断越界，因为扩大了棋盘，节省了判断的时间
		++m_limit[color][near_x][near_y];
	}

	m_limit[color][x][y] += m_inf2;
}

//拿走棋子后在周围8个点（包括自己的位置）-1
void board::limit_retract(CHESS_COLOR color, int x, int y) {
	// 便于群的判断， 收回一个棋子，就在它的周围limit 数组-1 它自己的位置减去一个特殊值 m_inf2

	int near_x(0), near_y(0);
	for (int i(0); i < dir_count; ++i) {
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// 无须判断越界，因为扩大了棋盘，节省了判断的时间
		--m_limit[color][near_x][near_y];
	}

	m_limit[color][x][y] -= m_inf2;
}

//游戏再开
void board::restart() {
	init();
	// 初始化 m_doubly_link  m_head
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int j(0); j < 4; ++j) {
				m_network[x][y][j].setZero();
				m_head[x][y][j]->setZero();
			}
		}
	}
}

//下棋
bool board::step(CHESS_COLOR color, int x, int y) {
	string c_color;
	switch (color) {
	case WHITE:c_color = "白"; break;
	case BLACK:c_color = "黑"; break;
	case NONE:c_color = "没有颜"; break;
	default:c_color = "未知"; break;
	}
	if (lay(color, x, y)) {
		m_chess[++m_chess_num] = data_xytoi(x, y);
		m_curr_color[x][y] = m_chess_num;
		cout << c_color << "方在(" << x << ',' << y << ")下子" << endl;
		pre_chess = data_xytoi(x, y);
		return true;
	}
	else {
		cout << "错误：在(" << x << ',' << y << ")处不能放下" << c_color << "色棋子" << endl;
		return false;
	}
}

//移棋第一步：选择要移动的棋子并删除它
bool board::move_select(int x, int y) {
	if (m_curr_color[x][y]) {
		string c_color;
		switch (m_curr_color[x][y]%2) {
		case WHITE:c_color = "白"; break;
		case BLACK:c_color = "黑"; break;
		default:c_color = "未知"; break;
		}
		selected_color = static_cast<CHESS_COLOR>(m_curr_color[x][y] % 2);
		selected_chess = data_xytoi(x, y);
		isSelected = true;
		retract(selected_color, x, y);
		cout << "选择在(" << x << ',' << y << ")的" << c_color << "色棋子移动" << endl;
		return true;
	}
	else {
		cout << "错误：在(" << x << ',' << y << ")处没有棋子可供选择！" << endl;
		return false;
	}
}

//移棋第二步：在新位置下棋
bool board::move_step(int x, int y) {
	if (isSelected) {
		string c_color;
		switch (selected_color) {
		case WHITE:c_color = "白"; break;
		case BLACK:c_color = "黑"; break;
		case NONE:c_color = "没有颜"; break;
		default:c_color = "未知"; break;
		}
		if (lay(selected_color, x, y)) {
			int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);
			m_chess[m_curr_color[b_x][b_y]] = data_xytoi(x, y);
			m_curr_color[x][y] = m_curr_color[b_x][b_y];
			cout << c_color << "方把(" << b_x << ',' << b_y << ")移动到(" << x << ',' << y << ')' << endl;
			isSelected = false;
			pre_chess = data_xytoi(x, y);
			return true;
		}
		else {
			int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);
			cout << "错误：在(" << b_x << ',' << b_y << ")处不能放下" << c_color << "色棋子，移棋失败" << endl;
			return false;
		}
	}
	else {
		cout << "错误：未选中棋子！" << endl;
		return false;
	}
}

//悔棋，p1：现在坐标，p2=为前坐标;mode=0下棋，1移棋
bool board::back(bool mode, int p1, int p2) {
	int current_x(0), current_y(0);
	data_itoxy(p1, current_x, current_y);
	int pre_x(0), pre_y(0);
	data_itoxy(p2, pre_x, pre_y);
	if (retract(static_cast<CHESS_COLOR>(m_curr_color[current_x][current_y] % 2), current_x, current_y)) {
		if (mode == 0) {	//下棋
			m_curr_color[current_x][current_y] = 0;
			pre_chess = p2;
			--m_chess_num;
			return true;
		}
		else {	//移棋
			m_curr_color[pre_x][pre_y] = m_curr_color[current_x][current_y];
			lay(static_cast<CHESS_COLOR>(m_curr_color[pre_x][pre_y] % 2), pre_x, pre_y);
			pre_chess = p2;
			return true;
		}
	}
	else {
		string c_color;
		switch (m_curr_color[current_x][current_y]%2) {
		case WHITE:c_color = "白"; break;
		case BLACK:c_color = "黑"; break;
		case NONE:c_color = "没有颜"; break;
		default:c_color = "未知"; break;
		}
		cout << "不能拿起在(" << current_x << ',' << current_y << ")的" << c_color << "色棋子!" << endl;
		return false;
	}
}

//放下棋子
bool board::lay(CHESS_COLOR color, int x, int y) {
	if (feasible(color, x, y)) {
		limit_lay(color, x, y);
		// 插入棋局链表中
		node * temp;
		for (int i(0); i < 4; ++i) {
			temp = m_head[x][y][i];
			while (!(temp->m_next == NULL) && (temp->m_next)->m_index < m_network[x][y][i].m_index)
				temp = temp->m_next;
			m_network[x][y][i].m_pioneer = temp;
			m_network[x][y][i].m_next = temp->m_next;
			if (temp->m_next != NULL) {
				temp->m_next->m_pioneer = &m_network[x][y][i];
			}
			temp->m_next = &m_network[x][y][i];
		}
		return true;
	}
	return false;
}

//收回棋子
bool board::retract(CHESS_COLOR color, int x, int y) {
	limit_retract(color, x, y);
	// 在棋局链表中删除结点
	for (int i = 0; i < 4; i++) {
		m_network[x][y][i].m_pioneer->m_next = m_network[x][y][i].m_next;
		if (!(m_network[x][y][i].m_next == NULL))
			m_network[x][y][i].m_next->m_pioneer = m_network[x][y][i].m_pioneer;
		m_network[x][y][i].setZero();
	}
}

