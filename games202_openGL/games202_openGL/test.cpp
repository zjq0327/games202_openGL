#include "application/Application.h"
#include "renders/sceneRender.h"
#include "glframework/pipeline.h"
#include "camera/camera.h"
#include "cameraControl/gameCameraControl.h"
#include "application/Application.h"


using namespace std;

Camera* camera = nullptr;

GameCameraControl* cameraControl = nullptr;

void OnResize(int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        myapp->setWindowShouldClose();

    cameraControl->onKey(key, action, mods);
}

//鼠标按下/抬起
void OnMouse(int button, int action, int mods) {
    double x, y;
    myapp->getCursorPosition(&x, &y);
    cameraControl->onMouse(button, action, x, y);
}

//鼠标移动
void OnCursor(double xpos, double ypos) {
    cameraControl->onCursor(xpos, ypos);
}

//鼠标滚轮
void OnScroll(double offset) {
    cameraControl->onScroll(offset);
}


Pipeline getLightPipeline(string lightVShader_path, string lightFShader_path)
{
    Shader lightVShader(lightVShader_path, 0);
    vector<string> light_attriNamesV = {
        "aVertexPosition"
    };
    vector<string> light_unifromNamesV = {
        "uModelViewMatrix",
        "uProjectionMatrix"
    };
    lightVShader.setAttributeName(light_attriNamesV);
    lightVShader.setUniformName(light_unifromNamesV);

    Shader lightFShader(lightFShader_path, 1);
    vector<string> light_uniformNamesF = {
        "uLigIntensity",
        "uLightColor"
    };
    lightFShader.setUniformName(light_uniformNamesF);

    // link program
    Pipeline lightPipe(lightVShader, lightFShader);
    return lightPipe;
}

Pipeline getPhongPipeline(string phongVShader_path, string phongFShader_path)
{
    // Phong pipe
    Shader phongVShader(phongVShader_path, 0);
    vector<string> phong_attriNamesV = {
        "aVertexPosition",
        "aNormalPosition",
        "aTextureCoord"
    };
    vector<string> phong_uniformNamesV = {
        "uModelViewMatrix",
        "uProjectionMatrix"
    };
    phongVShader.setAttributeName(phong_attriNamesV);
    phongVShader.setUniformName(phong_uniformNamesV);

    Shader phongFShader(phongFShader_path, 1);
    vector<string> phong_unifromNamesF = {
        "uKd",
        "uKs",
        "uLightPos",
        "uCameraPos",
        "uLightIntensity",
        "uTextureSample"
    };
    vector<string> phong_textureNamesF = {
        "uSampler"
    };
    phongFShader.setUniformName(phong_unifromNamesF);
    phongFShader.setTextureName(phong_textureNamesF);

    // link program
    Pipeline phongPipe(phongVShader, phongFShader);
    return phongPipe;
}


int main()
{
    myapp->init();

    myapp->setResizeCallback(OnResize);
    myapp->setKeyBoardCallback(OnKey);
    myapp->setMouseCallback(OnMouse);
    myapp->setCursorCallback(OnCursor);
    myapp->setScrollCallback(OnScroll);
    // 主程序流程

    string lightVShader_path = "assets/shaders/work1/lightshader/lightCubeVertexShader.glsl";
    string lightFShader_path = "assets/shaders/work1/lightshader/lightCubeFragment.glsl";

    string phongVShader_path = "assets/shaders/work1/phongShader/phongVertex.glsl";
    string phongFShader_path = "assets/shaders/work1/phongShader/phongFragment.glsl";

    string shadowVShader_path = "assets/shaders/work1/shadowShader/shadowVertex.glsl";
    string shadowFShader_path = "assets/shaders/work1/shadowShader/shadowFragment.glsl";

    string model_path = "assets/models/mary/Marry.obj";
    string floor_path = "assets/models/floor/floor.obj";


    // 一些作业一的参数
    glm::vec3 lightPos(0.f, 80.f, 80.f);
    // glm::vec3 lightPos(6.5f, 40.f, 101.f);
    glm::vec3 focalPoint(0.f, 0.f, 0.f);
    glm::vec3 lightUp(0.f, 1.f, 0.f);
    // glm::vec3 cameraPos(30.f, 30.f, 30.f);
    glm::vec3 cameraPos(6.5f, 80.f, 101.f);


       // light pipe
    Pipeline lightPipe = getLightPipeline(lightVShader_path, lightFShader_path);
    // Phong pipe
    Pipeline phongPipe = getPhongPipeline(phongVShader_path, phongFShader_path);
    // shadow pipe
    Pipeline shadowPipe = getPhongPipeline(shadowVShader_path, shadowFShader_path);

    SceneRender scene;
    scene.addModel(model_path); // id 0
    scene.addModel(model_path); // id 1
    scene.addModel(floor_path); // id 2

    scene.setObjectModelMatrix(0, glm::vec3(0), 0, glm::vec3(0, 0, 1), glm::vec3(20, 20, 20));
    scene.setObjectModelMatrix(1, glm::vec3(40, 0, -40), 0, glm::vec3(0, 0, 1), glm::vec3(10, 10, 10));
    scene.setObjectModelMatrix(2, glm::vec3(0, 0, -30), 0, glm::vec3(0, 0, 1), glm::vec3(4, 4, 4));

    //scene.addLight(250, lightPos, glm::vec3(1.f, 1.f, 1.f));

    scene.addLight(100, lightPos, glm::vec3(1.f, 1.f, 1.f));
    scene.lights[0].setTargetAndUp(focalPoint, lightUp);

    camera = new Camera(cameraPos, focalPoint, lightUp);

    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);

    scene.addCamera(camera);



    unsigned int lightPipe_id = scene.addPipeline(lightPipe);
    unsigned int phongPipe_id = scene.addPipeline(phongPipe);
    unsigned int shadowPipe_id = scene.addPipeline(shadowPipe);

    unsigned int n = 0;

    // 创建帧缓冲
    scene.addFrameBuffer(2048, 2048);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (myapp->update())
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // 设置一下场景内全局MVP
        // 让光源动起来
        /*float camX = sin(glfwGetTime() * 0.6 * 2) * 100;
        float camY = cos(glfwGetTime() * 0.4 * 2) * 150;
        float camZ = cos(glfwGetTime() * 0.2 * 2) * 100;*/
        scene.setModelMatrixLight(scene.getModelMatrix(lightPos, (float)glfwGetTime(), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)));
        scene.setModelMatrixModel(scene.getModelMatrix(glm::vec3(0, 0, 0), 0, glm::vec3(0, 0, 1), glm::vec3(52, 52, 52)));
        scene.setViewMatrix(scene.getViewMatrix(0));
        scene.setProjectionMatrix(scene.getProjectionMatrix(75, myapp->getWidth() / myapp->getHeight(), 0.1, 1000));

        // 更新光源位置
        //scene.lights[0].setPosition(glm::vec3(camX, camY, camZ));

        cameraControl->update();

        // 绘制
        scene.draw_1(lightPipe_id, phongPipe_id, shadowPipe_id);
    }

    glfwTerminate();
    return 0;
}

