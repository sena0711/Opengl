#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_GS_VERTEX{
    out vec4 position;
    out vec3 color;
    out mat4 mvp;
} vs_out;


out vec4 mWorldPos;

uniform mat4 model;
uniform mat4 mvp;

void main(){
    gl_Position = mvp * vec4(position, 1.0f);
    vs_out.color = color;
    vs_out.position = gl_Position;
    vs_out.mvp = mvp;

	
	// for  fog shader
	mWorldPos = model *vec4(position, 1.0);
	
}