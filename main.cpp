/* ****************************************************************************
 * Witty Wizard
 */
#include <Wt/WServer>
#include "WittyWizard.h"
/* ****************************************************************************
 * create Witty Wizard Application
 */
Wt::WApplication* createWittyWizardApplication(const Wt::WEnvironment& env)
{
    return new WittyWizard(env);
}
/* ****************************************************************************
 * main
 */
int main(int argc, char **argv)
{
    try
    {
        Wt::WServer server(argv[0]);
        //
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        //
        server.addEntryPoint(Wt::Application, boost::bind(&createWittyWizardApplication,  _1), "", "favicon.ico");
        // Start the Server
        if (server.start())
        {
            Wt::WServer::waitForShutdown();
            server.stop();
        }
    }
    catch (Wt::WServer::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
} // end main
// --- End Of File ------------------------------------------------------------
