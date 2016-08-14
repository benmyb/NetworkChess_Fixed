#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofEventUtils.h"
#include "../game/game.h"

enum MyView { MainView, TopView, DownView, TreeView, BoardView, ModeView };

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void setUIs();
		void guiEvent();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void setupViewport();
		void setupCam();
		void setupUI();
		void drawViewportOutline();
		void drawMain();
		void drawTop();
		void drawDown();
		void drawMode();
		void drawTree();
		void drawBoard();
		void drawStage();
private:
		float xOffset;
		float yOffset;
		vector<ofRectangle> m_view;
		vector<ofEasyCam> m_cam;
		ofMesh treeMesh;
		ofLight m_light;
		unique_ptr<ofxUISuperCanvas> m_UITree;
		unique_ptr<ofxUICanvas> m_UISetting, m_UIStart;
		unique_ptr<ofxUITabBar> m_UIMain;
		ofTrueTypeFont m_title, m_text, m_digtial;
		ofImage mode_back;
		bool m_isTreeView = false;
		bool m_isReset = false;

		ofImage m_chessboard_jpg;
		ofImage wood;
		ofImage wood_bowl;
		ofImage skin;
		ofImage white_chess;
		ofImage black_chess;
		ofLight up_light;
		ofLight down_light;
		ofLight on_light;
		ofLight other_light;
		ofPolyline success_path;
		vector<ofSpherePrimitive> chess;
		game m_game;
		string player_top, player_down;

		//test
		ofImage m_back_top, m_back_down;
		bool m_isTiming = true;
		
};
