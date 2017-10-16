#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	mySensorController.setup(cameraSensor, realTimeMode);
	//cameraSensor , kinectSensor// realTimeMode, simulationMode
	
	//by default load All Saved Parameters
	mySensorController.loadAllParamters();
}

//--------------------------------------------------------------
void ofApp::update(){
	mySensorController.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	mySensorController.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'S') {
		mySensorController.saveAllParams();
		cout << "Save all Params here" << endl;
	}
	if (key == 'L') {
		mySensorController.loadAllParamters();
		cout << "Load all Params here" << endl;
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
