#include "ofxSensors4Games.h"

/*
	If you are struggling to get the device to connect ( especially Windows Users )
	please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofxSensors4Games::setup(sensorType _myType) {
	ofSetLogLevel(OF_LOG_ERROR);//OF_LOG_VERBOSE

	cout << "Before setup we called loadAllParamters() " << endl;

	//required calls
	SensorManager::getInstance()->setSensorType(_myType);

	//SensorManager::getInstance()->typeSensor = _myType;
	//SensorManager::getInstance()->modeSensor = _modeSensor;

	gui.setup();

	//setups managers with selected SensorType

	SensorManager::getInstance()->setup(_myType);

	controllersRecognitionSetup();

	if (SensorManager::getInstance()->numPlayersAreas > 2) {
		cout << "Error:: There are not enought controllers classes" << endl;
	}



}

//--------------------------------------
void ofxSensors4Games::controllersRecognitionSetup() {

	//if (SensorManager::getInstance()->numPlayersAreas > 0) {
		myControllerRecognition1.setup(
			SensorManager::getInstance()->getWidth(),
			SensorManager::getInstance()->getHeight(),
			SensorManager::getInstance()->computerVisionSensor1.contourFinder,
			1
		);
	//}

	//if (SensorManager::getInstance()->numPlayersAreas > 1) {
		myControllerRecognition2.setup(
			SensorManager::getInstance()->getWidth(),
			SensorManager::getInstance()->getHeight(),
			SensorManager::getInstance()->computerVisionSensor2.contourFinder,
			2
		);
	//}
}

//--------------------------------------------------------------
void ofxSensors4Games::update() {

	SensorManager::getInstance()->update();

	for (int i = 0; i < SensorManager::getInstance()->playerAreas.size(); i++) {
		ofRectangle auxArea = SensorManager::getInstance()->playerAreas[i].rectArea;
		if (SensorManager::getInstance()->playerAreas[i].bAreaActive) {
			if (i == 0)myControllerRecognition1.update(auxArea);
			else if (i == 1)myControllerRecognition2.update(auxArea);
		}
	}
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

	if(ImGui::Button("Save Params")){
		saveAllParams();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reload Params")) {
		loadAllParamters();
	}
	
	//TODO
	//Set here Sensor Selections
	//Set Here a button to Save all params in XML or Json
	//Activate and Address OSC data
	//Check here values to send and change tags if its necesary
	
	//General Draws with possibles Guis windows
	SensorManager::getInstance()->draw();
	
	for (int i = 0; i < SensorManager::getInstance()->playerAreas.size(); i++) {
		if (i == 0 && SensorManager::getInstance()->playerAreas[i].bAreaActive)myControllerRecognition1.draw();
		if (i == 1 && SensorManager::getInstance()->playerAreas[i].bAreaActive)myControllerRecognition2.draw();
	}
	
	//required to call this at end
	gui.end();
	
    
}


//--------------------------------------------------------------
void ofxSensors4Games::exit() {
	myControllerRecognition1.exit();
	myControllerRecognition2.exit();
	SensorManager::getInstance()->exit();
}

//-------------------------------------------------------------
bool ofxSensors4Games::loadAllParamters() {

	string filePath = "allMyParams.json";
	ofxJSONElement jPreset = loadJSON(filePath);

	//read here sensormode Todo


	cout << "jPreset" << jPreset << endl;
	if (!jPreset[0]["numPlayersAreas"].isNull()) {
		cout << "jPreset[numPlayersAreas] = " << jPreset[0]["numPlayersAreas"] << endl;
		SensorManager::getInstance()->numPlayersAreas = ofToInt(jPreset[0]["numPlayersAreas"].asString());
	}
	else SensorManager::getInstance()->numPlayersAreas = 1;
	
	if (SensorManager::getInstance()->numPlayersAreas > 0) {
		myControllerRecognition1.setParams(jPreset, 1);
	}
	
	if (SensorManager::getInstance()->numPlayersAreas > 1) {
		myControllerRecognition2.setParams(jPreset, 2); 
	}
	
	if (SensorManager::getInstance()->numPlayersAreas > 2) {
		cout << "Error:: TODO more controllers classes" << endl;
	}

	SensorManager::getInstance()->setParams(jPreset);

	return true;
}


//-------------------------------------------------------------
ofxJSONElement ofxSensors4Games::loadJSON(string filePathName)
{
	ofxJSONElement jElement;
	bool parse = false;
	ofLogVerbose() << "Trying to load config at: " + filePathName;
	parse = jElement.open(filePathName);

	ofLogVerbose() << "ConfigLoaded";
	return jElement;
}

//------------------------------------------------------------
bool ofxSensors4Games::saveAllParams() {

	allParametersGui.clear();

	ofxJSONElement paramsRecognition1 = myControllerRecognition1.getParams();
	ofxJSONElement paramsRecognition2 = myControllerRecognition2.getParams();
	ofxJSONElement paramsSensor = SensorManager::getInstance()->getParams();

	ofxJSONElement localJsonParams;
	localJsonParams["numPlayersAreas"] = ofToString(SensorManager::getInstance()->numPlayersAreas, 0); // this wil lbe saved at 0 pos in the JSON... TODO  How do this better.
	allParametersGui.append(localJsonParams);

	allParametersGui.append(paramsRecognition1);
	allParametersGui.append(paramsRecognition2);
	allParametersGui.append(paramsSensor);

	allParametersGui.save("allMyParams.json", true);

	return true;
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
