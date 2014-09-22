/* ****************************************************************************
 * Menu Manager Implementation
 * Witty Wizard
 * Version: 1.0.0
 * Last Date Modified: 21 Aug 2014
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
    //Wt::log("start") << " *** MenuManImpl::GetMenu() menuName = " << menuPath << " | lang_ = " << lang_ << " *** ";
    std::string lookupPath = menuPath;
    if (lookupPath.empty())
    {
        lookupPath = "/";
    }
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
} // end void GetMenu
/* ****************************************************************************
 * Get Menus
 */
void MenuManImpl::GetMenus()
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
    }
} // end void GetMenus
/* ****************************************************************************
 * Get Meta Data
 */
MetaData* MenuManImpl::GetMetaData(const std::string& menuPath)
{
    //Wt::log("start") << " *** MenuManImpl::GetMetaData() menuPath = " << menuPath;
    std::string lookupPath = menuPath;
    if (lookupPath.empty())
    {
        lookupPath = "/";
    }
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
