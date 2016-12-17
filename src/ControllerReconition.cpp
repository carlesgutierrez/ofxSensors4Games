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
void ControllerReconition::setup(int w, int h, RecognitionMethod _myComputeBlobType, ofxCv::ContourFinder * _contourFinder, int _idCntroller){

	idController = _idCntroller;

	myContourFinder = _contourFinder;//Copy Adress 

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
	
	//Area ref tranking
	rectAreaPlayer.set(0, 0, sensorWidth*sensorScale, sensorHeight*sensorScale);

	
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

ofxJSONElement ControllerReconition::getParams()
{
	ofxJSONElement jsonParams;
	jsonParams.clear();

	if (idController == 1) {
		jsonParams["idController1"]["learningTime"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.learningTime);
		jsonParams["idController1"]["thresholdValue"], ofToString(SensorManager::getInstance()->computerVisionSensor1.thresholdValue);
		jsonParams["idController1"]["bSimpleBackgroundSubstraction"], ofToString(SensorManager::getInstance()->computerVisionSensor1.bSimpleBackgroundSubstraction);
		jsonParams["idController1"]["bLearnBackground"], ofToString(SensorManager::getInstance()->computerVisionSensor1.bLearnBackground);
		jsonParams["idController1"]["bAutoThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor1.bAutoThreshold);
		jsonParams["idController1"]["thresholdValue"], ofToString(SensorManager::getInstance()->computerVisionSensor1.thresholdValue);
		jsonParams["idController1"]["bContourFinderThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor1.bContourFinderThreshold);
		jsonParams["idController1"]["bContourFinderColorThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold);
	}
	else if(idController == 1) {
		jsonParams["idController2"]["learningTime"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.learningTime);
		jsonParams["idController2"]["thresholdValue"], ofToString(SensorManager::getInstance()->computerVisionSensor2.thresholdValue);
		jsonParams["idController2"]["bSimpleBackgroundSubstraction"], ofToString(SensorManager::getInstance()->computerVisionSensor2.bSimpleBackgroundSubstraction);
		jsonParams["idController2"]["bLearnBackground"], ofToString(SensorManager::getInstance()->computerVisionSensor2.bLearnBackground);
		jsonParams["idController2"]["bAutoThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor2.bAutoThreshold);
		jsonParams["idController2"]["thresholdValue"], ofToString(SensorManager::getInstance()->computerVisionSensor2.thresholdValue);
		jsonParams["idController2"]["bContourFinderThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor2.bContourFinderThreshold);
		jsonParams["idController2"]["bContourFinderColorThreshold"], ofToString(SensorManager::getInstance()->computerVisionSensor2.bContourFinderColorThreshold);

	}
	else cout << "Error Saving idController not recognized" << endl;

	return jsonParams;
}

//-------------------------------------------------------------------
bool ControllerReconition::setParams(ofxJSONElement jsonFile)
{
	bool bLoaded = true;


	if (idController == 1) {

		SensorManager::getInstance()->computerVisionSensor1.learningTime = ofToFloat(jsonFile["idController1"]["learningTime"].asString());
		SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile["idController1"]["thresholdValue"].asString());
		SensorManager::getInstance()->computerVisionSensor1.bSimpleBackgroundSubstraction = ofToFloat(jsonFile["idController1"]["bSimpleBackgroundSubstraction"].asString());
		SensorManager::getInstance()->computerVisionSensor1.bLearnBackground = ofToFloat(jsonFile["idController1"]["bLearnBackground"].asString());
		SensorManager::getInstance()->computerVisionSensor1.bAutoThreshold = ofToFloat(jsonFile["idController1"]["bAutoThreshold"].asString());
		SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile["idController1"]["thresholdValue"].asString());
		SensorManager::getInstance()->computerVisionSensor1.bContourFinderThreshold = ofToFloat(jsonFile["idController1"]["bContourFinderThreshold"].asString());
		SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold = ofToFloat(jsonFile["idController1"]["bContourFinderColorThreshold"].asString());
	}
	else if (idController == 2) {
		SensorManager::getInstance()->computerVisionSensor2.learningTime = ofToFloat(jsonFile["idController2"]["learningTime"].asString());
		SensorManager::getInstance()->computerVisionSensor2.thresholdValue = ofToFloat(jsonFile["idController2"]["thresholdValue"].asString());
		SensorManager::getInstance()->computerVisionSensor2.bSimpleBackgroundSubstraction = ofToFloat(jsonFile["idController2"]["bSimpleBackgroundSubstraction"].asString());
		SensorManager::getInstance()->computerVisionSensor2.bLearnBackground = ofToFloat(jsonFile["idController2"]["bLearnBackground"].asString());
		SensorManager::getInstance()->computerVisionSensor2.bAutoThreshold = ofToFloat(jsonFile["idController2"]["bAutoThreshold"].asString());
		SensorManager::getInstance()->computerVisionSensor2.thresholdValue = ofToFloat(jsonFile["idController2"]["thresholdValue"].asString());
		SensorManager::getInstance()->computerVisionSensor2.bContourFinderThreshold = ofToFloat(jsonFile["idController2"]["bContourFinderThreshold"].asString());
		SensorManager::getInstance()->computerVisionSensor2.bContourFinderColorThreshold = ofToFloat(jsonFile["idController2"]["bContourFinderColorThreshold"].asString());
	}
	else cout << "Error setting idController not recognized" << endl;

	return bLoaded;
}

//-----------------------------------------
void ControllerReconition::update(ofRectangle rectAreaPlayer){

	/*
	updateQuadAreasRecognition();
	*/
	updateRecognitionSystem(rectAreaPlayer);
	
	if (numBlobsDetected > 0) {
		sendOSCBlobData();
	}else{
		//do nothing
	}
	
	if(bResetHostIp){
		sender.setup(HOST, PORT);
		setupUDP(HOST, 29095); // 29095 port is used in GODOT (game engine) software.
		bResetHostIp = false;
	}
}


//-----------------------------------------
void ControllerReconition::updateRecognitionSystem(ofRectangle _rectAreaPlayer){
	
	rectAreaPlayer = _rectAreaPlayer;

	//If new SensorFrame
	if (SensorManager::getInstance()->isNewSensorFrame()) {
	
		if(myComputeBlobType == MaxMinsAllBlob){
			//Calc Max Mins and get the relative position to the Camera. //TODO get relative pos to the area1,2,n
			calcMainBlobLocation(_rectAreaPlayer);
			//WIP Calc some average to detect UP or Down Sudden movemnts.
			udpateRecognitionBlobAction();

		}
		else if(myComputeBlobType == TrackingBlobs){
			//TODO
			//Process all this Tracking data as you want.
			//Get some relevant situations of all of them
			//like density factor, distance average, velocity average,
			//check clusters aras
			//	- then find with blob is alone in his area
			//  - then find wich blob is in a populated area
			// Etc...
		}
		//TODO try to detect vector direction actions
		//else if (myComputeBlobType == KalmanVectorDirDetection) {
		//}
		else {
			cout << "Controller Error:: Update Recognition needs to know the Type of Data to process from SensorManager" << endl;
		}
	}

}

//-----------------------------------------
void ControllerReconition::calcMainBlobLocation(ofRectangle _rectAreaPlayer){
	//Udpate here desired values
	numBlobsDetected = myContourFinder->getContours().size();
	
	calculateMaxMin();
	
	//TODO set here the different blob detection
	if(item_resumedBlob_X == 0)xPosBlob = xMax.x;//max contourn point (right) X
	else if(item_resumedBlob_X == 1)xPosBlob = xMin.x;//min contourn point (left) X
	else if(item_resumedBlob_X == 2)xPosBlob = xMin.x + xDiff*0.5;//middle pos X of all contourns blobs detected
	
	if(item_resumedBlob_Y == 0)yPosBlob = yMax.y;//idem Y
	else if(item_resumedBlob_Y == 1)yPosBlob = yMin.y;//idem Y
	else if(item_resumedBlob_Y == 2)yPosBlob = yMin.y + yDiff*0.5;//middle pos Y of all contourns blobs detected
	
	//Find the relative position indide the Rectangle
	//and save it inside the OSC/UPD vars ready to send out
	float insideRectPosX = xPosBlob - _rectAreaPlayer.x;
	float insideRectPosY = _rectAreaPlayer.y - yPosBlob;

	//cout << "_rectAreaPlayer.x = " << _rectAreaPlayer.x << endl;
	//cout << "xPosBlob = " << xPosBlob << endl;
	//cout << " xPosBlob - _rectAreaPlayer.x = insideRectPosX = " << insideRectPosX << endl;

	if (_rectAreaPlayer.inside(xPosBlob, yPosBlob)) {
		xPosBlobFloatOsc = ofMap(insideRectPosX, 0, _rectAreaPlayer.width, 0, 1);
		yPosBlobFloatOsc = ofMap(insideRectPosY, 0, _rectAreaPlayer.height, 0, 1);
		if (bresumeBlob_inverX) {
			xPosBlobFloatOsc = 1 - xPosBlobFloatOsc;
		}
	}
	else {
		//xPosBlobFloatOsc = 0.5;
		//yPosBlobFloatOsc = 0.5;
		cout << "Error xPosBlob outside the area rectangle" << endl;
	}
}

//----------------------------------------------------------------
void ControllerReconition::calculateMaxMin(){
	
	if( myContourFinder->getPolylines().size() > 0 ){
		xMin.x=sensorWidth,
		xMax.x=0.0,
		yMin.y=sensorHeight,
		yMax.y=0.0;
	
	
		for (int i = 0; i < myContourFinder->getPolylines().size(); i++)
		{
		
			int length_of_contour = myContourFinder->getPolylines()[i].getVertices().size();
		
			//for Each blob seach Max Mins
			for(int j = 0; j < length_of_contour; j++){
			
				ofVec2f tmpPos = myContourFinder->getPolylines()[i].getVertices()[j];
			
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
	
	if (idController == 1) {

		if (bSendOsc_fMiddleX_fMinY_fUP_fDOWN) {

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

		if (bSendUDP_fMiddleX_fMinY_fUP_fDOWN) {

			string message = "/GameBlob ffff ";
			message = message + ofToString(xPosBlobFloatOsc, 2) + " " + ofToString(yPosBlobFloatOsc, 2) + " " + ofToString(fUpActionBlob_OSC, 2) + " " + ofToString(fDownActionBlob_OSC, 2);
			udpConnection.Send(message.c_str(), message.length());

			//cout << "message UDP = " << message << endl;
			//cout << "UPD properties GetTimeoutSend = " << udpConnection.GetTimeoutSend();

		}
	}
	else {
		if (bSendOsc_fMiddleX_fMinY_fUP_fDOWN) {

			ofxOscMessage m;
			m.clear();
			m.setAddress("/GameBlob2");
			m.addFloatArg(xPosBlobFloatOsc);
			m.addFloatArg(yPosBlobFloatOsc);

			// sending float to be able to make more actions filtering in the client.
			//Like Intenisty of the action
			m.addFloatArg(fUpActionBlob_OSC);
			m.addFloatArg(fDownActionBlob_OSC);

			sender.sendMessage(m, false);
		}

		if (bSendUDP_fMiddleX_fMinY_fUP_fDOWN) {

			string message = "/GameBlob2 ffff ";
			message = message + ofToString(xPosBlobFloatOsc, 2) + " " + ofToString(yPosBlobFloatOsc, 2) + " " + ofToString(fUpActionBlob_OSC, 2) + " " + ofToString(fDownActionBlob_OSC, 2);
			udpConnection.Send(message.c_str(), message.length());

			//cout << "message UDP = " << message << endl;
			//cout << "UPD properties GetTimeoutSend = " << udpConnection.GetTimeoutSend();

		}
	}

	
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	ofSetColor(ofColor::white);
	
	drawGui_Controller();
	
	//drawPolylinesAreas();
	
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
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	
	//cout << "Check this Out PreVious window" << endl;
	//bool wopen = true;
	string myControlerIdText = "ControllerRecognition Ctrl" + ofToString(idController);
	
	ImGui::Begin(myControlerIdText.c_str());
		
	drawGui_ResumedBlob();

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

//-------------------------------------------------
void ControllerReconition::drawGui_ResumedBlob(){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	string myControlerIdText = "MaxMins CtrlRecognition " + ofToString(idController);

	if (ImGui::Begin(myControlerIdText.c_str())) {
		
		
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
	
	ofEnableAlphaBlending();
	
	//Draw Vector Interaction for Jostick Mode
	ofPushStyle();
	ofColor myLineColor;
	if(idController == 1)myLineColor = ofColor::green;
	else if(idController == 2)myLineColor = ofColor::pink;
	else myLineColor = ofColor::orangeRed; //Error color line not definet yet

	ofSetColor(myLineColor.r, myLineColor.g, myLineColor.b, 150);

	ofDrawCircle(xPosBlob*sensorScale,
		yPosBlob*sensorScale + sensorWidth*(idController-1)*sensorScale, //Auto Down draw Variable
		10 * sensorScale);//Painting blob result over the Kinect Blob Drawer

	ofSetLineWidth(2);
	ofSetColor(myLineColor.r, myLineColor.g, myLineColor.b, 150);
	ofDrawLine(rectAreaPlayer.getCenter().x*sensorScale,
				rectAreaPlayer.getCenter().y*sensorScale + 0.5*sensorWidth*(idController - 1)*sensorScale, //Auto Down draw Variable,
				xPosBlob*sensorScale,
				yPosBlob*sensorScale + sensorWidth*(idController - 1)*sensorScale);
	
	ofDisableAlphaBlending();
	ofPopStyle();
	ofPopMatrix();
}


//-----------------------------------------
void ControllerReconition::keyReleased(ofKeyEventArgs & args){}
void ControllerReconition::keyPressed(ofKeyEventArgs & args){}

//-----------------------------------------
//void ControllerReconition::mouseMoved(ofMouseEventArgs & args){}
//void ControllerReconition::mouseDragged(ofMouseEventArgs & args){}
//void ControllerReconition::mousePressed(ofMouseEventArgs & args){}
void ControllerReconition::mouseScrolled(ofMouseEventArgs & args){}
void ControllerReconition::mouseEntered(ofMouseEventArgs & args){}
void ControllerReconition::mouseExited(ofMouseEventArgs & args){}
void ControllerReconition::mouseReleased (ofMouseEventArgs & args){}

//-----------------------------------------
void ControllerReconition::exit(){
	sender.~ofxOscSender(); //TODO how to do this properly. Shutdown() method is private.
}
