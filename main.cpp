/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include "UCB/grader.h"

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
void display(void) ;  // prototype for display function.  

Grader grader;
bool allowGrader = false;

// Uses the Projection matrices (technically deprecated) to set perspective 
// We could also do this in a more modern fashion with glm.  
void reshape(int width, int height){
	w = width;
	h = height;
        mat4 mv ; // just like for lookat

	glMatrixMode(GL_PROJECTION);
        float aspect = w / (float) h, zNear = 0.1, zFar = 99.0 ;
        // I am changing the projection stuff to be consistent with lookat
        if (useGlu) mv = glm::perspective(fovy,aspect,zNear,zFar) ; 
        else {
          mv = Transform::perspective(fovy,aspect,zNear,zFar) ; 
          mv = glm::transpose(mv) ; // accounting for row major 
        }
        glLoadMatrixf(&mv[0][0]) ; 

	glViewport(0, 0, w, h);
}

void saveScreenshot(string fname) {
	int pix = w * h;
	BYTE pixels[3*pix];	
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	
	std::cout << "Saving screenshot: " << fname << "\n";

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}


void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n" 
       << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n" 
            << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"       
            << "press 'r' to reset the transformations.\n"
            << "press 'v' 't' to do view [default] and translate.\n"
            << "press 'i' 'o' to zoom in or out.\n"
            << "press up down arrow keys to move forward or backward.\n"
            << "press left right arrow keys to rotate left or right.\n"
            << "move mouse vertically (up/down) to rotate up or down.\n"
            << "move mouse horizontally (left/right) to rotate left or right.\n"
            << "press ESC to quit.\n" ;      
}


void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case '+':
		amount++;
		std::cout << "amount set to " << amount << "\n" ;
		break;
	case '-':
		amount--;
		std::cout << "amount set to " << amount << "\n" ; 
		break;
	case 'i':
        // Zoom: change fovy, and change perspective by calling reshape()
        // Instead of grader, zooms in
        fovy -= 1 ;
        reshape(w,h) ;
        std::cout << "fovy set to " << fovy << "\n" ;
		break;
    case 'o':
        // Zooms out
        fovy += 1 ;
        reshape(w,h) ;
        std::cout << "fovy set to " << fovy << "\n" ;
        break;
	case 'g':
		useGlu = !useGlu;
                reshape(w,h) ; 
		std::cout << "Using glm::LookAt and glm::Perspective set to: " << (useGlu ? " true " : " false ") << "\n" ; 
		break;
	case 'h':
		printHelp();
		break;
        case 27:  // Escape to quit
                exit(0) ;
                break ;
        case 'r': // reset eye and up vectors, scale and translate, and fovy. 
		eye = eyeinit ; 
		up = upinit ; 
                sx = sy = 1.0 ; 
                tx = ty = 0.0 ; 
                fovy = fovyinit ;
                reshape(w,h) ;
		break ;   
        case 'v': 
                transop = view ;
                std::cout << "Operation is set to View\n" ; 
                break ; 
        case 't':
                transop = translate ; 
                std::cout << "Operation is set to Translate\n" ; 
                break ; 
        case 's':
                transop = scale ; 
                std::cout << "Operation is set to Scale\n" ; 
                break ; 
        }
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions
//  HW4 modification: viewing changed, translation kept

void specialKey(int key, int x, int y) {
	switch(key) {
	case 100: //left
          /*if (transop == view) Transform::left(amount, eye,  up);
          else if (transop == scale) sx -= amount * 0.01 ; 
          else if (transop == translate) tx -= amount * 0.01 ; 
          */
          // Moving left = rotating left
          if (transop == view) {
            float radians = amount * pi/180 ;
            eye[0] = eye[0]*cos(radians) - eye[1]*sin(radians) ;
            eye[1] = eye[1]*cos(radians) + eye[0]*sin(radians) ;
          } else if (transop == translate) tx -= amount * 0.01 ;
          break;
	case 101: //up
          /*if (transop == view) Transform::up(amount,  eye,  up);
          else if (transop == scale) sy += amount * 0.01 ; 
          else if (transop == translate) ty += amount * 0.01 ; 
          */
          // Moving up = moving forward
          if (transop == view) {
            eye[1] += amount * 0.01 ;
          } else if (transop == translate) ty += amount * 0.01 ;
          break;
	case 102: //right
          /*if (transop == view) Transform::left(-amount, eye,  up);
          else if (transop == scale) sx += amount * 0.01 ; 
          else if (transop == translate) tx += amount * 0.01 ; 
          */
          // Moving right = rotating right
          if (transop == view) {
            float radians = amount * pi/180 ;
            eye[0] = eye[0]*cos(-radians) - eye[1]*sin(-radians) ;
            eye[1] = eye[1]*cos(-radians) + eye[0]*sin(-radians) ;
          } else if (transop == translate) tx += amount * 0.01 ;
          break;
	case 103: //down
          /*if (transop == view) Transform::up(-amount,  eye,  up);
          else if (transop == scale) sy -= amount * 0.01 ; 
          else if (transop == translate) ty -= amount * 0.01 ; 
          */
          // Moving down = moving backward
          if (transop == view) {
            eye[1] -= amount * 0.01 ;
          } else if (transop == translate) ty -= amount * 0.01 ;
          break;
	}
	glutPostRedisplay();
}

void drag(int x, int y) {
    // Change in points
    int dfx = x-oldx ;
    int dfy = (h-y)-(h-oldy) ;

    // DEBUG
    //cout << "dfx: " << dfx << ", dfy: " << dfy << "\n" ;

    // Determine which way/plane to move eye
    // Mouse drag moves faster than arrow movements, so scale amount by fourth 
    float radians = amount/4 * pi/180 ;
    if (dfx >= -5 && dfx <= 5 && dfy > 0) {
        // Move up along yz plane
        eye[1] = eye[1]*cos(radians) - eye[2]*sin(radians) ;
        eye[2] = eye[2]*cos(radians) + eye[1]*sin(radians) ;
    } else if (dfx >= -5 && dfx <= 5 && dfy <= 0) {
        // Move down along yz plane
        eye[1] = eye[1]*cos(-radians) - eye[2]*sin(-radians) ;
        eye[2] = eye[2]*cos(-radians) + eye[1]*sin(-radians) ;
    } else if (dfx > 0 && dfy >= -10 && dfy <= 10) {
        // Move right along xy plane
        eye[0] = eye[0]*cos(-radians) - eye[1]*sin(-radians) ;
        eye[1] = eye[1]*cos(-radians) + eye[0]*sin(-radians) ;
    } else if (dfx < 0 && dfy >= -10 && dfy <= 10) {
        // Move left along xy plane
        eye[0] = eye[0]*cos(radians) - eye[1]*sin(radians) ;
        eye[1] = eye[1]*cos(radians) + eye[0]*sin(radians) ;
    }

    // Save oldx and oldy and redraw
    oldx = x ; 
    oldy = y ;

    glutPostRedisplay();

}

void init() {
      // Initialize shaders
      vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
      fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
      shaderprogram = initprogram(vertexshader, fragmentshader) ; 
      enablelighting = glGetUniformLocation(shaderprogram,"enablelighting") ;
      lightpos = glGetUniformLocation(shaderprogram,"lightposn") ;       
      lightcol = glGetUniformLocation(shaderprogram,"lightcolor") ;       
      numusedcol = glGetUniformLocation(shaderprogram,"numused") ;       
      ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;       
      diffusecol = glGetUniformLocation(shaderprogram,"diffuse") ;       
      specularcol = glGetUniformLocation(shaderprogram,"specular") ;       
      emissioncol = glGetUniformLocation(shaderprogram,"emission") ;       
      shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;       

}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
		exit(-1); 
	}

  	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW4: Scene");
	init();
        readfile(argv[1]) ; 
	glutDisplayFunc(display);
    glutMotionFunc(drag);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(w, h);

	if (argc > 2) {
		allowGrader = true;
		stringstream tcid;
		tcid << argv[1] << "." << argv[2];
		grader.init(tcid.str());
		grader.loadCommands(argv[2]);
		grader.bindDisplayFunc(display);
		grader.bindSpecialFunc(specialKey);
		grader.bindKeyboardFunc(keyboard);
		grader.bindScreenshotFunc(saveScreenshot);
	}

	printHelp();
	glutMainLoop();
	FreeImage_DeInitialise();
	return 0;
}
