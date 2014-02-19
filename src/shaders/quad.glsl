
#if defined(VERTEX)
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
	vTexCoord = aTexCoord;
	gl_Position = vec4(aVertexPosition, 0.f, 1.f);
}

#endif
/*
#if defined(GEOMETRY)

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vData
{
	vec2 uv;
    vec3 normal;
    vec3 position;
    float t;
}vertices[];

out fData
{
	vec2 uv;
    vec3 normal;
    vec3 position;
    float t;
}frag;

void main()
{
	int i;
	for (i = 0; i < gl_in.length(); ++i)
	{
		//gl_Position = gl_in[i].gl_Position;
		vec4 tempPos = gl_in[i].gl_Position;
		if(gl_PrimitiveIDIn % 2 == 0){
			// tempPos.x += 0.3*cos(vertices[i].t);
		}

		gl_Position = tempPos;

		frag.normal = vertices[i].normal;
		frag.position = vertices[i].position;
		frag.uv = vertices[i].uv;
		frag.t = vertices[i].t;
		EmitVertex();
	}
	EndPrimitive();
}

#endif
*/
#if defined(FRAGMENT)
in vec2 vTexCoord;

out vec4 fFragColor;

uniform sampler2D Texture;

uniform float uBlurSize = 4.0;
uniform int Passe;

void main() {
	vec4 color = vec4(0.f);
	
	float blurSize = uBlurSize;
	
	float blurSizeH = blurSize / 1024.0;
	float blurSizeV = blurSize / 1024.0;
	
	int blurKernelSize = 3;
	int blurHalfSize = (blurKernelSize - 1) / 2;
	
	for(int i = -blurHalfSize; i <= blurHalfSize; ++i){
		for(int j = -blurHalfSize; j <= blurHalfSize; ++j){
			
			color += texture( Texture, vec2(vTexCoord.x + i*blurSizeH, vTexCoord.y + j*blurSizeV) );
			
		}
	}
	
	color /= (blurKernelSize * blurKernelSize);
	
	if(color.r > 0.2 && color.r < 0.3) color.r = 0.25;
	if(color.r > 0.3 && color.r < 0.5) color.r = 0.45;
	if(color.r > 0.5 && color.r < 0.7) color.r = 0.65;
	if(color.r > 0.7) color.r = 0.95;
	
	if(color.g > 0.2 && color.g < 0.3) color.g = 0.25;
	if(color.g > 0.3 && color.g < 0.5) color.g = 0.35;
	if(color.g > 0.5 && color.g < 0.7) color.g = 0.55;
	if(color.g > 0.7) color.g = 0.75;
	
	
	fFragColor = color;
}

#endif
