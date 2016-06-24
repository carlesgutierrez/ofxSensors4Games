#pragma once

#include "ofMain.h"
#include "SensorManager.h"
#include "ControllerReconition.h"
#include "ofxImGui.h"


class ofxSensors4Games {
public:
	
	void setup(sensorType myType, RecognitionMethod _myComputeBlobType);
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

	//Type of Sensor used in the ComputerVision input
	SensorManager mySensor;
	
	//Computer Visition
	ControllerReconition myControllerRecognition;
	
	//Gui interaction
	ofxImGui gui;
};
