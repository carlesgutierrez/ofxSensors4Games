ofxSensors4Games
============

![ofxSensors4Games](header.png)

Introduction
------------

**!!! WARNING:** this addon uses oF v0.9.3 that is still in development.

This addon provides a interface to control games from sensors ( Kienct, Camera, others...) 



Installation
------------

Just copy the `ofxSensors4Games` folder into your `openFrameworks/addons/` folder.

Usage
-----


Examples
--------

myControllerOscServer sends different desired and selected data

//ResumedBlob
osc:/GameBlob
*	- (float) x pos 	[0..1]
*	- (float) y pos 	[0..1]
*	- (float) UP mov 	[0..1]
*	- (float) DOWN mov	[0..1]
* (TODO)	- (int) Poligon Points	[0..1] //For now just 4 ( Quad )
* (TODO)	- (float) Quad Area x0	[0..1]
* (TODO)	- (float) Quad Area x1	[0..1]
* (TODO)	- (float) Quad Area x2	[0..1]
* (TODO)	- (float) Quad Area x3	[0..1]

//Tracking Blobs (WIP)
osc:/TrackingBlobs
*	- (int) id
*	- (float) pos
*	- (float) vel
*	- (int) detected action
* (TODO)	- (int) Poligon Points	[0..1] //For now just 4 ( Quad )
* (TODO)	- (float) Quad Area x0	[0..1]
* (TODO)	- (float) Quad Area x1	[0..1]
* (TODO)	- (float) Quad Area x2	[0..1]
* (TODO)	- (float) Quad Area x3	[0..1]


Dependencies
------------
Externals:
* ofxImgui (https://github.com/jvcleave/ofxImGui)

Internals:
* ofxNetwork
* ofxOsc
* ofxOpencv
* ofxKinect


Compatibility
------------
This addon uses oF v0.9.3 (still in development in OSX) 


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
