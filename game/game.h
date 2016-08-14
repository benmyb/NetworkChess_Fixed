#pragma once
#include "board.h"
#include "human.h"
#include "computer.h"

enum GAME_MODE { STEP, MOVE, FREE, SHOW };
enum PLAYER_MODE { HvsC, HvsH, CvsC };
enum PLAYER_SIDE { TOP, DOWN };

class game {
private:
	board m_board;
	unique_ptr<player >m_player[2];
	GAME_MODE m_game_mode = FREE;
	PLAYER_MODE m_player_mode = HvsH;
public:
	game() {}

	int* on_board() { return m_board.m_chess; }
	unsigned long  available(CHESS_COLOR color) { return m_board.get_all_feasible(); }
	void resetGame(PLAYER_MODE player,bool TopisFirst) { 
		m_player_mode = player;
		m_board.restart();
		switch (m_player_mode) {
		case HvsC:
			m_player[TOP].reset(new computer(m_board));
			m_player[TOP]->settop(true);
			m_player[TOP]->setwhite(TopisFirst);

			m_player[DOWN].reset(new human());
			m_player[DOWN]->settop(false);
			m_player[DOWN]->setwhite(TopisFirst);
			break;

		case HvsH:
			m_player[TOP].reset(new human());
			m_player[TOP]->settop(false);
			m_player[TOP]->setwhite(TopisFirst);

			m_player[DOWN].reset(new human());
			m_player[DOWN]->settop(false);
			m_player[DOWN]->setwhite(TopisFirst);
			break;

		case CvsC:
			m_player[TOP].reset(new computer(m_board));
			m_player[TOP]->settop(true);
			m_player[TOP]->setwhite(TopisFirst);

			m_player[DOWN].reset(new computer(m_board));
			m_player[DOWN]->settop(true);
			m_player[DOWN]->setwhite(TopisFirst);
			break;
		}
		m_game_mode = FREE;
	}
	unique_ptr<player >& getPlayer(PLAYER_SIDE i) { return m_player[i]; }
	GAME_MODE& getGameMode() { return m_game_mode; }
	PLAYER_MODE& getPlayerMode() { return m_player_mode; }
	int getprechess() { return data2real(m_board.get_prechess()); }

	void step(int x, int y) { m_board.step(x, y); }
	void select(int x, int y) { m_board.move_select(x, y); }
	//移棋到的位置，如果能够移动就返回1
	bool move_step(int x, int y) { m_board.move_step(x, y); }
	void move_cancel() { m_board.move_cancel(); }
	//void back() { if (m_game_mode <= 1)m_board.back(m_game_mode); }

};