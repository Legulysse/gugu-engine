#include <QtWidgets/QApplication>
#include "Window/MainWindow.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "PugiXml/PugiXml.hpp"


int main(int argc, char *argv[])
{
    //Parse args
    std::string strArg1;
    std::string strArg2;
    if (argc >= 1)
        strArg1 = argv[0];
    if (argc >= 2)
        strArg2 = argv[1];

    //Init engine
    gugu::EngineConfig oConfig;
    oConfig.applicationName = "GuguEngine Editor";
    oConfig.pathAssets      = "Assets";
    oConfig.pathScreenshots = "Screenshots";
    oConfig.gameWindow      = gugu::EGameWindow::None;

    bool bFullPaths = false;
    bool bPreload = true;

    //Parse config file
    std::string strPathEditorConfig = "EditorConfig.xml";
    if (!strArg2.empty())
        strPathEditorConfig = strArg2;

    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(strPathEditorConfig.c_str());
    if (result)
    {
        pugi::xml_node oNodeConfig = oDoc.child("Config");
        if (oNodeConfig)
        {
            pugi::xml_node oNodeAssets = oNodeConfig.child("Assets");
            if (oNodeAssets)
            {
                oConfig.pathAssets = oNodeAssets.text().as_string(oConfig.pathAssets.c_str());
            }

            pugi::xml_node oNodeAssetsFullPaths = oNodeConfig.child("AssetsFullPaths");
            if (oNodeAssetsFullPaths)
            {
                bFullPaths = oNodeAssetsFullPaths.text().as_bool(bFullPaths);
            }

            //TODO: Handle resources without a preload (need to guess ResourceTypes before loading)
            //pugi::xml_node oNodePreload = oNodeConfig.child("Preload");
            //if (oNodePreload)
            //{
            //    bPreload = oNodePreload.text().as_bool(bPreload);
            //}
        }
    }

    oConfig.useAssetsFullPaths = bFullPaths;

    //Start Engine
    gugu::GetEngine()->Init(oConfig);

    if (bPreload)
        gugu::GetResources()->PreloadAll();

    int iExecReturn = 0;

    //Init UI
    QApplication a(argc, argv);
    {
        MainWindow* w = new MainWindow;
        w->show();
        w->InitTools();

        //Start loop
        iExecReturn = a.exec(); //MainWindow will delete itself once closed
    }

    gugu::GetEngine()->Release();

    //Exit
    return iExecReturn;
}
