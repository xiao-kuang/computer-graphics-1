#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;


uniform vec3 lightPos[];
uniform vec3 viewPos;
uniform vec3 lightColor[];
uniform vec3 objectColor;

vec3 get_result(vec3 Normal, vec3 FragPos, int i){
    // ambient
    vec3 ambient = 0.1 * lightColor[i];
    // diffuse
    vec3 diffuse = max(dot(normalize(Normal), normalize(lightPos[i] - FragPos)), 0.0) * lightColor[i];
    // specular
    vec3 specular = 0.5 * pow(max(dot(normalize(viewPos - FragPos), reflect(-normalize(lightPos[i] - FragPos), normalize(Normal))), 0.0),64) * lightColor[i];
    return ambient + diffuse + specular;
}

void main()
{
    vec3 result ;
    result += get_result(Normal, FragPos, 0) * texture(texture1, TexCoord).xyz;
    result += get_result(Normal, FragPos, 1) * texture(texture1, TexCoord).xyz;
//to be added
    FragColor = vec4(result, 1.0) ;
    //FragColor = texture(texture1, TexCoord);
}