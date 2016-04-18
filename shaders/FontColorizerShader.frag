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

	v.r = float(AND32(c / 1024));
	v.g = float(AND32(c / 32));
	v.b = float(AND32(c));

	return (v / mod32);
}

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0)
	{
		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))
		{
			int index = AND32(int(textureColor.r * 31.875));

			vec3 resultColor = Color16To32(colors[index]);

			gl_FragColor = vec4(resultColor, textureColor.a) * gl_Color;
		}
		else if (drawMode == 4 || (drawMode == 3 && textureColor.r > 0.04))
		{
			vec3 resultColor = Color16To32(colors[30]);

			gl_FragColor = vec4(resultColor, textureColor.a) * gl_Color;
		}
		else
			gl_FragColor = textureColor * gl_Color;
	}
	else
		gl_FragColor = textureColor * gl_Color;
}
