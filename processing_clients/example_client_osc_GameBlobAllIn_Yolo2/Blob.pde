class Blob {
  float xPos;
  float yPos;
  int id; 
  int time; 
  float probability;
  //TODO velx, vely
  float wScale, hScale;

  // Constructor
  Blob(float w, float h) {
    wScale = w;
    hScale = h;
    xPos = -1;
    yPos = -1;
    id = -1; //Id will be the order received or ID from tracking
    time = -1; //time -1 if not tracking
    probability = 0;
    //TODO add Velocity vector
    //TODO add Area info
  }

  // Custom method for updating the variables
  void updateOSC() {
  }

  //------------------------------
  void displayBlobInfo() {
      
      int gapY = 20; 
      text("id-"+str(id), xPos*wScale, yPos*hScale*1);
      text("time-"+str(time), xPos*wScale, yPos*hScale+gapY*2);
      //text("x-"+str(xPos*wScale), xPos*wScale, yPos*hScale+gapY*3);
      //text("y-"+str(yPos*hScale), xPos*wScale, yPos*hScale+gapY*4);  
  }

  // Custom method for drawing the object
  void display() {
    textAlign(LEFT);
    //Draw Received Blob. Probability = quality person detection.
    fill(255, 0, 0, 150);
    stroke(255, 255, 255, 150);
    strokeWeight(2);  // Thicker
    ellipse(xPos*wScale, yPos*hScale, 50*probability, 50*probability);


    fill(200, 0, 0, 250);
    //Draw Info
    
    pushMatrix();
      translate(60, 0, 0);
      displayBlobInfo();
    popMatrix();
  }
}