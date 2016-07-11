#include "ofxSensors4Games.h"

/*
    If you are struggling to get the device to connect ( especially Windows Users )
    please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofxSensors4Games::setup(sensorType _myType, sensorMode _modeSensor, RecognitionMethod _myComputeBlobType) {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//required calls
	SensorManager::getInstance()->setSensorType(_myType);
	SensorManager::getInstance()->setSensorMode(_modeSensor);
	
	//SensorManager::getInstance()->typeSensor = _myType;
	//SensorManager::getInstance()->modeSensor = _modeSensor;
	
	gui.setup();
	
	//setups managers with selected SensorType
	
	SensorManager::getInstance()->setup(_myType, _modeSensor);
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


/* Save some data example with Json ( need addon ofxJson)
//---------------------------------------------------------------------
void saveData() {
	
	//A fucntion to save all the array of pressets in a current Json
	ofxJSONElement saverySchedulerJS;

	saverySchedulerJS["Items"]["Description"] = myDynamicSchedulerPresetsVector[i].internalDescription;
	saverySchedulerJS["presetName"] = myDynamicSchedulerPresetsVector[i].typePresetName;
	saverySchedulerJS["Config"]["bSchedulerActive"] = bSchedulerActive;
	
	string name2Save = savefolder + fileName;
	saverySchedulerJS.save(name2Save, true);
}
 */
