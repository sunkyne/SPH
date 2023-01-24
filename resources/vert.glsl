#version 120

uniform mat4 P;
uniform mat4 MV;
uniform vec2 screenSize;

attribute vec4 aPos;
attribute vec3 aCol;
attribute float aSca;

varying vec3 vCol;

void main()
{
    gl_Position = P * MV * aPos;
    vCol = aCol;
    
    // http://stackoverflow.com/questions/25780145/gl-pointsize-corresponding-to-world-space-size
    gl_PointSize = screenSize.y * P[1][1] * aSca / gl_Position.w;
}
