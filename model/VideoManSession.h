#ifdef VIDEOMAN
#ifndef VIDEOMANSESSION_H
#define VIDEOMANSESSION_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>
#include <Wt/WComboBox>
//
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
//
#include "VideoMan.h"
#include "Crystallball.h"
//#include <Wt/WComboBox>
/* ****************************************************************************
 * Video Man Session
 * appPath:
 * useDb: 1 = true, 0 = false
 * connectionPool:
 */
class VideoManSession : public Wt::Dbo::Session
{
    public:
        VideoManSession(const std::string& appPath, const std::string& useDb, Wt::Dbo::SqlConnectionPool& connectionPool);
    private:
        std::string appPath_;
        std::string useDb_;
        Wt::Dbo::SqlConnectionPool& connectionPool_;
        //
        bool ImportXML();
}; // end class VideoManSession
#endif // VIDEOMANSESSION_H
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
