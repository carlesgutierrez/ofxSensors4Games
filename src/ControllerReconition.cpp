//
//  cvManagement.cpp
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//

#include "ControllerReconition.h"

//This Class Will perform hight filter game Controller recognition things like
// Set borders

//-----------------------------------------
void ControllerReconition::setup(int w, int h){
	sensorWidth = w;
	sensorHeight = h;
	
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
}

//-----------------------------------------
void ControllerReconition::update(vector<ofxCvBlob>  _myUpdatedBlobs){
	
	//What at copy
	myUpdatedBlobs = _myUpdatedBlobs;
	
	calcMainBlobLocation();
	
	sendOSCBlobData();
}

void ControllerReconition::calcMainBlobLocation(){
	//Udpate here desired values
	numBlobsDetected = myUpdatedBlobs.size();
	
	calculateMaxMin();
	
	//OP1 using just the biggger blob.
	//if(numBlobsDetected > 0){
	//	xPosBlob = SensorManager::getInstance()->contourFinder.blobs[0].centroid.x;
	//	yPosBlob = SensorManager::getInstance()->contourFinder.blobs[0].centroid.y;
	//}
	
	//OP2 Used max min calculated
	xPosBlob = xMin.x + xDiff*0.5;
	yPosBlob = sensorWidth - yMin.y;
	
	//Filtered for OSC and Gui Controller
	xPosBlobFloatOsc = (float)xPosBlob / (float)sensorWidth;
	yPosBlobFloatOsc = (float)yPosBlob / (float)sensorHeight;
}

//-----------------------------------------
void ControllerReconition::sendOSCBlobData(){
	ofxOscMessage m;
	m.setAddress("/PangBlob");
	m.addFloatArg(xPosBlobFloatOsc);
	m.addFloatArg(yPosBlobFloatOsc);
	sender.sendMessage(m, false);
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	//Draw Detected Point
	ofSetColor(ofColor::red);
	ofCircle(10 + xPosBlob*0.5, 320 + yPosBlob*0.5, 10);//Painting blob result over the Kinect Blob Drawer
	
	//Draw some Sensor Option
	ofSetColor(ofColor::white);
	bool isControllerWindow = true;
	if (isControllerWindow) {
		drawGuiControllerOptions(&isControllerWindow);
	}
}

//-----------------------------------------
void ControllerReconition::exit(){
	
}

//-----------------------------------------
void ControllerReconition::keyPressed(int key){
	
}

//----------------------------------------------------------------
void ControllerReconition::calculateMaxMin(){
	
	if( myUpdatedBlobs.size() > 0 ){
		xMin.x=sensorWidth,
		xMax.x=0.0,
		yMin.y=sensorHeight,
		yMax.y=0.0;
	}
	
	for (int i = 0; i < myUpdatedBlobs.size(); i++)
	{
		
		int length_of_contour = myUpdatedBlobs[i].pts.size();
		
		for(int j = 0; j < length_of_contour; j++){
			
			ofVec2f tmpPos = myUpdatedBlobs[i].pts[j];
			
			if( tmpPos.x > xMax.x){
				xMax.x=tmpPos.x;
				xMax.y=tmpPos.y;
			}
			
			if( tmpPos.x < xMin.x){
				xMin.x=tmpPos.x;
				xMin.y=tmpPos.y;
			}
			
			if( tmpPos.y > yMax.y){
				yMax.y=tmpPos.y;
				yMax.x=tmpPos.x;
			}
			
			if( tmpPos.y < yMin.y){
				yMin.y=tmpPos.y;
				yMin.x=tmpPos.x;
			}
			
		}
	}
	
	xDiff=xMax.x-xMin.x;
	yDiff=yMax.y-yMin.y;
	
	wBlob	= xDiff;
	hBlob	= yDiff;
	
}


//-------------------------------------------------
void ControllerReconition::drawGuiControllerOptions(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("Controller Gui Window", opened, ImGuiWindowFlags_MenuBar)) {
		
		//ImGui::PushItemWidth(100);
		
		string recognitionTextType = "Using Max Min Blob Data";
		
		ImGui::Text(recognitionTextType.c_str());
		ImGui::Text(("xRawPos = "+ofToString(xPosBlob)).c_str());
		ImGui::Text(("yRawPos = "+ofToString(yPosBlob)).c_str());

		
		ImGui::SliderFloat("xOSCBlob", &xPosBlobFloatOsc, 0, 1);
		ImGui::SliderFloat("yOSCBlob", &yPosBlobFloatOsc, 0, 1);
		
		
		//ImGui::PopItemWidth();
		
		
		ImGui::End();
	}
}

