#include "manager.h"

manager::manager() {

}

void manager::restart() {
	m_board.restart();
	while (!m_before[0].empty())m_before[0].pop();
	while (!m_before[1].empty())m_before[1].pop();
	while (!m_success.empty())m_success.pop();
	who_step = 0;
}

void manager::step(int x, int y) {
	if (white_is_first) {
		if (who_step % 2)m_board.step(WHITE, x, y);
		else m_board.step(BLACK, x, y);
		++who_step;
	}
	else {
		if (who_step % 2)m_board.step(BLACK, x, y);
		else m_board.step(WHITE, x, y);
		++who_step;
	}
}

void manager::select(int x, int y) {
	m_board.move_select(x, y);
	if (white_is_first) {
		if (who_step % 2)moving_color = WHITE;
		else moving_color = BLACK;
	}
	else {
		if (who_step % 2)moving_color = BLACK;
		else moving_color = WHITE;
	}
}

void manager::move_step(int x, int y) {
	m_board.move_step(x, y);
}

void manager::back(bool mode) {
	if (m_before[0].empty())return;
	if (mode) {
		int p1(m_before[0].top()), p2(m_before[1].top());
		m_board.back(mode, p1, p2);
		m_before[0].pop();
		m_before[1].pop();
		--who_step;
	}
	else {
		int p1(m_before[0].top()), p2(m_before[1].top());
		m_board.back(mode, p1, p2);
		m_before[0].pop();
		m_before[1].pop();
		--who_step;
	}
}

bool manager::network_dfs(int index, int & total, CHESS_COLOR color) {
	if (index) {	// pos >0 ��ʾ������
		int x = index % array_size;
		int y = index / array_size;
		if (!m_board.judge_border(x, y) || m_visited[x][y])return 0;
		//	if (m_visited[x][y])return 0;
		bool isfind(0);
		if (color==BLACK) {// ����
			// x==1 ���ظ���������������
			if (total&&y == 1)return 0;
			if ((m_board.m_curr_color[x][y].color) == WHITE)return 0;
			if (y == board_size) {
				if (total >= chess_need_to_win - 1) {
					isfind = 1;
				}
				else return 0;
			}
		}
		else {
			if (total&&x == 1)return 0;
			if ((m_board.m_curr_color[x][y].color) == BLACK)return 0;
			if (x == board_size) {
				if (total >= chess_need_to_win - 1) {
					isfind = 1;
				}
				else return 0;
			}
			/*		if (!(m_situation[x][y] % 2) && !m_visited[x][y] && y != 1)
			{
			if (total >= m_success_chess_num&&y == m_n) {
			isfind = 1;

			}
			}*/
		}
		m_visited[x][y] = 1;
		total++;
		m_success.push(index);
		if (isfind)return 1;
		for (int i(0); i < 4; ++i) {
			// ע���ж�ָ��ǿգ�
			if (m_board.m_network[x][y][i].m_pioneer != NULL)
				isfind = network_dfs(m_board.m_network[x][y][i].m_pioneer->m_index, total, color);
			if (isfind)return 1;
			if (m_board.m_network[x][y][i].m_next != NULL)
				isfind = network_dfs(m_board.m_network[x][y][i].m_next->m_index, total, color);
			if (isfind)return 1;
		}
		m_visited[x][y] = 0;
		total--;
		m_success.pop();
		return 0;
	}
	else return 0;
}

bool manager::judge_success(CHESS_COLOR color) {
	//���ӵ�����Ŀ�ſ����γ�����
	if (who_step >= 2 * chess_need_to_win - 1) {
		//������ͨ���� ��y->(1,m_n) ������ͨ����  x->(1,m_n)
		// ������ʼλ��Ϊ x=1
		// ������ʼλ��Ϊ Y=1 
		int total_chess(0);//�ɹ������ϵ�������
		node * temp_head = 0;
		if (color) {//���壬��ͨ
			temp_head = m_board.m_head[1][1][3];
		}

		else {
			temp_head = m_board.m_head[1][1][1];
		}

		bool isfind = 0;
		while (temp_head->m_next) {
			total_chess = 0;
			memset(m_visited, 0, sizeof(m_visited));
			isfind = 0;
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
