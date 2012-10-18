# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color ;  
// in vec4 mynormal ; 
// in vec4 myvertex ;
// That is certainly more modern

varying vec4 color ;
varying vec3 mynormal ; 
varying vec4 myvertex ; 

uniform sampler2D tex ; 
uniform int istex ; 

const int numLights = 10 ; 
uniform bool enablelighting ; // are we lighting at all (global).
uniform vec4 lightposn[numLights] ; // positions of lights 
uniform vec4 lightcolor[numLights] ; // colors of lights
uniform int numused ;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient ; 
uniform vec4 diffuse ; 
uniform vec4 specular ; 
uniform vec4 emission ; 
uniform float shininess ; 

void main (void) 
{
    if (istex > 0) gl_FragColor = texture2D(tex, gl_TexCoord[0].st) ;
    else if (enablelighting) {       
        
        vec4 finalcolor ; 

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        // Setup eye position and normal needed for shading, as from hw1 (main transform)
        const vec3 eyepos = vec3(0,0,0);
        vec4 _mypos = gl_ModelViewMatrix * myvertex;
        vec3 mypos = _mypos.xyz / _mypos.w; // Dehomogenize current location
        vec3 eyedirn = normalize(eyepos - mypos);

        vec3 _normal = (gl_ModelViewMatrixInverseTranspose * vec4(mynormal,0.0)).xyz;
        vec3 normal = normalize(_normal);

        // Shading equation: I = A + E + sum_over_i(L_i[D*max(N*L,0) + S*max(N*H,0)^s]
        // - Initialize finalcolor (I) with A + E
        finalcolor = ambient + emission;

        // - Initialize variables used in looping
        vec4 lightvector, lightvcolor;
        vec3 position, direction, halfn;
        float nDotL, nDotH;
        vec4 lambert, phong, retval;

        // - Loop over all 10 lights, compute light, and add color to final output
        for (int i = 0; i < numused; i++) {
            lightvector = lightposn[i];
            lightvcolor = lightcolor[i];
            if (lightvector.w != 0.0) {
                position = lightvector.xyz / lightvector.w;
                direction = normalize(position - mypos);
            } else {
                direction = normalize(lightvector.xyz);
            }
            halfn = normalize(direction + eyedirn);
            nDotL = dot(normal, direction);
            lambert = diffuse * lightvcolor * max(nDotL, 0.0);
            nDotH = dot(normal, halfn);
            phong = specular * lightvcolor * pow(max(nDotH, 0.0), shininess);
            retval = lambert + phong;
            finalcolor += retval;
        }

	    //Color all pixels blue for now, remove this in your implementation
        //finalcolor = vec4(0,0,1,1); 
        
        gl_FragColor = finalcolor ; 
        }
    else gl_FragColor = color ; 
}
