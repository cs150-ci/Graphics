/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Transform.h"

using namespace std ; 
#include "variables.h"
#include "readfile.h"

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
          }

        }
    
        glutSwapBuffers();
}
