/* ****************************************************************************
 * Menu Manager View
 */
#ifdef MENUMAN
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
        MenuManView(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& db, const std::string& lang, const std::string& useDb, const std::string& domainName);
        void SetInternalBasePath(const std::string& basePath);
    private:
        MenuManImpl* impl_;
}; // end class MenuManView
#endif // MENUMANVIEW_H
#endif // MENUMAN
// --- End Of File ------------------------------------------------------------
