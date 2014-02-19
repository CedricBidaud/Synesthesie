#if defined(VERTEX)

in vec2 VertexPosition;
in vec2 VertexTexCoord;

uniform vec2 uParticlePosition;
uniform float uParticleScale;

out vec2 vTexCoord;

out vec2 vFragPosition;

void main() {
	vFragPosition = VertexPosition;
	vTexCoord = VertexTexCoord;
	
	gl_Position = vec4(uParticlePosition + uParticleScale * VertexPosition, 0.f, 1.f);
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
in vec2 vFragPosition;
in vec2 vTexCoord;

out vec4 fFragColor;

uniform vec3 uParticleColor;

float computeAttenuation(float distance) {
	return 3.f * exp(-distance * distance * 9.f);
}

void main() {
	float distance = length(vFragPosition);
	float attenuation = computeAttenuation(distance);
	fFragColor = vec4(uParticleColor, attenuation);
}
#endif
