/*----------------------------------------------------------------------------------------
 *	
 *	project:	05_Overlay1
 *	author:		Rob Bateman
 *	note:		
 *				Quite often you may want to draw an overlay of 2D graphics over your 3D
 *				Data. This source file gives an example of how to do it. 
 *
 *				Essentually all you have to do is to set a 3D viewing projection, draw
 *				your 3D items, switch to orthographic display and then draw your 2D items
 *				over the top.  
 *
 *				In order to make the code a bit more readable, two functions have been 
 *				provided, Draw2D() & Draw3D(). These are called from within the Draw() 
 *				function. 
 */

/*----------------------------------------------------------------------------------------
 *	Includes
 */


#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <GL/glut.h>



/*----------------------------------------------------------------------------------------
 *	Global Variables
 */

/*
 *	A structure to represent the mouse information
 */
struct Mouse 
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

	/*
	 *	These two variables are a bit odd. Basically I have added these to help replicate
	 *	the way that most user interface systems work. When a button press occurs, if no
	 *	other button is held down then the co-ordinates of where that click occured are stored.
	 *	If other buttons are pressed when another button is pressed it will not update these
	 *	values. 
	 *
	 *	This allows us to "Set the Focus" to a specific portion of the screen. For example,
	 *	in maya, clicking the Alt+LMB in a view allows you to move the mouse about and alter
	 *	just that view. Essentually that viewport takes control of the mouse, therefore it is
	 *	useful to know where the first click occured.... 
	 */
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

/*
 *	rename the structure from "struct Mouse" to just "Mouse"
 */
typedef struct Mouse Mouse;

/*
 *	Create a global mouse structure to hold the mouse information.
 */
Mouse TheMouse = {0,0,0,0,0};

/*
 *	Variables to hold the current size of the window.
 */
int winw = 640;
int winh = 480;


/*----------------------------------------------------------------------------------------
 *	Button Stuff
 */

/*
 *	We will define a function pointer type. ButtonCallback is a pointer to a function that
 *	looks a bit like this :
 *
 *	void func() { 
 *	}
 */
typedef void (*ButtonCallback)();

/*
 *	This is a simple structure that holds a button.
 */
struct Button 
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};
typedef struct Button Button;



/*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	an the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */
void TheButtonCallback()
{
	printf("I have been called\n");
}

/*----------------------------------------------------------------------------------------
 *	This is the button visible in the viewport. This is a shorthand way of 
 *	initialising the structure's data members. Notice that the last data
 *	member is a pointer to the above function. 
 */
Button MyButton = {5,5, 100,25, 0,0, "Button", TheButtonCallback };


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap fonts.
 *	\param	font	-	the font to use. it can be one of the following : 
 *
 *					GLUT_BITMAP_9_BY_15		
 *					GLUT_BITMAP_8_BY_13			
 *					GLUT_BITMAP_TIMES_ROMAN_10	
 *					GLUT_BITMAP_TIMES_ROMAN_24	
 *					GLUT_BITMAP_HELVETICA_10	
 *					GLUT_BITMAP_HELVETICA_12	
 *					GLUT_BITMAP_HELVETICA_18	
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button 
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
int ButtonClickTest(Button* b,int x,int y) 
{
	if( b) 
	{
		/*
		 *	If clicked within button area, then return true
		 */
	    if( x > b->x      && 
			x < b->x+b->w &&
			y > b->y      && 
			y < b->y+b->h ) {
				return 1;
		}
	}

	/*
	 *	otherwise false.
	 */
	return 0;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(Button *b,int x,int y)
{
	if(b) 
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) && 
			ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse click was within the buttons client area, 
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPassive(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw. 
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to draw.
 */
void ButtonDraw(Button *b)
{
	int fontx;
	int fonty;

	if(b)
	{
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted) 
			glColor3f(0.7f,0.7f,0.8f);
		else 
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button. 
		 */
		if (b->state) 
			glColor3f(0.4f,0.4f,0.4f);
		else 
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state) 
			glColor3f(0.8f,0.8f,0.8f);
		else 
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);


		/*
		 *	Calculate the x and y coords for the text string in order to center it.
		 */
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label)) / 2 ;
		fonty = b->y + (b->h+10)/2;

		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) {
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 */

void Init()
{
	glEnable(GL_LIGHT0);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
	gluLookAt(0,1,5,0,0,0,0,1,0);
	glutSolidTeapot(1);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw an overlay over the 3D scene.
 *	This will be used to draw our fonts, buttons etc......
 */
void Draw2D()
{
	ButtonDraw(&MyButton);
}

/*----------------------------------------------------------------------------------------
 *	This is the main display callback function. It sets up the drawing for 
 *	The 3D scene first then calls the Draw3D() function. After that it switches to 
 *	an orthographic projection and calls Draw2D().
 */
void Draw()
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

	/*
	 *	Enable lighting and the z-buffer
	 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(winh==0)?(1):((float)winw/winh),1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 3D elements in the scene
	 */
	Draw3D();

	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 2D overlay
	 */
	Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *	This function is called when the window is resized. All this does is simply 
 *	store the new width and height of the window which are then referenced by
 *	the draw function to set the correct viewing transforms 
 */
void Resize(int w, int h)
{
	winw = w;
	winh = h;

	/*
	 *	Allow drawing in full region of the screen
	 */
	glViewport(0,0,w,h);
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever a mouse button is pressed or released
 *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
 *	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
 *						or pressed respectivly. 
 *	\param	x		-	the x-coord of the mouse cursor.
 *	\param	y		-	the y-coord of the mouse cursor.
 */
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN) 
	{
		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}

		/*
		 *	Which button was pressed?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			ButtonPress(&MyButton,x,y);
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 1;
			break;
		}
	}
	else 
	{
		/*
		 *	Which button was released?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(&MyButton,x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Note that I'm not using a glutPostRedisplay() call here. The passive motion function 
	 *	is called at a very high frequency. We really don't want much processing to occur here.
	 *	Redrawing the screen every time the mouse moves is a bit excessive. Later on we 
	 *	will look at a way to solve this problem and force a redraw only when needed. 
	 */
}

/*----------------------------------------------------------------------------------------
 *
 */
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);
	glutCreateWindow("03 - Mouse Motion");

	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	Init();

	glutMainLoop();
}








