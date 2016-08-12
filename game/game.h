#pragma once

#include "human.h"
#include "computer.h"

enum GAME_MODE { STEP, MOVE, FREE, SHOW };
enum PLAYER_MODE { HvsC, HvsH, CvsC };
enum PLAYER_SIDE { TOP, DOWN };

class game {
private:
	manager m_manager;
	unique_ptr<player >m_player[2];
	GAME_MODE m_game_mode = FREE;
	PLAYER_MODE m_player_mode = HvsH;
public:
	game() {}

	int* chess_on_board() { return m_manager.chess_on_board(); }
	unsigned long  available(CHESS_COLOR color) { return m_manager.available(color); }
	void resetGame(PLAYER_MODE player,bool TopisFirst) { 
		m_player_mode = player;
		m_manager.restart();
		switch (m_player_mode) {
		case HvsC:
			m_player[TOP].reset(new computer());
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
			m_player[TOP].reset(new computer());
			m_player[TOP]->settop(true);
			m_player[TOP]->setwhite(TopisFirst);

			m_player[DOWN].reset(new computer());
			m_player[DOWN]->settop(true);
			m_player[DOWN]->setwhite(TopisFirst);
			break;
		}
		m_game_mode = FREE;
	}
	unique_ptr<player >& getPlayer(PLAYER_SIDE i) { return m_player[i]; }
	GAME_MODE& getGameMode() { return m_game_mode; }
	PLAYER_MODE& getPlayerMode() { return m_player_mode; }
	int getprechess() { return data2real(m_manager.getprechess()); }
	int* on_board() { return m_manager.chess_on_board(); }

	void step(int x, int y) { m_manager.step(x, y); }
	void select(int x, int y) { m_manager.select(x, y); }
	//移棋到的位置，如果能够移动就返回1
	bool move_step(int x, int y) { m_manager.move_step(x, y); }
	void move_back(int x, int y) { m_manager.move_back(x, y); }
	void move_cancel() { m_manager.move_cancel(); }
	void back() { if (m_game_mode <= 1)m_manager.back(m_game_mode); }

};