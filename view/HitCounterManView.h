#ifdef HITCOUNTERMAN
#ifndef HITCOUNTERMANVIEW_H
#define HITCOUNTERMANVIEW_H
#include <Wt/WCompositeWidget>
#include <Wt/WText>
#include "HitCounterManImpl.h"
/* ****************************************************************************
 * Prototype Hit Counter Man Impl
 */
class HitCounterManImpl;
/* ****************************************************************************
 * class Hit Counter Man View
 */
class HitCounterManView : public Wt::WCompositeWidget
{
    public:
        HitCounterManView(Wt::Dbo::SqlConnectionPool& db, std::string myLocale);
        Wt::WWidget* Update();
    private:
        HitCounterManImpl *impl_;
        std::string theLocale_;
};
#endif // HITCOUNTERMANVIEW_H
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
