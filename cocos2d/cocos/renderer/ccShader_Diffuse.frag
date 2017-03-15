const char* ccPosition_Diffuse_frag = STRINGIFY(
                                             
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float outlineSize;
uniform vec3 outlineColor;
uniform vec2 texSize;
uniform vec3 foregroundColor;

int getIsStrokeWithAngelIndex(float cosA, float sinA)
{
    int stroke = 0;
    float a = texture2D(CC_Texture0, vec2(v_texCoord.x + outlineSize * cosA / texSize.x, v_texCoord.y + outlineSize * sinA / texSize.y)).a;
    if (a >= 0.5)
    {
        stroke = 1;
    }
    
    return stroke;
}

void main()
{
    vec4 myC = texture2D(CC_Texture0, v_texCoord);
    myC.rgb *= foregroundColor;
    
    if (myC.a >= 0.5)
    {
        gl_FragColor = v_fragmentColor * myC;
        return;
    }
    
    int strokeCount = 0;
    strokeCount += getIsStrokeWithAngelIndex(1.0, 0.0);
    strokeCount += getIsStrokeWithAngelIndex(0.866, 0.5);
    strokeCount += getIsStrokeWithAngelIndex(0.5, 0.866);
    strokeCount += getIsStrokeWithAngelIndex(0.0, 1.0);
    strokeCount += getIsStrokeWithAngelIndex(-0.5, 0.866);
    strokeCount += getIsStrokeWithAngelIndex(-0.866, 0.5);
    strokeCount += getIsStrokeWithAngelIndex(-1, 0.0);
    strokeCount += getIsStrokeWithAngelIndex(-0.866, -0.5);
    strokeCount += getIsStrokeWithAngelIndex(-0.5, -0.866);
    strokeCount += getIsStrokeWithAngelIndex(0.0, -1.0);
    strokeCount += getIsStrokeWithAngelIndex(0.5, -0.866);
    strokeCount += getIsStrokeWithAngelIndex(0.866, -0.5);

    bool stroke = false;
    if (strokeCount > 0)
    {
        stroke = true;
    }
    
    if (stroke)
    {
        myC.rgb = outlineColor;
        myC.a = 1.0;
    }
    vec4 test = vec4(0,1,1,1);
    gl_FragColor = v_fragmentColor * test;
}
);
