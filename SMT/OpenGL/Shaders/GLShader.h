#ifndef GLSHADER_H
#define GLSHADER_H

#include "OpenGL/GLCamera.h"


enum ShaderType {NoShaderType, SolidShaderType, GradientShaderType};



/**
 * @brief A virtual class that provides the functionality needed to use
 * a GLSL shader to draw on the OpenGL context
 *
 * This class provides a wrapper that allows us to use any type of shader
 * before performing a drawing operation. It is up to the programmer to
 * ensure the correct shaders are used when drawing different Layers by
 * using a LayerManager.
 *
 * All memory management on the OpenGL context is taken care of by this
 * virtual class. Subclasses only need to successfully compile the shader
 * program and set the programID variable.
 *
 */
class GLShader
{
	public:

		// Constructor/Destructor
		GLShader();
		~GLShader();

		// Function Definitions
		bool	Use();
		void	SetCamera(GLCamera *newCamera);

		// Getter Methods
		unsigned int		GetID();
		static unsigned int	GetNumShaders();
		virtual ShaderType	GetShaderType() = 0;

	protected:

		// Variable Definitions
		GLuint		programID;
		GLCamera	*camera;

		// Flags
		bool	loaded;
		bool	camSet;
		bool	uniformsSet;

		void		UpdateCameraUniform();
		GLuint		CompileShaderPart(const char *source, GLenum shaderType);

		/**
		 * @brief Compiles the full shader program
		 *
		 * This function, defined in a subclass of GLShader, compiles all parts of the shader
		 * program for use in the OpenGL context. Upon completion of compiling/linking, the
		 * programID value should be set.
		 *
		 */
		virtual void	CompileShader() = 0;

		/**
		 * @brief Updates the shader uniforms in the OpenGL context
		 *
		 * This function, defined in a subclass of GLShader, transfers all appropriate values
		 * from the shader object to the shader program in the OpenGL context using the
		 * glGetUniformLocation() and glUniform*() functions.
		 *
		 */
		virtual void	UpdateUniforms() = 0;


	private:

		static unsigned int	shaderCount;	/**< A running count of the number of Shader objects */
		static unsigned int	nextID;		/**< The next available shaderID */
		unsigned int		shaderID;	/**< A unique unsigned integer that identifies this Layer */

};

#endif // GLSHADER_H
