#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;

void main()
{
    vec4 tex_color = texture2D(texture, v_texcoord);
    if(tex_color.a < 0.9) discard;

    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

