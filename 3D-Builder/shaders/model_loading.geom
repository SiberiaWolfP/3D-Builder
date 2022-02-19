#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 GNormal;
out vec3 GPosition;
out vec2 GTexCoords;
out vec4 GColors;
noperspective out vec3 GEdgeDistance;
in vec3 Normal[];
in vec3 FragPos[];
in vec2 TexCoords[];
in vec4 Colors[];
uniform mat4 ViewportMatrix; // Viewport matrix

// 一般模型的几何着色器，主要用于得出每个面片的三个高
void main()
{
	// Transform each vertex into viewport space
	vec3 p0	= vec3(ViewportMatrix * (gl_in[0].gl_Position /
	gl_in[0].gl_Position.w));
	vec3 p1 = vec3(ViewportMatrix * (gl_in[1].gl_Position /
	gl_in[1].gl_Position.w));
	vec3 p2 = vec3(ViewportMatrix * (gl_in[2].gl_Position /
	gl_in[2].gl_Position.w));
	// Find the altitudes (ha, hb and hc)
	float a			= length(p1 - p2);
	float b			= length(p2 - p0);
	float c			= length(p1 - p0);
	float alpha		= acos( (b*b + c*c - a*a) / (2.0*b*c) );
	float beta		= acos( (a*a + c*c - b*b) / (2.0*a*c) );
	float ha		= abs( c * sin( beta ) );
	float hb		= abs( c * sin( alpha ) );
	float hc		= abs( b * sin( alpha ) );
	// 给每个顶点添加其相对对面边的高信息，三个高经插值后即可作为点与边的距离使用
	GEdgeDistance	= vec3( ha, 0, 0 );
	GNormal			= Normal[0]; 
	GPosition		= FragPos[0];
	GTexCoords		= TexCoords[0];
	GColors			= Colors[0];
	gl_Position		= gl_in[0].gl_Position;
	EmitVertex();
	GEdgeDistance	= vec3( 0, hb, 0 );
	GNormal			= Normal[1];
	GPosition		= FragPos[1];
	GTexCoords		= TexCoords[1];
	GColors			= Colors[1];
	gl_Position		= gl_in[1].gl_Position;
	EmitVertex();
	GEdgeDistance	= vec3( 0, 0, hc );
	GNormal			= Normal[2];
	GPosition		= FragPos[2];
	GTexCoords		= TexCoords[2];
	GColors			= Colors[2];
	gl_Position		= gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}