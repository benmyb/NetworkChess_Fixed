#include "computer.h"

//void computer::getxy(int & x, int & y)
//{
//}
//
//void computer::step(int & x, int & y, int & x1, int & y2, int mode)
//{
//}

void computer::step(int & pp1, int & pp2)
{
	search_init();
	while (!m_possible.empty())m_possible.pop();
	alpha_beta(m_level, -double_inf, double_inf, 0, m_color, 0, 0);
	
	pp1 = m_possible.top().p1;
	pp2 = m_possible.top().p2;
	int cx(0), cy(0);
	data_itoxy(pp1, cx, cy);
	if (pp2) {
		int bx(0), by(0);
		data_itoxy(pp2, bx, by);
		if (m_manager->move_select(bx, by)) {
			if (!m_manager->move_step(cx, cy)) {
				cout << "博弈树错了？？不选移动第二步选的点" << endl;
			}

		}

		else cout << "博弈树错了？？不选移动第一步选的点" << endl;
	}
	else {
		if (!m_manager->step(cx, cy)) {
			cout << "博弈树错了？？不能下棋" << endl;
		}
	}

	

}

double computer::alpha_beta(int depth, double alpha, double beta,int father,int color,int p1,int p2)
{

	m_state[++m_node_num] = m_current_state;

	m_tree.push_back(father);
	double ans = 0;
	if (m_manager->judge_success(m_color))
	{

		ans= m_scores[m_node_num] = double_inf;
	}
	else if (m_manager->judge_success((m_color+1)%2))
	{
		ans= m_scores[m_node_num] = -double_inf;
	}
	else if (depth == 0) {
		ans= m_scores[m_node_num] = m_manager->get_scores(m_color);
	}
	
	else {
		if (m_manager->get_mode()) {
			int bx(0), by(0);
			int b_data(0), color(0), c_data(0);
			double ret(0);
			bool cut(0);
			color = m_manager->moving_color();

			for (int i = 1 + (i == color); i <= m_manager->m_chess_num; i += 2) {
				data_itoxy(m_manager->m_chess[i], bx, by);
				if (m_manager->move_select(bx, by)) {

					b_data = data2real(data_xytoi(bx, by));
					m_current_state.first |= 1 << b_data;
					m_current_state.first |= color << b_data;
					for (int x = 1; x <= board_size; x++)
					{
						for (int y = 1; y <= board_size; y++)
						{
							if (m_manager->move_step(x, y)) {
								c_data = data2real(data_xytoi(x, y));
								m_current_state.first |= 1 << c_data;
								m_current_state.first |= color << c_data;
								ret = alpha_beta(depth - 1, alpha, beta, m_node_num, !color, data_xytoi(x, y), data_xytoi(bx, by));
								m_manager->move_retract_new();
								m_current_state.first |= 1 << c_data;
								m_current_state.first |= color << c_data;

								if (color == m_color) {// 极大值点：：
									alpha = max(alpha, ret);

									if (beta <= alpha) {// beta cut-off

										cut = 1;
										break;
									}
								}
								else {
									beta = min(beta, ret);

									if (beta <= alpha) {
										cut = 1;
										break;// alpha cut-off
									}
								}
							}
						}
						if (cut)break;
					}


					m_manager->move_cancel();
					m_current_state.first |= 1 << b_data;
					m_current_state.first |= color << b_data;
					if (cut)break;
				}
				if (cut)break;
			}
		}

		else {

			int temp1, temp2;
			int color, data;
			bool cut = 0;
			double ret(0);
			color = m_manager->moving_color();
			for (int x = 1; x <= board_size; x++)
			{

				for (int y = 1; y <= board_size; y++)
				{


					if (m_manager->step(x, y)) {
						data = data2real(data_xytoi(x, y));
						// 状态改变
						m_current_state.first |= 1 << data;
						m_current_state.first |= color << data;

						ret = alpha_beta(depth - 1, alpha, beta, m_node_num, !color, data_xytoi(x, y), 0);

						m_manager->back(temp1, temp2);
						// 状态恢复
						m_current_state.first |= 1 << data;
						m_current_state.first |= color << data;

						if (color == m_color) {// 极大值点：：
							alpha = max(alpha, ret);
							if (beta <= alpha) {// beta cut-off
								cut = 1;
								break;
							}
						}

						else {
							beta = min(beta, ret);
							if (beta <= alpha) {
								cut = 1;
								break;
								//return beta;// alpha cut-off
							}
						}
					}
				}
				if (cut)break;
			}

		}


		if (m_color == color) {// 极大值点：：
			ans = m_scores[m_node_num] = alpha;
		}
		else ans= m_scores[m_node_num] = beta;
	}

	if (father == 1) {
		m_possible.push(point(p1, p2, ans));

	}
	return ans;
}
