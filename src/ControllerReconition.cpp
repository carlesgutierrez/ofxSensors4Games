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
	sensorScale = SensorManager::getInstance()->sensorDrawScale;
	
	numAverageFrammes = 100;
	medianBlobHeightValue.setup(numAverageFrammes, 0, sensorHeight);
	
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
	//TODO Check if both can be created at same time. Theoretically if PORTS are different should work
	setupUDP(HOST, 29095); // 29095 port is used in GODOT (game engine) software.
	
	myComputeBlobType = _myComputeBlobType;
	
	//polyline
	polylinesIndex = -1;
	
	//ofRectangle rec1 = ofRectangle(0, 0, 0, 0);
	ofPolyline pol1;
	pol1.clear();
	polylines.push_back(pol1);
	ofPolyline pol2;
	pol2.clear();
	polylines.push_back(pol2);
	
	imageRecognitionPosition = ofPoint(sensorWidth*sensorScale, SensorManager::getInstance()->marginDraw);
	imageRecognitionW = sensorWidth*sensorScale;
	imageRecognitionH = sensorHeight*sensorScale;
	
	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
}

void ControllerReconition::setupUDP(string _ip, int _port){
	
	//create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect(_ip.c_str(),_port);
	udpConnection.SetNonBlocking(true);

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
		//cout << "polylines.size() " << polylines.size() << endl;
		for(int j = 0; j < polylines.size(); j++){
			if(polylines[j].isClosed()){
				if(polylines[j].inside(tmpPos*sensorScale)){
					int label = SensorManager::getInstance()->contourFinder.getLabel(i);
					cout << " Yey Im Inside this Poline " << j << " Blob # " <<  ofToString(label,0) << endl;
					//cout << " polylines[j] = " <<  polylines[j].getCentroid2D() << endl;
					//cout << " Blobs[i] = " <<  SensorManager::getInstance()->contourFinder.getCentroid(i) << endl;
				
					//TODO CHeck how to save in that is being tracked inside a polynine area X
				}
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
	
	if(bResetHostIp){
		sender.setup(HOST, PORT);
		setupUDP(HOST, 29095); // 29095 port is used in GODOT (game engine) software.
		bResetHostIp = false;
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
	
	//TODO set here the different blob detection
	if(item_resumedBlob_X == 0)xPosBlob = xMax.x;//max contourn point (right) X
	else if(item_resumedBlob_X == 1)xPosBlob = xMin.x;//min contourn point (left) X
	else if(item_resumedBlob_X == 2)xPosBlob = xMin.x + xDiff*0.5;//middle pos X of all contourns blobs detected
	
	if(item_resumedBlob_Y == 0)yPosBlob = yMax.y;//idem Y
	else if(item_resumedBlob_Y == 1)yPosBlob = yMin.y;//idem Y
	else if(item_resumedBlob_Y == 2)yPosBlob = yMin.y + yDiff*0.5;//middle pos Y of all contourns blobs detected
	
	//if(bresumeBlob_minY)yPosBlob = yMin.y;
	//else if(bresumeBlob_maxY)yPosBlob = yMax.x;
	
	//OP2 Used max min calculated
	//xPosBlob = xMax.x;
	//xPosBlob = xMin.x + xDiff*0.5;//middle pos
	//yPosBlob = yMin.y;
	
	//Filtered for OSC and Gui Controller
	if(bresumeBlob_inverX){
		xPosBlobFloatOsc = (float)(sensorWidth - xPosBlob) / (float)sensorWidth; //Middle pos
		
	}else{
		xPosBlobFloatOsc = (float)(xPosBlob) / (float)sensorWidth;
	}
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
	
	if(bSendOsc_fMiddleX_fMinY_fUP_fDOWN){
	
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
	
	if(bSendUDP_fMiddleX_fMinY_fUP_fDOWN){
		
		string message = "/GameBlob ffff ";
		message = message + ofToString(xPosBlobFloatOsc,2) + " " + ofToString(yPosBlobFloatOsc,2) + " " + ofToString(fUpActionBlob_OSC,2) + " " + ofToString(fDownActionBlob_OSC,2);
		udpConnection.Send(message.c_str(),message.length());
		
		cout << "message UDP = " << message << endl;
	
	}
	//TODO send here other data type
	//for(areas)
	//if(bSendOsc_fMiddleX_fMinY_fUP_fDOWN_Area1)
	//if(bSendOsc_fMiddleX_fMinY_fUP_fDOWN_Area2)
	//...
	//if(bSendOsc_Recognized_MoveMent1)
	//..
	
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	ofSetColor(ofColor::white);
	
	drawGui_Controller();
	
	drawPolylinesAreas();
	
}

//-------------------------------------------------
void ControllerReconition::drawPolylinesAreas(){
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

//-------------------------------------------------
void ControllerReconition::drawGui_OSC_configurable(){
	
	//if (ImGui::Begin("OSC Window")) {
		ImGui::Text("Sending OSC data to ");
		ImGui::Text(ofToString(PORT,0).c_str());
		ImGui::Text(HOST.c_str());
		
		//TODO InputTextFilterCharacter
		static char buf1[16] = "127.0.0.1";
		ImGui::PushItemWidth(90);
		ImGui::InputText("WIP Edit Host", buf1, 16);ImGui::SameLine();
		ImGui::Checkbox("Reset HOST IP", &bResetHostIp);
		ImGui::PopItemWidth();
		
		if(bResetHostIp){
			HOST = std::string(buf1);
			cout << "Lets reset HOST IP" << endl;
		}
		
		//ImGui::End();
	//}
}

//-------------------------------------------------
void ControllerReconition::drawGui_Controller(){
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	

	//bool wopen = true;
	if (ImGui::Begin("Controller Window")) {
		
		ImGui::Checkbox("MultiTracking Blob Detection", &isController_trackingMode);
		ImGui::Checkbox("Resumed Blob Detection", &isController_ResumedBlob);
		
		if (isController_ResumedBlob) {
			drawGui_ResumedBlob(&isController_ResumedBlob);
		}
		
		if(isController_ResumedBlob) {
			
		}
		
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		
		
		//Draw and Edit OSC info
		drawGui_OSC_configurable();
		
		ImGui::End();
	}

}

//-------------------------------------------------
void ControllerReconition::drawGui_ResumedBlob(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("p & Down + MaxMin Recognition", opened)) {
		
		
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

		const char* combo_resumedBlob_X[] = { "bresumeBlob_maxX", "bresumeBlob_minX", "bresumeBlob_middleX" };
		const char* combo_resumedBlob_Y[] = { "bresumeBlob_maxY", "bresumeBlob_minY", "bresumeBlob_middleY" };
		
		ImGui::Combo("ResumeBlob X type", &item_resumedBlob_X, combo_resumedBlob_X, IM_ARRAYSIZE(combo_resumedBlob_X));
		ImGui::Combo("ResumeBlob Y type", &item_resumedBlob_Y, combo_resumedBlob_Y, IM_ARRAYSIZE(combo_resumedBlob_Y));
		
		ImGui::Checkbox("Send UDP", &bSendUDP_fMiddleX_fMinY_fUP_fDOWN);
		
		if(bSendUDP_fMiddleX_fMinY_fUP_fDOWN){
			
			ImGui::Checkbox("OSC Invert X", &bresumeBlob_inverX);
			
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("(f0)##fMiddleX_fMinY_fUP_fDOWN", &xPosBlobFloatOsc, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f1)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50),&yPosBlobFloatOsc, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f2)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50), &fUpActionBlob_OSC, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f3)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50), &fDownActionBlob_OSC, 0, 1);
			//ImGui::SliderFloat("(f2) fUpActionBlobOSC", &fUpActionBlob_OSC, 0, 1);
			//ImGui::SameLine();
			//ImGui::SliderFloat("(f3) fDownActionBlobOSC", &fDownActionBlob_OSC, 0, 1);
			string dataDescription1 = "(f0) xOSCBlob - (f1) yOSCBlob ";
			string dataDescription2 = "(f2) fUpActionBlobOSC - (f3) fDownActionBlobOSC";
			ImGui::Text(dataDescription1.c_str());
			ImGui::Text(dataDescription2.c_str());
			
			ImGui::PopItemWidth();
		}
		
		ImGui::Checkbox("Send OSC", &bSendOsc_fMiddleX_fMinY_fUP_fDOWN);
		if(bSendOsc_fMiddleX_fMinY_fUP_fDOWN){
			
			ImGui::Checkbox("Send Inverted X", &bresumeBlob_inverX);
			
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("(f0)##fMiddleX_fMinY_fUP_fDOWN", &xPosBlobFloatOsc, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f1)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50),&yPosBlobFloatOsc, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f2)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50), &fUpActionBlob_OSC, 0, 1);ImGui::SameLine();
			ImGui::VSliderFloat("(f3)##fMiddleX_fMinY_fUP_fDOWN", ImVec2(20, 50), &fDownActionBlob_OSC, 0, 1);
			//ImGui::SliderFloat("(f2) fUpActionBlobOSC", &fUpActionBlob_OSC, 0, 1);
			//ImGui::SameLine();
			//ImGui::SliderFloat("(f3) fDownActionBlobOSC", &fDownActionBlob_OSC, 0, 1);
			string dataDescription1 = "(f0) xOSCBlob - (f1) yOSCBlob ";
			string dataDescription2 = "(f2) fUpActionBlobOSC - (f3) fDownActionBlobOSC";
			ImGui::Text(dataDescription1.c_str());
			ImGui::Text(dataDescription2.c_str());

			ImGui::PopItemWidth();
		}
		
		//TODO Add This to plot gui
		//medianBlobHeightValue.draw(500 , 500, sensorWidth, 100, 100, "medianBlobHeightValue", true, 150);
		
		ImGui::End();
		
		
		//Draw Detected Resumed Point
		drawResumedBlob();

	}
}

void ControllerReconition::drawResumedBlob(/*int transX ,int transY, int winW, int winH*/){
	//TODO add Translate X, translate Y , Point Position ( 0 .. 1 ), WidthWindow, HeightWindow
 
	ofPushMatrix();
	ofTranslate(sensorWidth*sensorScale, SensorManager::getInstance()->marginDraw);
	
	ofColor myMaxMinPointColor = ofColor::green;
	ofSetColor(myMaxMinPointColor.r, myMaxMinPointColor.g, myMaxMinPointColor.b, 150);
	
	ofEnableAlphaBlending();
	
	ofDrawCircle(xPosBlob*sensorScale,
				 yPosBlob*sensorScale,
				 10*sensorScale);//Painting blob result over the Kinect Blob Drawer
	
	//Draw Vector Interaction for Jostick Mode
	//If no polyline Area, then use middle point sensor
	ofColor myLineColor = ofColor::green;
	ofSetColor(myLineColor.r, myLineColor.g, myLineColor.b, 150);
	ofDrawLine(sensorWidth*sensorScale*0.5, sensorHeight*sensorScale*0.5, xPosBlob*sensorScale, yPosBlob*sensorScale);
	
	ofDisableAlphaBlending();
	
	ofPopMatrix();
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
	else if(args.key == OF_KEY_BACKSPACE){
		
		//TESTING OSC
		
		ofxOscMessage m;
		m.clear();
		m.setAddress("/GameBlob");
		m.addFloatArg(ofGetMouseX()/ofGetWidth()); // 0 .. 1 segun Mouse X
		m.addFloatArg(0); // 0 .. 1 segun Mouse Y
		
		// sending float to be able to make more actions filtering in the client.
		//Like Intenisty of the action
		m.addFloatArg(1 - (ofGetMouseY()/ofGetHeight())); //Jump Action!
		m.addFloatArg(1);
		
		sender.sendMessage(m, false);
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
	
	//cout << "args.x = " << args.x << endl;
	//cout << "args.y = " << args.y << endl;

	//cout << "imageRecognitionPosition.x = " << imageRecognitionPosition.x << endl;
	//cout << "imageRecognitionPosition.y = " << imageRecognitionPosition.y << endl;
	
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
