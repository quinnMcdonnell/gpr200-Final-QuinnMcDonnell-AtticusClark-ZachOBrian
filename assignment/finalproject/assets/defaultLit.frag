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

struct Material 
{
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};

#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];
uniform int numLights;

uniform Material _Material;
uniform vec3 _CameraPosition;
uniform sampler2D _Texture;

void main(){
	
	

	int numOfLights = numLights;
	if(numLights > 4)
	{
		numOfLights = 4;
	}

	vec4 newTex = texture(_Texture,fs_in.UV);
	vec3 texColor = newTex.rgb;

	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 position = fs_in.WorldPosition;
	vec3 I;

	vec3 totalLight = vec3(0);

	for(int i = 0; i < numOfLights; i++)
	{
	vec3 LightPosition = _Lights[i].position;
	vec3 omega = normalize(LightPosition - position); //Omega Vector
	vec3 v = normalize(_CameraPosition - position);
	vec3 h = normalize(omega + v);

	vec3 Amb = _Lights[i].color * _Material.ambientK;
	vec3 Dif = _Lights[i].color * _Material.diffuseK * max(dot(omega, normal),0);
	vec3 Spec = _Lights[i].color * _Material.specular * pow(max(dot(h,normal),0),_Material.shininess);

	totalLight += Amb;
	totalLight += Dif;
	totalLight += Spec;
	}

	texColor *= totalLight;
	FragColor = vec4(texColor,1);
}