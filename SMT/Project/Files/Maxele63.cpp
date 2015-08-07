#include "Maxele63.h"


Maxele63::Maxele63(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}

Maxele63::Maxele63(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
    projectFile(projectFile),
    isFullDomain(true),
    headerLine(),
    numNodes(0),
    targetFile()
{
}


// maxele constructor - aa15
Maxele63::Maxele63(QString targetFile,
                   QString domainName,
                   ProjectFile *projectFile,
                   QObject *parent) :
    QObject(parent),
    domainName(),
    projectFile(projectFile),
    isFullDomain(true),
    headerLine(),
    numNodes(0),
    targetFile(targetFile)
{

    ReadFile();

}

Maxele63::Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}


QString Maxele63::GetHeaderLine()
{
    if (headerLine.isEmpty()){
        ReadFile();
    }
    return headerLine;
}

void Maxele63::ReadFile()
{

    if (!targetFile.isEmpty()){

        std::ifstream file (targetFile.toStdString().data());
        if (file.is_open())
        {
            std::string currentLine;
            int dummy;

            // Read the header line
            std::getline(file, currentLine);
            headerLine = QString(currentLine.data());

            // read the number of nodes
            std::getline(file, currentLine);
            std::stringstream(currentLine) >> dummy >> numNodes;

            std::cout << dummy << " " << numNodes << std::endl;

            // read time
            std::getline(file, currentLine);

            // read maxeles
            unsigned int node;
            float maxele;
            for (int i=0;i<numNodes;i++){
                std::getline(file, currentLine);
                std::stringstream(currentLine) >> node >> maxele ;
                if (maxele==-99999.0)
                    maxele = 0.0;
                maxeles.push_back(maxele);
            }
        }
    }

    GetMinMaxele();
    GetMaxMaxele();
}


unsigned int Maxele63::GetNumNodes(){
    return  maxeles.size();
}


float Maxele63::GetMaxele(int index){
    return maxeles[index];
}

float Maxele63::GetMinMaxele(){
    float minmaxele = (float)(std::min_element(maxeles.begin(),maxeles.end()))[0];
    std::cout << minmaxele << std::endl;
    return minmaxele;
}

float Maxele63::GetMaxMaxele(){
    float maxmaxele = (float)(std::max_element(maxeles.begin(),maxeles.end()))[0];
    std::cout << maxmaxele << std::endl;
    return maxmaxele;
}
