uniform sampler2D usedTexture;
uniform int drawMode;
uniform int colors[32];

int AND32(int val)
{
	while (val > 31)
		val -= 32;

	return val;
}

vec3 Color16To32(int c)
{
	const vec3 mod32 = vec3(31.0, 31.0, 31.0);

	vec3 v;

	v.r = AND32(c / 1024.0);
	v.g = AND32(c / 32.0);
	v.b = AND32(c);

	return (v / mod32);
}

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0)
	{
		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))
		{
			int index = AND32(textureColor.r * 31.875);

			vec3 resultColor = Color16To32(colors[index]);

			gl_FragColor = vec4(resultColor, textureColor.a);
		}
		else if (drawMode == 3)
		{
			int index = AND32(textureColor.r * 7.96875);

			vec3 resultColor = Color16To32(colors[index]);

			gl_FragColor = vec4(resultColor, textureColor.a);
		}
		else
			gl_FragColor = textureColor;
	}
	else
		gl_FragColor = textureColor;
}
