#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "../game/manager.h"

enum MyView { MainView, TopView, DownView, TreeView, BoardView };

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
		void drawTree();
		void drawBoard();
private:
		float xOffset;
		float yOffset;
		vector<ofRectangle> m_view;
		vector<ofEasyCam> m_cam;
		ofMesh treeMesh;
		ofLight m_light;
		unique_ptr<ofxUISuperCanvas> m_UIMain, m_UISetting, m_UITree;
		ofTrueTypeFont m_title, m_normal;
		bool m_isTreeView = false;

		//test
		ofImage m_face_top, m_face_down;
		bool m_isTiming = true;
		
};
