#ifndef GRADIENTSHADER_H
#define GRADIENTSHADER_H

#include <string>
#include "GLShader.h"
#include <QGradient>


/**
 * @brief A two color gradient shader
 *
 * This shader draws using a gradient. The user sets low and high height values
 * as well as the colors corresponding to those values, and colors are interpolated
 * linearly within that height range. Height values below the low value are drawn
 * using the low color and height values above the high value are drawn using the
 * high value.
 *
 */
class GradientShader : public GLShader
{
	public:

		// Constructor
		GradientShader();

		// Modification Functions
		void	SetGradientStops(const QGradientStops &newStops);
		void	SetGradientRange(float lowValue, float newHigh);

		// Query Functions
		QGradientStops	GetGradientStops();
		ShaderType	GetShaderType();

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Shader Properties
		QGradientStops	gradientStops;
		float		lowValue;
		float		highValue;

		// Override virtual functions
		void	CompileShader();
		void	UpdateUniforms();

	private:

		float	GetValueFromStop(float stop);

};

#endif // GRADIENTSHADER_H
