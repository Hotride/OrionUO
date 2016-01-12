uniform sampler2D usedTexture;

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	float red = textureColor.r * 0.85;

	gl_FragColor = vec4(red, red, red, textureColor.a);
}
