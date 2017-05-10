/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

Blob[] blobs;

PFont f;

void setup() {
  size(800, 800);
  
  //pixelDensity(2);
  //println(width, height);
  //println(pixelWidth, pixelHeight);
  
  frameRate(25);

  //setup OSC
  oscP5 = new OscP5(this, 12345);
  //myRemoteLocation = new NetAddress("127.0.0.1", 12345);
  
  // Create the font
  //printArray(PFont.list());
  f = createFont("SourceCodePro-Regular.ttf", 24);
  textFont(f);
  
  //first time 0 elements
  clear2NewBlobs(0);
}


void draw() {
  background(0);
  
  for (Blob auxBlob : blobs) {
    auxBlob.display();
  }
  
  text("FrameRate-"+str(frameRate), 0, 10);
}

void mousePressed() {
}

//---------------------------------------------------
void clear2NewBlobs(int numNewBlobs) {

  //blobs = new Blob[0];//There is no clear funct Array?
  blobs = new Blob[numNewBlobs];
  println("numNewBlobs="+str(numNewBlobs));
  for (int i = 0; i < numNewBlobs; i++) {
    blobs[i] = new Blob(width, height);//setting here the display proportions
  }
}

//----------------------------------------------------
void oscEvent(OscMessage theOscMessage) {
  //Uncomment to Debug OSC messages, this prints the address pattern and the typetag of the received OscMessage
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());

  if (theOscMessage.checkAddrPattern("/GameBlobAllIn") == true) {
    //get how many new blobs are going to be received
    int numBlobs = theOscMessage.get(0).intValue(); //<>//
    
    //Prepare a new Array of Blobs
    clear2NewBlobs(numBlobs);
    
    //Read and save OSC info
    for (int i = 0; i< numBlobs; i++) {
      //if (theOscMessage.checkTypetag("ffff")) {
      float posBlobX = theOscMessage.get(1+i*4+0).floatValue(); // X position [0..1]
      float posBlobY = theOscMessage.get(1+i*4+1).floatValue();  // Y position [0..1]
      int idBlob = theOscMessage.get(1+i*4+2).intValue();
      int timeBlob = theOscMessage.get(1+i*4+3).intValue(); //<>//

      println("receive["+str(i)+"] x="+str(posBlobX)+" y="+str(posBlobY));

      //Save this in the new Array of Blob
      blobs[i].xPos = posBlobX;
      blobs[i].yPos = posBlobY;
      blobs[i].id = idBlob;
      blobs[i].time = timeBlob;
      
      println("blobs["+str(i)+"] x="+str(blobs[i].xPos)+" y="+str(blobs[i].yPos));
    }
  }
}