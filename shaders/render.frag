#version 450 core
#pragma debug(on)
#pragma optimize(off)

layout(binding=0) uniform sampler2D output_image;

out vec4 fragColor;

void main(){
	fragColor = texture(output_image, vec2(gl_FragCoord.xy)/ vec2(textureSize(output_image, 0)));
}