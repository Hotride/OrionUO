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

			gl_FragColor = vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a) * gl_Color;
		}
		else if (drawMode == 4 || (drawMode == 3 && textureColor.r > 0.04))
		{
			gl_FragColor = vec4(colors[90], colors[91], colors[92], textureColor.a) * gl_Color;
		}
		else
			gl_FragColor = textureColor * gl_Color;
	}
	else
		gl_FragColor = textureColor * gl_Color;
}
