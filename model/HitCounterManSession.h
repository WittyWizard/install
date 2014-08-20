#ifndef HITCOUNTERMANSESSION_H
#define HITCOUNTERMANSESSION_H
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>
#include "Crystallball.h"
#include "HitCounterMan.h"
/* ****************************************************************************
 * Hit Counter Man Session
 */
class HitCounterManSession : public Wt::Dbo::Session
{
    public:
        HitCounterManSession(Wt::Dbo::SqlConnectionPool& connectionPool);
        //
        Wt::Dbo::SqlConnectionPool& connectionPool_;
        void Update();
}; // end class HitCounterManSession
#endif // HITCOUNTERMANSESSION_H
// --- End Of File ------------------------------------------------------------
