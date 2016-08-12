#include "computer.h"

void computer::getxy(int & x, int & y)
{
}

void computer::step(int & x, int & y, int & x1, int & y2, int mode)
{
}

double computer::alpha_beta(int depth, double alpha, double beta, CHESS_COLOR color,int father,bool is_alpha)
{

	m_state[++m_node_num] = m_current_state;

	m_tree.push_back(father);
	if (m_manager->judge_success(color))
	{

		m_scores[m_node_num] = double_inf;
		return double_inf;
	}
	if (m_manager->judge_success(static_cast<CHESS_COLOR>((color + 1) % 2)))
	{
		m_scores[m_node_num] = -double_inf;
		return -double_inf;
	}
	if (depth == 0) {
		return m_scores[m_node_num] = m_manager->get_scores(color);
	}
	double scores;
	if (is_alpha)scores = -double_inf;
	else scores = double_inf;
	

	if (m_manager->get_mode()) {

	}

	else {
	
		unsigned long int temp_feasible(m_manager->available(color));
		int temp_x(0), temp_y(0);
		for (int i = 0; i < board_size*board_size; i++)
		{
			if (temp_feasible&(1 << i)) {
				real_itoxy(i, temp_x, temp_y);
				m_manager->step(temp_x, temp_y);
				m_current_state.first |= (1 << i);
				if (color % 2) m_current_state.second |= (1 << i);
				alpha_beta(depth-1,  alpha, beta, static_cast<CHESS_COLOR>((color+1)%2), m_node_num, !is_alpha);

				m_manager->back(0);
				// not finished 
			}
		}
	}

	
	
}

void computer::step(int & x, int & y, int & x1, int & y2,int mode)
{
	
}
