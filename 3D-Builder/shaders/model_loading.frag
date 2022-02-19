#version 440 core
out vec4 FragColor;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D texture_specular1;
	sampler2D texture_diffuse1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
};

// �߿����
struct LineInfo {
	float width;
	vec4 color;
};

in vec2 GTexCoords;
in vec3 GPosition;
in vec3 GNormal;
in vec4 GColors;

uniform Light light;
uniform Material material;
uniform LineInfo Line;
uniform vec3 viewPos;
uniform bool isWireFrame;
uniform vec3 customColors;
uniform float alpha;
uniform float reflectivity;
uniform samplerCube skybox;

noperspective in vec3 GEdgeDistance;

vec4 ordinaryRender(vec3 Position, vec3 Normal, vec4 Colors)
{
	// ������
	vec3 ambient = light.ambient * Colors.rgb;
	
	// ������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - Position);
	float diff = max(dot(norm, lightDir) , 0.0);
	vec3 diffuse = light.diffuse * diff * Colors.rgb;
	
	// ���淴��
	vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * Colors.rgb;  
	
	vec3 result = ambient + diffuse + specular;
	vec4 normal_result = vec4(result, Colors.a);

	vec3 I = -viewDir;
	vec3 R = reflect(I, normalize(Normal));
	vec4 reflect_result = vec4(texture(skybox, R).rgb, Colors.a);

	return mix(normal_result, reflect_result, reflectivity);

	// ���淴�价��Ч��
	// vec3 I = -viewDir;
	// vec3 R = reflect(I, norm);
	// FragColor = vec4(texture(skybox, R));
	// ��ͨ��������ɫЧ��
    // FragColor = vec4(result, 1.0) * Colors;

	// float ambientStrength = 1;
    // FragColor = texture(material.texture_diffuse1, TexCoords);
}

void main()
{   
	vec4 color = ordinaryRender(GPosition, GNormal, vec4(customColors, alpha));
	// �ҳ��������Ƭ������ıߵľ���
	float d = min(GEdgeDistance.x, GEdgeDistance.y);
	d = min(d, GEdgeDistance.z);
	float mixVal = smoothstep(Line.width - 1.0f, Line.width + 1.0f, d);
	if (isWireFrame == true) {
		FragColor = mix(Line.color, color, mixVal);
	} else {
		FragColor = color;
	}
	// FragColor = color;
}