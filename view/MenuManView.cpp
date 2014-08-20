#ifdef MENUMAN
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
 * basePath: "/" + langPath + "/video/"
 * connectionPool: SQL Database Connection Pool
 * lang: Language
 */
MenuManView::MenuManView(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName)
{
    impl_ = new MenuManImpl(appPath, basePath, connectionPool, lang, useDb, domainName);
    setImplementation(impl_);
} // end MenuManView
/* ****************************************************************************
 * Set Internal Base Path
 * basePath:
 */
void MenuManView::SetInternalBasePath(const std::string& basePath)
{
    impl_->SetInternalBasePath(basePath);
} // end SetInternalBasePath
#endif // MENUMAN
// --- End Of File ------------------------------------------------------------
