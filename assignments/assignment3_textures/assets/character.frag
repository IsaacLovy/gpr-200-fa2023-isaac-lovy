#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;
uniform float _Alpha;

void main(){
	
	vec4 col = texture(_Texture,UV);

	FragColor = vec4(col.xyz, col.w - _Alpha);
}