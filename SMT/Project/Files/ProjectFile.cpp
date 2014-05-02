#include "ProjectFile.h"

const QString ProjectFile::TAG_FULL_DOMAIN = "fullDomain";
const QString ProjectFile::TAG_PROJECT = "adcSubdomainProject";
const QString ProjectFile::TAG_SETTINGS = "settings";
const QString ProjectFile::TAG_SUB_DOMAIN = "subDomain";

const QString ProjectFile::ATTR_ADCIRCLOCATION = "adcircExe";
const QString ProjectFile::ATTR_BNLISTLOCATION = "bnListLoc";
const QString ProjectFile::ATTR_DIRECTORY = "dir";
const QString ProjectFile::ATTR_FORT10LOCATION = "fort10Loc";
const QString ProjectFile::ATTR_FORT11LOCATION = "fort11Loc";
const QString ProjectFile::ATTR_FORT13LOCATION = "fort13Loc";
const QString ProjectFile::ATTR_FORT14LOCATION = "fort14Loc";
const QString ProjectFile::ATTR_FORT15LOCATION = "fort15Loc";
const QString ProjectFile::ATTR_FORT19LOCATION = "fort19Loc";
const QString ProjectFile::ATTR_FORT20LOCATION = "fort20Loc";
const QString ProjectFile::ATTR_FORT22LOCATION = "fort22Loc";
const QString ProjectFile::ATTR_FORT23LOCATION = "fort23Loc";
const QString ProjectFile::ATTR_FORT24LOCATION = "fort24Loc";
const QString ProjectFile::ATTR_FORT63LOCATION = "fort63Loc";
const QString ProjectFile::ATTR_FORT64LOCATION = "fort64Loc";
const QString ProjectFile::ATTR_FORT015LOCATION = "fort015Loc";
const QString ProjectFile::ATTR_FORT019LOCATION = "fort019Loc";
const QString ProjectFile::ATTR_FORT020LOCATION = "fort020Loc";
const QString ProjectFile::ATTR_FORT021LOCATION = "fort021Loc";
const QString ProjectFile::ATTR_FORT065LOCATION = "fort065Loc";
const QString ProjectFile::ATTR_FORT066LOCATION = "fort066Loc";
const QString ProjectFile::ATTR_FORT067LOCATION = "fort067Loc";
const QString ProjectFile::ATTR_LASTSAVE = "savedOn";
const QString ProjectFile::ATTR_MAXELELOCATION = "maxeleLoc";
const QString ProjectFile::ATTR_MAXVELLOCATION = "maxvelLoc";
const QString ProjectFile::ATTR_NAME = "name";
const QString ProjectFile::ATTR_PY140 = "py140Loc";
const QString ProjectFile::ATTR_PY141 = "py141Loc";


ProjectFile::ProjectFile()
{
	FileModified();
}


/**
 * @brief Opens an Adcirc Subdomain Project file
 *
 * Once the file has been successfully opened, all ProjectFile access functions
 * become available.
 *
 * Opens an Adcirc Subdomain Project file. If a project is already open,
 * we prompt the user to either save it, discard changes, or cancel opening
 * a new project.
 *
 * @param filePath The project file to open
 */
bool ProjectFile::OpenProjectFile(QString filePath)
{
	if (!ProjectIsOpen() || WarnProjectAlreadyOpen())
	{
		SetProjectFile(filePath);
		OpenFileRead();
		ReadFile();
		CloseFile();
		return true;
	}
	return false;
}


/**
 * @brief Creates a project directory (and a project file within that directory) in the
 * parent directory
 *
 * Creates a project directory (and a project file within that directory) in the
 * parent directory. The newly created project file is opened and all ProjectFile
 * access functions become available.
 *
 * If a project file is already open, we first prompt the user to either save,
 * discard changes, or cancel creating a new project.
 *
 * @param parentDirectory The directory in which to create the project directory
 * @param projectName The name to give both the project directory and the project file
 */
bool ProjectFile::CreateProjectFile(QString parentDirectory, QString projectName)
{
	if (!ProjectIsOpen() || WarnProjectAlreadyOpen())
	{
		/* Move into the project directory */
		/* If the directory doesn't exist, try to create it */
		projectDirectory = QDir(parentDirectory);
		if (!projectDirectory.cd(projectName))
		{
			if (!projectDirectory.mkdir(projectName))
			{
				WarnFileError("Unable to create project directory");
				return false;
			}
			else if (!projectDirectory.cd(projectName))
			{
				WarnFileError("Unable to access the project directory");
				return false;
			}
		}

		/* Make sure there isn't a project file in the directory already */
		projectDirectory.setNameFilters(QStringList("*.spf"));
		if (!projectDirectory.entryList().isEmpty())
		{
			WarnFileError(QString("A project file already exists at ").append(parentDirectory+projectName));
			return false;
		}

		/* Create the file */
		SetProjectFile(projectDirectory.absolutePath() + QDir::separator() + projectName + ".spf");
		if (OpenFileWrite())
		{
			/* Create an empty project */
			CreateEmptyProject();

			/* Save the project file */
			SaveFile();

			/* Close the project file */
			CloseFile();

			return true;
		}
	}
	return false;
}


bool ProjectFile::SaveProject()
{
	if (OpenFileWrite())
		if (SaveFile())
			return CloseFile();
	return false;
}


bool ProjectFile::ProjectIsOpen()
{
	return documentElement().hasAttributes() || documentElement().hasChildNodes();
}


QString ProjectFile::GetAdcircLocation()
{
	return GetAttribute(TAG_SETTINGS, ATTR_ADCIRCLOCATION);
}


QString ProjectFile::GetFullDomainDirectory()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_DIRECTORY);
}


QString ProjectFile::GetFullDomainFort13()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT13LOCATION);
}


QString ProjectFile::GetFullDomainFort14()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT14LOCATION);
}


QString ProjectFile::GetFullDomainFort15()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT15LOCATION);
}


QString ProjectFile::GetFullDomainFort63()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT63LOCATION);
}


QString ProjectFile::GetFullDomainFort64()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT64LOCATION);
}


QString ProjectFile::GetFullDomainFort015()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT015LOCATION);
}


QString ProjectFile::GetFullDomainFort065()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT065LOCATION);
}


QString ProjectFile::GetFullDomainFort066()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT066LOCATION);
}


QString ProjectFile::GetFullDomainFort067()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT067LOCATION);
}


QString ProjectFile::GetFullDomainMaxele()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_MAXELELOCATION);
}


QString ProjectFile::GetFullDomainMaxvel()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_MAXVELLOCATION);
}


QDateTime ProjectFile::GetLastFileAccess()
{
	return lastModified;
}


QString ProjectFile::GetProjectDirectory()
{
	return projectDirectory.absolutePath();
}


QString ProjectFile::GetProjectFile()
{
	return projectFile.fileName();
}


QString ProjectFile::GetProjectName()
{
	return projectName;
}


QString ProjectFile::GetSubDomainBNList(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_BNLISTLOCATION);
}


QString ProjectFile::GetSubDomainDirectory(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_DIRECTORY);
}


QString ProjectFile::GetSubDomainFort13(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT13LOCATION);
}


QString ProjectFile::GetSubDomainFort14(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT14LOCATION);
}


QString ProjectFile::GetSubDomainFort15(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT15LOCATION);
}


QString ProjectFile::GetSubDomainFort63(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT63LOCATION);
}


QString ProjectFile::GetSubDomainFort64(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT64LOCATION);
}


QString ProjectFile::GetSubDomainFort015(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT015LOCATION);
}


QString ProjectFile::GetSubDomainFort019(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT019LOCATION);
}


QString ProjectFile::GetSubDomainFort020(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT020LOCATION);
}


QString ProjectFile::GetSubDomainFort021(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT021LOCATION);
}


QString ProjectFile::GetSubDomainMaxele(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_MAXELELOCATION);
}


QString ProjectFile::GetSubDomainMaxvel(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_MAXVELLOCATION);
}


QStringList ProjectFile::GetSubDomainNames()
{
	QStringList subdomainNames;

	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement subdomainNameAttribute = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (!subdomainNameAttribute.isNull())
		{
			subdomainNames.append(subdomainNameAttribute.text());
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}

	return subdomainNames;
}


QString ProjectFile::GetSubDomainPy140(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_PY140);
}


QString ProjectFile::GetSubDomainPy141(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_PY141);
}


void ProjectFile::SetAdcircLocation(QString newLoc)
{
	SetAttribute(TAG_SETTINGS, ATTR_ADCIRCLOCATION, newLoc);
}


void ProjectFile::SetFullDomainDirectory(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_DIRECTORY, newLoc);
}


void ProjectFile::SetFullDomainFort10(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT10LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT10LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort11(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT11LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT11LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort13(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT13LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT13LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort14(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT14LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT14LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort15(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT15LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT15LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort19(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT19LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT19LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort20(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT20LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT20LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort22(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT22LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT22LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort23(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT23LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT23LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort24(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT24LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT24LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort63(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT63LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT63LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort64(QString newLoc, bool symLink)
{
	if (symLink)
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT64LOCATION, LinkFile(newLoc, projectDirectory.absolutePath()));
	else
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT64LOCATION, CopyFile(newLoc, projectDirectory.absolutePath()));
}


void ProjectFile::SetFullDomainFort015(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT015LOCATION, newLoc);
}


void ProjectFile::SetFullDomainFort065(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT065LOCATION, newLoc);
}


void ProjectFile::SetFullDomainFort066(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT066LOCATION, newLoc);
}


void ProjectFile::SetFullDomainFort067(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT067LOCATION, newLoc);
}


void ProjectFile::SetFullDomainMaxele(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_MAXELELOCATION, newLoc);
}


void ProjectFile::SetFullDomainMaxvel(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_MAXVELLOCATION, newLoc);
}


void ProjectFile::SetSubDomainBNList(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_BNLISTLOCATION, newLoc);
}


void ProjectFile::SetSubDomainDirectory(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_DIRECTORY, newLoc);
}


void ProjectFile::SetSubDomainFort13(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT13LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort14(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT14LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort15(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT15LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort63(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT63LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort64(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT64LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort015(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT015LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort019(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT019LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort020(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT020LOCATION, newLoc);
}


void ProjectFile::SetSubDomainFort021(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT021LOCATION, newLoc);
}


void ProjectFile::SetSubDomainMaxele(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_MAXELELOCATION, newLoc);
}


void ProjectFile::SetSubDomainMaxvel(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_MAXVELLOCATION, newLoc);
}


void ProjectFile::SetSubDomainName(QString oldName, QString newName)
{
	SetAttributeSubdomain(oldName, ATTR_NAME, newName);
}


void ProjectFile::SetSubDomainPy140(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_PY140, newLoc);
}


void ProjectFile::SetSubDomainPy141(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_PY141, newLoc);
}


bool ProjectFile::AddSubdomain(QString newName)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == newName)
		{
			if (!WarnSubdomainExists(newName))
			{
				return false;
			} else {
				documentElement().removeChild(currentSubdomain);
				break;
			}
		}

		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
//		{
//			QDomNode attributeNode = currentSubdomain.namedItem(attribute);
//			if (!attributeNode.isNull())
//			{
//				currentSubdomain.removeChild(attributeNode);
//			}

//			QDomElement attributeElement = createElement(attribute);
//			attributeElement.appendChild(createTextNode(value));
//			currentSubdomain.appendChild(attributeElement);
//			break;
//		}
	}

	QDomElement newSubdomain = createElement(TAG_SUB_DOMAIN);
	QDomElement newSubdomainName = createElement(ATTR_NAME);
	documentElement().appendChild(newSubdomain);
	newSubdomain.appendChild(newSubdomainName);
	newSubdomainName.appendChild(createTextNode(newName));

	SaveProject();

	return true;
}


void ProjectFile::SetProjectFile(QString filePath)
{
	projectFile.setFileName(filePath);
	projectDirectory = QFileInfo(filePath).absoluteDir();
	projectName = QFileInfo(filePath).baseName();
}


bool ProjectFile::OpenFileRead()
{
	if (!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		std::cout << projectFile.fileName().toStdString().data() << std::endl;
		WarnFileError(QString("Unable to open file ").append(projectFile.fileName()));
		return false;
	}

	return true;
}


bool ProjectFile::OpenFileWrite()
{
	if (!projectFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		std::cout << projectFile.fileName().toStdString().data() << std::endl;
		WarnFileError(QString("Unable to open file ").append(projectFile.fileName()));
		return false;
	}

	return true;
}


bool ProjectFile::ReadFile()
{

	if (!setContent(&projectFile))
	{
		WarnFileError("Error setting project data");
		projectFile.close();
		return false;
	}

	FileModified();
	return true;
}


bool ProjectFile::SaveFile()
{
	FileModified();
	QString projectText = toString(5);
	QTextStream fileOut(&projectFile);
	fileOut << projectText;
	projectFile.close();
	return true;
}


bool ProjectFile::CloseFile()
{
	projectFile.close();
	return true;
}


void ProjectFile::CreateEmptyProject()
{
	clear();
	appendChild(createElement(TAG_PROJECT));
	QDomElement fullDomainElement = createElement(TAG_FULL_DOMAIN);
	QDomElement subDomainElement = createElement(TAG_SUB_DOMAIN);
	QDomElement settingsElement = createElement(TAG_SETTINGS);
	documentElement().appendChild(fullDomainElement);
	documentElement().appendChild(subDomainElement);
	documentElement().appendChild(settingsElement);
}


QString ProjectFile::GetAttribute(QString attribute)
{
	QDomElement attributeElement = documentElement().namedItem(attribute).toElement();
	if (!attributeElement.isNull())
	{
		return attributeElement.text();
	}
	return QString();
}


QString ProjectFile::GetAttribute(QString tag, QString attribute)
{
	QDomElement tagElement = documentElement().namedItem(tag).toElement();
	if (!tagElement.isNull())
	{
		QDomElement attributeElement = tagElement.namedItem(attribute).toElement();
		if (!attributeElement.isNull())
		{
			return attributeElement.text();
		}
	}
	return QString();
}


QString ProjectFile::GetAttribute(QString parentTag, QString childTag, QString attribute)
{
	QDomElement parentTagElement = documentElement().namedItem(parentTag).toElement();
	if (!parentTagElement.isNull())
	{
		QDomElement childTagElement = parentTagElement.namedItem(childTag).toElement();
		if (!childTagElement.isNull())
		{
			QDomElement attributeElement = childTagElement.namedItem(attribute).toElement();
			if (!attributeElement.isNull())
			{
				return attributeElement.text();
			}
		}
	}
	return QString();
}


QString ProjectFile::GetAttributeSubdomain(QString subdomainName, QString attribute)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == subdomainName)
		{
			QDomElement attributeElement = currentSubdomain.namedItem(attribute).toElement();
			if (!attributeElement.isNull())
			{
				return attributeElement.text();
			}
			break;
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}
	return QString();
}


void ProjectFile::SetAttribute(QString attribute, QString value)
{
	QDomNode attributeNode = documentElement().namedItem(attribute);
	if (!attributeNode.isNull())
	{
		documentElement().removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	documentElement().appendChild(attributeElement);

	SaveProject();
}


void ProjectFile::SetAttribute(QString tag, QString attribute, QString value)
{
	QDomElement tagElement = documentElement().namedItem(tag).toElement();
	if (tagElement.isNull())
	{
		tagElement = createElement(tag);
		documentElement().appendChild(tagElement);
	}

	QDomNode attributeNode = tagElement.namedItem(attribute);
	if (!attribute.isNull())
	{
		tagElement.removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	tagElement.appendChild(attributeElement);

	SaveProject();
}


void ProjectFile::SetAttribute(QString parentTag, QString childTag, QString attribute, QString value)
{
	QDomElement parentTagElement = documentElement().namedItem(parentTag).toElement();
	if (parentTagElement.isNull())
	{
		parentTagElement = createElement(parentTag);
		documentElement().appendChild(parentTagElement);
	}

	QDomElement childTagElement = parentTagElement.namedItem(childTag).toElement();
	if (childTagElement.isNull())
	{
		childTagElement = createElement(childTag);
		parentTagElement.appendChild(childTagElement);
	}

	QDomNode attributeNode = childTagElement.namedItem(value);
	if (!attributeNode.isNull())
	{
		childTagElement.removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	childTagElement.appendChild(attributeElement);

	SaveProject();
}


void ProjectFile::SetAttributeSubdomain(QString subdomainName, QString attribute, QString value)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == subdomainName)
		{
			QDomNode attributeNode = currentSubdomain.namedItem(attribute);
			if (!attributeNode.isNull())
			{
				currentSubdomain.removeChild(attributeNode);
			}

			QDomElement attributeElement = createElement(attribute);
			attributeElement.appendChild(createTextNode(value));
			currentSubdomain.appendChild(attributeElement);
			break;
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}

	SaveProject();
}


void ProjectFile::FileModified()
{
	lastModified = QDateTime::currentDateTime();

	QDomNode attributeNode = documentElement().namedItem(ATTR_LASTSAVE);
	if (!attributeNode.isNull())
	{
		documentElement().removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(ATTR_LASTSAVE);
	attributeElement.appendChild(createTextNode(lastModified.toString(Qt::ISODate)));
	documentElement().appendChild(attributeElement);
}


bool ProjectFile::CheckForExistingFile(QString filename)
{
	return QFile(filename).exists();
}


QString ProjectFile::CopyFile(QString oldFile, QString newDir)
{
	QString newFile = newDir + QDir::separator() + QFileInfo(oldFile).fileName();

	if (CheckForExistingFile(newFile))
	{
		if (WarnFileExists(newFile))
		{
			RemoveFile(newFile);
		} else {
			return oldFile;
		}
	}

	QFile file (oldFile);
	if (file.exists())
	{
		if (file.copy(newFile))
			return newFile;
	}
	return oldFile;
}


QString ProjectFile::LinkFile(QString oldFile, QString newDir)
{
	QString newLink = newDir + QDir::separator() + QFileInfo(oldFile).fileName();

#ifdef Q_OS_WIN32
	newLink.append(".lnk");
#endif

	if (CheckForExistingFile(newLink))
	{
		if (WarnFileExists(newLink))
		{
			RemoveFile(newLink);
		} else {
			return oldFile;
		}
	}

	QFile file (oldFile);
	if (file.exists())
	{
		if (file.link(newLink))
			return newLink;
	}
	return oldFile;
}


void ProjectFile::RemoveFile(QString oldFile)
{
	QDir().remove(oldFile);
}


/**
 * @brief Warns the user that a project is currently open and asks if they would like to
 * continue with or without saving
 *
 * Warns the user that a project is currently open and asks if they would like to
 * continue with or without saving. If they choose to continue with saving, the
 * saving will be performed here.
 *
 * @return true if the user would like to continue
 * @return false if the user would not like to continue
 */
bool ProjectFile::WarnProjectAlreadyOpen()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("Project already open. Would you like to save and close the current project?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);

	switch (msgBox.exec())
	{
		case QMessageBox::Save:
			/* Save and close the project */
			return true;
		case QMessageBox::Cancel:
			/* Return to the current project */
			return false;
		case QMessageBox::Discard:
			/* Close the project without saving */
			return true;
		default:
			/* Should never be reached */
			return false;
	}
}


/**
 * @brief Warns the user that an error occurred when reading a file
 *
 * Warns the user that an error occurred when reading a file. The message
 * is displayed, prepended with "File Error: "
 *
 * @param message The error message to display
 */
void ProjectFile::WarnFileError(QString message)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText(QString("File Error: ").append(message));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}


bool ProjectFile::WarnFileExists(QString fileLoc)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("The file " + fileLoc + " already exists. Would you like to overwrite it?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	switch (msgBox.exec())
	{
		case QMessageBox::Yes:
			/* Overwrite the file */
			return true;
		case QMessageBox::No:
			/* Do not overwrite the file */
			return false;
		default:
			/* Should never be reached */
			return false;
	}
}


bool ProjectFile::WarnSubdomainExists(QString subName)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("The subdomain " + subName + " already exists. Would you like to overwrite it?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	switch (msgBox.exec())
	{
		case QMessageBox::Yes:
			/* Overwrite the file */
			return true;
		case QMessageBox::No:
			/* Do not overwrite the file */
			return false;
		default:
			/* Should never be reached */
			return false;
	}
}
