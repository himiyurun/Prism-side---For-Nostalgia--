#ifndef SCREEN_H
#define SCREEN_H

#include <vector>

#include "GDK/Shader.h"
#include "SEL/Structure.h"
#include "UI.h"
#include "Loader.h"

class Screen
{
    const char* name;

public:

    std::vector<Structure> structure;

    std::vector<Shader> shader;

    std::vector<UI> ui;

    std::shared_ptr<Loader> loader;

    Screen(const char* name)
        : name(name)
        , loader(new Loader(0))
    {

    }

    const char (*getName() const) { return name; }
};

#endif // SCREEN_H