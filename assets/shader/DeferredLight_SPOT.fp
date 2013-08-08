#version 330

uniform sampler2D color_tex;
uniform sampler2D normal_tex;
uniform sampler2D position_tex;
uniform sampler2D shadow_tex;

uniform mat4 light_mat;
uniform int tex_shadow;

uniform vec3 campos;
uniform vec3 lightpos;
uniform vec3 lightcolor;
uniform vec3 lightdir;
uniform float lightintensity;
uniform float lightrange;
uniform float lightangle;

in vec4 vertexPosition;
out vec4 light_out;

vec4 shade(void)
{
	vec3 pos =  texture(position_tex, ((vertexPosition.xy / vertexPosition.w) + vec2(1.0f))*0.5f).xyz;
	vec3 N = normalize(texture(normal_tex, ((vertexPosition.xy / vertexPosition.w) + vec2(1.0f))*0.5f).xyz);
	vec3 L = lightpos - pos;
	vec3 E = normalize(campos-pos);
	vec3 R = normalize(-reflect(L,N)); 

	float diff = max(dot(N,normalize(L)),0.0f);
	float spec = clamp(pow(max(dot(R,E),0.0),60.0f),0.0f,1.0f);

	float atten1 = 1-min((length(L)/lightrange),1.0f);//linear
	float atten2 = atten1 * atten1;//quadratic
	float atten = (atten1 + atten2) / 2.0f;//combined

	float theta = cos(lightangle/2.0f);
	float umbra = min(theta + 0.03, 1.0f);//hardcoded umbra
	float penumbra = theta;
	
	theta = max(dot(normalize(L),normalize(-lightdir)),0.0f);
	
	float factor;	
	if (theta > umbra)factor = 1.0f;
	else{
		if (theta > penumbra)factor = (theta-penumbra)/(umbra-penumbra);
		else factor = 0.0f;
	}

	vec4 base = (texture(color_tex, ((vertexPosition.xy / vertexPosition.w)+ vec2(1.0f))*0.5f) * vec4(lightcolor,1.0f)) * atten;
	return vec4((base * diff + base * spec).xyz * lightintensity * factor,1.0f);
}

void main(void)
{
	if(tex_shadow != 0)
	{
		vec4 shadowpos =  vec4(texture(position_tex, ((vertexPosition.xy / vertexPosition.w) + vec2(1.0f))*0.5f).xyz, 1.0f);
		shadowpos = light_mat * shadowpos;
		shadowpos = shadowpos / shadowpos.w ;
		float dist = texture(shadow_tex,((shadowpos.xy) + vec2(1.0f))*0.5f).x;
		float disthardening = max((shadowpos.z+1.0)*0.5f-dist,0.0f);
		if(dist+0.0001f<=(shadowpos.z+1.0)*0.5f)light_out = vec4(0.0,0.0,0.0,1.0);
		else light_out = shade();	
	}
	else{
		light_out = shade();
	}
}