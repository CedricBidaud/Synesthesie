
#if defined(VERTEX)
layout(location = 0) in vec2 aVertexPosition;

void main() {
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
out vec3 fFragColor;

uniform vec3 uPolygonColor;

void main() {
	fFragColor = uPolygonColor;
	//~ fFragColor = vec3(0.f,1.f,0.f);
}

#endif
