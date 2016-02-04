varying vec3 l;
varying vec3 n;

uniform int drawMode;

void main(void)
{
	if (drawMode > 5)
	{
		l = normalize(vec3(gl_LightSource[0].position));
		n = normalize(gl_NormalMatrix * gl_Normal);
	}

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
