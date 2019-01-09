#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include "Hash.h"
#include <cmath>

using namespace std;

void display ( void );
//void reshape( int w, int h );

void initOpenGL( const char *filename, int nrows, int ncols, Hash<string, string> myhash, string Title);
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] );

Hash<string, string> myhash;

int main( int argc, char *argv[] )
{
	string Title;
    string val;

    //Hash<int, int> myhash;
	
    string k;
    string v;
    int counter = 0;
    ifstream fin;
    int max = 0;
    int temp = 0;
    fin.open(argv[1]);
    if(!fin)
    {
        cout << "your a terrible person" <<endl;
        return 0;
    }
    while (fin>>k)
    {
        if(!(fin >> v))
        break;
        //cout << v << endl;
        if(k.size() !=0)
        myhash.insert(k, v);
        /*counter = 
        temp = myhash.timesprobed(counter);
        cout << "TEMP IS " << temp <<endl;
        if(temp > max)
        {
        max = temp;
        index = counter;
        }
        counter ++;*/
        //val = myhash.find(k);
        //cout << val <<endl;
        
    }
    //cout << max << " ";
    //cout << index << endl;
    //cout << myhash.getkey(index) << endl;
    counter = myhash.findMax();
    max = myhash.timesprobed(counter);
    
    Title = "Load Factor: ";
	Title+= to_string(myhash.loadFactor(false));
	Title+= " %: MaxFreq = ";
	temp = max;
	Title+= to_string(temp);
	Title+= " : Most Freq Key = ";
	Title+= myhash.getkey(counter);
    
    fin.close();
    //cout << val <<endl;
    //string lent = "Fish";
	//string pigs = "Sticks";
	//myhash.insert(lent, pigs );
    //val = myhash.find(lent);
    //cout << val << endl;
  	 glutInit( &argc, argv );
  	 initOpenGL( argv[1], 500, 1000, myhash, Title);

  // go into OpenGL/GLUT main loop, never to return
  	glutMainLoop();

  // Keep the compiler from complaining
  return 0;
}

void initOpenGL( const char *windowTitle, int nrows, int ncols, Hash<string,string> myhash, string Title)
{
	const char *c;

	//Hash myhash;
  //int imgCols = 0, imgRows = 0;
  // 32-bit graphics and single buffering
  //glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	c = Title.c_str();
  glutInitWindowSize( ncols, nrows );  // initial window size
  glutInitWindowPosition( 100, 50 );   // initial window  position
  glutCreateWindow(c);// << myhash.loadFactor(false) )//<< 
  //" %: MaxFreq = "<< myhash.findMax().timesprobed << 
  //" : Most Freq Key = "<< myhash.findMax().value);   // window title
  //utilityCentral(Event(nrows, ncols));
  glClearColor( 0.0, 0.0, 0.0, 0.0 );         // use black for glClear command

  // callback routines
  glutDisplayFunc( display );   // how to redisplay window
  //glutReshapeFunc( reshape );   // how to resize window
  //glutKeyboardFunc( keyboard ); // how to handle key presses
  //glutMouseFunc(mouseClick);          // how to handle mouse clicks
  //glutMotionFunc(mouseMotion);        // how to handle mouse movements
}

void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_QUADS );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}

/*void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] )
{
    glColor3fv( color );
    glBegin( GL_POLYGON );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
    glFlush();
}*/

void display( void )
{
    float x, y;
    int j = 0;
    double i = 0;
    int k = 0;

    double h = glutGet(GLUT_WINDOW_HEIGHT);
    double w = glutGet(GLUT_WINDOW_WIDTH);
  glClear( GL_COLOR_BUFFER_BIT);
  const float White[] = {0.0, 0.0, 4.0};
  const float Yellow[] = {0.0, 1.0, 1.0};
  
  //utilityCentral(Event());
  myhash.rectSize(x, y);
  x = x/ w;
  y = y/ h;
  for( k = 0; k < myhash.size(); k++ )
  {  
  	 j = ((floor(k/50))/h);
  	 i = k % 50;
  	 //cout << " J is " << j <<endl;
  	 if(myhash.timesprobed(i) > 5)
 	 DrawRectangle(x - 1 + (i/500.0), -(y) + 1 - (j/500.0), x - 1 +(i/500.0) + (50.0/w), -(y) + 1 - (50.0/h) - (j/500.0) , Yellow );
 	 else
 	 DrawRectangle(x - 1 + (i/200.0), -(y) + 1 - (j/100.0), x - 1 +(i/350.0) + (50.0/w), -(y) + 1 - (50.0/h) - (j/100.0), White );
  }
  //insertChar(lastGuessedChar);
  glFlush();
  glutSwapBuffers();
}
/*
void reshape( int w, int h )
{
  // how to project 3-D scene onto 2-D
  glMatrixMode( GL_PROJECTION );    // use an orthographic projection
  glLoadIdentity();         // initialize transformation matrix
  gluOrtho2D( 0.0, w, 0.0, h );
  glViewport( 0, 0, w, h );       // adjust viewport to new window
  glutPostRedisplay();
}*/
