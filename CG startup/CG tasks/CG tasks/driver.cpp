#include <windows.h>
#include <CommDlg.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "AlgorithmFactory.h"

#include <gl/gl.h>
#include <gl/glut.h>

static vector<Point> inPoints, outPoints;
static vector<Line>  inLines , outLines;

#define POINT_MODE 1
#define LINE_MODE  2

#define POINT_SIZE 3

#define DEFAULT_POINT_COLOR_R 0.0
#define DEFAULT_POINT_COLOR_G 1.0
#define DEFAULT_POINT_COLOR_B 0.0

#define DEFAULT_LINE_COLOR_R 0.0
#define DEFAULT_LINE_COLOR_G 0.0
#define DEFAULT_LINE_COLOR_B 0.0

#define OUT_POINT_COLOR_R 1.0
#define OUT_POINT_COLOR_G 0.0
#define OUT_POINT_COLOR_B 0.0

#define OUT_LINE_COLOR_R 1.0
#define OUT_LINE_COLOR_G 0.0
#define OUT_LINE_COLOR_B 0.0


int DRAWING_MODE;
bool isDrawing;

int width = 600;
int height = 600;

int Point::drawID = 1;
int Line::drawID  = 1;
Point AngleComparer::about = Point(0,0);

void writePoint(const Point& p)
{
	//Not to be drawn point
	if(!p.pointDrawID)return;
	char str[10];
	glRasterPos2f(p.x+POINT_SIZE, p.y+POINT_SIZE);
	sprintf(str,"%d", p.pointDrawID);
	for(unsigned i = 0; i < strlen(str); i ++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}
void writeLine(const Line& l)
{
	if(!l.lineDrawID)return;
	char str[10];
	glRasterPos2f(l.start.x+POINT_SIZE,l.start.y+POINT_SIZE);
	sprintf(str, "%d", l.lineDrawID);
	for(unsigned i = 0; i < strlen(str); i ++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}
void drawPoints(const vector<Point>& points, float r, float g, float b)
{
	//glClearColor(1,1,1,1);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(r,g,b);
	glPointSize(POINT_SIZE);
	glBegin(GL_POINTS);
	for(unsigned i = 0; i < points.size(); i ++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();

	for(unsigned i = 0; i < points.size(); i ++)
	{
		glColor3f(r, g, b);
		writePoint(points[i]);
	}
	glFlush();
}

void drawLines(const vector<Line>& lines, float r, float g, float b)
{
	for(unsigned i = 0; i < lines.size(); i ++)
	{
		glBegin(GL_LINES);
		glColor3f(r,g,b);
		glLineWidth(5.0);
		glVertex2f(lines[i].start.x, lines[i].start.y);
		glVertex2f(lines[i].end.x, lines[i].end.y);
		glEnd();
		writeLine(lines[i]);
	}
}

/**
Open file dialog, returns empty string if user cancels the operation.
*/
string openFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.lpstrFilter = "Text\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)==TRUE);
	//hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
	//0, (LPSECURITY_ATTRIBUTES) NULL,
	//OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	//(HANDLE) NULL);
	return ofn.lpstrFile;
}
/**
Sets the logical coordinate system we will use to specify
our drawings.
*/
void SetTransformations() {
	//set up the logical coordinate system of the window: [-100, 100] x [-100, 100]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}
/**
Handles the paint event. This event is triggered whenever
our displayed graphics are lost or out-of-date.
ALL rendering code should be written here.
*/
void OnDisplay() {
	//set the background color to white
	glClearColor(1, 1, 1, 1);
	//fill the whole color buffer with the clear color
	glClear(GL_COLOR_BUFFER_BIT);
	SetTransformations();

	drawPoints(inPoints,DEFAULT_POINT_COLOR_R,DEFAULT_POINT_COLOR_G,DEFAULT_POINT_COLOR_B);
	drawLines(inLines,DEFAULT_LINE_COLOR_R,DEFAULT_LINE_COLOR_G,DEFAULT_LINE_COLOR_B);
	//force previously issued OpenGL commands to begin execution
	glFlush();
}
/**
Handles the mouse press event. This event is triggered whenever
we click with mouse.
Button: indicates which mouse button was clicked left, right or middle
State:  indicates wether the mouse button is up or down.
X & Y:  are the mouse coordinates, they are sometimes incorrect
when the window is scaled.
(This can be fixed by updating the gluOrtho2D to match the window size.)
*/
void processMouse(int button, int state, int x, int y)
{
	float wx = x;
	float wy = height - y;
	//cout<<wx<<" "<<wy<<endl;
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(DRAWING_MODE == POINT_MODE)
		{
			inPoints.push_back(Point(wx,wy,1));
		}
		if(DRAWING_MODE == LINE_MODE)
		{
			if(isDrawing == false)
			{
				inLines.push_back(Line(Point(wx,wy),Point(wx,wy),1));
				isDrawing = true;
			}
		}
		OnDisplay();
	}
	if(button==GLUT_LEFT_BUTTON && state == GLUT_UP && DRAWING_MODE==LINE_MODE&&isDrawing==true)
		isDrawing = false;
}
/**
Handles the mouse move event. This event is triggered whenever we move with
the mouse while we are pressing at any of its buttons.
X & Y: Mouse coordinates
*/
void processMouseMove(int x, int y)
{
	float wx = x;
	float wy = height - y;
	if(isDrawing)
	{
		inLines[inLines.size()-1].end = Point(wx,wy);
		OnDisplay();
	}
}
/**
Handles the keyboard press event. This event is triggered whenever any keyboard
key is pressed.
key: the key pressed
X & Y: mouse location in window relative coordinates when the key was pressed.
*/
void processKeyboard(unsigned char key, int x, int y)
{
	if(key == ' ')
	{
		inPoints.clear();
		inLines.clear();
		outPoints.clear();
		outLines.clear();
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		Point::drawID=1;
	}
	if(key == 27)//Escape
	{
		OnDisplay();
	}
}
/**
Handles resizing of the window. This event is triggered whenever the
window is resized such as by user interaction.
*/
void reshape(int newWidth, int newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, newWidth, 0, newHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth, height = newHeight;
}
void selectDrawingMode(int choice)
{
	if(choice==0)
		DRAWING_MODE = POINT_MODE;
	if(choice==1)
		DRAWING_MODE = LINE_MODE;
}
void openFileType(int choice)
{
	string fileName = openFile();
	if(fileName=="")
		return;
	ifstream input(fileName);
	if(choice == 0)//Points
	{
		float x, y;
		while(!input.eof())
		{
			input>>x>>y;
			inPoints.push_back(Point(x,y,1));
		}

		if(inPoints.size()>=2)
			if(inPoints[inPoints.size()-1]==inPoints[inPoints.size()-2])
				inPoints.pop_back();
	}
	if(choice == 1)//Lines
	{
		float x1, y1, x2, y2;
		while(!input.eof())
		{
			input>>x1>>y1>>x2>>y2;
			inLines.push_back(Line(Point(x1,y1),Point(x2,y2),1));
		}
		if(inLines.size()>=2)
			if(inLines[inLines.size()-1]==inLines[inLines.size()-2])
				inLines.pop_back();
	}
	OnDisplay();
}
void mainMenu(int choice){}
string getTimeNow()
{
	srand(time(0));

	time_t t = time(0);
	tm* now = localtime(&t);
	stringstream ss;
	ss<<
		now->tm_year+1900<<"-"<<
		now->tm_mon+1<<"-"<<
		now->tm_mday<<"-"<<
		now->tm_hour<<","<<
		now->tm_min<<","<<
		now->tm_sec;
	return ss.str();
	//return "koko-,1";
}
void writePointsInFile(char* path)
{
	strcat(path, "\\Points.txt");
	ofstream outFilePoints(path);
	for(unsigned i = 0; i < inPoints.size(); i ++)
		outFilePoints<<inPoints[i].x<<" "<<inPoints[i].y<<endl;
	while(path[strlen(path)-1]!='\\')
		path[strlen(path)-1] = '\0';
	outFilePoints.flush();
	outFilePoints.close();
}
void writeLinesInFile(char* path)
{
	strcat(path, "\\Lines.txt");
	ofstream outFileLines(path);
	for(unsigned i = 0; i < inLines.size(); i ++)
		outFileLines<<inLines[i].start.x<<" "<<inLines[i].start.y<<" "
					 <<inLines[i].end.x<<" "<<inLines[i].end.y<<" "<<endl;
	while(path[strlen(path)-1]!='\\')
		path[strlen(path)-1] = '\0';
	outFileLines.flush();
	outFileLines.close();
}
void saveData()
{
	string now = "\\" + getTimeNow();
	HMODULE hModule = GetModuleHandle(NULL);
	if(hModule == NULL)
		return;
	char ownPath[MAX_PATH];
	GetModuleFileName(hModule, ownPath, sizeof(ownPath));
	while(ownPath[strlen(ownPath)-1]!='\\')
		ownPath[strlen(ownPath)-1] = '\0';
	//If log directory doesn't exist
	strcat(ownPath,"log");
	
	CreateDirectory(ownPath,NULL);
	strcat(ownPath, now.c_str());
	CreateDirectory(ownPath, NULL);

	writePointsInFile(ownPath);
	writeLinesInFile(ownPath);
}
void runAlgorithms(int choice)
{
	saveData();
	Algorithm* algorithm = AlgorithmFactory::createAlgorithm(algorithmsNames[choice]);
	outPoints.clear(), outLines.clear();
	algorithm->run(inPoints,inLines,outPoints,outLines);
	//MessageBox(NULL,"DONE ya kbeer :D","Warning",MB_OK);
	//Todo: calculate time spent in the algorithm..
	drawPoints(outPoints,OUT_POINT_COLOR_R,OUT_POINT_COLOR_G,OUT_POINT_COLOR_B);
	drawLines(outLines,OUT_LINE_COLOR_R,OUT_LINE_COLOR_G,OUT_LINE_COLOR_B);
	glFlush();
}
void initMenus()
{
	int drawingSubmenu, openFileSubmenu, algorithmsSubmenu;

	drawingSubmenu = glutCreateMenu(selectDrawingMode);
	glutAddMenuEntry("Point", 0);
	glutAddMenuEntry("Line", 1);

	openFileSubmenu = glutCreateMenu(openFileType);
	glutAddMenuEntry("Points",0);
	glutAddMenuEntry("Lines", 1);

	algorithmsSubmenu = glutCreateMenu(runAlgorithms);

	for(unsigned i = 0; i < ALGORITHMS_NUM; i ++)
		glutAddMenuEntry(algorithmsNames[i].c_str(),i);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("Drawing Mode", drawingSubmenu);
	glutAddSubMenu("Run Algorithm", algorithmsSubmenu);
	glutAddSubMenu("Import Data", openFileSubmenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
void InitGraphics(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	//Create an 600x600 window with its top-left corner at pixel (100, 100)
	glutInitWindowPosition(100, 100); //pass (-1, -1) for Window-Manager defaults
	glutInitWindowSize(width, height);
	glutCreateWindow("Geometry Package");
	//OnDisplay will handle the paint event
	glutReshapeFunc(reshape);
	glutDisplayFunc(OnDisplay);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseMove);
	glutKeyboardFunc(processKeyboard);
	initMenus();
	//initially drawing mode is points
	DRAWING_MODE = POINT_MODE;
	isDrawing = false;
	glutMainLoop();
}

int main(int argc, char* argv[]) {
	InitGraphics(argc, argv);
	return 0;
}