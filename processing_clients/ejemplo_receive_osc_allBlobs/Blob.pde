class Blob {
  float xPos;
  float yPos;
  int id; 
  int time; 
  //TODO velx, vely
  int wScale, hScale;
  
  // Constructor
  Blob(int w, int h) {
    wScale = w;
    hScale = h;
    xPos = -1;
    yPos = -1;
    id = -1; //Id will be the order received or ID from tracking
    time = -1; //time -1 if not tracking
  }
  
  // Custom method for updating the variables
  void updateOSC() {

  }
  
  //------------------------------
  void displayBlobInfo(){
    fill(255);
    int gapY = 20; 
    text("id-"+str(id), xPos*wScale, yPos*hScale*1);
    text("time-"+str(time), xPos*wScale, yPos*hScale+gapY*2);
    text("x-"+str(xPos*wScale), xPos*wScale, yPos*hScale+gapY*3);
    text("y-"+str(yPos*hScale), xPos*wScale, yPos*hScale+gapY*4);
  }
  
  // Custom method for drawing the object
  void display() {
   
    ellipse(xPos*wScale, yPos*hScale, 6, 6);
    
    //Draw text
    textAlign(CENTER);
    displayBlobInfo();
  }
  

}