#include "Window.h"
/*, const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locX, float locY, const Object::Vertex *uv, const char* imgname*/
/*, vsrc, fsrc, vertexcount, vertex, indexcount, index, locX, locY, uv, imgname*/
Window::Window(int width, int height, const char* title, GLint size, GLenum param, float sscale = 0.0f)
    : Game(&window, size)
    , window(glfwCreateWindow(width, height, title, NULL, NULL))
    {
    if(window == NULL)
    {
        std::cerr<<"window is NULL"<<std::endl;
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr<<"Can't Load GLAD"<<std::endl;
        exit(1);
    }
    
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, resize);

    glfwSetKeyCallback(window, keyBoard);

    glfwSetMouseButtonCallback(window, button);

    resize(window, width, height);

    location[0] = 0.0f;
    location[1] = 0.0f;

    if(param != SPECIFY)
    {
        if(height < width)
            scale = height;
        else
            scale = width;
    }
    else{
        if(sscale != 0.0f)
            scale = sscale;
    }

    map.setStart(location, 1, 1, this->size, scale);
    }

Window::~Window()
{
    glfwDestroyWindow(window);
}