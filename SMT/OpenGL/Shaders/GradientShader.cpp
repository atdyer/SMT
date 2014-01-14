#include "GradientShader.h"


bool StopIsLessThan(const QGradientStop &stop1, const QGradientStop &stop2)
{
	return stop1.first < stop2.first;
}


/**
 * @brief Constructor that defines the shader source code and default color and height values
 */
GradientShader::GradientShader()
{
	vertexSource =	"#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform int stopCount;"
			"uniform float values[10];"
			"uniform vec4 colors[10];"
			"void main(void)"
			"{"
			"	ex_Color = colors[0];"
			"	for (int i=1; i<min(stopCount, 10); ++i)"
			"	{"
			"		ex_Color = mix(ex_Color, colors[i], smoothstep(values[i-1], values[i], in_Position.z));"
			"	}"
			"	gl_Position = MVPMatrix*in_Position;"
			"}";

	fragSource =	"#version 330"
			"\n"
			"in vec4 ex_Color;"
			"out vec4 out_Color;"
			"void main(void)"
			"{"
			"	out_Color = ex_Color;"
			"}";

	lowValue = 0.0;
	highValue = 1.0;

	CompileShader();
	UpdateUniforms();
}


void GradientShader::SetGradientStops(const QGradientStops &newStops)
{
	gradientStops = newStops;
	UpdateUniforms();
}


void GradientShader::SetGradientRange(float newLow, float newHigh)
{
	lowValue = newLow;
	highValue = newHigh;
	UpdateUniforms();
}


QGradientStops GradientShader::GetGradientStops()
{
	return gradientStops;
}


ShaderType GradientShader::GetShaderType()
{
	return GradientShaderType;
}


/**
 * @brief Compiles the shader parts and assembles them into a usable shader on the OpenGL context
 */
void GradientShader::CompileShader()
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
 * This function updates the MVP matrix as well as color and height range values used in
 * drawing operations.
 *
 */
void GradientShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint StopCountUniform = glGetUniformLocation(programID, "stopCount");
		GLint ValuesUniform = glGetUniformLocation(programID, "values");
		GLint ColorsUniform = glGetUniformLocation(programID, "colors");

		GLint stopCount = gradientStops.size();

		if (stopCount > 1)
		{

			qSort(gradientStops.begin(), gradientStops.end(), StopIsLessThan);
			GLfloat stopValues[stopCount];
			GLfloat colorValues[stopCount*4];
			for (int i=0; i<gradientStops.size(); ++i)
			{
				stopValues[i] = GetValueFromStop(gradientStops[i].first);
				colorValues[4*i+0] = gradientStops[i].second.red() / 255.0;
				colorValues[4*i+1] = gradientStops[i].second.green() / 255.0;
				colorValues[4*i+2] = gradientStops[i].second.blue() / 255.0;
				colorValues[4*i+3] = gradientStops[i].second.alpha() / 255.0;
			}

			glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
			glUniform1i(StopCountUniform, stopCount);
			glUniform1fv(ValuesUniform, stopCount, stopValues);
			glUniform4fv(ColorsUniform, stopCount, colorValues);
		}


		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("GradientShader OpenGL Error: " << errString);
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


float GradientShader::GetValueFromStop(float stop)
{
	return lowValue + ((1.0-stop) * (highValue - lowValue));
}
