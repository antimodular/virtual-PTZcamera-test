/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for rotating a sphere as an arcball
 * Dragging the mouse up down left right to apply an intuitive rotation to an object.
 *
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"

float maxSphereImgWidth;

float panValue,tiltValue,zoomValue;
float panIncr;


//http://vterrain.org/Textures/spherical.html

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(70, 70, 70);
    ofEnableSmoothing();
    glEnable(GL_DEPTH_TEST);
    
    sixt.loadImage("16-9.png");
    
    iMainCamera = 0;
    
    
    // user camera
    //	camEasyCam.setTarget(nodeSwarm);
    camEasyCam.setDistance(500);
    cameras[0] = &camEasyCam;
    
    
    // front
    camFront.scale = 100;
    
    cameras[1] = &camFront;
    
    // top
    camTop.scale = 100;
    camTop.tilt(-90);
    cameras[2] = &camTop;
    
    // left
    camLeft.scale = 100;
    camLeft.pan(-90);
    cameras[3] = &camLeft;
    
    
    //test cam
    //camInObject.setDistance(0); //.scale = 10; //
    
    // camInObject.setForceAspectRatio(true);
    camInObject.setAspectRatio(1.7778);
    camInObject.setFov(40);
    cameras[4] = &camInObject;
    
    //since we're going to be accessing
    //the matricies for this camera
    //every frame, let's cache them
    //for speed
    for (int i=0; i<N_CAMERAS; i++)
        // cameras[i]->cacheMatrices();
        
        //
        /////////////////////
        
        
        
        originPoint = ofVec3f(0,0,0);
    /////////////////////
    // DEFINE VIEWPORTS
    /////////////////////
    //
    setupViewports();
    //
    /////////////////////
    
    
    drawGrid = false;
    
    sphereDiameter = 100;
    //myPlane.init();
}


void ofApp::setupViewports()
{
    //call here whenever we resize the window
    
    /////////////////////
    // DEFINE VIEWPORTS
    /////////////////////
    //
    //float xOffset = ofGetWidth()/3;
    //	float yOffset = ofGetHeight()/N_CAMERAS;
    
    //float xOffset = 0;
    //float yOffset = N_CAMERAS;
    
    
    viewMain.x = 0;
    viewMain.y = 0;
    viewMain.width = ofGetWidth();
    viewMain.height = ofGetHeight();
    
    viewOnObject.x=0;
    viewOnObject.y = 0;
    viewOnObject.width = 400;
    viewOnObject.height = 800;
    
    
    
    float yOffset = ofGetHeight() / N_CAMERAS;
    float xOffset = yOffset * 1.77778;
    
    for(int i = 0; i < N_CAMERAS; i++){
        
        viewGrid[i].x = 0;
        viewGrid[i].y = yOffset * i;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    
    panValue +=1;
    if(panValue > 360) panValue = 0;
    
    tiltValue = 0;
    
    ofQuaternion latRot, longRot; //, spinQuat;
    latRot.makeRotate(tiltValue, 1, 0, 0);
    longRot.makeRotate(panValue, 0, 1, 0);
    ofVec3f center = ofVec3f(0,0,sphereDiameter);
    
    //multiplying a quat with another quat combines their rotations into one quat
    //multiplying a quat to a vector applies the quat's rotation to that vector
    //so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
    ofVec3f temp_worldPoint = latRot * longRot * center;
    
    lookAtNode.setPosition(temp_worldPoint);
    camInObject.lookAt(lookAtNode,ofVec3f(0,1,0));
    lookAtNode.lookAt(originPoint);
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("test", 10, 10);
    
    
    
    //////////////////////////
    // BACKGROUND HIGHLIGHT
    //////////////////////////
    //
    glDisable(GL_DEPTH_TEST);
    ofPushStyle();
    ofSetColor(100, 100, 100);
    ofRect(viewMain);
    ofPopStyle();
    glEnable(GL_DEPTH_TEST);
    //
    //////////////////////////
    
    
    
    //////////////////////////
    // DRAW ALL VIEWPORTS
    //////////////////////////
    //
    
   //needs to be called otherwise what camera see does not get drawn
    //later when we call inverseCameraMatrix
    cameras[4]->begin(viewOnObject); //viewMain);
    //drawScene(iMainCamera);
    cameras[4]->end();

    //draw main viewport
    cameras[iMainCamera]->begin(viewMain);
    drawScene(iMainCamera);
    
    cameras[iMainCamera]->end();
    
   
    // draw side viewports
    for(int i = 0; i < N_CAMERAS; i++){
        ofSetColor(255);
        cameras[i]->begin(viewGrid[i]);
        drawScene(i);
        cameras[i]->end();
    }

    
    //////////////////////////
    // DRAW STUFF ON TOP
    //////////////////////////
    //
    ofPushStyle();
    glDepthFunc(GL_ALWAYS); // draw on top of everything
    
    //draw some labels
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
    ofDrawBitmapString("Camera selected: " + ofToString(iMainCamera+1), viewMain.x + 20, 50);
    ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
    ofDrawBitmapString("Press 'p' to toggle parents on OrthoCamera's", viewMain.x + 20, 90);
    
    //draw outlines on views
    ofSetLineWidth(1);
    ofNoFill();
    ofSetColor(255, 255, 255);
    for (int i=0; i<N_CAMERAS; i++)
    {
        ofRect(viewGrid[i]);
    }
    ofRect(viewMain);
    
    glDepthFunc(GL_LESS);
    ofPopStyle();

}

void ofApp::drawScene(int iCameraDraw){
    
    
    
    //draw grid
    if(drawGrid){
        ofPushMatrix();
        ofRotate(90, 1, 0, 0);
        nodeGrid.draw();
        ofPopMatrix();
    }
    
    //draw line betwen camera and lookAt point
    ofSetColor(0, 255, 255);
    ofVec3f v1 = camInObject.getGlobalPosition();
    ofVec3f v2 = lookAtNode.getGlobalPosition();
    ofLine(v1,v2);
    
    ofSetColor(0,255,255);
    
    //draw a cube where camera looks at
    lookAtNode.draw();
    
    //draw a cube at camera location
    camInObject.draw();
    
    //draw big sphere
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofSetColor(50);
    ofNoFill();
    ofDrawSphere(0, 0, 0, sphereDiameter);
    ofFill();
    ofPopMatrix();
    
    
    //draw the image and place it right infront in camera view
    ofPushMatrix();

    ofQuaternion lookAtQ = lookAtNode.getGlobalOrientation();
    float x,y,z;
    float angle;
    ofVec3f translateVec;
    lookAtQ.getRotate(angle,x,y,z);

    ofTranslate(v2.x,v2.y,v2.z);
    ofRotate(angle,x,y,z);
     ofScale(0.1, 0.1);
    ofTranslate(-sixt.getWidth()/2, -sixt.getHeight()/2);
   
    ofSetColor(255);
    sixt.draw(0,0,0);
    ofPopMatrix();

    
    
    //////////////////////////////////
    // DRAW EASYCAM FRUSTUM PREVIEW
    //////////////////////////////////
 
    //let's not draw the camera
    //if we're looking through it
    //if (iCameraDraw != 4){
        
        //  ofSetColor(255, 0, 0);
        //  ofLine(camInObject.getPosition(), camInObject.getLookAtDir());
        
        ofPushStyle();
        
        //in 'camera space' this frustum
        //is defined by a box with bounds
        //-1->1 in each axis
        //
        //to convert from camera to world
        //space, we multiply by the inverse
        //matrix of the camera
        //
        //by applying this transformation
        //our box in camera space is
        //transformed into a frustum in
        //world space.
        
        ofMatrix4x4 inverseCameraMatrix;
        //ofMatrix4x4 correctCameraMatrix = camEasyCam.getModelViewProjectionMatrix( (iMainCamera == 0 ? viewMain : viewGrid[0]) );
        
        //the camera's matricies are dependant on
        //the aspect ratio of the viewport
        //so we must send the viewport if it's not
        //the same as fullscreen
        //
        //watch the aspect ratio of preview camera
        //inverseCameraMatrix.makeInvertOf(camEasyCam.getModelViewProjectionMatrix());
        
        inverseCameraMatrix.makeInvertOf(camInObject.getModelViewProjectionMatrix());
        
        
        
        // By default, we can say
        //	'we are drawing in world space'
        //
        // The camera matrix performs
        //	world->camera
        //
        // The inverse camera matrix performs
        //	camera->world
        //
        // Our box is in camera space, if we
        //	want to draw that into world space
        //	we have to apply the camera->world
        //	transformation.
        //
        
        ofPushMatrix();
        //glMultMatrixf(correctCameraMatrix.getPtr()); //
        glMultMatrixf(inverseCameraMatrix.getPtr());
        
        
        
        //--
        // Draw box in camera space
        // (i.e. frustum in world space)
        
        ofNoFill();
        // i.e. a box -1, -1, -1 to +1, +1, +1
        ofSetColor(255, 100, 100);
        ofDrawBox(0, 0, 0, 1.8f);
        
        /*
         //draw image always in same direction as camera looks
         //draw it in the camera's projection space
         ofSetColor(255);
         float dd = ofMap(mouseX, 0, ofGetWidth(), -1, 1);
         sixt.draw(-1,-1,dd,2,2);
         */
        ofPopStyle();
        ofPopMatrix();
   // }
    
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key >= '1' && key <= '5')
        iMainCamera = key - '1';
    
    if (key == 'f')
        ofToggleFullscreen();
    
    
    if(key == 'g'){
        drawGrid =! drawGrid;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    setupViewports();
}


