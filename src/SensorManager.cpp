//
//  sensorManagement.cpp
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//

#include "SensorManager.h"


//-----------------------------------------
void SensorManager::setup(sensorType _sensorType){
	
	sensorModel = _sensorType;
	
	///////////////////////
	//Sensor Configurations
	
	if (sensorModel == kinectSensor) {
	
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
		}
		
		// zero the tilt on startup
		angle = 0;
		kinect.setCameraTiltAngle(angle);
		
		//Related to ComputerVision
		nearThreshold = 230;
		farThreshold = 70;
		bThreshWithOpenCV = true;
		
		//Computer Vision Stuff
		colorImg.allocate(kinect.width, kinect.height);//general
		grayImage.allocate(kinect.width, kinect.height);//general
		grayThreshNear.allocate(kinect.width, kinect.height);//kinect uses
		grayThreshFar.allocate(kinect.width, kinect.height);//kinect uses
		
		///////////////////////////////////
		//General SensorData for others
		sensorWidth = kinect.width;
		sensorHeight = kinect.height;
		
	}
	
}

//-----------------------------------------
void SensorManager::update(){
	if(sensorModel == kinectSensor){
		
		kinect.update();
		
		// there is a new frame and we are connected
		if(kinect.isFrameNew()) {
			
			// load grayscale depth image from the kinect source
			grayImage.setFromPixels(kinect.getDepthPixels());
			
			// we do two thresholds - one for the far plane and one for the near plane
			// we then do a cvAnd to get the pixels which are a union of the two thresholds
			if(bThreshWithOpenCV) {
				grayThreshNear = grayImage;
				grayThreshFar = grayImage;
				grayThreshNear.threshold(nearThreshold, true);
				grayThreshFar.threshold(farThreshold);
				cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
			} else {
				
				// or we do it ourselves - show people how they can work with the pixels
				ofPixels & pix = grayImage.getPixels();
				int numPixels = pix.size();
				for(int i = 0; i < numPixels; i++) {
					if(pix[i] < nearThreshold && pix[i] > farThreshold) {
						pix[i] = 255;
					} else {
						pix[i] = 0;
					}
				}
			}
			
			// update the cv images
			grayImage.flagImageChanged();
			
			// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
			// also, find holes is set to true so we will get interior contours as well....
			contourFinder.findContours(grayImage, minSizeBlob, maxSizeBlob, numBlobs, false);
		}
		

	}
}

//-----------------------------------------
void SensorManager::draw(){
	
	if(sensorModel == kinectSensor){
		
		ofSetColor(255, 255, 255);
		

		// draw from the live kinect
		kinect.drawDepth(10, 10, kinect.width*0.5, kinect.height*0.5);
		kinect.draw(420, 10, kinect.width*0.5, kinect.height*0.5);
			
		grayImage.draw(10, 320, kinect.width*0.5, kinect.height*0.5);
		contourFinder.draw(10, 320, kinect.width*0.5, kinect.height*0.5);

		
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
		<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
		<< "set near threshold " << nearThreshold << " (press: + -)" << endl
		<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
		<< ", fps: " << ofGetFrameRate() << endl
		<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
		
		if(kinect.hasCamTiltControl()) {
			reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
			<< "press 1-5 & 0 to change the led mode" << endl;
		}
		
		ofDrawBitmapString(reportStream.str(), 20, 652);
	}
	

	
	//Draw some Sensor Option
	bool isSensorWindow = true;
	if (isSensorWindow) {
		this->drawGuiSensorOptions(&isSensorWindow);
	}


	
}

//-----------------------------------------
void SensorManager::drawGuiSensorOptions(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("Sensor Gui Window", opened, ImGuiWindowFlags_MenuBar)) {
		
		string textBlobsFound = "#blobs = "+ofToString(contourFinder.nBlobs, 0);
		ImGui::Text(textBlobsFound.c_str());
		
		
		string sensorTextType = "Not configured Yet";
		if(sensorModel == kinectSensor){
			sensorTextType = "Kinect 1";
		}else if (sensorModel == cameraSensor){
			sensorTextType = "Camera ... TODO Set here the USC device used";
		}
		
		ImGui::Text(sensorTextType.c_str());
		ImGui::Checkbox("bThreshWithOpenCV", &bThreshWithOpenCV);
		ImGui::SliderInt("nearThreshold", &nearThreshold, 0, 255);
		ImGui::SliderInt("farThreshold", &farThreshold, 0, 255);
		ImGui::Separator();
		ImGui::SliderInt("numBlobs ", &numBlobs, 1, 20);
		ImGui::SliderInt("minBlobs ", &minSizeBlob, 10, 640*480*0.5);
		ImGui::SliderInt("maxBlobs ", &maxSizeBlob, 10, 640*480*0.5);
		
		//ImGui::PopItemWidth();
		
		
		ImGui::End();
	}
}


//-----------------------------------------
int SensorManager::getWidth(){
	int auxWidth = -1;
	if(sensorModel == kinectSensor){
		auxWidth = kinect.getWidth();
	}
	
	return auxWidth;
}

//-----------------------------------------
int SensorManager::getHeight(){
	int auxHeight = -1;
	if(sensorModel == kinectSensor){
		auxHeight = kinect.getHeight();
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
				bThreshWithOpenCV = !bThreshWithOpenCV;
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


