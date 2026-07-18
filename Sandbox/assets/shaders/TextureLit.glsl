#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec2 u_TexScale;

out vec2 v_TexCoord;
out vec2 v_WorldPosition;

void main()
{
    vec4 worldPosition = u_Transform * vec4(a_Position * 2.0, 1.0);

    v_WorldPosition = worldPosition.xy;

    v_TexCoord = a_TexCoord * u_TexScale;
    gl_Position = u_ViewProjection * worldPosition;
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec2 v_WorldPosition;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

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
    vec4 textureColor = texture(u_Texture, v_TexCoord) * u_Color;

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
