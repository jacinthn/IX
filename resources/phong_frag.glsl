#version 120

uniform vec4 lightAmbt, lightDiff, lightSpec;
varying vec3 N, V, L, color;

void main( void )
{
    vec3 normal = normalize( N );
    vec3 R = -normalize( reflect( L, normal ) );
    
    vec3 ambient  = color * lightAmbt.xyz;
    vec3 diffuse  = color * lightDiff.xyz * max( dot( normal, L ), 0.0);
    vec3 specular = color * lightSpec.xyz * pow( max( dot( R, V ), 0.0), 60.0 );
    
    gl_FragColor.xyz = ambient + diffuse + specular;
    gl_FragColor.a = 1.0;
}