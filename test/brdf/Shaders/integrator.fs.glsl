out vec4 o_Color;

const float PI = 3.14159265359;

/*
 * Alpha remap
 */

subroutine float AlphaRemap(float f_Alpha);
subroutine uniform AlphaRemap s_AlphaRemap;

subroutine(AlphaRemap) float AR_Direct(float f_Alpha)
{
	float alpha = f_Alpha + 1;
	return (alpha * alpha) / 8;
}

subroutine(AlphaRemap) float AR_IBL(float f_Alpha)
{
	return (f_Alpha * f_Alpha) / 2;
}

/*
 * Normal distribution
 */

subroutine float NormalDistribution(float f_CosNormalHalfway, float f_Roughness);
subroutine uniform NormalDistribution s_NormalDistribution;

subroutine(NormalDistribution) float ND_Trowbridge(float f_CosNormalHalfway, float f_Roughness)
{
	float alpha = f_Roughness * f_Roughness;
	float alpha2 = alpha * alpha;
	float pok = (f_CosNormalHalfway * f_CosNormalHalfway) * (alpha2 - 1) + 1;
	return alpha2 / (4 * (pok * pok));
}

subroutine(NormalDistribution) float ND_Beckmann(float f_CosNormalHalfway, float f_Roughness)
{
	float alpha = f_Roughness * f_Roughness;
	float alpha2 = alpha * alpha;
	float cosNormalHalfway2 = f_CosNormalHalfway * f_CosNormalHalfway;
	float numerator = exp((cosNormalHalfway2 - 1) / (alpha2 * cosNormalHalfway2));
	float denominator = 4 * alpha2 * cosNormalHalfway2 * cosNormalHalfway2;

	return numerator / denominator;
}

subroutine(NormalDistribution) float ND_Blinn(float f_CosNormalHalfway, float f_Roughness)
{
	float alpha = f_Roughness * f_Roughness;
	float alpha2 = alpha * alpha;
	float exponent = 2 / alpha2 - 2;
	return (1 / (4 * alpha2)) * pow(max(f_CosNormalHalfway, 0), exponent);
}

/*
 * GGXAnisotropic not supported as the signature differs from that of subroutine(NormalDistribution)
 */

float ND_GGXAnisotropic(vec3 f_Normal, vec3 f_Halfway, vec3 f_Tangent, vec3 f_Bitangent, float f_XRoughness, float f_YRoughness)
{
	float cosNormalHalfway = dot(f_Normal, f_Halfway);

	float cosTangentHalfway = dot(f_Tangent, f_Halfway);
	float cosBitangentHalfway = dot(f_Bitangent, f_Halfway);

	float xAlpha = f_XRoughness * f_XRoughness;
	float yAlpha = f_YRoughness * f_YRoughness;

	float pok = 1 / (((cosTangentHalfway * cosTangentHalfway) / (xAlpha * xAlpha)) + ((cosBitangentHalfway * cosBitangentHalfway) / (yAlpha * yAlpha)) + cosNormalHalfway * cosNormalHalfway);

	return (1 / (4 * xAlpha * yAlpha)) * pok * pok;
}

/*
 * Geometry occlusion
 */

subroutine float GeometryOcclusion(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness);
subroutine uniform GeometryOcclusion s_GeometryOcclusion;

subroutine(GeometryOcclusion) float GO_Beckmann(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	float cosNormalView = max(dot(f_Normal, f_View), 0.0);

	float remapped = s_AlphaRemap(f_Roughness * f_Roughness);

	float c = cosNormalView / (remapped * remapped * sqrt(1 - cosNormalView * cosNormalView));

	if (c >= 1.6)
	{
		return 1;
	}

	float c2 = c * c;
	return (3.535 * c + 2.181 * c2) / (1 + 2.276 * c + 2.577 * c2);
}

subroutine(GeometryOcclusion) float GO_Implicit(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	return max(dot(f_Normal, f_Light), 0.0) * max(dot(f_Normal, f_View), 0.0);
}

subroutine(GeometryOcclusion) float GO_Neumann(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	float cosNormalView = max(dot(f_Normal, f_View), 0.0);
	float cosNormalLight = max(dot(f_Normal, f_Light), 0.0);

	return (cosNormalView * cosNormalLight) / max(cosNormalView, cosNormalLight);
}

subroutine(GeometryOcclusion) float GO_CookTorrance(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	float cosNormalHalfway = max(dot(f_Normal, f_Halfway), 0.0);
	float cosNormalView = max(dot(f_Normal, f_View), 0.0);
	float cosNormalLight = max(dot(f_Normal, f_Light), 0.0);
	float cosViewHalfway = max(dot(f_View, f_Halfway), 0.0);

	float pok = 2 * cosNormalHalfway / cosViewHalfway;

	return min(min(pok * cosNormalView, pok * cosNormalLight), 1);
}

subroutine(GeometryOcclusion) float GO_Kelemen(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	float cosNormalView = max(dot(f_Normal, f_View), 0.0);
	float cosNormalLight = max(dot(f_Normal, f_Light), 0.0);
	float cosViewHalfway = dot(f_View, f_Halfway);

	return (cosNormalLight * cosNormalView) / (cosViewHalfway * cosViewHalfway);
}

float Schlick(vec3 f_Normal, vec3 f_Vector, float f_Roughness)
{
	float cosNormalVector = max(dot(f_Normal, f_Vector), 0.0);
	return cosNormalVector / (cosNormalVector * (1 - f_Roughness) + f_Roughness);
}

subroutine(GeometryOcclusion) float GO_Smith(vec3 f_Light, vec3 f_View, vec3 f_Normal, vec3 f_Halfway, float f_Roughness)
{
	float remapped = s_AlphaRemap(f_Roughness * f_Roughness);
	float schlickLight = Schlick(f_Normal, f_Light, remapped);
	float schlickView = Schlick(f_Normal, f_View, remapped);
	return schlickLight * schlickView;
}

/*
 * Fresnel
 */

subroutine vec3 Fresnel(vec3 f_View, vec3 f_Halfway, vec3 f_Reflectance);
subroutine uniform Fresnel s_Fresnel;

subroutine(Fresnel) vec3 F_None(vec3 f_Halfway, vec3 f_View, vec3 f_Reflectance)
{
	return f_Reflectance;
}

subroutine(Fresnel) vec3 F_Schlick(vec3 f_Halfway, vec3 f_View, vec3 f_Reflectance)
{
	float cosHalfwayView = max(dot(f_Halfway, f_View), 0);
	float pok = 1 - cosHalfwayView;
	return f_Reflectance + (1 - f_Reflectance) * pok * pok * pok * pok * pok;
}

subroutine(Fresnel) vec3 F_CookTorrance(vec3 f_Halfway, vec3 f_View, vec3 f_Reflectance)
{
	float cosHalfwayView = dot(f_Halfway, f_View);

	vec3 sqrtReflectance = sqrt(f_Reflectance);

	vec3 eta = (1 + sqrtReflectance) / (1 - sqrtReflectance);

	vec3 g = sqrt(eta * eta + cosHalfwayView * cosHalfwayView - 1);

	vec3 sub = g - cosHalfwayView;
	vec3 add = g + cosHalfwayView;

	vec3 div = sub / add;

	vec3 tos = (add * (cosHalfwayView - 1)) / (sub * (cosHalfwayView + 1));

	return ((div * div) / 2) * (1 + (tos * tos));
}

vec3 BRDF(
	vec3 f_Normal,
	vec3 f_View,
	vec3 f_Light,
	vec3 f_Halfway,
	vec3 f_Reflectance,
	vec3 f_Albedo,
	vec3 f_Radiance,
	float f_Roughness,
	float f_Metallic
)
{
	float cosNormalView = max(dot(f_Normal, f_View), 0.0);
	float cosNormalLight = max(dot(f_Normal, f_Light), 0.0);
	float cosNormalHalfway = dot(f_Normal, f_Halfway);

	float trowbridge = s_NormalDistribution(cosNormalHalfway, f_Roughness);
	float smith = s_GeometryOcclusion(f_Light, f_View, f_Normal, f_Halfway, f_Roughness);
	vec3 fresnel = s_Fresnel(f_View, f_Halfway, f_Reflectance);

	vec3 numerator = trowbridge * smith * fresnel;
	float denominator = 4.0 * cosNormalView * cosNormalLight + 0.0001;
	vec3 specular = numerator / denominator;

	vec3 kD = vec3(1.0) - fresnel;

	kD *= (1.0 - f_Metallic);

	return (kD * f_Albedo / PI + specular) * f_Radiance * cosNormalLight;
}

uniform vec3 u_Albedo;
uniform vec4 u_World;
uniform vec3 u_BaseReflectivity;
uniform float u_Metalness;
uniform float u_Roughness;
uniform float u_AmbientWorld;
uniform float u_Ambient;

struct Point
{
	vec3 Position;
	vec3 Color;
};
uniform int u_PointCount;
uniform Point u_PointLight[LIGHT_ARRAY_SIZE];

uniform vec3 u_ViewPosition;

struct v_Out
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 Texture;
};

in v_Out Vertex;

void main()
{
	vec3 albedo = pow(u_Albedo, vec3(2.2));
	vec3 reflectance = mix(u_BaseReflectivity, albedo, u_Metalness);
	vec3 view = normalize(u_ViewPosition - Vertex.Position);
	vec3 normal = normalize(Vertex.Normal);

	vec3 color = vec3(0.0);
	for(int i = 0; i < u_PointCount; i++) 
	{
		vec3 light = u_PointLight[i].Position - Vertex.Position;
		float distance = length(light);
		light = normalize(light);

		vec3 halfway = normalize(view + light);

		vec3 radiance = u_PointLight[i].Color / (distance * distance);

		color += BRDF(normal, view, light, halfway, reflectance, albedo, radiance, u_Roughness, u_Metalness);
	}

	vec3 ambient = u_Ambient * albedo + (u_AmbientWorld * u_World.xyz * dot(normal, view));

	color += ambient;

	color = color / (color + vec3(1.0));

	color = pow(color, vec3(1.0 / 2.2));

	o_Color = vec4(color, 1.0);
}