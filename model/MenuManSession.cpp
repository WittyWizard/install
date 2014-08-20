#ifdef MENUMAN
/* ****************************************************************************
 * Menu Manager Session
 * Witty Wizard
 */
#include "MenuManSession.h"
/* ****************************************************************************
 * Menu Manager Session
 */
MenuManSession::MenuManSession(const std::string& appPath, const std::string& useDb, Wt::Dbo::SqlConnectionPool& connectionPool) : appPath_(appPath), useDb_(useDb), connectionPool_(connectionPool)
{
    //Wt::log("start") << " *** MenuManSession::MenuManSession() *** ";
    if (useDb == "1")
    {
        setConnectionPool(connectionPool_);
        mapClass<MenuMan>("menuman"); // table name menuman
        Wt::WApplication* app = Wt::WApplication::instance();
        std::string path = app->internalPath(); // /lang/menuman/
        bool doXmlUpdate = false;  // FIXME add security for logon or certificate
        // hard code /admin/menuman/updatexml for admin work
        if (path.find("/admin/menuman/updatexml") != std::string::npos) { doXmlUpdate = true; }
        if (Crystallball::InitDb)
        {
            try
            {
                Wt::Dbo::Transaction t(*this);
                // Note: you must drop tables to do update
                if (doXmlUpdate)
                {
                    Wt::log("warning") << "MenuManSession::MenuManSession() droptables = ";
                    dropTables();
                }
                createTables();
                std::cerr << "Created database: menuman " << std::endl;
                t.commit();
                if (!ImportXML())
                {
                    Wt::log("error") << " *** MenuManSession::MenuManSession() ImportXML failed! *** ";
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Using existing menuman database";
                Wt::log("notice") << " *** MenuManSession::MenuManSession()  Using existing menuman database ";
            }
        }
    } // end if (useDb == "1")
    //Wt::log("end") << " *** MenuManSession::MenuManSession() *** ";
} // end MenuManSession
/* ****************************************************************************
 * Menu Man Session
 * Check Database for existing records and delete them
 * Read in XML file and populate Database
 */
bool MenuManSession::ImportXML()
{
    Wt::log("start") << " *** MenuManSession::ImportXML()  *** ";
    std::string fullFilePath = appPath_ + "menuman-db-import.xml";
    if (Crystallball::IsFile(fullFilePath))
    {
        Wt::log("info") << "MenuManSession::ImportXML: " << fullFilePath;
    }
    else
    {
        Wt::log("error") << "-> Missing XML Configuration File MenuManSession::ImportXML: " << fullFilePath;
        return false;
    }
    try
    {
        // Open XML File
        const char *filePath = fullFilePath.c_str();
        rapidxml::file<> xmlFile(filePath);
        rapidxml::xml_document<> xmlDoc;
        xmlDoc.parse<0>(xmlFile.data());
        // Find our root node
        /*
         *
            <?xml version="1.0" encoding="UTF-8" ?>
            <menuman name="Test" path="test" content="This is a test"></menuman>
        */
        rapidxml::xml_node<> * root_node = xmlDoc.first_node("menusman");
        rapidxml::xml_attribute<> *nodeAttrib;
        for (rapidxml::xml_node<> * domain_node = root_node->first_node("menuman"); domain_node; domain_node = domain_node->next_sibling("menuman"))
        {
            Wt::log("progress") << "MenuManSession::ImportXML: Start Loop = " << domain_node->name();
            // Start a Transaction
            Wt::Dbo::Transaction t(*this);
            // Create a new Video Instance
            Wt::Dbo::ptr<MenuMan> thisMenu = add(new MenuMan());
            // Set object to Modify
            MenuMan *menuDb = thisMenu.modify();
            // name of Menu Item that will be shown in Menu
            nodeAttrib = domain_node->first_attribute("name");
            if (!nodeAttrib)
            {
                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: name = " << domain_node->name();
                return false;
            }
            menuDb->name = nodeAttrib->value();
            Wt::log("progress") << "MenuManSession::ImportXML: name = " << nodeAttrib->value();
            // path of Menu Item that will be shown in address bar
            nodeAttrib = domain_node->first_attribute("path");
            if (!nodeAttrib)
            {
                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: path = " << domain_node->name();
                return false;
            }
            menuDb->path = nodeAttrib->value();
            Wt::log("progress") << "MenuManSession::ImportXML: path = " << nodeAttrib->value();
            // language of Menu Item that will be shown in address bar: en, cn, ru...
            nodeAttrib = domain_node->first_attribute("language");
            if (!nodeAttrib)
            {
                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: language = " << domain_node->name();
                return false;
            }
            menuDb->language = nodeAttrib->value();
            Wt::log("progress") << "MenuManSession::ImportXML: language = " << nodeAttrib->value();
            // content of page for menu item, XHTML for WidgetFunction
            nodeAttrib = domain_node->first_attribute("content");
            if (!nodeAttrib)
            {
                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: content = " << domain_node->name();
                return false;
            }
            menuDb->content = nodeAttrib->value();
            Wt::log("progress") << "MenuManSession::ImportXML: content = " << nodeAttrib->value();
            // Commit Transaction
            t.commit();
        } // end for
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "MenuManSession::ImportXML(): Failed writting to menuman database";
        Wt::log("error") << "-> MenuManSession::ImportXML()  Failed writting to menuman database: " << e.what();
        return false;
    }
    Wt::log("end") << "MenuManSession::ImportXML()";
    return true;
} // end void MenuManSession::MenuManSession::ImportXML
#endif // MENUMAN
// --- End Of File ------------------------------------------------------------
