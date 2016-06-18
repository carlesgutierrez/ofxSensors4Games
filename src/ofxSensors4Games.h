#pragma once

#include "ofMain.h"
#include "ControllerReconition.h"
#include "SensorManager.h"
#include "ofxImGui.h"


class ofxSensors4Games {
public:
	
	void setup(sensorType myType);
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

	//Computer Visition
	ControllerReconition myControllerRecognition;
	
	//Type of Sensor used in the ComputerVision input
	SensorManager mySensor;
	
	//Gui interaction
	ofxImGui gui;
};
