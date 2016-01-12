uniform sampler2D usedTexture;

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	gl_FragColor = vec4(textureColor.r, textureColor.r, textureColor.r, textureColor.a);
}
