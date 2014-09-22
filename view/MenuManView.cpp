/* ****************************************************************************
 * Menu Manager View
 * Witty Wizard
 * Version: 1.0.0
 * Last Date Modified: 19 Aug 2014
 *
 */
#include "MenuManView.h"
/* ****************************************************************************
 * Menu Man View
 * appPath: appRoot() + "home/" + domainName + "/"
 * connectionPool: SQL Database Connection Pool
 * lang: Language
 * useDb:
 * domainName:
 * menuOrientation: Wt::Horizontal or Wt::Vertical
 */
MenuManView::MenuManView(const std::string& appPath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName, Wt::Orientation menuOrientation)
{
    impl_ = new MenuManImpl(appPath, connectionPool, lang, useDb, domainName, menuOrientation);
    setImplementation(impl_);
} // end MenuManView
/* ****************************************************************************
 * Get Menu
 */
Wt::WString MenuManView::GetMenu( const std::string& menuPath)
{
    return impl_->GetMenu(menuPath);
} // end GetMenu
/* ****************************************************************************
 * Get Menus
 */
void MenuManView::GetMenus()
{
    impl_->GetMenus();
    SetMasterMenu();
} // end GetMenus
/* ****************************************************************************
 * Menu Count
 */
void MenuManView::SetMasterMenu()
{
    mastermenus = impl_->mastermenus;
}
/* ****************************************************************************
 * Get Meta Data
 */
MetaData* MenuManView::GetMetaData(const std::string& path)
{
    return impl_->GetMetaData(path);
} // end GetMetaData
// --- End Of File ------------------------------------------------------------
