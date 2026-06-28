#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;

void main()
{
    vec4 screenPos = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
    gl_Position = screenPos;
    v_Color = vec4(screenPos);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;

void main()
{
    color = v_Color;
}