class Blob {
  float xPos;
  float yPos;
  int id; 
  int time; 
  //float probability;
  float wRawBlob;
  float hRawBlob;
  //TODO velx, vely

  // Constructor
  Blob() {
    xPos = -1;
    yPos = -1;
    id = -1; //Id will be the order received or ID from tracking
    time = -1; //time -1 if not tracking
    //probability = 0;
    //TODO add Velocity vector
    //TODO add Area info
  }

  // Custom method for updating the variables
  void updateOSC() {
  }

  //------------------------------
  void displayBlobInfo(int w, int h) {
    int deltaX = -40;
    int deltaY = -30; 
    text("["+str(id)+"]"+"("+str(time)+")", xPos*w+deltaX, yPos*h*1+deltaY);
  }

  // Custom method for drawing the object
  void display(int w, int h, float _scaleRawDims, Boolean _bDrawInfo) {


    textAlign(LEFT);
    //Draw Received Blob. Probability = quality person detection.
    int idColor = id*100 % 255;
    fill(idColor, 255, 255, 200);
    //stroke(255, 255, 255, 255);
    noStroke();
    strokeWeight(2);  // Thicker
    if (_scaleRawDims>0){
      rectMode(CENTER);  // Set rectMode to CENTER
      rect(xPos*w, yPos*h, wRawBlob*_scaleRawDims, hRawBlob*_scaleRawDims);
    }
    else ellipse(xPos*w, yPos*h, 50, 50);


    //Draw Info
    if (_bDrawInfo) {
      fill(0, 200, 0, 250);
      pushMatrix();
      translate(60, 0, 0);
      displayBlobInfo(w, h);
      popMatrix();
    }
  }
}