uniform sampler2D texture_main;
uniform sampler2D texture_refractive;

void main()
{
	const float refractive_strength = 0.015;
	
    vec4 refractive = texture2D(texture_refractive, gl_TexCoord[0].xy);
	refractive.xyz = normalize(refractive.xyz * 2.0 - 1.0);
	vec2 offset = refractive.xy * refractive_strength ;
    vec4 diffuse = texture2D(texture_main, gl_TexCoord[0].xy + offset);

    gl_FragColor = diffuse.rgba;
}
