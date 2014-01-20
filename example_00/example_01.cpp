#include <cmath>
#include <iostream>
#include <stdlib.h>  // Some versions of MS-Vis C++ have broken <cstdlib>
#include <GL/glut.h> // GLUT stuff, includes OpenGL headers as well

// Global variables

const int startwinsize = 400; // Starting window width & height, in pixels
int winw, winh;               // Window width & height, in pixels, saved by reshape

// Mouse
bool mouseleftdown = false;   // True if mouse LEFT button is down.

int mousex, mousey;           // Mouse x,y coords, in GLUT format (pixels from upper-left corner).
// Only guaranteed to be valid if a mouse button is down.
// Saved by mouse, motion.

bool keep_highlight = false;
double last_x, last_y;

#define SIZE_OF_SQUARE 0.25
#define HALF_OF_SQUARE 0.125

// Other
const double pointsize = 40;  // Size of point

void DrawCircle(float cx, float cy, float r, int num_segments, float color1, float color2, float color3) 
{
    glColor3f(color1, color2, color3);
    glBegin(GL_POLYGON); 
    for(int ii = 0; ii < num_segments; ii++) 
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 

        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 

        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
}


// display
// The GLUT display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Make a small box at the mouse position, if the LEFT button is down
    if (mouseleftdown)
    {
        // Convert mouse position to OpenGL's coordinate system
        double temp_last_x = double(mousex)/winw;
        double temp_last_y = 1-double(mousey)/winh;
        bool in_x_range = (temp_last_x < (last_x + HALF_OF_SQUARE) && temp_last_x > (last_x - HALF_OF_SQUARE));
        bool in_y_range = (temp_last_y < (last_y + HALF_OF_SQUARE) && temp_last_y > (last_y - HALF_OF_SQUARE));     
        
        if(in_x_range && in_y_range && keep_highlight){
            keep_highlight = false;
        } else {
            keep_highlight = true;
            last_x = temp_last_x;
            last_y = temp_last_y;
        }
        
    }
    if(keep_highlight) {
        DrawCircle(last_x, last_y, HALF_OF_SQUARE, 50, 0, 0, 0);
    }

    glutSwapBuffers();
}


// mouse
// The GLUT mouse function
void mouse(int button, int state, int x, int y)
{
    // Save the left button state
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseleftdown = (state == GLUT_DOWN);
        glutPostRedisplay();  // Left button has changed; redisplay!
    }

    // Save the mouse position
    mousex = x;
    mousey = y;
}


// reshape
// The GLUT reshape function
void reshape(int w, int h)
{
    // Set the viewport to be the entire drawing area of the window
    glViewport(0, 0, w, h);

    // Save the window size
    winw = w;
    winh = h;

    // Set up the projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


// init
// Initializes GL states
// Called by main after window creation
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}


int main(int argc, char ** argv)
{
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Make a window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 - Simple Mouse Demo");

    // Initialize GL states & register callbacks
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    // Do something
    glutMainLoop();

    return 0;
}
