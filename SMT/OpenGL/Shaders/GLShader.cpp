#include "GLShader.h"

// Initialize static members
unsigned int	GLShader::shaderCount = 0;
unsigned int	GLShader::nextID = 1;


/**
 * @brief Constructor for the GLShader class
 *
 * Sets default values, increments the shaderCount and assings the new shader
 * a unique shaderID
 *
 */
GLShader::GLShader()
{
	programID = 0;
	camera = 0;
	loaded = false;
	camSet = false;
	uniformsSet = false;

	shaderCount++;
	shaderID = nextID;
	nextID++;
}


/**
 * @brief Deconstructor that tells the OpenGL context to delete the shader program
 */
GLShader::~GLShader()
{
	shaderCount--;
	if (loaded)
	{
		glUseProgram(0);
		glDeleteProgram(programID);
	}
}


/**
 * @brief Call this function to use this shader on subsequent glDraw*() operations
 *
 * This function ensures that the shader program is loaded and that uniform values have
 * been set, and it then tells the OpenGL context to use the shader for subsequent draw
 * operations.
 *
 */
bool GLShader::Use()
{
//	UpdateUniforms();
	UpdateCameraUniform();
	if (uniformsSet)
	{
		glUseProgram(programID);
		return true;
	}
	return false;
}


/**
 * @brief Set the camera to be used during glDraw*() operations
 *
 * This function essentially tells the shader the values of the MVP matrix to use
 * when drawing points to the screen.
 *
 * @param newCamera Pointer to the GLCamera object to be used for drawing operations
 */
void GLShader::SetCamera(GLCamera *newCamera)
{
	if (newCamera != 0)
	{
		camera = newCamera;
		camSet = true;
		DEBUG("Camera set for shader " << shaderID);
		UpdateUniforms();
	} else {
		DEBUG("Error setting camera for shader " << shaderID << "  ---  " << newCamera);
	}
}


/**
 * @brief Returns the unique ID associated with the Layer object
 * @return The unique ID associated with the Layer object
 */
unsigned int GLShader::GetID()
{
	return shaderID;
}


/**
 * @brief Returns the total number of shaders that currently exist
 * @return The number of GLShader objects that currently exist
 */
unsigned int GLShader::GetNumShaders()
{
	return shaderCount;
}


void GLShader::UpdateCameraUniform()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);
		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
	}
}


/**
 * @brief Helper function that is used to compile parts of a shader program
 *
 * Because different shaders may use any different number of programs (eg. vertex,
 * fragment, geometry, tesellation, etc.) this function provides an easy way to
 * compile each of these parts individually before combining them into a shader program
 * that can be used for drawing.
 *
 * Subclasses must override the CompileShader virtual function and call this function as
 * needed in order to build the complete shader.
 *
 * @param source The source code of the shader part
 * @param shaderType The shader type (GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
 * GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER)
 * @return The reference value of the shader part in the OpenGL context
 */
GLuint GLShader::CompileShaderPart(const char *source, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	if (shaderID != 0)
	{
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);

		GLint compileResult;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			DEBUG("Shader Compile Error: " << compileResult);

			GLint logSize;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
			char log[logSize];
			glGetShaderInfoLog(shaderID, 250, NULL, log);

			DEBUG("Shader Log: " << log);

			return 0;
		} else {
			return shaderID;
		}
	} else {
		DEBUG("Error creating shader");
		return 0;
	}
}
