#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

float trackerresults;
//--------------------------------------------------------------
void ofApp::setup() {
	welcomeImage.load("welcomeScreen.png");
	// =========================================================
	// set up FaceTracker and camera
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);

	tracker.setup();
	tracker.setRescale(.5);
	// =========================================================

	// ---------------------------------------------------------
	//create asteroids to start
	/*for (int i = 0; i < 20; ++i) {
		Asteroid temp;
		asteroids.push_back(temp);
		temp.asteroid_pic.load("asteroid01.png");
	}*/
	// ---------------------------------------------------------

	// =========================================================
	// init assign booleans and progress counter 
	attack = false;
	isJawOpen = false;
	welcomeScreen = true;
	currentProgress = 0;
	// =========================================================

	// set up enemies
	Enemy t1(ofPoint(0, 0));
	Enemy t2(ofPoint(ofGetWidth(), ofGetHeight()));

	enemies.push_back(t1);
	enemies.push_back(t2);
}

//--------------------------------------------------------------
void ofApp::update() {
	// ---------------------------------------------------------
	//
	//if (dodge) {
	//	for (int i = 0; i < asteroids.size(); ++i) {
	//		//check if it's outside of the canvas, set new goal and go
	//		if (asteroids[i].position.x + asteroids[i].radius < 0
	//			|| asteroids[i].position.x + asteroids[i].radius > (int)ofGetWidth
	//			|| asteroids[i].position.y + asteroids[i].radius < 0
	//			|| asteroids[i].position.y + asteroids[i].radius > (int)ofGetHeight) {
	//			asteroids[i].goal = headposition;

	//		}
	//		else {
	//			//rotate , from KavinHan,https://gist.github.com/KavinHan/fe4005e95a6225f6be751210836a85a2
	//			// http://stackoverflow.com/questions/12516550/openframeworks-rotate-an-image-from-its-center-through-opengl-calls
	//			//ofPushMatrix();
	//			//	ofTranslate(asteroids[]..width / 2, leafImg.height / 2, 0);//move pivot to centre
	//			//	ofRotate(ofGetFrameNum() * .01, 0, 0, 1);//rotate from centre
	//			//	ofPushMatrix();
	//			//		ofTranslate(-leafImg.width / 2, -leafImg.height / 2, 0);//move back by the centre offset
	//			//		leafImg.draw(0, 0);
	//			//	ofPopMatrix();
	//			//ofPopMatrix();

	//			//translate
	//			asteroids[i].position.x += asteroids[i].radius*asteroids[i].speed;
	//			asteroids[i].position.y += asteroids[i].radius*asteroids[i].speed;
	//		}
	//	}
	//}

	// =========================================================
	//update camera
	cam.update();
	if (cam.isFrameNew()) {
		if (tracker.update(toCv(cam))) {
			classifier.classify(tracker);
		}
	}
	// =========================================================

	// =========================================================
	//update asteroid position
	//for (int i = 0; i < asteroids.size(); ++i) {
	//	asteroids[i].position.x += asteroids[i].speed;
	//	asteroids[i].position.y += asteroids[i].speed;

	//	if (asteroids[i].position.x > ofGetWidth()+ asteroids[i].radius &&
	//		asteroids[i].position.y > ofGetHeight() + asteroids[i].radius) {
	//		asteroids[i].position = asteroids[i].start;
	//		asteroids[i].speed = (int)ofRandom(1, 5);
	//	}
	//}
	// =========================================================

	// =========================================================
	//update facetracking ie. jaw open --> progress bar
	float openness = tracker.getGesture(ofxFaceTracker::JAW_OPENNESS); 
	headposition = ofPoint(tracker.getPosition()[0], tracker.getPosition()[1]);
	//cout<< openness <<endl;
	//<22 closed <26 open
	if (openness >= 25 && isJawOpen == false) {
		isJawOpen = true;
		//cout << "JAW OPEN" << endl;
		ofResetElapsedTimeCounter();
		lastSecond = floor(ofGetElapsedTimeMillis());
		if (!attack) {
			attack = true;
		}
	}
	if (openness < 25) {
		isJawOpen = false;
		//cout << "JAW CLOSED" << endl;
	}

	if (isJawOpen) {
		//cout << "current secs: " << floor(ofGetElapsedTimeMillis()) << " lasts secs: " << lastSecond << endl;
		if (floor(ofGetElapsedTimeMillis()) >= lastSecond + 1 && progress.width < progress.goal) {
			//update time passed with mouth open
			currentProgress += 1;
			lastSecond = floor(ofGetElapsedTimeMillis());

			//update bar
			progress.width = 200 * (currentProgress/250);
			//cout << progress.width << endl;

		}
	}
	// =========================================================

	// =========================================================
	
	if (progress.width > 200 *.75) { //2/4
		enemies[1].attack();
	}
	else if (progress.width > 200 * .50) { //1/4
		enemies[0].attack();
	}
	
	
	
	

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(ofColor::black);
	ofSetColor(255);
	if(welcomeScreen){
		welcomeImage.draw(14,20);
	}
	else {
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		//draw cam
		//cam.draw(0, 0);
		//tracker.draw();
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

		// =========================================================
		// draw asteroids
		for (int i = 0; i < asteroids.size(); ++i) {
			//asteroids[i].asteroid_pic.draw(asteroids[i].position.x, asteroids[i].position.y);
			ofDrawCircle(asteroids[i].position, (float)asteroids[i].radius);
		}
		// =========================================================

		
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		//draw face
		ofSetColor(ofColor::greenYellow);
		tracker.getImageMesh().draw();
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

		// =========================================================
		if (attack) {
			for (int i = 0; i < enemies.size(); ++i) {
				enemies[i].draw();
				for(int j=0; j<enemies[i].ammo.size(); ++j){
					ofDrawCircle(enemies[i].ammo[j], 10);
					enemies[i].ammo[j].x = enemies[i].ammo[j].x + 10;
					enemies[i].ammo[j].y = enemies[i].ammo[j].y + 10;
				}

			}
		}
		// =========================================================
		
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		//draw ProgressBar
		progress.draw();
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	welcomeScreen = !welcomeScreen;
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
	speed = (int)ofRandom(1,5);

	bool side = ofRandom(0, 2);
	if (side == 1) {
		position = ofPoint((radius) * -1, ofRandom(0, ofGetHeight()));
	}
	else {
		position = ofPoint(ofRandom(10, ofGetWidth()), (radius) * -1);
	}
	start = position;

}

ProgressBar::ProgressBar() {
	start = ofPoint(ofGetWidth()-220,20);
	goal = 200;
	width = 0;
	height = 20;
	color = ofColor::red;
}

ProgressBar::ProgressBar(string health) {
	start = ofPoint(20, 20);
	goal = 200;
	width = 0;
	height = 20;
	color = ofColor::red;
}

void ProgressBar::draw() {
	//border
	if (width == goal) {
		color = ofColor::green;
	}
	ofSetColor(color);
	ofNoFill();
	ofDrawRectangle(ofPoint(start.x-3,start.y-3), goal+6, height+6);

	//actual bar
	ofFill();
	ofDrawRectangle(start, width, height);

}


Enemy::Enemy() {

}
Enemy::Enemy(ofPoint l) {
	location = l;
	if (l.x > 0) {
		direction = 1;
	}
	else {
		direction = -1;
	}
}

void Enemy::draw() {
	if (isAttacking) {

		ofSetColor(ofColor::orangeRed);
		if (location.y > ofGetHeight()) {
			direction = -1;
		}
		if (location.y < 0) {
			direction = 1;
		}

		location.y = location.y + (ofRandom(3,9) * direction);

		ofDrawCircle(location, 100);
	}
}

void Enemy::attack() {
	isAttacking = true;
	for (int i = 0; i < (int)ofRandom(6, 25); ++i) {
		ammo.push_back(location);
	}
}
