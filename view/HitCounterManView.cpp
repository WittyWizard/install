#ifdef HITCOUNTERMAN
/* ****************************************************************************
 * Hit Counter Manager View
 */
#include <Wt/WText>
#include "HitCounterManView.h"
/* ****************************************************************************
 * Hit Counter Man View
 */
HitCounterManView::HitCounterManView(Wt::Dbo::SqlConnectionPool& db, std::string myLocale)
{
    //Wt::log("start") << " *** HitCounterManView::HitCounterManView() myLocale = " << myLocale << " *** ";
    theLocale_ = myLocale;
    impl_ = new HitCounterManImpl(db, myLocale);
    setImplementation(impl_);
} // end HitCounterManView
/* ****************************************************************************
 * Update
 */
Wt::WWidget* HitCounterManView::Update()
{
    //Wt::log("start") << " *** HitCounterManView::Update() theLocale_ = " << theLocale_ << " *** ";
    Wt::WContainerWidget *container = new Wt::WContainerWidget();

    Wt::WText *text1 =  new Wt::WText(Wt::WString::tr("hits") + ": " + impl_->GetHits() + "<br />", container);
    text1->setStyleClass("hits");

    Wt::WText *text2 =  new Wt::WText(Wt::WString::tr("unique-hits") + ": " + impl_->GetUniqueHits(),  container);
    text2->setStyleClass("unique-hits");

    return container;
} // end Update
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
