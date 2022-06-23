layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in vec3 l_Tangent;
layout (location = 3) in vec3 l_Bitangent;
layout (location = 4) in vec2 l_Texture;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

struct v_Out
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 Texture;
};

out v_Out Vertex;

void main()
{
	Vertex.Position = vec3(u_Model * vec4(l_Position, 1.0));
	Vertex.Normal = mat3(u_Model) * l_Normal;
	Vertex.Tangent = l_Tangent;
	Vertex.Bitangent = l_Bitangent;
	Vertex.Texture = l_Texture;

	gl_Position = u_ViewProjection * vec4(Vertex.Position, 1.0);
}