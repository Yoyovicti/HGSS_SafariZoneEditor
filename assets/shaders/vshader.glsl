#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;
// attribute vec3 a_normal;
attribute vec2 a_texcoord;

// varying vec3 v_normal;
varying vec2 v_texcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
    // v_normal = normalize(mat3(mvp_matrix) * a_normal);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
