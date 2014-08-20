#ifdef HITCOUNTERMAN
/* ****************************************************************************
 * Hit Counter Manager Session
 * Witty Wizard
 */
#include "HitCounterManSession.h"
/* ****************************************************************************
 * Hit Counter Man Session
 */
HitCounterManSession::HitCounterManSession(Wt::Dbo::SqlConnectionPool& connectionPool) : connectionPool_(connectionPool)
{
    //Wt::log("start") << " *** HitCounterManSession::HitCounterManSession() *** ";
    setConnectionPool(connectionPool_);
    mapClass<HitCounterMan>("hitcounterman"); // table name hitcounterman
    if (Crystallball::InitDb)
    {
        try
        {
            Wt::Dbo::Transaction t(*this);
            // Note: you must drop tables to do update
            if (0)
            {
                Wt::log("warning") << "HitCounterManSession::HitCounterManSession() droptables = ";
                dropTables();
            }
            createTables();
            std::cerr << "Created database: hitcounterman " << std::endl;
            t.commit();
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "Using existing hitcounterman database";
            Wt::log("notice") << " *** HitCounterManSession::HitCounterManSession()  Using existing hitcounterman database ";
        }
    }
    Update();
    //Wt::log("end") << " *** HitCounterManSession::HitCounterManSession() *** ";
} // end HitCounterManSession
/* ****************************************************************************
 * Update
 */
void HitCounterManSession::Update()
{
    // Create an instance of app to access Internal Paths
    Wt::WApplication* app = Wt::WApplication::instance();
    //Wt::log("start") << " *** HitCounterManSession::Update()  *** Path = " << app->internalPath();
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(*this);
        // Create a new Video Instance
        Wt::Dbo::ptr<HitCounterMan> thisCounter = add(new HitCounterMan());
        // Set object to Modify
        HitCounterMan *counterDb = thisCounter.modify();
        // IP Address
        counterDb->ipaddress = app->environment().clientAddress(); // Note for haproxy use: Make sure behind-reverse-proxy is set true in wt_config.xml
        // page
        counterDb->page = app->internalPath();
        // date
        counterDb->date = Wt::WDateTime::currentDateTime();;
        // Commit Transaction
        t.commit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "HitImpl::Update: Failed writting to hitcounterman database";
        Wt::log("error") << "-> HitCounterManSession::Update()  Failed writting to hitcounterman database)";
    }
    //Wt::log("end") << "HitCounterManSession::Update() app->environment().clientAddress()=" << app->environment().clientAddress();
} // end Update
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
