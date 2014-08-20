#ifdef HITCOUNTERMAN
/* ****************************************************************************
 * Hit Counter Man Implement
 */
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
#include <Wt/WContainerWidget>
#include <Wt/WText>

#include "WittyWizard.h"
#include "model/HitCounterMan.h"
#include "HitCounterManImpl.h"
typedef Wt::Dbo::collection< Wt::Dbo::ptr<HitCounterMan> > HitCounterMans;
/* ****************************************************************************
 * Hit Counter Man Implement
 */
HitCounterManImpl::HitCounterManImpl(Wt::Dbo::SqlConnectionPool& connectionPool, std::string myLocale) : session_(connectionPool)
{
    //Wt::log("start") << " *** HitCounterManImpl::HitCounterManImpl() *** ";
    theLocale = myLocale;
    Set();
} // end HitCounterManImpl
/* ****************************************************************************
 * ~Hit Counter Implement
 */
HitCounterManImpl::~HitCounterManImpl()
{
    clear();
} // end ~HitCounterManImpl
/* ****************************************************************************
 * Set
 */
void HitCounterManImpl::Set()
{
    // Create an instance of app to access Internal Paths
    Wt::WApplication* app = Wt::WApplication::instance();
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);
        HitCounterMans myHitCounterMan = session_.find<HitCounterMan>();
        //
        hits = myHitCounterMan.size();
        //
        // Commit Transaction
        t.commit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "HitCounterManImpl::Update: Failed reading from hitcounterman database";
        Wt::log("error") << "HitCounterManImpl::Update()  Failed reading from hitcounterman database";
    }

    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);
        //
        uniqueHits = session_.query<int>("select COUNT(distinct ipaddress) from hitcounterman").where("page = ?").bind(app->internalPath());;
        // Commit Transaction
        t.commit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "HitCounterManImpl::Update: Failed reading from hitcounterman database";
        Wt::log("error") << "HitCounterManImpl::Update()  Failed reading from hitcounterman database";
    }

} // end
/* ****************************************************************************
 * Get Hits
 */
std::string HitCounterManImpl::GetHits()
{
    // Create an instance of app to access Internal Paths
    Wt::WApplication* app = Wt::WApplication::instance();
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);

        hits = session_.query<int>("select count(*) from hitcounterman").where("page = ?").bind(app->internalPath());
        //Wt::log("notice") << "HitCounterManImpl::GetHits()  hits = " << hits << " | page = " << app->internalPath();

        // Commit Transaction
        t.commit();    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        //Wt::log("error") << "HitCounterManImpl::GetHits()  Failed reading from hitcounterman database.";
        hits = 0;
    }

    try
    {
        Wt::WLocale myString = Wt::WLocale(theLocale.c_str());
        // Note: this only works if you set the Separator, use case to set it
        myString.setGroupSeparator(",");
        std::string myReturn = myString.toString(hits).toUTF8();
        return myReturn;
        /*
         * This requires locale to be installed and configured on server
        std::stringstream ss;
        ss.imbue(std::locale(theLocale.c_str()));
        ss << std::fixed << hits;
        return ss.str();
        */
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "HitCounterManImpl::GetHits: Failed local not installed";
        Wt::log("error") << "HitCounterManImpl::GetHits() Failed local not installed";
    }

    return std::to_string(hits);
} // end GetHits
/* ****************************************************************************
 * Get Unique Hits
 */
std::string HitCounterManImpl::GetUniqueHits()
{
    // Create an instance of app to access Internal Paths
    Wt::WApplication* app = Wt::WApplication::instance();
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);

        uniqueHits = session_.query<int>("select count(distinct ipaddress) from hitcounterman").where("page = ?").bind(app->internalPath());
        //Wt::log("notice") << "HitCounterManImpl::GetHits()  uniqueHits = " << uniqueHits << " | page = " << app->internalPath();

        // Commit Transaction
        t.commit();    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        Wt::log("error") << "HitCounterManImpl::GetUniqueHits()  Failed reading from hitcounterman database.";
        hits = 0;
    }

    try
    {
        Wt::WLocale myString = Wt::WLocale(theLocale.c_str());
        // Note: this only works if you set the Separator, use case to set it
        myString.setGroupSeparator(",");
        std::string myReturn = myString.toString(uniqueHits).toUTF8();
        return myReturn;
        /*
         * This requires locale to be installed and configured on server
        std::stringstream ss;
        ss.imbue(std::locale(theLocale.c_str()));
        ss << std::fixed << hits;
        return ss.str();
        */
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "HitCounterManImpl::GetUniqueHits: Failed local not installed";
        Wt::log("error") << "HitCounterManImpl::GetUniqueHits() Failed local not installed";
    }
    return std::to_string(uniqueHits);
} // end GetUniqueHits
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
