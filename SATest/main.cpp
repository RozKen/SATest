#include "main.h"

#include <string>
#include <GL/glut.h>
#include <iostream>

#include <Windows.h>
#include <time.h>
#include "gl_screenshot.h"
#include "Utilities.h"

///////For OpenGL Manipulation///////
float zoom = 30.0f;
//X-axis Rotation [degree]
float rotx = 30.0f;
//Y-axis Rotation [degree]
float roty = 0.0f;
float tx = 0;
float ty = -15.0f;	//負号があるのは，カメラが動くのではなく，カメラ以外を動かすから．???
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};

#define DISP_LAYER	10

////////For OpenGL Screen Capturing//////////
#define	CAPTURE
#ifdef	CAPTURE
gl_screenshot gs;
std::string captureFolder = "F:\Captures\\";
int frame = 0;
bool capture = false;
#endif	//CAPTURE

///////For Map Display Selection///////
///robotごとの地形の表示をON/OFF
bool geoFlags[DISP_LAYER];
///robotごとの放射線量の表示をON/OFF
bool radFlags[DISP_LAYER];
///世界全体の障害物(地形)の表示をON/OFF
bool geoFlagW = false;
///世界全体の放射線量の表示をON/OFF
bool radFlagW = false;
/**
	@brief キーボード操作のモードを切り替える
	true -> radFlags, false -> geoFlags
 */
bool radSwitch = true;
/**
	@brief x,y,z軸と，Gridの表示をON/OFF
	true -> display axis, false -> hide axis
 */
bool axisSwitch = true;
///RANGE_DANGERの範囲をON/OFF
bool dangerSwitch = false;
///Wi-Fiの接続可能範囲をON/OFF
bool wifiSwitch = false;
///陰影をON/OFF
bool renderShadow = true;

void Init();
void glIdle();
void glDisplay();
void glReshape(int w, int h);
void glMotion(int x,int y);
void glMouse(int b,int s,int x,int y);
void glKeyboard(unsigned char key , int x, int y);

World* world;
std::string directory;
std::vector<RobotMAV*>* mav;
clock_t start, end;

bool onSimulation = false;

/**
	@brief 色の配列を返す
 */
float* CalcColor(float radValue);

/**
	@brief Flagの組み合わせから，表示/非表示を算出
	@param flags	array of boolean flags
	@param rad		radMap->true, geoMap->false
	@param i		index : i
	@param j		index : j
 */
bool CalcDisplay(bool* flags, bool rad, int i, int j);

int main(int argc, char** argv){	

#ifdef TEST_ROBOT
	RobotTest();
#endif	//TEST_ROBOT

#ifdef TEST_LOGGER
	LoggerTest();
#endif	//TEST_LOGGER

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Radiation Mapping Agents");

	glutDisplayFunc(glDisplay);
	glutReshapeFunc(glReshape);
	glutMouseFunc(glMouse);
	glutMotionFunc(glMotion);
	glutKeyboardFunc(glKeyboard);
	glutIdleFunc(glIdle);

	Init();

	glutMainLoop();

	delete(world);
	for(int i = 0; i < mav->size(); i++){
		delete mav->at(i);
	}
	mav->clear();
	delete(mav);
	
	return 0;
}

void Init(){

	for(int i = 0; i < DISP_LAYER; i++){
		radFlags[i] = false;
		geoFlags[i] = false;
	}

	glEnable(GL_DEPTH_TEST);

	directory = logPathGenerator();
	world = new World(directory, "world.csv");
	std::cout << "world:Directory: " << world->getLogFilePath() << std::endl;

#ifdef	CAPTURE
	captureFolder = directory;
	captureFolder.append("\\capture");
	if(!PathIsDirectory(s2ws(captureFolder).c_str())){
		_mkdir(captureFolder.c_str());
	}
	captureFolder.append("\\");
#endif	//CAPTURE
	
	//最初の位置のX座標をランダムにずらす
	Random<boost::uniform_int<> > _numBatGen(-5, 5);

	mav = new std::vector<RobotMAV*>();
	for(int i = 0; i < NUM_ROBOT; i++){
		mav->push_back(new RobotMAV(directory, "mav" + world->intToString(i) + ".csv"));
		world->addRobot(mav->at(i));
		//初期値を設定
		mav->at(i)->setInput(0, MAX_BAT);
		mav->at(i)->setInput(1, START_X + (float)_numBatGen());
		mav->at(i)->setInput(2, START_Y + (float)_numBatGen());
	}
	start = clock();
}

void glIdle(){
	end = clock();
	if((double)(end - start) > TIME_STEP){
		start = clock();
		if(onSimulation){
			world->Run();
		}
	}
	glutPostRedisplay();
	//Sleep( 500 );
}


void glDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);

	//Set up Light
	if(renderShadow){
		GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
		GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat lightPos[] = {0.0, 100.0, 100.0, 1.0};

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}else{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

	//For Shadow Rendering
	GLfloat matR[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat matG[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat matB[] = {0.0, 0.0, 1.0, 1.0};

	//Draw X,Y,Z-Axis
	if(axisSwitch){
		glTranslatef(0.0f, 10.0f, 0.0f);
		if(renderShadow){
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matR);
		}else{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		glTranslatef(1.0f, 0.0f, 0.0f);
		glutSolidCube(1.0f);
		glTranslatef(2.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0, 1, 0);
		glutSolidCone(0.5f, 1.0f, 12, 4);
		glRotatef(-90.0f, 0, 1, 0);
		glTranslatef(-2.0f, 0.0f, 0.0f);
		glTranslatef(-1.0f, 0.0f, 0.0f);
		
		if(renderShadow){
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matG);
		}else{
				glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}

		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidCube(1.0f);
		glTranslatef(0.0f, 2.0f, 0.0f);
		glRotatef(-90.0f, 1, 0, 0);
		glutSolidCone(0.5f, 1.0f, 12, 4);
		glRotatef(90.0f, 1, 0, 0);
		glTranslatef(0.0f, -2.0f, 0.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);

		if(renderShadow){
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matB);
		}else{
				glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		}

		glTranslatef(0.0f, 0.0f, 1.0f);
		glutSolidCube(1.0f);
		glTranslatef(0.0f, 0.0f, 2.0f);
		glRotatef(90.0f, 0, 0, 1);
		glutSolidCone(0.5f, 1.0f, 12, 4);
		glRotatef(-90.0f, 0, 0, 1);
		glTranslatef(0.0f, 0.0f, -2.0f);
		glTranslatef(0.0f, 0.0f, -1.0f);

		glTranslatef(0.0f, -10.0f, 0.0f);
	}

	RobotMAV* robot;

	double offset = -50.0;		//FIELD_SIZEの半分
	glTranslatef(offset, 0, offset);

		//Draw Start Point as Cone
		glTranslatef(START_X, 1.0f, START_Y);
		glRotatef(-90.0f, 1, 0, 0);
		if(renderShadow){
			GLfloat matTmp[] = {0.9f, 0.9f, 0.9f, 1.0f};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
		}else{
			glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
		}
		glutSolidCone(1.0f, 3.0f, 12, 4);
		glTranslatef(0.0f, -1.0f, 0.0f);
		if(renderShadow){
			GLfloat matTmp[] = {1.0f, 1.0f, 0.0f, 0.3f};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
		}else{
			glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
		}
		glutSolidCone((float)START_R, 0.1f, 12, 4);
		glRotatef(90.0f, 1, 0, 0);
		glTranslatef(-START_X, 0.0f, -START_Y);

		//Draw Barriers as Boxes
		for(int i = 0; i < FIELD_SIZE; i++){
			for(int j = 0; j < FIELD_SIZE; j++){
				//On Barrier
				if(world->geoField[i][j] == OUTOFAREA){
					if(CalcDisplay(geoFlags, false, i, j) || geoFlagW){
						if(renderShadow){
							glMaterialfv(GL_FRONT, GL_DIFFUSE, matB);
						}else{
							glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
						}
					}else{
						if(renderShadow){
							GLfloat matTmp[] = {0.0, 0.0, 0.2, 1.0};
							glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
						}else{
							glColor4f(0.0f, 0.0f, 0.2f, 1.0f);
						}
					}

					glTranslatef((GLfloat)i, 0, (GLfloat)j);
					glutSolidCube(1.0);
					glTranslatef((GLfloat)-i, 0, (GLfloat)-j);
				}else{		//Not on Barrier
					float* color = CalcColor(world->radField[i][j]);
					if(CalcDisplay(radFlags, true, i, j) || radFlagW){
						float color0 = color[0];
						float color1 = color[1];
						float color2 = color[2];
						if(renderShadow){
							GLfloat matTmp[] = {color[0], color[1], color[2], 1.0};
							glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
						}else{
							glColor4f(color[0], color[1], color[2], 1.0f);
						}
						delete color;	//CalcColor内でnewしてるから
					}else{
						if(renderShadow){
							GLfloat matTmp[] = {color[0] * 0.2f, color[1] * 0.2f, color[2] * 0.2f, 1.0f};
							glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
						}else{
							glColor4f(color[0] * 0.2f, color[1] * 0.2f, color[2] * 0.2f, 1.0f);
						}
						delete color;	//CalcColor内でnewしてるから
					}
					glTranslatef((GLfloat)i, -1.0f, (GLfloat)j);
					glutSolidCube(1.0);
					glTranslatef((GLfloat)-i, +1.0f, (GLfloat)-j);
				}
			}
		}

		//Draw Robots as Spheres
		for(int index = 0; index < world->getNumOfModules(); index++){
			robot = world->getRobot(index);
			if(renderShadow){
				GLfloat matTmp[] = {robot->getColorR(), robot->getColorG(),
					robot->getColorB(), 1.0};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
			}else{
				glColor4f(robot->getColorR(), robot->getColorG()
					, robot->getColorB(), 1.0f);
			}
			float x = robot->getPosX();
			float y = robot->getPosY();
			glTranslatef(x, 0, y);
			//Draw Robot
			glutSolidSphere(0.5, 12, 12);
			
			if(dangerSwitch){
				//Draw Reach of Sensors
				if(renderShadow){
					GLfloat matTmp[] = {1.0, 0.0, 0.0, 0.3};
					glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
				}else{
					glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
				}
				glTranslatef(0, -0.4f, 0);
				glRotatef(-90.0f, 1, 0, 0);
				glutSolidCone(RANGE_DANGER, 0.2, 12, 2);
				glRotatef(90.0f, 1, 0, 0);
				glTranslatef(0, 0.4f, 0);
			}
			if(wifiSwitch){
				//Draw Reach of Wi-Fi
				if(renderShadow){
					GLfloat matTmp[] = {0.0, 1.0, 0.0, 0.1};
					glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
				}else{
					glColor4f(0.0f, 1.0f, 0.0f, 0.1f);
				}
				glTranslatef(0, -0.4f, 0);
				glRotatef(-90.0f, 1, 0, 0);
				glutSolidCone(WIFI_REACH, 0.1, 32, 5);
				glRotatef(90.0f, 1, 0, 0);
				glTranslatef(0, 0.4f, 0);
			}
			glTranslatef(-x, -0, -y);

			//Draw Exploring Objective
			if(renderShadow){
				GLfloat matTmp[] = {1.0, 1.0, 1.0, 1.0};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
			}else{
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}

			glTranslatef(robot->getObjectiveX(), 0, robot->getObjectiveY());
			glRotatef(-90.0f, 1, 0, 0);
			glutSolidCone(0.3, 2.0, 12, 3);
			glRotatef(90.0f, 1, 0, 0);
			glTranslatef(-robot->getObjectiveX(), 0, -robot->getObjectiveY());
		}

	glTranslatef(-offset, 0, -offset);

	// draw grid
	if(renderShadow){
		GLfloat matTmp[] = {0.0, 0.8, 0.0, 0.8};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
	}else{
		glColor4f(0.0f, 0.8f, 0.0f, 0.8f);
	}
	if(axisSwitch){
		glBegin(GL_LINES);
		//glTranslatef(-0.5, -0.5, -0.5);
		for(int i=-50;i<=50;++i) {
			glVertex3f(i - 0.5,-0.4,-50 - 0.5);
			glVertex3f(i - 0.5,-0.4,50 -0.5);

			glVertex3f(50 - 0.5,-0.4,i - 0.5);
			glVertex3f(-50 - 0.5,-0.4,i - 0.5);
		}
		//glTranslatef(0.5, 0.5, 0.5);
		glEnd();
	}

	glutSwapBuffers();
	glDisable(GL_BLEND);
	
#ifdef	CAPTURE
	if(capture){
		std::string fileName = captureFolder;
		fileName.append(intToString5(frame));
		fileName.append(".bmp");
		gs.screenshot(fileName.c_str(), 24);
		frame++;
	}
#endif	//CAPTURE
}

void glReshape(int w, int h)
{
	// prevent divide by 0 error when minimised
	if(w==0) 
		h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,0.1,200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void glMotion(int x, int y){
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[0] && Buttons[2] )
	{
		zoom -= (float) 0.05f * diffy;
	}else if( Buttons[0] ){
		rotx += (float) 0.5f * diffy;
		roty += (float) 0.5f * diffx;		
	}else if( Buttons[2] ){
		tx += (float) 0.05f * diffx;
		ty -= (float) 0.05f * diffy;
	}
	glutPostRedisplay();
}

void glMouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}

void glKeyboard(unsigned char key , int x, int y){
	switch(key){
	case 't':
		std::cout << " 't' pushed !" << std::endl;
		break;
	case 27:	//ESC
		exit(0);
		break;
	case 32:	//Space Key
		onSimulation = !onSimulation;
		break;
	case '0':
		if(radSwitch){
			radFlags[0] = !radFlags[0];
		}else{
			geoFlags[0] = !geoFlags[0];
		}
		break;
	case '1':
		if(radSwitch){
			radFlags[1] = !radFlags[1];
		}else{
			geoFlags[1] = !geoFlags[1];
		}
		break;
	case '2':
		if(radSwitch){
			radFlags[2] = !radFlags[2];
		}else{
			geoFlags[2] = !geoFlags[2];
		}
		break;
	case '3':
		if(radSwitch){
			radFlags[3] = !radFlags[3];
		}else{
			geoFlags[3] = !geoFlags[3];
		}
		break;
	case '4':
		if(radSwitch){
			radFlags[4] = !radFlags[4];
		}else{
			geoFlags[4] = !geoFlags[4];
		}
		break;
	case '5':
		if(radSwitch){
			radFlags[5] = !radFlags[5];
		}else{
			geoFlags[5] = !geoFlags[5];
		}
		break;
	case '6':
		if(radSwitch){
			radFlags[6] = !radFlags[6];
		}else{
			geoFlags[6] = !geoFlags[6];
		}
		break;
	case '7':
		if(radSwitch){
			radFlags[7] = !radFlags[7];
		}else{
			geoFlags[7] = !geoFlags[7];
		}
		break;
	case '8':
		if(radSwitch){
			radFlags[8] = !radFlags[8];
		}else{
			geoFlags[8] = !geoFlags[8];
		}
		break;
	case '9':
		if(radSwitch){
			radFlags[9] = !radFlags[9];
		}else{
			geoFlags[9] = !geoFlags[9];
		}
		break;
	case 'd':
		dangerSwitch = !dangerSwitch;
		break;
	case 'i':
		wifiSwitch = !wifiSwitch;
		break;
	case 'w':
		if(radSwitch){
			radFlagW = !radFlagW;
		}else{
			geoFlagW = !geoFlagW;
		}
		break;
	case 's':	//Switch
		radSwitch = !radSwitch;
		break;
	case 'c':	//Coordinate and X, Y, Z - Axis
		axisSwitch = !axisSwitch;
		break;
	case 'r':	//Render Shadow
		renderShadow = !renderShadow;
		break;
	case 'a':
		for(int i = 0; i < DISP_LAYER; i++){
			if(radSwitch){
				radFlags[i] = true;
			}else{
				geoFlags[i] = true;
			}
		}
		break;
	case 'z':
		for(int i = 0; i < DISP_LAYER; i++){
			if(radSwitch){
				radFlags[i] = false;
			}else{
				geoFlags[i]  = false;
			}
		}
		break;
	case 'q':
		capture = !capture;
		break;
	default:
		break;
	}
}

float* CalcColor(float radValue){
	float* color = new float[3];
	if(radValue < 500.0f){
		color[0] = 0.0f;
		color[1] = radValue / 500.0f;
		color[2] = 1.0f - color[1];
	}else if(radValue < 6000){
		color[0] = (radValue - 500.0f)/5500.0f;
		color[1] = 1.0f - color[0];
		color[2] = 0.0f;
	}else{	//6000.0f <= radValue <= 10000.0f
		//完全にはBlueにしないよう，4000ではなく，8000で割る.
		color[0] = 1.0f - (radValue - 6000.0f)/ 8000.0f;
		color[1] = 0.0f;
		color[2] = 1.0f - color[0];
	}
	return color;
}

bool CalcDisplay(bool* flags, bool rad, int i, int j){
	bool answer = false;
	
	if(rad){
		for(int robot = 0; robot < world->getNumOfModules() && robot < DISP_LAYER; robot++){
			if(flags[robot] && ((int)(world->getRobot(robot)->radMap[i][j] - 0.5f) != (int)NO_DATA_ON_FIELD )){
				//std::cout << (int)(world->getRobot(robot)->radMap[i][j] - 0.5f) << std::endl;
				//std::cout << NO_DATA_ON_FIELD << std::endl;
				answer = true;
				break;
			}
		}	
	}else{
		for(int robot = 0; robot < world->getNumOfModules() && robot < DISP_LAYER; robot++){
			if(flags[robot] && ((int)(world->getRobot(robot)->geoMap[i][j] - 0.5f) != (int)NO_DATA_ON_FIELD )){
				answer = true;
				break;
			}
		}
	}
	return answer;
}