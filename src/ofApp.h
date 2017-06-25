#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Matrix.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		ofVec2f newBySizeAngle(float angle, float dist);

		ofxPanel PainelP1,PainelP2,PainelMain;
		ofxIntSlider angleBaseP1,angleBaseP2,angleTowerP1, angleTowerP2;
		ofxIntSlider speed1, speed2;
		ofxButton FinishMatch;
		const int angleMovement = 30;
		
		bool inMove;
		float currentTime;
		const float MaxTime = 2;
		const int maxSpeedMove = 150;
		const int maxDist = ofGetWidth() / 4;
		struct player {
			ofVec2f position, momentum;
			float angleBase,angleTower;

			bool shotIsTrue;
			ofVec2f bulletPos;
			float distCurrent;
			int score;
		}P1,P2;

		//Variable to Draw
		int w = 20, h = 8, r = 15;
		
		Matrix Player1Transform = Matrix(3, 3);
		Matrix Player1Rotate = Matrix(3, 3);
		Matrix Player1Translate = Matrix(3, 3);
		Matrix Player1Vec3 = Matrix(3, 1);

		Matrix Player2Transform = Matrix(3, 3);
		Matrix Player2Rotate = Matrix(3, 3);
		Matrix Player2Translate = Matrix(3, 3);
		Matrix Player2Vec3 = Matrix(3, 1);

		Matrix WorldMatrix = Matrix(3, 3);

};
