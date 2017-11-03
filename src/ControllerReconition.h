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
#include "ofxJSON.h"


//Status Regonition Controller Type
enum ActionRecognitionType { GoUpAction, GoDownAction }; //Up and Down from median height blob player.
enum ControllerMethod { MaxMinBlob, /*UpDownLeftRightBlobs ,*/ AllBlobsIn };

class ControllerReconition {
public:
	int idController = -1;
	void setup(int w, int h, ofxCv::ContourFinder * _contourFinder, int idControll/*, int _totalPlayersAreas*/);
	void update(ofRectangle _rectAreaPlayer);
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
	
	/////////////////////
	//Udpated from Camera
	ofxCv::ContourFinder * myContourFinder;

	
	//////////////////////////////
	//OSC
	void sendOSCBlobData();
	
	//////////////////////////////
	//TODO make a class of this? //Depends if laserBlob may be a different blobType then separate by types
	//Main Controller vars copied from Sensor to avoid very long names in code (multiple calls)
	int sensorWidth, sensorHeight;
	float sensorScale;
	int numBlobsDetected = -1;
	int xPosBlob = -1;
	int yPosBlob = -1;
	int wBlob = -1;
	int hBlob = -1;
	ofRectangle rectAreaPlayer;
	
	//Patch to save the right Working area position // TODO get this data from sourceSensor // Wait until is being done properly in the GUI
	ofPoint imageRecognitionPosition;
	int imageRecognitionW;
	int imageRecognitionH;
	
	
	//Advanced Filtered Blob Data
	//median stat value
	ControllerMethod myControllerMethod = AllBlobsIn; // MaxMinBlob; //TODO CHECK TO LAD THIS AT JSON
	detectionMethod  myDetectMethod; //Contournd, tracking Contourns or Future detection methods such YOLO2
	void updateRecognitionSystem(ofRectangle _rectAreaPlayer);
	void udpate_MaxMins_Recognition_UpDown_Actions(float _value, statsRecorder & _stats);
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//Resume Blob MaxMin Blob & OSC RELATED
	ofVec2f		xMin, xMax, yMin, yMax;
	void drawGui_OSCUPD_sendingData();
	void drawGui_Controller();
	void drawGui_HostIP_configurable();
	bool isController_ResumedBlob = true;
	void drawGui_ResumedBlob_MaxMinBlobs();
	void drawResumedBlob_MaxMinBlobs();
	void calcMainBlobLocation(ofRectangle _rectAreaPlayer);
	void send_OSC_UPD_Data_MaxMinBlob(string nameTag);

	void send_OSC_Data_AllInBlobs();

	float xDiff;
	float yDiff;
	void calculateMaxMin();

	int numAverageFrammes;
	statsRecorder medianNormValue;
	float medianResult;
	
	float fUpActionBlob = 0;
	float fDownActionBlob = 0;
	
	float fUpActionBlob_OSC = 0;
	float fDownActionBlob_OSC = 0;
	
	float xPosBlobFloatOsc;
	float yPosBlobFloatOsc;

	int item_resumedBlob_X = 0;
	int item_resumedBlob_Y = 1;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//ALLIN_VARS & OSC VARS RELATED

	///////////////////////////////
	////OSC VARS
	bool bSendOsc_fMiddleX_fMinY_fUP_fDOWN = true;
	bool bresumeBlob_inverX = true;
	bool bresumeBlob_inverY = false;
	//Sender	
	void setupUDP(string _ip, int _port);
	ofxUDPManager udpConnection;
	bool bSendUDP_fMiddleX_fMinY_fUP_fDOWN = true;
	
	//OSC filterd data
	ofxOscSender sender;
	
	//OSC CONFIG
	bool bResetHostIp = false;
	int PORT = 12345;
	string HOST = "127.0.0.1";//MLP: "192.168.2.254";

	//////////////////////////////////////////
	//Json save params
	ofxJSONElement getParams();
	bool setParams(ofxJSONElement myJson, int _idController);
	
};