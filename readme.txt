Name: Cheng-yu Hong (cs184-eh)
      Linda Dai (cs184-aq)

Website: castle-scene.atwebpages.com

DETAILS: This project showcases a surrealistic castle scene created for the Foundations of Computer Graphics class at the University of California - Berkeley. The code is mostly done using OpenGL. Some interesting aspects of the project includes shader effects such as changing brightness and contrast, toggling of textures and wireframes, and insect/flame animations.

Scene Objects: Castle, drawbridge/moat, table, kiteshield, shield (rectangular), sword, fireplace, torches, ground floor, mace, fly

REQUIREMENTS:

Completeness: 

Object(s) created by hand: Table, kiteshield

Object(s) loaded from suitable geometric format: Castle, sword, and fly were loaded from .obj files

Placement and scaling by hand: A .txt file defines placement (translation/rotation) and scaling of objects, similar to hw2.

Textured objects: Ground floor is textured with a grass image, fireplace is textured with a stone image, drawbridge is textured with a wood image, as well as tapestries textured with images of existing tapestries. The castle and fly are textured with .mtl files included with the objects.

Shiny/dull object: A sword and nearby kiteshield appear shiny, while another sword appears dull.

Lighting: We used a directional light to simulate sunlight, and point lights to simulate light from the flames from the torches in the hallway and the fireplace. 

Objects instantiated more than once: Fly, sword, and torch were each instantiated twice.

Half of objects have correct normals: For hand-created objects, the normals were computed by taking the cross product of two adjacent edges for each face. The rest of the objects have normals pre-defined by using OpenGL geometric primitives, or normals loaded in as part of the .obj file.

Double buffering, hidden surface elimination, and perspective projection: Implemented for the scene by specifying parameters in openGL functions, as in hw2.

Turning on/off textures: Pressing 'x' key turns on/off all textures

Programmable shaders: Changing brightness, contrast, and saturation are implemented by modulating the final color with certain constants and/or parameters passed into the fragment shader.

Camera/mouse movement: In addition to the movement functions in hw2, we added first-person camera rotation in the xy and yz direction, moving forward and backwards, and translation in the z direction. The keyboard is used to rotate in the xy direction and moving forward and backwards, while the mouse is used to rotate in the yz direction and translate in the z direction. Zoom is implemented by changing fovy parameter as key presses 'i' and 'o'. 

Animation: The flames in the fireplace and torch are constantly animated. The flies are animated as well, and can be started/stopped with key presses. 

Wireframe mode: Pressing 'w' toggles between a wireframe and smooth shading representation of the scene.


