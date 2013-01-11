// simple toon fragment shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;
varying float col;

void main()
{
	//float intensity;
	//vec3 n;
	//vec4 color;

	//n = normalize(normal);
	//intensity = max(dot(lightDir,n),0.0); 
	
	
	//color = vec4(intensity*gl_Color[0], intensity*gl_Color[1], intensity*gl_Color[2] + 0.05, 1.0);	
	
		
	gl_FragColor = gl_Color;
}
