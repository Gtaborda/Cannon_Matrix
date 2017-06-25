#include "ofApp.h"

ofVec2f ofApp::newBySizeAngle(float angle, float dist) {
	return ofVec2f({ cos(angle)*dist,
					sin(angle)*dist});
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor(20, 20, 20));
	//SetPainel
	//P1
	PainelP1.setup("Player1");
	PainelP1.setName("Player1");
	PainelP1.add(angleBaseP1.setup("Angle", angleMovement - angleMovement, -angleMovement, angleMovement));
	PainelP1.add(angleTowerP1.setup("Angle", angleMovement - angleMovement, -angleMovement, angleMovement));
	PainelP1.add(speed1.setup("Speed",0,0, maxSpeedMove));

	speed1.setName("Speed Player");
	angleBaseP1.setName("Angle Base");
	angleTowerP1.setName("Angle Tower");

	angleBaseP1.setBackgroundColor(ofColor(100, 0, 0));
	angleBaseP1.setFillColor(ofColor(255, 0, 0));	
	angleTowerP1.setBackgroundColor(ofColor(100, 0, 0));
	angleTowerP1.setFillColor(ofColor(255, 0, 0));
	speed1.setBackgroundColor(ofColor(100, 0, 0));
	speed1.setFillColor(ofColor(255, 0, 0));
	//P2
	PainelP2.setup("Player2");
	PainelP2.setName("Player2");
	PainelP2.add(angleBaseP2.setup("Angle", 0, -angleMovement, angleMovement));
	PainelP2.add(angleTowerP2.setup("Angle", 0, -angleMovement, angleMovement));
	PainelP2.add(speed2.setup("Speed", 0, 0, maxSpeedMove));

	angleBaseP2.setBackgroundColor(ofColor(0, 0, 100));
	angleBaseP2.setFillColor(ofColor(0, 0, 255));
	speed2.setBackgroundColor(ofColor(0, 0, 100));
	speed2.setFillColor(ofColor(0, 0, 255));
	angleTowerP2.setBackgroundColor(ofColor(0, 0, 100));
	angleTowerP2.setFillColor(ofColor(0, 0, 255));
	
	speed2.setName("Speed Player");
	angleBaseP2.setName("Angle Base");
	angleTowerP2.setName("Angle Tower");

	//PainelMain
	PainelMain.setup("Main");
	PainelMain.setName("Game Controller");
	PainelMain.add(FinishMatch.setup("next Turn"));
	FinishMatch.setName("Next Turn");
	P1.score = 1;
	P2.score = 4;

	////World Matrix
	WorldMatrix = Translate3X3(WorldMatrix, ofGetWidth() / 2, ofGetHeight() / 2);

	Player1Translate = Translate3X3(Player1Translate, -250, 0);
	Player1Translate = Player1Translate * WorldMatrix;
	Player1Rotate = Rotate3X3(Player1Rotate, (0*PI / 180));

	Player1Transform = Player1Transform * Player1Translate *Player1Rotate;
	Player1Vec3 = Transform(Player1Transform, P1.position.x, P1.position.y);
	P1.position.x = Player1Vec3.get(1, 1);
	P1.position.y = Player1Vec3.get(2, 1);

	Player2Translate = Translate3X3(Player2Translate, 250, 0);
	Player2Translate = Player2Translate * WorldMatrix;
	Player2Rotate = Rotate3X3(Player2Rotate, (180*PI / 180));

	Player2Transform = Player2Transform * Player2Translate *Player2Rotate;
	Player2Vec3 = Transform(Player2Transform, P2.position.x, P2.position.y);
	P2.position.x = Player2Vec3.get(1, 1);
	P2.position.y = Player2Vec3.get(2, 1);

}

//--------------------------------------------------------------
void ofApp::update(){
	double time = ofGetLastFrameTime();
	PainelP1.setPosition(15, 15);
	PainelP2.setPosition(ofGetWidth()-PainelP2.getWidth()-15,15);
	PainelMain.setPosition(ofGetWidth() / 2 - PainelMain.getWidth()/2, 15);

	if (FinishMatch) {
		PainelP1.clear();
		PainelP2.clear();

		P1.angleBase = angleBaseP1;
		P1.angleTower = angleTowerP1;
		PainelP1.setup("Player1");
		PainelP1.setName("Player1");
		PainelP1.add(angleBaseP1.setup("AngleBase", P1.angleBase, P1.angleBase -angleMovement , angleMovement + P1.angleBase));
		PainelP1.add(angleTowerP1.setup("AngleTower", P1.angleTower, P1.angleTower - angleMovement, P1.angleTower + angleMovement));
		PainelP1.add(speed1.setup("Speed", speed1, 0, maxSpeedMove));

		P2.angleBase = angleBaseP2;
		P2.angleTower = angleTowerP2;
		PainelP2.setup("Player2");
		PainelP2.setName("Player2");
		PainelP2.add(angleBaseP2.setup("AngleBase", P2.angleBase, P2.angleBase-angleMovement, P2.angleBase+angleMovement));
		PainelP2.add(angleTowerP2.setup("AngleTower", P2.angleTower, P2.angleTower - angleMovement, P2.angleTower + angleMovement));
		PainelP2.add(speed2.setup("Speed", speed2, 0, maxSpeedMove));

		inMove = true;
	}
	//Turn Battle
	if (inMove) {
		if (currentTime < MaxTime) {
			currentTime += time;

			//PLAYER1
			float Rad1 = (PI / 180)*(angleBaseP1);
			Matrix Player1Momentum = Matrix(3, 3);
			Player1Momentum = Rotate3X3(Player1Momentum, Rad1);

			P1.position += ofVec2f(Player1Momentum(1,1),Player1Momentum(2,1)) * speed1 * time;


			//PLAYER2
			float Rad2 = (PI / 180)*(angleBaseP2);
			Matrix Player2Momentum = Matrix(3, 3);
			Player2Momentum = Rotate3X3(Player2Momentum, Rad2);
			P2.position -= ofVec2f(Player2Momentum(1, 1), Player2Momentum(2, 1))* speed2 * time;

		}
		else {
			currentTime = 0;
			inMove = false;
			P1.shotIsTrue = true;
			P2.shotIsTrue = true;
		}
	}

	//set Move Bullet
	if (P1.shotIsTrue) {
		if (P1.distCurrent < maxDist) {
			P1.distCurrent += 1*time*maxDist;
			float rad = (PI / 180)*(angleTowerP1+ angleBaseP1);
			Matrix momentum = Matrix(3, 3);
			momentum = Rotate3X3(momentum, rad);
			P1.bulletPos += ofVec2f(momentum(1, 1), momentum(2, 1))* maxDist * time;
			if (P1.bulletPos.distance(P2.position) < r) {
				ofSystemAlertDialog("Jogador 1 venceu!");
				ofAppGlutWindow close;
				close.close();

			}
		}
		else {
			P1.distCurrent = 0;
			P1.shotIsTrue = false;
		}
	}
	if (P2.shotIsTrue) {
		ofVec2f newPos = P2.bulletPos - P2.position;

		if (P2.distCurrent < maxDist) {
			P2.distCurrent += 1 * time*maxDist;
			float Rad = (PI / 180)*(angleTowerP2+angleBaseP2);
			Matrix momentum = Matrix(3, 3);
			momentum = Rotate3X3(momentum, Rad);
			P2.bulletPos -= ofVec2f(momentum(1, 1), momentum(2, 1))* maxDist * time;

			if (P2.bulletPos.distance(P1.position) < r ) {
				ofSystemAlertDialog("Jogador 2 venceu!");
				ofAppGlutWindow close;
				close.close();

			}
		}
		else {
			
			P2.distCurrent = 0;
			P2.shotIsTrue = false;
		}
	}
	//Set Low, Medium or Fast Speed In Name
	if (speed1 > 0 && speed1 < speed1.getMax() / 3) {
		speed1.setName("LOW SPEED");
	}
	else if (speed1 > speed1.getMax() / 3 && speed1 < speed1.getMax() /3*2) {
		speed1.setName("MEDIUM SPEED");
	}
	else if (speed1 > speed1.getMax() / 3*2 && speed1 < speed1.getMax()) {
		speed1.setName("FAST SPEED");
	}

	if (speed2 > 0 && speed2 < speed2.getMax() / 3) {
		speed2.setName("LOW SPEED");
	}
	else if (speed2 > speed2.getMax() / 3 && speed2 < speed2.getMax() / 3 * 2) {
		speed2.setName("MEDIUM SPEED");
	}
	else if (speed2 > speed2.getMax() / 3 * 2 && speed2 < speed2.getMax()) {
		speed2.setName("FAST SPEED");
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofVec2f dirP1 = P1.position + newBySizeAngle((angleBaseP1*PI / 180), 20);

	ofVec2f dirP2 = P2.position - newBySizeAngle((angleBaseP2*PI / 180), 20);

	ofVec2f P1wheelsL = P1.position + newBySizeAngle(((angleBaseP1+90)*PI / 180), 15),
			P1wheelsR = P1.position + newBySizeAngle(((angleBaseP1-90)*PI / 180), 15);
	ofVec2f P2wheelsL = P2.position + newBySizeAngle(((angleBaseP2+90)*PI / 180), 15),
			P2wheelsR = P2.position + newBySizeAngle(((angleBaseP2-90)*PI / 180), 15);

	PainelP1.draw();
	PainelP2.draw();
	PainelMain.draw();
	//Player1
	//Base
	ofSetColor(200, 20, 20);
	ofDrawCircle(P1wheelsL.x, P1wheelsL.y, r / 2);
	ofDrawCircle(P1wheelsR.x, P1wheelsR.y, r / 2);
	ofDrawCircle(P1.position.x, P1.position.y, r);
	ofDrawCircle(dirP1, r/4);

	//Tower
	ofSetColor(255, 100, 0);
	if (!P1.shotIsTrue) {

		P1.bulletPos = P1.position + newBySizeAngle(((angleTowerP1+angleBaseP1)*PI / 180), 35);
	} 
	ofDrawCircle(P1.bulletPos.x, P1.bulletPos.y, r / 2);



	//Player2
	//Base
	ofSetColor(20, 100, 200);
	ofDrawCircle(P2.position.x, P2.position.y, r);
	ofDrawCircle(dirP2, r / 4);
	ofDrawCircle(P2wheelsL.x, P2wheelsL.y, r / 2);
	ofDrawCircle(P2wheelsR.x, P2wheelsR.y, r / 2);

	//Tower
	ofSetColor(0, 255, 255);
	if (!P2.shotIsTrue) {
		P2.bulletPos = P2.position - newBySizeAngle(((angleTowerP2+angleBaseP2)*PI / 180), 35);
	}
	ofDrawCircle(P2.bulletPos.x, P2.bulletPos.y, r / 2);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	//ofVec2f newpos = ofVec2f(ofGetWidth() / 2 - x,ofGetHeight() / 2 - y);
	//cout << newpos << endl;
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
