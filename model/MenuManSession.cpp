/* ****************************************************************************
 * Menu Manager Session
 * Witty Wizard
 */
#include "MenuManSession.h"
/* ****************************************************************************
 * Menu Manager Session
 * appPath:
 * useDb:
 * lang: FIXME: default|lang1|lang2|lang3
 * connectionPool:
 */
MenuManSession::MenuManSession(const std::string& appPath, const std::string& useDb, const std::string& lang, Wt::Dbo::SqlConnectionPool& connectionPool) : appPath_(appPath), useDb_(useDb), lang_(lang), connectionPool_(connectionPool)
{
    if (useDb == "1")
    {
        setConnectionPool(connectionPool_);
        mapClass<MenuMan>("menuman"); // table name menuman
        Wt::WApplication* app = Wt::WApplication::instance();
        std::string path = app->internalPath(); // /lang/menuman/
        bool doXmlUpdate = false;
        // FIXME add security for logon or certificate
        // hard code /admin/menuman/updatexml for admin work
        if (path.find("/admin/menuman/updatexml") != std::string::npos) { doXmlUpdate = true; }
        //Wt::log("start") << " *** MenuManSession::MenuManSession() useDb | path = " << path << " | doXmlUpdate = " << doXmlUpdate << " *** ";
        if (CrystalBall::InitDb || doXmlUpdate)
        {
            try
            {
                Wt::Dbo::Transaction t(*this);
                // Note: you must drop table to do update
                if (doXmlUpdate)
                {
                    Wt::log("warning") << "MenuManSession::MenuManSession() SQL Drop Table menuman";
                    dropTables();
                    Wt::WApplication::instance()->setInternalPath("/", true);
                }
                createTables();
                Wt::log("warning") << "Created database: menuman ";
                if (!ImportXML())
                {
                    Wt::log("error") << " *** MenuManSession::MenuManSession() ImportXML failed! *** ";
                    return;
                }
                t.commit();
            }
            catch (std::exception& e)
            {
                Wt::log("warning") << " *** MenuManSession::MenuManSession()  Using existing menuman database = " << e.what();
            }
        }
    } // end if (useDb == "1")
    //Wt::log("end") << " *** MenuManSession::MenuManSession() *** ";
} // end MenuManSession
/* ****************************************************************************
 * Menu Man Session
 * Check Database for existing records and delete them
 * Read in XML file and populate Database
 * <menuman name="" path="" type="" parent="" grandparent="" language="" content="" description="" keywords="" title="" copyright="" rating=""></menuman>
 */
bool MenuManSession::ImportXML()
{
    Wt::log("start") << " *** MenuManSession::ImportXML()  *** ";
    std::string fullFilePath = appPath_ + "db/menuman-db-import.xml";
    // FIXME: Multilanguage support, _cn.xml for chinese, make a loop
    /*
    */
    std::vector<std::string> myDir;
    CrystalBall::GetFilesInDirectory(myDir, appPath_ + "db");
    for (std::vector<std::string>::iterator it = myDir.begin() ; it != myDir.end(); ++it)
    {
       Wt::log("notice") <<  "File = " << *it;
       fullFilePath = *it;
       if (CrystalBall::IsFile(fullFilePath))
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
               menuDb->name = Wt::WString::fromUTF8(nodeAttrib->value());
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
               // type
               nodeAttrib = domain_node->first_attribute("type");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: type = " << domain_node->name();
                   return false;
               }
               std::string menuType = nodeAttrib->value();
               menuDb->type = nodeAttrib->value();
               Wt::log("progress") << "MenuManSession::ImportXML: type = " << nodeAttrib->value();
               // parent
               nodeAttrib = domain_node->first_attribute("parent");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: parent = " << domain_node->name();
                   return false;
               }
               menuDb->parent = nodeAttrib->value();
               Wt::log("progress") << "MenuManSession::ImportXML: parent = " << nodeAttrib->value();
               // grandparent
               nodeAttrib = domain_node->first_attribute("grandparent");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: grandparent = " << domain_node->name();
                   return false;
               }
               menuDb->grandparent = nodeAttrib->value();
               Wt::log("progress") << "MenuManSession::ImportXML: grandparent = " << nodeAttrib->value();
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

               // Read from file
               if (menuType == "" || menuType == "submenu")
               {
                   nodeAttrib = domain_node->first_attribute("content");
                   if (!nodeAttrib)
                   {
                       Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: content = " << domain_node->name();
                       return false;
                   }
                   std::string fileName = nodeAttrib->value();
                   menuDb->content = CrystalBall::GetTemplate(appPath_ + "xml/" + fileName + ".xml");
                   Wt::log("progress") << "MenuManSession::ImportXML: content";
               } // end if (menuType == "" || menuType == "submenu")
               // description
               nodeAttrib = domain_node->first_attribute("description");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: description = " << domain_node->name();
                   return false;
               }
               menuDb->description = Wt::WString::fromUTF8(nodeAttrib->value());
               Wt::log("progress") << "MenuManSession::ImportXML: description";
               // keywords
               nodeAttrib = domain_node->first_attribute("keywords");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: keywords = " << domain_node->name();
                   return false;
               }
               menuDb->keywords = Wt::WString::fromUTF8(nodeAttrib->value());
               Wt::log("progress") << "MenuManSession::ImportXML: keywords";
               // title
               nodeAttrib = domain_node->first_attribute("title");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: title = " << domain_node->name();
                   return false;
               }
               menuDb->title = Wt::WString::fromUTF8(nodeAttrib->value());
               Wt::log("progress") << "MenuManSession::ImportXML: title";
               // copyright
               nodeAttrib = domain_node->first_attribute("copyright");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: copyright = " << domain_node->name();
                   return false;
               }
               menuDb->copyright = Wt::WString::fromUTF8(nodeAttrib->value());
               Wt::log("progress") << "MenuManSession::ImportXML: copyright";
               // rating
               nodeAttrib = domain_node->first_attribute("rating");
               if (!nodeAttrib)
               {
                   Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: rating = " << domain_node->name();
                   return false;
               }
               menuDb->rating = nodeAttrib->value();
               Wt::log("progress") << "MenuManSession::ImportXML: rating = " << nodeAttrib->value();
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
    }

    Wt::log("end") << "MenuManSession::ImportXML()";
    return true;
} // end void MenuManSession::MenuManSession::ImportXML

// --- End Of File ------------------------------------------------------------
