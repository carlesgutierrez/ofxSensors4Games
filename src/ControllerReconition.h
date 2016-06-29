//
//  ControllerReconition.h
//  a Class that use already recognized data sensor and implement over some high level user recognition to be used in an App
//  
//
//  Created by carles on 17/06/16.
//
//
#pragma once

#include "ofMain.h"
//#include "ofxCvBlob.h"
#include "ofxImGui.h"
#include "ofxOsc.h"
#include "statsRecorder.h"
#include "SensorManager.h"

//OSC DATA
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//Status Regonition Controller Type
enum RecognitionMethod { MaxMinsAllBlob, GigestBlob, RadarBlob, LaserBlob }; //TODO Add all this methods
enum ActionRecognitionType { GoUpAction, GoDownAction }; //Up and Down from median height blob player.

class ControllerReconition {
public:
	
	void setup(int w, int h, RecognitionMethod _myComputeBlobType);
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void drawGuiControllerOptions(bool* opened);
	void calcMainBlobLocation();
	void sendOSCBlobData();
	
	//////////////////////////////
	//TODO make a class of this? //Depends if laserBlob may be a different blobType then separate by types
	//Main Controller vars
	int sensorWidth, sensorHeight;
	//std::vector<ofPolyline>  myUpdatedBlobs;
	int numBlobsDetected = -1;
	
	int xPosBlob = -1;
	int yPosBlob = -1;
	int wBlob = -1;
	int hBlob = -1;
	////////////////////////////////

	//TODO other movements detection
	//bool bMoveLeft = false;
	//bool bMoveRight
	
	//Some simple blob Min Max Points detection
	//Post Tracking detection things
	ofVec2f		xMin,xMax,yMin,yMax;
	float xDiff;
	float yDiff;
	void calculateMaxMin();
	
	//Advanced Filtered Blob Data
	//median stat value
	RecognitionMethod myComputeBlobType;
	void updateRecognitionSystem();
	void udpateRecognitionBlobAction();
	
	//int numFramesStates = 100; //TODO Addd to gui
	int numAverageFrammes;
	statsRecorder medianBlobHeightValue;
	float medianHeightBlob;
	
	float fUpActionBlob = 0;
	float fDownActionBlob = 0;
	
	float fUpActionBlob_OSC = 0;
	float fDownActionBlob_OSC = 0;
	
	//OSC filterd data
	ofxOscSender sender;
	float xPosBlobFloatOsc;
	float yPosBlobFloatOsc;
	
	//OSC CONFIG
	int PORT = 12345;
	string HOST = "127.0.0.1";
	
	
};