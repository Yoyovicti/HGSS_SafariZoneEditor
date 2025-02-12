#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform int passtype;

varying vec2 v_texcoord;
// varying vec3 v_normal;

void main()
{
    // Set fragment color from texture
    vec4 tex_color = texture2D(texture, v_texcoord);

    // Apply tex color or discard based on pass type
    if(passtype == 0 && tex_color.a < 0.9) discard;
    if(passtype == 1 && tex_color.a > 0.9) discard;

    // Check normals
    // gl_FragColor = vec4(v_normal * 0.5 + 0.5, 1.0);

    gl_FragColor = tex_color;
}

