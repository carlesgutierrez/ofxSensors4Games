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
void ControllerReconition::setup(int w, int h, RecognitionMethod _myComputeBlobType){
	sensorWidth = w;
	sensorHeight = h;
	
	numAverageFrammes = 100;
	medianBlobHeightValue.setup(numAverageFrammes, 0, sensorHeight);
	
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
	
	myComputeBlobType = _myComputeBlobType;
}

//-----------------------------------------
void ControllerReconition::update(){
	
	//What at copy
	//myUpdatedBlobs = _myUpdatedBlobs;
	
	updateRecognitionSystem();
	
	sendOSCBlobData();
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	//Draw Detected Point
	ofSetColor(ofColor::red);
	ofDrawCircle(10 + xPosBlob*0.5, 320 + yPosBlob*0.5, 10);//Painting blob result over the Kinect Blob Drawer
	
	//Draw some Sensor Option
	ofSetColor(ofColor::white);
	bool isControllerWindow = true;
	if (isControllerWindow) {
		drawGuiControllerOptions(&isControllerWindow);
	}
}

//-----------------------------------------
void ControllerReconition::updateRecognitionSystem(){
	
	//If new SensorFrame
	if (SensorManager::getInstance()->isNewSensorFrame()) {
	//	cout << "Singletoon Working !!! !!! !!! ! !!! !!! !" << endl;
	
		if(myComputeBlobType == MaxMinsAllBlob){
			calcMainBlobLocation();
			//Recognize Blob Action
			//TODO recover wierd error here...
			udpateRecognitionBlobAction();

		}else {
			//by default get just bigger blob and update cacl values to send OSC too
			if(SensorManager::getInstance()->contourFinder.getContours().size() > 0){
				//TODO
				//xPosBlob = myUpdatedBlobs[0].centroid.x;
				//yPosBlob = myUpdatedBlobs[0].centroid.y;
			}
		
		//No Up & down Detections
	}
	}

}

//-----------------------------------------
void ControllerReconition::calcMainBlobLocation(){
	//Udpate here desired values
	numBlobsDetected = SensorManager::getInstance()->contourFinder.getContours().size();
	
	calculateMaxMin();
	
	//OP2 Used max min calculated
	xPosBlob = xMin.x + xDiff*0.5;
	yPosBlob = yMin.y;
	
	//Filtered for OSC and Gui Controller
	xPosBlobFloatOsc = (float)(sensorWidth - xPosBlob) / (float)sensorWidth;
	yPosBlobFloatOsc = (float)(yPosBlob) / (float)sensorHeight;
}

//----------------------------------------------------------------
void ControllerReconition::calculateMaxMin(){
	
	if( SensorManager::getInstance()->contourFinder.getPolylines().size() > 0 ){
		xMin.x=sensorWidth,
		xMax.x=0.0,
		yMin.y=sensorHeight,
		yMax.y=0.0;
	
	
		for (int i = 0; i < SensorManager::getInstance()->contourFinder.getPolylines().size(); i++)
		{
		
			int length_of_contour = SensorManager::getInstance()->contourFinder.getPolylines()[i].getVertices().size();
		
			//for Each blob seach Max Mins
			for(int j = 0; j < length_of_contour; j++){
			
				ofVec2f tmpPos = SensorManager::getInstance()->contourFinder.getPolylines()[i].getVertices()[j];
			
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
	
}

//-----------------------------------------
void ControllerReconition::udpateRecognitionBlobAction(){
	
	//Udpate stats
	medianBlobHeightValue.update(yPosBlob);
	
	//Check yPosBlob is > medianBlobHeightValue
	medianHeightBlob = medianBlobHeightValue.getAverage(numAverageFrammes);
	
	
	//Means Last Values are smaller than the average, so , This is DOWN DIR
	if(medianBlobHeightValue.getLastValueNormal() - medianHeightBlob < 0){
		fUpActionBlob = medianHeightBlob - medianBlobHeightValue.getLastValueNormal();
		fUpActionBlob_OSC = ofMap(fUpActionBlob, 0, 1, 0, 1);
	}
	else {
		fDownActionBlob = medianBlobHeightValue.getLastValueNormal() - medianHeightBlob;
		fDownActionBlob_OSC = ofMap(fDownActionBlob, 0, 1, 0, 1);
	}
	
	//Check yPosBlob is < medianBlobHeightValue
}



//-----------------------------------------
void ControllerReconition::sendOSCBlobData(){
	ofxOscMessage m;
	m.clear();
	m.setAddress("/GameBlob");
	m.addFloatArg(xPosBlobFloatOsc);
	m.addFloatArg(yPosBlobFloatOsc);
	
	// sending float to be able to make more actions filtering in the client.
	//Like Intenisty of the action
	m.addFloatArg(fUpActionBlob_OSC);
	m.addFloatArg(fDownActionBlob_OSC);
	
	sender.sendMessage(m, false);
}



//-----------------------------------------
void ControllerReconition::exit(){
	
}

//-----------------------------------------
void ControllerReconition::keyPressed(int key){
	
}




//-------------------------------------------------
void ControllerReconition::drawGuiControllerOptions(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("Controller Options Gui", opened, ImGuiWindowFlags_MenuBar)) {
		
		//ImGui::PushItemWidth(100);
		string recognitionTextType = "Using Kinect & MaxMin Recognition";
		
		ImGui::Text(recognitionTextType.c_str());
		
		ImGui::Separator();
		ImGui::Text("Up&Down Detection config:");
		
		if(ImGui::Button("Reset Averega", ImVec2(120,20))){
			medianBlobHeightValue.reset();
			medianBlobHeightValue.setup(numAverageFrammes, 0, sensorHeight);
		}

		ImGui::SliderInt("Frames used", &numAverageFrammes, 0, 100);
		ImGui::SliderFloat("medianHeightBlob", &medianHeightBlob, 0, 1);
		
		ImGui::Separator();
		
		string textOscInfo0 = "Sending OSC data:";
		string textOscInfo1 = "Port:";
		string textOscInfo2 = "Ip:";
		
		ImGui::Text("Sending OSC data to ");
		ImGui::Text(ofToString(PORT,0).c_str());
		ImGui::Text(HOST.c_str());
		//ImGui::InputText("Host IP", HOST, IM_ARRAYSIZE(HOST));
		//ImGui::InputText("Port Num", PORTText, IM_ARRAYSIZE(PORTText));
		ImGui::SliderFloat("(OSC f 0) xOSCBlob", &xPosBlobFloatOsc, 0, 1);
		ImGui::SliderFloat("(OSC f 1) yOSCBlob", &yPosBlobFloatOsc, 0, 1);
		ImGui::SliderFloat("(OSC f 2) fUpActionBlobOSC", &fUpActionBlob_OSC, 0, 1);
		ImGui::SliderFloat("(OSC f 3) fDownActionBlobOSC", &fDownActionBlob_OSC, 0, 1);
		
		
		//ImGui::PopItemWidth();
		
		
		ImGui::End();
		
		medianBlobHeightValue.draw(500 , 500, 320, 100, 100, "medianBlobHeightValue", true, 150);
	}
}

