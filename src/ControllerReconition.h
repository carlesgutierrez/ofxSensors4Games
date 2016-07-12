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
#include "statsRecorder.h"
#include "SensorManager.h"



//Status Regonition Controller Type
enum RecognitionMethod { MaxMinsAllBlob, GigestBlob, RadarBlob, LaserBlob }; //TODO Add all this methods
enum ActionRecognitionType { GoUpAction, GoDownAction }; //Up and Down from median height blob player.

class ControllerReconition {
public:
	
	void setup(int w, int h, RecognitionMethod _myComputeBlobType);
	void update();
	void draw();
	void exit();
	
	void keyPressed(ofKeyEventArgs & args);
	void keyReleased(ofKeyEventArgs & args);
	
	void mouseMoved(ofMouseEventArgs & args){}
	void mouseDragged(ofMouseEventArgs & args){}
	void mousePressed(ofMouseEventArgs & args){}
	void mouseReleased (ofMouseEventArgs & args);
	void mouseScrolled (ofMouseEventArgs & args);
	void mouseEntered (ofMouseEventArgs & args);
	void mouseExited (ofMouseEventArgs & args);
	
	
	void setupUDP(string _ip, int _port);
	ofxUDPManager udpConnection;
	bool bSendUDP_fMiddleX_fMinY_fUP_fDOWN = false;
	
	//////////////////////////////
	//RESUMED BLOB
	//
	void drawResumedBlob();
	void drawPolylinesAreas();
	void drawGui_Controller();
	void drawGui_OSC_configurable();
	bool isController_ResumedBlob = true;
	void drawGui_ResumedBlob(bool* opened);
	bool bSendOsc_fMiddleX_fMinY_fUP_fDOWN = false;
	bool bresumeBlob_inverX = true;
	//bool bresumeBlob_maxX = true;
	//bool bresumeBlob_minX = false;
	//bool bresumeBlob_minY = true;
	//bool bresumeBlob_maxY = false;
	int item_resumedBlob_X = 0;
	
	int item_resumedBlob_Y = 1;
	
	void calcMainBlobLocation();
	
	//////////////////////////////
	//OSC
	void sendOSCBlobData();
	
	//////////////////////////////
	//TODO make a class of this? //Depends if laserBlob may be a different blobType then separate by types
	//Main Controller vars copied from Sensor to avoid very long names in code (multiple calls)
	int sensorWidth, sensorHeight;
	float sensorScale;
	//std::vector<ofPolyline>  myUpdatedBlobs;
	int numBlobsDetected = -1;
	
	int xPosBlob = -1;
	int yPosBlob = -1;
	int wBlob = -1;
	int hBlob = -1;
	////////////////////////////////
	//Polyline areas recognition
	void updateRecognitionBlobsInsideAreas();
	void updateQuadAreasRecognition();
	vector<ofPolyline> polylines;
	int polylinesIndex = -1;
	
	//Patch to save the right Working area position // TODO get this data from sourceSensor // Wait until is being done properly in the GUI
	ofPoint imageRecognitionPosition;
	int imageRecognitionW;
	int imageRecognitionH;
	
	/////////////////////////////////
	//Tracking Blobs detection
	bool isController_trackingMode = false;
	//bool bMoveRight
	
	//Resume Blob MaxMinMiddel detections
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
	bool bResetHostIp = false;
	int PORT = 12345;
	string HOST = "127.0.0.1";//MLP: "192.168.2.254";
	
	
};