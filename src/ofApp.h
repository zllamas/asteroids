#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvHaarFinder.h"
#include "ofxFaceTracker.h"


struct Asteroid {
	Asteroid();
	
	ofPoint position, start;
	int radius, speed;
	ofImage asteroid_pic;
};

struct ProgressBar {
	ProgressBar();
	
	void draw();
	
	ofPoint start;
	float goal;
	float width;
	float height;
	ofColor color;
};


//convert cam to ofimage and then crop
//toOF(cam,img)
//img.crop ir drawsubsection
//find if has bounding box of face
//mask it inside an oval?

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	/*int camWidth, camHeight;

	ofVideoGrabber vidGrabber;

	ofxCvHaarFinder finder;

	ofxCvColorImage rgb, hsb;
	ofxCvGrayscaleImage hue, sat, bri, filter1, finalImage;

	ofxCvContourFinder   contours;
	int findHue, findSat;*/

	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ExpressionClassifier classifier;


	//-----------------------------------------------------

	vector<Asteroid> asteroids;
	int timer;
	ofPoint headposition;

	bool attack;
	bool isJawOpen;

	float currentProgress;
	int lastSecond;

	ProgressBar progress;
};

//activate alien attack
//figure outhow to get lazer beams to target you
//firgure out how lazer beams will hit you
//draw graphics: helpmet, enemyspaceships, your spaceship, asteroids/spacerocks, text, resuce ship?

