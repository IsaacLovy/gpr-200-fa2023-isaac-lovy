#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];
uniform vec3 _CameraPos;
uniform float _Shininess;
uniform float _DiffuseK;
uniform float _SpecularK;
uniform float _AmbientK;

uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec4 intensity = vec4(0,0,0,1.0);
	for(int i = 0; i < MAX_LIGHTS; i ++)
	{
		vec3 toLight = normalize(_Lights[i].position - fs_in.WorldPosition);
		vec3 toCam = normalize(_CameraPos - fs_in.WorldPosition);

		vec3 diffuseLightI = _Lights[i].color*max(dot(normal, toLight), 0)* _DiffuseK;

		vec3 halfVector = normalize((toLight + toCam)/ (toLight.length() + toCam.length()));
		vec3 specLightI = _Lights[i].color*pow(max(dot(halfVector, normal), 0), _Shininess) * _SpecularK;

		vec3 ambientI = _Lights[i].color * _AmbientK;

		intensity += vec4((diffuseLightI + specLightI + ambientI), 1.0);
	}
	FragColor = intensity * texture(_Texture,fs_in.UV);
}