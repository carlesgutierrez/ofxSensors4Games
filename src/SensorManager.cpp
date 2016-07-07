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
{}
//----------------------------------------------
SensorManager::~SensorManager()
{}

//-----------------------------------------
void SensorManager::setup(sensorType _sensorType){
	
	sensorModel = _sensorType;
	
	///////////////////////
	//Sensor Configurations
	bool bSensorReady = false;
	
	if (sensorModel == kinectSensor) {
		bSensorReady = setupKinectSensor();
	}
	
	else if (sensorModel == cameraSensor){
		bSensorReady = setupCameraSensor();
		
	}
	
	if(bSensorReady == false){
		cout << "//*///*/*/*/*/*/*/* Error sensor setup **/*/*/*////**///" << endl;
		ofExit(0);
	}
	
}


//-----------------------------------------
void SensorManager::update(){
	
	bNewSensorFrame = false;
	
	if(sensorModel == kinectSensor){
		
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
		
	}
	else if (sensorModel == cameraSensor){
		cam.update();
		
		if(bLearnBackground){
			if(bresetBackground) {
				background.reset();
				bresetBackground = false;
			}
		}
		if(cam.isFrameNew()) {
			
			//blur(movie, 10);//TODO This! easy and fast. Add slider options
			
			//TODO FIX THIS bLearnBackground to not apply really a Learning background if its not active.
			if(bLearnBackground){
				background.setLearningTime(learningTime);
				background.setThresholdValue(thresholdValue);
				background.update(cam, computerVisionImage);
				computerVisionImage.update();
				
				contourFinder.findContours(computerVisionImage);
			}
			else if(bSimpleBackgroundSubstraction){
				
				//Camera Image to Gray
				ofxCv::convertColor(cam, computerVisionImage, CV_RGB2GRAY);
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
				computerVisionImage.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);
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

			
			
			bNewSensorFrame = true;
		}
		
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
	
	if(sensorModel == kinectSensor){
		
		ofSetColor(255, 255, 255);
		

		// draw from the live kinect
		kinect.drawDepth(10, 10, kinect.width*0.5, kinect.height*0.5);
		kinect.draw(420, 10, kinect.width*0.5, kinect.height*0.5);
			
		computerVisionImage.draw(10, 320, kinect.width*0.5, kinect.height*0.5);
		
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(10, 320);
		ofScale(0.5, 0.5);
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
	}
	else if (sensorModel == cameraSensor){
		
		if(computerVisionImage.isAllocated()) {
			
			ofSetColor(255, 255, 255);
			
			//--------------------------------------
			//Draw CV images
			
			//movie.draw(0, 0); //TODO VIDEO OPTION
			cam.draw(10, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
			
			if(bLearnBackground){
				computerVisionImage.draw(cam.getWidth()*0.5, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
				ofxCv::toOf(background.getBackground(), backGroundCam);
				backGroundCam.draw(2*cam.getWidth()*0.5, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
				
			}
			else if(bSimpleBackgroundSubstraction){
				diffCam.draw(cam.getWidth()*0.5, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
				backGroundCam.draw(2*cam.getWidth()*0.5, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
			}
			else{
				computerVisionImage.draw(cam.getWidth()*0.5, 10, cam.getWidth()*0.5, cam.getHeight()*0.5);
			}
			
			
			//---------------------------------------
			//Kyle Blob Tracker Visualization
			if(bTrackgingActive){
				
				//TODO To acces this from outside may be neceseary to clean
				ofxCv::RectTracker& tracker = contourFinder.getTracker();
			
			
				if(showLabels) {
					
					for(int i = 0; i < contourFinder.size(); i++) {
						ofPoint center = ofxCv::toOf(contourFinder.getCentroid(i));
						ofPushMatrix();
						ofTranslate(320, 10);
						ofTranslate(center.x*0.5, center.y*0.5);
						int label = contourFinder.getLabel(i);
						string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
						ofDrawBitmapString(msg, 0, 0);
						ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
						ofDrawLine(0, 0, velocity.x*0.5, velocity.y*0.5);
						ofPopMatrix();
						
					}
					
				}
				else {
					
					ofPushMatrix();
					ofTranslate(320, 10);
					ofScale(0.5, 0.5);
					
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
		ofTranslate(cam.getWidth()*0.5, 10);
		ofScale(0.5, 0.5);
		contourFinder.draw();
		ofPopMatrix();

	
	}

	ofSetColor(255, 255, 255);
	//Draw some Sensor Option
	bool isSensorWindow = true;
	if (isSensorWindow) {
		this->drawGuiSensorOptions(&isSensorWindow);
	}


	
}

//-----------------------------------------
void SensorManager::drawGuiSensorOptions(bool* opened){
	
	string textBlobsFound = "#blobs = "+ofToString(contourFinder.size(), 0);
	ImGui::Text(textBlobsFound.c_str());
	
	
	string sensorTextType = "Not configured Yet";
	if(sensorModel == kinectSensor){
		sensorTextType = "Kinect 1";
		
		ImGui::Text(sensorTextType.c_str());
		//ImGui::Checkbox("bThreshWithOpenCV", &bThreshWithOpenCV);
		ImGui::SliderInt("nearThreshold", &nearThreshold, 0, 255);
		ImGui::SliderInt("farThreshold", &farThreshold, 0, 255);
		ImGui::Separator();
		//TODO missing this property in ofxCv
		//ImGui::SliderInt("numBlobs ", &numBlobs, 1, 20);
		if(ImGui::SliderInt("minBlobs ", &minSizeBlob, 10, kinect.width*kinect.height*0.5)){
			contourFinder.setMinAreaRadius(minSizeBlob);
		}
		if(ImGui::SliderInt("maxBlobs ", &maxSizeBlob, 10, kinect.width*kinect.height*0.5)){
			contourFinder.setMaxAreaRadius(maxSizeBlob);
		}
		
		//ImGui::PopItemWidth();
	}else if (sensorModel == cameraSensor){
		sensorTextType = "OF Camera";
		ImGui::Text(sensorTextType.c_str());
		
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
		
		if(ImGui::SliderInt("min Area Blob", &minSizeBlob, 10, kinect.width*kinect.height*0.5)){
			contourFinder.setMinAreaRadius(minSizeBlob);
		}
		if(ImGui::SliderInt("max Area Blob", &maxSizeBlob, 10, kinect.width*kinect.height*0.5)){
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
	else{
		cout << "SensorManager::Error Set Sensor Mode Type" << endl;
	}

}


//-----------------------------------------
int SensorManager::getWidth(){
	int auxWidth = -1;
	if(sensorModel == kinectSensor){
		auxWidth = kinect.getWidth();
	}else if(sensorModel == cameraSensor){
		auxWidth = cam.getWidth();
	}
	
	return auxWidth;
}

//-----------------------------------------
int SensorManager::getHeight(){
	int auxHeight = -1;
	if(sensorModel == kinectSensor){
		auxHeight = kinect.getHeight();
	}else if(sensorModel == cameraSensor){
		auxHeight = cam.getHeight();
	}
	
	return auxHeight;
}

//-----------------------------------------
void SensorManager::exit(){
	
	if(sensorModel == kinectSensor){
			
		kinect.setCameraTiltAngle(0); // zero the tilt on exit
		kinect.close();
		
	}
}

//------------------------------------------
void SensorManager::keyPressed(int key){
	
	if(sensorModel == kinectSensor){
		switch (key) {
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
	}
}

//----------------------------------------
void SensorManager::setTrackingMode(bool _status){
	bTrackgingActive = _status;
}


//-----------------------------------------
bool SensorManager::setupCameraSensor(){
	cam.setup(640, 480);
	
	bool bConnected = false;
	
	//Computer Vision Stuff
	computerVisionImage.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_GRAYSCALE);
	
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
	
	
	///////////////////////////////////
	//General SensorData for others
	sensorWidth = cam.getWidth();
	sensorHeight = cam.getHeight();
	
	if(cam.isInitialized()){
		bConnected = true;
	}
	
	return bConnected;
}


//-----------------------------------------
bool SensorManager::setupKinectSensor(){
	
	bool bConnected = false;
	
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
	
	return bConnected;
	
}


