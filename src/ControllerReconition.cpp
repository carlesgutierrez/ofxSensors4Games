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
	
	//polyline
	polylinesIndex = -1;
	ofPolyline pol1;
	polylines.push_back(pol1);
	ofPolyline pol2;
	polylines.push_back(pol2);
	
	imageRecognitionPosition = ofPoint(sensorWidth*SensorManager::getInstance()->sensorDrawScale, SensorManager::getInstance()->marginDraw);
	imageRecognitionW = sensorWidth*SensorManager::getInstance()->sensorDrawScale;
	imageRecognitionH = sensorHeight*SensorManager::getInstance()->sensorDrawScale;
	
	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
}

//-----------------------------------------
void ControllerReconition::updateQuadAreasRecognition(){
	updateRecognitionBlobsInsideAreas();
}

//-----------------------------------------
void ControllerReconition::updateRecognitionBlobsInsideAreas(){
	
	for (int i = 0; i < SensorManager::getInstance()->contourFinder.size(); i++)
	{
		
		ofPoint tmpPos = ofxCv::toOf(SensorManager::getInstance()->contourFinder.getCentroid(i));
		
		//ALMOST WORKING. CHECK Values: Points and Quads
		
		//Get if this Blob Point is Inside Area 1
		
		for(int j = 0; j < polylines.size(); j++){
		
			if(polylines[j].inside(tmpPos*SensorManager::getInstance()->sensorDrawScale)){
				int label = SensorManager::getInstance()->contourFinder.getLabel(i);
				cout << " Yey Im Inside this Poline " << j << " Blob # " <<  ofToString(label,0) << endl;
				cout << " polylines[j] = " <<  polylines[j].getCentroid2D() << endl;
				cout << " Blobs[i] = " <<  SensorManager::getInstance()->contourFinder.getCentroid(i) << endl;
				
				//TODO CHeck how to save in that is being tracked inside a polynine area X
			}
		
		}
		//Get if this Blob Point is Inside Area 2
		   //idem ...
	}
}


//-----------------------------------------
void ControllerReconition::update(){
	
	updateQuadAreasRecognition();
	
	updateRecognitionSystem();
	
	sendOSCBlobData();
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	//Draw Detected Point
	ofSetColor(ofColor::red);
	if(SensorManager::getInstance()->getSensorType() == kinectSensor){
		ofDrawCircle(10 + xPosBlob*0.5, sensorWidth + yPosBlob*0.5, 10);//Painting blob result over the Kinect Blob Drawer
	}
	else if(SensorManager::getInstance()->getSensorType() == cameraSensor){
		ofDrawCircle(sensorWidth + xPosBlob*0.5, 10 + yPosBlob*0.5, 10);//Painting blob result over the Kinect Blob Drawer
	}
	
	//Draw some Sensor Option
	ofSetColor(ofColor::white);
	bool isControllerWindow = true;
	if (isControllerWindow) {
		drawGuiControllerOptions(&isControllerWindow);
	}
	
	
	if(polylines.size() > 1){
		//Draw polyline Areas
		ofPushMatrix();
		
		ofTranslate(imageRecognitionPosition.x, imageRecognitionPosition.y, 0);
		
		ofSetColor(ofColor::yellowGreen);
		polylines[0].draw();
		vector<ofPoint> vertexes0 = polylines[0].getVertices();
		for(int i = 0; i < vertexes0.size(); i++){
			ofDrawBitmapString("x0 = "+ ofToString(vertexes0[i].x, 0), vertexes0[i].x, vertexes0[i].y);
			ofDrawBitmapString("y0 = "+ ofToString(vertexes0[i].y, 0), vertexes0[i].x, vertexes0[i].y+10);
		}
		
		ofSetColor(ofColor::royalBlue);
		polylines[1].draw();
		vector<ofPoint> vertexes1 = polylines[1].getVertices();
		for(int i = 0; i < vertexes1.size(); i++){
			ofDrawBitmapString("x1 = "+ ofToString(vertexes1[i].x, 0), vertexes1[i].x, vertexes1[i].y);
			ofDrawBitmapString("y1 = "+ ofToString(vertexes1[i].y, 0), vertexes1[i].x, vertexes1[i].y+10);
		}
		ofPopMatrix();
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

//-------------------------------------------------
void ControllerReconition::drawGuiControllerOptions(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("p & Down + MaxMin Recognition", opened, ImGuiWindowFlags_MenuBar)) {
		
		
		string recognitionTextType = "Configure Up&Down Values";
		
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
		
		ImGui::PushItemWidth(100);
		
		ImGui::Text("Sending OSC data to ");
		ImGui::Text(ofToString(PORT,0).c_str());
		ImGui::Text(HOST.c_str());
		//ImGui::InputText("Host IP", HOST, IM_ARRAYSIZE(HOST));
		//ImGui::InputText("Port Num", PORTText, IM_ARRAYSIZE(PORTText));
		ImGui::SliderFloat("(f0) xOSCBlob", &xPosBlobFloatOsc, 0, 1);
		ImGui::SameLine();
		ImGui::SliderFloat("(f1) yOSCBlob", &yPosBlobFloatOsc, 0, 1);
		//ImGui::VSliderFloat("(OSC f 2) fUpActionBlobOSC", ImVec2(20, 50), &fUpActionBlob_OSC, 0, 1);ImGui::SameLine();
		//ImGui::VSliderFloat("(OSC f 3) fDownActionBlobOSC", ImVec2(20, 50), &fDownActionBlob_OSC, 0, 1);
		ImGui::SliderFloat("(f2) fUpActionBlobOSC", &fUpActionBlob_OSC, 0, 1);
		ImGui::SameLine();
		ImGui::SliderFloat("(f13) fDownActionBlobOSC", &fDownActionBlob_OSC, 0, 1);
		
		ImGui::PopItemWidth();
		
		
		ImGui::End();
		
		//TODO Add This to plot gui
		//medianBlobHeightValue.draw(500 , 500, sensorWidth, 100, 100, "medianBlobHeightValue", true, 150);
	}
}


//-----------------------------------------
void ControllerReconition::keyReleased(ofKeyEventArgs & args){}
void ControllerReconition::keyPressed(ofKeyEventArgs & args){
	
	//	cout << "polylinesIndex = " << polylinesIndex << endl;
	
	if(args.key == '1'){
		polylinesIndex = 0;
		polylines[polylinesIndex].clear();
	}
	else if(args.key == '2'){
		polylinesIndex = 1;
		polylines[polylinesIndex].clear();
	}else if(args.key == OF_KEY_RETURN){
		if(polylines.size() > 0 && polylinesIndex < polylines.size() && polylines[polylinesIndex].size() > 0){
			polylines[polylinesIndex].close();
		}
	}

}

//-----------------------------------------
//void ControllerReconition::mouseMoved(ofMouseEventArgs & args){}
//void ControllerReconition::mouseDragged(ofMouseEventArgs & args){}
//void ControllerReconition::mousePressed(ofMouseEventArgs & args){}
void ControllerReconition::mouseScrolled(ofMouseEventArgs & args){}
void ControllerReconition::mouseEntered(ofMouseEventArgs & args){}
void ControllerReconition::mouseExited(ofMouseEventArgs & args){}
void ControllerReconition::mouseReleased (ofMouseEventArgs & args){
	
	cout << "args.x = " << args.x << endl;
	cout << "args.y = " << args.y << endl;

	cout << "imageRecognitionPosition.x = " << imageRecognitionPosition.x << endl;
	cout << "imageRecognitionPosition.y = " << imageRecognitionPosition.y << endl;
	
	if(args.x > imageRecognitionPosition.x && args.x < imageRecognitionPosition.x+imageRecognitionW){
		if(args.y > imageRecognitionPosition.y && args.y < imageRecognitionPosition.y+imageRecognitionH){

			if(polylines.size() > 0 && polylinesIndex < polylines.size()){
				polylines[polylinesIndex].addVertex(ofPoint(args.x- imageRecognitionPosition.x, args.y - imageRecognitionPosition.y));
			}
			
		}
	}
	
}

//-----------------------------------------
void ControllerReconition::exit(){
	sender.~ofxOscSender(); //TODO how to do this properly. Shutdown() method is private.
}
