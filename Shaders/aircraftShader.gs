// ================
// Geometry shader:
// ================
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 Normal; 
	vec3 FragPos; 
	vec2 TexCoords; 
}gs_in;

out vec2 TexCoords; 

out VS_OUT {
	vec3 Normal; 
	vec3 FragPos; 
	vec2 TexCoords; 
} gs_out;

uniform float time;
uniform int isHit;

vec4 explode(vec4 position, vec3 normal)
{
    if (isHit>0)
    {
        float magnitude = 10.0f;
        vec3 direction = normal * (-(time) ) * magnitude; 
        return position + vec4(direction, 0.0f) ;//+ vec4(0.0f, -1.0f * time*time*time, 0.0f , 0.0f);
    }
    else
    {
        return position;
    }
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.TexCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}
