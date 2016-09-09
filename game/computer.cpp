#include "computer.h"



bool computer::step(int & pp1, int & pp2) {
	//下第一部的时候；
	if (m_manager->m_chess_num==0)
	{
		//构造随机数为了有下棋的自由度；
		int rand_int = rand() % 2;
		if (rand_int == 0)
		{
			//背靠自己的特区；
			m_manager->step(2, 2);
		}
		else
		{
			m_manager->step(6, 2);
		}
		return 1;
	}
	//下第二个；
	else if (m_manager->m_chess_num == 2)
	{
		if (m_manager->GetColor(4, 2) == 0)
		{
			m_manager->step(4, 2);
		}
		else
		{
			m_manager->step(4, 4);
		}
		return 1;
	}
	//下第三个；
	else if (m_manager->m_chess_num == 4)
	{
		if (m_manager->GetColor(2, 2) == 1)
		{
			if ((m_manager->GetColor(4,2)%2) == 1)
			{
				if (m_manager->GetColor(2, 4) == 0)
				{
					m_manager->step(2, 4);
				}
				else if (m_manager->GetColor(4,4)==0)
				{
					m_manager->step(4, 4);
				}
				else
				{
					m_manager->step(4, 6);
				}
			}
			else
			{
				if (m_manager->GetColor(4,2)== 0)
				{
					m_manager->step(4,2);
				}
				else if (m_manager->GetColor(2,4)== 0)
				{
					m_manager->step(2,4);
				}
				else
				{
					m_manager->step(4, 6);
				}
			}
		}
		else
		{
			if ((m_manager->GetColor(4, 2)%2) == 1)
			{
				if (m_manager->GetColor(4, 4)== 0)
				{
					m_manager->step(4, 4);
				}
				else if (m_manager->GetColor(6, 4)== 0)
				{
					m_manager->step(6, 4);
				}
				else
				{
					m_manager->step(6, 6);
				}
			}
			else
			{
				if (m_manager->GetColor(4, 2)== 0)
				{
					m_manager->step(4, 2);
				}
				else if (m_manager->GetColor(6, 4)== 0)
				{
					m_manager->step(6, 4);
				}
				else
				{
					m_manager->step(6, 6);
				}
			}
		}
		return 1;
	}
	if (m_manager->judge_success((m_color + 1) % 2)) {
		return 0;
	}
	search_init();
	m_possible.clear();
	//while (!m_possible.empty())m_possible.pop();
	alpha_beta(m_level, -double_inf, double_inf, 0, m_color, 0, 0);
	if (m_possible.empty()) {
		return 0;
	}

	int rand = m_u_double(m_e)*m_possible.size();

	pp1 = m_possible[rand].p1;
	pp2 = m_possible[rand].p2;
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


	return 1;
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
			int i = 0, x, y;
			for (i = 1+(i==color) ; i <= m_manager->m_chess_num; i += 2) {
				data_itoxy(m_manager->m_chess[i], bx, by);

				if (m_manager->move_select(bx, by)) {
					m_manager->cal_all_feasible();
					for (int i = 0; i < board_size*board_size; i++) {
						if (m_manager->get_all_feasible()[i]) {
							real_itoxy(i, x, y);
							real2data(x, y);

									if (bx == x&&by == y)continue;
									if (m_manager->move_step(x, y)) {
								
										ret = alpha_beta(depth - 1, alpha, beta, m_node_num, !color, data_xytoi(x, y), data_xytoi(bx, by));
										m_manager->move_retract_new();
		
										if (depth == m_level) {
											if (m_max < ret)m_possible.clear();
											m_possible.push_back(point(data_xytoi(x, y), data_xytoi(bx, by), ret));

										}
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
					}
	


					m_manager->move_cancel();
					
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
			
						ret = alpha_beta(depth - 1, alpha, beta, m_node_num, !color, data_xytoi(x, y), 0);
						m_manager->back(temp1, temp2);
					

						if (depth == m_level) {
							if (m_max < ret)m_possible.clear();

							m_possible.push_back(point(data_xytoi(x,y),0,ret));

						}
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


	return ans;
}
