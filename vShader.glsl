#version 120

attribute vec3 v_coord3d;

attribute vec3 v_corRGB;

uniform mat4x4 matrizProj;

uniform mat4x4 matrizModelView;

varying vec3 f_corRGB;

void main( void )
{
    gl_Position = matrizProj * matrizModelView * vec4( v_coord3d, 1.0 );

    f_corRGB = v_corRGB;
}
