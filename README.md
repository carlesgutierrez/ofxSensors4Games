ofxSensors4Games
============

![ofxSensors4Games](header.png)

Introduction
------------

This addon provides a remote interface that aims to control your game or App by interpret sense action into Network packages. 
Setup a camera ( or another available sensor ), define regions of interests, select for each one a method to interpred such actions. The easier method filter maximum X/Y detected objects and send this with a proper label to the network by OSC protocol.

Sensors available: 
* Camera / Videoplayer sensors. Cameras ( Infrared or not ) are quite cheap and practical to use it for outdoors environtmets. 
But There are other powerfull sensors that can be also used. You could reconfigure this addon and use: 
* Kinect1( require specific installation for windows). Actually remain disabled.
* External RadarGlxRss3 ( a speed radar usually for cars ofxRadarGlxRss3). 
* External LaserRange Sensor: Sick LMS1XX. ( Receive OSC from resulst from ofxSick )


Installation
------------
Copy or clone the `ofxSensors4Games` folder into your `openFrameworks/addons/` folder. Open and Run the example. In order to select another sensor instead of camera or videoplayer, you will require to modify setup function at ofApp.cpp. Camera is default life camera and will start if it's available or otherwise will play a default video ( located at bin/data/ )

	
Dependencies
------------
OF Addons (from 0.98):

* ofxNetwork
* ofxOsc
* ofxOpencv
* //ofxKinect add this manually. !!! README  !!! ofxkinect README (usb libs install) 

ofxAddons:

* ofxImgui (https://github.com/jvcleave/ofxImGui)
* ofxJSON (https://github.com/jeffcrouse/ofxJSON)
* ofxCv (https://github.com/kylemcdonald/ofxCv)
(for each update projet you do , You will have to this at Properties, C/C++ -> Output Files -> "Object File Name" to be the
following: $(IntDir)/%(RelativeDir)/)
//Addons only for Windows
*ofxSpout2 (https://github.com/Kj1/ofxSpout2) // TODO try trhis fork (https://github.com/sheridanis/ofxSpout2), that fix potencial crash
    
For sensors SickLMS111 or RadarGlxRss3:
* ofxRadarGlxRss3: https://github.com/carlesgutierrez/ofxRadarGlxRss3
* ofxSick: https://github.com/carlesgutierrez/ofxSick
Check Nuclai workshop examples (July 2016): https://github.com/carlesgutierrez/openLab_AdvancedSensors4PublicGames
I know this is proper documented, so send me an email [carles.gutierrez[at]gmail.com] if you have any doubt.


Compatibility
------------
Tested with OF 0.9.x on: Windows 7/10, Visual Studio 2015. Require full install (http://openframeworks.cc/setup/vs/)

Usage 
-----
Configure the sensor: 
* Setup the number and the locations of ROI ( Regions of Interest )
* Setup the Controller Method ( MaxMinXY or AllBlobs )
* Select OSC destination. 

Then your App or Game ( ready for OSC or UDP ) will receive at port 12342 (OSC) or UPD (29095) the desired filtered data, that will represent your desired actions for your App. That are commonly designed for VideoGames but could be used for other propouses.

## Network data type ( OSC ) 

### For MaxMinXY Controller: 

	#### OSC:

		//ResumedBlob
		//ROI 1
		osc:/GameBlob
		*	- (float) x pos 	[0..1]
		*	- (float) y pos 	[0..1]
		*	- (float) UP mov 	[0..1]
		*	- (float) DOWN mov	[0..1]

		//ROI 2
		osc:/GameBlob2
		*	- (float) x pos 	[0..1]
		*	- (float) y pos 	[0..1]
		*	- (float) UP mov 	[0..1]
		*	- (float) DOWN mov	[0..1]

	#### For this controller is available UPD protocol at port 29095 
		* "SEND ffff f0(X)  f1(Y)  f2(UP)  f3(DOWN)";
			-> ffff x, y, up, down 
	
### For AllBlobIn Controller: 

	#### OSC: 
	
		osc:/GameBlobAllIn
		* - (int) Total num of Blobs "N"
		Then "N" times:
		* - (float) X [ 0 .. 1]
		* - (float) Y [ 0 .. 1]
		if( trackingMode == FindContournsTracking )
		{
			//for Tracking add int ID & int TIME
			* - (int) id 
			* - (int) time 
		}
}
		
	## TODO other filters?


**********************************************************

License
-------
The MIT License (MIT)

Copyright (c) 2017 Carles Gutierrez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
