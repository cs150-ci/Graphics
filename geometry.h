void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) ;
void initobjectnocol(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type) ;
void drawobject(GLuint object) ;
void initcolorscube (void) ;
void drawcolor(GLuint object, GLuint color) ;
void inittexture (const char * filename, GLuint program) ;
void drawtexture(GLuint object, GLuint texture) ;

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 

void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) {
   int offset = object * numperobj ;
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices+offset]) ;
   glBufferData(GL_ARRAY_BUFFER, sizevert, vert,GL_STATIC_DRAW);
   glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ;
   glEnableClientState(GL_VERTEX_ARRAY) ;
   glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors+offset]) ;
   glBufferData(GL_ARRAY_BUFFER, sizecol, col,GL_STATIC_DRAW);
   glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)) ;
   glEnableClientState(GL_COLOR_ARRAY) ;
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements+offset]) ;
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds,GL_STATIC_DRAW);
   PrimType[object] = type ;
   NumElems[object] = sizeind ;
}

// Very basic code to read a ppm file
// And then set up buffers for texture coordinates
void inittexture (const char * filename, GLuint program) {
  int i,j,k ;
  FILE * fp ;
  GLint err ;
   assert(fp = fopen(filename,"rb")) ;
   fscanf(fp,"%*s %*d %*d %*d%*c") ;
   for (i = 0 ; i < 256 ; i++)
     for (j = 0 ; j < 256 ; j++)
       for (k = 0 ; k < 3 ; k++)
     fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
   fclose(fp) ;

   // Set up Texture Coordinates
   glGenTextures(1, texNames) ;

