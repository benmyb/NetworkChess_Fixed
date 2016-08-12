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

	m_game.resetGame(HvsC, true);

	m_text.loadFont("sans-serif", 21, true, true, true);
	m_title.loadFont("/ARDESTINE.ttf", 80, true, true, true);
	m_digtial.loadFont("/Digital Dismay.otf", 80, true, true, true);
	int index1((rand() % 37) + 1), index2((rand() % 37) + 1);
	m_face_top.loadImage("/faces/human/" + ofToString(index1) + ".jpg");
	m_face_down.loadImage("/faces/human/" + ofToString(index2) + ".jpg");
	index1 = (rand() % 92) + 1;
	index2 = (rand() % 92) + 1;
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
	m_face_top.resize(xOffset / 2, xOffset * 7 / 12);
	m_face_down.resize(xOffset / 2, xOffset * 7 / 12);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (!m_isTreeView) {
		drawMain();
		drawTop();
		drawDown();
		drawMode();
	}
	else {
		drawTree();
		drawBoard();
	}
	drawViewportOutline();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 't':
		m_isTreeView = !m_isTreeView;
		setupViewport();
		setupCam();
		break;

	case ' ':
		ofResetElapsedTimeCounter();
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
	int pre(m_game.getprechess());
	if (pre >= 0 && pre < 64) {
		ofSetColor(0, 255, 45);
		ofPoint pos(chess[pre].getPosition());
		pos.z += 5;
		ofDrawSphere(pos, 10);
	}
	for (unsigned i(1); i < 2 * chess_num + 1; ++i) {
		if (i % 2 + 1 == WHITE && m_game.on_board()[i] != 0) {
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
	se <<setw(2) << setfill('0') << setiosflags(ios::right)<< 60 - (static_cast<int>(ofGetElapsedTimef()));
	m_digtial.drawString(se.str(), xOffset / 2 - 40, yOffset - 40);
	ofSetColor(255, 255, 255);
	m_game.getPlayer(TOP)->face().draw(xOffset - m_face_top.getWidth() - 20, 20, 0);
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
	se << setw(2) << setfill('0') << setiosflags(ios::right) << 60 - (static_cast<int>(ofGetElapsedTimef()));
	m_digtial.drawString(se.str(), xOffset / 2 - 40, 100);
	ofSetColor(255, 255, 255);
	m_game.getPlayer(DOWN)->face().draw(xOffset- m_face_down.getWidth() -20, yOffset- m_face_down.getHeight() -20, 0);
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
	ofDrawPlane(100, 100);

	m_cam[BoardView].end();
	
	ofPushView();
	ofViewport(m_view[BoardView]);
	ofSetupScreen();
	stringstream ss;
	ss << "Board Viewport";
	ofSetColor(0, 0, 0);
	m_title.drawString(ss.str(), 5, 25);
	ofPopView();
}

void ofApp::drawStage() {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
