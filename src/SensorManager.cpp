//
//  sensorManagement.cpp
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//

#include "SensorManager.h"

// SINGLETON initalizations
bool SensorManager::instanceFlag = false;
SensorManager* SensorManager::single = NULL;

//----------------------------------------------

SensorManager* SensorManager::getInstance()
{
	if(! instanceFlag)
	{
		single = new SensorManager();
		instanceFlag = true;
		return single;
	}else{
		return single;
	}
}

//----------------------------------------------
SensorManager::SensorManager()
{
	
}
//----------------------------------------------
SensorManager::~SensorManager()
{}

//Getters
//-----------------------------------------
sensorType SensorManager::getSensorType(){
	return typeSensor;
}
//-----------------------------------------
sensorMode SensorManager::getSensorMode(){
	return modeSensor;
}

//Setters
//-----------------------------------------
void SensorManager::setSensorType(sensorType _type){
	typeSensor =_type;
}
//-----------------------------------------
void  SensorManager::setSensorMode(sensorMode _mode){
	modeSensor = _mode;
}


//-----------------------------------------
void SensorManager::setup(sensorType _sensorType, sensorMode _sensorMode){
	
	typeSensor = _sensorType;
	modeSensor = _sensorMode;
	
	///////////////////////
	//Sensor Configurations
	bool bSensorReady = false;
	
	if (typeSensor == kinectSensor) {
#ifdef USE_SENSOR_KINECT
		bSensorReady = setupKinectSensor();
#endif
	}else if (typeSensor == cameraSensor){
		bSensorReady = setupCameraSensor();
		
	}else if(typeSensor == externalSickSensor){
		bSensorReady = setupExternalSickSensor();
	}
	
	if(bSensorReady == false){
		cout << "//*///*/*/*/*/*/*/* Error sensor setup **/*/*/*////**///" << endl;
		ofExit(0);
	}
	
	ofRegisterKeyEvents(this);
	
}


//-----------------------------------------
void SensorManager::update(){
	
	bNewSensorFrame = false;
	
	if(typeSensor == kinectSensor){

#ifdef USE_SENSOR_KINECT

		kinect.update();
		
		// there is a new frame and we are connected
		if(kinect.isFrameNew()) {
			
			// load grayscale depth image from the kinect source
			computerVisionImage.setFromPixels(kinect.getDepthPixels());
			
			//we do two thresholds - one for the far plane and one for the near plane
			//we do it ourselves - show people how they can work with the pixels
			ofPixels & pix = computerVisionImage.getPixels();
			int numPixels = pix.size();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
			
			//Set pixels back to main cv image
			computerVisionImage.setFromPixels(pix);
		}

		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		//contourFinder.findContours(grayImage, minSizeBlob, maxSizeBlob, numBlobs, false);
		contourFinder.findContours(computerVisionImage);
		bNewSensorFrame = true;
#endif

	}
	else if (typeSensor == cameraSensor){
		
		if(modeSensor == simulationMode){
			videoPlayerCam.update();
			bNewSensorFrame = videoPlayerCam.isFrameNew();
		}
		else if(modeSensor == realTimeMode){
			cam.update();
			bNewSensorFrame = cam.isFrameNew();
		}
		
		if(bLearnBackground){
			if(bresetBackground) {
				background.reset();
				bresetBackground = false;
			}
		}
		
		if(bNewSensorFrame) {
			
			//blur(movie, 10);//TODO This! easy and fast. Add slider options
			
			//TODO FIX THIS bLearnBackground to not apply really a Learning background if its not active.
			if(bLearnBackground){
				

				
				background.setLearningTime(learningTime);
				background.setThresholdValue(thresholdValue);
				//Camera Image to Gray
				if(modeSensor == simulationMode) background.update(videoPlayerCam, computerVisionImage);
				else if(modeSensor == realTimeMode) background.update(cam, computerVisionImage);
				computerVisionImage.update();
				
				contourFinder.findContours(computerVisionImage);
			}
			else if(bSimpleBackgroundSubstraction){
				
				//Camera Image to Gray
				if(modeSensor == simulationMode) ofxCv::convertColor(videoPlayerCam, computerVisionImage, CV_RGB2GRAY);
				else if(modeSensor == realTimeMode) ofxCv::convertColor(cam, computerVisionImage, CV_RGB2GRAY);
				
				
				computerVisionImage.update();
				
				//Save Background Frame
				if(bresetBackground){
					backGroundCam = computerVisionImage;
					backGroundCam.update();
					bresetBackground = false;
				}
				
				
				//then background substraction //TODO check diferent methods
				ofxCv::absdiff(computerVisionImage, backGroundCam, diffCam);
				
				//Apply invert Threshold
				if(bInvertContourFinderThreshold)contourFinder.setInvert(true);
				else contourFinder.setInvert(false);
				
				if(bContourFinderThreshold){
					//FindContours Threshold
					contourFinder.setThreshold(thresholdValue);
					contourFinder.findContours(diffCam);
				}else if(bAutoThreshold){
					//Automatic Thresholding
					ofxCv::autothreshold(diffCam);
					contourFinder.findContours(diffCam);
				}
				else{
					//Regular Threshold
					ofxCv::threshold(diffCam, thresholdValue);
					contourFinder.findContours(diffCam);
				}
				
				diffCam.update();
				
			}
			else{ //ContourFinder Methods
				
				//Update Camera colors
				if(modeSensor == simulationMode){
					computerVisionImage.setFromPixels(videoPlayerCam.getPixels(), sensorWidth, sensorHeight, OF_IMAGE_COLOR);
				}
				else if(modeSensor == realTimeMode){
					computerVisionImage.setFromPixels(cam.getPixels(), sensorWidth, sensorHeight, OF_IMAGE_COLOR);
				}
				
				computerVisionImage.update();
				
				//Threshold
				
				if(bContourFinderThreshold){
					
					contourFinder.setAutoThreshold(true);
					
					if(bContourFinderColorThreshold){
						//FindContours Threshold
						contourFinder.setUseTargetColor(true);
						contourFinder.setTargetColor(colorTargetContourFinder);
						//TODO ADD Color Picker From Camera.
					}
				
				}else{
					//Default Threshold Method
					/*
					 ContourFinder::ContourFinder()
					 :autoThreshold(true)
					 ,invert(false)
					 ,simplify(true)
					 ,thresholdValue(128.)
					 ,useTargetColor(false)
					 ,contourFindingMode(CV_RETR_EXTERNAL)
					 ,sortBySize(false) {
						resetMinArea();
						resetMaxArea();
					 }*/
					
					contourFinder.setAutoThreshold(true);
					contourFinder.setInvert(false);
					contourFinder.setUseTargetColor(false);
					contourFinder.setThreshold(thresholdValue);
				}
				
				
				//Apply invert Threshold
				if(bInvertContourFinderThreshold)contourFinder.setInvert(true);
				else contourFinder.setInvert(false);
				
				
				//Apply Configured Thresdhold
				contourFinder.setThreshold(thresholdValue);
				
				//Find Countours
				contourFinder.findContours(computerVisionImage);

			}

		}
		
	}
	else if (typeSensor == externalSickSensor){
		
		//This updates osc data that contains all Blobs
		bNewSensorFrame = updateExternalSickSensor();

	}
	
	
	
	//Some extra Operation at the end of sensor Dection ?
	//use  isNewSensorFrame for Lattely actions in other parts of code
	if(bNewSensorFrame){
		
	}

}



//-----------------------------------------
bool SensorManager::isNewSensorFrame(){
	return bNewSensorFrame;
}

//-----------------------------------------
void SensorManager::draw(){
	
	if(typeSensor == kinectSensor){
		
#ifdef USE_SENSOR_KINECT

		ofSetColor(255, 255, 255);
		

		// draw from the live kinect
		kinect.drawDepth(marginDraw, marginDraw, kinect.width*sensorDrawScale, kinect.height*sensorDrawScale);
		kinect.draw(2*kinect.width*sensorDrawScale, marginDraw, kinect.width*sensorDrawScale, kinect.height*sensorDrawScale);
			
		computerVisionImage.draw(marginDraw, sensorWidth*sensorDrawScale, kinect.width*sensorDrawScale, kinect.height*sensorDrawScale);
		
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(marginDraw, sensorWidth*sensorDrawScale);
		ofScale(sensorDrawScale, sensorDrawScale);
		contourFinder.draw();
		ofPopMatrix();

		
		// draw instructions
		ofSetColor(255, 255, 255);
		stringstream reportStream;
		
		if(kinect.hasAccelControl()) {
			reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
			<< ofToString(kinect.getMksAccel().y, 2) << " / "
			<< ofToString(kinect.getMksAccel().z, 2) << endl;
		} else {
			reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
			<< "motor / led / accel controls are not currently supported" << endl << endl;
		}
		
		reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
		//<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
		<< "set near threshold " << nearThreshold << " (press: + -)" << endl
		<< "set far threshold " << farThreshold
		//<< " (press: < >) num blobs found " << contourFinder.nBlobs
		//<< ", fps: " << ofGetFrameRate() << endl
		<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
		
		if(kinect.hasCamTiltControl()) {
			reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
			<< "press 1-5 & 0 to change the led mode" << endl;
		}
		
		ofDrawBitmapString(reportStream.str(), 20, 652);
#endif

	}
	else if (typeSensor == cameraSensor){
		
		if(computerVisionImage.isAllocated()) {
			
			ofSetColor(255, 255, 255);
			
			//--------------------------------------
			//Draw Raw Sensor images
			if(modeSensor == simulationMode) computerVisionImage.draw(marginDraw, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
			else if(modeSensor == realTimeMode) cam.draw(marginDraw, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
			
			
			if(bLearnBackground){
				computerVisionImage.draw(sensorWidth*sensorDrawScale, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
				ofxCv::toOf(background.getBackground(), backGroundCam);
				backGroundCam.draw(2*sensorWidth*sensorDrawScale, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
				
			}
			else if(bSimpleBackgroundSubstraction){
				diffCam.draw(sensorWidth*sensorDrawScale, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
				backGroundCam.draw(2*sensorWidth*sensorDrawScale, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
			}
			else{
				computerVisionImage.draw(sensorWidth*sensorDrawScale, marginDraw, sensorWidth*sensorDrawScale, sensorHeight*sensorDrawScale);
			}
			
			
			//---------------------------------------
			//Kyle Mcdonnal - Blob Tracker Visualization
			if(bTrackgingActive){
				
				//TODO To acces this from outside may be neceseary to clean
				ofxCv::RectTracker& tracker = contourFinder.getTracker();
			
			
				if(showLabels) {
					
					for(int i = 0; i < contourFinder.size(); i++) {
						ofPoint center = ofxCv::toOf(contourFinder.getCentroid(i));
						ofPushMatrix();
						ofTranslate(sensorWidth*sensorDrawScale, marginDraw);
						ofTranslate(center.x*sensorDrawScale, center.y*sensorDrawScale);
						int label = contourFinder.getLabel(i);
						string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
						ofDrawBitmapString(msg, 0, 0);
						ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
						ofDrawLine(0, 0, velocity.x*sensorDrawScale, velocity.y*sensorDrawScale);
						ofPopMatrix();
						
					}
					
				}
				else {
					
					ofPushMatrix();
					ofTranslate(sensorWidth*sensorDrawScale, marginDraw);
					ofScale(sensorDrawScale, sensorDrawScale);
					
					for(int i = 0; i < contourFinder.size(); i++) {
						unsigned int label = contourFinder.getLabel(i);
						// only draw a line if this is not a new label
						if(tracker.existsPrevious(label)) {
							// use the label to pick a random color
							ofSeedRandom(label << 24);
							ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
							// get the tracked object (cv::Rect) at current and previous position
							const cv::Rect& previous = tracker.getPrevious(label);
							const cv::Rect& current = tracker.getCurrent(label);
							// get the centers of the rectangles
							ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
							ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
							ofDrawLine(previousPosition, currentPosition);
						}
					}
					
					ofPopMatrix();
				}
				
			
				
				// this chunk of code visualizes the creation and destruction of labels
				const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
				const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
				const vector<unsigned int>& newLabels = tracker.getNewLabels();
				const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
				
				ofSetColor(ofxCv::cyanPrint);
				for(int i = 0; i < currentLabels.size(); i++) {
					int j = currentLabels[i];
					ofDrawLine(j, 0, j, 4);
				}
				ofSetColor(ofxCv::magentaPrint);
				for(int i = 0; i < previousLabels.size(); i++) {
					int j = previousLabels[i];
					ofDrawLine(j, 4, j, 8);
				}
				ofSetColor(ofxCv::yellowPrint);
				for(int i = 0; i < newLabels.size(); i++) {
					int j = newLabels[i];
					ofDrawLine(j, 8, j, 12);
				}
				ofSetColor(ofColor::white);
				for(int i = 0; i < deadLabels.size(); i++) {
					int j = deadLabels[i];
					ofDrawLine(j, 12, j, 16);
				}
			
				
			}

		}
		
		ofSetColor(255, 0, 0);
		
		ofPushMatrix();
		ofTranslate(sensorWidth*sensorDrawScale, marginDraw); //TODO change sensorDrawScale as int sensorScale var
		ofScale(sensorDrawScale, sensorDrawScale);
		contourFinder.draw();
		ofPopMatrix();

	
	}
	else if(typeSensor == externalSickSensor){
		
		ofSetColor(ofColor::grey);
		ofDrawRectangle(0, 0, sensorWidth, sensorHeight);
		
		ofSetColor(ofColor::greenYellow);
		
		for (int i = 0; i < sickBlobs.size() ; i++){
			ofDrawBitmapString("id: " + ofToString(sickBlobs[i].id, 0), sickBlobs[i].pos.x*sensorWidth*sensorDrawScale, sickBlobs[i].pos.y*sensorWidth*sensorDrawScale + 7);
			ofDrawCircle(sickBlobs[i].pos.x*sensorWidth*sensorDrawScale, sickBlobs[i].pos.y*sensorWidth*sensorDrawScale, 10); //Rescaling
		}
	}

	ofSetColor(255, 255, 255);
	//Draw some Sensor Option
	bool isSensorWindow = true;
	if (isSensorWindow) {
		this->drawGuiSensorOptions(&isSensorWindow);
	}


	
}

//---------------------------------------------------------------------
bool SensorManager::updateVideoFolderComboSelections(string _videosPaths) {
	
	videosAvailable.clear();
	
	svideosDirPath = _videosPaths; //TODO Create State Video or Sensor Ready or not Ready
	
	myVideosDir.listDir(svideosDirPath);
	if( selectedMovieIndex < myVideosDir.size() ){

		smovieFileName = myVideosDir.getPath(selectedMovieIndex); //Get the one preselected
		
	}//else{
		//cout << "Error No Video in the Default Video Folder = " << svideosDirPath << endl;
		//selectedMovieIndex = -1;
	//}
	
	
	for (int i = 0; i < myVideosDir.size(); i++) {
		videosAvailable.push_back(myVideosDir.getFile(i).getFileName());
	}
	
	//smoviePath = std::string(svideosDirPath.c_str());
	videoPlayerCam.stop();
	videoPlayerCam.close();
	bool bLoaded = videoPlayerCam.load(svideosDirPath+videosAvailable[selectedMovieIndex]);
	if(bLoaded) videoPlayerCam.play();
	if(bLoaded){
		if(sensorWidth != videoPlayerCam.getWidth()){
			sensorWidth = videoPlayerCam.getWidth();
			sensorHeight = videoPlayerCam.getHeight();
		}
		//TODO reset other important Vars?
		
	}
	
	return bLoaded;
}

//-----------------------------------------
void SensorManager::resetSimpleSensorCamera() {
	cam.close();
	cam.setDeviceID(selectedCameraIndex);
	cam.setup(640, 480);//TODO editable format
}

//-----------------------------------------
void SensorManager::drawGuiSensorOptions(bool* opened){
	
	string textBlobsFound = "#blobs = "+ofToString(contourFinder.size(), 0);
	ImGui::Text(textBlobsFound.c_str());
	
	
	string sensorTextType = "Not configured Yet";
	if(typeSensor == kinectSensor){

#ifdef USE_SENSOR_KINECT

		sensorTextType = "Kinect 1";

		ImGui::Text(sensorTextType.c_str());
		//ImGui::Checkbox("bThreshWithOpenCV", &bThreshWithOpenCV);
		ImGui::SliderInt("nearThreshold", &nearThreshold, 0, 255);
		ImGui::SliderInt("farThreshold", &farThreshold, 0, 255);
		ImGui::Separator();
		//TODO missing this property in ofxCv
		//ImGui::SliderInt("numBlobs ", &numBlobs, 1, 20);
		ImGui::SliderInt("accuracyMaxSizeBlob", &maxBlobsAccuracyMaxValue, 0, sensorWidth *sensorHeight*sensorDrawScale);
		
		if(ImGui::SliderInt("minBlobs ", &minSizeBlob, 5, maxBlobsAccuracyMaxValue)){
			contourFinder.setMinAreaRadius(minSizeBlob);
		}
		
		
		if(ImGui::SliderInt("maxBlobs ", &maxSizeBlob, marginDraw, maxBlobsAccuracyMaxValue)){
			contourFinder.setMaxAreaRadius(maxSizeBlob);
		}
		
#endif
		//ImGui::PopItemWidth();
	}else if (typeSensor == cameraSensor){
		sensorTextType = "OF Camera";
		ImGui::Text(sensorTextType.c_str());
		
		if (modeSensor == realTimeMode) {
		

			ImGui::PushItemWidth(200);
			static int myGuiCameraIndex = 0;
			if (ImGui::InputInt("Index Camera", &myGuiCameraIndex, 1)) {//Step one by one
				if (myGuiCameraIndex > -1 && myGuiCameraIndex < cam.listDevices().size()) {
					selectedCameraIndex = myGuiCameraIndex;
				}
				else {
					myGuiCameraIndex = selectedCameraIndex;
				}
			}
			ImGui::PopItemWidth();

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
/* 
//WIP. This require public access to VI (Directshow)
			static bool bCameraComposite = false;
			if (ImGui::Checkbox("SET COMPOSITE", &bCameraComposite)) {
				if (bCameraComposite) {
					shared_ptr<ofDirectShowGrabber> auxGrabber = shared_ptr <OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE);
					auxGrabber->VI.setupDevice(selectedCameraIndex, VI_COMPOSITE);
					cam.setGrabber(auxGrabber);
				}
				else {
					resetSimpleSensorCamera();
				}
				
			}
*/
#endif
			if (ImGui::Button("Reset Camera", ImVec2(150, 20))) {
				resetSimpleSensorCamera();
			}

			if (ImGui::Button("Settings Camera", ImVec2(150, 30))) {
				cam.videoSettings();
			}


		
		}
		else if(modeSensor == simulationMode){
			
			//TODO InputTextFilterCharacter
			static char cmoviePath[60] = "videos/default/"; //This will load fisrt Video available at the this Folder
			ImGui::PushItemWidth(200);
			ImGui::InputText("Videos Path", cmoviePath, IM_ARRAYSIZE(cmoviePath));
			static int myGuiVideoIndex = 0;
			if(ImGui::InputInt("Index video", &myGuiVideoIndex, 1)){//Step one by one
				if(myGuiVideoIndex > -1 && myGuiVideoIndex < videosAvailable.size()){
					selectedMovieIndex = myGuiVideoIndex;
				}else{
					myGuiVideoIndex = selectedMovieIndex;
				}
			}
			
			ImGui::SameLine();
			ImGui::PopItemWidth();
			ImGui::Checkbox("Reset Path", &bResetMoviePath);
			
			
			if(bResetMoviePath && selectedMovieIndex != -1){
				updateVideoFolderComboSelections(cmoviePath);
				
				bResetMoviePath = false;
				cout << "Reset Movie to " << svideosDirPath+videosAvailable[selectedMovieIndex] << endl;
			}
			
			videoPlayerCam_pos = videoPlayerCam.getPosition();
			if(ImGui::SliderFloat("VideoCam Position", &videoPlayerCam_pos, 0, 1)){
				videoPlayerCam.setPosition(videoPlayerCam_pos);
			}
		}
		
		ImGui::Checkbox("Do Learning Background", &bLearnBackground);
		
		if(ImGui::Button("Reset Background")){
			bresetBackground = true;
		}
		
		if(bLearnBackground){
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("Learning Time", &learningTime, 0, 255);
			ImGui::PopItemWidth();
		}
		
		ImGui::Checkbox("Background Substraction", &bSimpleBackgroundSubstraction);
		
		if(bSimpleBackgroundSubstraction){
			ImGui::SameLine();
			ImGui::Checkbox("Auto Threshold", &bAutoThreshold);
		}
		
		ImGui::Checkbox("ContoursFinder Options", &bContourFinderThreshold);
		ImGui::Checkbox("Invert Threshold ContoursFinder", &bInvertContourFinderThreshold);
	
		if(bContourFinderThreshold){
			
			ImGui::Checkbox("ContoursFinder Color Target", &bContourFinderColorThreshold);
			if(bContourFinderColorThreshold){
				ImVec4 colorTargetVec = colorTargetContourFinder;
				ImGui::ColorEdit3("Color Target ContourFinder##ContourFinder", (float*)& colorTargetVec);
				
				if(bContourFinderColorThreshold){
					colorTargetContourFinder.r = colorTargetVec.x*255;
					colorTargetContourFinder.g = colorTargetVec.y*255;
					colorTargetContourFinder.b = colorTargetVec.z*255;
				}
			}
				
		}

		ImGui::SliderFloat("Threshold Value", &thresholdValue, 0, 255);
		
		ImGui::SliderInt("accuracyMaxSizeBlob", &maxBlobsAccuracyMaxValue, 0, sensorWidth *sensorHeight*sensorDrawScale);
		
		if(ImGui::SliderInt("min Area Blob", &minSizeBlob, marginDraw, maxBlobsAccuracyMaxValue)){
			contourFinder.setMinAreaRadius(minSizeBlob);
		}
		if(ImGui::SliderInt("max Area Blob", &maxSizeBlob, marginDraw, maxBlobsAccuracyMaxValue)){
			contourFinder.setMaxAreaRadius(maxSizeBlob);
		}
		
		ImGui::Separator();
		
		ImGui::Checkbox("Activate Tracking", &bTrackgingActive);
		
		if(bTrackgingActive){
			ImGui::SliderInt("Max Persistance", &maxPersistenceTracking, 5, 100);
			ImGui::SliderInt("Max Distance", &maxDistanceTracking, 10, 100);
			ImGui::Checkbox("Show Labels", &showLabels);
		}
		
	}
	else if(typeSensor == externalSickSensor){
		string auxmessage = "Sick received in OSC at PORT " + ofToString(PortRecvExt, 0);
		ImGui::Text(auxmessage.c_str());
	}
	else{
		cout << "SensorManager::Error Set Sensor Mode Type" << endl;
	}

}


//-----------------------------------------
int SensorManager::getWidth(){
	return sensorWidth;
}

//-----------------------------------------
int SensorManager::getHeight(){
	return sensorHeight;
}

//-----------------------------------------
void SensorManager::exit(){
	
	if(typeSensor == kinectSensor){

#ifdef USE_SENSOR_KINECT
		kinect.setCameraTiltAngle(0); // zero the tilt on exit
		kinect.close();
#endif
		
	}
	else if(typeSensor == cameraSensor){
		//I guess this is not really necesary. They do it internally
		//videoPlayerCam.close();
		//cam.close();
	}
}

//------------------------------------------
void SensorManager::keyReleased(ofKeyEventArgs & args){}
void SensorManager::keyPressed(ofKeyEventArgs & args){
	
	if(typeSensor == kinectSensor){

#ifdef USE_SENSOR_KINECT

		switch (args.key) {
			case ' ':
				//bThreshWithOpenCV = !bThreshWithOpenCV;
				break;
				
			case '>':
			case '.':
				farThreshold ++;
				if (farThreshold > 255) farThreshold = 255;
				break;
				
			case '<':
			case ',':
				farThreshold --;
				if (farThreshold < 0) farThreshold = 0;
				break;
				
			case '+':
			case '=':
				nearThreshold ++;
				if (nearThreshold > 255) nearThreshold = 255;
				break;
				
			case '-':
				nearThreshold --;
				if (nearThreshold < 0) nearThreshold = 0;
				break;
				
			case 'w':
				kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
				break;
				
			case 'o':
				kinect.setCameraTiltAngle(angle); // go back to prev tilt
				kinect.open();
				break;
				
			case 'c':
				kinect.setCameraTiltAngle(0); // zero the tilt
				kinect.close();
				break;
				
			case '1':
				kinect.setLed(ofxKinect::LED_GREEN);
				break;
				
			case '2':
				kinect.setLed(ofxKinect::LED_YELLOW);
				break;
				
			case '3':
				kinect.setLed(ofxKinect::LED_RED);
				break;
				
			case '4':
				kinect.setLed(ofxKinect::LED_BLINK_GREEN);
				break;
				
			case '5':
				kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
				break;
				
			case '0':
				kinect.setLed(ofxKinect::LED_OFF);
				break;
				
			case OF_KEY_UP:
				angle++;
				if(angle>30) angle=30;
				kinect.setCameraTiltAngle(angle);
				break;
				
			case OF_KEY_DOWN:
				angle--;
				if(angle<-30) angle=-30;
				kinect.setCameraTiltAngle(angle);
				break;
		}
#endif
	}
}

//----------------------------------------
void SensorManager::setTrackingMode(bool _status){
	bTrackgingActive = _status;
}


//-----------------------------------------
bool SensorManager::setupCameraSensor(){
	
	bool bConnected = false;
	
	if(modeSensor == realTimeMode){
		
		cam.setVerbose(true);
		cam.listDevices();
		cam.setDeviceID(selectedCameraIndex);
		cam.setup(640, 480);
		
		computerVisionImage.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_GRAYSCALE);

		sensorWidth = cam.getWidth();
		sensorHeight = cam.getHeight();
		
		if(cam.isInitialized()){
			bConnected = true;
		}
		
	}else if(modeSensor == simulationMode){
		
#ifdef TARGET_WIN32
		svideosDirPath = "videos/default/";
#else
		svideosDirPath = "videos/default/";
#endif
		
		selectedMovieIndex = 0; //Default Index
		bConnected = updateVideoFolderComboSelections(svideosDirPath);

	}
	
	
	//filter minSizeBlob, maxSizeBlob, numBlobs
	contourFinder.setMinAreaRadius(minSizeBlob);
	contourFinder.setMaxAreaRadius(maxSizeBlob);
	contourFinder.setThreshold(numBlobs);
	
	///Tracker
	if(bTrackgingActive){
		// wait for half a frame before forgetting something
		contourFinder.getTracker().setPersistence(maxPersistenceTracking);
		// an object can move up to 32 pixels per frame
		contourFinder.getTracker().setMaximumDistance(maxDistanceTracking);
		showLabels = true;
	}
	
	return bConnected;
}


//-----------------------------------------
bool SensorManager::setupKinectSensor(){
	
	bool bConnected = false;

#ifdef USE_SENSOR_KINECT

	
	// enable depth->video image calibration
	kinect.setRegistration(true);
	
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
		
		bConnected = true;
	}
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	//Related to ComputerVision
	nearThreshold = 230;
	farThreshold = 70;
	
	
	//Computer Vision Stuff
	//Main Image used to Cv
	computerVisionImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
	//filter minSizeBlob, maxSizeBlob, numBlobs
	contourFinder.setMinAreaRadius(minSizeBlob);
	contourFinder.setMaxAreaRadius(maxSizeBlob);
	contourFinder.setThreshold(numBlobs);
	
	///////////////////////////////////
	//General SensorData for others
	sensorWidth = kinect.width;
	sensorHeight = kinect.height;

#endif	
	return bConnected;


}


//-----------------------------------------
bool SensorManager::setupExternalSickSensor(){
		receiverExt.setup(PortRecvExt);
		sensorWidth = 640; //Fake Dims
		sensorHeight = 480;//
		//We supouse this is allways right? But port could be ocupied... //TODO check this case
	return true;
	
}

//-----------------------------------------
bool SensorManager::updateExternalSickSensor(){
	
	int counterBlobsId;
	bool hasReceivedSomeThing = false;
	
	if(receiverExt.hasWaitingMessages() > 0){
		
		counterBlobsId = 0;//Manual counter
		sickBlobs.clear();//Will Work? //Check in wich frame was sent to avoid delays?
		//cout << "SensorManager::update OSC => Checking how long take this Timer = " << ofToString(ofGetElapsedTimeMillis(),0) << endl;
	}
	
	// check for waiting messages
	while(receiverExt.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiverExt.getNextMessage(m);
		
		//get all sick blobs
		if(m.getAddress() == "/SickBlobs"){
			
			sickData auxSickData;
			
			//int auxId = m.getArgAsFloat(0); //In future
			int auxId = counterBlobsId;
			auxSickData.id = auxId;
			
			ofPoint auxPos;
			auxPos.x = m.getArgAsFloat(0); //x
			auxPos.y = m.getArgAsFloat(1); //y
			auxSickData.pos = auxPos;
			
			sickBlobs.push_back(auxSickData);
			
			//manual counter
			counterBlobsId++;
			
			hasReceivedSomeThing = true;
		}
		
		
	}
}


