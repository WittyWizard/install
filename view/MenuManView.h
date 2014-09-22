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
        MenuManView(const std::string& appPath, Wt::Dbo::SqlConnectionPool& db, const std::string& lang, const std::string& useDb, const std::string& domainName, Wt::Orientation menuOrientation);
        Wt::WString GetMenu(const std::string& menuPath);
        void GetMenus();
        // Menu Vector Array
        std::vector<MasterMenu> mastermenus;
        void SetMasterMenu();
        MetaData* GetMetaData(const std::string& menuPath);
    private:
        MenuManImpl* impl_;
}; // end class MenuManView
#endif // MENUMANVIEW_H
// --- End Of File ------------------------------------------------------------
