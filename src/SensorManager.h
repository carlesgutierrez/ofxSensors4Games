//
//  SensorManager.h
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//


#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxImGui.h"

enum sensorType { kinectSensor, cameraSensor };

class SensorManager {
	
public:

	void setup(sensorType _sensorType);
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
	int getWidth();
	int getHeight();
	
	void drawGuiSensorOptions(bool* opened);
		
	//----------------------
	//Kinect
	ofxKinect kinect;
	bool bThreshWithOpenCV;
	
	int nearThreshold;
	int farThreshold;
	int angle;

	//Raw ComputerVision
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	ofxCvContourFinder contourFinder;
	
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