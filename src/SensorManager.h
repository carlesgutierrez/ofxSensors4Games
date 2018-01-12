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
#include "SensorComputerVision.h"
#define IM_ARRAYSIZE_TEMP2(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "ofxJSON.h"


//Uncomment this to Use it
#define USE_SHARECAM_SPOUT



#if defined(USE_SHARECAM_SPOUT)//and (TARGET_WIN32)
#include "ofxSpout2Sender.h"
#endif

struct PlayerArea {
	ofRectangle rectArea;
	bool bAreaActive = false;
};


//Uncomment this to Use it
//#define USE_SENSOR_KINECT

#ifdef USE_SENSOR_KINECT
#include "ofxKinect.h"
#endif


struct sickData
{
	int id;
	ofPoint pos;
	//int britness;
};

struct radarData
{
	int id;
	ofPoint pos;
	int strength;
};



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

	void setup(sensorType _sensorType);
	void resetVideoInterface();
	void update();
	void draw();
	void drawAreaRectangle(ofRectangle _areaRect, int idSensorId);
	void exit();
	void keyReleased(ofKeyEventArgs & args);
	void keyPressed(ofKeyEventArgs & args);
	
	int getWidth();
	int getHeight();
	
	void drawGuiSensorOptions(bool* opened);

	//-----------------------------------------
	//Modes Set/Get setup
	sensorType getSensorType();
	void setSensorType(sensorType _type);
	sensorMode getSensorMode();
	ofxJSONElement getParams();
	bool setParams(ofxJSONElement jsonFile);
	void setSensorMode(sensorMode _mode);
	bool isNewSensorFrame();

	//----------------------
	//Kinect
	bool setupKinectSensor();
#ifdef USE_SENSOR_KINECT
	ofxKinect kinect;
	ofImage depthKinectImage;
	ofImage myThresholdKinect;
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
	
	//Computer Vision Vars
	//SensorComputerVision computerVisionSensor1;
	//SensorComputerVision computerVisionSensor2;
	vector<SensorComputerVision> computerVisionSensorVector;

	void updateSubImagesFromImageRaw(ofRectangle _rectArea, ofImage &image2Update);
	void applyMaskToImgVideoCam(ofRectangle _rectArea, ofImage & imageToMask);
	//------------------------
	//External OSC sick
	//TODO add struct or class with Id & britness?
	vector<sickData> sickBlobs;
	bool setupExternalSickSensor();
	bool updateExternalSickSensor();//Returns true if has received something at least
	
	//Drawable Vars
	float sensorDrawScale = 0.5;
	int marginDraw = 0;

	//TODO set shit rect at Gui 
	int numPlayersAreas = 2;//TODO dinamic //Minimum Areas Editables. This Can grow for now until 2. TODO extend to N
	vector<PlayerArea> playerAreas;

	
	
private:
	
	//Previous vars for ComputerVision
	bool bNewSensorFrame = false;
	//-----------------------
	ofImage sourceImageRaw;
	ofFbo mySourcedSensorFbo;
	vector<ofImage> sensorImageVector;
	//ofImage sensorImage1;
	//ofImage sensorImage2;


	//-----------------------
	//WebCam or similar video camera
	//Control Vars
	int sensorWidth = 640;//TODO check this is always modif
	int sensorHeight = 480;
	sensorType typeSensor;
	sensorMode modeSensor;
	vector<string> selectableSensorMode{ "realTimeMode", "simulationMode" };
	ofPixels videoPixels;
	bool bVideoPlayer;
	int idVideoGrabber;
	
	//Receiver externalSickSensor
	const int PortRecvExt = 4000; //Default value
	ofxOscReceiver receiverExt;
	bool bResetHostIp = false;

#if defined(USE_SHARECAM_SPOUT)//AND (TARGET_WIN32)
	bool bSpoutCameraActive = false;
	ofxSpout2::Sender sender;
#endif

};