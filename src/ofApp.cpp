#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	srand((unsigned)time(NULL));
	ofDisableArbTex();
	ofSetVerticalSync(true);
	ofEnableDepthTest(); //Enable z-buffering
	ofEnableSmoothing();
	ofSetBackgroundAuto(true);
	m_view.resize(6);
	for (unsigned i(0); i < 5; ++i) {
		m_cam.push_back(move(ofEasyCam()));
	}
	setupViewport();
	setupCam();

	m_cam[MainView].begin(m_view[MainView]);
	up_light.setPosition(1000, 1000, 2000);
	up_light.lookAt(ofVec3f(0, 0, 0));
	down_light.setPosition(-1000, -1000, -2000);
	down_light.lookAt(ofVec3f(0, 0, 0));
	on_light.setPosition(0, 0, 1000);
	on_light.lookAt(ofVec3f(0, 0, 0));
	other_light.setPosition(750, 750, 1000);
	other_light.lookAt(ofVec3f(400, 400, 0));
	m_cam[MainView].end();
	black_chess.loadImage("black.jpg");
	white_chess.loadImage("white.jpg");
	m_chessboard_jpg.load("chessboard.jpg");
	wood.loadImage("wood.jpg");
	wood_bowl.loadImage("wood2.jpg");
	skin.loadImage("skin.jpg");
	ofSpherePrimitive c;
	c.setRadius(35);
	c.setScale(1, 1, 0.3);
	c.setResolution(30);
	c.rotate(180, 1, 0, 0);
	for (int y(0); y < 8; ++y) {
		for (int x(0); x < 8; ++x) {
			c.setPosition(-m_chessboard_jpg.getWidth() / 2 + m_chessboard_jpg.getWidth() / 16 + x * m_chessboard_jpg.getWidth() / 8, m_chessboard_jpg.getHeight() / 2 - m_chessboard_jpg.getHeight() / 16 - y * m_chessboard_jpg.getHeight() / 8, 12);
			chess.push_back(c);
		}
	}

	m_game.resetGame(HvsC, m_topFirst);

	m_text.loadFont("sans-serif", 21, true, true, true);
	m_title.loadFont("/ARDESTINE.ttf", 80, true, true, true);
	m_digtial.loadFont("/Digital Dismay.otf", 80, true, true, true);
	int index1 = (rand() % 61) + 1;
	int index2 = (rand() % 61) + 1;
	m_back_top.loadImage("/back/back (" + ofToString(index1) + ").jpg");
	m_back_down.loadImage("/back/back (" + ofToString(index2) + ").jpg");
	if (m_back_top.getWidth() > m_back_top.getHeight())m_back_top.rotate90(0);
	if (m_back_down.getWidth() > m_back_down.getHeight())m_back_down.rotate90(0);
	m_back_top.resize(m_back_top.getWidth() / m_back_top.getHeight()*yOffset*1.2, yOffset*1.2);
	m_back_down.resize(m_back_down.getWidth() / m_back_down.getHeight()*yOffset*1.2, yOffset*1.2);
	mode_back.loadImage("mode_back.png");
	mode_back.resize(m_view[ModeView].width, m_view[ModeView].height);
	m_game.getPlayer(TOP)->face().resize(xOffset / 2, xOffset * 7 / 12);
	m_game.getPlayer(DOWN)->face().resize(xOffset / 2, xOffset * 7 / 12);
	ofSetColor(255);
	up_light.enable();
	down_light.enable();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (m_game.getPlayerMode() == HvsC) {
		int p1(0), p2(0),place(0);
		ofResetElapsedTimeCounter();
		if (m_istopturn && m_game.getGameMode() == STEP && m_timeForComputer) {
			if (!m_game.getPlayer(TOP)->step(p1, p2)) {
				m_game.getGameMode() = FREE;
				m_istopturn = false;
			}
			cout << "Computer takes " << ofGetElapsedTimef() << " s" << endl;
			m_istopturn = false;
			m_timeForComputer = false;
			ofResetElapsedTimeCounter();
		}
		else if (m_istopturn && m_game.getGameMode() == MOVE && m_timeForComputer) {
			ofResetElapsedTimeCounter();
			if (!m_game.getPlayer(TOP)->step(p1, p2)) {
				m_game.getGameMode() = FREE;
				m_istopturn = false;
			}
			cout << "Computer takes " << ofGetElapsedTimef() << " s" << endl;
			m_istopturn = false;
			m_timeForComputer = false;
			ofResetElapsedTimeCounter();
		}
	}
	if (m_game.getchessnum() >= 11 && m_isFin == false) {
		if (m_game.judge_success(BLACK) || m_game.judge_success(WHITE)) {
			m_isFin = true;
			stack<int> success_point(m_game.getsuccess());
			while (!success_point.empty()) {
				success_path.addVertex(chess[data2real(success_point.top())].getPosition());
				success_point.pop();
			}
			m_game.getGameMode() = FREE;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (!m_isTreeView) {
		drawMain();
		ofEnableSeparateSpecularLight();
		drawTop();
		drawDown();
		drawMode();
		if (m_isFin) {
			ofSetLineWidth(20);
			m_cam[MainView].begin(m_view[MainView]);
			ofSetColor(255, 0, 0);
			success_path.draw();
			ofSetColor(255);
			m_cam[MainView].end();
			ofSetLineWidth(1);
		}
	}
	else {
		drawTree();
		drawBoard();
	}
	drawViewportOutline();
	if(m_istopturn && m_game.getPlayerMode()==HvsC)m_timeForComputer = true;
}

void ofApp::setUIs() {
	m_UITree.reset(&(ofxUISuperCanvas("TreeView", 2)));
	m_UITree->toggleVisible();
	m_UISetting.reset(&(ofxUICanvas()));
	m_UIStart.reset(&(ofxUICanvas()));
	m_UIMain.reset(&(ofxUITabBar()));

	m_UIMain->addLabel("Network Chess", OFX_UI_FONT_LARGE);
	m_UIMain->addFPS(OFX_UI_FONT_SMALL);
	m_UIMain->addLabel("Press 'h'-> visiable", OFX_UI_FONT_SMALL);
	m_UIMain->addSpacer();
	m_UIMain->setColorBack(10);
	
	m_UISetting->setColorBack(10);
	m_UISetting->setName("Settings");
	m_UISetting->addLabel("Settings", OFX_UI_FONT_MEDIUM);
	m_UISetting->addSpacer();
	vector<string> names = { "player vs player","player vs computer","computer vs computer" };
	m_UISetting->addRadio("player_type", names);
	m_UISetting->addSpacer();
	m_UISetting->addTextInput("player_top", player_top);
	m_UISetting->addTextInput("player_down", player_down);
	m_UISetting->addSpacer();
	m_UISetting->addLabelToggle("RESET", m_isReset);
	m_UISetting->autoSizeToFitWidgets();
	//ofAddListener(m_UISetting.get()->newGUIEvent, this, &ofApp::guiEvent);
	m_UIMain->addCanvas(m_UISetting.get());

}

void ofApp::guiEvent() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 't':
		m_isTreeView = !m_isTreeView;
		setupViewport();
		setupCam();
		break;

	case OF_KEY_RETURN:
		m_game.getGameMode() = STEP;
		m_isFin = false;
		success_path.clear();
		ofResetElapsedTimeCounter();
		break;

	case 'b':
		int p1, p2;
		m_game.back(p1, p2);
		break;

	case 'f':
		m_game.getGameMode() = FREE;
		break;

	case OF_KEY_F1:
		if (!m_game.getPlayer(TOP)->ishuman())m_game.getPlayer(TOP)->m_level = 4;
		break;

	case OF_KEY_F2:
		if (!m_game.getPlayer(TOP)->ishuman())m_game.getPlayer(TOP)->m_level = 6;
		break;

	case OF_KEY_F3:
		if (!m_game.getPlayer(TOP)->ishuman())m_game.getPlayer(TOP)->m_level = 8;
		break;

	case '1':
		m_game.resetGame(HvsH, m_topFirst);
		m_isFin = false;
		success_path.clear();
		m_istopturn = m_topFirst;
		if (m_back_top.getWidth() > m_back_top.getHeight())m_back_top.rotate90(0);
		if (m_back_down.getWidth() > m_back_down.getHeight())m_back_down.rotate90(0);
		m_game.getPlayer(TOP)->face().resize(xOffset / 2, xOffset * 7 / 12);
		m_game.getPlayer(DOWN)->face().resize(xOffset / 2, xOffset * 7 / 12);
		break;

	case '2':
		m_game.resetGame(HvsC, m_topFirst);
		m_isFin = false;
		success_path.clear();
		m_istopturn = m_topFirst;
		if (m_back_top.getWidth() > m_back_top.getHeight())m_back_top.rotate90(0);
		if (m_back_down.getWidth() > m_back_down.getHeight())m_back_down.rotate90(0);
		m_game.getPlayer(TOP)->face().resize(xOffset / 2, xOffset * 7 / 12);
		m_game.getPlayer(DOWN)->face().resize(xOffset / 2, xOffset * 7 / 12);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

int ofApp::xytoi(int x, int y) {
	ofVec3f zero(m_cam[MainView].worldToScreen(ofVec3f(-m_chessboard_jpg.getWidth() / 2, m_chessboard_jpg.getHeight() / 2), m_view[MainView])),
		max(m_cam[MainView].worldToScreen(ofVec3f(m_chessboard_jpg.getWidth() / 2, -m_chessboard_jpg.getHeight() / 2), m_view[MainView]));
	double x_ruler((max.x - zero.x) / 8), y_ruler((max.y - zero.y) / 8);
	return (floor((x - zero.x) / x_ruler) + 8 * floor((y - zero.y) / y_ruler));
}

//--------------------------------------------------------------
void ofApp::screen2board(int & x, int & y) {
	ofVec3f zero(m_cam[MainView].worldToScreen(ofVec3f(-m_chessboard_jpg.getWidth() / 2, m_chessboard_jpg.getHeight() / 2), m_view[MainView])),
		max(m_cam[MainView].worldToScreen(ofVec3f(m_chessboard_jpg.getWidth() / 2, -m_chessboard_jpg.getHeight() / 2), m_view[MainView]));
	double x_ruler((max.x - zero.x) / 8), y_ruler((max.y - zero.y) / 8);
	x = floor((x - zero.x) / x_ruler);
	y = floor((y - zero.y) / y_ruler);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (m_topFirst && m_game.getGameMode() <= 2) {
		if (m_game.who_step()) {
			int index(xytoi(x, y));
			if (index >= 0 && index<64)
			m_game.getPlayer(DOWN)->hand().setPosition(chess[index].getPosition().x, chess[index].getPosition().y, chess[index].getPosition().z);
		}
		else {
			int index(xytoi(x, y));
			if (index >= 0 && index<64)
			m_game.getPlayer(TOP)->hand().setPosition(chess[index].getPosition().x, chess[index].getPosition().y, chess[index].getPosition().z);
		}
	}
	else if (!m_topFirst && m_game.getGameMode() <= 2) {
		if (m_game.who_step()) {
			int index(xytoi(x, y));
			if (index >= 0 && index<64)
			m_game.getPlayer(TOP)->hand().setPosition(chess[index].getPosition().x, chess[index].getPosition().y, chess[index].getPosition().z);
		}
		else {
			int index(xytoi(x, y));
			if (index >= 0 && index<64)
			m_game.getPlayer(DOWN)->hand().setPosition(chess[index].getPosition().x, chess[index].getPosition().y, chess[index].getPosition().z);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (m_game.getGameMode() == STEP) {
		if (m_istopturn && m_game.getPlayer(TOP)->ishuman()) {
			screen2board(x, y);
			real2data(x, y);
			if (m_game.step(x, y)) {
				m_istopturn = false;
				ofResetElapsedTimeCounter();
			}
		}
		else if (!m_istopturn && m_game.getPlayer(DOWN)->ishuman()) {
			screen2board(x, y);
			real2data(x, y);
			if (m_game.step(x, y)) {
				m_istopturn = true;
				ofResetElapsedTimeCounter();
			}
		}
		if (m_game.getmode()) {
			m_game.getGameMode() = MOVE;
		}
	}
	else if (m_game.getGameMode() == MOVE) {
		if (m_istopturn && m_game.getPlayer(TOP)->ishuman()) {
			if (m_game.isselect()) {
				screen2board(x, y);
				real2data(x, y);
				if (m_game.move_step(x, y)) {
					m_istopturn = false;
					ofResetElapsedTimeCounter();
				}
			}
			else {
				screen2board(x, y);
				real2data(x, y);
				m_game.select(x, y);
			}
		}
		else if (!m_istopturn && m_game.getPlayer(DOWN)->ishuman()) {
			if (m_game.isselect()) {
				screen2board(x, y);
				real2data(x, y);
				if (m_game.move_step(x, y)) {
					m_istopturn = true;
					ofResetElapsedTimeCounter();
				}
			}
			else {
				screen2board(x, y);
				real2data(x, y);
				m_game.select(x, y);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	setupViewport();
	//if (m_back_top.getWidth() > m_back_top.getHeight())m_back_top.rotate90(0);
	m_back_top.resize(m_back_top.getWidth() / m_back_top.getHeight()*yOffset*1.2, yOffset*1.2);
	//if (m_back_down.getWidth() > m_back_down.getHeight())m_back_down.rotate90(0);
	m_back_down.resize(m_back_down.getWidth() / m_back_down.getHeight()*yOffset*1.2, yOffset*1.2);
	mode_back.resize(m_view[ModeView].width, m_view[ModeView].height);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::setupViewport() {
	xOffset = ofGetWidth() / 4;
	if (!m_isTreeView) {
		yOffset = ofGetHeight() / 2 - 50;

		m_view[MainView].x = 0;
		m_view[MainView].y = 0;
		m_view[MainView].width = xOffset * 3;
		m_view[MainView].height = ofGetHeight();

		m_view[TopView].x = xOffset * 3;
		m_view[TopView].y = 0;
		m_view[TopView].width = xOffset;
		m_view[TopView].height = yOffset;

		m_view[ModeView].x = xOffset * 3;
		m_view[ModeView].y = yOffset;
		m_view[ModeView].width = xOffset;
		m_view[ModeView].height = 100;

		m_view[DownView].x = xOffset * 3;
		m_view[DownView].y = yOffset+100;
		m_view[DownView].width = xOffset;
		m_view[DownView].height = yOffset;
	}
	else {
		yOffset = ofGetHeight() / 4;

		m_view[TreeView].x = 0;
		m_view[TreeView].y = 0;
		m_view[TreeView].width = ofGetWidth();
		m_view[TreeView].height = ofGetHeight();

		m_view[BoardView].x = xOffset * 3;
		m_view[BoardView].y = 0;
		m_view[BoardView].width = xOffset;
		m_view[BoardView].height = yOffset;
	}
}

void ofApp::setupCam() {
	if (!m_isTreeView) {
		m_cam[MainView].setAutoDistance(true);
		m_cam[TopView].setDistance(100);
		m_cam[DownView].setDistance(100);

		m_cam[TopView].disableMouseInput();
		m_cam[DownView].disableMouseInput();
	}
	else {
		m_cam[TreeView].setAutoDistance(true);
		m_cam[BoardView].setDistance(100);

		m_cam[BoardView].disableMouseInput();
	}
}

void ofApp::setupUI() {

}

void ofApp::drawViewportOutline() {
	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything
							// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(0, 0, 0);

	if (!m_isTreeView) {
		ofRect(m_view[MainView]);
		ofRect(m_view[TopView]);
		ofRect(m_view[DownView]);
		ofRect(m_view[ModeView]);
	}
	else {
		ofRect(m_view[TreeView]);
		ofRect(m_view[BoardView]);
	}

	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();
}

void ofApp::drawMain() {
	m_cam[MainView].begin(m_view[MainView]);
	
	wood.bind();
	ofDrawBox(0, 0, -15, m_chessboard_jpg.getWidth(), m_chessboard_jpg.getHeight(), -20);
	wood.unbind();
	m_chessboard_jpg.draw(-m_chessboard_jpg.getWidth() / 2, -m_chessboard_jpg.getHeight() / 2);
	int pre((m_game.getprechess()));
	if (pre >= 0 && pre < 64) {
		ofSetColor(0, 255, 45);
		ofPoint pos(chess[pre].getPosition());
		pos.z += 5;
		ofDrawSphere(pos, 10);
	}
	ofSetColor(255);
	for (unsigned i(1); i <= m_game.getchessnum(); ++i) {
		if (i % 2 == WHITE && m_game.on_board()[i] != 0) {
			white_chess.bind();
			chess[data2real(m_game.on_board()[i])].draw();
			white_chess.unbind();

			

		}
		else if(m_game.on_board()[i] != 0) {
			black_chess.bind();
			chess[data2real(m_game.on_board()[i])].draw();
			black_chess.unbind();
		}
	}

	if (m_istopturn)m_game.getPlayer(TOP)->hand().drawFaces();
	else m_game.getPlayer(DOWN)->hand().drawFaces();

	int select(data2real(m_game.getselect()));
	ofSetColor(255, 0, 0);
	if (select >= 0 && select < 64 && m_game.isselect())ofDrawCircle(chess[select].getPosition(), 50);
	ofSetColor(255, 255, 255);

	m_cam[MainView].end();

	ofPushView();
	ofViewport(m_view[MainView]);
	ofSetupScreen();
	ofSetColor(255, 255, 255);
	ofPopView();
}

void ofApp::drawTop() {
	ofPushView();
	ofViewport(m_view[TopView]);
	ofSetupScreen();
	stringstream ss,se;
	ss << m_game.getPlayer(TOP)->name();
	ofSetColor(0, 0, 0);
	m_text.drawString(ss.str(), 10, 25);
	if (m_istopturn && m_game.getGameMode()<2) {
		int sec(60 - (static_cast<int>(ofGetElapsedTimef())));
		if (sec >= 0) {
			se << setw(2) << setfill('0') << setiosflags(ios::right) << sec;
			m_digtial.drawString(se.str(), xOffset / 2 - 40, yOffset - 40);
		}
		else m_digtial.drawString("lose", xOffset / 2 - 80, yOffset - 40);
	}
	else m_digtial.drawString(ofToString(60), xOffset / 2 - 40, yOffset - 40);
	ofSetColor(255, 255, 255);
	m_game.getPlayer(TOP)->face().draw(xOffset - m_game.getPlayer(TOP)->face().getWidth() - 20, 20, 0);
	m_back_top.draw(0, 0, -1);
	ofPopView();
}

void ofApp::drawDown() {
	ofPushView();
	ofViewport(m_view[DownView]);
	ofSetupScreen();
	stringstream ss, se;
	ss << m_game.getPlayer(DOWN)->name();
	ofSetColor(0, 0, 0);
	m_text.drawString(ss.str(), 10, yOffset-10);
	if (!m_istopturn && m_game.getGameMode()<2) {
		int sec(60 - (static_cast<int>(ofGetElapsedTimef())));
		if (sec >= 0) {
			se << setw(2) << setfill('0') << setiosflags(ios::right) << sec;
			m_digtial.drawString(se.str(), xOffset / 2 - 40, 100);
		}
		else m_digtial.drawString("lose", xOffset / 2 - 80, 100);
	}
	else m_digtial.drawString(ofToString(60), xOffset / 2 - 40, 100);
	ofSetColor(255, 255, 255);
	m_game.getPlayer(DOWN)->face().draw(xOffset- m_game.getPlayer(DOWN)->face().getWidth() -20, yOffset- m_game.getPlayer(DOWN)->face().getHeight() -20, 0);
	m_back_down.draw(0, 0, -1);
	ofPopView();
}

void ofApp::drawMode() {
	ofPushView();
	ofViewport(m_view[ModeView]);
	ofSetupScreen();
	mode_back.draw(0, 0, -1);
	string ss;
	ofSetColor(205, 10, 100);
	switch (m_game.getGameMode()) {
	case STEP:
		ss = "STEP";
		break;
	case MOVE:
		ss = "MOVE";
		break;
	case FREE:
		ss = "FREE";
		break;
	case SHOW:
		ss = "SHOW";
		break;
	}
	m_title.drawString(ss, 2, 90);
	ofSetColor(255, 255, 255);
	ofPopView();
}

void ofApp::drawTree() {
	m_cam[TreeView].begin(m_view[TreeView]);
	ofEnableSeparateSpecularLight();
	ofDrawPlane(500, 500);

	m_cam[TreeView].end();

	ofPushView();
	ofViewport(m_view[TreeView]);
	ofSetupScreen();
	stringstream ss;
	ss << "Tree Viewport";
	ofSetColor(0, 0, 0);
	m_text.drawString(ss.str(), 5, 25);
	ofPopView();
}

void ofApp::drawBoard() {
	m_cam[BoardView].begin(m_view[BoardView]);
	ofEnableSeparateSpecularLight();
	ofSetColor(255, 255, 255);
	m_chessboard_jpg.draw(-m_chessboard_jpg.getWidth() / 2, -m_chessboard_jpg.getHeight() / 2);

	m_cam[BoardView].end();
	
	ofPushView();
	ofViewport(m_view[BoardView]);
	ofSetupScreen();
	ofPopView();
}

void ofApp::drawStage() {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
