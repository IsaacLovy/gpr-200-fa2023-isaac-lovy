#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;
uniform sampler2D _Overlay;
uniform vec4 _OverlayColor;
uniform float _Time;
uniform float _TimeScale;

void main(){
	
	vec4 col = texture(_Texture,UV);

	vec2 overlayUV =  vec2(UV.x + (_Time *_TimeScale), UV.y);

	float a = texture(_Overlay,overlayUV).x;

	a = a - 0.2;

	FragColor = vec4(mix(col, _OverlayColor, a).xyz, 1.0);
}