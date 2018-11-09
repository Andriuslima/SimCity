#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
    static DWORD last_idle_time;
#else
    #include <sys/time.h>
    static struct timeval last_idle_time;
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
GLfloat AspectRatio;

int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 800;

int NUM_COLORS;
int colors[10][3];

int JUMP = 2;
float userX = 0;
float userY = 0;
float userZ = 0;
float lookX = 0;
float lookY = 0;
float lookZ = -8;

int CITY_MAXWIDTH = 100;
int CITY_MAXDEPTH = 100;

int OBJ_WIDTH = 10;
int OBJ_DEPTH = 10;

ifstream inFile;
int x;

typedef struct{
    int width, depth;
    int form[100][100];
}City;

City simCity;

void readColors(char fileName[50]){
    inFile.open(fileName);
    if(!inFile){
        cout << "Não consegui abrir o arquivo " << fileName << endl;
        exit(1);
    }else{
        cout << "Arquivo "<< fileName <<" aberto" << endl;
    }

    while(inFile >> x){
        NUM_COLORS = x;
        for(int i = 0; i < NUM_COLORS; i++){
            int id, r, g, b;
            inFile >> id >> r >> g >> b;
            colors[i][0] = r;
            colors[i][1] = g;
            colors[i][2] = b;
        }
    }
    inFile.close();
}

void readCity(char fileName[50]){
    inFile.open(fileName);
    if(!inFile){
        cout << "Não consegui abrir o arquivo " << fileName << endl;
        exit(1);
    }else{
        cout << "Arquivo "<< fileName <<" aberto" << endl;
    }

    inFile >> simCity.width >> simCity.depth;

    for(int i = 0; i < simCity.width; i++){
        for(int j = 0; j < simCity.depth; j++){
            inFile >> simCity.form[i][j];
        }
    }
    inFile.close();
}

void DefineLuz(void){
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.24725f, 0.1995f, 0.07f } ;
  GLfloat LuzDifusa[]   = {0.75164f, 0.60648f, 0.22648f, 1.0f };
  GLfloat LuzEspecular[] = {0.626281f, 0.555802f, 0.366065f, 1.0f };
  GLfloat PosicaoLuz0[]  = {3.0f, 3.0f, 0.0f, 1.0f };
  GLfloat PosicaoLuz1[]  = {-3.0f, -3.0f, 0.0f, 1.0f };
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f, 1.0f };

  // ****************  Fonte de Luz 0
  glEnable(GL_COLOR_MATERIAL);

  // Habilita o uso de iluminação
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);

  // Define os parametros da Luz número Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentração do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado será o brilho. (Valores válidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

  // ****************  Fonte de Luz 1

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da Luz número Zero
  glLightfv(GL_LIGHT1, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT1, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT1, GL_POSITION, PosicaoLuz1 );
  glEnable(GL_LIGHT1);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentração do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado será o brilho. (Valores válidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,20);

}

void DrawObject(float x, float y, float z){
	glBegin ( GL_QUADS );
        // Front Face
        glNormal3f(0,0,1);
        glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
        glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
        glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
        // Back Face
        glNormal3f(0,0,-1);
        glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
        glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
        // Top Face
        glNormal3f(0,1,0);
        glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
        glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
        // Bottom Face
        glNormal3f(0,-1,0);
        glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
        glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
        // Right face
        glNormal3f(1,0,0);
        glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
        glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
        glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
        // Left Face
        glNormal3f(-1,0,0);
        glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
        glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
        glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
        glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
	glEnd();
}

void DrawCity(City c){

    for(int i = 0; i < c.width; i++){
        for(int j = 0; j < c.depth; j++){
            int objHeight = c.form[i][j];

            glPushMatrix();
                glColor3f(255.0, 0.0, 0.0);
                glTranslatef((float)(i + 10), (float)(objHeight/2) ,(float)(j + 10));
                DrawObject(OBJ_WIDTH, c.form[i][j], OBJ_DEPTH);
            glPopMatrix();
        }
    }
}

void PosicUser(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(userX, userY, userZ,
		      lookX, lookY, lookZ,
			  0.0f,1.0f,0.0f);
}

void reshape( int w, int h ){

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	PosicUser();

}

void display( void ){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);

	DrawCity(simCity);

	glutSwapBuffers();
}

void animate(){
    static float dt;
    static float AccumTime=0;

#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    AccumTime +=dt;
    if (AccumTime >=3)
    {
        cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }

    last_idle_time = time_now;

    glutPostRedisplay();
}

void keyboard ( unsigned char key, int x, int y ){
	switch ( key )
	{
    case 27:
        exit(0);
        break;
    case ' ':
        userY -= JUMP;
        glutPostRedisplay();
        break;
    case 'b':
        userY += JUMP;
        glutPostRedisplay();
        break;
    default:
        cout << key;
      break;
  }
}

void arrow_keys ( int a_keys, int x, int y ){
	switch ( a_keys )
	{
		case GLUT_KEY_UP:
			userZ -= JUMP;
            lookZ -= JUMP;
            glutPostRedisplay();
            break;
	    case GLUT_KEY_DOWN:
			userZ += JUMP;
            lookZ += JUMP;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            userX += JUMP;
            lookX += JUMP;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            userX -= JUMP;
            lookX -= JUMP;
            glutPostRedisplay();
            break;
		default:
			break;
	}
}

void init(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glShadeModel(GL_SMOOTH);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);

    // Obtem o tempo inicial
    #ifdef WIN32
        last_idle_time = GetTickCount();
    #else
        gettimeofday (&last_idle_time, NULL);
    #endif

    readColors("objects/colors.txt");
    readCity("objects/city01.txt");
}

int main(int argc, char** argv){
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );

	glutInitWindowPosition (0,0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("SimCity");

	init ();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);
	glutIdleFunc(animate);

	glutMainLoop();

	return 0;
}
