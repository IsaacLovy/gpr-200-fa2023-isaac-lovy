/*

	Authors: Henry Foley & Isaac Lovy

*/

#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
	mat3 TBN;
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
uniform float _NormalIntensity;
uniform float _HeightScale;

uniform sampler2D _Texture;
uniform sampler2D _NormalTex;
uniform sampler2D _DepthMap;

uniform int _ParallaxClipping;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	//Number of Depth Layers
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
	//Calculate the size of each Layer
	float layerDepth = 1.0/numLayers;
	//Depth of current layer
	float currentLayerDepth = 0.0;

	vec2 P = viewDir.xy * _HeightScale;
	vec2 deltaTexCoords = P/numLayers;

	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = texture(_DepthMap, currentTexCoords).r;
	
	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(_DepthMap, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}
	
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(_DepthMap, prevTexCoords).r - currentLayerDepth +layerDepth;

	float weight = afterDepth / (afterDepth -beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight +currentTexCoords * (1.0- weight);

	//float height = texture(_DepthMap, texCoords).r;
	//Invert Height Map
	//height = 1.0 - height;
	//vec2 p = viewDir.xy / viewDir.z * (height * _HeightScale);
	return finalTexCoords;
}

void main(){
	//vec3 normal = normalize(fs_in.WorldNormal);
	mat3 transposedTBN = transpose(fs_in.TBN);
	vec3 tanViewDir = normalize((transposedTBN * _CameraPos) - (transposedTBN * fs_in.WorldPosition));

	//Parallax UVs
	vec2 texCoords = ParallaxMapping(fs_in.UV, tanViewDir);

	if (_ParallaxClipping == 1)
		if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
			discard;

	//Normals
	vec3 normal = texture(_NormalTex, texCoords).rgb * 2.0 - 1.0;
	normal = mix(vec3(0,0,1), normal, _NormalIntensity);
	normal = normalize(normal);
	normal = normalize(fs_in.TBN * normal);

	//Lighting
	vec4 intensity = vec4(0,0,0,1.0);
	//vec3 toCam = normalize(_CameraPos - fs_in.WorldPosition);
	vec3 toCam = tanViewDir;
	for(int i = 0; i < MAX_LIGHTS; i ++)
	{
		vec3 toLight = normalize(_Lights[i].position - fs_in.WorldPosition);

		vec3 diffuseLightI = _Lights[i].color*max(dot(normal, toLight), 0)* _DiffuseK;

		vec3 halfVector = normalize((toLight + toCam)/ (toLight.length() + toCam.length()));
		vec3 specLightI = _Lights[i].color*pow(max(dot(halfVector, normal), 0), _Shininess) * _SpecularK;

		vec3 ambientI = _Lights[i].color * _AmbientK;

		intensity += vec4((diffuseLightI + specLightI + ambientI), 1.0);
	}

	//Output
	FragColor = intensity * texture(_Texture,texCoords);
}