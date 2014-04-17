#include "OpenStreetMapShader.h"

OpenStreetMapShader::OpenStreetMapShader()
{
	vertexSource = "#version 330\n"
		       "layout (location=0) in vec4 in_Position;"
		       "layout (location=1) in vec2 vTexCoords;"
		       "out vec2 vVaryingTexCoords;"
		       "uniform mat4 MVPMatrix;"
		       "void main() {"
		       "    vVaryingTexCoords = vTexCoords;"
		       "    gl_Position = MVPMatrix*in_Position;"
		       "}";

	fragSource = "#version 330\n"
		     "uniform sampler2D colorMap;"
		     "in vec2 vVaryingTexCoords;"
		     "out vec4 vFragColor;"
		     "void main() {"
		     "    vFragColor = texture(colorMap, vVaryingTexCoords.st);"
		     "}";

	CompileShader();
	UpdateUniforms();
}


OpenStreetMapShader::~OpenStreetMapShader()
{

}


void OpenStreetMapShader::CompileShader()
{
	const char* fullVertSource = vertexSource.data();
	const char* fullFragSource = fragSource.data();

	GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);

	if (vertexShaderID && fragmentShaderID)
	{
		programID = glCreateProgram();
		glBindAttribLocation(programID, 0, "in_Position");
		glBindAttribLocation(programID, 1, "vTexCoords");
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		loaded = true;
	}
}


void OpenStreetMapShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint TexUniform = glGetUniformLocation(programID, "colorMap");

		std::cout << TexUniform << std::endl;

		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
			DEBUG("Error getting uniform locations");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform1i(TexUniform, 0);

		errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("OpenStreetMap OpenGL Error: " << errString);
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


void OpenStreetMapShader::UpdateCameraUniform()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint TexUniform = glGetUniformLocation(programID, "colorMap");

		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
			DEBUG("Error getting uniform locations");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform1i(TexUniform, 0);

		errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("OpenStreetMap OpenGL Error: " << errString);
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


ShaderType OpenStreetMapShader::GetShaderType()
{
	return SolidShaderType;
}
