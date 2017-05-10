/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

float last_OSCvalue0 = -1;//Vars to detect movement from Blobs
float last_OSCvalue1 = -1;
Boolean bOscActive = true;
//Recibimos el area 1 ( Posicion )
float osc1X = 0;
float osc1Y = 0;
//Recibimos el area 2 ( Disparo )
float osc2X = 0;
float osc2Y = 0;

void setup() {
  size(400,400);
  frameRate(25);

  //setup OSC
  oscP5 = new OscP5(this, 12345);
  //myRemoteLocation = new NetAddress("127.0.0.1", 12345);
  
}


void draw() {
  background(0);  
  
  //osc1
  fill(0, 255, 0);
  ellipse(osc1X*width, osc1Y*height, 10, 10);
  
  //osc2?
  //fill(255, 0, 0);
  //ellipse(osc1X*widthWindow, osc1Y*heightWindow, 1, 1);
}

void mousePressed() {

}



//Ejemplo basico leer cualquier OSC message
/* incoming osc message are forwarded to the oscEvent method. 
void oscEvent(OscMessage theOscMessage) {
  // print the address pattern and the typetag of the received OscMessage
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}*/

//----------------------------------------------------
void oscEvent(OscMessage theOscMessage) {

  if (theOscMessage.checkAddrPattern("/GameBlob") == true) {
    if (theOscMessage.checkTypetag("ffff")) {
      float OSCvalue0 = theOscMessage.get(0).floatValue(); // X position [0..1]
      osc1X = OSCvalue0;
      
      float OSCvalue1 = theOscMessage.get(1).floatValue();  // Y position [0..1]
      osc1Y = OSCvalue1;
      
      println(" osc1X: "+OSCvalue0);
      println(" osc1Y: "+OSCvalue1);
    }
  } else if (theOscMessage.checkAddrPattern("/GameBlob2") == true) {

    if (theOscMessage.checkTypetag("ffff")) {
      float OSCvalue0 = theOscMessage.get(0).floatValue(); // X position [0..1]
      float OSCvalue1 = theOscMessage.get(1).floatValue();  // Y position [0..1]
      
      //////////////////////////////////////////
      //Memorizar primer valor para luego poder comparar
      Boolean bInitedOscVars = false;
      if (last_OSCvalue0 == -1) {
        last_OSCvalue0 = OSCvalue0;
      } else bInitedOscVars = true;
      if (last_OSCvalue1 == -1) {//Just init this
        last_OSCvalue1 = OSCvalue1;
      } else bInitedOscVars = true;

      ////////////////////////////////////// 
      float diffBlobY = abs(last_OSCvalue0 - OSCvalue0);

      if (bInitedOscVars) {
        //save last values
        last_OSCvalue1 = OSCvalue1;
        last_OSCvalue0 = OSCvalue0;

        //Detectar Diferencias
        if (diffBlobY > 0.05) {
             println("DISPARA "+diffBlobY);
        }
      }
    }
  }
}