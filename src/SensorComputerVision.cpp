#pragma once

#include "SensorComputerVision.h"


//-----------------------------------------
void SensorComputerVision::setup(int _id, int _cameraW, int _cameraH, sensorType _myTypeSensor) {

	idSensorCV = _id;

	cameraWidth = _cameraW;
	cameraHeight = _cameraH;

	mySensorType = _myTypeSensor;

	maxSizeBlob = cameraHeight; // Direct from Camera resolution. seems ok working at 640 x 480

	computerVisionImage.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);

	contourFinder = new (ofxCv::ContourFinder);
	//filter minSizeBlob, maxSizeBlob, numBlobs
	contourFinder->setMinAreaRadius(minSizeBlob);
	contourFinder->setMaxAreaRadius(maxSizeBlob);
	contourFinder->setThreshold(numBlobs);

	///Tracker Setup Always
	//if (trackingMode == FindContournsTracking) {
		// wait for half a frame before forgetting something
		contourFinder->getTracker().setPersistence(maxPersistenceTracking);
		// an object can move up to 32 pixels per frame
		contourFinder->getTracker().setMaximumDistance(maxDistanceTracking);
		showLabels = true;
	//}

	//In Case Kinect Force to do regular threslholding ... guessing
	if (mySensorType == kinectSensor) {
		bSimpleBackgroundSubstraction = false;
		bLearnBackground = false;
		bContourFinderThreshold = false;

		//Computer Vision Stuff
		//Main Image used to Cv
		computerVisionImage.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);
		//filter minSizeBlob, maxSizeBlob, numBlobs
		contourFinder->setMinAreaRadius(minSizeBlob);
		contourFinder->setMaxAreaRadius(maxSizeBlob);
		contourFinder->setThreshold(numBlobs);
	}
}


//----------------------------------------
void SensorComputerVision::udpateBackground() {
	if (bLearnBackground) {

		if (bresetBackground) {
			background.reset();
			bresetBackground = false;
		}
	}
}

//-----------------------------------------
void SensorComputerVision::mainComputerVision(ofImage _image2Compute) {


	if (mySensorType == kinectSensor) {
		//hard reset to apply simple and default threshold image
		bSimpleBackgroundSubstraction = false;
		bLearnBackground = false;
		bContourFinderThreshold = false;

		//Update Texture
		//computerVisionImage.setFromPixels(_image2Compute.getPixelsRef().getPixels(), cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);
		computerVisionImage = _image2Compute;
		computerVisionImage.update();

		//Is really INvert Useful? 
		if (bInvertContourFinderThreshold)contourFinder->setInvert(true);
		else contourFinder->setInvert(false);

		//Find Countours
		contourFinder->findContours(computerVisionImage);
	}
	else {
		//Then it's a regular cameraSensor
		if (blur > 0)ofxCv::blur(_image2Compute, blur);
		//if (gaussianBlur > 0)ofxCv::GaussianBlur(_image2Compute, gaussianBlur);
		
		if (bLearnBackground) {

			//TODO MEDTHOD Thresholds
			background.setDifferenceMode(background.BRIGHTER);

			background.setLearningTime(learningTime);
			background.setThresholdValue(thresholdValue);
			
			//Mode Threshold setup
			if (selectedThersholdMethodId == 0) { //ABSDIFF
				background.setDifferenceMode(background.ABSDIFF);
			}
			else if (selectedThersholdMethodId == 1) { //BRIGHTER
				background.setDifferenceMode(background.BRIGHTER);
			}else background.setDifferenceMode(background.DARKER);
			
			//Camera Image to Gray
			//TODO Check for lower resolutions (320x240) what can crash over here
			background.update(_image2Compute, computerVisionImage);
			computerVisionImage.update();

			if (erode > 0)ofxCv::erode(computerVisionImage, erode);
			if (dilate > 0)ofxCv::dilate(computerVisionImage, dilate);
			if (postBlur > 0)ofxCv::blur(computerVisionImage, postBlur);

			contourFinder->findContours(computerVisionImage);


		}
		else if (bSimpleBackgroundSubstraction) {

			//Camera Image to Gray
			ofxCv::convertColor(_image2Compute, computerVisionImage, CV_RGB2GRAY);

			computerVisionImage.update();

			//Save Background Frame
			if (bresetBackground) {
				backGroundCam = computerVisionImage;
				backGroundCam.update();
				bresetBackground = false;
			}


			//background substraction modes
			switch (selectedThersholdMethodId) {	
				case 0: ofxCv::absdiff(computerVisionImage, backGroundCam, diffCam); break;
				case 1: ofxCv::subtract(computerVisionImage, backGroundCam, diffCam); break;
				case 2: ofxCv::subtract(backGroundCam, computerVisionImage, diffCam); break;
			}

			//Configure invert Threshold
			if (bInvertContourFinderThreshold)contourFinder->setInvert(true);
			else contourFinder->setInvert(false);

			if (bContourFinderThreshold) {
				//FindContours Threshold
				contourFinder->setThreshold(thresholdValue);
				if (erode > 0)ofxCv::erode(diffCam, erode);
				if (dilate > 0)ofxCv::dilate(diffCam, dilate);
				if (postBlur > 0)ofxCv::blur(diffCam, postBlur);
				contourFinder->findContours(diffCam);
			}
			else if (bAutoThreshold) {
				//Automatic Thresholding
				ofxCv::autothreshold(diffCam);
				if (erode > 0)ofxCv::erode(diffCam, erode);
				if (dilate > 0)ofxCv::dilate(diffCam, dilate);
				if (postBlur > 0)ofxCv::blur(diffCam, postBlur);
				contourFinder->findContours(diffCam);
			}
			else {
				//Regular Threshold
				ofxCv::threshold(diffCam, thresholdValue);
				if (erode > 0)ofxCv::erode(diffCam, erode);
				if (dilate > 0)ofxCv::dilate(diffCam, dilate);
				if (postBlur > 0)ofxCv::blur(diffCam, postBlur);
				contourFinder->findContours(diffCam);
			}

			diffCam.update();

		}
		else { // IFNOT bLearnBackground and bSimpleBackgroundSubstraction

			//Update Camera colors

			computerVisionImage.setFromPixels(_image2Compute.getPixelsRef().getPixels(), cameraWidth, cameraHeight, OF_IMAGE_COLOR);

			computerVisionImage.update();

			if (bContourFinderThreshold) {

				contourFinder->setAutoThreshold(true);

				if (bContourFinderColorThreshold) {
					//FindContours Threshold
					contourFinder->setUseTargetColor(true);
					contourFinder->setTargetColor(colorTargetContourFinder);
					//TODO ADD Color Picker From Camera.
				}

			}
			else {
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

				//contourFinder->setAutoThreshold(true);
				//contourFinder->setInvert(false);
				contourFinder->setUseTargetColor(false);
				//contourFinder->setThreshold(thresholdValue);
			}

			//Finally FindContours

			//Apply invert Threshold
			if (bInvertContourFinderThreshold)contourFinder->setInvert(true);
			else contourFinder->setInvert(false);

			//Apply Configured Thresdhold
			if (bAutoThreshold) contourFinder->setAutoThreshold(true);
			else contourFinder->setThreshold(thresholdValue);

			//Find Countours
			contourFinder->findContours(computerVisionImage);
		}



	}
}

//----------------------------------------
void SensorComputerVision::draw(float _sensorDrawScale, int _marginDraw) {

	if (computerVisionImage.isAllocated()) {

		ofEnableAlphaBlending();
		ofSetColor(ofColor::lightGray);

		if (bLearnBackground) {
			computerVisionImage.draw(cameraWidth*_sensorDrawScale, _marginDraw, cameraWidth*_sensorDrawScale, cameraHeight*_sensorDrawScale);
			ofxCv::toOf(background.getBackground(), backGroundCam);
			backGroundCam.draw(2 * cameraWidth*_sensorDrawScale, _marginDraw, cameraWidth*_sensorDrawScale, cameraHeight*_sensorDrawScale);

		}
		else if (bSimpleBackgroundSubstraction) {
			diffCam.draw(cameraWidth*_sensorDrawScale, _marginDraw, cameraWidth*_sensorDrawScale, cameraHeight*_sensorDrawScale);
			backGroundCam.draw(2 * cameraWidth*_sensorDrawScale, _marginDraw, cameraWidth*_sensorDrawScale, cameraHeight*_sensorDrawScale);
		}
		else {
			computerVisionImage.draw(cameraWidth*_sensorDrawScale, _marginDraw, cameraWidth*_sensorDrawScale, cameraHeight*_sensorDrawScale);
		}

		ofDisableAlphaBlending();

		ofSetColor(255, 255, 255);

		//---------------------------------------
		//Kyle Mcdonnal - Blob Tracker Visualization
		if (trackingMode == FindContournsTracking) {

			//TODO To acces this from outside may be neceseary to clean
			ofxCv::RectTracker& tracker = contourFinder->getTracker();


			if (showLabels) {

				for (int i = 0; i < contourFinder->size(); i++) {
					ofPoint center = ofxCv::toOf(contourFinder->getCentroid(i));
					ofPushMatrix();
					ofTranslate(cameraWidth*_sensorDrawScale, _marginDraw);
					ofTranslate(center.x*_sensorDrawScale, center.y*_sensorDrawScale);
					int label = contourFinder->getLabel(i);
					string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
					ofDrawBitmapString(msg, 0, 0);
					ofVec2f velocity = ofxCv::toOf(contourFinder->getVelocity(i));
					ofDrawLine(0, 0, velocity.x*_sensorDrawScale, velocity.y*_sensorDrawScale);
					ofPopMatrix();

				}

			}
			else {

				ofPushMatrix();
				ofTranslate(cameraWidth*_sensorDrawScale, _marginDraw);
				ofScale(_sensorDrawScale, _sensorDrawScale);

				for (int i = 0; i < contourFinder->size(); i++) {
					unsigned int label = contourFinder->getLabel(i);
					// only draw a line if this is not a new label
					if (tracker.existsPrevious(label)) {
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
			for (int i = 0; i < currentLabels.size(); i++) {
				int j = currentLabels[i];
				ofDrawLine(j, 0, j, 4);
			}
			ofSetColor(ofxCv::magentaPrint);
			for (int i = 0; i < previousLabels.size(); i++) {
				int j = previousLabels[i];
				ofDrawLine(j, 4, j, 8);
			}
			ofSetColor(ofxCv::yellowPrint);
			for (int i = 0; i < newLabels.size(); i++) {
				int j = newLabels[i];
				ofDrawLine(j, 8, j, 12);
			}
			ofSetColor(ofColor::white);
			for (int i = 0; i < deadLabels.size(); i++) {
				int j = deadLabels[i];
				ofDrawLine(j, 12, j, 16);
			}



		}

		ofPushStyle();
		//Finally the countours matching our image
		ofSetColor(255, 0, 0);

		ofPushMatrix();
		ofTranslate(cameraWidth*_sensorDrawScale, _marginDraw); //TODO change _sensorDrawScale as int sensorScale var
		ofScale(_sensorDrawScale, _sensorDrawScale);
		if(trackingMode == FindContourns || trackingMode == FindContournsTracking)contourFinder->draw();
		ofPopMatrix();

		ofPopStyle();
	}
}

//--------------------------------------------
void SensorComputerVision::drawGui() {

	string IdTextCamera = "CV [" + ofToString(idSensorCV, 0) + "]";

	if (ImGui::CollapsingHeader(IdTextCamera.c_str())) {

		
		ImGui::SliderInt("Blur Cv", &blur, 0, 10);
		//ImGui::SliderInt("gaussianBlur Cv", &gaussianBlur, 0, 10);
		ImGui::SliderInt("Erode Cv", &erode, 0, 10);
		ImGui::SliderInt("Dilate Cv", &dilate, 0, 10);

		ImGui::SliderInt("Post Blur Cv", &postBlur, 0, 10);
		//TODO missing this property in ofxCv
		//ImGui::SliderInt("numBlobs ", &numBlobs, 1, 20);
		//ImGui::SliderInt("accuracyMaxSizeBlob", &maxBlobsAccuracyMaxValue, 0, cameraWidth *cameraHeight);

		string minAreaText = "minArea##" + IdTextCamera;
		string maxAreaText = "maxArea##" + IdTextCamera;

		if (ImGui::SliderInt(maxAreaText.c_str(), &maxSizeBlob, minBlobsArea, maxBlobsArea)) {
			contourFinder->setMaxAreaRadius(maxSizeBlob);
		}

		if (ImGui::SliderInt(minAreaText.c_str(), &minSizeBlob, minBlobsArea, maxSizeBlob)) {
			contourFinder->setMinAreaRadius(minSizeBlob);
		}



		string textBlobsFound = "#blobs = " + ofToString(contourFinder->size(), 0);
		ImGui::Text(textBlobsFound.c_str());

		if(mySensorType == cameraSensor){

			string doLearningBackgroundText = "Do Learning Background##" + ofToString(idSensorCV, 0);
			ImGui::Checkbox(doLearningBackgroundText.c_str(), &bLearnBackground);

			if (ImGui::Button("Reset Background")) {
				bresetBackground = true;
			}


			if (bLearnBackground) {
				string learningTimeText = "Learning Time##" + ofToString(idSensorCV, 0);
				ImGui::SameLine();
				ImGui::PushItemWidth(100);
				ImGui::SliderFloat(learningTimeText.c_str(), &learningTime, 0, 255);
				ImGui::PopItemWidth();
			}

			string BackgroundSubstractionText = "Background Substraction##" + ofToString(idSensorCV, 0);
			ImGui::Checkbox(BackgroundSubstractionText.c_str(), &bSimpleBackgroundSubstraction);

			if (bSimpleBackgroundSubstraction) {
				ImGui::SameLine();
				ImGui::Checkbox("Auto Threshold", &bAutoThreshold);
			}

			string contoursFinderOptionsText = "ContoursFinder Options##" + ofToString(idSensorCV, 0);
			ImGui::Checkbox(contoursFinderOptionsText.c_str(), &bContourFinderThreshold);
			string invertThresholdContoursFinderText = "Invert Threshold ContoursFinder##" + ofToString(idSensorCV, 0);
			ImGui::Checkbox(invertThresholdContoursFinderText.c_str(), &bInvertContourFinderThreshold);

			if (bContourFinderThreshold) {

				string contoursFinderColorTargetText = "ContoursFinder Color Target##" + ofToString(idSensorCV, 0);
				ImGui::Checkbox(contoursFinderColorTargetText.c_str(), &bContourFinderColorThreshold);
				if (bContourFinderColorThreshold) {
					ImVec4 colorTargetVec = colorTargetContourFinder;
					string colorTargetContourFinderText = "ContoursFinder Color Target##" + ofToString(idSensorCV, 0);
					ImGui::ColorEdit3(colorTargetContourFinderText.c_str(), (float*)& colorTargetVec);

					if (bContourFinderColorThreshold) {
						colorTargetContourFinder.r = colorTargetVec.x * 255;
						colorTargetContourFinder.g = colorTargetVec.y * 255;
						colorTargetContourFinder.b = colorTargetVec.z * 255;
					}
				}

			}

			ComboCinder("Threslod Mode", &selectedThersholdMethodId, diffThresholdModes, diffThresholdModes.size());
			string thresholdValueText = "Threshold Value##" + ofToString(idSensorCV, 0);
			ImGui::SliderFloat(thresholdValueText.c_str(), &thresholdValue, 0, 255);
		}

		ImGui::Separator();
		//ImGui::Checkbox("Activate Tracking", &bTrackgingActive);
		
		const char* RecognitionMethod_items[] = { "FindContourns", "FindContournsTracking", "DarkNetTODO" };
		//ImGui::Combo("MaxMin X type", &item_resumedBlob_X, RecognitionMethod_items, IM_ARRAYSIZE_TEMP2(combo_resumedBlob_X));
		static int RecognitionMethod_item_current = trackingMode;//Deafult simple
		if (ImGui::ListBox("Tracking Mode", &RecognitionMethod_item_current, RecognitionMethod_items, IM_ARRAYSIZE_TEMP1(RecognitionMethod_items), 4)) {
			trackingMode = static_cast<detectionMethod>(RecognitionMethod_item_current);
		}
		ImGui::Separator();

		

		if (trackingMode == FindContournsTracking) {
			ImGui::SliderInt("Max Persistance", &maxPersistenceTracking, 5, 100);
			ImGui::SliderInt("Max Distance", &maxDistanceTracking, 10, 100);
			ImGui::Checkbox("Show Labels", &showLabels);
		}
	}

}
