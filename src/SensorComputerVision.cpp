#pragma once

#include "SensorComputerVision.h"


//-----------------------------------------
void SensorComputerVision::setup(int _id, int _cameraW, int _cameraH) {

	idSensorCV = _id;

	cameraWidth = _cameraW;
	cameraHeight = _cameraH;


	computerVisionImage.allocate(cameraWidth, cameraHeight, OF_IMAGE_GRAYSCALE);

	contourFinder = new (ofxCv::ContourFinder);
	//filter minSizeBlob, maxSizeBlob, numBlobs
	contourFinder->setMinAreaRadius(minSizeBlob);
	contourFinder->setMaxAreaRadius(maxSizeBlob);
	contourFinder->setThreshold(numBlobs);

	///Tracker
	if (bTrackgingActive) {
		// wait for half a frame before forgetting something
		contourFinder->getTracker().setPersistence(maxPersistenceTracking);
		// an object can move up to 32 pixels per frame
		contourFinder->getTracker().setMaximumDistance(maxDistanceTracking);
		showLabels = true;
	}
}

//----------------------------------------
void SensorComputerVision::setTrackingMode(bool _status) {
	bTrackgingActive = _status;
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

	//blur(movie, 10);//TODO This! easy and fast. Add slider options

	//TODO FIX THIS bLearnBackground to not apply really a Learning background if its not active.
	if (bLearnBackground) {



		background.setLearningTime(learningTime);
		background.setThresholdValue(thresholdValue);
		//Camera Image to Gray
		background.update(_image2Compute, computerVisionImage);

		computerVisionImage.update();

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


		//then background substraction //TODO check diferent methods
		ofxCv::absdiff(computerVisionImage, backGroundCam, diffCam);

		//Apply invert Threshold
		if (bInvertContourFinderThreshold)contourFinder->setInvert(true);
		else contourFinder->setInvert(false);

		if (bContourFinderThreshold) {
			//FindContours Threshold
			contourFinder->setThreshold(thresholdValue);
			contourFinder->findContours(diffCam);
		}
		else if (bAutoThreshold) {
			//Automatic Thresholding
			ofxCv::autothreshold(diffCam);
			contourFinder->findContours(diffCam);
		}
		else {
			//Regular Threshold
			ofxCv::threshold(diffCam, thresholdValue);
			contourFinder->findContours(diffCam);
		}

		diffCam.update();

	}
	else { //ContourFinder Methods

		   //Update Camera colors

		computerVisionImage.setFromPixels(_image2Compute.getPixels(), cameraWidth, cameraHeight, OF_IMAGE_COLOR);

		computerVisionImage.update();

		//Threshold

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

			contourFinder->setAutoThreshold(true);
			contourFinder->setInvert(false);
			contourFinder->setUseTargetColor(false);
			contourFinder->setThreshold(thresholdValue);
		}


		//Apply invert Threshold
		if (bInvertContourFinderThreshold)contourFinder->setInvert(true);
		else contourFinder->setInvert(false);


		//Apply Configured Thresdhold
		contourFinder->setThreshold(thresholdValue);

		//Find Countours
		contourFinder->findContours(computerVisionImage);

	}
}

//----------------------------------------
void SensorComputerVision::draw(float _sensorDrawScale, int _marginDraw) {

	if (computerVisionImage.isAllocated()) {

		ofSetColor(255, 255, 255);

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


		//---------------------------------------
		//Kyle Mcdonnal - Blob Tracker Visualization
		if (bTrackgingActive) {

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

		//Finally the countours matching our image
		ofSetColor(255, 0, 0);

		ofPushMatrix();
		ofTranslate(cameraWidth*_sensorDrawScale, _marginDraw); //TODO change _sensorDrawScale as int sensorScale var
		ofScale(_sensorDrawScale, _sensorDrawScale);
		contourFinder->draw();
		ofPopMatrix();
	}
}

//--------------------------------------------
void SensorComputerVision::drawGui() {

	string IdTextCamera = "ComputerVision Id[" + ofToString(idSensorCV, 0) + "]";

	if (ImGui::CollapsingHeader(IdTextCamera.c_str())) {

		string textBlobsFound = "#blobs = " + ofToString(contourFinder->size(), 0);
		ImGui::Text(textBlobsFound.c_str());

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

		string thresholdValueText = "Threshold Value##" + ofToString(idSensorCV, 0);
		ImGui::SliderFloat(thresholdValueText.c_str(), &thresholdValue, 0, 255);

		/*
		ImGui::SliderInt("accuracyMaxSizeBlob", &maxBlobsAccuracyMaxValue, 0, cameraWidth *cameraHeight*sensorDrawScale);

		if (ImGui::SliderInt("min Area Blob", &minSizeBlob, _marginDraw, maxBlobsAccuracyMaxValue)) {
		contourFinder.setMinAreaRadius(minSizeBlob);
		}
		if (ImGui::SliderInt("max Area Blob", &maxSizeBlob, _marginDraw, maxBlobsAccuracyMaxValue)) {
		contourFinder.setMaxAreaRadius(maxSizeBlob);
		}*/

		ImGui::Separator();

		ImGui::Checkbox("Activate Tracking", &bTrackgingActive);

		if (bTrackgingActive) {
			ImGui::SliderInt("Max Persistance", &maxPersistenceTracking, 5, 100);
			ImGui::SliderInt("Max Distance", &maxDistanceTracking, 10, 100);
			ImGui::Checkbox("Show Labels", &showLabels);
		}
	}

}
