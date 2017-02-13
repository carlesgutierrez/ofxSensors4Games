ofxSensors4GamesofxSensors4Games
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

Copy the `ofxSensors4Games` folder into your `openFrameworks/addons/` folder.
Use included Examples for testing: In ofApp.cpp at setup choose your sensor as realTime or Simulation mode.

	
Dependencies
------------
Externals:
* ofxImgui (https://github.com/jvcleave/ofxImGui)

Internals:
* ofxNetwork
* ofxOsc
* ofxOpencv
* //ofxKinect add this manually. !!! README  !!! ofxkinect README (usb libs install) 


Compatibility
------------

Tested with OF 0.9.x on

	Ubuntu (Not tested)
    Mac OS 10.11, Xcode 7 (Not tested)
    Windows 7, QTcreator (Not tested)
    Windows 7/10, Visual Studio 2015

For dedicated camera uses. It's important to have full control of sensor. And  Windows drivers allow auto White Balance and Auto Exposition are required to allow a precise camera control.
In linux or mac, we may use firewire or similar cameras that allow such kind of manual control. 

Usage
-----

For now we can configure 2 different and selectables  areas.

osc:/GameBlob
*	- (float) x pos 	[0..1]
*	- (float) y pos 	[0..1]
*	- (float) UP mov 	[0..1]
*	- (float) DOWN mov	[0..1]
* (TODO)	
* (TODO)	
* (TODO)	
* (TODO)	


osc:/GameBlob2
*	- (float) x pos 	[0..1]
*	- (float) y pos 	[0..1]
*	- (float) UP mov 	[0..1]
*	- (float) DOWN mov	[0..1]
* (TODO)	
* (TODO)	
* (TODO)	
* (TODO)	


 
<!-- TODO 

-->

<!--Known issues
-------------->


<!--Version history
-------------->

<!-- It make sense to include a version history here (newest releases first), describing new features and changes to the addon. Use [git tags](http://learn.github.com/p/tagging.html) to mark release points in your repo, too! -->

<!-- 
### Version 0.1 (Date):
Describe relevant changes etc. -->

License
-------
The MIT License (MIT)

Copyright (c) 2014 Elie Génard

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