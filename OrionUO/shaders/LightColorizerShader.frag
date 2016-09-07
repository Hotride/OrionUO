uniform sampler2D usedTexture;
uniform int drawMode;
uniform float colors[96];

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0 && drawMode == 1)
	{
		int index = int(textureColor.r * 7.96875) * 3;

		gl_FragColor = (textureColor * vec4(colors[index], colors[index + 1], colors[index + 2], 1.0)) * 3.0;
	}
	else
		gl_FragColor = textureColor;
}
