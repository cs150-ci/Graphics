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
#include "texture.h"

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

void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glUniform1i(istex,0) ;
    //cout << texturing << endl;
    glUniform1i(istex, (int)texturing) ; 
    glBindTexture(GL_TEXTURE_2D, (int)(texturing-1)) ;
    //drawtexture(FLOOR,texNames[0]) ; // Texturing floor 
    //drawobject(CUBE) ; 
    //glUniform1i(istex,0) ; // Other items aren't textured 
        
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

          {
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
          }

          // Actually draw the object
          // We provide the actual glut drawing functions for you.  
          if (obj -> type == cube) {
            glutSolidCube(obj->size) ; 
          }
          else if (obj -> type == sphere) {
            const int tessel = 20 ; 
            glutSolidSphere(obj->size, tessel, tessel) ; 
          }
          else if (obj -> type == teapot) {
            glutSolidTeapot(obj->size) ; 
            //drawobject(FLOOR) ; // Texturing floor 
          }

        }
    
        glutSwapBuffers();
}

/*
void load_obj(const char* filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements) {
  ifstream in(filename, ios::in);
  if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
 
  string line;
  while (getline(in, line)) {
    printf("%c",line[0]);
    if (line.substr(0,2) == "v ") {
      istringstream s(line.substr(2));
      glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
      vertices.push_back(v);
    }  else if (line.substr(0,2) == "f ") {
      istringstream s(line.substr(2));
      GLushort a,b,c;
      s >> a; s >> b; s >> c;
      a--; b--; c--;
      elements.push_back(a); elements.push_back(b); elements.push_back(c);
    }
    else if (line[0] == '#') { }
    else { }
  }
 
  //normals.resize(mesh->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  for (int i = 0; i < elements.size(); i+=3) {
    GLushort ia = elements[i];
    GLushort ib = elements[i+1];
    GLushort ic = elements[i+2];
    glm::vec3 normal = glm::normalize(glm::cross(
      glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
      glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
    normals[ia] = normals[ib] = normals[ic] = normal;
  }
}
*/

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
        }
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
	switch(key) {
	case 100: //left
          if (transop == view) Transform::left(amount, eye,  up);
          else if (transop == scale) sx -= amount * 0.01 ; 
          else if (transop == translate) tx -= amount * 0.01 ; 
          break;
	case 101: //up
          if (transop == view) Transform::up(amount,  eye,  up);
          else if (transop == scale) sy += amount * 0.01 ; 
          else if (transop == translate) ty += amount * 0.01 ; 
          break;
	case 102: //right
          if (transop == view) Transform::left(-amount, eye,  up);
          else if (transop == scale) sx += amount * 0.01 ; 
          else if (transop == translate) tx += amount * 0.01 ; 
          break;
	case 103: //down
          if (transop == view) Transform::up(-amount,  eye,  up);
          else if (transop == scale) sy -= amount * 0.01 ; 
          else if (transop == translate) ty -= amount * 0.01 ; 
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

	glGenBuffers(numperobj*numobjects+ncolors+1, buffers) ; // 1 for texcoords 
	initcolorscube() ; 

	// Initialize texture
	// inittexture("wood.ppm", fragmentprogram) ; 
	inittexture("data/stone.ppm", shaderprogram, 1) ;
        inittexture("data/wood.ppm", shaderprogram, 0) ;
        // Define a sampler.  See page 709 in red book, 7th ed.
        GLint texsampler ; 
        texsampler = glGetUniformLocation(shaderprogram, "tex") ; 
        glUniform1i(texsampler, 0) ; // Could also be GL_TEXTURE0 
        istex = glGetUniformLocation(shaderprogram,"istex") ; 

	// Initialize objects
	initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof(floorcol), (GLubyte *) floorinds, sizeof(floorinds), GL_POLYGON) ; 
	//       initobject(CUBE, (GLfloat *) cubeverts, sizeof(cubeverts), (GLfloat *) cubecol, sizeof (cubecol), (GLubyte *) cubeinds, sizeof (cubeinds), GL_QUADS) ; 
	initobjectnocol(CUBE, (GLfloat *) cubeverts, sizeof(cubeverts), (GLubyte *) cubeinds, sizeof(cubeinds), GL_QUADS) ; 

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
