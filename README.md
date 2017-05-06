ofxSensors4Games
============

![ofxSensors4Games](header.png)

Introduction
------------

This addon provides a remote interface that aims to control games that can be controlled using simple button actions. 
This it's possible by using external sensors such for example an Infrared Camera, then sending detected actions to external games by OSC / UPD.

Infrared cameras are cheap and practical to use. But There are other powerfull sensors that can be also used. 
For now we keep the posibility to add external sensors here and send data filtered to your game or App.  

Internal Sensors allowed: 
* Camera and Videoplayer for testing.
* Kinect ( require specific installation ). In actual version has been disabled.
* RadarGlxRss3 
* LaserRange Sensor: Sick LMS1XX. 


This addon is using the last v0.9.8 release but should work with others 0.9.x Of versions.


Installation
------------

Copy the `ofxSensors4Games` folder into your `openFrameworks/addons/` folder.  Modify setup in order to select you sensor (Now only available cameraSensor) and running mode ( realtime or simulation) ( life videoCamera or just play a video located at bin/data )

	
Dependencies
------------
Externals:
* ofxImgui (https://github.com/jvcleave/ofxImGui)
* ofxJSON (https://github.com/jeffcrouse/ofxJSON)
* ofxCv (https://github.com/kylemcdonald/ofxCv)
(for each update projet you do , You will have to this at Properties, C/C++ -> Output Files -> "Object File Name" to be the
following: $(IntDir)/%(RelativeDir)/)
//Addons only for Windows
*ofxSpout2 (https://github.com/Kj1/ofxSpout2) // TODO try trhis fork (https://github.com/sheridanis/ofxSpout2), that fix potencial crash
    
 
Internals:
* ofxNetwork
* ofxOsc
* ofxOpencv
* //ofxKinect add this manually. !!! README  !!! ofxkinect README (usb libs install) 


Compatibility
------------

Tested with OF 0.9.x on

    Windows 7/10, Visual Studio 2015. Require full install (http://openframeworks.cc/setup/vs/)


For camera sensor: 
 Sometimes it's important to have full control of sensor: Check camera properties: To set Auto White Balance and Auto Exposition are required to allow a precise camera control.


Usage 
-----
Your App or Game will receive at "127.0.0.1" by port 12342 (OSC) or UPD (29095)
If sends different desired and selected data

//ResumedBlob

osc:/GameBlob
*	- (float) x pos 	[0..1]
*	- (float) y pos 	[0..1]
*	- (float) UP mov 	[0..1]
*	- (float) DOWN mov	[0..1]


//Tracking Blobs (WIP)
osc:/GameBlob2
*	- (float) x pos 	[0..1]
*	- (float) y pos 	[0..1]
*	- (float) UP mov 	[0..1]
*	- (float) DOWN mov	[0..1]


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
