#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T15:07:18
#
#-------------------------------------------------

QT       += core gui opengl xml

DEFINES += GLEW_STATIC

TARGET = SMT
TEMPLATE = app

!win32 {
	LIBS += -lGLU
}

LIBS += -lcurl

SOURCES += main.cpp\
        MainWindow.cpp \
    OpenGL/OpenGLPanel.cpp \
    OpenGL/glew.c \
    OpenGL/GLCamera.cpp \
    OpenGL/Shaders/SolidShader.cpp \
    OpenGL/Shaders/GradientShader.cpp \
    OpenGL/Shaders/GLShader.cpp \
    OpenGL/Shaders/CulledSolidShader.cpp \
    Widgets/ShaderOptionsStackedWidget.cpp \
    Widgets/ColorWidgets/ValueSlider.cpp \
    Widgets/ColorWidgets/TriangleSliderButton.cpp \
    Widgets/ColorWidgets/SliderItemDelegate.cpp \
    Widgets/ColorWidgets/GradientSliderWidget.cpp \
    Widgets/ColorWidgets/GradientSliderFrame.cpp \
    Widgets/ColorWidgets/CustomColorsWidget.cpp \
    Widgets/ColorWidgets/ColorGradientFrame.cpp \
    Widgets/ColorWidgets/ColorButton.cpp \
    Widgets/ColorWidgets/BasicColorsWidget.cpp \
    Layers/TerrainLayer.cpp \
    Layers/SelectionLayer.cpp \
    Layers/Layer.cpp \
    Layers/Actions/NodeAction.cpp \
    Layers/Actions/ElementState.cpp \
    Layers/Actions/ElementAction.cpp \
    Layers/Actions/Action.cpp \
    Layers/SelectionLayers/FullDomainSelectionLayer.cpp \
    Layers/SelectionLayers/CreationSelectionLayer.cpp \
    SubdomainTools/SelectionTool.cpp \
    SubdomainTools/RectangleTool.cpp \
    SubdomainTools/PolygonTool.cpp \
    SubdomainTools/ClickTool.cpp \
    SubdomainTools/CircleTool.cpp \
    SubdomainTools/BoundaryFinder.cpp \
    Quadtree/RectangleSearchNew.cpp \
    Quadtree/QuadtreeSearch.cpp \
    Quadtree/Quadtree.cpp \
    Quadtree/PolygonSearchNew.cpp \
    Quadtree/PointSearch.cpp \
    Quadtree/ConvexCircleSearch.cpp \
    Quadtree/CircleSearchNew.cpp \
    Quadtree/SearchTools/RectangleSearch.cpp \
    Quadtree/SearchTools/PolygonSearch.cpp \
    Quadtree/SearchTools/DepthSearch.cpp \
    Quadtree/SearchTools/ClickSearch.cpp \
    Quadtree/SearchTools/CircleSearch.cpp \
    Project/Project.cpp \
    Project/ProjectSettings.cpp \
    Project/Domains/SubDomain.cpp \
    Project/Domains/FullDomain.cpp \
    Project/Domains/Domain.cpp \
    Project/Files/Py141.cpp \
    Project/Files/Py140.cpp \
    Project/Files/ProjectFile.cpp \
    Project/Files/Maxvel63.cpp \
    Project/Files/Maxele63.cpp \
    Project/Files/Fort067.cpp \
    Project/Files/Fort066.cpp \
    Project/Files/Fort64.cpp \
    Project/Files/Fort63.cpp \
    Project/Files/Fort022.cpp \
    Project/Files/Fort22.cpp \
    Project/Files/Fort021.cpp \
    Project/Files/Fort020.cpp \
    Project/Files/Fort015.cpp \
    Project/Files/Fort15.cpp \
    Project/Files/Fort14.cpp \
    Project/Files/BNList14.cpp \
    Project/Files/Workers/Fort14Reader.cpp \
    Adcirc/SubdomainRunner.cpp \
    Adcirc/BoundaryConditionsExtractor.cpp \
    Dialogs/ProjectSettingsDialog.cpp \
    Dialogs/FullDomainRunOptionsDialog.cpp \
    Dialogs/DisplayOptionsDialog.cpp \
    Dialogs/CreateSubdomainDialog.cpp \
    Dialogs/CreateProjectDialog.cpp \
    Adcirc/FullDomainRunner.cpp \
    Adcirc/SubdomainCreator.cpp \
    Layers/SelectionLayers/SubDomainSelectionLayer.cpp \
    Project/Files/Workers/Fort14Writer.cpp \
    Project/Files/Fort13.cpp \
    Layers/OpenStreetMapLayer.cpp

HEADERS  += MainWindow.h \
    OpenGL/wglew.h \
    OpenGL/OpenGLPanel.h \
    OpenGL/glxew.h \
    OpenGL/glew.h \
    OpenGL/GLData.h \
    OpenGL/GLCamera.h \
    OpenGL/Shaders/SolidShader.h \
    OpenGL/Shaders/GradientShader.h \
    OpenGL/Shaders/GLShader.h \
    OpenGL/Shaders/CulledSolidShader.h \
    Widgets/ShaderOptionsStackedWidget.h \
    Widgets/ColorWidgets/ValueSlider.h \
    Widgets/ColorWidgets/TriangleSliderButton.h \
    Widgets/ColorWidgets/SliderItemDelegate.h \
    Widgets/ColorWidgets/GradientSliderWidget.h \
    Widgets/ColorWidgets/GradientSliderFrame.h \
    Widgets/ColorWidgets/CustomColorsWidget.h \
    Widgets/ColorWidgets/ColorGradientFrame.h \
    Widgets/ColorWidgets/ColorButton.h \
    Widgets/ColorWidgets/BasicColorsWidget.h \
    Layers/TerrainLayer.h \
    Layers/SelectionLayer.h \
    Layers/Layer.h \
    Layers/Actions/NodeAction.h \
    Layers/Actions/ElementState.h \
    Layers/Actions/ElementAction.h \
    Layers/Actions/Action.h \
    Layers/SelectionLayers/FullDomainSelectionLayer.h \
    Layers/SelectionLayers/CreationSelectionLayer.h \
    SubdomainTools/SelectionTool.h \
    SubdomainTools/RectangleTool.h \
    SubdomainTools/PolygonTool.h \
    SubdomainTools/ClickTool.h \
    SubdomainTools/CircleTool.h \
    SubdomainTools/BoundaryFinder.h \
    adcData.h \
    Quadtree/RectangleSearchNew.h \
    Quadtree/QuadtreeSearch.h \
    Quadtree/QuadtreeData.h \
    Quadtree/Quadtree.h \
    Quadtree/PolygonSearchNew.h \
    Quadtree/PointSearch.h \
    Quadtree/ConvexCircleSearch.h \
    Quadtree/CircleSearchNew.h \
    Quadtree/SearchTools/RectangleSearch.h \
    Quadtree/SearchTools/PolygonSearch.h \
    Quadtree/SearchTools/DepthSearch.h \
    Quadtree/SearchTools/ClickSearch.h \
    Quadtree/SearchTools/CircleSearch.h \
    Project/Project.h \
    Project/ProjectSettings.h \
    Project/Domains/SubDomain.h \
    Project/Domains/FullDomain.h \
    Project/Domains/Domain.h \
    Project/Files/Py141.h \
    Project/Files/Py140.h \
    Project/Files/ProjectFile.h \
    Project/Files/Maxvel63.h \
    Project/Files/Maxele63.h \
    Project/Files/Fort067.h \
    Project/Files/Fort066.h \
    Project/Files/Fort64.h \
    Project/Files/Fort63.h \
    Project/Files/Fort022.h \
    Project/Files/Fort22.h \
    Project/Files/Fort021.h \
    Project/Files/Fort020.h \
    Project/Files/Fort015.h \
    Project/Files/Fort15.h \
    Project/Files/Fort14.h \
    Project/Files/BNList14.h \
    Project/Files/Workers/Fort14Reader.h \
    Adcirc/SubdomainRunner.h \
    Adcirc/BoundaryConditionsExtractor.h \
    Dialogs/ProjectSettingsDialog.h \
    Dialogs/FullDomainRunOptionsDialog.h \
    Dialogs/DisplayOptionsDialog.h \
    Dialogs/CreateSubdomainDialog.h \
    Dialogs/CreateProjectDialog.h \
    Adcirc/FullDomainRunner.h \
    Adcirc/SubdomainCreator.h \
    Layers/SelectionLayers/SubDomainSelectionLayer.h \
    Project/Files/Workers/Fort14Writer.h \
    Project/Files/Fort13.h \
    Layers/OpenStreetMapLayer.h

FORMS    += MainWindow.ui \
    Widgets/ShaderOptionsStackedWidget.ui \
    Dialogs/ProjectSettingsDialog.ui \
    Dialogs/FullDomainRunOptionsDialog.ui \
    Dialogs/DisplayOptionsDialog.ui \
    Dialogs/CreateSubdomainDialog.ui \
    Dialogs/CreateProjectDialog.ui

RESOURCES += \
    icons.qrc
