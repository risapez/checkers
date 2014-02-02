// CS184 Simple OpenGL Example

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

using namespace std;


//****************************************************
// General Functions
//****************************************************

void DrawCircle(float circle_x, float circle_y, float radius, int number_of_segments, float color1, float color2, float color3) {
    glColor3f(color1, color2, color3);
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < number_of_segments; i++)  {
        float theta = 2.0f * PI * float(i) / float(number_of_segments);
        float x = radius*cosf(theta);
        float y = radius*sinf(theta);
        glVertex2f(x + circle_x, y + circle_y);
    }
    glEnd();
}

//****************************************************
// Some Classes
//****************************************************
class Viewport {
public:
    int w, h; // width and height
};

class Piece {
public:
    Piece();
    Piece(double, double, int);
    
    double circ;
    double x, y;
    int black_or_not;
    int border;
    
    void set_color(int);
    void change_location(double, double);
    void draw();
};

Piece::Piece() {
    circ = 0.2;
    x = 0.0;
    y = 0.0;
    black_or_not = 1;
    border = 0;
}

Piece::Piece(double new_x, double new_y, int black) {
    circ = 0.2;
    x = new_x;
    y = new_y;
    black_or_not = black;
    border = 0;
}

void Piece::set_color (int black) {
    black_or_not = black;
}

void Piece::change_location (double new_x, double new_y) {
    x = new_x;
    y = new_y;
}

void Piece::draw() {
    if(border && black_or_not){
        DrawCircle(x,y,circ/2,50, 0.7, 0.4, 0.4);
    } else if(border && !black_or_not){
        DrawCircle(x,y,circ/2,50, 0.7, 0.7, 1.0);
    }
    else if(black_or_not) {
        DrawCircle(x,y,circ/2, 50, 0.7, 0.0, 0.1);
    } else {
        DrawCircle(x,y,circ/2,50, 0.5, 0.5, 1.0);
    }
}


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;
double x_coor[32];
double y_coor[32];

const int startwinsize = 400;
int winw, winh;
bool mouseleftdown = false;
double mousex, mousey;
bool new_click = true;

bool keep_highlight = false;
double last_x, last_y;

#define SIZE_OF_SQUARE 0.25
#define HALF_OF_SQUARE 0.125

int selected_piece = -1;
int old_selected_piece = -1;

Piece piece_array[24];


double curr_x;
double curr_y;

int turn = 1; // white = 0, black = 1

std::map<std::pair<double,double>, int> coord_to_square;


//****************************************************
// mouse function
//****************************************************

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseleftdown = (state == GLUT_DOWN);
        glutPostRedisplay();
    }
    
    mousex = x;
    mousey = y;
}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
    viewport.w = w;
    viewport.h = h;
    
    // Save the window size
    winw = w;
    winh = h;
    
    
    glViewport(0,0,viewport.w,viewport.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-1, 1, -1, 1, 1, -1);
    
    //------------------------------------------------------------
}




void setup_coord_system() {
    int a;
    for(a = 0; a < 32; a+=8){
        x_coor[a] = -0.875;
        x_coor[a+1] = -0.375;
        x_coor[a+2] = 0.125;
        x_coor[a+3] = 0.625;
        x_coor[a+4] = -0.625;
        x_coor[a+5] = -0.125;
        x_coor[a+6] = 0.375;
        x_coor[a+7] = 0.875;
    }
    
    for(a = 0; a < 4; a+=1){
        y_coor[a] = 0.875;
        y_coor[a+4] = 0.625;
        y_coor[a+8] = 0.375;
        y_coor[a+12] = 0.125;
        y_coor[a+16] = -0.125;
        y_coor[a+20] = -0.375;
        y_coor[a+24] = -0.625;
        y_coor[a+28] = -0.875;
    }
    
}



void draw_board() {
    // WHITE BACKGROUND
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POLYGON);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    
    double x = -1.0;
    double y;
    int y_count = 1;
    
    for(y = 1.0; y > -1.0; y -= 0.25){
        if(y_count%2 == 0){
            x = -0.75;
        } else {
            x = -1.0;
        }
        for(x; x < 1.0; x += 0.5) {
            glColor3f(0.0f,0.0f,0.0f);
            glBegin(GL_POLYGON);
            glVertex3d(x, y-0.25, 0.0f);
            glVertex3d(x, y, 0.0f);
            glVertex3d(x+0.25, y, 0.0f);
            glVertex3d(x+0.25, y-0.25, 0.0f);
            glEnd();
        }
        y_count += 1;
    }
}


void setup_board(Piece piece_array[]) {
    for(int i = 0; i < 24; i++) {
        int loc = i;
        int black = 1;
        if(i > 11){
            loc = i + 8;
            black = 0;
        }
        piece_array[i].x = x_coor[loc];
        piece_array[i].y = y_coor[loc];
        piece_array[i].set_color(black);
    }
}


void draw_pieces(Piece piece_array[]) {
    for(int i = 0; i < 24; i++) {
        piece_array[i].draw();
    }
}

int checklocation(Piece piece_array[], int turn, double x, double y) {
    for(int i = 0; i < 24; i++) {
        if(piece_array[i].black_or_not == turn){
            if((x < (piece_array[i].x + HALF_OF_SQUARE)) && (x > (piece_array[i].x -HALF_OF_SQUARE))) {
                if ((y < (piece_array[i].y + HALF_OF_SQUARE)) && (y > (piece_array[i].y - HALF_OF_SQUARE))) {
                    return i;
                }
            }
        }
    }
    return -1;
}

void move_if_valid(Piece* selected, double x, double y, Piece piece_array[]) {
    // check what square the selected number is on
    int selected_square = coord_to_square[std::make_pair(selected->x, selected->y)];
    
    int valid_squares[4] = {-1, -1, -1, -1};
    
    if(selected->black_or_not == 1) {
        //black
        if(selected_square % 8 == 0 || (selected_square+1) % 8 == 0) {
            valid_squares[0] = selected_square+4;
        }
        else {
            int row = selected_square/4;
            if(row % 2 == 0) {
                valid_squares[0] = selected_square+3;
                valid_squares[1] = selected_square+4;
            } else {
                valid_squares[0] = selected_square+4;
                valid_squares[1] = selected_square+5;
            }
        }
    }
    
    if(selected->black_or_not == 0) {
        //black
        if(selected_square % 8 == 0 || (selected_square+1) % 8 == 0) {
            valid_squares[0] = selected_square-4;
        }
        else {
            int row = selected_square/4;
            if(row % 2 == 0) {
                valid_squares[0] = selected_square-4;
                valid_squares[1] = selected_square-5;
            } else {
                valid_squares[0] = selected_square-3;
                valid_squares[1] = selected_square-4;
            }
        }
    }
    
    
    // next check if x,y is in the range of any of the valid squares
    for(int i = 0; i < 4; i++) {
        if(valid_squares[i] != -1) {
            double valid_x = x_coor[valid_squares[i]];
            double valid_y = y_coor[valid_squares[i]];
            if(x < valid_x+HALF_OF_SQUARE && x > valid_x-HALF_OF_SQUARE && y > valid_y-HALF_OF_SQUARE && x < valid_x+HALF_OF_SQUARE) {
                bool open_spot = true;
                for(int j = 0; j < 24; j++) {
                    if(piece_array[j].x == valid_x && piece_array[j].y == valid_y) {
                        open_spot = false;
                    }
                }
                
                if(open_spot) {
                    selected->x = valid_x;
                    selected->y = valid_y;
                    if(turn == 0) {
                        turn = 1;
                    } else {
                        turn = 0;
                    }
                    //selected_piece = valid_squares[i];
                }
            }
        }
        
    }
    
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
    
    myReshape(viewport.w,viewport.h);
    setup_coord_system();
    
    for(int i = 0; i < 32; i++) {
        coord_to_square[std::make_pair(x_coor[i], y_coor[i])] = i;
    }
    
    draw_board();
    setup_board(piece_array);
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {
    
    
    //----------------------- ----------------------- -----------------------
    // This is a quick hack to add a little bit of animation.
    static float tip = 0.5f;
    const  float stp = 0.0001f;
    const  float beg = 0.1f;
    const  float end = 0.9f;
    
    tip += stp;
    if (tip>end) tip = beg;
    //----------------------- ----------------------- -----------------------
    
    
    glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    //----------------------- code to draw objects --------------------------
    
    
    draw_board();
    
    
    // Make a circle at the mouse position, if the LEFT button is down
    if (mouseleftdown)
    {
        
        // Convert mouse position to OpenGL's coordinate system
        double last_x = curr_x;
        double last_y = curr_y;
        
        keep_highlight = true;
        if(mousex < winw/2){
            curr_x = mousex/(winw/2)-1;
        } else {
            curr_x = (mousex-winw/2)/(winw/2);
        }
        if(mousey < winh/2) {
            curr_y = 1-mousey/(winh/2);
        } else {
            curr_y = -(mousey-winh/2)/(winh/2);
        }
        
        int new_selected = checklocation(piece_array, turn, curr_x, curr_y);
        if(new_click && new_selected != -1) {
            if(selected_piece == new_selected && new_click){
                piece_array[new_selected].border = 0;
                selected_piece = -1;
            }
            else if(selected_piece != new_selected){
                piece_array[new_selected].border = 1;
                piece_array[selected_piece].border = 0;
                selected_piece = new_selected;
            }
        }
        if(selected_piece != -1 && new_click) {
            move_if_valid(&piece_array[selected_piece], curr_x, curr_y, piece_array);
        }
        new_click = false;
    } else {
        new_click = true;
    }
    
    draw_pieces(piece_array);
    
    
    
    //-----------------------------------------------------------------------
    
    glFlush();
    glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
    //nothing here for now
#ifdef _WIN32
    Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
    glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //This initializes glut
    glutInit(&argc, argv);
    
    //This tells glut to use a double-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Initalize theviewport size
    viewport.w = 400;
    viewport.h = 400;
    
    //The size and position of the window
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Checkers!");
    
    initScene();                                 // quick function to set up scene
    
    glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
    glutReshapeFunc(myReshape);                  // function to run when the window gets resized
    
    glutMouseFunc(mouse);
    
    glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
    glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else
    
    return 0;
}