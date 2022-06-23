Texture2D diffTexture;
SandboxrState SandboxType;

float4 main(float4 pos : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 textureColor = diffTexture.Sandbox(SandboxType, texcoord);

	return textureColor;
}