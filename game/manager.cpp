#include "manager.h"

manager::manager() {

}

void manager::restart() {
	m_board.restart();
	while (!m_before[0].empty())m_before[0].pop();
	while (!m_before[1].empty())m_before[1].pop();
	who_step = 0;
}

void manager::step(int x, int y) {
	if (white_is_first) {
		if (who_step % 2 == WHITE)m_board.step(WHITE, x, y);
		else m_board.step(BLACK, x, y);
		++who_step;
	}
	else {
		if (who_step % 2 == BLACK)m_board.step(BLACK, x, y);
		else m_board.step(WHITE, x, y);
		++who_step;
	}
}

void manager::select(int x, int y) {
	m_board.move_select(x, y);
	if (white_is_first) {
		if (who_step % 2 == WHITE)moving_color = WHITE;
		else moving_color = BLACK;
	}
	else {
		if (who_step % 2 == BLACK)moving_color = BLACK;
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


bool manager::judge_success(CHESS_COLOR color) {
	m_board.success_clear();
	if (m_board.judge_success(who_step, color))return true;
	else return false;
}
