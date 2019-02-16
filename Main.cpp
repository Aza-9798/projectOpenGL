#include<windows.h>
#include<gl/gl.h>
#include<gl/glut.h>
#include<gl/glaux.h>
#include<cmath>
#include<stdio.h>

#pragma comment (lib,"GLU32.lib")
#pragma comment (lib,"GLAUX.lib")
#pragma comment (lib,"OPENGL32.lib")
#pragma comment (lib,"glut32.lib")

unsigned int tex1;
AUX_RGBImageRec* image1;
unsigned int tex2;
AUX_RGBImageRec* image2;
unsigned int tex3;
AUX_RGBImageRec* image3;
unsigned int tex4;
AUX_RGBImageRec* image4;
unsigned int tex5;
AUX_RGBImageRec* image5;
unsigned int currentTex;

unsigned int outTex;
AUX_RGBImageRec* outImage;



int xLen = 50, yLen = 50, zLen = 100, CBUTTON = 1;
const int SPHERE = 0, CUBE = 1, CONE = 2, CYLINDER = 3,DISK = 4;
int figure = SPHERE;
double scale = 1,angle;
double  sphereR = 8;//Sphere parameters 
double cubeL = 16;//Cube parameters
double coneBase = 8,coneHeight = 24;
double cylinderR = 8, cylinderHeight = 24;
double diskInnerR = 8, diskOuterR = 12;
double X = 0, Y = 0, Z = 0;
double cameraX = 2, cameraY = 5, cameraZ = 10,destX = 0,destY = 0,destZ = 0;//Camera parameters
int width = 640, height = 640;
int frameWidth = 10,lLeft = 20,lUpper = 55,buttonWidth = 7,buttonHeight = 5,verticalDist = 2;
double xAngle = 0, yAngle = 0, zAngle = 0, da = 0.5;
int  xRotation = 0, yRotation = 0, zRotation = 0;

int ff = 0; //Fog type
double fogStart = 1.0, fogEnd = 3.0, fogDensity = 0.3;
GLfloat fogColor[4] = { 0.0, 0.0, 0.5, 1.0f }; // Цвет тумана

bool translationSettings = false, rotationSettings = false, scaleSettings = false, idleRotation = false;
bool cameraSettings = false;
int cameraRotation = 0;
double cameraAngle = 0,cameraD = 0.25;

//Material and lighting settings
bool lightIsOn = true, lightSettings = false, textureSettings = false, fogSettings = false, textureOn = false,fogOn= false;
int lightMode = 0; //Light settings
double cutoff = 50;
//
int textureCount = 0;

GLfloat mat_specular[] = { 1.0,1.0,1.0,1 };
GLfloat mat_shininess = 50.0;
//Lighting
GLfloat light_ambient[] = { 2.0,0.0,0.0,1.0 };
GLfloat light_diffuse[] = { 0.0,1.0,1.0,1.0 };
GLfloat white_light[] = { 1.0,1.0,1.0,1.0 };
GLfloat blue_light[] = { 0.0,0.0,1.0,1.0 };
GLfloat red_light[] = { 1.0,0.0,0.0,1.0 };
GLfloat green_light[] = { 0.0,1.0,0.0,1.0 };
GLfloat yellow_light[] = { 0.5,0.8,0,1.0 };
GLfloat zero_dir[] = { 0,0,0 };
GLfloat light_in_center[] = { 0,0,0,6 };

void myInit() {
	glClearColor(0.5, 0.5, 1.0, 0.0);
	glLineWidth(1.0);
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xLen, xLen, -yLen, yLen, -zLen, zLen);
	//Material
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	//Light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	//Light1
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 50);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-xLen, xLen, -yLen * (GLfloat)h / (GLfloat)w, yLen * (GLfloat)h / (GLfloat)w, -zLen, zLen);
	else
		glOrtho(-xLen * (GLfloat)w / (GLfloat)h, xLen * (GLfloat)w / (GLfloat)h, -yLen, yLen, -zLen, zLen);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mouse(int button, int state, int x, int y) {//Mouse function
	if (button == GLUT_LEFT_BUTTON&& state == GLUT_DOWN) {
		int id;
		glReadPixels(x, height - y, 1, 1, GL_STENCIL_INDEX, GL_INT, &id);
		if (id==1) {
			//Corresponding set of commands
			figure = SPHERE;
		}
		else if (id==2) {
			figure = CUBE;
		}
		if (id==3) {
			figure = CONE;
		}
		else if (id==4) {
			figure = CYLINDER;
		}
		else if (id == 5) {//Light
			lightSettings = true;
		}
		else if (id == 6) {//Textures
			textureSettings = true;
		}
		else if (id == 7) {//Fog
			fogSettings = true;
		}
		else if (id==8) {
			translationSettings = true;
		}
		else if (id==9) {
			rotationSettings = true;
		}
		else if (id==10) {
			scaleSettings = true;
		}
		else if (id==11) {
			lightIsOn = !lightIsOn;
		}
		else if (id == 12) {
			lightMode = (++lightMode) % 3;
			if (lightMode == 0) {
				printf("Mode changed to directed\n");
			}
			else if (lightMode == 1) {
				printf("Mode changed to pointed\n");
			}
			else {
				printf("Mode changed to spotlight\n");
			}
		}
		else if (id == 13) {
			cutoff += 1;
			if (cutoff >= 180) {
				cutoff = 2;
			}
		}
		else if (id == 14) {
			cutoff -= 2;
			if (cutoff < 2) {
				cutoff = 178;
			}
		}
		else if (id == 15) {
			lightSettings = false;
		}
		else if (id == 16) {
			textureOn = !textureOn;
		}
		else if(id == 17) {
			textureCount = 1;
		}
		else if (id == 18) {
			textureCount = 2;
		}
		else if (id == 19) {
			textureCount = 3;
		}
		else if (id == 20) {
			textureCount = 4;
		}
		else if (id == 21) {
			textureCount = 5;
		}
		else if (id == 22) {
			textureSettings = false;
		}
		else if (id == 23) {
			fogOn = !fogOn;
		}
		else if (id == 24) {
			ff = 0;
		}
		else if (id == 25) {
			ff = 1;
		}
		else if (id == 26) {
			ff = 2;
		}
		else if (id == 27) {
			fogDensity += 0.05;
			if (fogDensity > 1) {
				fogDensity = 0;
			}
		}
		else if (id == 28) {
			fogDensity -= 0.05;
			if (fogDensity < 0) {
				fogDensity = 1;
			}
		}
		else if (id == 29) {
			fogSettings = false;
		}
		else if (id == 30) {
			X++;
		}
		else if (id == 31) {
			X--;
		}
		else if (id == 32) {
			Y++;
		}
		else if (id == 33) {
			Y--;
		}
		else if (id == 34) {
			Z++;
		}
		else if (id == 35) {
			Z--;
		}
		else if (id == 36) {
			translationSettings = false;
		}
		else if (id == 37) {
			xRotation = (xRotation + 1) % 2;
			yRotation = 0;
			zRotation = 0;

			xAngle += da;
		}
		else if (id == 38) {
			yRotation = (yRotation + 1) % 2;
			xRotation = 0;
			zRotation = 0;

			yAngle += da;
		}
		else if (id == 39) {
			zRotation = (zRotation + 1) % 2;
			yRotation = 0;
			xRotation = 0;

			zAngle += da;
		}
		else if (id == 40) {
			idleRotation = !idleRotation;
		}
		else if (id == 41) {
			da = -da;
		}
		else if (id == 42) {
			rotationSettings = false;
		}
		else if (id == 43) {
			if (scale <= 3) {
				scale += 0.2;
			}
		}
		else if (id == 44) {
			if (scale > 0.2) {
				scale -= 0.2;
			}
		}
		else if (id == 45) {
			scaleSettings = false;
		}
		else if (id == 46) {
			figure = DISK;
		}
		else if (id == 47) {
			cameraSettings = true;
		}
		else if (id == 48) {
			cameraRotation = (++cameraRotation) % 2;
		}
		else if (id == 49) {
			cameraD = -cameraD;
		}
		else if (id == 50) {
			cameraSettings = false;
		}
		printf("x = %d , y = %d\n", x, y);
	}
}

void drawBox(double x, double y,int z, double a, double b, double c) {
	glBegin(GL_QUADS);
	glVertex3d(x, y, z);//Front
	glVertex3d(x + a, y, z);
	glVertex3d(x + a, y - b, z);
	glVertex3d(x, y - b, z);
	glVertex3d(x, y, z);//Left
	glVertex3d(x, y, z - c);
	glVertex3d(x, y - b, z - c);
	glVertex3d(x, y - b, z);
	glVertex3d(x + a, y, z);//Right
	glVertex3d(x + a, y, z - c);
	glVertex3d(x + a, y - b, z - c);
	glVertex3d(x + a, y - b, z);
	glVertex3d(x , y, z-c);//Back
	glVertex3d(x, y - b, z - c);
	glVertex3d(x + a, y - b, z - c);
	glVertex3d(x + a, y, z - c);
	glVertex3d(x, y - b, z);//Bottom
	glVertex3d(x, y - b, z - c);
	glVertex3d(x + a, y - b, z - c);
	glVertex3d(x + a, y - b, z );
	glVertex3d(x, y + b, z);//Top
	glVertex3d(x, y + b, z - c);
	glVertex3d(x + a, y + b, z - c);
	glVertex3d(x + a, y + b, z);
	glEnd();
}

void writeText(char* str, double x, double y) {
	glRasterPos2f(x, y);
	int i = 0;
	while (str[i] != '\0')
	{
		glPushMatrix();
		glScalef(0.5, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
		glPopMatrix();
		i++;
		glRasterPos2f(x + i, y);
	}
}

void drawButton(double x, double y,double width,double height,char* str) {
	glPushMatrix();
	glTranslated(x, y, 1.2);
	glColor3d(0, 0, 0);
	int i = 0;
	double diff;
	while (str[i] != '\0') {
		i++;
	}
	diff = width - i;
	if (diff > 0) {
		writeText(str, diff / 2, -2 * buttonHeight / 3);
	}
	else {
		writeText(str, 0.5, -2 * buttonHeight / 3);
	}
	glPopMatrix();
	glColor3d(0.8, 0.8, 0.5);
	drawBox(x, y, 1, width, height, 2);
}

void drawPP_WT(double x, double y, double z, double xlen, double ylen, double zlen, unsigned int tex) {

	glBindTexture(GL_TEXTURE_2D, tex);//Front
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + xlen / 2, y - ylen / 2, z + zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + xlen / 2, y + ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x - xlen / 2, y + ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x - xlen / 2, y - ylen / 2, z + zlen / 2);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, tex);//Back
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x - xlen / 2, y - ylen / 2, z - zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x - xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x + xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x + xlen / 2, y - ylen / 2, z - zlen / 2);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, tex);//Left
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x - xlen / 2, y - ylen / 2, z + zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x - xlen / 2, y + ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x - xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x - xlen / 2, y - ylen / 2, z - zlen / 2);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, tex);//Right
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + xlen / 2, y - ylen / 2, z - zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x + xlen / 2, y + ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x + xlen / 2, y - ylen / 2, z + zlen / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, tex);//Bottom
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + xlen / 2, y - ylen / 2, z - zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + xlen / 2, y - ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x - xlen / 2, y - ylen / 2, z + zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x - xlen / 2, y - ylen / 2, z - zlen / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, tex);//Top
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + xlen / 2, y + ylen / 2, z + zlen / 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(x - xlen / 2, y + ylen / 2, z - zlen / 2);
	glTexCoord2f(0.0, 0.0); glVertex3f(x - xlen / 2, y + ylen / 2, z + zlen / 2);
	glEnd();
}

void texturedButton(double x, double y, double width, double height, unsigned int tex) {
	drawPP_WT(x + width / 2, y - height/2 , 0, width, height, 2, tex);
}

void lightButtons() {
	char** lightStrs = new char*[5];
	lightStrs[0] = "On/Off";
	lightStrs[1] = "Mode";
	lightStrs[2] = "+ angle";
	lightStrs[3] = "- angle";
	lightStrs[4] = "Back";
	CBUTTON = 11;
	for (int i = 0; i < 5; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(-xLen + frameWidth + i*(buttonWidth + verticalDist + 2), yLen - 2, buttonWidth + 2, buttonHeight, lightStrs[i]);
		CBUTTON++;
	}
}

void textureButtons() {
	char** textureStrs = new char*[7];
	unsigned int texes[] = { tex1,tex2,tex3,tex4,tex5 };
	textureStrs[0] = "On/Off";
	textureStrs[1] = "tex1";
	textureStrs[2] = "tex2";
	textureStrs[3] = "tex3";
	textureStrs[4] = "tex4";
	textureStrs[5] = "tex5";
	textureStrs[6] = "Back";
	CBUTTON = 16;
	for (int i = 0; i < 7; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (i == 0 || i == 6) {
			drawButton(-xLen + frameWidth + i*(buttonWidth + verticalDist + 2), yLen - 2, buttonWidth + 2, buttonHeight, textureStrs[i]);
		}
		else {
			glEnable(GL_TEXTURE_2D);
			texturedButton(-xLen + frameWidth + i*(buttonWidth + verticalDist + 2), yLen - 2, buttonWidth + 2, buttonHeight, texes[i - 1]);
			glDisable(GL_TEXTURE_2D);
		}
		CBUTTON++;
	}
}

void fogButtons() {
	char** fogStrs = new char*[7];
	fogStrs[0] = "On/Off";
	fogStrs[1] = "Exp";
	fogStrs[2] = "Exp^2";
	fogStrs[3] = "Linear";
	fogStrs[4] = "Density+";
	fogStrs[5] = "Density-";
	fogStrs[6] = "Back";
	CBUTTON = 23;
	for (int i = 0; i < 7; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(-xLen + frameWidth + i*(buttonWidth + verticalDist + 2), yLen - 2, buttonWidth + 2, buttonHeight, fogStrs[i]);
		CBUTTON++;
	}
}


void translationButtons() {
	char** translationStrs = new char*[7];
	translationStrs[0] = "X+";
	translationStrs[1] = "X-";
	translationStrs[2] = "Y+";
	translationStrs[3] = "Y-";
	translationStrs[4] = "Z+";
	translationStrs[5] = "Z-";
	translationStrs[6] = "Back";
	CBUTTON = 30;
	for (int i = 0; i < 7; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, translationStrs[i]);
		CBUTTON++;
	}
}

void rotationButtons() {
	char** rotationStrs = new char*[7];
	rotationStrs[0] = "X axis";
	rotationStrs[1] = "Y axis";
	rotationStrs[2] = "Z axis";
	rotationStrs[3] = "Idle";
	rotationStrs[4] = "Direction";
	rotationStrs[5] = "Back";
	CBUTTON = 37;
	for (int i = 0; i < 6; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, rotationStrs[i]);
		CBUTTON++;
	}
}

void scaleButtons() {
	char** scaleStrs = new char*[3];
	scaleStrs[0] = "+";
	scaleStrs[1] = "-";
	scaleStrs[2] = "Back";
	CBUTTON = 43;
	for (int i = 0; i < 3; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, scaleStrs[i]);
		CBUTTON++;
	}
}

void cameraButtons() {
	char** cameraStrs = new char*[3];
	cameraStrs[0] = "On/Off";
	cameraStrs[1] = "Direction";
	cameraStrs[2] = "Back";
	CBUTTON = 48;
	for (int i = 0; i < 3; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, cameraStrs[i]);
		CBUTTON++;
	}
}

void drawOutlines() {
	glColor3d(0.5, 0.5, 0.5);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, outTex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2d(-xLen,yLen);//Left 
	glTexCoord2f(0.0, 0.0); glVertex2d(-xLen,-yLen);
	glTexCoord2f(0.1, 0.0); glVertex2d(-xLen+frameWidth,-yLen);
	glTexCoord2f(0.1, 1.0); glVertex2d(-xLen+frameWidth,yLen);
	glTexCoord2f(1.0, 1.0); glVertex2d(xLen, yLen);//Right 
	glTexCoord2f(0.9, 1.0); glVertex2d(xLen - frameWidth, yLen);
	glTexCoord2f(0.9, 0.0); glVertex2d(xLen - frameWidth, -yLen);
	glTexCoord2f(1.0, 0.0); glVertex2d(xLen, -yLen);
	glTexCoord2f(0.1, 0.1); glVertex2d(-xLen + frameWidth, -yLen + frameWidth);//Bottom 
	glTexCoord2f(0.1, 0.0); glVertex2d(-xLen + frameWidth, -yLen);
	glTexCoord2f(0.9, 0.0); glVertex2d(xLen - frameWidth, -yLen);
	glTexCoord2f(0.9, 0.1); glVertex2d(xLen - frameWidth, -yLen+frameWidth);
	glTexCoord2f(0.1, 1.0); glVertex2d(-xLen + frameWidth, yLen);//Top
	glTexCoord2f(0.1, 0.9); glVertex2d(-xLen + frameWidth, yLen - frameWidth);
	glTexCoord2f(0.9, 0.9); glVertex2d(xLen - frameWidth, yLen - frameWidth);
	glTexCoord2f(0.9, 1.0); glVertex2d(xLen - frameWidth, yLen);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	char** initStrs = new char*[11];
	initStrs[0] = "Sphere";
	initStrs[1] = "Cube";
	initStrs[2] = "Cone";
	initStrs[3] = "Cylinder";
	initStrs[4] = "Light";
	initStrs[5] = "Textures";
	initStrs[6] = "Fog";
	initStrs[7] = "Move";
	initStrs[8] = "Rotate";
	initStrs[9] = "Scale";
	initStrs[10] = "Disk";
	glEnable(GL_STENCIL_TEST);
	CBUTTON = 1;
	for (int i = 0; i < 4; i++) {
		glStencilFunc(GL_ALWAYS, CBUTTON, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(-xLen + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, initStrs[i]);
		CBUTTON++;
	}
	//Disk
	glStencilFunc(GL_ALWAYS, 46, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawButton(-xLen + 1.5, yLen - frameWidth - 4*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, initStrs[10]);
	//Disk
	if (lightSettings) {
		lightButtons();
	}
	else if (textureSettings) {
		textureButtons();
	}
	else if (fogSettings) {
		fogButtons();
	}
	else {
		CBUTTON = 5;
		for (int i = 0; i < 3; i++) {
			glStencilFunc(GL_ALWAYS, CBUTTON, 0);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			drawButton(-xLen + frameWidth + i*(buttonWidth + 7), yLen - 2, buttonWidth+2, buttonHeight, initStrs[i + 4]);
			CBUTTON++;
		}
	}
	if (translationSettings) {
		translationButtons();
	}
	else if (rotationSettings) {
		rotationButtons();
	}
	else if(scaleSettings){
		scaleButtons();
	}
	else if (cameraSettings) {
		cameraButtons();
	}
	else {
		CBUTTON = 8;
		for (int i = 0; i < 3; i++) {
			glStencilFunc(GL_ALWAYS, CBUTTON, 0);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - i*(buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, initStrs[i + 7]);
			CBUTTON++;
		}
		//Camera
		glStencilFunc(GL_ALWAYS, 47, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawButton(xLen - frameWidth + 1.5, yLen - frameWidth - 3 * (buttonHeight + verticalDist) - 1, buttonWidth, buttonHeight, "Camera");
		//Camera
	}
	glDisable(GL_STENCIL_TEST);
}

void fogSetting() {
	GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR }; // Хранит три типа тумана


	glEnable(GL_FOG);                       // Включает туман (GL_FOG)

	glFogi(GL_FOG_MODE, fogMode[ff]);         // Выбираем тип тумана

	glFogfv(GL_FOG_COLOR, fogColor);        // Устанавливаем цвет тумана

	glFogf(GL_FOG_DENSITY, fogDensity);          // Насколько густым будет туман

	glHint(GL_FOG_HINT, GL_DONT_CARE);      // Вспомогательная установка тумана

	glFogf(GL_FOG_START, fogStart);             // Глубина, с которой начинается туман

	glFogf(GL_FOG_END, fogEnd);
}

void lightSetup() {

	GLfloat light_position[4] = { 0.0,scale*1.0,scale*1.0,0.0 };

	GLfloat light1_position[] = { X ,Y + 15 ,Z + 15,1 };
	GLfloat light1_dir[] = { 0, -1,-1 };

	if (lightMode == 1) {
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180);
		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
		glDisable(GL_LIGHT0);
	}
	else if (lightMode == 2) {
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
		glDisable(GL_LIGHT0);
	}
	else {
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glDisable(GL_LIGHT1);
	}
}


void Display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.5, 0.5, 1.0, 0.0);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricTexture(quadObj, GL_TRUE);
	drawOutlines();
	glColor3f(1.0, 0.7, 0.0);

	if (fogOn) {
		glEnable(GL_FOG);
		fogSetting();
	}

	if (lightIsOn) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		lightSetup();
	}
	glPushMatrix();
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	glRotated(cameraAngle, 0, 1, 0);
	glPushMatrix();
	glTranslated(X, Y, Z);
	glRotated(xAngle, 1, 0, 0);
	glRotated(yAngle, 0, 1, 0);
	glRotated(zAngle, 0, 0, 1);
	glScalef(scale, scale, scale);
	if (textureOn) {
		glEnable(GL_TEXTURE_2D);
		if (textureCount == 1) {
			glBindTexture(GL_TEXTURE_2D, tex1);
			currentTex = tex1;
		}
		else if (textureCount == 2) {
			glBindTexture(GL_TEXTURE_2D, tex2);
			currentTex = tex2;
		}
		else if (textureCount == 3) {
			glBindTexture(GL_TEXTURE_2D, tex3);
			currentTex = tex3;
		}
		else if (textureCount == 4) {
			glBindTexture(GL_TEXTURE_2D, tex4);
			currentTex = tex4;
		}
		else if (textureCount == 5) {
			glBindTexture(GL_TEXTURE_2D, tex5);
			currentTex = tex5;
		}
	}
	if (figure == SPHERE) {
		gluSphere(quadObj, sphereR, 30, 30);
	}
	else if (figure == CUBE) {
		if (textureOn) {
			drawPP_WT(0, 0, 0, cubeL, cubeL, cubeL, currentTex);
		}
		else {
			auxSolidBox(cubeL, cubeL, cubeL);
		}
	}
	else if (figure == CONE) {
		gluCylinder(quadObj, coneBase, 0, coneHeight, 20, 20);
	}
	else if (figure == CYLINDER) {
		gluCylinder(quadObj, cylinderR, cylinderR, cylinderHeight, 20, 20);
	}
	else if (figure==DISK) {
		gluDisk(quadObj, diskInnerR, diskOuterR, 20, 20);
	}
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	if (idleRotation) {
		xAngle += xRotation*da;
		yAngle += yRotation*da;
		zAngle += zRotation*da;
	}
	cameraAngle += cameraRotation*cameraD;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH|AUX_STENCIL);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("GUI");
	myInit();
	//glutReshapeFunc(reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutMouseFunc(mouse);
	glEnable(GL_DEPTH_TEST);

	image1 = auxDIBImageLoadA("el.bmp");
	image2 = auxDIBImageLoadA("fire.bmp");
	image3 = auxDIBImageLoadA("terra.bmp");
	image4 = auxDIBImageLoadA("wall.bmp");
	image5 = auxDIBImageLoadA("water.bmp");
	outImage = auxDIBImageLoadA("outTex.bmp");

	glGenTextures(1, &tex1);
	glGenTextures(1, &tex2);
	glGenTextures(1, &tex3);
	glGenTextures(1, &tex4);
	glGenTextures(1, &tex5);
	glGenTextures(1, &outTex);

	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		image1->sizeX,
		image1->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image1->data);

	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		image2->sizeX,
		image2->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image2->data);

	glBindTexture(GL_TEXTURE_2D, tex3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		image3->sizeX,
		image3->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image3->data);

	glBindTexture(GL_TEXTURE_2D, tex4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		image4->sizeX,
		image4->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image4->data);

	glBindTexture(GL_TEXTURE_2D, tex5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		image5->sizeX,
		image5->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image5->data);

	glBindTexture(GL_TEXTURE_2D, outTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		outImage->sizeX,
		outImage->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		outImage->data);

	glutMainLoop();
	return(0);
}