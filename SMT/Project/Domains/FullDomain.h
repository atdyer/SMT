#ifndef FULLDOMAIN_H
#define FULLDOMAIN_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QProgressBar>

#include "Project/Domains/Domain.h"

#include "Layers/SelectionLayers/FullDomainSelectionLayer.h"

#include "Project/Files/Fort13.h"
#include "Project/Files/Fort14.h"
#include "Project/Files/Fort15.h"
#include "Project/Files/Fort22.h"
#include "Project/Files/Fort63.h"
#include "Project/Files/Fort64.h"
#include "Project/Files/Fort015.h"
#include "Project/Files/Fort066.h"
#include "Project/Files/Fort067.h"
#include "Project/Files/Maxele63.h"
#include "Project/Files/Maxvel63.h"
#include "Project/Files/ProjectFile.h"

#include "OpenGL/GLCamera.h"


class FullDomain : public Domain
{
		Q_OBJECT
	public:
		FullDomain(ProjectFile *projectFile, QObject *parent=0);
		~FullDomain();

		virtual bool	IsFullDomain();

		Fort015*			GetFort015();
		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
        QString			GetPath();
		std::vector<Element*>		GetSelectedElements();
        void visualizeDomain(QString displayMode, QString FileName);

	private:

		FullDomainSelectionLayer*	selectionLayerFullDomain;

		Fort13*		fort13;
		Fort15*		fort15;
		Fort22*		fort22;
		Fort63*		fort63;
		Fort64*		fort64;
		Fort015*	fort015;
		Fort066*	fort066;
		Fort067*	fort067;
        Maxele63*	maxele63;
        Maxvel63*	maxvel;

		void	CreateAllFiles();

};

#endif // FULLDOMAIN_H
