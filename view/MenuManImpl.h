#ifdef MENUMAN
#ifndef MENUMANIMPL_H
#define MENUMANIMPL_H
#include <Wt/WApplication>
//#include <Wt/WEnvironment>
//#include <Wt/WLogger>
#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WViewWidget>
// Database
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>

#include "model/MenuManSession.h"
/* ****************************************************************************
 * Video Man Implement
 * appPath:
 * basePath:
 * connectionPool:
 * lang:
 */
class MenuManImpl : public Wt::WContainerWidget
{
    public:
        MenuManImpl(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName);
        void SetInternalBasePath(const std::string& basePath);
        virtual ~MenuManImpl();
        /* --------------------------------------------------------------------
         * session
         */
        MenuManSession& session()  { return session_;  }

    private:
        // Init in constructor
        std::string appPath_;
        std::string basePath_;
        MenuManSession session_;
        std::string lang_;
        std::string useDb_;
        std::string domainName_;
        //
        void refresh();
}; // end class MenuManImpl
#endif // MENUMANIMPL_H
#endif // MENUMAN
// --- End Of File ------------------------------------------------------------
