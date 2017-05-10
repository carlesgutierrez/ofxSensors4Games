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
void ControllerReconition::setup(int w, int h, ofxCv::ContourFinder * _contourFinder, int _idCntroller){

	idController = _idCntroller;

	myContourFinder = _contourFinder;//Copy Adress 

	sensorWidth = w;
	sensorHeight = h;
	sensorScale = SensorManager::getInstance()->sensorDrawScale;
	
	numAverageFrammes = 100;
	medianNormValue.setup(numAverageFrammes, 0, 1);
	
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
	//TODO Check if both can be created at same time. Theoretically if PORTS are different should work
	setupUDP(HOST, 29095); // 29095 port is used in GODOT (The game engine) to receive UPD data
	
		
	//Required to have a area ref for tracking
	rectAreaPlayer.set(0, 0, sensorWidth*sensorScale, sensorHeight*sensorScale);
	
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
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["learningTime"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.learningTime);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.thresholdValue);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bSimpleBackgroundSubstraction"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bSimpleBackgroundSubstraction);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bLearnBackground"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bLearnBackground);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bAutoThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bAutoThreshold);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.thresholdValue);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bContourFinderThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bContourFinderThreshold);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold);
		//jsonParams["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold);
	}
	else if(idController == 2) {
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["learningTime"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.learningTime);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.thresholdValue);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bSimpleBackgroundSubstraction"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.bSimpleBackgroundSubstraction);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bLearnBackground"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.bLearnBackground);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bAutoThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.bAutoThreshold);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.thresholdValue);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bContourFinderThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.bContourFinderThreshold);
		jsonParams["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"] = ofToString(SensorManager::getInstance()->computerVisionSensor2.bContourFinderColorThreshold);

	}
	else cout << "Error Saving idController not recognized" << endl;

	return jsonParams;
}

//-------------------------------------------------------------------
bool ControllerReconition::setParams(ofxJSONElement jsonFile)
{
	bool bLoaded = true;

	//cout << "traying to get data from ControllerRecognition = " << ofToString(idController, 0) << endl;
	//cout << "Data JSon preview = " << jsonFile << endl;
	//cout << "Data JSon Size = " << jsonFile.size() << endl;

	for (int i = 0; i < jsonFile.size(); i++) {
		if (jsonFile[i]["ControllerReconition"].size() > 0) {
			if (jsonFile[i]["ControllerReconition"][ofToString(idController, 0)].size() > 0) {
				//cout << "jsonFile[i][ControllerReconition][x] =" << jsonFile[i]["ControllerReconition"]["1"] << endl;
				
				if (idController == 1) {
					SensorManager::getInstance()->computerVisionSensor1.learningTime = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["learningTime"].asString());
					SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
					SensorManager::getInstance()->computerVisionSensor1.bSimpleBackgroundSubstraction = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bSimpleBackgroundSubstraction"].asString());
					SensorManager::getInstance()->computerVisionSensor1.bLearnBackground = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bLearnBackground"].asString());
					SensorManager::getInstance()->computerVisionSensor1.bAutoThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bAutoThreshold"].asString());
					SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
					SensorManager::getInstance()->computerVisionSensor1.bContourFinderThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bContourFinderThreshold"].asString());
					SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"].asString());
				}
				else if (idController == 2) {
					SensorManager::getInstance()->computerVisionSensor2.learningTime = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["learningTime"].asString());
					SensorManager::getInstance()->computerVisionSensor2.thresholdValue = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
					SensorManager::getInstance()->computerVisionSensor2.bSimpleBackgroundSubstraction = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bSimpleBackgroundSubstraction"].asString());
					SensorManager::getInstance()->computerVisionSensor2.bLearnBackground = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bLearnBackground"].asString());
					SensorManager::getInstance()->computerVisionSensor2.bAutoThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bAutoThreshold"].asString());
					SensorManager::getInstance()->computerVisionSensor2.thresholdValue = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
					SensorManager::getInstance()->computerVisionSensor2.bContourFinderThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bContourFinderThreshold"].asString());
					SensorManager::getInstance()->computerVisionSensor2.bContourFinderColorThreshold = ofToFloat(jsonFile[i]["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"].asString());
				}

				/*
				SensorManager::getInstance()->computerVisionSensor1.learningTime = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["learningTime"].asString());
				SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
				SensorManager::getInstance()->computerVisionSensor1.bSimpleBackgroundSubstraction = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["bSimpleBackgroundSubstraction"].asString());
				SensorManager::getInstance()->computerVisionSensor1.bLearnBackground = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["bLearnBackground"].asString());
				SensorManager::getInstance()->computerVisionSensor1.bAutoThreshold = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["bAutoThreshold"].asString());
				SensorManager::getInstance()->computerVisionSensor1.thresholdValue = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["thresholdValue"].asString());
				SensorManager::getInstance()->computerVisionSensor1.bContourFinderThreshold = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["bContourFinderThreshold"].asString());
				SensorManager::getInstance()->computerVisionSensor1.bContourFinderColorThreshold = ofToFloat(jsonFile["ControllerReconition"][ofToString(idController, 2)]["bContourFinderColorThreshold"].asString());
				*/
			}
		}
	}


	

	return bLoaded;
}

//-----------------------------------------
void ControllerReconition::update(ofRectangle rectAreaPlayer, detectionMethod _detectionMethod){

	myDetectMethod = _detectionMethod; //updated on the fly, if SensorComputerVision Change, then here we will know it
	
	//Update high level data to detect desired Players Actions
	updateRecognitionSystem(rectAreaPlayer);
	
	//OSC
	sendOSCBlobData();
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

		//update the num of blobs found in this Controller / Area
		numBlobsDetected = myContourFinder->getContours().size();
	
		if(myControllerMethod == MaxMinBlob){
			//Calc Max Mins and get the relative position to the Camera. //TODO get relative pos to the area1,2,n
			calcMainBlobLocation(_rectAreaPlayer);
			//WIP Calc some average to detect UP or Down Sudden movemnts.
			udpate_MaxMins_Recognition_UpDown_Actions(yPosBlobFloatOsc, medianNormValue); //yPosBlobFloatOsc or another value

		}
		else if(myControllerMethod == AllBlobsIn){

			//All we receive we resend it here, so there is no filter
			//For now we do NOTHING


			//BUT TODO
			//Process all this Tracking data as you want.
			//Get some relevant situations of all of them
			//like density factor, distance average, velocity average,
			//check clusters aras
			//	- then find with blob is alone in his area
			//  - then find wich blob is in a populated area
			// Etc...
		}
		else if (myControllerMethod == UpDownLeftRightBlobs) {
			//Calc the most UP , down, left, right for simplex body part detections for example
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
	float insideRectPosY = yPosBlob - _rectAreaPlayer.y;

	//cout << "_rectAreaPlayer.y = " << _rectAreaPlayer.y << endl;
	//cout << "yPosBlob = " << xPosBlob << endl;
	//cout << " _rectAreaPlayer.y - yPosBlob = insideRectPosY = " << insideRectPosY << endl;


	xPosBlobFloatOsc = ofMap(insideRectPosX, 0, _rectAreaPlayer.width, 0, 1, true);
	yPosBlobFloatOsc = ofMap(insideRectPosY, 0, _rectAreaPlayer.height, 0, 1, true);
	if (bresumeBlob_inverX) {
		xPosBlobFloatOsc = 1 - xPosBlobFloatOsc;
	}
	if (bresumeBlob_inverY) {
		yPosBlobFloatOsc = 1 - yPosBlobFloatOsc;
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
//MaxMinsAllBlob Calcs
void ControllerReconition::udpate_MaxMins_Recognition_UpDown_Actions(float _value, statsRecorder & _stats){
	
	//Udpate stats
	_stats.update(_value);
	medianResult = _stats.getAverage(numAverageFrammes);//Check yPosBlob is > medianBlobHeightValue ?¿?¿
	
	//Means Last Values are smaller than the average, so , This is DOWN DIR
	if(_stats.getLastValueNormal() - medianResult < 0){
		fUpActionBlob = medianResult - _stats.getLastValueNormal();
		fUpActionBlob_OSC = ofMap(fUpActionBlob, 0, 1, 0, 1);
	}
	else {
		fDownActionBlob = _stats.getLastValueNormal() - medianResult;
		fDownActionBlob_OSC = ofMap(fDownActionBlob, 0, 1, 0, 1);
	}
	
	//Check yPosBlob is < medianBlobHeightValue ¿?¿
}


//-----------------------------------------
void ControllerReconition::send_OSC_UPD_Data_MaxMinBlob(string nameTag) {
	
	//OSC
	if (bSendOsc_fMiddleX_fMinY_fUP_fDOWN) {

		ofxOscMessage m;
		m.clear();
		m.setAddress("/"+ nameTag);
		m.addFloatArg(xPosBlobFloatOsc);
		m.addFloatArg(yPosBlobFloatOsc);

		// sending float to be able to make more actions filtering in the client.
		//Like Intenisty of the action
		m.addFloatArg(fUpActionBlob_OSC);
		m.addFloatArg(fDownActionBlob_OSC);

		sender.sendMessage(m, false);
	}

	//UPD
	if (bSendUDP_fMiddleX_fMinY_fUP_fDOWN) {

		string message = "/"+nameTag+" ffff ";
		message = message + ofToString(xPosBlobFloatOsc, 2) + " " + ofToString(yPosBlobFloatOsc, 2) + " " + ofToString(fUpActionBlob_OSC, 2) + " " + ofToString(fDownActionBlob_OSC, 2);
		udpConnection.Send(message.c_str(), message.length());

		//cout << "message UDP = " << message << endl;
		//cout << "UPD properties GetTimeoutSend = " << udpConnection.GetTimeoutSend();

	}
}

//-----------------------------------------
void ControllerReconition::send_OSC_Data_AllInBlobs() {

	ofxCv::RectTracker& tracker = myContourFinder->getTracker();

	//TODO Decide desired data and parameters to send
	ofxOscMessage m;
	m.clear();
	m.setAddress("/GameBlobAllIn");//TODO tracking Label
	m.addIntArg(numBlobsDetected); //Add the number of Blobs detected in order to read them properly and easy

	for (int i = 0; i < numBlobsDetected > 0; i++) {
		cv::Point2f centerBlobi = myContourFinder->getCenter(i); //TODO getCentroid
																
		float resumedPosX = (myContourFinder->getCenter(i).x - rectAreaPlayer.x) / rectAreaPlayer.width; //Forced to 0..1 inside the RectArea 
		float resumedPosY = (myContourFinder->getCenter(i).y - rectAreaPlayer.y) / rectAreaPlayer.height; //Forced to 0..1 inside the RectArea 
		m.addFloatArg(resumedPosX);
		m.addFloatArg(resumedPosY);

		if(myDetectMethod == FindContournsTracking){
			//for Tracking add int ID & int TIME
			int idAux = myContourFinder->getLabel(i);
			m.addIntArg(idAux); //Sending ID Label
			int timeAux = tracker.getAge(idAux);
			m.addIntArg(timeAux); //Sending Time Tracked
		}
		sender.sendMessage(m, false);
	}

}

//-----------------------------------------
void ControllerReconition::sendOSCBlobData(){

	if (numBlobsDetected > 0) {
		if (myControllerMethod == MaxMinBlob) {
			//working at selected Areas. Finding the proper XY related and Sending to a Client the results
			//TODO Convert this into dynamic controlers
			if (idController == 1) {
				send_OSC_UPD_Data_MaxMinBlob("GameBlob");
			}
			else if (idController == 2) {
				send_OSC_UPD_Data_MaxMinBlob("GameBlob2");
			}
		}
		else if (myControllerMethod == AllBlobsIn) {

			if (myDetectMethod == FindContournsTracking || myDetectMethod == FindContourns) {
				//TODO that should work for all Areas. Now only for the area 1.
				if (idController == 1) {
					send_OSC_Data_AllInBlobs(); //IF TRACKINGMODE active this is senind 2 LABELS.ID and TIME 
				}
				//else if (idController == 2) {
				//	send_OSC_Data_AllInBlobs(); //IF TRACKINGMODE active this is senind 2 LABELS.ID and TIME 
				//}
			}
		}


	}
	else {
		//do nothing
	}
	
}

//-----------------------------------------
void ControllerReconition::draw(){
	
	ofSetColor(ofColor::white);
	
	drawGui_Controller();
	
	//drawPolylinesAreas();
	
}


//-------------------------------------------------
void ControllerReconition::drawGui_HostIP_configurable(){
	
		ImGui::Text("Sending Data to Host: ");
		ImGui::Text(ofToString(PORT,0).c_str());
		ImGui::Text(HOST.c_str());
		
		//TODO InputTextFilterCharacter
		static char buf1[16] = "127.0.0.1";
		ImGui::SameLine();
		ImGui::PushItemWidth(90);
		ImGui::InputText("Edit", buf1, 16);ImGui::SameLine();
		ImGui::Checkbox("Reset", &bResetHostIp);
		ImGui::PopItemWidth();
		
		if(bResetHostIp){
			HOST = std::string(buf1);
			cout << "Lets reset HOST IP" << endl;
		}

		ImGui::Text("-------------------------");
}

//----------------------------------------------------
void ControllerReconition::drawGui_OSCUPD_sendingData() {

	string myControlerIdText = "OSC | UPD" + ofToString(idController);
	ImGui::Text(myControlerIdText.c_str());

	/////SEND OSC / UPD

	string dataDescription1 = "SEND ffff f0(X)  f1(Y)  f2(UP)  f3(DOWN)";
	ImGui::Text(dataDescription1.c_str());

	ImGui::PushItemWidth(50);
	ImGui::Checkbox("UDP:29095", &bSendUDP_fMiddleX_fMinY_fUP_fDOWN); ImGui::SameLine();

	if (bSendUDP_fMiddleX_fMinY_fUP_fDOWN) {
		ImGui::SliderFloat("##(f0)UPD", &xPosBlobFloatOsc, 0, 1); ImGui::SameLine();
		ImGui::SliderFloat("##(f1)UPD", &yPosBlobFloatOsc, 0, 1); ImGui::SameLine();
		ImGui::VSliderFloat("##(f2)UPD", ImVec2(20, 50), &fUpActionBlob_OSC, 0, 1); ImGui::SameLine();
		ImGui::VSliderFloat("##(f3)UPD", ImVec2(20, 50), &fDownActionBlob_OSC, 0, 1);
	}
	ImGui::Separator();

	ImGui::Checkbox("OSC:12345", &bSendOsc_fMiddleX_fMinY_fUP_fDOWN); ImGui::SameLine();
	if (bSendOsc_fMiddleX_fMinY_fUP_fDOWN) {
		ImGui::SliderFloat("##(f0)OSC", &xPosBlobFloatOsc, 0, 1); ImGui::SameLine();
		ImGui::VSliderFloat("##(f1)OSC", ImVec2(20, 50), &yPosBlobFloatOsc, 0, 1); ImGui::SameLine();
		ImGui::VSliderFloat("##(f2)OSC", ImVec2(20, 50), &fUpActionBlob_OSC, 0, 1); ImGui::SameLine();
		ImGui::VSliderFloat("##(f3)OSC", ImVec2(20, 50), &fDownActionBlob_OSC, 0, 1);
	}

	ImGui::PopItemWidth();

}
//-------------------------------------------------
void ControllerReconition::drawGui_Controller(){
	//ImGui::SetNextWindowSize(ImVec2(310, 350));
	
	//cout << "Check this Out PreVious window" << endl;
	//bool wopen = true;
	string myControlerHOSTIPText = "HOST_IP Ctr l" + ofToString(idController);
	ImGui::Begin(myControlerHOSTIPText.c_str());
	//Draw and Edit OSC info
	drawGui_HostIP_configurable();
	drawGui_OSCUPD_sendingData();
	ImGui::End();

	//string myControlerIdText = "ControllerRecognition Ctrl" + ofToString(idController);
	string myControlerIdTextPostCV = "Post-CV [" + ofToString(idController, 0) + "]";
	if (ImGui::CollapsingHeader(myControlerIdTextPostCV.c_str())) {

		static int ControllerMethod_item_current = 0;//Deafult simple //TODO use load this from JSon
		const char* combo_controllerTypeStrings[] = { "MaxMinBlob", "UpDownLeftRightBlobs" , "AllBlobsIn" };
		ImGui::Combo("Controller Data Type", &ControllerMethod_item_current, combo_controllerTypeStrings, IM_ARRAYSIZE_TEMP2(combo_controllerTypeStrings));
		myControllerMethod = static_cast<ControllerMethod>(ControllerMethod_item_current);

		if (myControllerMethod == MaxMinBlob) {

			drawGui_ResumedBlob_MaxMinBlobs();

			//TODO Add This to plot gui
			//medianBlobHeightValue.draw(500 , 500, sensorWidth, 100, 100, "medianBlobHeightValue", true, 150);

			drawResumedBlob_MaxMinBlobs();

		}
		else if (myControllerMethod == UpDownLeftRightBlobs) {
			//TODO
			//At least 4 elements with X and Y 
		}
		else if (myControllerMethod == AllBlobsIn) {
			//TODO
			//Receiving all blobs, then sending them
		}
	}

		
	
	

}

//-------------------------------------------------
void ControllerReconition::drawGui_ResumedBlob_MaxMinBlobs() {

	ImGui::Text("MaxMinBlob: Read all Blob contourns &\nCalc a resulting pt -> (MaxX, MinY),...");
	ImGui::Separator();
	//Slider to select the -> RecognitionMethod _myComputeBlobType ... From 0 .. 2? 

	ImGui::Text("-+-+-+-+-+-+-+-+");
	ImGui::Separator();
		
	string myControlerIdText_MAXMINSetup = "MaxMins Setup " + ofToString(idController);
	ImGui::Text(myControlerIdText_MAXMINSetup.c_str());

	//if(ImGui::CollapsingHeader(myControlerIdText_MAXMINSetup.c_str())){

		ImGui::PushItemWidth(160);

		//// X Y 
		const char* combo_resumedBlob_X[] = { "bresumeBlob_maxX", "bresumeBlob_minX", "bresumeBlob_middleX" };
		const char* combo_resumedBlob_Y[] = { "bresumeBlob_maxY", "bresumeBlob_minY", "bresumeBlob_middleY" };

		ImGui::Combo("MaxMin X type", &item_resumedBlob_X, combo_resumedBlob_X, IM_ARRAYSIZE_TEMP2(combo_resumedBlob_X));
		ImGui::Combo("MaxMin Y type", &item_resumedBlob_Y, combo_resumedBlob_Y, IM_ARRAYSIZE_TEMP2(combo_resumedBlob_Y));

		ImGui::PopItemWidth();

		ImGui::PushItemWidth(50);

		ImGui::Text("Invert:"); ImGui::SameLine();
		ImGui::Checkbox("X", &bresumeBlob_inverX); ImGui::SameLine();
		ImGui::Checkbox("Y", &bresumeBlob_inverY);

		ImGui::Separator();
		//// UP DOWN

		ImGui::Text("AVG of: Y"); ImGui::SameLine(); //TODO COMBO SELECTABLE VAR...x, y, ... mode to study about?
		if (ImGui::SliderInt("#AVG", &numAverageFrammes, 0, 100)) {
			medianNormValue.reset();
			medianNormValue.setup(numAverageFrammes, 0, 1);
		}
		ImGui::SameLine();

		ImGui::SameLine();
		ImGui::Text(" = "); ImGui::SameLine();
		ImGui::SliderFloat("##medianResult", &medianResult, 0, 1);

		ImGui::PopItemWidth();
		ImGui::Separator();

	//}

	ImGui::Text("-+-+-+-+-+-+-+-+");
	ImGui::Separator();
}


void ControllerReconition::drawResumedBlob_MaxMinBlobs(/*int transX ,int transY, int winW, int winH*/){
	//TODO add Translate X, translate Y , Point Position ( 0 .. 1 ), WidthWindow, HeightWindow
 
	ofPushMatrix();
	ofTranslate(sensorWidth*sensorScale, SensorManager::getInstance()->marginDraw);
	
	ofEnableAlphaBlending();
	
	//Draw Vector Interaction for Jostick Mode
	ofPushStyle();
	ofColor myLineColor;
	if(idController == 1)myLineColor = ofColor::green;
	else if(idController == 2)myLineColor = ofColor::blueSteel;
	else myLineColor = ofColor::red; //Error color line not definet yet

	ofSetColor(myLineColor.r, myLineColor.g, myLineColor.b, 220);

	ofNoFill();
	ofDrawRectangle(ofRectangle(
		rectAreaPlayer.x*sensorScale,//x
		rectAreaPlayer.y*sensorScale+ sensorHeight*(idController - 1)*sensorScale,//y
		rectAreaPlayer.width*sensorScale,//w
		rectAreaPlayer.height*sensorScale)//h
	);

	ofFill();
	ofDrawCircle(
		xPosBlob*sensorScale, //x
		yPosBlob*sensorScale + sensorHeight*(idController-1)*sensorScale, //y
		10 * sensorScale);//size

	ofSetLineWidth(2);
	ofSetColor(myLineColor.r, myLineColor.g, myLineColor.b, 150);
	ofDrawLine(
		rectAreaPlayer.getCenter().x*sensorScale,//x1
		rectAreaPlayer.getCenter().y*sensorScale + sensorHeight*(idController - 1)*sensorScale, //y1
		xPosBlob*sensorScale, //x2
		yPosBlob*sensorScale + sensorHeight*(idController - 1)*sensorScale); //y2
	
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
