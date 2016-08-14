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
	while (!m_success.empty())m_success.pop();
	// ���¿�ʼ�����m_before
	while (!m_before[0].empty())m_before[0].pop();

	while (!m_before[1].empty())m_before[1].pop();
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
	selected_color = 1;
	isSelected = false;

	// ��ʼ������m_chess��λ����0����û������
	m_chess_pos = 0;
	who_step = 0;

	
}

void board::init_link() {
	//��ʼ������ÿ�����ӵĽ��
	memset(m_network, 0, sizeof(m_network));
	// ��ʼ��ÿ������Ӧ�����̵�λ��
	for (int x(1); x <= board_size; ++x) {
		for (int y(1); y <= board_size; ++y) {
			for (int dir(0); dir < dir_count / 2; ++dir) {
				m_network[x][y][dir].m_index =data_xytoi(x,y)/* x + y * array_size*/;
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
	// O(m_n/2) =O(4)
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

bool board::feasible(int color, int x, int y) {
	// judge_border ��ʾ�Ƿ�Խ��
	//  m_curr_color ��Ϊ0 ����ʾ�����ӣ���Ϊ����
	if (!judge_border(x, y) || m_curr_color[x][y] || m_limit[color][x][y] >= m_inf)return false;

	//������2������
	if (m_limit[color][x][y] >= 2)return 0;

	int count = 0;//��¼��Χ�м���ͬɫ����

	int near_x, near_y;
	for (int i(0); i < dir_count; ++i)
	{
		near_x = x + m_surround[i][0];
		near_y = y + m_surround[i][1];
		// ��ʾ��Χ��һ��ͬɫ���ӣ��������������ΧҲ��һ��ͬɫ����
		if (m_limit[color][near_x][near_y] == m_inf2 + 1)return false;
	}
	return true;
}

//�������Ӻ�����Χ8���� ���Լ���λ��+m_inf2
void board::limit_lay(int color, int x, int y) {
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

//�������Ӻ�����Χ8���㣬�Լ���λ��-m_inf2
void board::limit_retract(int color, int x, int y) {
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
	// ��ʼ�� m_network  m_head
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

//����
bool board::step_inside(int x, int y) {
	

	if (selected_color != (m_chess_num + 1) % 2) {
		cout << "step_inside error:����ɫ�Բ��ϣ�������" << endl;
		return 0;
	}

	CHESS_COLOR color = static_cast<CHESS_COLOR>((m_chess_num + 1) % 2);
	string c_color;
	switch (color) {
	case WHITE:c_color = "����"; break;
	case BLACK:c_color = "����"; break;
	case NONE:c_color = "û��"; break;
	default:c_color = "δ֪"; break;
	}
	//who_step++;

	if (lay(color, x, y)) {
		m_chess_pos=++m_chess_num;
		m_chess[m_chess_pos] = data_xytoi(x, y);
		m_curr_color[x][y] = m_chess_pos;
		cout << c_color << "����(" << x << ',' << y << ")����" << endl;
		pre_chess = data_xytoi(x, y);
		return true;
	}
	else {
		cout << "������(" << x << ',' << y << ")�����ܷ���" << c_color << "����" << endl;
		return false;
	}
}

CHESS_COLOR board::moving_color()
{
	return CHESS_COLOR(selected_color);
}

bool board::get_before_color()
{
	return !selected_color;
	//return false;
}

//�����һ����ѡ��Ҫ�ƶ������Ӳ�ɾ����
bool board::move_select(int x, int y) {

	if (m_curr_color[x][y] == m_inf || m_curr_color[x][y] == 0) {
		cout << "move select error :: ���λ��û�����ˣ�������" << endl;
		return 0;
	}

	if (isSelected) {
		cout << "move select error ::  �ϴε����ӻ�û�з���" << endl;
		return 0;
	}

	if (m_curr_color[x][y] % 2 != selected_color) {
		cout << "move select error :: �����ɫ�������ƶ�" << endl;
		return 0;
	}

	if (m_curr_color[x][y]) {
		string c_color;
		switch (m_curr_color[x][y]%2) {
		case WHITE:c_color = "����"; break;
		case BLACK:c_color = "����"; break;
		case NONE:c_color = "û��"; break;
		default:c_color = "δ֪"; break;
		}
		//selected_color = m_curr_color[x][y] % 2;
		m_chess_pos = m_curr_color[x][y];
		m_curr_color[x][y] = 0;
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
		case WHITE:c_color = "����"; break;
		case BLACK:c_color = "����"; break;
		case NONE:c_color = "û��"; break;
		default:c_color = "δ֪"; break;
		}
		if (lay(selected_color, x, y)) {
			int b_x(0), b_y(0);
			data_itoxy(selected_chess, b_x, b_y);

			if (m_chess_pos == 0) {
				cout << "move_step error:��λ��m_chess_pos�����ˣ�Ϊʲô����" << endl;
			}
			if (m_chess_pos % 2 != selected_color) {
				cout << "move_step error ::���ŵ�λ�ú���ɫ��һ����Ϊʲô����" << endl;
			}
			m_chess[m_chess_pos] = data_xytoi(x, y);
			m_curr_color[x][y] = m_chess_pos;
			m_chess_pos = 0;
			//m_curr_color[b_x][b_y] = 0;
			cout << c_color << "����(" << b_x << ',' << b_y << ")�ƶ���(" << x << ',' << y << ')' << endl;
			isSelected = false;
			pre_chess = data_xytoi(x, y);
			

			// �ж������޸�
			selected_color = (selected_color + 1) % 2;
			who_step++;

			m_before[0].push(pre_chess);
			m_before[1].push( selected_chess);

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

//������������������壬�����ӷŻ�ȥ
void board::move_cancel()
{


	if (!isSelected) {
		cout << "move cancel error : û��ѡ�����ӣ���ô�Ż�ȥ???" << endl;
	}

	int x, y;

	data_itoxy(selected_chess,x,y);

	isSelected = false;
	if (lay(selected_color, x, y)){
		string c_color;
		switch (selected_color) {
		case WHITE:c_color = "����"; break;
		case BLACK:c_color = "����"; break;
		case NONE:c_color = "û��"; break;
		default:c_color = "δ֪"; break;
		}

		cout << c_color << "�������ӷŻص�(" << x << ',' << y << ')' << endl;

	}
	else {
		cout << "������֪��Ϊʲô�Ų���ȥ���ӣ������ں�������" << endl;
	}

	//return false;
}

// ����ɺ��ӣ� �ջ��ƶ���λ�õ�����
void board::move_retract_new()
{
	if (isSelected) {
		cout << "move_retract_new error :: ֮ǰ������û�з���" << endl;
		return;
	}

	if (who_step <= 2 * chess_num) {
		cout << "move_retract_new error :: ģʽ���ԣ���һ��������" << endl;
		return;
	}

	else {

		isSelected = 1;


		// �ж������޸� ,�൱�ڷ�����һ��
		selected_color = (selected_color + 1) % 2;
		who_step--;

		selected_chess = m_before[1].top();
		m_before[1].pop();
		
		int current(m_before[0].top()), current_x(0), current_y(0);
		m_before[0].pop();
		if (!m_before[0].empty())
			pre_chess = m_before[0].top();
		else {
			cout << "move_retract_new error ::  Ϊʲô��һ��û������" << endl;
			return;
			//	pre_chess = -1;
		}

		data_itoxy(current, current_x, current_y);
		if (m_curr_color[current_x][current_y] == m_inf || m_curr_color[current_x][current_y] == 0) {
			cout << "move_retract_new error ::   ���λ��û�����ˣ�������" << endl;
			return;
		}

		m_chess_pos = m_curr_color[current_x][current_y];
		m_curr_color[current_x][current_y] = 0;

		if (m_chess_pos % 2 != selected_color) {
			cout << "move_retract_new error ::   ���������ɫ��λ�ò�һ����������" << endl;
		}

		retract(m_chess_pos % 2, current_x, current_y);
	//	isSelected = 1;

	}
}
// ���壺���봫������� �������� �� p1 ����λ�õ����ӣ� p2=0 ����ģʽ��p2!=0 ֮ǰλ�õ�����
void board::back(int& p1,int& p2)
{
	if (m_before[0].empty()){
		cout << "error::������ɻ�" << endl;
		return;
	}

	p1=(m_before[0].top()), p2=(m_before[1].top());
	m_before[0].pop();
	m_before[1].pop();

	if (m_before[0].empty()) pre_chess = -1;
	else 
	pre_chess = m_before[0].top();


	// ����ı� 
	who_step--;
	selected_color = (selected_color + 1) % 2;


	int cx(0), cy(0);
	
	data_itoxy(p1, cx, cy);

	m_chess_pos = m_curr_color[cx][cy];

	if (m_chess_pos % 2!=selected_color) {
		cout << "back error;����ɫ��һ��������" << endl;
		return;
	}

	m_curr_color[cx][cy] = 0;
	if (!retract(selected_color, cx, cy))
	{
		cout << "back error :���������壿����" << endl;
		return;
	}


	// p2!=0 ��ʾ�����ģʽ
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
			cout << "error:: ɾ�������Ӳ�������µ����ӣ���" << endl;
		}
		m_chess_num--;
	}
}




////���壬p1���������꣬p2=Ϊǰ����;mode=0���壬1����
//bool board::back(bool mode, int p1, int p2) {
//	int current_x(0), current_y(0);
//	data_itoxy(p1, current_x, current_y);
//	int pre_x(0), pre_y(0);
//	data_itoxy(p2, pre_x, pre_y);
//	if (retract(static_cast<CHESS_COLOR>(m_curr_color[current_x][current_y] % 2), current_x, current_y)) {
//		if (mode == 0) {	//����
//			m_curr_color[current_x][current_y] = 0;
//			pre_chess = p2;
//			--m_chess_num;
//			return true;
//		}
//		else {	//����
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
//		case WHITE:c_color = "����"; break;
//		case BLACK:c_color = "����"; break;
//		case NONE:c_color = "û��"; break;
//		default:c_color = "δ֪"; break;
//		}
//		cout << "����������(" << current_x << ',' << current_y << ")��" << c_color << "ɫ����!" << endl;
//		return false;
//	}
//}

//��������
bool board::lay(int color, int x, int y) {
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
bool board::retract(int color, int x, int y) {
	if (m_network[x][y][0].m_pioneer == NULL)return 0;
	limit_retract(color, x, y);
	// �����������ɾ�����
	for (int i = 0; i < 4; i++) {

		if (m_network[x][y][i].m_pioneer == NULL) {
			cout << "retract error ::Ϊʲôǰ������ǿգ�����" << endl;
		}
		m_network[x][y][i].m_pioneer->m_next = m_network[x][y][i].m_next;
		if (!(m_network[x][y][i].m_next == NULL))
			m_network[x][y][i].m_next->m_pioneer = m_network[x][y][i].m_pioneer;
		m_network[x][y][i].setZero();
	}

	return 1;
}

bool board::network_dfs(int index, int & total, int color) {
	if (index) {	// index >0 ��ʾ������
		int x = index % array_size;
		int y = index / array_size;

		

		// m_visited[x][y]=1 ��ʾ���������Ѿ������������
		if (!judge_border(x, y) || m_visited[x][y])return 0;
		//	if (m_visited[x][y])return 0;
		// isfind =1 ��ʾ�Ѿ��ҵ�һ���ɹ����磬����ֱ�ӷ���
		bool isfind(0);
		// (x,y)λ���ϵ������ǲ�ͬɫ��
		if ((m_curr_color[x][y] % 2) != color)return 0;

		if (color == BLACK) {// ����
			// y==1 ���ظ���������������
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
			// ע���ж�ָ��ǿգ�
			// change by dyy 8 10
			//if (m_network[x][y][i].m_pioneer != NULL)
				isfind = network_dfs(m_network[x][y][i].m_pioneer->m_index, total, color);
			if (isfind)return 1;
			// ע���ж�ָ��ǿգ�
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
	//���ӵ�����Ŀ�ſ����γ�����
	if (who_step >= 2 * chess_need_to_win - 1) {
		//������ͨ���� ��y->(1,m_n) ������ͨ����  x->(1,m_n)
		// ������ʼλ��Ϊ x=1
		// ������ʼλ��Ϊ Y=1 
		int total_chess(0);//�ɹ������ϵ�������
		node * temp_head = 0;
		if (color) {//���壬��ͨ
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

			// ���֮ǰ�ĳɹ�����
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

// ĳһλΪ1 ��ʾ�������壬��������
unsigned long board::get_all_feasible()
{

	bool current_feasible[array_size][array_size];
	memset(current_feasible, 0, sizeof(current_feasible));

	int temp_x, temp_y;
	for (int i = 1; i <= m_chess_num; i++) {// O(m_chess_num  )=O(20)
		data_itoxy(i, temp_x, temp_y);
		current_feasible[temp_x][temp_y] = 1;// �Ѿ������ӵĵط�������
		if (i % 2==selected_color) {
			
			if (m_limit[selected_color][temp_x][temp_y] == m_inf2 + 1) {//������Ӹ�����һ��ͬɫ���ӣ���������������ͬɫ����
				for (int j = 0; j < 4; j++)
				{
					current_feasible[temp_x + m_surround[j][0]][temp_x + m_surround[j][1]] = 1;

				}
			}
		}
		
	}

	unsigned long ans = 0;
	for (int y = board_size; y >=1; y--) {
		for (int x = board_size; x >= 1; x--)
		{
			ans >>= 1;
			if (!current_feasible[x][y] && m_limit[selected_color][x][y] <= 1) {
				ans |= 1;
			}
		}
	}
	
	return ans;
}

// first ��ʾĳһλ��û�����ӣ�second ��ʾ��һλ�����Ǻ�ɫ��
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
// �򵥼���ÿ������Χ�Ŀɼ�����Ŀ
int board::get_scores(int color)
{
	int scores[2];
	int temp, x, y;
	int total_next;
	for (int i = 1; i <=chess_num; i++) {
		temp = m_chess[i];
		data_itoxy(temp, x, y);
		temp = 0;
		total_next = 0;
		int x1, y1;
		for (int i = 0; i < 4; i++)
		{
			temp = m_network[x][y][i].m_pioneer->m_index;
			data_itoxy(temp, x1, y1);
			if (m_curr_color[x1][y1] < m_inf && (m_curr_color[x1][y1] % 2) == (m_curr_color[x][y] % 2)) {
				total_next++;
			}
		}

		scores[m_curr_color[x][y] % 2] += total_next;
	}

	return scores[color] - scores[(color + 1) % 2];

	//return 0;
}


