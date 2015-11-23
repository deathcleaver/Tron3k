#version 410
layout (location = 0) in vec2 UV;

uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D UVcord;	
uniform sampler2D Depth;

vec4 Position0;
vec4 Diffuse0;
vec4 Normal0;
vec4 Depth0;

struct SpotLight
{
	vec3 Color;
	float DiffuseIntensity;
	vec3 Position;
	float AmbientIntensity;
	vec3 Direction;
	float Cutoff;
	float Constant;
	float Linear;
	float Exp;
	float padd;
};

layout (std140) uniform Light
{ 
	SpotLight lights[30];
};

uniform int NumSpotLights;
uniform int NumSpotLightsShadow; 
uniform vec3 eyepos;

uniform sampler2D ShadowMaps;
uniform mat4 ProjectionMatrixSM;
uniform mat4 ViewMatrixSM;

float gSpecularPower = 20;
float gMatSpecularIntensity = 0.4;

out vec4 fragment_color;
					
vec4 CalcLightInternal(SpotLight l, vec3 LightDirection, vec3 Normal)                   
{                                                                                           
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;                   
	float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                           
	vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  
                                                                                           
	if (DiffuseFactor > 0) 
	{                                                                
		DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;    
                                                                                           
		vec3 VertexToEye = normalize(eyepos - Position0.xyz);                             
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
		float SpecularFactor = dot(VertexToEye, LightReflect);                              
		SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
		if (SpecularFactor > 0) 
		{                                                           
			SpecularColor = vec4(l.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;                         
		}                                                                                   
	}                                                                                       
                                                                                   
	return (AmbientColor + DiffuseColor + SpecularColor);                                   
}               

vec4 CalcPointLight(SpotLight l, vec3 Normal)
{
	vec3 LightDirection = Position0.xyz - l.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);    

	return (CalcLightInternal(l, LightDirection, Normal));                          
}                                                                                           
                                                                                           
vec4 CalcSpotLight(SpotLight l, vec3 Normal)                                                
{                                                                                           
	vec3 LightToPixel = normalize(Position0.xyz - l.Position);                             
	float SpotFactor = dot(LightToPixel, l.Direction);                                      
                                                                                           
	if (SpotFactor > l.Cutoff) {                                                            
		vec4 Color = CalcPointLight(l, Normal);                             
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   
	}                                                                                       
	else {                                                                                  
		return vec4(0,0,0,0);                                                               
	}     
}  		

void main()
{
	if(Use == 0)
	{
		float Depth = texture(Depth, vec2(UV.x, UV.y)).x;
		fragment_color = vec4(1.0 - (1.0 - Depth) * 25.0); 
	}
	else if(Use == 1)
		fragment_color = texture(Position, vec2(UV.x, UV.y)) * 0.25;
	else if(Use == 2)	 
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else if(Use == 3)	 
		fragment_color = texture(Normal, vec2(UV.x, UV.y));
	else if(Use == 4)	 
		fragment_color = texture(UVcord, vec2(UV.x, UV.y));
	else if(Use == 5)
	{
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	}
	else
		fragment_color = vec4(0,1,1,1);		
}