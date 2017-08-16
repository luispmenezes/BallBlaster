#version 120

varying vec3 f_corRGB;

void main( void )
{
    gl_FragColor = vec4( f_corRGB, 1.0 );
}
