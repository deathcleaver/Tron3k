#version 410

layout(location = 0) in vec4 dist;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main () 
{
	float maxlen = 20;
	float transp = length(dist);
	
	if(transp > maxlen)
		transp = 0.0;
	else
		transp =  1 - (transp / maxlen);
  
	WorldPosOut	= vec4(0);
	DiffuseOut = vec4(1.25 - transp , 0 , transp, transp / 4);	
	NormalOut = vec4(0);
	GlowMap = vec4(0);
}