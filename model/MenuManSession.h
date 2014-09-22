#ifndef MENUMANSESSION_H
#define MENUMANSESSION_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>
//
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
//
#include <iostream>
#include <fstream>
//
#include "CrystalBall.h"
#include "MenuMan.h"
/* ****************************************************************************
 * Menu Man Session
 * appPath:
 * useDb: 1 = true, 0 = false
 * connectionPool:
 */
class MenuManSession : public Wt::Dbo::Session
{
    public:
        MenuManSession(const std::string& appPath, const std::string& useDb, const std::string& lang, Wt::Dbo::SqlConnectionPool& connectionPool);
    private:
        std::string appPath_;
        std::string useDb_;
        std::string lang_;
        Wt::Dbo::SqlConnectionPool& connectionPool_;
        //
        bool ImportXML();
        Wt::WString GetTemplate(std::string id);
}; // end class MenuManSession
#endif // MENUMANSESSION_H
// --- End Of File ------------------------------------------------------------
