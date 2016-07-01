#include "ofxSensors4Games.h"

/*
    If you are struggling to get the device to connect ( especially Windows Users )
    please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofxSensors4Games::setup(sensorType _myType, RecognitionMethod _myComputeBlobType) {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//required calls
	SensorManager::getInstance()->sensorModel = _myType;
	gui.setup();
	
	//setups managers with selected SensorType
	
	SensorManager::getInstance()->setup(SensorManager::getInstance()->sensorModel);
	myControllerRecognition.setup(SensorManager::getInstance()->getWidth(),
								  SensorManager::getInstance()->getHeight(),
								  _myComputeBlobType);
	
	
}

//--------------------------------------------------------------
void ofxSensors4Games::update() {
	
	SensorManager::getInstance()->update();
	myControllerRecognition.update();
}

//--------------------------------------------------------------
void ofxSensors4Games::draw() {
	
	///////////////////////////////////////////////////////////////
	//Gui Draw and controls
	
	//required to call this at beginning
	gui.begin();
	string frameRateAvailable = "Fps: " + ofToString(ofGetFrameRate(), 0);
	ImGui::Text("Sensors For Games!");
	
	ImGui::Text(frameRateAvailable.c_str());
	
	//TODO
	//Set here Sensor Selections
	//Set Here a button to Save all params in XML or Json
	//Activate and Address OSC data
	//Check here values to send and change tags if its necesary
	
	//General Draws with possibles Guis windows
	SensorManager::getInstance()->draw();
	myControllerRecognition.draw();
	
	//required to call this at end
	gui.end();
	
    
}


//--------------------------------------------------------------
void ofxSensors4Games::exit() {
	myControllerRecognition.exit();
	SensorManager::getInstance()->exit();
}

//--------------------------------------------------------------
void ofxSensors4Games::keyPressed (int key) {
	myControllerRecognition.keyPressed(key);
	SensorManager::getInstance()->keyPressed(key);
}
