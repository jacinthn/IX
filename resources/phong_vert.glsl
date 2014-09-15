#version 120

uniform vec4 lightPos;
varying vec3 N, V, L, color;

void main( void )
{
    vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
    color = gl_Color.xyz;
    
    V = normalize( -eyeCoord.xyz );
    L = normalize( lightPos.xyz - eyeCoord.xyz );
    N = normalize( gl_NormalMatrix * gl_Normal );
    
    gl_Position = ftransform();
}