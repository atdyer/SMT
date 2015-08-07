#ifndef MAXELELAYER_H
#define MAXELELAYER_H

#include <QString>

#include "Project/Files/Fort14.h"
#include "Project/Files/Maxele63.h"

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/GradientShader.h"
#include "OpenGL/Shaders/CulledSolidShader.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <QThread>


/*
this class is used to read and display maxele data from maxele.63 file
aa15
*/

class MaxeleLayer
{
public:

    MaxeleLayer();
    ~MaxeleLayer();

    void	Draw();
    void	SetCamera(GLCamera *newCamera);
    void	SetFort14(Fort14 *newFort14);
    void    SetMaxele63(Maxele63 *newMaxele63);

    std::string     GetMaxele63Location();
    float GetMinMaxele();
    float GetMaxMaxele();


protected:

    std::string maxele63location;

private:

    Fort14* fort14;
    Maxele63* maxele63;
    unsigned int ReadMaxeleData(unsigned int nodeCount, std::ifstream* fileStream);

    // Rendering camera
    GLCamera*	camera;

    // Rendering shaders
    SolidShader*	outlineShader;
    SolidShader*	fillShader;

    // OpenGL rendering surface
    GLuint	VAOId;
    GLuint	VBOId;
    GLuint	IBOId;

public slots:

    void readMaxele63();



};

#endif // MAXELELAYER_H
