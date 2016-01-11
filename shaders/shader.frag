uniform sampler2D u_texture1;

void main(void)
{
	vec4 texColor = texture2D(u_texture1, gl_TexCoord[0].st);

	gl_FragColor = vec4(texColor.r, texColor.g, texColor.b, 1.0);
}
