/* ****************************************************************************
 * Menu Manager View
 */
#ifndef MENUMANVIEW_H
#define MENUMANVIEW_H
//
#include <Wt/WCompositeWidget>
#include "MenuManImpl.h"
/* ****************************************************************************
 * Prototype MenuManImpl
 */
class MenuManImpl;
/* ****************************************************************************
 * Menu Man View
 */
class MenuManView : public Wt::WCompositeWidget
{
    public:
        MenuManView(const std::string& appPath, Wt::Dbo::SqlConnectionPool& db, const std::string& lang, const std::string& useDb, const std::string& domainName, Wt::Orientation menuOrientation, std::string path);
        Wt::WString GetMenu(const std::string& menuPath);
        void GetMenus();
        // Menu Vector Array
        std::vector<MasterMenu> mastermenus;
        // Add Menu
        void AddMenu(const MasterMenu& l) { mastermenus.push_back(l); }
        //
        void SetMasterMenu();
        MetaData* GetMetaData(const std::string& menuPath);
    private:
        MenuManImpl* impl_;
        std::string myPath;
        std::string myLanguage;
}; // end class MenuManView
#endif // MENUMANVIEW_H
// --- End Of File ------------------------------------------------------------
