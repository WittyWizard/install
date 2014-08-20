#ifdef VIDEOMAN
/* ****************************************************************************
 * Menu Manager Implementation
 * Witty Wizard
 * Version: 1.0.0
 * Last Date Modified: 19 Aug 2014
 *
 */
#include "MenuManImpl.h"
/* ****************************************************************************
 * Menu Man Impl
 * This gets called every time the page is refreshed
 * appPath: appRoot() + "home/" + domainName + "/videoman/" ~ /full-path/app_root/home/domainName/videoman/
 * basePath: "/" + langPath + "/video/" ~ /en/menuman/
 * connectionPool: dbo
 * lang: Name: en, cn, ru ...
 */
MenuManImpl::MenuManImpl(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName) : appPath_(appPath), basePath_(basePath), session_(appPath, useDb, connectionPool), lang_(lang), useDb_(useDb), domainName_(domainName)
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
 * set Internal Base Path
 */
void MenuManImpl::SetInternalBasePath(const std::string& basePath)
{
    basePath_ = basePath;
    refresh();
} // end void SetInternalBasePath
/* ****************************************************************************
 * refresh
 */
void MenuManImpl::refresh()
{
    //HandlePathChange(wApp->internalPath());
} // end void refresh()
#endif
// --- End Of File ------------------------------------------------------------
