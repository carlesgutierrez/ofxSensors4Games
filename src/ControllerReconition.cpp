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
void ControllerReconition::setup(int width, int height){
	


	
}

//-----------------------------------------
void ControllerReconition::update(){
	
}

//-----------------------------------------
void ControllerReconition::draw(){
	
}

//-----------------------------------------
void ControllerReconition::exit(){
	
}

//-----------------------------------------
void ControllerReconition::keyPressed(int key){
	
}

/*
void ControllerReconition::drawGuiControllerOptions(bool* opened){
	
	
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	
	if (ImGui::Begin("Sensor Gui Window", opened, ImGuiWindowFlags_MenuBar)) {
		
		//ImGui::PushItemWidth(100);
		
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
		
		
		//ImGui::PopItemWidth();
		
		
		ImGui::End();
	}
}
 */
