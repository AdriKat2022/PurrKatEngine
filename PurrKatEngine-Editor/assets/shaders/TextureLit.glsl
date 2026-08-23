#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec2 a_UVTiling;
layout(location = 4) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_WorldPosition;
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    vec4 worldPos = vec4(a_Position, 1.0);    

    v_WorldPosition = vec2(worldPos.x, worldPos.y);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord * a_UVTiling;
    v_TexIndex = a_TexIndex;
    gl_Position = u_ViewProjection * worldPos;
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_WorldPosition;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

const int MAX_LIGHTS = 16;

struct LightSource
{
    vec2 Position;
    vec3 Color;
    float Radius;
    float Intensity;
};

uniform int u_LightCount;
uniform LightSource u_Lights[MAX_LIGHTS];
uniform float u_AmbientStrength;


void main()
{
    vec4 textureColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
    
    vec3 lighting = textureColor.rgb * max(u_AmbientStrength, 0.0);

    for (int i = 0; i < u_LightCount; ++i)
    {
        float radius = max(u_Lights[i].Radius, 0.0001);
        float distanceToLight = distance(v_WorldPosition, u_Lights[i].Position);
        float attenuation = 1.0 - smoothstep(0.0, radius, distanceToLight);

        lighting += textureColor.rgb * u_Lights[i].Color
                  * attenuation * max(u_Lights[i].Intensity, 0.0);
    }

    color = vec4(lighting, textureColor.a);
}
