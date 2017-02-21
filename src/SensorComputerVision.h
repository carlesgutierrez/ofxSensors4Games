#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxImGui.h"
class SensorComputerVision {

public:

	//FUNCTIONS
	void setup(int _id, int _cameraW, int _cameraH);
	void setTrackingMode(bool _status);
	void udpateBackground();
	void mainComputerVision(ofImage _image2Compute);
	void draw(float _sensorDrawScale, int _marginDraw);
	void drawGui();


	//To identify active CV
	int idSensorCV = -1;

	//VARS
	ofxCv::RunningBackground background;
	float learningTime = 30;
	float thresholdValue = 10;
	bool bresetBackground = true;
	bool bSimpleBackgroundSubstraction = true;
	bool bLearnBackground = false;
	bool bBlackWhiteCv = true;
	bool bInvertContourFinderThreshold = false;
	bool bAutoThreshold = true;
	bool bContourFinderThreshold = false;
	bool bContourFinderColorThreshold = false;
	ofColor colorTargetContourFinder = ofColor(0, 0, 255); //Default Blue Color

	ofImage diffCam;
	ofImage backGroundCam;
	int cameraWidth, cameraHeight;

	//-----------------------
	//Tracking mode
	ofImage computerVisionImage;
	ofxCv::ContourFinder * contourFinder;

private:

	//Tracking mode
	bool bTrackgingActive = false;
	bool showLabels = true;
	int maxPersistenceTracking = 15;
	int maxDistanceTracking = 32;
	//Blobs
	int minSizeBlob = 5;
	int maxSizeBlob = 1000;
	int maxBlobsAccuracyMaxValue = maxSizeBlob;
	int numBlobs = 20;


};