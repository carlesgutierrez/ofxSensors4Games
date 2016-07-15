//
//  SensorManager.h
//  ofxControlArkadeGames
//
//  Created by carles on 17/06/16.
//
//


#pragma once
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxImGui.h"
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


#ifdef USE_SENSOR_KINECT
#include "ofxKinect.h"
#endif




enum sensorType { kinectSensor, cameraSensor };
enum sensorMode{ realTimeMode, simulationMode };

class SensorManager {
	
// variables & methods for singleton
private:
	static bool	instanceFlag;
	static SensorManager *single;
public:
	static SensorManager* getInstance();
// end singleton
	
public:

	SensorManager();
	~SensorManager();

	void setup(sensorType _sensorType, sensorMode _sensorMode);
	void update();
	void draw();
	void exit();
	void keyReleased(ofKeyEventArgs & args);
	void keyPressed(ofKeyEventArgs & args);
	
	int getWidth();
	int getHeight();
	
	void drawGuiSensorOptions(bool* opened);
	
	bool isNewSensorFrame();
	

	//----------------------
	//Kinect
	bool setupKinectSensor();
#ifdef USE_SENSOR_KINECT
	ofxKinect kinect;
#endif
	//bool bThreshWithOpenCV;
	int nearThreshold;
	int farThreshold;
	int angle;
	
	//-----------------------
	//Webcam, Video or similar
private:
	void resetSimpleSensorCamera();
	bool updateVideoFolderComboSelections(string _videosPaths);
public:
	bool setupCameraSensor();
	ofVideoGrabber cam;
	ofVideoPlayer videoPlayerCam;
	ofDirectory myVideosDir;
	string svideosDirPath;
	string smovieFileName;
	int selectedMovieIndex = -1;
	int selectedCameraIndex = 0;
	bool bResetMoviePath = false;
	vector <string> videosAvailable;
	float videoPlayerCam_pos = 0;
	
	ofxCv::RunningBackground background;
	float learningTime = 30;
	float thresholdValue = 10;
	bool bresetBackground = true;
	bool bSimpleBackgroundSubstraction = true;
	bool bLearnBackground = false;
	bool bBlackWhiteCv = true;
	bool bInvertContourFinderThreshold = false;
	bool bAutoThreshold = true;
	bool bContourFinderThreshold = false;
	bool bContourFinderColorThreshold = false;
	ofColor colorTargetContourFinder = ofColor(0, 0, 255); //Default Blue Color
	
	ofImage diffCam;
	ofImage backGroundCam;
	
	//-----------------------
	//Tracking mode
	void setTrackingMode(bool _status);
	ofxCv::ContourFinder contourFinder;
	
	sensorType getSensorType();
	void setSensorType(sensorType _type);
	sensorMode getSensorMode();
	void setSensorMode(sensorMode _mode);
	
	
	//Drawable Vars
	float sensorDrawScale = 0.5;
	int marginDraw = 0;
	
private:
	
	//----------------------
	//General Sensor frame available
	bool bNewSensorFrame = false;

	
	//-----------------------
	//Tracking mode
	bool bTrackgingActive = false;
	bool showLabels = true;
	int maxPersistenceTracking = 15;
	int maxDistanceTracking = 32;

	//Related to ComputerVision
	ofImage computerVisionImage;
	
	//Blobs
	//int minTotalBlobs = 10;
	//int maxTotalBlobs = 200;
	int minSizeBlob = 5;
	int maxSizeBlob = 1000;
	int maxBlobsAccuracyMaxValue = maxSizeBlob;
	int numBlobs = 20;
	
	//-----------------------
	//WebCam or similar video camera
	//ofVideoCamera...

	//Control Vars
	int sensorWidth = 640;//TODO check this is always modif
	int sensorHeight = 480;
	sensorType typeSensor;
	sensorMode modeSensor;

};