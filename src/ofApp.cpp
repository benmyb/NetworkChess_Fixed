#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	srand((unsigned)time(NULL));
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
	m_text.loadFont("sans-serif", 21, true, true);
	m_title.loadFont("AR DESTINE", 80, true, true);
	m_digtial.loadFont("Digital Dismay", 80, true, true);
	int index1((rand() % 37) + 1), index2((rand() % 37) + 1);
	m_face_top.loadImage("/faces/human/" + ofToString(index1) + ".jpg");
	m_face_down.loadImage("/faces/human/" + ofToString(index2) + ".jpg");
	mode_back.loadImage("mode_back.png");
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
	
	ofDrawPlane(500, 500);
	
	m_cam[MainView].end();

	ofPushView();
	ofViewport(m_view[MainView]);
	ofSetupScreen();
	stringstream ss;
	ss << "Main viewport";
	ofSetColor(0, 0, 0);
	m_title.drawString(ss.str(), 5, 105);
	ofSetColor(255, 255, 255);
	ofPopView();
}

void ofApp::drawTop() {
	ofPushView();
	ofViewport(m_view[TopView]);
	ofSetupScreen();
	stringstream ss,se;
	ss << "Ben";
	ofSetColor(0, 0, 0);
	m_text.drawString(ss.str(), 10, 25);
	se <<setw(2) << setfill('0') << setiosflags(ios::right)<< 60 - (static_cast<int>(ofGetElapsedTimef()));
	m_digtial.drawString(se.str(), xOffset / 2 - 40, yOffset - 40);
	ofSetColor(255, 255, 255);
	m_face_top.draw(xOffset - m_face_top.getWidth() - 20, 20, 0);
	ofPopView();
}

void ofApp::drawDown() {
	ofPushView();
	ofViewport(m_view[DownView]);
	ofSetupScreen();
	stringstream ss, se;
	ss << "Daisy";
	ofSetColor(0, 0, 0);
	m_text.drawString(ss.str(), 10, yOffset-10);
	se << setw(2) << setfill('0') << setiosflags(ios::right) << 60 - (static_cast<int>(ofGetElapsedTimef()));
	m_digtial.drawString(se.str(), xOffset / 2 - 40, 100);
	ofSetColor(255, 255, 255);
	m_face_down.draw(xOffset- m_face_down.getWidth() -20, yOffset- m_face_down.getHeight() -20, 0);
	ofPopView();
}

void ofApp::drawMode() {
	ofPushView();
	ofViewport(m_view[ModeView]);
	ofSetupScreen();
	mode_back.draw(0, 0, -1);
	string ss;
	ofSetColor(205, 10, 100);
	ss = "STEP";
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
