//
//  ofxSensors4Games
//
//	ofxSensors4Games is a tool that help to use some sensors and use detected action into a pad Controller or others
//  This addon use a SensorManager for sensor uses
//	and ControllerRecognition to read sensor data and implement some other high level operation over this data
//  For internal comunication between this 2 classes, there is a third class. WIP (sensorblobs)
//    - vector<gameblobs>
//		sensorBlobs
//			- For cameras ( normal or kinect ): Opencv Blob.
//			- For tracking Cameras : ( Kyle ofxCv blobs)
//			- For LaserSensor: LaserBlob
//
//
//  Created by carles on 17/06/16.
//
//

#pragma once


#include "ofMain.h"


//#define USE_SENSOR_KINECT // comment this to avoid Kinect issues Lib and not use it

#include "SensorManager.h"
#include "ControllerReconition.h"
#include "ofxImGui.h"
#include "ofxJSON.h"


class ofxSensors4Games {
public:
	
	void setup(sensorType myType);
	void controllersRecognitionSetup();
	void update();
	void draw();
	void exit();

	bool loadAllParamters();
	ofxJSONElement loadJSON(string filePathName);
	bool saveAllParams();
	//void keyPressed(int key);
	//void mouseReleased (int x, int y, int button);

	//Type of Sensor used in the ComputerVision input
	//SensorManager mySensor;
	
	//Computer Visition
	//TODO MEMO and LOAD
	//CV mode contourFinderTrack or Regular
	//Area Sizes!?
	int numPlayersAreasLoaded = 2; //Hack

	ControllerReconition myControllerRecognition1;
	ControllerReconition myControllerRecognition2;
	
	//Gui interaction
	ofxImGui::Gui gui;

	//JSon Save Load all params in one
	ofxJSONElement allParametersGui;
};
