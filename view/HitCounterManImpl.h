#ifdef HITCOUNTERMAN
#ifndef HITMANCOUNTERIMPL_H
#define HITMANCOUNTERIMPL_H
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include "model/HitCounterMan.h"
#include "model/HitCounterManSession.h"
/* ****************************************************************************
 * Hit Counter Man Implement
 */
class HitCounterManImpl : public Wt::WContainerWidget
{
    public:
        HitCounterManImpl(Wt::Dbo::SqlConnectionPool& connectionPool, std::string myLocale);
        virtual ~HitCounterManImpl();
        //
        HitCounterManSession session_;
        /* --------------------------------------------------------------------
         * session
         */
        HitCounterManSession& session()  { return session_;  }
        //
        Wt::WWidget* Update();
        std::string GetHits();
        std::string GetUniqueHits();
    private:
        void Set();
        long hits = 0;
        long uniqueHits = 0;
        std::string theLocale;
}; // end class HitCounterManImpl
#endif // HITMANCOUNTERIMPL_H
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
