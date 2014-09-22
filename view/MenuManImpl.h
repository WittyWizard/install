#ifndef MENUMANIMPL_H
#define MENUMANIMPL_H
#include <Wt/WApplication>
//#include <Wt/WEnvironment>
//#include <Wt/WLogger>
#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WViewWidget>
#include <Wt/WText>
// Database
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>

#include "model/MenuManSession.h"
/* ****************************************************************************
 * Master Menu
 * name: Name of Menu Item
 * path: Internal Path
 * type: "", "submenu", "topsub", "topend", "subsub", "subend"
 * parent: Desending from here
 * grandparent: Desending from here if sub of a sub menu
 * isSelected: True if path is selected, else false
 */
struct MasterMenu
{
    MasterMenu(const Wt::WString& name, std::string path, std::string type, std::string parent, std::string grandparent, bool isSelected) : name_(name), path_(path), type_(type), parent_(parent), grandparent_(grandparent), isSelected_(isSelected) { }
    Wt::WString name_;
    std::string path_;
    std::string type_;
    std::string parent_;
    std::string grandparent_;
    bool isSelected_;
}; // end struct MasterMenu
/* ****************************************************************************
 * MetaData
 */
struct MetaData
{
    Wt::WString title;            // title:
    Wt::WString description;      // description:
    Wt::WString keywords;         // keywords:
    std::string rating;           // rating: general, mature, restricted, 14 years, safe for kids
}; // end struct MetaData
/* ****************************************************************************
 * Menu Manager Implementaion
 * appPath:
 * basePath:
 * connectionPool:
 * lang:
 */
class MenuManImpl : public Wt::WContainerWidget
{
    public:
        MenuManImpl(const std::string& appPath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName, Wt::Orientation& menuOrientation);
        virtual ~MenuManImpl();
        /* --------------------------------------------------------------------
         * session
         */
        MenuManSession& session()  { return session_;  }
        Wt::WString GetMenu(const std::string& menuPath);
        void GetMenus();
        MetaData* GetMetaData(const std::string& menuPath);
        // Menu Vector Array
        std::vector<MasterMenu> mastermenus;
        // Add Menu
        void AddMenu(const MasterMenu& l) { mastermenus.push_back(l); }
    private:
        // Init in constructor
        std::string appPath_;
        MenuManSession session_;
        std::string lang_;
        std::string useDb_;
        std::string domainName_;
        Wt::Orientation menuOrientation_;
        //
        MetaData metaData;
        //
        std::string GetUl();
        std::string GetWrapperClass();
        std::string GetMenuClass();
}; // end class MenuManImpl
#endif // MENUMANIMPL_H
// --- End Of File ------------------------------------------------------------
