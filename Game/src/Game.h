#ifndef _GAME_H_
#define _GAME_H_

#include "GDK/Input.h"
#include "Screen.h"
#include "Renderer/Shape.h"
#include "SEL/Player.h"
#include "SEL/Intersect.h"
#include "Gobj/Enemy.h"
#include "Gobj/Map.h"
#include "Gobj/Aster.h"

//#include "SEL/Intersect_4trees.h"

class Game
{
    const GLint size;

    std::vector<Screen> screen;

    std::vector<MapRect> mazeRect;

    static const GLuint mazeIndex[6];

    std::vector<uint32_t> strIndex;

    std::vector<Texture> str_texture;

    Astar astar;

    int lastLocation[2];

protected:

    GLint screenIndex;

    std::unique_ptr<Enemy> enemy;

    Map map;

    playerDrection direction;

    std::vector<IntersectTile<Structure>> intersect;

    std::unique_ptr<Player> player;

    std::shared_ptr<Input> input;

    GLfloat location[2];

    GLboolean moveStat;

    std::vector<Structure> mask;

public:

    std::vector<Shader> shader;
    //  constractor
    //  window : pointer to created window
    //  size : Frame Work size
    Game(GLFWwindow* const* window, const GLint size);

    virtual ~Game();

    void InitPlayer(const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const Object::Vertex *uv, const char* imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    void InitMask();
    
    void InitEnemy(const float size[2], float scale, const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float location[2], float positionld[2], float positionru[2], const Object::Vertex *uv, const char* imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    void searchPlayerPath(std::vector<Points> &point, const float playerLoc[2], float size[2], float scale);

    bool GameOver(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const;

    void UpdatePlayerPath(const float playerLoc[2], float size[2], float scale);

    void convertMapStr(GLuint offset, const float size[2], float scale);

    void InitMapRect(Object::Vertex vertex[4], Object::Vertex uv[4]) const;

    void createScreen(const char* name, GLfloat insecPos[2], GLfloat insecSize[2]);

    void setTexture(const char* imgname);

    void setShader(const char* vsrc, const char* fsrc, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    void excuteShader(GLsizei shaderIndex, const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const;

    void setUI(GLint offset, const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locx, float locy, const char* imgname, const Object::Vertex *uv, const void (*UIfunc)(int*, float[2], float), const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    void setStructure(GLint offset, GLsizei vertexcount, const Object::Vertex *vertex, GLfloat location[2], const char* imgname, const Object::Vertex* uv, GLsizei indexcount, const GLuint *index, const float size[2], float scale);

    void drawTile(GLint locationLoc, const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const;

    void actvtex(GLsizei strnum, int8_t handle) const;

    void darwUI(GLsizei num, float size[2], float scale, int loc[5]) const;

    void moveEnemy(const float size[2], float scale, const float playerLoc[2]);

    void drawEnemy(const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const;

    void drawMask(const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const;

    void excuteUI(GLsizei num, float mospos[2], float size[2], float scale);

    virtual void KeyInputProcess(float size[2], float scale);

    virtual void MouseInputProcess(float size[2], float scale);

    const float *(getUILoc(size_t num) const) { return screen[screenIndex].ui[num].getPos(); }

    [[nodiscard]] const float *(getStructureLoc(size_t num) const) { return screen[screenIndex].structure[num].getStrLocation(); }

    [[nodiscard]] bool getStrIntersect(size_t num, float size[2], float scale) const
    {
        constexpr Object::Vertex d{0.0f, 0.0f };
        return screen[screenIndex].structure[num].InterSect(location, &d, size, scale);
    }
};

#endif // _GAME_H_