#version 450
out vec4 FragColor;
uniform float _Time;
uniform float _TimeScale;
uniform vec2 _WindowSize;
uniform vec3 _SkyCol;
uniform vec3 _GroundCol;
uniform vec3 _BGMixCol;
uniform vec4 _SunCol;
uniform vec3 _SkylineCol;
uniform float _SunRadius;

float circleSDF(vec2 p, float r){
    return length(p)-r;
}

void main(){

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/_WindowSize.xy;
    //0,0 center of screen
    vec2 sdfUV = (2.0 * gl_FragCoord.xy - _WindowSize.xy) / _WindowSize.y;

    float normalTime = sin(_Time * _TimeScale) * 0.5 + 0.5;

    float bgVal= fract(uv.y);
    vec3 bgCol = mix(_GroundCol,_SkyCol,bgVal);

    bgCol = mix(bgCol, _BGMixCol, normalTime * 0.5);
    
    vec3 color = bgCol;
   
    vec2 sunPos = vec2(0.0, (1.0-normalTime) - 0.5);
    float d = circleSDF(sdfUV - sunPos, _SunRadius);
    d = smoothstep(0.05, -0.05, d);
    
    color = mix(color, _SunCol.rgb, d * _SunCol.a);

    float t = sin(uv.x * 50.0) * mod(uv.x * 20.0, 2.0) / 20.0;
    t = ceil(t * 25.0) / 25.0;
    t = t + 0.45;
    t = step(t, uv.y);
    
    color = mix(color, _SkylineCol, 1.0-t); 
    
    // Output to screen
    FragColor = vec4(color,1.0);
}