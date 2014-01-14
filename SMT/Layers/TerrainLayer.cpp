#include "TerrainLayer.h"

TerrainLayer::TerrainLayer()
{
	fort14Location = "";
	numNodes = 0;
	numElements = 0;
	minX = 99999.0;
	midX = 0.0;
	maxX = -99999.0;
	minY = 99999.0;
	midY = 0.0;
	maxY = -99999.0;
	minZ = 99999.0;
	maxZ = -99999.0;
	max = 1.0;

	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	outlineShader = 0;
	fillShader = 0;
	boundaryShader = 0;

	flipZValue = true;
	fileLoaded = false;
	glLoaded = false;
	largeDomain = false;

	quadtree = 0;
	drawQuadtreeOutline = false;
	numVisibleElements = 0;
	viewingDepth = 5;

	useCulledShaders = false;
	solidOutline = 0;
	solidFill = 0;
	solidBoundary = 0;
	gradientOutline = 0;
	gradientFill = 0;
	gradientBoundary = 0;

	connect(this, SIGNAL(fort14Valid()), this, SLOT(readFort14()));
}


TerrainLayer::~TerrainLayer()
{

	DEBUG("Deleting Terrain Layer. Layer ID: " << GetID());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (solidOutline)
		delete solidOutline;
	if (solidFill)
		delete solidFill;
	if (solidBoundary)
		delete solidBoundary;
	if (gradientOutline)
		delete gradientOutline;
	if (gradientFill)
		delete gradientFill;
	if (gradientBoundary)
		delete gradientBoundary;

	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	if (quadtree)
		delete quadtree;
}


/**
 * @brief Attempts to draw as much of the layer's data as possible
 *
 * This function attempts to draw the layer's data using each of the defined shaders
 *
 */
void TerrainLayer::Draw()
{
	if (fileLoaded && glLoaded)
	{
		glBindVertexArray(VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				if (largeDomain)
					glDrawElements(GL_TRIANGLES, numVisibleElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
				else
					glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				if (largeDomain)
					glDrawElements(GL_TRIANGLES, numVisibleElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
				else
					glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (!largeDomain && boundaryShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0);
			if (boundaryShader->Use())
				glDrawElements(GL_LINE_STRIP, boundaryNodes.size(), GL_UNSIGNED_INT, (GLvoid*)(0 + sizeof(GLuint)*numElements*3));
			glLineWidth(1.0);
		}

		if (drawQuadtreeOutline && quadtree)
		{
			quadtree->DrawOutlines();
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


/**
 * @brief Sends data to the GPU for use in drawing operations
 *
 * This function is used to send the data that is read from the fort.14 file to the GPU
 * for use in drawing the layer. We make use of a Vertex Array Object, which keeps track of
 * OpenGL state, as well as Vertex Buffer Objects and Index Buffer Objects, which are
 * used for drawing large amounts of data very quickly.
 *
 */
void TerrainLayer::LoadDataToGPU()
{
	if (fileLoaded)
	{

		/* First check if we should be using culled shaders */
//		if (largeDomain)
//			SwitchToCulledShaders();

		/* Send the data to the GPU */
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*numNodes;

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &VBOId);
		glGenBuffers(1, &IBOId);

		glBindVertexArray(VAOId);

		// Send Vertex Data
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
		GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			for (unsigned int i=0; i<numNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)nodes[i].normX;
				glNodeData[4*i+1] = (GLfloat)nodes[i].normY;
				glNodeData[4*i+2] = (GLfloat)nodes[i].z;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
		} else {
			glLoaded = false;
			emit emitMessage("<p style:color='red'><strong>Error: Unable to load vertex data to GPU</strong>");
			return;
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			DEBUG("ERROR: Unmapping vertex buffer for TerrainLayer " << GetID());
			return;
		}

		// Send Index Data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		if (largeDomain)
		{
			UpdateVisibleElements();
		} else {
			const size_t IndexBufferSize = 3*sizeof(GLuint)*numElements + sizeof(GLuint)*boundaryNodes.size();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
			GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glElementData)
			{
				for (unsigned int i=0; i<numElements; i++)
				{
					glElementData[3*i+0] = (GLuint)elements[i].n1->nodeNumber-1;
					glElementData[3*i+1] = (GLuint)elements[i].n2->nodeNumber-1;
					glElementData[3*i+2] = (GLuint)elements[i].n3->nodeNumber-1;
				}
				for (unsigned int i=0; i<boundaryNodes.size(); i++)
				{
					glElementData[3*numElements+i] = boundaryNodes[i]-1;
				}
			} else {
				glLoaded = false;
				emit emitMessage("<p style:color='red'><strong>Error: Unable to load index data to GPU</strong>");
				return;
			}

			if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
			{
				glLoaded = false;
				DEBUG("ERROR: Unmapping index buffer for TerrainLayer " << GetID());
				return;
			}

			glBindVertexArray(0);
		}

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
				emit finishedLoadingToGPU();
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


void TerrainLayer::SetData(QString fileLocation)
{
	std::ifstream testFileValid (fileLocation.toStdString().data());
	if (testFileValid.good())
	{
		fort14Location = fileLocation.toStdString();
		emit fort14Valid();
	} else {
		emit emitMessage("<p style='color:red'><strong>Error:</strong> fort.14 file not found.</p>");
	}
}


bool TerrainLayer::DataLoaded()
{
	return fileLoaded;
}


/**
 * @brief Getter method that returns the fort.14 location
 * @return The fort.14 absolute path
 */
std::string TerrainLayer::GetFort14Location()
{
	return fort14Location;
}


/**
 * @brief Returns a pointer to the Node with the corresponding node number
 *
 * This functions provides access to Nodes in the node list through node number.
 * The Node is first looked up on the assumption that the Node list is ordered,
 * but if the list is not ordered, a linear lookup on the Node list is performed
 * until the correct Node is found.
 *
 * @param nodeNumber The node number as defined in the fort.14 file
 * @return A pointer to the Node with the corresponding node number
 * @return 0 if the Node is not in the node list
 */
Node* TerrainLayer::GetNode(unsigned int nodeNumber)
{
	if (nodes.size() > 0 && nodeNumber <= nodes.size() && nodeNumber == nodes[nodeNumber-1].nodeNumber)
		return &nodes[nodeNumber-1];
	else
		for (unsigned int i=0; i<nodes.size(); i++)
			if (nodes[i].nodeNumber == nodeNumber)
				return &nodes[i];
	return 0;
}


/**
 * @brief Returns a pointer to the Node closest to the provided x-y coordinates
 *
 * This function provides access to Nodes in the node list by finding the Node
 * with the x-y coordinates closest to the provided x-y coordinates. A quick
 * lookup is possible by parsing the Quadtree used to map the Node data.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Node closest to the provided x-y coordinates
 * @return 0 if the click was outside the bounds of the quadtree or if there is no data loaded
 */
Node* TerrainLayer::GetNode(float x, float y)
{
	if (quadtree)
		return quadtree->FindNode(x, y);
	return 0;
}


/**
 * @brief Returns a list of Nodes that fall within the given circle
 *
 * This function provides access to the Quadtree lookup function that finds all
 * nodes within a defined circle.
 *
 * @param x The circle center x-coordinate
 * @param y The circle center y-coordinate
 * @param radius The circle radius
 * @return A list of Nodes that fall within the circle
 */
std::vector<Node*> TerrainLayer::GetNodesFromCircle(float x, float y, float radius)
{
	if (quadtree)
		return quadtree->FindNodesInCircle(x, y, radius);
	else
	{
		std::vector<Node*> fail;
		return fail;
	}
}


/**
 * @brief Returns a pointer to the Element with the corresponding element number
 *
 * This function provides access to Elements in the element list through element
 * number. The element is first looked up on the assumption that the Element
 * list is ordered, but if the list is not ordered, a linear lookup on the
 * Element list is performed until the correct Element is found.
 *
 * @param elementNumber The element number as defined in the fort.14 file
 * @return A pointer to the Element with the corresponding element number
 * @return 0 if the Element is not in the element list
 */
Element* TerrainLayer::GetElement(unsigned int elementNumber)
{
	if (elements.size() > 0 && elementNumber <= elements.size() && elementNumber == elements[elementNumber-1].elementNumber)
		return &elements[elementNumber-1];
	else
		for (unsigned int i=0; i<elements.size(); i++)
			if (elements[i].elementNumber == elementNumber)
				return &elements[i];
	return 0;
}


/**
 * @brief Returns a pointer to the Element closest to the provided x-y coordinates
 *
 * This function provides access to Elements in the element list by finding the Element
 * closest to the provided x-y coordinates. If the x-y coordinates are inside of an
 * Element, that Element will be returned. If the coordinates do not fall inside of
 * any Element in the list, 0 is returned.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Element that contains the provided x-y coordinates
 * @return 0 if the element list is empty or if the point is not inside of any Element
 */
Element* TerrainLayer::GetElement(float x, float y)
{
	if (quadtree)
	{
//		if (camera)
//		{
//			float newX, newY;
//			camera->GetUnprojectedPoint(x, y, &newX, &newY);
			return quadtree->FindElement(x, y);
//		}
	}
	return 0;
}


std::vector<Element>* TerrainLayer::GetAllElements()
{
	return &elements;
}


std::vector<Element*> TerrainLayer::GetElementsFromCircle(float x, float y, float radius)
{
	if (quadtree)
	{
		return quadtree->FindElementsInCircle(x, y, radius);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


std::vector<Element*> TerrainLayer::GetElementsFromRectangle(float l, float r, float b, float t)
{
	if (quadtree)
	{
		return quadtree->FindElementsInRectangle(l, r, b, t);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


std::vector<Element*> TerrainLayer::GetElementsFromPolygon(std::vector<Point> polyLine)
{
	if (quadtree)
	{
		return quadtree->FindElementsInPolygon(polyLine);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


/**
 * @brief Get the number of nodes
 * @return The number of nodes
 */
unsigned int TerrainLayer::GetNumNodes()
{
	return numNodes;
}


/**
 * @brief Get the number of elements
 * @return The number of elements
 */
unsigned int TerrainLayer::GetNumElements()
{
	return numElements;
}


/**
 * @brief Get the minimum x-coordinate
 * @return The minimum x-coordinate
 */
float TerrainLayer::GetMinX()
{
	return minX;
}


/**
 * @brief Get the maximum x-coordinate
 * @return The maximum x-coordinate
 */
float TerrainLayer::GetMaxX()
{
	return maxX;
}


/**
 * @brief Get the minimum y-coordinate
 * @return The minimum y-coordinate
 */
float TerrainLayer::GetMinY()
{
	return minY;
}


/**
 * @brief Get the maximum y-coordinate
 * @return The maximum y-coordinate
 */
float TerrainLayer::GetMaxY()
{
	return maxY;
}


/**
 * @brief Get the minimum z-coordinate
 * @return The minimum z-coordinate
 */
float TerrainLayer::GetMinZ()
{
	return minZ;
}


/**
 * @brief Get the maximum z-coordinate
 * @return The maximum z-coordinate
 */
float TerrainLayer::GetMaxZ()
{
	return maxZ;
}


/**
 * @brief Given an x-coordinate in OpenGL space, this function returns the denormalized value
 * of that coordinate.
 *
 * Drawing in OpenGL space is done around the origin. This is achieved by normalizing all
 * coordinates defined in the fort.14 file. This function essentially does the opposite, by
 * converting the coordinate back into the coordinate system from the fort.14 file.
 *
 * @param x The coordinate in OpenGL space
 * @return The coordinate in the domain's coordinate system
 */
float TerrainLayer::GetUnprojectedX(float x)
{
	return x*max + midX;
}


/**
 * @brief Given a y-coordinate in OpenGL space, this funtion returns the denormalized value
 * of that coordinate.
 *
 * Drawing in OpenGL space is done around the origin. This is achieved by normalizing all
 * coordinates defined in the fort.14 file. This function essentially does the opposite, by
 * converting the coordinate back into the coordinate system from the fort.14 file.
 *
 * @param y The coordinate in OpenGL space
 * @return The coordinate in the domain's coordinate system
 */
float TerrainLayer::GetUnprojectedY(float y)
{
	return y*max + midY;
}


ShaderType TerrainLayer::GetOutlineShaderType()
{
	if (outlineShader)
		return outlineShader->GetShaderType();
	return NoShaderType;
}


ShaderType TerrainLayer::GetFillShaderType()
{
	if (fillShader)
		return fillShader->GetShaderType();
	return NoShaderType;
}


/**
 * @brief Returns the properties being used to draw a solid outline
 *
 * Returns the properties being used to draw a solid outline
 *
 * @return The properties being used to draw a solid outline
 */
QColor TerrainLayer::GetSolidOutline()
{
	if (solidOutline)
		return solidOutline->GetShaderProperties();
	return QColor();
}


/**
 * @brief Returns the properties being used to draw a solid fill
 *
 * Returns the properties being used to draw a solid fill
 *
 * @return The properties being used to draw a solid fill
 */
QColor TerrainLayer::GetSolidFill()
{
	if (solidFill)
		return solidFill->GetShaderProperties();
	return QColor();
}


/**
 * @brief Returns the properties being used to draw a solid boundary segment
 *
 * Returns the properties being used to draw a solid boundary segment
 *
 * @return The properties being used to draw a solid boundary segment
 */
QColor TerrainLayer::GetSolidBoundary()
{
	if (solidBoundary)
		return solidBoundary->GetShaderProperties();
	return QColor();
}


/**
 * @brief Returns the properties being used to draw a gradient outline
 *
 * Returns the properties being used to draw a gradient outline
 *
 * @return The properties being used to draw a gradient outline
 */
QGradientStops TerrainLayer::GetGradientOutline()
{
	if (gradientOutline)
		return gradientOutline->GetGradientStops();
	return QGradientStops();
}


/**
 * @brief Returns the properties being used to draw a gradient fill
 *
 * Returns the properties being used to draw a gradient fill
 *
 * @return The properties being used to draw a gradient fill
 */
QGradientStops TerrainLayer::GetGradientFill()
{
	if (gradientFill)
		return gradientFill->GetGradientStops();
	return QGradientStops();
}


/**
 * @brief Returns the properties being used to draw a gradient boundary segment
 *
 * Returns the properties being used to draw a gradient boundary segment
 *
 * @return The properties being used to draw a gradient boundary segment
 */
QGradientStops TerrainLayer::GetGradientBoundary()
{
	if (gradientBoundary)
		return gradientBoundary->GetGradientStops();
	return QGradientStops();
}


/**
 * @brief Returns the ID of the Vertex Buffer that contains Nodal data
 *
 * Returns the ID of the Vertex Buffer that contains Nodals data. This ID is
 * to be used only for the drawing of subsets of the data. Modifying the
 * data in this buffer will result in undefined behavior from the GPU.
 *
 * @return The Vertex Buffer ID
 */
GLuint TerrainLayer::GetVBOId()
{
	return VBOId;
}


/**
 * @brief Sets the GLCamera object to be used when drawing this Layer
 *
 * Sets the GLCamera object to be used when drawing this Layer. The Layer does
 * not take ownership of the GLCamera object.
 *
 * @param newCamera Pointer to the GLCamera object
 */
void TerrainLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;
	if (solidOutline)
		solidOutline->SetCamera(camera);
	if (solidFill)
		solidFill->SetCamera(camera);
	if (gradientOutline)
		gradientOutline->SetCamera(camera);
	if (gradientFill)
		gradientFill->SetCamera(camera);
	if (quadtree)
		quadtree->SetCamera(camera);
}


/**
 * @brief Sets the location of the fort.14 file for this layer and checks its validity
 *
 * This function will store the given location of the fort.14 file and then attempt to
 * check the validity of the path given. Once it has verified that the file is valid,
 * it emits the fort14Valid() signal.
 *
 * @param newLocation The absolute path of the fort.14 file
 */
void TerrainLayer::SetFort14Location(std::string newLocation)
{
	std::ifstream testFileValid (newLocation.data());
	if (testFileValid.good())
	{
		fort14Location = newLocation;
		emit fort14Valid();
	} else {
		emit emitMessage("<p style='color:red'><strong>Error:</strong> fort.14 file not found.</p>");
	}
}


/**
 * @brief Sets the solid color used for drawing this Layer's outline
 *
 * Sets the solid color used for drawing this Layer's outline. If the
 * terrain has over 1,000,000 elements, we choose to use a culled
 * solid shader to increase responsiveness. The shader is owned by
 * the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetSolidOutline(QColor newColor)
{
	if (!solidOutline)
	{
		if (useCulledShaders)
			solidOutline = new CulledSolidShader();
		else
			solidOutline = new SolidShader();
		solidOutline->SetCamera(camera);
	}

	solidOutline->SetColor(newColor);
	outlineShader = solidOutline;
}


/**
 * @brief Sets the solid color used for drawing this Layer's fill
 *
 * Sets the solid color used for drawing this Layer's fill. The shader
 * is owned by the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetSolidFill(QColor newColor)
{
	if (!solidFill)
	{
		if (useCulledShaders)
			solidFill = new CulledSolidShader();
		else
			solidFill = new SolidShader();
		solidFill->SetCamera(camera);
	}

	solidFill->SetColor(newColor);
	fillShader = solidFill;
}


/**
 * @brief Sets the solid color used for drawing this Layer's boundary segment
 *
 * Sets the solid color used for drawing this Layer's boundary segment. The shader
 * is owned by the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetSolidBoundary(QColor newColor)
{
	if (!solidBoundary)
		solidBoundary = new SolidShader();

	solidBoundary->SetCamera(camera);
	solidBoundary->SetColor(newColor);
	boundaryShader = solidBoundary;
}


/**
 * @brief Sets the gradient used for drawing this Layer's outline
 *
 * Sets the gradient used for drawing the Layer's outline. The shader
 * is owned by the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetGradientOutline(QGradientStops newStops)
{
	if (!gradientOutline)
	{
		gradientOutline = new GradientShader();
		gradientOutline->SetCamera(camera);
		gradientOutline->SetGradientRange(minZ, maxZ);
	}

	gradientOutline->SetGradientStops(newStops);

	outlineShader = gradientOutline;
}


/**
 * @brief Sets the gradient used for drawing this Layer's fill
 *
 * Sets the gradient used for drawing this Layer's fill. The shader
 * is owned by the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetGradientFill(QGradientStops newStops)
{
	if (!gradientFill)
	{
		gradientFill = new GradientShader();
		gradientFill->SetCamera(camera);
		gradientFill->SetGradientRange(minZ, maxZ);
	}

	gradientFill->SetGradientStops(newStops);

	fillShader = gradientFill;
}


/**
 * @brief Sets the gradient used for drawing this Layer's boundary segment
 *
 * Sets the gradient used for drawing this Layer's boundary segment. The shader
 * is owned by the TerrainLayer.
 *
 * @param newProperties The new shader properties
 */
void TerrainLayer::SetGradientBoundary(QGradientStops newStops)
{
	if (!gradientBoundary)
	{
		gradientBoundary = new GradientShader();
		gradientBoundary->SetCamera(camera);
		gradientBoundary->SetGradientRange(minZ, maxZ);
	}

	gradientBoundary->SetGradientStops(newStops);

	boundaryShader = gradientBoundary;
}


/**
 * @brief Sets whether or not the outline of the Quadtree will be displayed
 *
 * Sets whether or not the outline of the Quadtree will be displayed
 *
 * @param toggleValue true to display the outline, false to disable the outline
 */
void TerrainLayer::ToggleQuadtreeVisible()
{
	drawQuadtreeOutline = !drawQuadtreeOutline;
}


void TerrainLayer::UpdateZoomLevel(float zoomAmount)
{
	if (largeDomain && camera && quadtree)
	{
		/* Get the bounds of the viewport in domain space */
		float xTopLeft, yTopLeft, xBotRight, yBotRight;
		camera->GetUnprojectedPoint(0, 0, &xTopLeft, &yTopLeft);
		camera->GetUnprojectedPoint(camera->GetViewportWidth(), camera->GetViewportHeight(), &xBotRight, &yBotRight);
//		xTopLeft = GetUnprojectedX(xTopLeft);
//		yTopLeft = GetUnprojectedY(yTopLeft);
//		xBotRight = GetUnprojectedX(xBotRight);
//		yBotRight = GetUnprojectedY(yBotRight);

//		DEBUG(xTopLeft << " " << yTopLeft << " " << xBotRight << " " <<yBotRight);

//		if (zoomAmount > 0)
//			viewingDepth++;
//		else if (zoomAmount < 0 && viewingDepth != 0)
//			viewingDepth--;

		/* Get the visible elements from the quadtree */
		visibleElementLists = quadtree->GetElementsThroughDepth(viewingDepth, xTopLeft, xBotRight, yBotRight, yTopLeft);

		/* Update the elements on the OpenGL context */
		UpdateVisibleElements();
	}
}


/**
 * @brief Function used internally to make the switch over to culled shaders in the
 * event that the number of elements exceeds a certain limit.
 *
 * Function used internally to make the switch over to culled shaders in the
 * event that the number of elements exceeds a certain limit. It will only
 * replace shaders that have already been created with their culled equivalents.
 *
 */
void TerrainLayer::SwitchToCulledShaders()
{
	useCulledShaders = true;

	if (solidOutline)
	{
		QColor currentProperties = solidOutline->GetShaderProperties();
		delete solidOutline;
		solidOutline = 0;
		SetSolidOutline(currentProperties);
	}

	if (solidFill)
	{
		QColor currentProperties = solidFill->GetShaderProperties();
		delete solidFill;
		solidFill = 0;
		SetSolidFill(currentProperties);
	}

	DEBUG("Terrain Layer: Switched to culled shaders");

}


void TerrainLayer::UpdateGradientShadersRange()
{
	if (gradientOutline)
	{
		gradientOutline->SetGradientRange(minZ, maxZ);
	}
	if (gradientFill)
	{
		gradientFill->SetGradientRange(minZ, maxZ);
	}
	if (gradientBoundary)
	{
		gradientBoundary->SetGradientRange(minZ, maxZ);
	}
}


/**
 * @brief Function that determines if the domain is large enough to warrant extra GPU optimizations
 *
 * Function that determines if the domain is large enough to warrant extra GPU optimizations. If it
 * is large enough (possibly based on function that takes into account CPU speed, GPU speed, etc),
 * we'll use the Quadtree to do some special culling/drawing.
 *
 */
void TerrainLayer::CheckForLargeDomain()
{
	if (numElements > 500000)
		largeDomain = true;
}


void TerrainLayer::UpdateVisibleElements()
{
	if (fileLoaded && quadtree)
	{
//		visibleElementLists = quadtree->GetElementsThroughDepth(5);

		numVisibleElements = 0;
		for (unsigned int i=0; i<visibleElementLists.size(); i++)
			numVisibleElements += visibleElementLists[i]->size();
//		DEBUG("Number of visible elements: " << numVisibleElements);
		const size_t IndexBufferSize = 3*sizeof(GLuint)*numVisibleElements;

//		glBindVertexArray(VAOId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_DYNAMIC_DRAW);
		GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glElementData)
		{
			int count = 0;
			for (unsigned int i=0; i<visibleElementLists.size(); i++)
			{
				for (unsigned int j=0; j<visibleElementLists[i]->size(); j++)
				{
					glElementData[count++] = (GLuint)(*visibleElementLists[i])[j]->n1->nodeNumber-1;
					glElementData[count++] = (GLuint)(*visibleElementLists[i])[j]->n2->nodeNumber-1;
					glElementData[count++] = (GLuint)(*visibleElementLists[i])[j]->n3->nodeNumber-1;
				}
			}
		} else {
			glLoaded = false;
			emit emitMessage("<p style:color='red'><strong>Error: Unable to load index data to GPU</strong>");
			DEBUG("Error mapping to terrain element list");
			return;
		}

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			DEBUG("ERROR: Unmapping index buffer for TerrainLayer " << GetID());
			return;
		}

//		GLenum errorCheck = glGetError();
//		if (errorCheck == GL_NO_ERROR)
//		{
//			if (VAOId && VBOId && IBOId)
//			{
//				glLoaded = true;
//				emit finishedLoadingToGPU();
//			}
//		} else {
//			const GLubyte *errString = gluErrorString(errorCheck);
//			DEBUG("OpenGL Error: " << errString);
//			glLoaded = false;
//		}

//		glBindVertexArray(0);
	}
}


/**
 * @brief Reads the fort.14 file data
 *
 * This function is used to read data from the fort.14 file. It is implemented as
 * a slot so that it can be part of the signal/slot mechanism used for threading.
 * Typical threaded usage of this function:
 * - Create TerrainLayer object on heap (using new)
 * - Create new QThread
 * - Move the TerrainLayer object to the newly created QThread
 * - Set the fort.14 file when ready to get everything ready for drawing
 *
 */
void TerrainLayer::readFort14()
{
	std::ifstream fort14 (fort14Location.data());

	if (fort14.is_open())
	{
		emit startedReadingData();

		std::string line;
		std::getline(fort14, infoLine);
		std::getline(fort14, line);
		std::stringstream(line) >> numElements >> numNodes;

		emit foundNumNodes(numNodes);
		emit foundNumElements(numElements);

		/* Progress bar stuff */
		int currentProgress = 0;
		int totalProgress = CalculateTotalProgress(true, true, true, true, true);

		if (numNodes > 0 && numElements > 0)
		{
			nodes.reserve(numNodes);
			elements.reserve(numElements);

			/* Read all of the nodal data with progress bar enabled */
			currentProgress = ReadNodalData(numNodes, &fort14, currentProgress, totalProgress);


			/* Read all of the element data with progress bar enabled */
			currentProgress = ReadElementData(numElements, &fort14, currentProgress, totalProgress);


			/* Read all of the boundary data if this is a subdomain */
			currentProgress = ReadBoundaryNodes(&fort14, currentProgress, totalProgress);

			/* All data has been read from fort.14, so close it */
			fort14.close();
			fileLoaded = true;

			/* Calculate normalized coordinates */
			currentProgress = NormalizeCoordinates(currentProgress, totalProgress);


			// Organize the data in a quadtree
			if (!quadtree)
			{
				quadtree = new Quadtree(nodes, elements, 2, (minX-midX)/max, (maxX-midX)/max, (minY-midY)/max, (maxY-midY)/max);
				quadtree->SetCamera(camera);
			}

			CheckForLargeDomain();

			if (largeDomain)
				visibleElementLists = quadtree->GetElementsThroughDepth(viewingDepth);

			emit finishedReadingData();
			emit emitMessage(QString("Terrain layer created: <strong>").append(infoLine.data()).append("</strong>"));

			DEBUG("x-range:\t" << minX << "\t" << maxX);
			DEBUG("y-range:\t" << minY << "\t" << maxY);
			DEBUG("z-range:\t" << minZ << "\t" << maxZ);
		} else {
			fileLoaded = false;
			emit emitMessage("Error reading fort.14 file");
		}
	} else {
		fileLoaded = false;
		emit emitMessage("Error opening fort.14 file");
	}
}


/**
 * @brief Helper function that calculates the value used to represent a fully processed fort.14 file
 *
 * Helper function that calculates the value used to represent a fully processed fort.14 file. The value
 * starts as 0 and the total number of points that will be accumulated during each process is added
 * (eg. 1 per node when reading the nodes, so the total number of nodes is added).
 *
 * @param readNodes true if Nodes will be read, false otherwise
 * @param readElements true if Elements will be read, false otherwise
 * @param readBoundaries true if boundaries will be read, false otherwise
 * @param normalizeCoordinates true if normal coordinates will be calculated, false otherwise
 * @param createQuadtree true if a Quadtree will be created, false otherwise
 * @return The total progress value that represents a fully processed fort.14 file
 */
unsigned int TerrainLayer::CalculateTotalProgress(bool readNodes, bool readElements, bool readBoundaries, bool normalizeCoordinates, bool createQuadtree)
{
	unsigned int totalProgress = 0;
	if (readNodes)
		totalProgress += numNodes;
	if (readElements)
		totalProgress += numElements;
	if (readBoundaries)
		totalProgress += BOUNDARY_PROGRESS_VALUE;
	if (normalizeCoordinates)
		totalProgress += numNodes;
	if (createQuadtree)
		totalProgress += QUADTREE_PROGRESS_VALUE;
	return totalProgress;
}


/**
 * @brief Helper function that reads all Nodal data from a fort.14 file
 *
 * Helper function that reads all Nodal data from a fort.14 file. Compares Nodal coordinates
 * against current min/max values and modifies those values accordingly.
 *
 * File stream object must already be at the beginning of the nodal list in the fort.14 file.
 *
 * @param nodeCount The number of Nodes to read
 * @param fileStream File stream located at the beginning of the node list in fort.14
 * @return 0
 */
unsigned int TerrainLayer::ReadNodalData(unsigned int nodeCount, std::ifstream *fileStream)
{
	return ReadNodalData(nodeCount, fileStream, 0, 0);
}


/**
 * @brief Helper function that reads all Nodal data from a fort.14 file
 *
 * Helper function that reads all Nodal data from a fort.14 file. Compares Nodal coordinates
 * against current min/max values and modifies those values accordingly.
 *
 * File stream object must already be at the beginning of the nodal list in the fort.14 file.
 *
 * Emits the current fort.14 processing progress.
 *
 * @param nodeCount The number of Nodes to read
 * @param fileStream File stream located at the beginning of the node list in fort.14
 * @param currProgress The current progress in processing the fort.14 file
 * @param totalProgress The value that indicates all of the fort.14 file has processed
 * @return The progress in processing the fort.14 file after the nodal data has been read
 */
unsigned int TerrainLayer::ReadNodalData(unsigned int nodeCount, std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress)
{
	Node currNode;
	for (unsigned int i=0; i<nodeCount; i++)
	{
		*fileStream >> currNode.nodeNumber;
		*fileStream >> currNode.xDat;
		*fileStream >> currNode.yDat;
		*fileStream >> currNode.zDat;
		currNode.x = atof(currNode.xDat.data());
		currNode.y = atof(currNode.yDat.data());
		currNode.z = atof(currNode.zDat.data());
//		*fileStream >> currNode.x;
//		*fileStream >> currNode.y;
//		*fileStream >> currNode.z;

		// Check against min/max values and flip the z-value if needed
		if (currNode.x < minX)
			minX = currNode.x;
		else if (currNode.x > maxX)
			maxX = currNode.x;
		if (currNode.y < minY)
			minY = currNode.y;
		else if (currNode.y > maxY)
			maxY = currNode.y;
		if (flipZValue)
			currNode.z *= -1.0;
		if (currNode.z < minZ)
			minZ = currNode.z;
		else if (currNode.z > maxZ)
			maxZ = currNode.z;
		nodes.push_back(currNode);
		if (totalProgress)
			emit progress(100*(++currProgress)/totalProgress);
	}

	UpdateGradientShadersRange();

	return currProgress;
}


/**
 * @brief Helper function that reads all Element data from the fort.14 file
 *
 * Helper function that reads all Element data from the fort.14 file.
 *
 * File stream object must already be at the beginning of the element list in the fort.14 file.
 *
 * @param elementCount The number of Elements to read
 * @param fileStream File stream located at the beginning of the element list in the fort.14 file
 * @return 0
 */
unsigned int TerrainLayer::ReadElementData(unsigned int elementCount, std::ifstream *fileStream)
{
	return ReadElementData(elementCount, fileStream, 0, 0);
}


/**
 * @brief Helper function that reads all Element data from the fort.14 file
 *
 * Helper function that reads all Element data from the fort.14 file.
 *
 * File stream object must already be at the beginning of the element list in the fort.14 file.
 *
 * Emits the current fort.14 processing progress.
 *
 * @param elementCount The number of Elements to read
 * @param fileStream File stream located at the beginning of the element list in the fort.14 file
 * @param currProgress The current progress in processing the fort.14 file
 * @param totalProgress The value that indicates all of the fort.14 file has processed
 * @return The progress in processing the fort.14 file after the element data has been read
 */
unsigned int TerrainLayer::ReadElementData(unsigned int elementCount, std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress)
{
	Element currElement;
	int trash, currNodeNumber;
	for (unsigned int i=0; i<elementCount; i++)
	{
		*fileStream >> currElement.elementNumber;
		*fileStream >> trash;
		*fileStream >> currNodeNumber;
		currElement.n1 = GetNode(currNodeNumber);
		*fileStream >> currNodeNumber;
		currElement.n2 = GetNode(currNodeNumber);
		*fileStream >> currNodeNumber;
		currElement.n3 = GetNode(currNodeNumber);
		elements.push_back(currElement);
		if (totalProgress)
			emit progress(100*(++currProgress)/totalProgress);
	}
	return currProgress;
}


/**
 * @brief Helper function that reads the list of boundary nodes from the fort.14 file
 *
 * Helper function that reads the list of boundary nodes from the fort.14 file.
 *
 * File stream object must already be at the beginning of the list of boundaries in the fort.14 file.
 *
 * @param fileStream File stream located at the beginning of the list of boundaries in the fort.14 file
 * @return 0
 */
unsigned int TerrainLayer::ReadBoundaryNodes(std::ifstream *fileStream)
{
	return ReadBoundaryNodes(fileStream, 0, 0);
}


/**
 * @brief Helper function that reads the list of boundary nodes from the fort.14 file
 *
 * Helper function that reads the list of boundary nodes from the fort.14 file.
 *
 * File stream object must already be at the beginning of the list of boundaries in the fort.14 file.
 *
 * Emits the current fort.14 processing progress.
 *
 * @param fileStream File stream located at the beginning of the list of boundaries in the fort.14 file
 * @param currProgress The current progress in processing the fort.14 file
 * @param totalProgress The value that indicates all of the fort.14 file has processed
 * @return The progress in processing the fort.14 file after the boundary data has been read
 */
unsigned int TerrainLayer::ReadBoundaryNodes(std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress)
{
	std::string line;
	int numSegments, numBoundaryNodes, finalProgressValue;
	finalProgressValue = currProgress + BOUNDARY_PROGRESS_VALUE;
	std::getline(*fileStream, line);
	std::getline(*fileStream, line);
	std::stringstream(line) >> numSegments;
	std::getline(*fileStream, line);
	std::stringstream(line) >> numBoundaryNodes;
	if (numSegments == 1)
	{
		int numNextBoundaryNodes, nextNodeNumber;
		*fileStream >> numNextBoundaryNodes;
		int progressPointSize = BOUNDARY_PROGRESS_VALUE/numNextBoundaryNodes;
		for (int i=0; i<numNextBoundaryNodes; i++)
		{
			*fileStream >> nextNodeNumber;
			boundaryNodes.push_back(nextNodeNumber);
			if (totalProgress)
			{
				currProgress += progressPointSize;
				emit progress(100*(currProgress)/totalProgress);
			}
		}
	}
	else if (totalProgress)
	{
		emit progress(finalProgressValue);
		return finalProgressValue;
	}
	return currProgress;
}


/**
 * @brief Helper function that calculates the normalized coordinates for each Node
 *
 * Helper function that calculates the normalized coordinates for each Node
 *
 * @return 0
 */
unsigned int TerrainLayer::NormalizeCoordinates()
{
	return NormalizeCoordinates(0, 0);
}


/**
 * @brief Helper function that calculates the normalized coordinates for each Node
 *
 * Helper function that calculates the normalized coordinates for each Node
 *
 * Emits the current fort.14 processing progress.
 *
 * @param currProgress The current progress in processing the fort.14 file
 * @param totalProgress The value that indicates all of the fort.14 file has processed
 * @return The progress in processing the fort.14 file after the normalized coordinates have been calculated
 */
unsigned int TerrainLayer::NormalizeCoordinates(unsigned int currProgress, unsigned int totalProgress)
{
	midX = minX + (maxX - minX) / 2.0;
	midY = minY + (maxY - minY) / 2.0;
	max = fmax(maxX-minX, maxY-minY);
	for (unsigned int i=0; i<numNodes; i++)
	{
		nodes[i].normX = (nodes[i].x - midX)/max;
		nodes[i].normY = (nodes[i].y - midY)/max;
		nodes[i].normZ = nodes[i].z / (maxZ-minZ);
		if (totalProgress)
			emit progress(100*(++currProgress)/totalProgress);
	}
	return currProgress;
}

