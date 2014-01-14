#ifndef SOLIDSHADER_H
#define SOLIDSHADER_H

#include <string>
#include <QColor>
#include "GLShader.h"


/**
 * @brief A single color shader
 *
 * This shader draws everything the same solid color.
 *
 */
class SolidShader : public GLShader
{
	public:

		// Constructor
		SolidShader();

		// Modification Functions
		void	SetColor(QColor newColor);

		// Query Functions
		QColor		GetShaderProperties();
		ShaderType	GetShaderType();

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Shader Properties
		QColor	color;

		// Override virtual functions
		virtual void	CompileShader();
		virtual void	UpdateUniforms();
};

#endif // SOLIDSHADER_H
