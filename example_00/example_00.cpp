			// CS184 Simple OpenGL Example

		#include <iostream>
		#include <fstream>
		#include <cmath>
		#include <list>
		
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
	    double width, height;
	    double x, y;
	    int black_or_not;
	    void change_location(double, double);
	    void draw();
	    void set_color(int);
	};

	Piece::Piece() {
	  width = 0.2;
	  height = 0.2;
	  x = 0.0;
	  y = 0.0;
	  black_or_not = 1;
	}

	Piece::Piece(double new_x, double new_y, int black) {
	  width = 0.2;
	  height = 0.2;
	  x = new_x;
	  y = new_y;
	  black_or_not = black;
	}

	void Piece::change_location (double new_x, double new_y) {
	   x = new_x;
	   y = new_y;
	}

	void Piece::set_color (int black) {
	   black_or_not = black;
	}

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

	void Piece::draw() {
	   if(black_or_not) {
	       DrawCircle(x,y,width/2, 50, 0.7, 0.7, 0.7);
	   } else {
		DrawCircle(x,y,width/2,50, 1, 1, 1);
	   }
	}

		//****************************************************
		// Global Variables
		//****************************************************
		Viewport    viewport;

		//****************************************************
		// reshape viewport if the window is resized
		//****************************************************
		void myReshape(int w, int h) {
		  viewport.w = w;
		  viewport.h = h;

		  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
		  glMatrixMode(GL_PROJECTION);
		  glLoadIdentity();                // loading the identity matrix for the screen

		  //----------- setting the projection -------------------------
		  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


		  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
		  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

		  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

		  //------------------------------------------------------------
		}


		//****************************************************
		// sets the window up
		//****************************************************
		void initScene(){
		  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

		  myReshape(viewport.w,viewport.h);
		}



double x_coor[32];
double y_coor[32];

void setup_coor() {
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
		piece_array[i].change_location(x_coor[loc], y_coor[loc]);
		piece_array[i].set_color(black);
	  }
	}

	void draw_pieces(Piece piece_array[]) {
	   for(int i = 0; i < 24; i++) {
	       piece_array[i].draw();
	   }
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

		Piece piece_array[24];
		setup_coor();

		draw_board();
		setup_board(piece_array);
		draw_pieces(piece_array);	


	

/**********************************************/
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
  glutCreateWindow("CS184!");

  initScene();                                 // quick function to set up scene

  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








