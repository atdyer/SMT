#ifndef CULLEDSOLIDSHADER_H
#define CULLEDSOLIDSHADER_H

#include <string>
#include "GLShader.h"
#include "SolidShader.h"


/**
 * @brief A subclass of SolidShader that has culling capabilities.
 *
 * This subclass of SolidShader has culling capabilities that are
 * achieved using a geometry shader. When a primitive (triangle)
 * is passed through the geometry shader, the shader determines
 * the average distance between the three corners, and if that
 * distance is smaller than some set value, the primitive is not
 * drawn. This provides for increased responsiveness in very
 * large domains.
 *
 */
class CulledSolidShader : public SolidShader
{
	public:

		// Constructor
		CulledSolidShader();
		virtual ShaderType	GetShaderType();

	protected:

		// Geometry Shader Source
		std::string	geoSource;

		// Override Virtual Functions
		virtual void	CompileShader();
		virtual void	UpdateUniforms();
};

#endif // CULLEDSOLIDSHADER_H
