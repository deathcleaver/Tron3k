#version 410
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 skinWeights;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;
uniform mat4 inverseBindpose[100];

layout(location = 0) out vec3 bonePos0;
layout(location = 1) out vec2 boneUV0;
layout(location = 2) out vec3 boneNormal0;
layout(location = 3) out vec3 boneTagent0;

void main()
{
	bonePos0 = (vec4(pos, 1.0f) * inverseBindpose[boneIndices.x]) * skinWeights.x;
	boneNormal0 = (vec4(normal, 1.0f) * inverseBindpose[boneIndices.x]) * skinWeights.x;
	boneTagent0 = (vec4(tangent, 1.0f) * inverseBindpose[boneIndices.x]) * skinWeights.x;
	
	bonePos0 = (vec4(pos, 1.0f) * inverseBindpose[boneIndices.y]) * skinWeights.y;
	boneNormal0 = (vec4(normal, 1.0f) * inverseBindpose[boneIndices.y]) * skinWeights.y;
	boneTagent0 = (vec4(tangent, 1.0f) * inverseBindpose[boneIndices.y]) * skinWeights.y;
	
	bonePos0 = (vec4(pos, 1.0f) * inverseBindpose[boneIndices.z]) * skinWeights.z;
	boneNormal0 = (vec4(normal, 1.0f) * inverseBindpose[boneIndices.z]) * skinWeights.z;
	boneTagent0 = (vec4(tangent, 1.0f) * inverseBindpose[boneIndices.z]) * skinWeights.z;
	
	bonePos0 = (vec4(pos, 1.0f) * inverseBindpose[boneIndices.w]) * skinWeights.w;
	boneNormal0 = (vec4(normal, 1.0f) * inverseBindpose[boneIndices.w]) * skinWeights.w;
	boneTagent0 = (vec4(tangent, 1.0f) * inverseBindpose[boneIndices.w]) * skinWeights.w;


	bonePos0 = (vec4(bonePos0, 1.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0, 1.0);
	boneUV0 = uv;
}