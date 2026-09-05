#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform int texture_pass;

varying vec2 v_texcoord;

const int OPAQUE_PASS = 0;
const int TRANSPARENT_PASS = 1;

void main()
{
    // Set fragment color from texture
    vec4 tex_color = texture2D(texture, v_texcoord);

    // Remove black pixels (shadows)
    if (tex_color.rgb == vec3(0.0)) discard;

    // Apply tex color or discard based on pass type
    if(texture_pass == OPAQUE_PASS         && tex_color.a < 0.9) discard;
    if(texture_pass == TRANSPARENT_PASS    && tex_color.a > 0.9) discard;

    gl_FragColor = tex_color;
}

