#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

// Phong related variables
uniform sampler2D uSampler;
uniform vec3 uKd;
uniform vec3 uKs;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;
// uniform vec3 uLightIntensity;
uniform float uLightIntensity;
uniform int uTextureSample;

varying highp vec2 vTextureCoord;
varying highp vec3 vFragPos;
varying highp vec3 vNormal;

// Shadow map related variables
#define NUM_SAMPLES 120
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES 120 // 由于当时变量复制错了 这里是检测搜索blocker的采样个数!!!
#define NUM_RINGS 10
#define W_LIGHT 50
#define NEAR 1


#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

uniform sampler2D uShadowMap;

varying vec4 vPositionFromLight;

highp float rand_1to1(highp float x ) { 
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv ) { 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

void uniformDiskSamples( const in vec2 randomSeed ) {

  float randNum = rand_2to1(randomSeed);
  float sampleX = rand_1to1( randNum ) ;
  float sampleY = rand_1to1( sampleX ) ;

  float angle = sampleX * PI2;
  float radius = sqrt(sampleY);

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

    sampleX = rand_1to1( sampleY ) ;
    sampleY = rand_1to1( sampleX ) ;

    angle = sampleX * PI2;
    radius = sqrt(sampleY);
  }
}

vec2 poissonDisk_blocker[PCF_NUM_SAMPLES];

void poissonDiskSamples_blocker( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( PCF_NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( PCF_NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < PCF_NUM_SAMPLES; i ++ ) {
    poissonDisk_blocker[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}




float findBlocker( sampler2D shadowMap,  vec2 uv, float zReceiver ) {
  // 结果变量与个数计量变量
  float result = 0.0;
  int nBlocker = 0;
  float shadowRadius = float(W_LIGHT)*(zReceiver-float(NEAR)/1000.0)/zReceiver;
  // shadowRadius = 16.0;


  // 圆盘采样
  const vec2 seed = vec2(2.0,2.0);
  poissonDiskSamples_blocker(seed);

  for (int i=0; i<PCF_NUM_SAMPLES; i++){
    vec2 coord = vec2(uv.x + poissonDisk_blocker[i].x*float(shadowRadius)*(1.0/2048.0), uv.y + poissonDisk_blocker[i].y*float(shadowRadius)*(1.0/2048.0));
    vec4 rgbaDepth = texture2D(uShadowMap, coord);
    float shadowDepth = unpack(rgbaDepth);
    if (zReceiver + 2.0*EPS > shadowDepth){  
      // 此时是障碍物
      result += shadowDepth; // 深度都是0 - 1
      nBlocker++;
    } else {
      // 无任何操作
    }
  }

  // 防止result出现0
  // if (result == 0.0){
  //   return 2.0*EPS;
  // }

	return result/float(nBlocker);
}

float PCF_mySelf(sampler2D shadowMap, vec4 shadowCoord) {
  vec2 shadowTexCoord = shadowCoord.xy;
  shadowTexCoord = shadowTexCoord * 0.5;
  shadowTexCoord = shadowTexCoord + 0.5;

  // 定义滤波范围 这里我准备均匀遍历率 5x5 接近课程里的随机采样个数
  const int n = 35;
  const int nLimit = int(float(n)/2.0);
  float result = 0.0;
  for (int i=-nLimit; i<nLimit; i++){
    for (int j=-nLimit; j<nLimit; j++){
      // nxn次循环
      vec2 coord = vec2(shadowTexCoord.x + float(i)*(1.0/2048.0), shadowTexCoord.y + float(j)*(1.0/2048.0));
      vec4 rgbaDepth = texture2D(uShadowMap, coord);
      float shadowDepth = unpack(rgbaDepth);
      if ((shadowCoord.z * 0.5 + 0.5 - 4.0 * EPS) > shadowDepth){
        // 此时不累计
      } else {
        result += 1.0;
      }
    }
  }

  return result/float(n*n);
}

float PCF_classUse(sampler2D shadowMap, vec4 shadowCoord) {
  vec2 shadowTexCoord = shadowCoord.xy;
  shadowTexCoord = shadowTexCoord * 0.5;
  shadowTexCoord = shadowTexCoord + 0.5;
  
  // 滤波范围 半径
  const int n = 35;

  // 结果
  float result = 0.0;

  // 圆盘采样
  const vec2 seed = vec2(1.0,1.0);
  poissonDiskSamples(seed);

  for (int i=0; i<NUM_SAMPLES; i++){
    vec2 coord = vec2(shadowTexCoord.x + poissonDisk[i].x*float(n)*(1.0/2048.0), shadowTexCoord.y + poissonDisk[i].y*float(n)*(1.0/2048.0));
    vec4 rgbaDepth = texture2D(uShadowMap, coord);
    float shadowDepth = unpack(rgbaDepth);
    if ((shadowCoord.z * 0.5 + 0.5 - 4.0 * EPS) > shadowDepth){
      // 此时不累计
    } else {
      result += 1.0;
    }
  }


  return result/float(NUM_SAMPLES);
}

float PCSS(sampler2D shadowMap, vec4 shadowCoord){

  // 前期处理
  vec2 shadowTexCoord = shadowCoord.xy;
  shadowTexCoord = shadowTexCoord * 0.5;
  shadowTexCoord = shadowTexCoord + 0.5;
  float zReceiver = shadowCoord.z * 0.5 + 0.5;

  // STEP 1: avgblocker depth
  float depthAveBlocker = findBlocker(uShadowMap, shadowTexCoord, zReceiver);

  // STEP 2: penumbra size
  float filterRedius = float(W_LIGHT)*(zReceiver-depthAveBlocker)/depthAveBlocker;
  // 重点 防止出现0 或是太小
  // if (filterRedius < 7.0){
  //   filterRedius = 7.0;
  // }
  // filterRedius = 35.0;

  // STEP 3: filtering

  // 结果
  float result = 0.0;

  const vec2 seed = vec2(1.0,1.0);
  poissonDiskSamples(seed);

  for (int i=0; i<NUM_SAMPLES; i++){
    vec2 coord = vec2(shadowTexCoord.x + poissonDisk[i].x*float(filterRedius)*(1.0/2048.0), shadowTexCoord.y + poissonDisk[i].y*float(filterRedius)*(1.0/2048.0));
    vec4 rgbaDepth = texture2D(uShadowMap, coord);
    float shadowDepth = unpack(rgbaDepth);
    if ((shadowCoord.z * 0.5 + 0.5 - 4.0 * EPS) > shadowDepth){
      // 此时不累计
    } else {
      result += 1.0;
    }
  }

  return result/float(NUM_SAMPLES);

}


float useShadowMap(sampler2D shadowMap, vec4 shadowCoord){
  // 获取shadow map上对应点的深度信息 与当前点深度进行比较
  vec2 shadowTexCoord = shadowCoord.xy;
  shadowTexCoord = shadowTexCoord * 0.5;
  shadowTexCoord = shadowTexCoord + 0.5;
  // shadowTexCoord = vec2(shadowTexCoord.x + 0.5,-(shadowTexCoord.y - 0.5));
  vec4 rgbaDepth = texture2D(uShadowMap, shadowTexCoord);
  float shadowDepth = unpack(rgbaDepth);
  if ((shadowCoord.z * 0.5 + 0.5 - 4.0 * EPS) > shadowDepth){
    return 0.0;
  }
  return 1.0;
}

vec3 blinnPhong() {
  // vec3 color = texture2D(uSampler, vTextureCoord).rgb;
  vec3 color;
  if (uTextureSample == 1) {
    color = pow(texture2D(uSampler, vTextureCoord).rgb, vec3(2.2));
  } else {
    color = uKd;
  }
  // color = pow(color, vec3(2.2));

  vec3 ambient = 0.05 * color;
  
  // vec3 lightDir = normalize(uLightPos);
  vec3 lightDir = normalize(uLightPos - vFragPos);
  vec3 normal = normalize(vNormal);
  float diff = max(dot(lightDir, normal), 0.0);
  // vec3 light_atten_coff = uLightIntensity / pow(length(uLightPos - vFragPos), 2.0);
  float light_atten_coff = uLightIntensity / length(uLightPos - vFragPos);
  vec3 diffuse = diff * light_atten_coff * color;

  vec3 viewDir = normalize(uCameraPos - vFragPos);
  vec3 halfDir = normalize((lightDir + viewDir));
  float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
  vec3 specular = uKs * light_atten_coff * spec;

  vec3 radiance = (ambient + diffuse + specular);
  vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
  
  return phongColor;
}

void main(void) {

  float visibility;
  vec3 shadowCoord = (vPositionFromLight/vPositionFromLight.w).xyz;

  //visibility = useShadowMap(uShadowMap, vec4(shadowCoord, 1.0));
   visibility = PCF_mySelf(uShadowMap, vec4(shadowCoord, 1.0));
  // visibility = PCF_classUse(uShadowMap, vec4(shadowCoord, 1.0));
  // visibility = PCSS(uShadowMap, vec4(shadowCoord, 1.0));

  vec3 phongColor = blinnPhong();

  gl_FragColor = vec4(phongColor * visibility, 1.0);
  //gl_FragColor = vec4(phongColor, 1.0);
  //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
  
}