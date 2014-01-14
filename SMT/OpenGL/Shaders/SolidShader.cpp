#include "SolidShader.h"


/**
 * @brief Constructor that defines the source code and default color values
 *
 * Constructor that defines the source code and default color values
 *
 */
SolidShader::SolidShader()
{
	vertexSource =  "#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform vec4 ColorVector;"
			"void main(void)"
			"{"
			"       gl_Position = MVPMatrix*in_Position;"
			"       ex_Color = ColorVector;"
			"}";

	fragSource =	"#version 330"
			"\n"
			"in vec4 ex_Color;"
			"out vec4 out_Color;"
			"void main(void)"
			"{"
			"	out_Color = ex_Color;"
			"}";

	color = QColor(1.0, 1.0, 1.0, 1.0);

	CompileShader();
	UpdateUniforms();
}


/**
 * @brief Set the color to be used in glDraw*() operations
 *
 * Pass a negative value to any individual color value to keep it the same
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @param a Alpha value
 */
void SolidShader::SetColor(QColor newColor)
{
	color = newColor;
	UpdateUniforms();
}


/**
 * @brief Retrieves the shader's properties
 *
 * Retrieves the shader's properties (ie. color).
 *
 * @return The shader's properties
 */
QColor SolidShader::GetShaderProperties()
{
	return color;
}


ShaderType SolidShader::GetShaderType()
{
	return SolidShaderType;
}


/**
 * @brief Compiles the shader parts and assembles them into a usable shader on the OpenGL context
 *
 * Compiles the shader parts and assembles them into a usable shader on the OpenGL context
 *
 */
void SolidShader::CompileShader()
{
	const char* fullVertSource = vertexSource.data();
	const char* fullFragSource = fragSource.data();

	GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);

	if (vertexShaderID && fragmentShaderID)
	{
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		loaded = true;
	}
}


/**
 * @brief Updates values used for drawing
 *
 * This function updates the MVP matrix as well as the color used in drawing operations.
 *
 */
void SolidShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint ColorUniform = glGetUniformLocation(programID, "ColorVector");

		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
			DEBUG("Error getting uniform locations");

		GLfloat currColor[4] = {color.red() / 255.0,
					color.green() / 255.0,
					color.blue() / 255.0,
					color.alpha() / 255.0};

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(ColorUniform, 1, currColor);

		errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("SolidShader OpenGL Error: " << errString);
			uniformsSet = false;
		} else {
			uniformsSet = true;
		}

	} else {
		if (!loaded)
			DEBUG("Uniforms not updated: Shader not loaded");
		else
			DEBUG("Uniforms not updated: Camera not set");
		uniformsSet = false;
	}
}
