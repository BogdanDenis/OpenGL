#version 430 core
	layout(triangles) in;
	layout(line_strip, max_vertices = 3) out;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	
	in Data {
		vec3 Normal;
		vec4 Position;
	} vdata[3];

	out vec3 fragPos;
	out vec3 Normal;

	void main () {
		vec4 middle = (vdata[0].Position + vdata[1].Position + vdata[2].Position)/3;
		middle.w = 1.0;
		vec3 normal = normalize((vdata[0].Normal + vdata[1].Normal + vdata[2].Normal)/3);

		gl_Position = projection * view * model * middle;
		EmitVertex();
		
		gl_Position = projection * view * model  * (middle + vec4(normal*0.4, 1.0));
		EmitVertex();
		Normal = normal;
		fragPos = middle.xyz;

		EndPrimitive();
	}
