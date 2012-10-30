/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye
EXTERN vec3 up; // The (regularly updated) vector coordinates of the up
EXTERN vec3 center; // center also regularly updated
EXTERN float fovy; // fovy variable

#ifdef MAINPROGRAM
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 centerinit(0.0,0.0,0.0) ; // Center look at point
int w = 600, h = 400 ; // width and height
float fovyinit = 90.0 ; // For field of view, initial
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 centerinit ;
EXTERN int w, h ;
EXTERN float fovyinit ;
#endif 

// Mouse movement
EXTERN int oldx ;
EXTERN int oldy ;

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
static enum {view, translate, scale, oldview} transop ; // which operation to transform 
enum shape {cube, sphere, cone, teapot, castle, sword, shield, tapestry, fly, table} ;
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ; 
EXTERN GLfloat lightposn [4*numLights] ; // Light Positions
EXTERN GLfloat lightcolor[4*numLights] ; // Light Colors
EXTERN GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
EXTERN int numused ;                     // How many lights are used 

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN GLfloat ambient[4] ; 
EXTERN GLfloat diffuse[4] ; 
EXTERN GLfloat specular[4] ; 
EXTERN GLfloat emission[4] ; 
EXTERN GLfloat shininess ; 

// For multiple objects, read from a file.  
const int maxobjects = 100 ; 
EXTERN int numobjects ; 
EXTERN struct object {
  shape type ; 
  GLfloat size ;
  GLfloat ambient[4] ; 
  GLfloat diffuse[4] ; 
  GLfloat specular[4] ;
  GLfloat emission[4] ; 
  GLfloat shininess ;
  GLfloat texturing ;
  mat4 transform ; 
} objects[maxobjects] ;

// Variables to set uniform params for lighting fragment shader 
EXTERN GLuint lightcol ; 
EXTERN GLuint lightpos ; 
EXTERN GLuint numusedcol ; 
EXTERN GLuint enablelighting ; 
EXTERN GLuint ambientcol ; 
EXTERN GLuint diffusecol ; 
EXTERN GLuint specularcol ; 
EXTERN GLuint emissioncol ; 
EXTERN GLuint shininesscol ; 

// Variables to use for texture mapping
// EXTERN GLubyte texture[256][256][3] ;
EXTERN GLuint texNames[20] ; // texture buffer
EXTERN GLuint istex ;  // texture variable passed to shaders
EXTERN GLfloat texturing ; // global texture value
EXTERN GLuint toggleTexture ;
EXTERN bool wired ; // toggle wireframe mode

// Animation
EXTERN bool isAnimate ; // Animation flag

// Changing Brightness
EXTERN GLuint imgmode ; // toggle brightness/contrast/rotation
EXTERN GLfloat alphab ; // alpha value passed to shaders
EXTERN GLuint alphabptr ; 
EXTERN GLfloat alphac ; // alpha value passed to shaders
EXTERN GLuint alphacptr ; 

