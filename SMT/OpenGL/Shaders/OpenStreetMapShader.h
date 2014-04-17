#ifndef OPENSTREETMAPSHADER_H
#define OPENSTREETMAPSHADER_H

#include <OpenGL/Shaders/GLShader.h>

class OpenStreetMapShader : public GLShader
{
	public:
		OpenStreetMapShader();
		~OpenStreetMapShader();

	protected:

		std::string	vertexSource;
		std::string	fragSource;

		void	CompileShader();
		void	UpdateUniforms();
		void	UpdateCameraUniform();

		ShaderType	GetShaderType();
};

#endif // OPENSTREETMAPSHADER_H
