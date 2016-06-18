//
//  ControllerReconition.h
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//
#pragma once

#include "ofMain.h"
#include "ofxCvBlob.h"
#include "ofxImGui.h"

class ControllerReconition {
public:
	
	void setup(int w, int h);
	void update(vector<ofxCvBlob>  _myUpdatedBlobs);
	void draw();
	void exit();
	void keyPressed(int key);
	void drawGuiControllerOptions(bool* opened);
	
	//TODO make a class of this
	//Controller vars
	int sensorWidth, sensorHeight;
	vector<ofxCvBlob>  myUpdatedBlobs;
	int numBlobsDetected = -1;
	
	int xPosBlob = -1;
	int yPosBlob = -1;
	int wBlob = -1;
	int hBlob = -1;
	
	bool bJump = false;
	//TODO other movements detection
	//bool bMoveLeft = false;
	//bool bMoveRight
	
	//Some simple blob Min Max Points detection
	//Post Tracking detection things
	ofVec2f		xMin,xMax,yMin,yMax;
	float xDiff;
	float yDiff;
	void calculateMaxMin();
	
	//OSC filterd data
	float xPosBlobFloatOsc;
	float yPosBlobFloatOsc;
	
	
};