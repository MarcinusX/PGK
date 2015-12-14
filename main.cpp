#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "House.h"
void pressedKey(int key, int x, int y);
void Reshape(int width, int height);
void Draw();
void idle();
void drawSquareFloor(int sideSize);
void move(int direction);
void turn(int directionHorizontally, int directionVertically);
void releasedKey(int key, int x, int y);
void onMouseMoved(int x, int y);
void setSkyColor(int color);
void setFloorColor(int color);
float* getColorArray(int color);
void createMenus();
void initColors();
float* setValuesOfColorArray(float R, float G, float B);
void onMouseEntry(int state);
void pointlessFunction(int a);
void onReallyPassiveMouseMoved(int x, int y);
void onMouseButtonPressed(int button, int state, int x, int y);
void drawHouses();
void pressedKeyboardKey(unsigned char key, int x, int y);
void releasedKeyboardKey(unsigned char key, int x, int y);
void initStars();
void drawLight1();
void drawLight0();
void drawLight2();
void drawLight3();

enum kolor{
czarny, czerwony, niebieski, zielony, bialy, blekitny, zolty, fioletowy
};

using namespace std;

bool isTurningNormal=false; //DO USTAWIENIA
int windowHeight=600, windowWidth=800;
float angle=1.6, angleY=-0.3, ratio;
float positionX=-5, positionY=6.75, positionZ=10;
float rotateX=0, rotateY=-0.5, rotateZ=0;
int moveDirection=0, rotateDirection=0;
float moveSpeed=0.1, rotateSpeed=0.005;
float *skyColor, *roofColor, *wallColor, *floorColor;
bool menuOpen=false, isLeftButtonPressed=false;
int mouseX=windowWidth/2, mouseY=windowHeight/2;
float step=0;
int houseSize=1, houseShape=cube;
bool light0_enabled=true, light1_enabled=true, light2_enabled=true, light3_enabled=true;
float light_angle=0;

void refreshLooking() {
    glLoadIdentity();
    gluLookAt(positionX, positionY, positionZ,
              positionX+rotateX, positionY+rotateY, positionZ+rotateZ,
              0,1,0);   //look straight
}

int main(int argc, char *argv[])
{
    initColors();
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(200,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Domki");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_COLOR_MATERIAL);
    glutIdleFunc(idle);
    if (isTurningNormal)
        glutPassiveMotionFunc(onMouseMoved);
    else {
        glutMotionFunc(onMouseMoved);
        glutPassiveMotionFunc(onReallyPassiveMouseMoved);
    }

    createMenus();
    glClearColor(1,0,0,0);
    glutKeyboardFunc(pressedKeyboardKey);
    glutKeyboardUpFunc(releasedKeyboardKey);
    glutSpecialFunc(pressedKey);
    glutSpecialUpFunc(releasedKey);
    glutEntryFunc(onMouseEntry);
    glutMouseFunc(onMouseButtonPressed);
    glutMainLoop();
    return EXIT_SUCCESS;
}

void idle() {
    glutPostRedisplay();
}

void Draw() {
    glClearColor(skyColor[0],skyColor[1],skyColor[2],0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSquareFloor(100);

    move(moveDirection);
    turn(rotateDirection,0);

    drawHouses();

    light_angle+=2;
    if (light0_enabled)
        drawLight0();
    if (light1_enabled)
        drawLight1();
    if (light2_enabled)
        drawLight2();
    if (light3_enabled)
        drawLight3();
    glutSwapBuffers();
}

void Reshape(int width, int height) {
    windowWidth=width;
    windowHeight=height;

    if (height==0)
       height=1;

    ratio = 1.0 * width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    refreshLooking();
             //  glutPostRedisplay();
}

void initColors() {
    floorColor=getColorArray(zielony);
    skyColor=getColorArray(blekitny);
    roofColor=getColorArray(fioletowy);
    wallColor=getColorArray(zolty);
}

//****DRAWING FUNCTIONS*****//
void initStars() {
    if (skyColor[0]==0 && skyColor[1]==0 && skyColor[2]==0)
    {
        glPointSize(2);
        glColor3f(1,1,1);
        for (int i=-1000; i<1000; i+=10)
        {
            for (int j=-1000; j<1000; j+=10)
            {
                glBegin(GL_POINTS);
                glVertex3f(i,100,j);
                glEnd();
            }
        }
    }
}

void drawLight0(){
    glPushMatrix();
    float x[]={-0.5,1,-1,0};
    glLightfv(GL_LIGHT0,GL_POSITION,x);
    refreshLooking();
    glPopMatrix();
}
void drawLight1(){
    glPushMatrix();
    float position[]={10,5,0,1};
    float ambient[]={0.2,0.2,0.2,1};
    float diffuse[]={1,1,1,1};
    float specular[]={1,1,1,1};
    float spotDirection[]={0,-1,1};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 64);
    glLightfv(GL_LIGHT1, GL_POSITION,position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    refreshLooking();
    glPopMatrix();
}
void drawLight2(){
    glPushMatrix();
    float position[]={10,5,20,1};
    float ambient[]={0.2,0.2,0.2,1};
    float diffuse[]={1,1,1,1};
    float specular[]={1,1,1,1};
    float spotDirection[]={0,-1,-1};
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 64);
    glLightfv(GL_LIGHT2,GL_POSITION,position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    refreshLooking();
    glPopMatrix();
}

void drawLight3(){

    glPushMatrix();
    float position[]={0,0,0,1};
    float ambient[]={0.5,0.5,0.5,0.5};
    float diffuse[]={0.2,0.2,0.2,1};
    float specular[]={1,1,1,1};
    float spotDirection[]={0,-1,-1};
    glTranslatef(10,1,10);
    glRotatef(light_angle,0,1,0);
    glTranslatef(3,0,0);
        glPointSize(5);
        glColor3f(1,1,1);
        glBegin(GL_POINTS);
        glVertex3f(0,0,0);
        glEnd();
    //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    //glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0f);
    //glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 64);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 1);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);
    glLightfv(GL_LIGHT3,GL_POSITION,position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
    refreshLooking();
    glPopMatrix();
}

void drawHouses() {
    int housesInRow=10;
    int distanceBetween=5;
    for (int i=0; i<housesInRow; i++)
    {
        for (int j=0; j<housesInRow; j++)
        {
            glPushMatrix();
            glTranslatef(distanceBetween*i,0,distanceBetween*j);
            House house(houseSize,houseShape,wallColor,roofColor);
            house.display();
            glPopMatrix();
        }
    }
}

void drawSquareFloor(int sideSize) {
    int startingX=-50;
    int startingZ=-50;

    glColor3fv(floorColor);
    for (int x=startingX; x<startingX+sideSize; x++)
    {
        for(int z=startingZ; z<startingZ+sideSize; z++)
        {
            glBegin(GL_QUADS);
                glVertex3f(x,0,z);
                glVertex3f(x+1,0,z);
                glVertex3f(x+1,0,z+1);
                glVertex3f(x,0,z+1);
            glEnd();
        }
    }
}

//*****KEYBOARD FUNCTIONS*****//
void pressedKeyboardKey(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'a':
        case 'A': rotateDirection=-1; break;
        case 'd':
        case 'D': rotateDirection=1; break;
        case 'w':
        case 'W': moveDirection=1; break;
        case 's':
        case 'S': moveDirection=-1; break;
        case 'q':
        case 'Q': positionY+=0.1; break;
        case 'e':
        case 'E': positionY-=0.1; break;
    }
}

void releasedKeyboardKey(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'a':
        case 'A':
        case 'd':
        case 'D': rotateDirection=0; break;
        case 'w':
        case 'W':
        case 's':
        case 'S': moveDirection=0; break;
    }
}

void pressedKey(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_LEFT: rotateDirection=-1; break;
        case GLUT_KEY_RIGHT: rotateDirection=1; break;
        case GLUT_KEY_UP: moveDirection=1; break;
        case GLUT_KEY_DOWN: moveDirection=-1; break;
        case GLUT_KEY_F1:
            if (light0_enabled)
                glDisable(GL_LIGHT0);
            else
                glEnable(GL_LIGHT0);
            light0_enabled=!light0_enabled;
            break;
        case GLUT_KEY_F2:
            if (light1_enabled)
                glDisable(GL_LIGHT1);
            else
                glEnable(GL_LIGHT1);
            light1_enabled=!light1_enabled;
            break;
        case GLUT_KEY_F3:
            if (light2_enabled)
                glDisable(GL_LIGHT2);
            else
                glEnable(GL_LIGHT2);
            light2_enabled=!light2_enabled;
            break;
        case GLUT_KEY_F4:
            if (light3_enabled)
                glDisable(GL_LIGHT3);
            else
                glEnable(GL_LIGHT3);
            light3_enabled=!light3_enabled;
            break;
    }
}

void releasedKey(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT: rotateDirection=0; break;
        case GLUT_KEY_DOWN:
        case GLUT_KEY_UP: moveDirection=0; break;
    }
}

//*****MOUSE FUNCTIONS*****//
void onReallyPassiveMouseMoved(int x, int y) {
    mouseX=x;
    mouseY=y;
}

void onMouseMoved(int x, int y) {
    if(isLeftButtonPressed || isTurningNormal)
    {
        if (!menuOpen)
        {
           int horizontalDirection = isTurningNormal ? x-mouseX : mouseX-x ;
            int verticalDirection = isTurningNormal ? mouseY-y : y-mouseY ;
            turn(horizontalDirection,verticalDirection);
        }
        else
            menuOpen=false;

    }
    mouseX=x;
    mouseY=y;
}

void onMouseEntry(int state) {
    if (state==GLUT_LEFT)
    {
        skyColor=getColorArray(czarny);
    }
    else if (state==GLUT_ENTERED)
    {
        skyColor=getColorArray(blekitny);
    }

}

void onMouseButtonPressed(int button, int state, int x, int y) {

    isLeftButtonPressed = state == GLUT_DOWN;
    if (isLeftButtonPressed){
        mouseX=x;
        mouseY=y;
    }
}

//*****MOVEMENT FUNCTIONS*****//
void move(int direction) {
    positionX=positionX+direction*rotateX*moveSpeed;
    positionZ=positionZ+direction*rotateZ*moveSpeed;
    if (direction){
        float szybkosc_skakania=0.3;
        float wysokosc_skakania=0.025;
        step+=szybkosc_skakania;
        positionY=positionY+sin(step)*wysokosc_skakania;
    }

    refreshLooking();
}

void turn(int directionHorizontally, int directionVertically) {

    angle+=directionHorizontally*rotateSpeed;
    angleY+=directionVertically*rotateSpeed;

    if (angleY>M_PI_2 || angleY<-M_PI_2)
        angleY-=directionVertically*rotateSpeed;

    rotateY=sin(angleY);
    rotateX=sin(angle)*cos(angleY);
    rotateZ=-cos(angle)*cos(angleY);
    refreshLooking();
}

//*****UTILS*****//
float* getColorArray(int color) {
    switch (color)
    {
        case czerwony: { return setValuesOfColorArray(1,0,0); }
        case niebieski: { return setValuesOfColorArray(0,0,1); }
        case zielony: { return setValuesOfColorArray(0,1,0); }
        case bialy: { return setValuesOfColorArray(1,1,1); }
        case blekitny: { return setValuesOfColorArray(0,1,1); }
        case zolty: { return setValuesOfColorArray(1,1,0); }
        case fioletowy: { return setValuesOfColorArray(0.7,0,0.7); }
        case czarny:
            default: { return setValuesOfColorArray(0,0,0); }
    }
}
float* setValuesOfColorArray(float R, float G, float B) {
    float* tab=new float[3];
    tab[0]=R;
    tab[1]=G;
    tab[2]=B;
    return tab;
}
void setWallColor(int color) { wallColor=getColorArray(color); }
void setRoofColor(int color) { roofColor=getColorArray(color); }
void setFloorColor(int color) { floorColor=getColorArray(color); }
void setSkyColor(int color) { skyColor=getColorArray(color); }
void setHouseSize(int size) { houseSize=size; }
void setHouseShape(int shape) { houseShape=shape; }


//*****MENU FUNCTIONS****//
void menu_setWallColor(int color) {
    setWallColor(color);
    menuOpen=true;
}
void menu_setRoofColor(int color) {
    setRoofColor(color);
    menuOpen=true;
}
void menu_setSkyColor(int color) {
    setSkyColor(color);
    menuOpen=true;
}
void menu_setFloorColor(int color) {
    setFloorColor(color);
    menuOpen=true;
}
void menu_setHouseShape(int shape) {
    setHouseShape(shape);
    menuOpen=true;
}
void menu_setHouseSize(int size) {
    setHouseSize(size);
    menuOpen=true;
}

void addColorMenuEntries() {
    glutAddMenuEntry("Czerwony",czerwony);
    glutAddMenuEntry("Zielony",zielony);
    glutAddMenuEntry("Niebieski",niebieski);
    glutAddMenuEntry("Zolty",zolty);
    glutAddMenuEntry("Czarny", czarny);
    glutAddMenuEntry("Fioletowy", fioletowy);
    glutAddMenuEntry("Bialy", bialy);
    glutAddMenuEntry("Blekitny", blekitny);
}

void createMenus() {
    int houseMenu = glutCreateMenu(pointlessFunction);
    int floorMenu = glutCreateMenu(menu_setFloorColor);
    int skyMenu = glutCreateMenu(menu_setSkyColor);
    addColorMenuEntries();

    glutSetMenu(floorMenu);
    addColorMenuEntries();

    glutSetMenu(houseMenu);
    int houseSizeMenu = glutCreateMenu(menu_setHouseSize);
    glutAddMenuEntry("1",1);
    glutAddMenuEntry("2",2);
    glutAddMenuEntry("3",3);
    int houseShapeMenu = glutCreateMenu(menu_setHouseShape);
    glutAddMenuEntry("Kosc",cube);
    glutAddMenuEntry("Czajnik",teapot);
    glutAddMenuEntry("Kula", sphere);
    int wallColorMenu = glutCreateMenu(menu_setWallColor);
    addColorMenuEntries();
    int roofColorMenu = glutCreateMenu(menu_setRoofColor);
    addColorMenuEntries();

    glutSetMenu(houseMenu);
    glutAddSubMenu("Zmien rozmiar", houseSizeMenu);
    glutAddSubMenu("Zmien ksztalt", houseShapeMenu);
    glutAddSubMenu("Zmien kolor scian", wallColorMenu);
    glutAddSubMenu("Zmien kolor dachu", roofColorMenu);

    glutCreateMenu(pointlessFunction);

    glutAddSubMenu("Zmien ziemie",floorMenu);
    glutAddSubMenu("Zmien tlo", skyMenu);
    glutAddSubMenu("Zmien domki", houseMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void pointlessFunction(int a) {}