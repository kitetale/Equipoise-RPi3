// 'Equipoise' Kinect-based real-time interactive installation RPi3 code
// Ashley Kim (@kitetale) 
// 4/20/2023

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255); //white
    
    //kinect.setLogLevel(OF_LOG_NOTICE);
    kinect.init();
    kinect.open();
    imitate(imgPx,kinect);
    
    h = kinect.height;
    w = kinect.width;
    
    nearClip = 380; // in mm
    farClip = 700; // in mm
    kinect.setDepthClipping(nearClip,farClip);
    
    colorImage.allocate(w,h);
    grayImage.allocate(w,h);
    grayBg.allocate(w,h);
    grayDiff.allocate(w,h);
    
    prevPx.allocate(w,h,OF_IMAGE_COLOR);
    imgPx.allocate(w,h,OF_IMAGE_COLOR);
    output.allocate(w,h,OF_IMAGE_COLOR);
    
    simplifyTolerance = 3;
    
    viewType = 1;
    contourImg.allocate(w,h);
    grayThreshold = 30;
    learnBg = false;
    
    smoothingSize = 0.0;
    
    cf.setSortBySize(true);
    cfv.setMinAreaRadius(10);
    cfv.setMaxAreaRadius(500);
    cfv.setAutoThreshold(true);
    
	particles = new ParticleSystem();
	particles->setup(w,h,ofPoint(0),20.0f,120.0f);
	// font.load("Montserrat.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if (kinect.isFrameNew()){
        colorImage.setFromPixels(kinect.getPixels());
        grayImage.setFromPixels(kinect.getDepthPixels());
        depthImg.setFromPixels(kinect.getDepthPixels());
        depthImg.mirror(false,true);
        depthImg.update();
        
        absdiff(kinect,prevPx,imgDiff);
        imgDiff.update();
        imgDiff.mirror(false, true);
        copy(kinect,prevPx);
        
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(grayThreshold);
       
             
        //flatten depth a bit for contour
		contourImg = grayImage;
		contourImg.threshold(30);
		
		contourFinder.findContours(contourImg, 30, (w*h), 10, true);
		simplifyContour();
		
		
		cf.findContours(depthImg);
		cfv.findContours(grayDiff);
		contours.clear();
		velContours.clear();
		contours.resize(cf.size());
		velContours.resize(cfv.size());
        
        for(int i=0; i<cf.size(); i++){
			ofPolyline ct;
			ct = cf.getPolyline(i);
			ct = ct.getSmoothed(smoothingSize,0.5); //smoothing size
			contours[i] = ct;
		}
		for(int i=0; i<cfv.size(); i++){
			ofPolyline ctv;
			ctv = cfv.getPolyline(i);
			velContours[i] = ctv;
		}

		particles->update(contours);
    }
    
    
}
//--------------------------------------------------------------
void ofApp::simplifyContour(){
	polys.resize(contourFinder.blobs.size());
	mesh.clear();
	for (int i=0; i<contourFinder.blobs.size(); i++){
		ofPolyline &poly = polys[i];
		poly.clear();
		poly.addVertices(contourFinder.blobs[i].pts);
		poly.simplify(simplifyTolerance);
		mesh.addVertices(poly.getVertices());
		
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255); 
	ofSetColor(255);
	if (viewType == 1){
		ofBackground(255); 
		kinect.draw(0,0,w,h);
    } else if (viewType == 2){
		contourFinder.draw(0,0,w,h);
	} else if (viewType == 3){
		ofBackground(255); 
		contourImg.draw(0,0,w,h);
	} else if (viewType == 4){
		//for(auto poly: polys) poly.draw();
		ofPushMatrix();
		ofSetColor(0);
		mesh.drawWireframe();
		ofSetColor(0,255,0);
		glPointSize(2);
		mesh.drawVertices();
		ofPopMatrix();
	} else if (viewType == 5){
		ofPushStyle();
		ofBackground(255); 
		ofSetColor(0);
		ofSetLineWidth(1.5);
		for (int i=0; i<contours.size(); i++){
			contours[i].draw();
		}


		particles->draw();
		ofPopStyle();
	} else {
		ofBackground(255); 
		ofSetColor(0);
		for (int i = 0; i < kinect.getHeight(); i+=8){
			ofPolyline polyline;
			for (int j = 0; j < kinect.getWidth(); j++){
				ofColor col = imgDiff.getPixels().getColor(j,i);
				int brightness = col.getBrightness();
				polyline.addVertex(j, i+ofMap(brightness, 0, 255, 0, -64));
				//sends y coord up if bright
			}
			polyline = polyline.getSmoothed(10);
			polyline.draw();
		}
	}
	
	// draw debug info
	ofSetColor(0);
	//if(viewType==3) ofSetColor(255);
	stringstream reportStream;
	reportStream << "set near threshold " << nearClip << " (press: + -)" << endl
	<< "set far threshold " << farClip << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl;
	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
	
	ofDrawBitmapString(reportStream.str(), 20, h-30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case OF_KEY_UP:
            if (angle < 30) {
                ++angle;
            }
            kinect.setCameraTiltAngle(angle);
            break;

        case OF_KEY_DOWN:
            if (angle > -30) {
                --angle;
            }
            kinect.setCameraTiltAngle(angle);
            break;
        case '1':
			viewType = 1;
			break;
		case '2':
			viewType = 2;
			break;
        case '3':
			viewType = 3;
			break;
		case '4':
			viewType = 4;
			break;
		case '5':
			viewType = 5;
			break;
		case '>':
		case '.':
			farClip += 10;
			if (farClip > 8000) farClip = 8000;
			break;
			
		case '<':
		case ',':
			farClip -= 10;
			if (farClip < 0) farClip = 0;
			break;
			
		case '+':
		case '=':
			nearClip += 10;
			if (nearClip > 8000) nearClip = 8000;
			break;
			
		case '-':
			nearClip -= 10;
			if (nearClip < 0) nearClip = 0;
			break;
		case 'l':
			if (ledOn){
				kinect.setLed(ofxKinect::LED_OFF);
				ledOn = false;
			} else {
				kinect.setLed(ofxKinect::LED_GREEN);
				ledOn = true;
			}
			break;
        default:
			viewType = 0;
            break;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
