#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxImGui.h"

enum sensorType { kinectSensor, cameraSensor, externalSickSensor };
enum sensorMode { realTimeMode, simulationMode };
enum detectionMethod { FindContourns, FindContournsTracking/*, DarkNetTODO*/ }; //TODO Add all this methods


//GUi util Funtion
#define IM_ARRAYSIZE_TEMP1(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class SensorComputerVision {

public:

	//FUNCTIONS
	void setup(int _id, int _cameraW, int _cameraH, sensorType _myTypeSensor);
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

	sensorType mySensorType;
	detectionMethod trackingMode = FindContournsTracking;// FindContourns;

private:

	//Tracking mode
	//bool bTrackgingActive = false;
	bool showLabels = true;
	int maxPersistenceTracking = 15;
	int maxDistanceTracking = 32;
	//Blobs
	int minSizeBlob = 5;
	int maxSizeBlob = 500; // TODO this should get values from Camera resolution. Now 640 x 480
	int minBlobsArea = minSizeBlob;
	int maxBlobsArea = maxSizeBlob;
	int numBlobs = 20;


};