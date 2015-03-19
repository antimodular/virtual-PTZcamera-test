#pragma once

#include "ofMain.h"
#include "OrthoCamera.h"
#include "Grid.h"



#define N_CAMERAS 5

class ofApp : public ofBaseApp{

    
public:
    void setup();
    void update();
    void draw();
    
    void setupViewports();
    void drawScene(int iCameraDraw);

    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    
    //cameras (all these inherit from ofCamera)
    ofEasyCam				camEasyCam;
    orthoCamera				camFront;
    orthoCamera				camTop;
    orthoCamera				camLeft;
    ofCamera camInObject;
    

    //camera pointers
    ofCamera*cameras[N_CAMERAS];
    int						iMainCamera;
    
    //viewports
    ofRectangle				viewMain;
    ofRectangle viewOnObject;
    ofRectangle viewGrid[N_CAMERAS];

    grid					nodeGrid;

    
//    float xCnt;
//    float curAngle;
    
    bool drawGrid;
  //  bool moveSphere;
  //  ofVec3f mousePos;
    
    
    
    ofVec3f iSectPoint;
    
    //ofSphere(mousePos.x,mousePos.y,mousePos.z, 0.3);
    ofVec3f originPoint;
    ofVec3f farPoint;
    ofNode farNode;
    ofNode cameNode;
    ofNode lookAtNode;
    
    ofVec3f trackPoint; // = ofVec3f(ofGetMouseX(),ofGetMouseY(),0);
    
    float pendulumAngle,lookAtAngle;
    
    float sphereDiameter;
    
    ofImage sixt;
 
};
