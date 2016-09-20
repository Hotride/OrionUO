varying vec3 l;
varying vec3 n;

uniform sampler2D usedTexture;
uniform int drawMode;

void main(void)
{
	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0)
	{
		if (drawMode > 5)
		{
			vec3 n2 = normalize(n);
			vec3 l2 = normalize(l);

			vec4 normal = vec4(max(dot(n2, l2) + 0.5, 0.0));

			textureColor = (textureColor * gl_Color) * normal;
		}

		float red = (0.6 * textureColor.r + 0.05 * textureColor.g);

		textureColor = vec4(red, red, red, textureColor.a);
	}

	gl_FragColor = textureColor;
}
