// Transform.cpp: implementation of the Transform class.


#include "Transform.h"


// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.  

  // Setup parameters and matrices
  float radians = degrees * pi/180;
  mat3 rotation_matrix;
  mat3 identity_matrix = mat3(
     1., 0., 0.,
     0., 1., 0.,
     0., 0., 1.
     );
  mat3 aa_transpose = mat3(
     axis[0]*axis[0], axis[0]*axis[1], axis[0]*axis[2],
     axis[0]*axis[1], axis[1]*axis[1], axis[1]*axis[2],
     axis[0]*axis[2], axis[2]*axis[1], axis[2]*axis[2]
  );
  mat3 dual_matrix = mat3(
     0., -1.*axis[2], axis[1],
     axis[2], 0., -1.*axis[0],
     -1.*axis[1], axis[0], 0.
  );

  // Compute and return axis-angle rotation matrix
  rotation_matrix = cos(radians)*identity_matrix + (1.-cos(radians))*aa_transpose + sin(radians)*dual_matrix;
  return rotation_matrix;
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  

  // Construct left rotation matrix about 'up' axis, and rotate 'eye'
  mat3 left_rotate_matrix = Transform::rotate(degrees, up);
  eye = eye * left_rotate_matrix;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1. 

  // Find up rotation axis by normalizing the cross product of
  // 'eye' and 'up' vectors
  vec3 up_rotate_axis = glm::normalize(glm::cross(eye, up));

  // Construct up rotation matrix about new axis, and rotate 'eye'
  mat3 up_rotate_matrix = Transform::rotate(degrees, up_rotate_axis);
  eye = eye * up_rotate_matrix;

  // Find new 'up' vector by normalizing the cross product of 
  // new 'eye' and up rotation axis vectors
  up = glm::normalize(glm::cross(up_rotate_axis, eye)); 
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  

  // 3 vectors: 'eye', 'center' (origin), 'up'
  // Step 1: create coordinate frame for the camera (from lecture notes)
  vec3 w = glm::normalize(eye-center);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w, u);

  // Step 2: define rotation matrix from u, v, and w
  // - see first 3x3 submatrix in final product

  // Step 3: apply appropriate translation for camera (eye) location
  // - translation first (bring camera to origin), then rotation * translation
  // - translation matrix: 3x3 Identity + negative 'eye', 0 0 0 1
  // - negative 'eye' = subtract 'eye' from 0 to bring to origin
  // - 4D homogeneous coordinates for translation + rotation --> w = 1
  // - final product P' = (RT)P
  mat4 final_product = mat4(
     u[0], u[1], u[2], -1*u[0]*eye[0]-u[1]*eye[1]-u[2]*eye[2],
     v[0], v[1], v[2], -1*v[0]*eye[0]-v[1]*eye[1]-v[2]*eye[2],
     w[0], w[1], w[2], -1*w[0]*eye[0]-w[1]*eye[1]-w[2]*eye[2],
     0, 0, 0, 1
  );
  return final_product;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  

  // Setup parameters for perspective matrix
  float theta = fovy/2.0;
  float radians = theta * pi/180;
  float d = cos(radians)/sin(radians); 
  float A = -1*(zFar+zNear)/(zFar-zNear);
  float B = -1*(2.0*zFar*zNear)/(zFar-zNear);

  // Construct perspective matrix (final result)
  mat4 P = mat4(
     d/aspect, 0, 0, 0,
     0, d, 0, 0,
     0, 0, A, B,
     0, 0, -1, 0
  );
  return P;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 

  // Construct 3D scaling matrix
  // - last row/column as homogeneous coordinates
  mat4 S = mat4(
     sx, 0, 0, 0,
     0, sy, 0, 0,
     0, 0, sz, 0,
     0, 0, 0, 1
  );
  return S;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
  // YOUR CODE FOR HW2 HERE
  // Implement translation 

  // Construct 3D translation matrix
  mat4 T = mat4(
     1, 0, 0, tx,
     0, 1, 0, ty,
     0, 0, 1, tz,
     0, 0, 0, 1
  );
  return T;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
  vec3 x = glm::cross(up,zvec) ; 
  vec3 y = glm::cross(zvec,x) ; 
  vec3 ret = glm::normalize(y) ; 
  return ret ; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
