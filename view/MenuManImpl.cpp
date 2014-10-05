/* ****************************************************************************
 * Menu Manager Implementation
 * Witty Wizard
 * Version: 1.0.0
 * Last Date Modified: 23 Sep 2014
 *
 */
#include "MenuManImpl.h"
/* ****************************************************************************
 * Menu Man Impl
 * This gets called every time the page is refreshed
 * appPath: appRoot() + "home/" + domainName + "/menuman/" ~ /full-path/app_root/home/domainName/menuman/
 * basePath: "/" + langPath + "/menuman/" ~ /en/menuman/
 * connectionPool: dbo
 * lang: Name: en, cn, ru ...
 * useDb: 0 = false (use XML), 1 = true (use Db)
 * domainName: domain.tdl
 */
MenuManImpl::MenuManImpl(const std::string& appPath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName, Wt::Orientation& menuOrientation) : appPath_(appPath), session_(appPath, useDb, lang, connectionPool), lang_(lang), useDb_(useDb), domainName_(domainName), menuOrientation_(menuOrientation)
{
} // end MenuManImpl
/* ****************************************************************************
 * ~Menu Man Impl
 */
MenuManImpl::~MenuManImpl()
{
    clear();
} // end ~MenuManImpl
/* ****************************************************************************
 * Get Menu
 */
Wt::WString MenuManImpl::GetMenu(const std::string& menuPath)
{
    std::string lookupPath = menuPath;
    if (lookupPath.empty())
        { lookupPath = "/"; }
    if (useDb_ == "1")
    {
        //Wt::log("start") << " *** MenuManImpl::GetMenu() menuName = " << menuPath << " | lang_ = " << lang_ << " *** ";
        try
        {
            // Start a Transaction
            Wt::Dbo::Transaction t(session_);
            Wt::Dbo::ptr<MenuMan> thisMenu = session_.find<MenuMan>().where("path = ?").bind(lookupPath).where("language = ?").bind(lang_);
            // Commit Transaction
            t.commit();
            return Wt::WString::fromUTF8(thisMenu->content.toUTF8());
        }
        catch (std::exception& e)
        {
            Wt::log("error") << "MenuManImpl::GetMenu()  Failed reading from menuman database = " << e.what();
            return "";
        }
    }
    else
    {
        Wt::WString content;
        std::string myLang = lang_;
        if (myLang == "en")
            { myLang = ""; }
        std::string fullFilePath = appPath_ + "db/menuman-db-import" + myLang + ".xml";
        if (CrystalBall::IsFile(fullFilePath))
        {
            Wt::log("info") << "MenuManImpl::GetMenu: " << fullFilePath;
        }
        else
        {
            Wt::log("error") << "-> Missing XML Configuration File MenuManImpl::GetMenu: " << fullFilePath;
            return "";
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
                Wt::log("progress") << "MenuManImpl::GetMenu: Start Loop = " << domain_node->name();
                // language
                nodeAttrib = domain_node->first_attribute("language");
                if (!nodeAttrib)
                {
                    Wt::log("error") << "MenuManImpl::GetMenu: Missing XML Element: language = " << domain_node->name();
                    return "";
                }
                std::string language = nodeAttrib->value();
                Wt::log("progress") << "MenuManImpl::GetMenu: language = " << nodeAttrib->value();
                //
                if (lang_ == language)
                {
                    // type
                    nodeAttrib = domain_node->first_attribute("type");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenu: Missing XML Element: type = " << domain_node->name();
                        return "";
                    }
                    std::string type = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMenu: type = " << nodeAttrib->value();
                    if (type == "" || type == "submenu")
                    {
                        // path of Menu Item that will be shown in address bar
                        nodeAttrib = domain_node->first_attribute("path");
                        if (!nodeAttrib)
                        {
                            Wt::log("error") << "MenuManImpl::GetMenu: Missing XML Element: path = " << domain_node->name();
                            return "";
                        }
                        std::string path = nodeAttrib->value();
                        Wt::log("progress") << "MenuManImpl::GetMenu: path = " << nodeAttrib->value();
                        // Read from file
                        if (path == menuPath)
                        {
                            content = CrystalBall::GetTemplate(appPath_, path, lang_);
                            Wt::log("progress") << "MenuManSession::ImportXML: content";
                            break;
                        } // end if (path == menuPath)
                    } // end if (type == "" || type == "submenu")
                } // end if (lang_ == language)
            } // end for
        }
        catch (std::exception& e)
        {
            Wt::log("error") << "-> MenuManImpl::GetMenu()  Failed writting to menuman database: " << e.what();
            return "";
        }
        return content;
    }
} // end void GetMenu
/* ****************************************************************************
 * Get Menus
 */
bool MenuManImpl::GetMenus()
{
    if (useDb_ == "1")
    {
        try
        {
            // Start a Transaction
            Wt::Dbo::Transaction t(session_);
            typedef Wt::Dbo::collection< Wt::Dbo::ptr<MenuMan> > MenuMans;
            // Get database list of all Videos
            MenuMans menus = session_.find<MenuMan>();
            for (MenuMans::const_iterator i = menus.begin(); i != menus.end(); ++i)
            {
                std::string language    = (*i)->language;
                if (lang_ == language)
                {
                    Wt::WString name        = (*i)->name;
                    std::string path        = (*i)->path;
                    std::string type        = (*i)->type;
                    std::string parent      = (*i)->parent;
                    std::string grandparent = (*i)->grandparent;
                    AddMenu(MasterMenu(Wt::WString::fromUTF8(name.toUTF8()), path.c_str(), type.c_str(), parent.c_str(), grandparent.c_str(), false));
                }
            }
            // Commit Transaction
            t.commit();
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "MenuManImpl::GetMenu: Failed reading from menuman database";
            Wt::log("error") << "MenuManImpl::GetMenu()  Failed reading from menuman database";
            return false;
        }
        return true;
    }
    else
    {
        std::string myLang = lang_;
        if (myLang == "en")
            { myLang = ""; }
        std::string fullFilePath = appPath_ + "db/menuman-db-import" + myLang + ".xml";
        if (CrystalBall::IsFile(fullFilePath))
        {
            Wt::log("info") << "MenuManImpl::GetMenus: " << fullFilePath;
        }
        else
        {
            Wt::log("error") << "-> Missing XML Configuration File MenuManImpl::GetMenus: " << fullFilePath;
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
                Wt::log("progress") << "MenuManImpl::GetMenus: Start Loop = " << domain_node->name();
                // language of Menu Item that will be shown in address bar: en, cn, ru...
                nodeAttrib = domain_node->first_attribute("language");
                if (!nodeAttrib)
                {
                    Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: language = " << domain_node->name();
                    return false;
                }
                std::string language = nodeAttrib->value();
                Wt::log("progress") << "MenuManImpl::GetMenus: language = " << nodeAttrib->value();
                //
                if (lang_ == language)
                {
                    // name of Menu Item that will be shown in Menu
                    nodeAttrib = domain_node->first_attribute("name");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: name = " << domain_node->name();
                        return false;
                    }
                    Wt::WString name = Wt::WString::fromUTF8(nodeAttrib->value());
                    Wt::log("progress") << "MenuManImpl::GetMenus: name = " << nodeAttrib->value();
                    // path of Menu Item that will be shown in address bar
                    nodeAttrib = domain_node->first_attribute("path");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: path = " << domain_node->name();
                        return false;
                    }
                    std::string path = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMenus: path = " << nodeAttrib->value();
                    // type
                    nodeAttrib = domain_node->first_attribute("type");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: type = " << domain_node->name();
                        return false;
                    }
                    std::string type = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMenus: type = " << nodeAttrib->value();
                    // parent
                    nodeAttrib = domain_node->first_attribute("parent");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: parent = " << domain_node->name();
                        return false;
                    }
                    std::string parent = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMenus: parent = " << nodeAttrib->value();
                    // grandparent
                    nodeAttrib = domain_node->first_attribute("grandparent");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMenus: Missing XML Element: grandparent = " << domain_node->name();
                        return false;
                    }
                    std::string grandparent = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMenus: grandparent = " << nodeAttrib->value();
                    AddMenu(MasterMenu(Wt::WString::fromUTF8(name.toUTF8()), path.c_str(), type.c_str(), parent.c_str(), grandparent.c_str(), false));
                }
            } // end for
        }
        catch (std::exception& e)
        {
            Wt::log("error") << "-> MenuManImpl::GetMenus()  Failed writting to menuman database: " << e.what();
            return false;
        }
        return true;
    } // end if (useDb != "1")
} // end void GetMenus
/* ****************************************************************************
 * Get Meta Data
 */
MetaData* MenuManImpl::GetMetaData(const std::string& menuPath)
{
    //Wt::log("start") << " *** MenuManImpl::GetMetaData() menuPath = " << menuPath;
    std::string lookupPath = menuPath;
    if (lookupPath.empty())
        { lookupPath = "/"; }
    if (useDb_ == "1")
    {
        try
        {
            // Start a Transaction
            Wt::Dbo::Transaction t(session_);
            Wt::Dbo::ptr<MenuMan> thisMenu = session_.find<MenuMan>().where("path = ?").bind(lookupPath).where("language = ?").bind(lang_);
            metaData.title = thisMenu->title;
            metaData.description = thisMenu->description;
            metaData.keywords = thisMenu->keywords;
            metaData.rating = thisMenu->rating;
            // Commit Transaction
            t.commit();
            return &metaData;
        }
        catch (std::exception& e)
        {
            Wt::log("error") << "MenuManImpl::GetMetaData()  Failed reading from menuman database Menu Path = " << menuPath << " | error = " << e.what();
            return NULL;
        }
    }
    else // Read from XML Files in /xml
    {
        std::string myLang = lang_;
        if (myLang == "en")
            { myLang = ""; }
        std::string fullFilePath = appPath_ + "db/menuman-db-import" + myLang + ".xml";
        if (CrystalBall::IsFile(fullFilePath))
        {
            Wt::log("info") << "MenuManImpl::GetMetaData: " << fullFilePath;
        }
        else
        {
            Wt::log("error") << "-> Missing XML Configuration File MenuManImpl::GetMetaData: " << fullFilePath;
            return NULL;
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
                Wt::log("progress") << "MenuManImpl::GetMetaData: Start Loop = " << domain_node->name();
                // name of Menu Item that will be shown in Menu
                // language of Menu Item that will be shown in address bar: en, cn, ru...
                nodeAttrib = domain_node->first_attribute("language");
                if (!nodeAttrib)
                {
                    Wt::log("error") << "MenuManImpl::GetMetaData: Missing XML Element: language = " << domain_node->name();
                    return NULL;
                }
                std::string language = nodeAttrib->value();
                Wt::log("progress") << "MenuManImpl::GetMetaData: language = " << nodeAttrib->value();
                //
                if (lang_ == language)
                {
                    // type
                    nodeAttrib = domain_node->first_attribute("type");
                    if (!nodeAttrib)
                    {
                        Wt::log("error") << "MenuManImpl::GetMetaData: Missing XML Element: type = " << domain_node->name();
                        return NULL;
                    }
                    std::string type = nodeAttrib->value();
                    Wt::log("progress") << "MenuManImpl::GetMetaData: type = " << nodeAttrib->value();
                    if (type == "submenu" || type == "")
                    {
                        // path of Menu Item that will be shown in address bar
                        nodeAttrib = domain_node->first_attribute("path");
                        if (!nodeAttrib)
                        {
                            Wt::log("error") << "MenuManImpl::GetMetaData: Missing XML Element: path = " << domain_node->name();
                            return NULL;
                        }
                        std::string path = nodeAttrib->value();
                        Wt::log("progress") << "MenuManImpl::GetMetaData: path = " << nodeAttrib->value();
                        if (path == lookupPath)
                        {
                            // title
                            nodeAttrib = domain_node->first_attribute("title");
                            if (!nodeAttrib)
                            {
                                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: title = " << domain_node->name();
                                return NULL;
                            }
                            metaData.title = Wt::WString::fromUTF8(nodeAttrib->value());
                            Wt::log("progress") << "MenuManSession::ImportXML: title";
                            // description
                            nodeAttrib = domain_node->first_attribute("description");
                            if (!nodeAttrib)
                            {
                                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: description = " << domain_node->name();
                                return NULL;
                            }
                            metaData.description = Wt::WString::fromUTF8(nodeAttrib->value());
                            Wt::log("progress") << "MenuManSession::ImportXML: description";
                            // keywords
                            nodeAttrib = domain_node->first_attribute("keywords");
                            if (!nodeAttrib)
                            {
                                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: keywords = " << domain_node->name();
                                return NULL;
                            }
                            metaData.keywords = Wt::WString::fromUTF8(nodeAttrib->value());
                            Wt::log("progress") << "MenuManSession::ImportXML: keywords";
                            // copyright
                            nodeAttrib = domain_node->first_attribute("copyright");
                            if (!nodeAttrib)
                            {
                                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: copyright = " << domain_node->name();
                                return NULL;
                            }
                            metaData.copyright = Wt::WString::fromUTF8(nodeAttrib->value());
                            Wt::log("progress") << "MenuManSession::ImportXML: copyright";
                            // rating
                            nodeAttrib = domain_node->first_attribute("rating");
                            if (!nodeAttrib)
                            {
                                Wt::log("error") << "MenuManSession::ImportXML: Missing XML Element: rating = " << domain_node->name();
                                return NULL;
                            }
                            metaData.rating = nodeAttrib->value();
                            Wt::log("progress") << "MenuManSession::ImportXML: rating = " << nodeAttrib->value();
                            return &metaData;
                        }
                    } // end if (type == "submenu")
                } // end if (lang_ == language)
            } // end for
        }
        catch (std::exception& e)
        {
            Wt::log("error") << "-> MenuManImpl::GetMetaData()  Failed writting to menuman database: " << e.what();
            return NULL;
        }
        return NULL;
    } // end if (useDb_ != "1")
} // end GetMetaData
/* ****************************************************************************
 * Make Type: horz, vert
 * Return correct tag for each
 */
std::string MenuManImpl::GetUl()
{
    if (menuOrientation_ == Wt::Orientation::Horizontal)
    {
        return "sfhmenu";
    }
    return "sfvmenu";
}
/* ****************************************************************************
 *
 */
std::string MenuManImpl::GetWrapperClass()
{
    if (menuOrientation_ == Wt::Orientation::Horizontal)
    {
        return "sfmenufixhorz";
    }
    return "sfmenufixvert";
}
/* ****************************************************************************
 *
 */
std::string MenuManImpl::GetMenuClass()
{
    if (menuOrientation_ == Wt::Orientation::Horizontal)
    {
        return "sfmenumodhorz";
    }
    return "sfmenumodvert";
}
// --- End Of File ------------------------------------------------------------
