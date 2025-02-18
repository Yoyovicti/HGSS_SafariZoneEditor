#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform int highlight;
uniform float time;

varying vec2 v_texcoord;

void main()
{
    vec4 tex_color = texture2D(texture, v_texcoord);
    if(tex_color.a < 0.9) discard;

    float t = 0.5 * (sin(3.14159 * time) + 1);
    vec3 white = vec3(1.0, 1.0, 1.0);
    vec3 blue = vec3(100.0 / 255, 100.0 / 255, 1.0);
    vec3 red = vec3(1.0, 100.0 / 255, 100.0 / 255);
    vec3 highlight_color = mix(red, blue, t);

    vec4 outline_color = vec4(white, 1.0);
    if(highlight == 1) outline_color = vec4(highlight_color, 1.0);
    // outline_color.r = t;
    gl_FragColor = outline_color;
}

