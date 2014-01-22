#include "Quadtree.h"


/**
 * @brief This constructor builds the Quadtree data structure from a list of Nodes
 * @param nodes A list of Node objects to be included in the Quadtree
 * @param size The maximum number of Node objects allowed in each leaf
 * @param minX The lower bound x-value
 * @param maxX The upper bound x-value
 * @param minY The lower bound y-value
 * @param maxY The upper bound y-value
 */
Quadtree::Quadtree(std::vector<Node> nodes, int size, float minX, float maxX, float minY, float maxY)
{
	nodeList = nodes;
	binSize = size;

	glLoaded = false;
	pointCount = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	outlineShader = 0;
	camera = 0;

	// Create the root branch
	root = newBranch(minX, maxX, minY, maxY);

	if (binSize > 0)
		for (unsigned int i=0; i<nodeList.size(); i++)
			addNode(&nodeList[i], root);

	hasElements = false;
}


/**
 * @brief This constructor builds the Quadtree data structure from a list of Nodes and
 * a list Elements
 * @param nodes
 * @param elements
 * @param size
 * @param minX
 * @param maxX
 * @param minY
 * @param maxY
 */
Quadtree::Quadtree(std::vector<Node> nodes, std::vector<Element> elements, int size, float minX, float maxX, float minY, float maxY)
{
	nodeList = nodes;
	elementList = elements;
	binSize = size;

	glLoaded = false;
	pointCount = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	outlineShader = 0;
	camera = 0;

	// Create the root branch
	std::cout << "Creating quadtree: " << minX << ", " << maxX << ", " << minY << ", " << maxY << std::endl;
	root = newBranch(minX, maxX, minY, maxY);

	if (binSize > 0)
	{
		for (unsigned int i=0; i<nodeList.size(); i++)
			addNode(&nodeList[i], root);

		for (unsigned int i=0; i<elementList.size(); i++)
			addElement(&elementList[i], root);
	}

	hasElements = true;

}


Quadtree::~Quadtree()
{
	for (unsigned int i=0; i<leafList.size(); i++)
		if (leafList[i] != 0)
			delete leafList[i];
	for (unsigned int i=0; i<branchList.size(); i++)
		if (branchList[i] != 0)
			delete branchList[i];

	/* Clean up shader */
	if (outlineShader)
		delete outlineShader;

	/* Clean up OpenGL stuff */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);
}


void Quadtree::DrawOutlines()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded)
	{
		glBindVertexArray(VAOId);
		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				glDrawElements(GL_LINES, 2*pointCount, GL_UNSIGNED_INT, (GLvoid*)0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void Quadtree::SetCamera(GLCamera *newCam)
{
	camera = newCam;
	if (outlineShader)
		outlineShader->SetCamera(camera);
}


/**
 * @brief This function is called by the user to find the Node closest to the provided x-y coordinates
 *
 * This function is called by the user to find the Node closest to the provided x-y coordinates.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Quadtree's copy of the Node closest to (x, y)
 * @return 0 if the point is outside the bounds of the Quadtree
 */
Node* Quadtree::FindNode(float x, float y)
{
//	return clickSearch.FindNode(root, x, y);
	pointSearch.SetPointParameters(x, y);
	std::vector<Node*> result = pointSearch.FindNodes(root);
	if (result.size())
		return result[0];
	else
		return 0;
}


/**
 * @brief Finds the Element that contains the given point
 *
 * Finds the Element that contains the given point.
 *
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @return Pointer to the Element that contains the point
 * @return 0 if no Element contains the point
 */
Element* Quadtree::FindElement(float x, float y)
{
	pointSearch.SetPointParameters(x, y);
	std::vector<Element*> result = pointSearch.FindElements(root);
	if (result.size())
		return result[0];
	else
		return 0;
}


/**
 * @brief This function is called by the user to find all Nodes that fall inside of the provided circle
 *
 * This function is called by the user to find all Nodes that fall inside of the provided circle
 *
 * @param x The x-coordinate of the circle center
 * @param y The y-coordinate of the circle center
 * @param radius The radius of the circle
 * @return A vector of pointers to the Nodes that fall inside of the circle
 */
std::vector<Node*> Quadtree::FindNodesInCircle(float x, float y, float radius)
{
	return circleSearch.FindNodes(root, x, y, radius);
}


std::vector<Element*> Quadtree::FindElementsInCircle(float x, float y, float radius)
{
	newCircleSearch.SetCircleParameters(x, y, radius);
	return newCircleSearch.FindElements(root);
}


std::vector<Element*> Quadtree::FindElementsInRectangle(float l, float r, float b, float t)
{
	newRectangleSearch.SetRectangleParameters(l, r, b, t);
	return newRectangleSearch.FindElements(root);
}


std::vector<Element*> Quadtree::FindElementsInPolygon(std::vector<Point> polyLine)
{
	newPolySearch.SetPolygonParameters(polyLine);
	return newPolySearch.FindElements(root);
}


std::vector<std::vector<Element*>*> Quadtree::GetElementsThroughDepth(int depth)
{
	return depthSearch.FindElements(root, depth);
}


std::vector<std::vector<Element*>*> Quadtree::GetElementsThroughDepth(int depth, float l, float r, float b, float t)
{
	return depthSearch.FindElements(root, depth, l, r, b, t);
}


/**
 * @brief Creates a new leaf with the specified boundaries
 *
 * This function creates a new leaf on the heap with the specified boundaries
 *
 * @param l The lower bound x-value
 * @param r The upper bound x-value
 * @param b The lower bound y-value
 * @param t The upper bound y-value
 * @return A pointer to the new leaf object
 */
leaf* Quadtree::newLeaf(float l, float r, float b, float t)
{
	// Create leaf in heap and put pointer into leaf list
	leaf *currLeaf = new leaf;
	leafList.push_back(currLeaf);

	// Set the boundaries of the leaf
	currLeaf->bounds[0] = l;
	currLeaf->bounds[1] = r;
	currLeaf->bounds[2] = b;
	currLeaf->bounds[3] = t;

	return currLeaf;
}


/**
 * @brief Creates a new branch with the specified boundaries
 *
 * This function creates a new branch on the heap with the specified boundaries
 *
 * @param l The lower bound x-value
 * @param r The upper bound x-value
 * @param b The lower bound y-value
 * @param t The upper bound y-value
 * @return A pointer to the new branch object
 */
branch* Quadtree::newBranch(float l, float r, float b, float t)
{
	// Create branch in heap and put pointer into branch list
	branch *currBranch = new branch;
	branchList.push_back(currBranch);

	// Set the boundaries of the branch
	currBranch->bounds[0] = l;
	currBranch->bounds[1] = r;
	currBranch->bounds[2] = b;
	currBranch->bounds[3] = t;

	// Set all branch pointers in the branch to 0
	for (int i=0; i<4; i++)
		currBranch->branches[i] = 0;

	// Create all four leaves in the branch
	float x = l+(r-l)/2.0;
	float y = b+(t-b)/2.0;
	currBranch->leaves[0] = newLeaf(x, r, y, t);
	currBranch->leaves[1] = newLeaf(l, x, y, t);
	currBranch->leaves[2] = newLeaf(l, x, b, y);
	currBranch->leaves[3] = newLeaf(x, r, b, y);

	return currBranch;
}


/**
 * @brief Converts a leaf into a branch
 *
 * This function is used to turn a leaf into a branch when the leaf needs to add more nodes
 * but has reached its maximum capacity. All of the nodes that were in the old leaf are added
 * to the new branch and the old leaf is permanently deleted.
 *
 * @param currLeaf A pointer to the leaf that will be turned into a branch
 * @return  A pointer to the new branch object
 */
branch* Quadtree::leafToBranch(leaf *currLeaf)
{
	// Create new branch with same bounds as the old leaf
	branch *currBranch = newBranch(currLeaf->bounds[0], currLeaf->bounds[1], currLeaf->bounds[2], currLeaf->bounds[3]);

	// Add all of the nodes that were in the old leaf to the new branch
	for (unsigned int i=0; i<currLeaf->nodes.size(); i++)
		addNode(currLeaf->nodes[i], currBranch);

	// Remove the old leaf pointer from the leaf list (just change to 0, removing is inefficient) and delete it
	for (unsigned int i=0; i<leafList.size(); i++)
	{
		if (leafList[i] == currLeaf && leafList[i] != 0)
		{
			leafList[i] = 0;
			delete currLeaf;
		}
	}
	return currBranch;
}


/**
 * @brief Adds a Node to a leaf
 *
 * This function is used to add a Node to a leaf. If the leaf is full, it will be turned
 * into a branch and a pointer to the new branch object will be returned.
 *
 * @param currNode A pointer to the Node object that will be added to the leaf
 * @param currLeaf A pointer to the leaf that the Node will be added to
 * @return A pointer to a new branch if the leaf was full and a new branch was created
 * @return 0 if the Node was added successfully to the leaf
 */
branch* Quadtree::addNode(Node *currNode, leaf *currLeaf)
{
	// Make sure leaf has room for the new node. If so, add it. Otherwise,
	// turn the leaf into a branch and add the node to the new branch.
	if ((int)currLeaf->nodes.size() < binSize)
	{
		currLeaf->nodes.push_back(currNode);
		return 0;
	} else {
		branch *currBranch = leafToBranch(currLeaf);
		addNode(currNode, currBranch);
		return currBranch;
	}
}


/**
 * @brief Adds a Node to a branch
 *
 * This function is used to add a Node to a branch. It is called recursively until
 * a leaf is found that the Node will fit into.
 *
 * @param currNode A pointer to the Node object that will be added to the branch
 * @param currBranch A pointer to the branch that the Node will be added to
 */
void Quadtree::addNode(Node *currNode, branch *currBranch)
{
	// Loop through the branches to find a fit
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0)
		{
			// Found a fit, add the node to this branch
			if (nodeIsInside(currNode, currBranch->branches[i]))
			{
				addNode(currNode, currBranch->branches[i]);
				return;
			}
		}
	}

	// Couldn't find a branch to go into, now look for a leaf to go into
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			// Found a fit, add the node to this leaf
			if (nodeIsInside(currNode, currBranch->leaves[i]))
			{
				branch *result = addNode(currNode, currBranch->leaves[i]);

				// Result is 0, so leaf still exists
				if (result == 0)
					return;

				// Leaf was turned into a branch, so update the current branch to include it
				else
				{
					// Leaf gets deleted by the leafToBranch() function
					currBranch->leaves[i] = 0;
					currBranch->branches[i] = result;
					return;
				}
			}
		}
	}

	// This node doesn't fit anywhere, let the user know
	DEBUG("Error adding Node to Quadtree, node picking will not work for node number " << currNode->nodeNumber);
}


/**
 * @brief Adds an Element to the quadtree
 *
 * Adds an Element to the quadtree. An element is considered to be inside of a branch
 * or leaf if any of its Nodes are inside of the branch or leaf. Adding the Element is done
 * by recursively going down the tree based on n1 location.
 *
 * @param currElement The Element being added
 * @param currBranch The current branch we'd like to add the Element to
 */
void Quadtree::addElement(Element *currElement, branch *currBranch)
{
	// See if any of the Nodes fall into any of the branches
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0)
		{
			if (nodeIsInside(currElement->n1, currBranch->branches[i]) ||
			    nodeIsInside(currElement->n2, currBranch->branches[i]) ||
			    nodeIsInside(currElement->n3, currBranch->branches[i]))
			{
				addElement(currElement, currBranch->branches[i]);
//				return;
			}
		}
	}

	// See if any of the Nodes fall into any of the leaves
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			if (nodeIsInside(currElement->n1, currBranch->leaves[i]) ||
			    nodeIsInside(currElement->n2, currBranch->leaves[i]) ||
			    nodeIsInside(currElement->n3, currBranch->leaves[i]))
			{
				currBranch->leaves[i]->elements.push_back(currElement);
//				return;
			}
		}
	}
}


/**
 * @brief A helper function that determines if the Node is inside of the leaf
 *
 * A helper function that determines if the Node is inside of the leaf.
 *
 * @param currNode A pointer to the Node being tested
 * @param currLeaf A pointer to the leaf being tested
 * @return true if the Node is inside of the leaf
 * @return false if the Node is not inside of the leaf
 */
bool Quadtree::nodeIsInside(Node *currNode, leaf *currLeaf)
{
	if (currNode->normX <= currLeaf->bounds[1] && currNode->normX >= currLeaf->bounds[0])
		if (currNode->normY <= currLeaf->bounds[3] && currNode->normY >= currLeaf->bounds[2])
			return true;
	return false;
}


/**
 * @brief A helper function that determines if the Node is inside of the branch
 *
 * A helper function that determines if the Node is inside of the branch.
 *
 * @param currNode A pointer to the Node being tested
 * @param currBranch A pointer to the branch being tested
 * @return true if the Node is inside of the branch
 * @return false if the Node is not inside of the branch
 */
bool Quadtree::nodeIsInside(Node *currNode, branch *currBranch)
{
	if (currNode->normX <= currBranch->bounds[1] && currNode->normX >= currBranch->bounds[0])
		if (currNode->normY <= currBranch->bounds[3] && currNode->normY >= currBranch->bounds[2])
			return true;
	return false;
}


void Quadtree::InitializeGL()
{
	if (!outlineShader)
		outlineShader = new SolidShader();
	outlineShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 1.0*255));
	outlineShader->SetCamera(camera);

	if (!VAOId)
		glGenVertexArrays(1, &VAOId);
	if (!VBOId)
		glGenBuffers(1, &VBOId);
	if (!IBOId)
		glGenBuffers(1, &IBOId);

	glBindVertexArray(VAOId);

	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

	glBindVertexArray(0);

	std::vector<Point> pointsList = BuildOutlinesList();
	std::vector<GLuint> indicesList = BuildOutlinesIndices();

	if (pointsList.size() > 0 && indicesList.size() > 0)
	{
		LoadOutlinesToGPU(pointsList, indicesList);
	} else {
		glLoaded = false;
		return;
	}

	GLenum errorCheck = glGetError();
	if (errorCheck == GL_NO_ERROR)
	{
		if (VAOId && VBOId && IBOId)
		{
			glLoaded = true;
		} else {
			DEBUG("Quadtree drawing not initialized");
			glLoaded = false;
		}
	} else {
		const GLubyte *errString = gluErrorString(errorCheck);
		DEBUG("Quadtree Drawing OpenGL Error: " << errString);
		glLoaded = false;
	}
}


std::vector<Point> Quadtree::BuildOutlinesList()
{
	std::vector<Point> pointsList;

	if (root)
	{
		AddOutlinePoints(root, &pointsList);
	}

	return pointsList;
}


std::vector<GLuint> Quadtree::BuildOutlinesIndices()
{
	std::vector<GLuint> indexList;

	for (int i=0; i<pointCount/4; ++i)
	{
		indexList.push_back(4*i+0); indexList.push_back(4*i+1);
		indexList.push_back(4*i+1); indexList.push_back(4*i+2);
		indexList.push_back(4*i+2); indexList.push_back(4*i+3);
		indexList.push_back(4*i+3); indexList.push_back(4*i+0);
	}

	return indexList;
}


void Quadtree::AddOutlinePoints(branch *currBranch, std::vector<Point> *pointsList)
{
	pointsList->push_back(Point(currBranch->bounds[0], currBranch->bounds[2]));
	pointsList->push_back(Point(currBranch->bounds[1], currBranch->bounds[2]));
	pointsList->push_back(Point(currBranch->bounds[1], currBranch->bounds[3]));
	pointsList->push_back(Point(currBranch->bounds[0], currBranch->bounds[3]));
	pointCount += 4;

	for (int i=0; i<4; ++i)
		if (currBranch->branches[i])
			AddOutlinePoints(currBranch->branches[i], pointsList);
	for (int i=0; i<4; ++i)
		if (currBranch->leaves[i])
			AddOutlinePoints(currBranch->leaves[i], pointsList);
}


void Quadtree::AddOutlinePoints(leaf *currLeaf, std::vector<Point> *pointsList)
{
	pointsList->push_back(Point(currLeaf->bounds[0], currLeaf->bounds[2]));
	pointsList->push_back(Point(currLeaf->bounds[1], currLeaf->bounds[2]));
	pointsList->push_back(Point(currLeaf->bounds[1], currLeaf->bounds[3]));
	pointsList->push_back(Point(currLeaf->bounds[0], currLeaf->bounds[3]));
	pointCount += 4;
}


void Quadtree::LoadOutlinesToGPU(std::vector<Point> pointsList, std::vector<GLuint> indicesList)
{
	const size_t VertexBufferSize = 4*sizeof(GLfloat)*pointsList.size();
	const size_t IndexBufferSize = sizeof(GLuint)*indicesList.size();

	if (VBOId)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
		GLfloat* glNodeData = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			for (unsigned int i=0; i<pointsList.size(); ++i)
			{
				glNodeData[4*i+0] = (GLfloat)pointsList[i].x;
				glNodeData[4*i+1] = (GLfloat)pointsList[i].y;
				glNodeData[4*i+2] = (GLfloat)1.0;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	if (IBOId)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		GLuint* glIndexData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glIndexData)
		{
			for (unsigned int i=0; i<indicesList.size(); ++i)
			{
				glIndexData[i] = indicesList[i];
			}
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}
