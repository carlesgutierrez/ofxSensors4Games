/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */


PFont f;
import java.util.concurrent.CopyOnWriteArrayList;


void setup() {
  size(800, 800);

  //pixelDensity(2);
  //println(width, height);
  //println(pixelWidth, pixelHeight);

  frameRate(25);

  // Create the font
  printArray(PFont.list());
  f = createFont("SourceCodePro-Regular.ttf", 24);
  textFont(f);

  setup_clientSensor4Games();
}


void draw() {
  background(0);

  fill(255);
  text("Client example receiving Blobs at localhost port:12345", 0, height-0.05*height);
  text("FrameRate --> "+str(frameRate), 0, height-0.1*height);

  noFill();
  stroke(255, 0, 0);

  fill(0, 255, 0);
  draw_clientSensor4Games();
}

void mousePressed() {
}