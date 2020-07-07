cbuffer VS_VP : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer VS_World : register(b1)
{
	matrix World;
}

cbuffer PS_Color : register(b0)
{
	float4 pickColor;
}

struct VertexInput
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
};

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	//output.Position = mul(input.Position, World);
	//output.Position = mul(output.Position, View);
	//output.Position = mul(output.Position, Projection);
	
	output.Position = mul(World, input.Position);
	output.Position = mul(View, output.Position);
	output.Position = mul(Projection, output.Position);

	output.Color = input.Color;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	return pickColor;
}