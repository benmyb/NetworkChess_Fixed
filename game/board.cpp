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
	// ��������
	for (int i(1); i <= board_size; ++i) {
		//���岻�����������У����岻������������
		//������ͨ���� ��y->(1,m_n) ������ͨ����  x->(1,m_n)
		m_limit[1][1][i] = m_limit[1][board_size][i] = m_limit[0][i][1] = m_limit[0][i][board_size] = m_inf;
	}

	m_curr_color[1][1] = m_curr_color[1][board_size] = m_curr_color[board_size][1] = m_curr_color[board_size][board_size] = m_inf;//���ý���
	m_chess_num = 0;	// ��¼�������̵�������
	pre_chess = -1;	// ǰһ������
	selected_chess = -1;	// ��ѡ���ƶ�������
	selected_color = NONE;
	isSelected = false;
}

void board::init_link() {
	//��ʼ������ÿ�����ӵĽ��
	memset(m_network, 0, sizeof(m_network));
	// ��ʼ��ÿ������Ӧ�����̵�λ��
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int dir(0); dir < dir_count / 2; ++dir) {
				m_network[x][y][dir].m_index = x + y * array_size;
			}
		}
	}
	// ��ʼ������ÿ��λ�õ�4�����������ͷ��㣬ʱ�临�Ӷ�O(m_size*m_size*4)
	// direction ��ʾ��������ӷ��� x ,y ��ʾ������ʼλ�ã�Ҳ���ǿ�ͷ����λ�ã�����һ���յ�ͷ��㣬��������ɾ�����
	// direction �������� 	int next_step[m_direction][2] = { {-1,1},{0,1},{1,1},{1,0},{-1,0},{-1,-1},{0,-1},{1,-1}};
	int x(0), y(0), direction(0);
	for (int i(0); i < array_size; ++i) {
		// init   y= i  0<x<=m_n
		y = i;
		direction = 3;
		init_line_head(x, y, direction);

		// init  ��y-x=i  (1<=y<=m_n,1<=x<=m_n)
		direction = 2;
		init_line_head(x, y, direction);
	}
	y = 0;
	for (int i(0); i < array_size; ++i) {
		// init x+y=i; ��1<=y<=m_n,1<=x<=m_n��
		x = i;
		direction = 0;
		init_line_head(x, y, direction);

		// init x=i; ��1<=y<=m_n��
		direction = 1;
		init_line_head(x, y, direction);

		// init x-y=i; ��1<=y<=m_n,1<=x<=m_n��
		direction = 2;
		init_line_head(x, y, direction);
	}
	x = board_size + 1;
	direction = 0;
	for (int i(0); i < array_size; ++i) {
		// init x+y=m_n+1+i   ��1<=y<=m_n,1<=x<=m_n��
		y = i;
		init_line_head(x, y, direction);
	}
}

void board::init_line_head(int x, int y, int direction) {
	int count(0);	// ��¼�ǵڼ������
					//�������������ˣ����ܱ�û���õĽ����ΪһЩ����Ŀ�ͷ��㣬���ӽ��������ʣ����ö�̬��㣬�����ʼ��
	node* head = &m_network[x][y][direction];
	while (1) {
		//���ݷ������飬�����������һ������λ�ã�
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

	//������2������
	if (m_limit[color][x][y] >= 2)return 0;

	int count = 0;//��¼��Χ�м���ͬɫ����

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

//�������Ӻ�����Χ8���㣨�����Լ���λ�ã�+1
void board::limit_lay(CHESS_COLOR color, int x, int y) {
	// ����Ⱥ���жϣ� ����һ�����ӣ�����������Χlimit ����+1 �����Լ���λ�ü���һ������ֵ m_inf2

	int near_x(0), near_y(0);
	for (int i(0); i < dir_count; ++i) {
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// �����ж�Խ�磬��Ϊ���������̣���ʡ���жϵ�ʱ��
		++m_limit[color][near_x][near_y];
	}

	m_limit[color][x][y] += m_inf2;
}

//�������Ӻ�����Χ8���㣨�����Լ���λ�ã�-1
void board::limit_retract(CHESS_COLOR color, int x, int y) {
	// ����Ⱥ���жϣ� �ջ�һ�����ӣ�����������Χlimit ����-1 ���Լ���λ�ü�ȥһ������ֵ m_inf2

	int near_x(0), near_y(0);
	for (int i(0); i < dir_count; ++i) {
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// �����ж�Խ�磬��Ϊ���������̣���ʡ���жϵ�ʱ��
		--m_limit[color][near_x][near_y];
	}

	m_limit[color][x][y] -= m_inf2;
}

//��Ϸ�ٿ�
void board::restart() {
	init();
	// ��ʼ�� m_doubly_link  m_head
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int j(0); j < 4; ++j) {
				m_network[x][y][j].setZero();
				m_head[x][y][j]->setZero();
			}
		}
	}
}

//����
bool board::step(CHESS_COLOR color, int x, int y) {
	string c_color;
	switch (color) {
	case WHITE:c_color = "��"; break;
	case BLACK:c_color = "��"; break;
	case NONE:c_color = "û����"; break;
	default:c_color = "δ֪"; break;
	}
	if (lay(color, x, y)) {
		m_chess[++m_chess_num] = data_xytoi(x, y);
		m_curr_color[x][y] = m_chess_num;
		cout << c_color << "����(" << x << ',' << y << ")����" << endl;
		pre_chess = data_xytoi(x, y);
		return true;
	}
	else {
		cout << "������(" << x << ',' << y << ")�����ܷ���" << c_color << "ɫ����" << endl;
		return false;
	}
}

//�����һ����ѡ��Ҫ�ƶ������Ӳ�ɾ����
bool board::move_select(int x, int y) {
	if (m_curr_color[x][y]) {
		string c_color;
		switch (m_curr_color[x][y]%2) {
		case WHITE:c_color = "��"; break;
		case BLACK:c_color = "��"; break;
		default:c_color = "δ֪"; break;
		}
		selected_color = static_cast<CHESS_COLOR>(m_curr_color[x][y] % 2);
		selected_chess = data_xytoi(x, y);
		isSelected = true;
		retract(selected_color, x, y);
		cout << "ѡ����(" << x << ',' << y << ")��" << c_color << "ɫ�����ƶ�" << endl;
		return true;
	}
	else {
		cout << "������(" << x << ',' << y << ")��û�����ӿɹ�ѡ��" << endl;
		return false;
	}
}

//����ڶ���������λ������
bool board::move_step(int x, int y) {
	if (isSelected) {
		string c_color;
		switch (selected_color) {
		case WHITE:c_color = "��"; break;
		case BLACK:c_color = "��"; break;
		case NONE:c_color = "û����"; break;
		default:c_color = "δ֪"; break;
		}
		if (lay(selected_color, x, y)) {
			int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);
			m_chess[m_curr_color[b_x][b_y]] = data_xytoi(x, y);
			m_curr_color[x][y] = m_curr_color[b_x][b_y];
			cout << c_color << "����(" << b_x << ',' << b_y << ")�ƶ���(" << x << ',' << y << ')' << endl;
			isSelected = false;
			pre_chess = data_xytoi(x, y);
			return true;
		}
		else {
			int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);
			cout << "������(" << b_x << ',' << b_y << ")�����ܷ���" << c_color << "ɫ���ӣ�����ʧ��" << endl;
			return false;
		}
	}
	else {
		cout << "����δѡ�����ӣ�" << endl;
		return false;
	}
}

//���壬p1���������꣬p2=Ϊǰ����;mode=0���壬1����
bool board::back(bool mode, int p1, int p2) {
	int current_x(0), current_y(0);
	data_itoxy(p1, current_x, current_y);
	int pre_x(0), pre_y(0);
	data_itoxy(p2, pre_x, pre_y);
	if (retract(static_cast<CHESS_COLOR>(m_curr_color[current_x][current_y] % 2), current_x, current_y)) {
		if (mode == 0) {	//����
			m_curr_color[current_x][current_y] = 0;
			pre_chess = p2;
			--m_chess_num;
			return true;
		}
		else {	//����
			m_curr_color[pre_x][pre_y] = m_curr_color[current_x][current_y];
			lay(static_cast<CHESS_COLOR>(m_curr_color[pre_x][pre_y] % 2), pre_x, pre_y);
			pre_chess = p2;
			return true;
		}
	}
	else {
		string c_color;
		switch (m_curr_color[current_x][current_y]%2) {
		case WHITE:c_color = "��"; break;
		case BLACK:c_color = "��"; break;
		case NONE:c_color = "û����"; break;
		default:c_color = "δ֪"; break;
		}
		cout << "����������(" << current_x << ',' << current_y << ")��" << c_color << "ɫ����!" << endl;
		return false;
	}
}

//��������
bool board::lay(CHESS_COLOR color, int x, int y) {
	if (feasible(color, x, y)) {
		limit_lay(color, x, y);
		// �������������
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

//�ջ�����
bool board::retract(CHESS_COLOR color, int x, int y) {
	limit_retract(color, x, y);
	// �����������ɾ�����
	for (int i = 0; i < 4; i++) {
		m_network[x][y][i].m_pioneer->m_next = m_network[x][y][i].m_next;
		if (!(m_network[x][y][i].m_next == NULL))
			m_network[x][y][i].m_next->m_pioneer = m_network[x][y][i].m_pioneer;
		m_network[x][y][i].setZero();
	}
}

