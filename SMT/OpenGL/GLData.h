#ifndef GLDATA_H
#define GLDATA_H

#include "glew.h"


/**
 * @brief A 4x4 matrix of floating point values
 */
typedef struct {
		float m[16];
} Matrix;


/**
 * @brief The value of Pi accurate to 20 decimal points
 */
static const double PI = 3.14159265358979323846;


/**
 * @brief The 4x4 identity matrix
 */
static const Matrix IDENTITY_MATRIX = {{
					       1.0, 0.0, 0.0, 0.0,
					       0.0, 1.0, 0.0, 0.0,
					       0.0, 0.0, 1.0, 0.0,
					       0.0, 0.0, 0.0, 1.0
				       }};

#endif // GLDATA_H
