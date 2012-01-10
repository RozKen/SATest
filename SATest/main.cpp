#include "main.h"

#include <string>
#include <GL/glut.h>
#include <iostream>

#include <Windows.h>
#include <time.h>
#include "gl_screenshot.h"
#include "Utilities.h"

///////For OpenGL Manipulation///////
float zoom = 50.0f;
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
std::string captureFolder = "F:\\Captures\\";
int frame = 0;
bool capture = false;
#endif	//CAPTURE

///////For Map Display Selection///////
/**
	@brief x,y,z軸と，Gridの表示をON/OFF
	true -> display axis, false -> hide axis
 */
bool axisSwitch = true;
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
float* CalcColor(int x);

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

	mav = new std::vector<RobotMAV*>();
	int i = 0;
	mav->push_back(new RobotMAV(0, 1.0f, directory, "mav" + world->intToString(i) + ".csv"));
	i++;
	mav->push_back(new RobotMAV(1, 1.0f, directory, "mav" + world->intToString(i) + ".csv"));
	i++;
	mav->push_back(new RobotMAV(4, 1.0f, directory, "mav" + world->intToString(i) + ".csv"));
	i++;
	mav->push_back(new RobotMAV(3, 1.0f, directory, "mav" + world->intToString(i) + ".csv"));
	i++;
	for(; i < NUM_ROBOTS; i++){
		mav->push_back(new RobotMAV(2, 0.1f * (float)(i - 1), directory, "mav" + world->intToString(i) + ".csv"));
	}
	for(i = 0; i < NUM_ROBOTS; i++){
		world->addRobot(mav->at(i));
		//初期値を設定
		mav->at(i)->setBattery(MAX_BAT);
		mav->at(i)->setPosX(0);
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

	double offsetX = -LENGTH / 2;		//FIELD_SIZEの半分
	double offsetY = -NUM_ROBOTS / 2;
	glTranslatef(offsetX, 0, offsetY);

		//Draw Battery Charger as Cone
		int geo = NO_SIGNAL;
		for(int i = 0; i < LENGTH; i++){
			float* color = CalcColor(i);
			if(renderShadow){
				GLfloat matTmp[] = {color[0], color[1], color[2], 1.0};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, matTmp);
			}else{
				glColor4f(color[0], color[1], color[2], 1.0f);
			}
			//CalcColor内でnewしている．
			delete color;

			glTranslatef(i, 0, 0);
			geo = world->geoField[i][0];
			for(int j = 0; j < NUM_ROBOTS; j++){
				glTranslatef(0, 0, j);
				if(geo == ONCHARGER){
					glRotatef(-90.0f, 1, 0, 0);
					glutSolidCone(0.3, 2.0, 12, 3);
					glRotatef(90.0f, 1, 0, 0);
				}
				glTranslatef(0, -1, 0);
					glutSolidCube(1);
				glTranslatef(0, 1, 0);
				glTranslatef(0, 0, -j);
			}
			glTranslatef(-i, 0, 0);
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
			float y = index;
			glTranslatef(x, 0, y);
			//Draw Robot
			glutSolidSphere(0.5, 12, 12);
			
			glTranslatef(-x, -0, -y);
		}

	glTranslatef(-offsetX, 0, -offsetY);

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
	case 27:	//ESC
		exit(0);
		break;
	case 32:	//Space Key
		onSimulation = !onSimulation;
		break;
	case 'c':	//Coordinate and X, Y, Z - Axis
		axisSwitch = !axisSwitch;
		break;
	case 'r':	//Render Shadow
		renderShadow = !renderShadow;
		break;
	case 'q':
		capture = !capture;
		break;
	default:
		break;
	}
}

float* CalcColor(int x){
	float* color = new float[3];
	int status = world->geoField[x][0];
	switch(status){
	case ONSTART:
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		break;
	case ONGOAL:
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
		break;
	case ONCHARGER:
		color[0] = 0.0f;
		color[1] = 1.0f;
		color[2] = 0.0f;
		break;
	default:
		color[0] = 0.3f;
		color[1] = 0.3f;
		color[2] = 0.3f;
		break;
	}
	return color;
}

bool CalcDisplay(bool* flags, bool rad, int i, int j){
	bool answer = true;
	return answer;
}