#include "board.h"

void data2real(int & x, int & y) {
	x -= 1;
	y -= 1;
}

int data2real(int index) {
	index -= 11;
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

board::board():m_feasible(board_size*board_size) {
	memset(m_head, 0, sizeof(m_head));
	init();
	init_link();
}

void board::init() {
	memset(m_limit, 0, sizeof(m_limit));
	m_chess_num = 0;
	memset(m_curr_color, 0, sizeof(m_curr_color));
	while (!m_success.empty())m_success.pop();
	// 重新开始，清空m_before
	while (!m_before[0].empty())m_before[0].pop();

	while (!m_before[1].empty())m_before[1].pop();
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
	selected_color = 1;
	isSelected = false;

	// 初始棋子在m_chess的位置是0，即没有棋子
	m_chess_pos = 0;
	who_step = 0;

	
}

void board::init_link() {
	//初始化棋盘每个棋子的结点
	memset(m_network, 0, sizeof(m_network));
	// 初始化每个结点对应在棋盘的位置
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int dir(0); dir < dir_count / 2; ++dir) {
				m_network[x][y][dir].m_index =data_xytoi(x,y)/* x + y * array_size*/;
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
	// O(m_n/2) =O(4)
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

bool board::feasible(int color, int x, int y) {
	// judge_border 表示是否越界
	//  m_curr_color 不为0 ，表示有棋子，或为禁区
	if (!judge_border(x, y) || m_curr_color[x][y] || m_limit[color][x][y] >= m_inf)return false;

	//附近有2个棋子
	if (m_limit[color][x][y] >= 2)return 0;

	int count = 0;//记录周围有几个同色棋子

	int near_x, near_y;
	for (int i(0); i < dir_count; ++i)
	{
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// 表示周围有一个同色棋子，而且这个棋子周围也有一个同色棋子
		if (m_limit[color][near_x][near_y] >= m_inf2 + 1)return false;
	}
	return true;
}

//放下棋子后在周围8个点 ，自己的位置+m_inf2
void board::limit_lay(int color, int x, int y) {
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

//拿走棋子后在周围8个点，自己的位置-m_inf2
void board::limit_retract(int color, int x, int y) {
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
	// 初始化 m_network  m_head
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int j(0); j < 4; ++j) {
				m_network[x][y][j].setZero();
				m_head[x][y][j]->setZero();
			}
		}
	}



}

bool board::get_mode()
{
	return who_step >= 2 * chess_num;
}

bool board::step(int x, int y)
{

	
	if (step_inside(x, y)) {
		m_before[0].push(data_xytoi(x, y));
		m_before[1].push(0);
		who_step++;
		selected_color = (selected_color + 1) % 2;
		return 1;
	}
	else return 0;
}

//下棋
bool board::step_inside(int x, int y) {
	

	if (selected_color != (m_chess_num + 1) % 2) {
		//cout << "step_inside error:　颜色对不上？？？？" << endl;
		return 0;
	}

	CHESS_COLOR color = static_cast<CHESS_COLOR>((m_chess_num + 1) % 2);
	/*string c_color;
	switch (color) {
	case WHITE:c_color = "后手"; break;
	case BLACK:c_color = "先手"; break;
	case NONE:c_color = "没有"; break;
	default:c_color = "未知"; break;
	}*/
	//who_step++;

	if (lay(color, x, y)) {
		m_chess_pos=++m_chess_num;
		m_chess[m_chess_pos] = data_xytoi(x, y);
		m_curr_color[x][y] = m_chess_pos;
		//cout << c_color << "方在(" << x << ',' << y << ")下子" << endl;
		pre_chess = data_xytoi(x, y);
		return true;
	}
	else {
		//cout << "错误：在(" << x << ',' << y << ")处不能放下" << c_color << "棋子" << endl;
		return false;
	}
}

int board::moving_color()
{
	return (selected_color);
}

bool board::get_before_color()
{
	return !selected_color;
	//return false;
}

//移棋第一步：选择要移动的棋子并删除它
bool board::move_select(int x, int y) {

	if (m_curr_color[x][y] == m_inf || m_curr_color[x][y] == 0) {

		//cout << "move select error :: 这个位置没有棋了？？？？" << endl;
		return 0;
	}

	if (isSelected) {
		//cout << "move select error ::  上次的棋子还没有放下" << endl;
		return 0;
	}

	if (m_curr_color[x][y] % 2 != selected_color) {
		//cout << "move select error :: 这个颜色不允许移动" << endl;
		return 0;
	}

	if (m_curr_color[x][y]) {
		/*string c_color;
		switch (m_curr_color[x][y]%2) {
		case WHITE:c_color = "后手"; break;
		case BLACK:c_color = "先手"; break;
		case NONE:c_color = "没有"; break;
		default:c_color = "未知"; break;
		}*/
		//selected_color = m_curr_color[x][y] % 2;
		m_chess_pos = m_curr_color[x][y];
		m_curr_color[x][y] = 0;
		selected_chess = data_xytoi(x, y);
		isSelected = true;
		retract(selected_color, x, y);
		//cout << "选择在(" << x << ',' << y << ")的" << c_color << "色棋子移动" << endl;


		

		return true;
	}
	else {
		//cout << "错误：在(" << x << ',' << y << ")处没有棋子可供选择！" << endl;
		return false;
	}
}

//移棋第二步：在新位置下棋
bool board::move_step(int x, int y) {
	if (isSelected) {
		/*string c_color;
		switch (selected_color) {
		case WHITE:c_color = "后手"; break;
		case BLACK:c_color = "先手"; break;
		case NONE:c_color = "没有"; break;
		default:c_color = "未知"; break;
		}*/
		if (lay(selected_color, x, y)) {
			/*int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);*/

			if (m_chess_pos == 0) {
				//cout << "move_step error:　位置m_chess_pos出错了，为什么？？" << endl;
			}
			if (m_chess_pos % 2 != selected_color) {
				//cout << "move_step error ::　放的位置和颜色不一样，为什么？？" << endl;
			}
			m_chess[m_chess_pos] = data_xytoi(x, y);
			m_curr_color[x][y] = m_chess_pos;
			m_chess_pos = 0;
			//m_curr_color[b_x][b_y] = 0;
			//cout << c_color << "方把(" << b_x << ',' << b_y << ")移动到(" << x << ',' << y << ')' << endl;
			isSelected = false;
			pre_chess = data_xytoi(x, y);
			

			// 判断棋面修改
			selected_color = (selected_color + 1) % 2;
			who_step++;

			m_before[0].push(pre_chess);
			m_before[1].push( selected_chess);

			return true;
		}
		else {
			/*int b_x(0), b_y(0);
			data_itoxy(pre_chess, b_x, b_y);*/
			//cout << "错误：在(" << b_x << ',' << b_y << ")处不能放下" << c_color << "色棋子，移棋失败" << endl;
			return false;
		}
	}
	else {
		//cout << "错误：未选中棋子！" << endl;
		return false;
	}
}

//移棋第三步：不想移棋，把棋子放回去
void board::move_cancel()
{


	if (!isSelected) {
		//cout << "move cancel error : 没有选中棋子，怎么放回去???" << endl;
	}

	int x, y;

	data_itoxy(selected_chess,x, y);


	isSelected = false;
	if (lay(selected_color, x, y)) {
		m_curr_color[x][y] = m_chess_pos;
		m_chess[m_chess_pos] = selected_chess;
		isSelected = false;
		/*string c_color;
		switch (selected_color) {
		case WHITE:c_color = "后手"; break;
		case BLACK:c_color = "先手"; break;
		case NONE:c_color = "没有"; break;
		default:c_color = "未知"; break;
		}*/

		//cout << c_color << "方把棋子放回到(" << x << ',' << y << ')' << endl;

	}
	else {
		//cout << "出错：不知道为什么放不回去棋子？？？内核有问题" << endl;
	}

	//return false;
}

// 界面可忽视： 收回移动新位置的棋子
void board::move_retract_new()
{
	if (isSelected) {
		//cout << "move_retract_new error :: 之前的棋子没有放下" << endl;
		return;
	}

	if (who_step <= 2 * chess_num) {
		//cout << "move_retract_new error :: 模式不对，上一步是下棋" << endl;
		return;
	}

	else {

		isSelected = 1;


		// 判断棋面修改 ,相当于返回上一步
		selected_color = (selected_color + 1) % 2;
		who_step--;

		selected_chess = m_before[1].top();
		m_before[1].pop();
		
		int current(m_before[0].top()), current_x(0), current_y(0);
		m_before[0].pop();
		if (!m_before[0].empty())
			pre_chess = m_before[0].top();
		else {
			//cout << "move_retract_new error ::  为什么上一步没有棋子" << endl;
			return;
			//	pre_chess = -1;
		}

		data_itoxy(current, current_x, current_y);
		if (m_curr_color[current_x][current_y] == m_inf || m_curr_color[current_x][current_y] == 0) {
			//cout << "move_retract_new error ::   这个位置没有棋了？？？？" << endl;
			return;
		}

		m_chess_pos = m_curr_color[current_x][current_y];
		m_curr_color[current_x][current_y] = 0;

		if (m_chess_pos % 2 != selected_color) {
			//cout << "move_retract_new error ::   这个棋子颜色和位置不一样？？？？" << endl;
		}

		retract(m_chess_pos % 2, current_x, current_y);
		//cout << "在(" << current_x << ',' << current_y << ")处收回新棋子" << endl;
	//	isSelected = 1;

	}
}
// 悔棋：无须传入参数： 传出参数 ： p1 现在位置的棋子， p2=0 下棋模式，p2!=0 之前位置的棋子
void board::back(int& p1,int& p2)
{
	if (m_before[0].empty()){
		//cout << "error::　无棋可悔" << endl;
		return;
	}

	p1=(m_before[0].top()), p2=(m_before[1].top());
	m_before[0].pop();
	m_before[1].pop();

	if (m_before[0].empty()) pre_chess = -1;
	else 
	pre_chess = m_before[0].top();


	// 局面改变 
	who_step--;
	selected_color = (selected_color + 1) % 2;


	int cx(0), cy(0);
	
	data_itoxy(p1, cx, cy);

	m_chess_pos = m_curr_color[cx][cy];

	if (m_chess_pos % 2!=selected_color) {
		//cout << "back error;　颜色不一样？？？" << endl;
		return;
	}

	m_curr_color[cx][cy] = 0;
	if (!retract(selected_color, cx, cy))
	{
		//cout << "back error :：不能移棋？？？" << endl;
		return;
	}


	// p2!=0 表示移棋的模式
	if (p2) {
		int bx(0), by(0);
		data_itoxy(p2, bx, by);
		lay(selected_color, bx, by);
		m_curr_color[bx][by] = m_chess_pos;
		m_chess[m_chess_pos] = p2;
		//	m_chess_num--;
	}


	else {
		if (m_chess_pos != m_chess_num) {
			//cout << "error:: 删除的棋子不是最后下的棋子？？" << endl;
		}
		m_chess_num--;
		int b_x(0), b_y(0);
		data_itoxy(m_chess[m_chess_pos], b_x, b_y);
		//cout << "错误：在(" << b_x << ',' << b_y << ")处success" <<endl;
	}
	
}




////悔棋，p1：现在坐标，p2=为前坐标;mode=0下棋，1移棋
//bool board::back(bool mode, int p1, int p2) {
//	int current_x(0), current_y(0);
//	data_itoxy(p1, current_x, current_y);
//	int pre_x(0), pre_y(0);
//	data_itoxy(p2, pre_x, pre_y);
//	if (retract(static_cast<CHESS_COLOR>(m_curr_color[current_x][current_y] % 2), current_x, current_y)) {
//		if (mode == 0) {	//下棋
//			m_curr_color[current_x][current_y] = 0;
//			pre_chess = p2;
//			--m_chess_num;
//			return true;
//		}
//		else {	//移棋
//			m_curr_color[pre_x][pre_y] = m_curr_color[current_x][current_y];
//			m_curr_color[current_x][current_y] = 0;
//			lay(static_cast<CHESS_COLOR>(m_curr_color[pre_x][pre_y] % 2), pre_x, pre_y);
//			pre_chess = p2;
//			return true;
//		}
//	}
//	else {
//		string c_color;
//		switch (m_curr_color[current_x][current_y]%2) {
//		case WHITE:c_color = "后手"; break;
//		case BLACK:c_color = "先手"; break;
//		case NONE:c_color = "没有"; break;
//		default:c_color = "未知"; break;
//		}
//		cout << "不能拿起在(" << current_x << ',' << current_y << ")的" << c_color << "色棋子!" << endl;
//		return false;
//	}
//}

//放下棋子
bool board::lay(int color, int x, int y) {
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
bool board::retract(int color, int x, int y) {
	if (m_network[x][y][0].m_pioneer == NULL)return 0;
	limit_retract(color, x, y);
	// 在棋局链表中删除结点
	for (int i = 0; i < 4; i++) {

		if (m_network[x][y][i].m_pioneer == NULL) {
			//cout << "retract error ::为什么前驱结点是空？？？" << endl;
		}
		m_network[x][y][i].m_pioneer->m_next = m_network[x][y][i].m_next;
		if (!(m_network[x][y][i].m_next == NULL))
			m_network[x][y][i].m_next->m_pioneer = m_network[x][y][i].m_pioneer;
		m_network[x][y][i].setZero();
	}

	return 1;
}

bool board::network_dfs(int index, int & total, int color) {
	if (index) {	// index >0 表示有棋子
		int x = index % array_size;
		int y = index / array_size;

		

		// m_visited[x][y]=1 表示网络里面已经有这个棋子了
		if (!judge_border(x, y) || m_visited[x][y])return 0;
		//	if (m_visited[x][y])return 0;
		// isfind =1 表示已经找到一个成功网络，可以直接返回
		bool isfind(0);
		// (x,y)位置上的棋子是不同色的
		if ((m_curr_color[x][y] % 2) != color)return 0;

		if (color == BLACK) {// 黑棋
			// y==1 有重复的棋子在特区，
			if (total&&y == 1)return 0;
			//if ((m_curr_color[x][y] % 2) == WHITE)return 0;
			if (y == board_size) {
				if (total >= chess_need_to_win - 1) {
					isfind = 1;
				}
				else return 0;
			}
		}
		else {
			if (total&&x == 1)return 0;
		//	if ((m_curr_color[x][y] % 2) == BLACK)return 0;
			if (x == board_size) {
				if (total >= chess_need_to_win - 1) {
					isfind = 1;
				}
				else return 0;
			}
		}
		m_visited[x][y] = 1;
		total++;
		m_success.push(index);
		if (isfind)return 1;

		for (int i(0); i < 4; ++i) {
			// 注意判断指针非空，
			// change by dyy 8 10
			//if (m_network[x][y][i].m_pioneer != NULL)
				isfind = network_dfs(m_network[x][y][i].m_pioneer->m_index, total, color);
			if (isfind)return 1;
			// 注意判断指针非空，
			if (m_network[x][y][i].m_next != NULL)
				isfind = network_dfs(m_network[x][y][i].m_next->m_index, total, color);
			if (isfind)return 1;
		}
		m_visited[x][y] = 0;
		total--;
		m_success.pop();
		return 0;
	}
	else return 0;
}

bool board::judge_success(int color) {
	//棋子到达数目才可能形成网络
	if (who_step >= 2 * chess_need_to_win - 1) {
		//白棋连通上下 即y->(1,m_n) 黑棋连通左右  x->(1,m_n)
		// 黑棋起始位置为 x=1
		// 白棋起始位置为 Y=1 
		int total_chess(0);//成功网络上的棋子数
		node * temp_head = 0;
		if (color) {//黑棋，连通
			temp_head = m_head[1][1][3];
		}

		else {
			temp_head = m_head[1][1][1];
		}

		bool isfind = 0;
		while (temp_head->m_next) {
			total_chess = 0;
			memset(m_visited, 0, sizeof(m_visited));
			isfind = 0;

			// 清空之前的成功网络
			while (!m_success.empty())m_success.pop();
			isfind = network_dfs(temp_head->m_next->m_index, total_chess, color);
			if (isfind) {
				return 1;
			}
			temp_head = temp_head->m_next;
		}
	}
	return false;
}

//// 某一位为1 表示可以下棋，否则不能下
//unsigned long board::get_all_feasible()
//{
//
//	bool current_feasible[array_size][array_size];
//	memset(current_feasible, 0, sizeof(current_feasible));
//
//	int temp_x, temp_y;
//	for (int i = 1; i <= m_chess_num; i++) {// O(m_chess_num  )=O(20)
//		data_itoxy(i, temp_x, temp_y);
//		current_feasible[temp_x][temp_y] = 1;// 已经有棋子的地方不能下
//		if (i % 2==selected_color) {
//			
//			if (m_limit[selected_color][temp_x][temp_y] == m_inf2 + 1) {//这个棋子附近有一个同色棋子，附近不可以再下同色棋子
//				for (int j = 0; j < 4; j++)
//				{
//					current_feasible[temp_x + m_surround[j][0]][temp_x + m_surround[j][1]] = 1;
//
//				}
//			}
//		}
//		
//	}
//
//	unsigned long ans = 0;
//	for (int y = board_size; y >=1; y--) {
//		for (int x = board_size; x >= 1; x--)
//		{
//			ans >>= 1;
//			if (!current_feasible[x][y] && m_limit[selected_color][x][y] <= 1) {
//				ans |= 1;
//			}
//		}
//	}
//	
//	return ans;
//}

// first 表示某一位有没有棋子，second 表示这一位棋子是黑色的
pair<unsigned long, unsigned long> board::get_current_state()
{
	unsigned long temp_state(0), temp_color(0);
	int temp;
	for (int i = 1; i <= m_chess_num; i++) {
		temp = data2real(m_chess[i]);
		temp_state |= (1 << temp);
		if (i % 2) {
			temp_color |= (1 << temp);
		}
	}

	
	return pair<unsigned long, unsigned long>(temp_state,temp_color);
}
// 简单计算每个点周围的可见点数目
//int board::get_scores(int color)
//{
//	int scores[2] = { 0,0 };
//	int temp, x, y;
//	int total_next;
//	for (int i = 1; i <=chess_num; i++) {
//		temp = m_chess[i];
//		data_itoxy(temp, x, y);
//		temp = 0;
//		total_next = 0;
//		int x1, y1;
//		for (int i = 0; i < 4; i++)
//		{
//			if (m_network[x][y][i].m_pioneer != NULL) {
//				temp = m_network[x][y][i].m_pioneer->m_index;
//				data_itoxy(temp, x1, y1);
//				if (m_curr_color[x1][y1] < m_inf && (m_curr_color[x1][y1] % 2) == (m_curr_color[x][y] % 2)) {
//					total_next++;
//				}
//			}
//
//		}
//
//		scores[m_curr_color[x][y] % 2] += total_next;
//	}
//
//	return scores[color] - scores[(color + 1) % 2];
//
//	//return 0;
//}

//find back and ahead node at one time to reduce the time cost;
void board::GetAheadBackStone(int index, int direction, int & index_ahead, int & index_back)
{
	int x = 0, y = 0;
	data_itoxy(index, x, y);
	node * temp = m_head[x][y][direction];
	while (temp->m_next->m_index != index)
	{
		temp = temp->m_next;
	}
	if (temp->m_next->m_index == index)
	{
		index_ahead = -1;//没有前节点；
	}
	else
	{
		index_ahead = temp->m_index;
	}
	temp = temp->m_next->m_next;
	if (temp == nullptr)
	{
		index_back = -1;
	}
	else
	{
		index_back = temp->m_index;
	}
}




//judge whether to be 3-3 situation;
bool board::WhetherToThr(int index1, int index2)
{
	int index1_x = 0, index1_y = 0, index2_x = 0, index2_y = 0;
	data_itoxy(index1, index1_x, index1_y);
	data_itoxy(index2, index2_x, index2_y);
	if (index1_x == index2_x)
	{
		if (abs(index1_y - index2_y) == 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (index1_y == index2_y)
	{
		if (abs(index1_x - index2_x) == 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (abs(index1_x - index2_x) == 2)
	{
		if (abs(index1_y - index2_y) == 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//Evaluate the situation;
double board::get_scores_inside(int color)
{
	//棋局总分；
	double all_score = 0;
	//评价棋子的个数；
	int number_evaluated = 0;
	//judge m_chess_num to be an even or an odd;
	if ((m_chess_num & 1) == 1)
	{
		if (color == WHITE)
		{
			number_evaluated = (m_chess_num + 1) / 2;
		}
		else
		{
			number_evaluated = (m_chess_num - 1) / 2;
		}
	}
	else
	{
		number_evaluated = m_chess_num / 2;
	}
	//虽然for循环是遍历计算每个白棋的单独评分，
	//但是基于双向链表的查找，加上棋子的数目并不多，
	//所以他们的时间复杂度并不高:    O（stone_number*size*size/2）
	//size是棋盘的边长
	for (int i = 0; i < number_evaluated; ++i)
	{
		//单个评分；
		double self_score = 0;
		//记录三三边的个数；
		int number_thr_edge = 0;
		//记录八个方向中被阻挡的方向个数；可能为分数是因为每个方向完全阻死的可能性很小；
		double  number_blocked = 0.0;
		//确定评价点的坐标；
		int point_evaluated = 0;
		if (color == WHITE)
		{
			point_evaluated = 2 + 2 * i;
		}
		else
		{
			point_evaluated = 1 + 2 * i;
		}
		int index_evaluated = m_chess[point_evaluated];
		//四个大方向的遍历；
		for (int direction = 0; direction < 4; ++direction)
		{
			//通过双向链表得到第i个棋子的前面的棋子
			int ahead_stone = 0, back_stone = 0;
			GetAheadBackStone(index_evaluated, direction, ahead_stone, back_stone);
			if (ahead_stone != -1)
			{
				int ahead_stone_x = 0, ahead_stone_y = 0;
				data_itoxy(ahead_stone, ahead_stone_x, ahead_stone_y);
				//如果同色；
				if ((m_curr_color[ahead_stone_x][ahead_stone_y] % 2) == color)
				{
					//加上基准分1分，因为只要是相连的话就有了一定的优势；
					self_score += 1.0;
					//如果做相邻的棋子与之构成“三三”的一部分；评分应该高出很多，并且做一个记录；
					if (WhetherToThr(ahead_stone, index_evaluated))
					{
						self_score += 5.0;
						++number_thr_edge;
					}
					//没有形成3-3时；距离越长，评分越少，因为距离长的时候容易被阻断；
					else
					{
						int x_evaluated = 0, y_evaluated = 0;
						data_itoxy(index_evaluated, x_evaluated, y_evaluated);
						int dis = abs(x_evaluated - ahead_stone_x) + abs(y_evaluated - ahead_stone_y);
						//为了让棋子朝向自己对面的的特区方向；
						if (y_evaluated - ahead_stone_y > 0)
						{
							//更加朝向自己的特区方向；
							self_score += 10.0 / (dis + 1.0);
						}
						else
						{
							self_score += 5.0 / (dis + 1.0);
						}
						
					}
				}
				//异色；距离越远影响越小；
				else
				{
					int x_evaluated = 0, y_evaluated = 0;
					data_itoxy(index_evaluated, x_evaluated, y_evaluated);
					int dis = abs(x_evaluated - ahead_stone_x) + abs(y_evaluated - ahead_stone_y);
					//阻隔的力度更大；因为朝向了自己的特区方向被挡住；
					if (y_evaluated - ahead_stone_y > 0)
					{
						number_blocked += dis / 8.0;
					}
					else
					{
						number_blocked += (dis / 16.0);
					}
	
				}
			}


			//第i个棋子后面的棋子；
			if (back_stone != -1)
			{
				int back_stone_x = 0, back_stone_y = 0;
				data_itoxy(back_stone, back_stone_x, back_stone_y);
				//如果同色；
				if ((m_curr_color[back_stone_x][back_stone_y] % 2) == color)
				{
					//加上基准分1分，因为只要是相连的话就有了一定的优势；
					self_score += 1.0;
					//如果做相邻的棋子与之构成“三三”的一部分；评分应该高出很多，并且做一个记录；
					if (WhetherToThr(back_stone, index_evaluated))
					{
						self_score += 5.0;
						++number_thr_edge;
					}
					//没有形成3-3时；距离越长，评分越少，因为距离长的时候容易被阻断；
					else
					{
						int x_evaluated = 0, y_evaluated = 0;
						data_itoxy(index_evaluated, x_evaluated, y_evaluated);
						int dis = abs(x_evaluated - back_stone_x) + abs(y_evaluated - back_stone_y);
						//为了让棋子朝向自己对面的的特区方向；
						if (y_evaluated - back_stone_y > 0)
						{
							//更加朝向自己的特区方向；
							self_score += 10.0 / (dis + 1.0);
						}
						else
						{
							self_score += 5.0 / (dis + 1.0);
						}
					}
				}
				//异色；距离越远影响越小；
				else
				{
					int x_evaluated = 0, y_evaluated = 0;
					data_itoxy(index_evaluated, x_evaluated, y_evaluated);
					int dis = abs(x_evaluated - back_stone_x) + abs(y_evaluated - back_stone_y);

					//阻隔的力度更大；因为朝向了自己的特区方向被挡住；
					if (y_evaluated - back_stone_y > 0)
					{
						number_blocked += dis / 8.0;
					}
					else
					{
						number_blocked += (dis / 16.0);
					}
				}
			}
		}
		//下面考虑单个点的评分；形成三三
		if (number_thr_edge >= 2)
		{
			self_score += 20;
		}
		//number_blocked+1是考虑到有很多棋子周很多限制，不能下;例如：位于特区周围的时候，聚群的时候；（这里只是折中，没有仔细判断，所以只是加一）
		else
		{
			self_score *= (number_blocked + 1.0) / 8.0;
		}
		all_score += self_score;


	}

	double score_2 = 0;

	//看他们是否横向相连；
	for (auto i = 1; i<=number_evaluated; ++i)
	{

		int index_evaluated = m_chess[2*i-1];
		int ahead_stone = 0, back_stone = 0;
		bool IsConnected = false;
		for (int j = 0; j < 4; ++j)
		{
			GetAheadBackStone(index_evaluated, j, ahead_stone, back_stone);
			int x_ahead = 0, y_ahead = 0;
			int x_evaluated = 0, y_evaluated = 0;
			//判断前驱是否是行相连；
			if (ahead_stone != -1)
			{
				data_itoxy(ahead_stone, x_ahead, y_ahead);
			}
			data_itoxy(index_evaluated, x_evaluated, y_evaluated);
			if (ahead_stone != -1 && y_evaluated != y_ahead&&m_curr_color[x_ahead][y_ahead]==color)
			{
				IsConnected = true;
				break;
			}
			int x_back = 0, y_back = 0;
			//判断后继是否是行相连；
			if (back_stone != -1)
			{
				data_itoxy(back_stone, x_back, y_back);
			}
			data_itoxy(index_evaluated, x_evaluated, y_evaluated);
			if (back_stone != -1 && y_evaluated != y_back&&m_curr_color[x_back][y_back] == color)
			{
				IsConnected = true;
				break;
			}
		}
		if (IsConnected)
		{
			score_2 += 3.0;
		}

	}
	all_score += score_2;
	return all_score;
}


// 某一位为1 表示可以下棋，否则不能下
void board::cal_all_feasible()
{

	bool current_feasible[array_size][array_size];
	memset(current_feasible, 0, sizeof(current_feasible));

	int temp_x, temp_y;
	for (int i = 1; i <= m_chess_num; i++) {// O(m_chess_num  )=O(20)
		data_itoxy(m_chess[i], temp_x, temp_y);
		//	current_feasible[temp_x][temp_y] = 1;// 已经有棋子的地方不能下
		if (i % 2 == selected_color) {

			if (m_limit[selected_color][temp_x][temp_y] >= m_inf2 + 1) {//这个棋子附近有一个同色棋子，附近不可以再下同色棋子
				for (int j = 0; j < dir_count; j++)
				{
					current_feasible[temp_x + m_surround[j][0]][temp_y + m_surround[j][1]] = 1;

				}
			}
		}

	}
//	m_feasible.assign(board_size*board_size, 0);
	int x, y;
	//bool no = 0;
	for (int i = 0; i <board_size*board_size; i++) {
		m_feasible[i] = 0;
		real_itoxy(i, x, y);
		real2data(x, y);

	

		if (m_curr_color[x][y]) {
		}

		else if (m_limit[selected_color][x][y] >= m_inf) {
			//	no = 0;
		}
		else if (m_limit[selected_color][x][y] >= 2) {

		}
		else if (!current_feasible[x][y] /*&& m_limit[selected_color][x][y]<m_inf&& m_curr_color[x][y]==0*/) {
			//ans |= 1;
			//cout << "(" << x << "," << y << ")";
			feasible(selected_color, x, y);
			//	no = 1;
			m_feasible[i] = 1;
		}
		else m_feasible[i] = 0;


	}

	//cout << endl;
	int index;



}