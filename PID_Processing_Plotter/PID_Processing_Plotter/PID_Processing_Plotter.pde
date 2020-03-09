import processing.serial.*;
Serial myPort;

String myStr;
int screenW = 640;
int screenH = 360;
int spacing = 10;
int xVals = (screenW - 2*spacing)/spacing;
String[] dataArr = new String[3];
int[] yArr1 = new int[xVals];
int[] yArr2 = new int[xVals];
int[] xArr = new int[xVals];
int x1, x2, y1, y2, x3, x4, y3, y4;
int count = 0;

void settings() {
  size(screenW, screenH);
}
void setup() {
  for (int i=0; i<xVals; i++)
    xArr[i] = (i+1)*spacing;
  println("Available serial ports:");
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  frameRate(12);
  drawBg();
}

void drawBg() {
  background(0);
  stroke(0, 255, 0);
  strokeWeight(1);
  line(spacing, (screenH-spacing)/2, screenW-spacing, (screenH-spacing)/2);
  line(spacing, screenH-spacing, spacing, spacing);
}

void drawLines() {
  drawBg();
  for(int i=0; i<count-1; i++)
  {
    stroke(255, 0, 0);
    line(xArr[i], yArr1[i], xArr[i+1], yArr1[i+1]);
    stroke(0, 255, 255);
    line(xArr[i], yArr2[i], xArr[i+1], yArr2[i+1]);
  }
}

void draw()
{
  if ( myPort.available() > 0) 
  { 
    myStr = myPort.readStringUntil('\n');
    if (myStr != null) 
    {
      myStr = myStr.trim();
      dataArr = split(myStr, ",");
      if (count < xVals) {
        count ++;
      } else {
        count = xVals-1;
        for(int i=0; i<yArr1.length-1; i++) {
          yArr1[i] = yArr1[i+1];
          yArr2[i] = yArr2[i+1];
        }
      }
      yArr1[count-1] = Integer.parseInt(dataArr[0])/50+(screenH-spacing)/2;
      yArr2[count-1] = Integer.parseInt(dataArr[1])/50+(screenH-spacing)/2;
      if(count > 1)
          drawLines();
    }
  }
}
