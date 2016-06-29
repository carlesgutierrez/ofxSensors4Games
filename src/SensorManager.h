//
//  SensorManager.h
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//


#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxImGui.h"

enum sensorType { kinectSensor, cameraSensor };

class SensorManager {
	
// variables & methods for singleton
private:
	static bool	instanceFlag;
	static SensorManager *single;
public:
	static SensorManager* getInstance();
// end singleton
	
public:

	SensorManager();
	~SensorManager();

	void setup(sensorType _sensorType);
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
	int getWidth();
	int getHeight();
	
	void drawGuiSensorOptions(bool* opened);
	
	bool isNewSensorFrame();
	
	//----------------------
	//Kinect
	ofxKinect kinect;
	//bool bThreshWithOpenCV;
	int nearThreshold;
	int farThreshold;
	int angle;
	
private:
	bool bNewSensorFrame = false;

public:
	//Related to ComputerVision
	
	
	ofImage computerVisionImage;
	ofxCv::ContourFinder contourFinder;
	
	//Blobs
	int minSizeBlob = 10;
	int maxSizeBlob = 640*480*0.5;
	int numBlobs = 20;
	
	
	//-----------------------
	//WebCam or similar video camera
	//ofVideoCamera...


	
	//Control Vars
	int sensorWidth = 640;//TODO check this is always modif
	int sensorHeight = 480;
	sensorType sensorModel;
	


};