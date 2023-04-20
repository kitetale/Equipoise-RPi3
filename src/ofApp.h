// 'Equipoise' Kinect-based real-time interactive installation RPi3 code
// Ashley Kim (@kitetale) 
// 4/20/2023

#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Particle.h"

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void simplifyContour();
    
    
    ofxKinect kinect;
    ofxCvColorImage colorImage;
    ofImage imgDiff;
    ofPixels prevPx;
    ofPixels imgPx;
    ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
    
    ofxCvContourFinder contourFinder;
    ofxCvGrayscaleImage contourImg;
    
    int grayThreshold;
    bool learnBg;
    ofImage output; //ofImage for saving to os
    
    vector<ofPolyline> polys;
    ofMesh mesh;
    
    float simplifyTolerance;
    
    ofTrueTypeFont font;
    
    int h,w;
    int nearClip,farClip;
    int angle;
    
    int viewType;
	bool ledOn;
};
