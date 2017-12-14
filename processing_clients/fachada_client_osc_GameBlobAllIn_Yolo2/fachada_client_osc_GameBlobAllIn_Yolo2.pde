/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */


PFont f;
import java.util.concurrent.CopyOnWriteArrayList;
int widthDesiredScale = 192;
int heightDesiredScale = 157;
float scaleRawSize = 0.5;
Boolean bDrawInfo = true;

void setup() {
 
  fullScreen();
  

  frameRate(25);

  // Create the font
  printArray(PFont.list());
  f = createFont("SourceCodePro-Regular.ttf", 24);
  textFont(f);

  setup_clientSensor4Games();
}


void draw() {
  background(0);
  
  stroke(255, 0, 0); //RGB Contour Color. https://processing.org/reference/stroke_.html
  drawFacadeContourInside(); //Facade Contour

  //Text info
  fill(255);
  text("Client example receiving Blobs at localhost port:12345", 0, height-0.05*height);
  text("FrameRate --> "+str(frameRate), 0, height-0.1*height);

pushMatrix();
  translate(40, 40 + 32);
  noFill();
  stroke(255, 0, 0);

  fill(0, 255, 0);
  draw_clientSensor4Games(widthDesiredScale, heightDesiredScale - 32, scaleRawSize, bDrawInfo);
popMatrix();
 
}

//-----------------------------------
void drawFacadeContourInside()
{

  //left line
  line(40, 72, 40, 196);

  //bottom
  line(40, 196, 231, 196);

  //right side
  line(231, 72, 231, 196);

  // steps
  //flat left
  line(40, 72, 76, 72);

  //vert
  line(76, 72, 76, 56);

  // hor
  line(76, 56, 112, 56);

  //vert
  line(112, 56, 112, 40);

  //top
  line(112, 40, 159, 40);

  //vert right side
  line(159, 40, 159, 56);

  //hors
  line(160, 56, 195, 56);

  //  vert
  line(195, 56, 195, 72);

  //hor
  line(196, 72, 231, 72);
}

void mousePressed() {
}


void keyPressed() {

  if (keyCode==BACKSPACE)bDrawInfo = !bDrawInfo;
  
}