#pragma once
#include <GL/glew.h>
#include <vector>

class GT_Skybox
{
    public:
        GT_Skybox();
        ~GT_Skybox();

        GLuint skyboxVAO;

    private:
        void loadCubeMaps();
        void createSkyboxVAO();

        GLuint skyboxTexID;
        std::vector<const char*> cubemaps;
};
