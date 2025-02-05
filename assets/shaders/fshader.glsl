// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform int passtype;
varying vec2 v_texcoord;

void main()
{
    // Set fragment color from texture
    vec4 tex_2d = texture2D(texture, v_texcoord);

    // Discard fragments for rendering passes
    if(passtype == 0) {
        // Opaque pass, discard transparent fragments
        if(tex_2d.a < 0.9) discard;
    } else {
        // Transparent pass, discard opaque fragments
        if(tex_2d.a > 0.9) discard;
    }

    // Apply FXAA
    float threshold = 0.01;
    vec4 color_right = texture2D(texture, v_texcoord + vec2(1.0 / 500, 0.0)).rgba;
    vec4 color_down = texture2D(texture, v_texcoord + vec2(0.0, 1.0 / 500)).rgba;
    float diff_right = length(tex_2d.rgba - color_right);
    float diff_down = length(tex_2d.rgba - color_down);
    if(diff_right > threshold || diff_down > threshold) {
        tex_2d.rgba = (tex_2d.rgba + color_right + color_down) / 3.0; // Simple blur on edges
        // tex_2d.ra = vec2(1.0, 1.0);
    }

    gl_FragColor = tex_2d;
}

