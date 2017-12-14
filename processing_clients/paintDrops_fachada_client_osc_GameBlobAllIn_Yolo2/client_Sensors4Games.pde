import oscP5.*;
import netP5.*;


OscP5 oscP5;
NetAddress myRemoteLocation;

ArrayList<Blob> blobs= new ArrayList<Blob>();


//-------------------------------
void setup_clientSensor4Games() {
  //setup OSC
  oscP5 = new OscP5(this, 12345);
  //myRemoteLocation = new NetAddress("127.0.0.1", 12345);

  blobs.clear();
}

//-----------------------------
void draw_clientSensor4Games(int w, int h, float _scaleRaWBlobSize, Boolean _bDrawInfo) {
  synchronized (blobs) {
    //println("NumBlobs to display" + str(blobs.size()));
    for (Blob auxBlob : blobs) {
      auxBlob.display(w, h, _scaleRaWBlobSize, _bDrawInfo);
    }
  }
}


//----------------------------------------------------
void oscEvent(OscMessage theOscMessage) {
  //Uncomment to Debug OSC messages, this prints the address pattern and the typetag of the received OscMessage
  //print("### received an osc message.");
  //print(" addrpattern: "+theOscMessage.addrPattern());
  //println(" typetag: "+theOscMessage.typetag());

  if (theOscMessage.checkAddrPattern("/GameBlobAllIn") == true) {
    //get how many new blobs are going to be received
    int numBlobs = theOscMessage.get(0).intValue();

    //Prepare a new Array of Blobs
    //clear2NewBlobs(numBlobs);
    synchronized (blobs) {
      blobs.clear();
    }
    //println("PRE Fill numBlobs" + str(numBlobs));

    //Read and save OSC info
    for (int i = 0; i< numBlobs; i++) {
      int nItms = 6; // X items per received pakage. 
      //if (theOscMessage.checkTypetag("ffff")) {
      float posBlobX = theOscMessage.get(1+i*nItms+0).floatValue(); // X position [0..1]
      float posBlobY = theOscMessage.get(1+i*nItms+1).floatValue();  // Y position [0..1]
      float sizeBlobW = theOscMessage.get(1+i*nItms+2).floatValue(); // X position [0..1]
      float sizeBlobH = theOscMessage.get(1+i*nItms+3).floatValue();  // Y position [0..1] 
      int idBlob     = theOscMessage.get(1+i*nItms+4).intValue();
      int timeBlob   = theOscMessage.get(1+i*nItms+5).intValue();
      //float probBlob = theOscMessage.get(1+i*nItms+6).floatValue();

      //println("PRE blob("+str(idBlob)+") receive["+str(i)+"] x="+str(posBlobX)+" y="+str(posBlobY));

      //Save this in the new Array of Blob
      Blob auxBlob = new Blob();
      auxBlob.xPos = posBlobX;
      auxBlob.yPos = posBlobY;
      auxBlob.wRawBlob = sizeBlobW;
      auxBlob.hRawBlob = sizeBlobH;
      auxBlob.id = idBlob;
      auxBlob.time = timeBlob;
      //auxBlob.probability = probBlob; 

      synchronized (blobs) {
        blobs.add(auxBlob);
      }

      //println("POST blobs["+str(i)+"] x="+str(blobs.get(i).xPos)+" y="+str(blobs.get(i).yPos));
    }
  }
}