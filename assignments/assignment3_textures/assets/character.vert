#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float _Scale;
uniform vec2 _Center;
uniform float _Radius;
uniform float _Time;
uniform float _TimeScale;
void main(){	
	vec3 scaledPos = vPos * _Scale;

	vec3 offset = vec3(sin(_Time*_TimeScale) * _Radius, cos(_Time*_TimeScale) * _Radius, 0);

	UV = vUV;
	gl_Position = vec4(scaledPos + offset,1.0);
}