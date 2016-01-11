uniform sampler2D u_texture1;

void main(void)
{
	vec4 texColor = texture2D(u_texture1, gl_TexCoord[0].xy, 0);

	gl_FragColor = vec4(texColor.r, texColor.r, texColor.r, texColor.a);
}
