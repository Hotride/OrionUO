varying vec3 l;
varying vec3 n;

uniform sampler2D usedTexture;
uniform int drawMode;
uniform float colors[96];

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0)
	{
		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))
		{
			int index = int(textureColor.r * 31.875) * 3;

			gl_FragColor = vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a); // * gl_Color;
		}
		else if (drawMode > 5)
		{
			if (drawMode > 9)
			{
				float red = textureColor.r;

				if (drawMode > 10)
					red *= 0.5;
				else
					red *= 1.5;

				gl_FragColor = vec4(red, red, red, textureColor.a) * gl_Color;
			}
			else
			{
				vec3 n2 = normalize(n);
				vec3 l2 = normalize(l);

				vec4 normal = vec4(max(dot(n2, l2) + 0.5, 0.0));

				if (drawMode > 6)
				{
					int index = int(textureColor.r * 31.875) * 3;

					gl_FragColor = (vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a) * gl_Color) * normal;
				}
				else
					gl_FragColor = (textureColor * gl_Color) * normal;
			}
		}
		else
			gl_FragColor = textureColor * gl_Color;
	}
	else
		gl_FragColor = textureColor * gl_Color;
}
