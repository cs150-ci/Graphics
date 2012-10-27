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

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp   
#include "Texture.h"
#include "glm.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  

void transformvec (const GLfloat input[4], GLfloat output[4]) {
  GLfloat modelview[16] ; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ; 
  
  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0 ; 
    for (int j = 0 ; j < 4 ; j++) 
      output[i] += modelview[4*j+i] * input[j] ; 
  }
}

GLMmodel* pmodel1 = NULL;
void drawmodel(char* filename, GLuint texid)
{
  // Load the model only if it hasn't been loaded before
  // If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
  if (!pmodel1)
  {
    // this is the call that actualy reads the OBJ and creates the model object
    pmodel1 = glmReadOBJ(filename);
    if (!pmodel1) exit(0);
    // This will rescale the object to fit into the unity matrix
    // Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
    glmUnitize(pmodel1);
    // These 2 functions calculate triangle and vertex normals from the geometry data.
    // To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
    // So if you can export these directly from you modeling tool do it and comment these line
    // 3DS Max can calculate these for you and GLM is perfectly capable of loading them
    glmFacetNormals(pmodel1);
    glmVertexNormals(pmodel1, 90.0);
  }
  // This is the call that will actually draw the model
  // Don't forget to tell it if you want textures or not :))
  glmDraw(pmodel1, GLM_SMOOTH | GLM_TEXTURE);
}

void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    // I'm including the basic matrix setup for model view to 
    // give some sense of how this works.  

	glMatrixMode(GL_MODELVIEW);
	mat4 mv ; 

    if (useGlu) mv = glm::lookAt(eye,center,up) ; 
	else {
          mv = Transform::lookAt(eye,center,up) ; 
          mv = glm::transpose(mv) ; // accounting for row major
        }
        glLoadMatrixf(&mv[0][0]) ; 

        // Set Light and Material properties for the teapot
        // Lights are transformed by current modelview matrix. 
        // The shader can't do this globally. 
        // So we need to do so manually.  
        if (numused) {
          glUniform1i(enablelighting,true) ;

          // YOUR CODE FOR HW 2 HERE.  
          // You need to pass the lights to the shader. 
          // Remember that lights are transformed by modelview first.  

          // Transform lights by modelview first
          GLfloat lightinput[4], lightoutput[4];
          for (int i = 0; i < numused; i++) {
              lightinput[0] = lightposn[i*4];
              lightinput[1] = lightposn[i*4+1];
              lightinput[2] = lightposn[i*4+2];
              lightinput[3] = lightposn[i*4+3];
              transformvec(lightinput, lightoutput);
              lightransf[i*4] = lightoutput[0];
              lightransf[i*4+1] = lightoutput[1];
              lightransf[i*4+2] = lightoutput[2];
              lightransf[i*4+3] = lightoutput[3];
          }
              
          // Pass to shaders, as in mytest3.cpp display code
          glUniform1i(numusedcol, numused);

          glUniform4fv(lightpos,10,lightransf);
          glUniform4fv(lightcol,10,lightcolor);

          glUniform4fv(ambientcol,1,ambient);
          glUniform4fv(diffusecol,1,diffuse);
          glUniform4fv(specularcol,1,specular);
          glUniform4fv(emissioncol,1,emission);
          glUniform1f(shininesscol,shininess);
          
        }
        else glUniform1i(enablelighting,false) ; 

        // Transformations for objects, involving translation and scaling 
        mat4 sc(1.0) , tr(1.0), transf(1.0) ; 
        sc = Transform::scale(sx,sy,1.0) ; 
        tr = Transform::translate(tx,ty,0.0) ; 

        // YOUR CODE FOR HW 2 HERE.  
        // You need to use scale, translate and modelview to 
        // set up the net transformation matrix for the objects.  
        // Account for GLM issues etc.  
        transf = mv*glm::transpose(tr)*glm::transpose(sc);
        glLoadMatrixf(&transf[0][0]) ; 

        for (int i = 0 ; i < numobjects ; i++) {
          object * obj = &(objects[i]) ; 

          // YOUR CODE FOR HW 2 HERE. 
          // Set up the object transformations 
          // And pass in the appropriate material properties
          mat4 objtrans = transf * (obj->transform);
          glLoadMatrixf(&objtrans[0][0]);
          glUniform1i(numusedcol, numused);
          glUniform4fv(lightpos,10,lightransf);
          glUniform4fv(lightcol,10,lightcolor);
          glUniform4fv(ambientcol,1,obj->ambient);
          glUniform4fv(diffusecol,1,obj->diffuse);
          glUniform4fv(specularcol,1,obj->specular);
          glUniform4fv(emissioncol,1,obj->emission);
          glUniform1f(shininesscol,obj->shininess);

          //Setup textures
          glUniform1i(istex, (int)obj->texturing) ;
          glBindTexture(GL_TEXTURE_2D, texNames[(int)(obj->texturing-1)]) ;

          // Actually draw the object
          // We provide the actual glut drawing functions for you.  
          if (obj -> type == cube) {
            if (wired) glutSolidCube(obj->size) ; 
            else glutSolidCube(obj->size) ; 
          }
          else if (obj -> type == sphere) {
            const int tessel = 20 ; 
            if (wired) glutSolidSphere(obj->size, tessel, tessel) ; 
            else glutSolidSphere(obj->size, tessel, tessel) ; 
          }
          else if (obj -> type == teapot) {
            if (wired) glutWireTeapot(obj->size) ;
            else glutSolidTeapot(obj->size) ; 
          }
          else if (obj -> type == castle) {
            drawmodel("data/saint-riquier_obj/saintriqT3DS.obj", (GLuint)obj->size) ;
          }
          else if (obj -> type == sword) {
            drawmodel("data/sword.obj", (GLuint)obj->size) ;
          }
          else if (obj -> type == tapestry) {
            glBegin(GL_QUADS) ;
            glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1*obj->size, 0.0) ;
            glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, obj->size, 0.0) ;
            glTexCoord2f(1.0, 1.0); glVertex3f(1.0, obj->size, 0.0) ;
            glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1*obj->size, 0.0) ;
            glEnd() ;
          }
        }
    
        glutSwapBuffers();
}

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
            << "press 'v' 't' 's' to do view [default], translate, scale.\n"
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
        case 'c': // Screenshot
		saveScreenshot("screenshot.png");
		break;
        case 'r': // reset eye and up vectors, scale and translate. 
		eye = eyeinit ; 
		up = upinit ; 
                sx = sy = 1.0 ; 
                tx = ty = 0.0 ; 
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
        case 'q':
                transop = oldview ;
                std::cout << "Operation is set to OldView (crystal ball interface)\n" ;
                break;
        case 'w': // Toggle wireframe
                wired = !wired ;
                break ;
        }
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
	switch(key) {
        case 100: //left
          // Moving left = rotating left
          if (transop == view) {
            float radians = amount/2 * pi/180 ;
            // Translate eye to origin (same), center by same amount
            center = center - eye;
            // Rotate center by amount
            center[0] = center[0]*cos(radians) - center[1]*sin(radians) ;
            center[1] = center[1]*cos(radians) + center[0]*sin(radians) ;
            // Translate eye back (same), center by same amount
            center = center + eye;

          } else if (transop == scale) sx -= amount * 0.01 ;
          else if (transop == translate) tx -= amount * 0.01 ;
          else if (transop == oldview) Transform::left(amount, eye, up);
          break;
        case 101: //up
          // Moving up = moving forward
          if (transop == view) {
            // Get change in x/y directions, and normalize by some constant
            float dx = (center[0] - eye[0])/100 ;
            float dy = (center[1] - eye[1])/100 ;
            // Add change to corresponding x/y components to move
            eye[0] += dx ;
            eye[1] += dy ;
            center[0] += dx ;
            center[1] += dy ;
          } else if (transop == scale) sy += amount * 0.01 ;
          else if (transop == translate) ty += amount * 0.01 ;
          else if (transop == oldview) Transform::up(amount, eye, up);
          break;
        case 102: //right
          // Moving right = rotating right
          if (transop == view) {
            float radians = amount/2 * pi/180 ;
            // Translate eye to origin (same), center by same amount
            center = center - eye;
            // Rotate center by amount
            center[0] = center[0]*cos(-radians) - center[1]*sin(-radians) ;
            center[1] = center[1]*cos(-radians) + center[0]*sin(-radians) ;
            // Translate eye back (same), center by same amount
            center = center + eye;
          } else if (transop == scale) sx += amount * 0.01 ;
          else if (transop == translate) tx += amount * 0.01 ;
          else if (transop == oldview) Transform::left(-amount, eye, up);
          break;
        case 103: //down
          // Moving down = moving backward
          if (transop == view) {
            // Get change in x/y directions, and normalize by some constant
            float dx = (center[0] - eye[0])/100 ;
            float dy = (center[1] - eye[1])/100 ;
            // Subtract change to corresponding x/y components to move
            eye[0] -= dx ;
            eye[1] -= dy ;
            center[0] -= dx ;
            center[1] -= dy ;
          } else if (transop == scale) sy -= amount * 0.01 ;
          else if (transop == translate) ty -= amount * 0.01 ;
          else if (transop == oldview) Transform::up(-amount, eye, up);
          break;
        }
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

      // Initialize textures
      LoadTexture("data/wood.tga", 0) ;
      LoadTexture("data/stone.tga", 1) ;
      LoadTexture("data/sky.tga", 2) ;
      LoadTexture("data/grass.tga", 3) ;
      LoadTexture("data/tree_of_life.tga", 4) ;
      LoadTexture("data/ArrasWawel.tga", 5) ;
      istex = glGetUniformLocation(shaderprogram,"istex") ; 

      // Enable the depth test
      glEnable(GL_DEPTH_TEST) ;
      glDepthFunc (GL_LESS) ; // The default option
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << "Usage: transforms scenefile\n"; 
		exit(-1); 
	}

  	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW4: Scene");
	init();
        readfile(argv[1]) ; 
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(w, h);

	printHelp();
	glutMainLoop();
	FreeImage_DeInitialise();
	return 0;
}

