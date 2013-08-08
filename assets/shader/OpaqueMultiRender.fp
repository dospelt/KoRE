#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 vertPosition_global;
in vec3 vertNormals_global;
in vec3 vertTangents_global;
in vec3 vertNormals_view;

layout(location = 0 ) out vec4 tColor;
layout(location = 1 ) out vec4 tWorldPosition;
layout(location = 2 ) out vec4 tWorldNormals;

uniform sampler2D mesh_tex;
uniform sampler2D normal_tex;
uniform sampler2D attribute_tex;//TODO

void main(void)
{	
	mat3 tangentspace_mat = mat3(vertTangents_global,cross(vertTangents_global,vertNormals_global),vertNormals_global);
	vec3 tex_normal = tangentspace_mat * ((texture(normal_tex, UV).xyz * 2.0f)- vec3(1.0f));
	tColor = vec4(texture(mesh_tex, UV).xyz, 1.0f);//emissive material factor as alpha?
	tWorldPosition = vec4(vertPosition_global, 1.0f);//specular material factor as alpha?
	tWorldNormals = vec4(normalize(tex_normal), 1.0f);//shininess factor as alpha?
}