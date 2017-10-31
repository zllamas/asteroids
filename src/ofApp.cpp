#include "ofApp.h"

bool newAsteroids = false;

//--------------------------------------------------------------
void ofApp::setup() {
	camWidth = 640;
	camHeight = 480;

	vidGrabber.setDeviceID(0);
	vidGrabber.initGrabber(camWidth, camHeight);

	finder.setup("haarcascade_frontalface_default.xml");

	rgb.allocate(camWidth, camHeight);
	hsb.allocate(camWidth, camHeight);
	hue.allocate(camWidth, camHeight);
	sat.allocate(camWidth, camHeight);
	bri.allocate(camWidth, camHeight);
	filter1.allocate(camWidth, camHeight);
	finalImage.allocate(camWidth, camHeight);

	// ---------------------------------------------------------
	dodge = false;
	//create 2 asteroids to start
	for (int i = 0; i < 3; ++i) {
		Asteroid temp;
		asteroids.push_back(temp);
	}


}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		rgb.setFromPixels(vidGrabber.getPixels());
		hsb = rgb;
		hsb.convertRgbToHsv();
		hsb.convertToGrayscalePlanarImages(hue, sat, bri);

		int hueRange = 10;
		for (int i = 0; i < camWidth * camHeight; ++i) {
			filter1.getPixels()[i] = ofInRange(hue.getPixels()[i],
				findHue - hueRange,
				findHue + hueRange) ? 255 : 0;
		}
		finder.findHaarObjects(vidGrabber.getPixels());
	}

	// ---------------------------------------------------------
	
	if (dodge) {
		for (int i = 0; i < asteroids.size(); ++i) {
			//check if it's outside of the canvas, set new goal and go
			if (asteroids[i].position.x + asteroids[i].radius < 0
				|| asteroids[i].position.x + asteroids[i].radius > (int)ofGetWidth
				|| asteroids[i].position.y + asteroids[i].radius < 0
				|| asteroids[i].position.y + asteroids[i].radius > (int)ofGetHeight) {
				asteroids[i].goal = headposition;

			}
			else {
				//rotate , from KavinHan,https://gist.github.com/KavinHan/fe4005e95a6225f6be751210836a85a2
				// http://stackoverflow.com/questions/12516550/openframeworks-rotate-an-image-from-its-center-through-opengl-calls
				//ofPushMatrix();
				//	ofTranslate(leafImg.width / 2, leafImg.height / 2, 0);//move pivot to centre
				//	ofRotate(ofGetFrameNum() * .01, 0, 0, 1);//rotate from centre
				//	ofPushMatrix();
				//		ofTranslate(-leafImg.width / 2, -leafImg.height / 2, 0);//move back by the centre offset
				//		leafImg.draw(0, 0);
				//	ofPopMatrix();
				//ofPopMatrix();

				//translate
				asteroids[i].position.x += asteroids[i].radius;
				asteroids[i].position.y += asteroids[i].radius;
			}

		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	// update asteroids
	for (int i = 0; i < asteroids.size(); ++i) {
		ofDrawCircle(asteroids[i].position, (float)asteroids[i].radius);
	}
	// =========================================================
	vidGrabber.draw(0, 0, camWidth, camHeight);

	ofSetColor(ofColor::lawnGreen);
	ofNoFill();
	for (int i = 0; i < finder.blobs.size(); ++i) {
		ofDrawRectangle(finder.blobs[i].boundingRect);
		vidGrabber.draw(finder.blobs[i].boundingRect.getPosition(), camWidth, camHeight);
		headposition = ofPoint(finder.blobs[i].boundingRect.getPosition());
	}
	// =========================================================

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	dodge = !dodge;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


Asteroid::Asteroid() {
	radius = (int)ofRandom(1,31);
	speed = (int)ofRandom(1,6);
}
