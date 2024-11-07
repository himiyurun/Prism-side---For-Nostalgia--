#include <cstdlib>
#include "src/GDK/Window.h"
#include "src/GDK/Shader.h"
//#include "src/Renderer/Shape.h"
//#include "src\SEL\Structure.h"

const void startButton(int *handle, float size[2], float scale)
{
    switch(*handle)
    {
        case 1:
        *handle = 0;
        break;

        case 0:
        *handle = 1;
        break;
    }
}

const void setshaderValue0(const float size[2], float scale, const float location[2], int texIndex, float Light, const int loc[5])
{
    glUniform2fv(loc[0], 1, size);
    glUniform1f(loc[1], scale);
    glUniform2fv(loc[2], 1, location);
}

const void setshaderValue1(const float size[2], float scale, const float location[2], int texIndex, float Light, const int loc[5])
{
    glUniform2fv(loc[0], 1, size);
    glUniform1f(loc[1], scale);
    glUniform2fv(loc[2], 1, location);
    
    if(texIndex >= 0)
        glUniform1i(loc[3], texIndex);
    if(Light >= 0.f)
        glUniform1f(loc[4], Light);
}

constexpr Object::Vertex playerVertex[] = {
    {  0.4f,  0.4f },
    { -0.4f,  0.4f },
    { -0.4f, -0.4f },
    {  0.4f, -0.4f }
};

constexpr Object::Vertex rectangleVertex[] = {
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
    { -0.5f, -0.5f },
    {  0.5f, -0.5f }
};

constexpr Object::Vertex ui[] = {
    { -10.0f,  2.5f },
    { -10.0f, -2.5f },
    {  10.0f, -2.5f },
    {  10.0f,  2.5f }
};

constexpr GLuint index[] = {
    1, 0, 3, 1, 2, 3
};

constexpr Object::Vertex uv[] = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f }
};

constexpr Object::Vertex uiuv[] = {
    { 0.0f, 1.0f },
    { 0.0f, 0.75f },
    { 1.0f, 0.75f },
    { 1.0f, 1.0f }
};

constexpr Object::Vertex str_uv[] = {
    { 1.0f / 8.0f * 1, 1.0f / 8.0f * 1 },
    { 0.0f, 1.0f / 8.0f * 1 },
    { 0.0f, 0.0f },
    { 1.0f / 8.0f * 1, 0.0f }
};


constexpr Object::Vertex recttest[] = {
    {  0.5f,  0.5f },
    { -0.5f,  0.5f },
    { -0.5f, -0.5f },
    {  0.5f, -0.5f }
};

GLfloat position[]  = {  0.5f,  0.5f };
GLfloat position2[] = { -0.5f, -0.5f };
GLfloat position3[] = {  1.0f,  1.0f };
GLfloat position4[] = { -1.0f,  1.0f };
GLfloat position5[] = {  0.8f,  0.1f };
GLfloat position6[] = { -0.4f, -0.9f };

GLfloat IntersectPosition[2] = { 0.0f, 0.0f };
GLfloat IntersectSize[2] = { 2.0f, 2.0f };

GLfloat positionld[2] = { -0.5f, -0.5f };
GLfloat positionru[2] = {  0.5f,  0.5f };

int main() {

    srand(static_cast<unsigned int>(time(nullptr)));
    
    //  initlize GLFW
    if(!glfwInit()) {
        std::cerr<<"Can't Initlize GLFW"<<std::endl;
        return 1;
    }
    //  set exit process
    atexit(glfwTerminate);

    //  choose opengl version and settings
    //  set window parameter
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Window window(1920, 1020, "Game", 2, SPECIFY, 100.0f);

    std::unique_ptr<const Shader> test(new Shader("src/shader/test.vert", "src/shader/test.frag", setshaderValue1));
    //std::unique_ptr<const Texture> texture(new Texture(2, 4, uv, "src/enemypre.bmp"));

    //  create screenObj
    window.createScreen("Main_Menu", IntersectPosition, IntersectSize);
    window.createScreen("Game_Play", IntersectPosition, IntersectSize);

    //  set Player param
    window.InitPlayer("src/shader/shader.vert", "src/shader/shader.frag", 4, playerVertex, 6, index, uv, "src/player.bmp", setshaderValue1);

    //  set Enemy param
    window.InitEnemy(window.getSize(), window.getScale(), "src/shader/shader.vert", "src/shader/shader.frag", 4, rectangleVertex, 6, index, position2, positionld, positionru, uv, "src/enemypre.bmp", setshaderValue1);

    //  set UIobj
    window.setUI(0, "src/shader/shader.vert", "src/shader/shader.frag", 4, ui, 6, index, 0.0f, 0.0f, "src/gameui.bmp", uiuv, startButton, setshaderValue1);
    window.setUI(1, "src/shader/shader.vert", "src/shader/shader.frag", 4, ui, 6, index, 0.0f, 0.0f, "src/gameui.bmp", uiuv, startButton, setshaderValue1);

    //  set Structure Shader
    window.setShader("src/shader/shader.vert", "src/shader/shader.frag", setshaderValue0);

    //  set Texture
    window.setTexture("src/gameobject.bmp");

    /*
    //  set Structure
    window.setStructure(1, 4, recttest, position, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(1, 4, recttest, position3, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(1, 4, recttest, position4, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(1, 4, recttest, position5, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(0, 4, recttest, position2, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(0, 4, recttest, position3, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(0, 4, recttest, position4, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    window.setStructure(0, 4, recttest, position6, "src/objpre.bmp", str_uv, 6, index, window.getSize(), window.getScale());
    */
   //g++  -Iinclude  -o For_Nostalgia_demo  main.cpp glad.cpp src/GDK/Window.cpp src/GDK/Shader.cpp src/Renderer/Object.cpp src/Game.cpp src/SEL/Player.cpp src/SEL/Structure.cpp src/Renderer/Texture.cpp src/UI.cpp src/SEL/Intersect.cpp src/Gobj/Enemy.cpp src/Gobj/Map.cpp -Llib -lglfw3 -lgdi32 -lshell32 -lopengl32 -mwindows

    window.convertMapStr(1, window.getSize(), window.getScale());

    window.InitMask();

    const GLint sizeLoc(test->getUnfLoc("size"));
    const GLint locationLoc(test->getUnfLoc("location"));
    const GLint scaleLoc(test->getUnfLoc("scale"));

    GLint Loc[5]{ test->getUnfLoc("size"), test->getUnfLoc("scale"), test->getUnfLoc("location"), -1, -1 };

    while(window)
    {
        //system("cls");
        window.clearColor();

        window.drawEnemy(window.getSize(), window.getScale(), window.getUILoc(0), 0, 0.0f, Loc);

        window.drawTile(locationLoc, window.getSize(), window.getScale(), nullptr, 0, 0.0f, Loc);

        window.drawMask(window.getSize(), window.getScale(), nullptr, 0, 0.0f, Loc);

        window.darwUI(0, const_cast<float*>(window.getSize()), window.getScale(), Loc);

        window.drawPlayer();

        window.swapBuffers();

        window.moveEnemy(window.getSize(), window.getScale(), nullptr);

        window.UpdatePlayerPath(nullptr, const_cast<float*>(window.getSize()), window.getScale());
    }
}