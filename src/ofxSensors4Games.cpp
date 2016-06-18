#include "ofxSensors4Games.h"

/*
    If you are struggling to get the device to connect ( especially Windows Users )
    please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofxSensors4Games::setup(sensorType myType) {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//required calls
	mySensor.sensorModel = myType;
	gui.setup();
	
	//setups managers with selected SensorType
	if(mySensor.sensorModel == kinectSensor){
		mySensor.setup(mySensor.sensorModel);
		myControllerRecognition.setup(mySensor.getWidth(), mySensor.getHeight());
	}
	

}

//--------------------------------------------------------------
void ofxSensors4Games::update() {
	
	myControllerRecognition.update();
	mySensor.update();

}

//--------------------------------------------------------------
void ofxSensors4Games::draw() {
	
	///////////////////////////////////////////////////////////////
	//Gui Draw and controls
	
	//required to call this at beginning
	gui.begin();
	ImGui::Text("Sensors For Games!");
	
	//TODO
	//Set here Sensor Selections
	//Set Here a button to Save all params in XML or Json
	//Activate and Address OSC data
	//Check here values to send and change tags if its necesary
	
	//General Draws with possibles Guis windows
	myControllerRecognition.draw();
	mySensor.draw();
	
	//required to call this at end
	gui.end();
	
    
}


//--------------------------------------------------------------
void ofxSensors4Games::exit() {
	myControllerRecognition.exit();
	mySensor.exit();
}

//--------------------------------------------------------------
void ofxSensors4Games::keyPressed (int key) {
	myControllerRecognition.keyPressed(key);
	mySensor.keyPressed(key);
}
