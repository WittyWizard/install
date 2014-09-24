/* ****************************************************************************
 * Poor mans Plugin System
 * I want to a good Plugin System, this works, but its not what I want
 *
 * Module Name is derived from the Internal Path, which starts out as language (en, cn, ru...)
 * so blogman, chatman, videoman, and menuman are all Module Names.
 *
 * Subpaths are different for every Module, so each module must return the minimal and maximum number of paths (seperated as /)
 *
 *
 */
#include "WittyWizard.h"
/* ****************************************************************************
 * Call Menu Plugin
 */
void WittyWizard::CallMenuPlugin()
{
    // FIXME: Make sure you can completly remove any module, make this a plugin
    #ifdef BLOGMAN
        if (theIncludes.find("|blog|") != std::string::npos)
        {
            mainMenu_->addItem(Wt::WString::tr("blog"),  deferCreate(boost::bind(&WittyWizard::Blog, this))); // http://localhost:8088/?_=/blog or http://localhost:8088/blog
        }
    #endif // BLOGMAN
    #ifdef CHATMAN
        if (theIncludes.find("|chat|") != std::string::npos)
        {
            mainMenu_->addItem(Wt::WString::tr("chat"),  deferCreate(boost::bind(&WittyWizard::Chat, this))); //
        }
    #endif // CHATMAN
} // end CallMenuPlugin
/* ****************************************************************************
 * Call Plugin
 */
void WittyWizard::CallPlugin()
{
    #ifdef HITCOUNTERMAN
        if (theIncludes.find("|HitCounterMan|") != std::string::npos)
        {
            homeTemplate->bindWidget("hitcounterman", HitCounterMan());
        }
    #else
        homeTemplate_->bindWidget("hitcounterman", new Wt::WText(""));
    #endif // HITCOUNTERMAN
}
/* ****************************************************************************
 * Plugin Handle Path Change
 * return true to continue, false to return because Plugin will handle call
 */
bool WittyWizard::PluginHandlePathChange(std::string moduleName, int newLanguage)
{
    (void)moduleName;
    (void)newLanguage;
    return true;
} // end PluginHandlePathChange
/* ****************************************************************************
 * Call Plugin Set Language
 */
void WittyWizard::CallPluginSetLanguage(std::string moduleName, std::string languageName, int newLanguage)
{
    (void)moduleName;
    (void)languageName;
    (void)newLanguage;
    #ifdef BLOGMAN
    BlogView *blog = dynamic_cast<BlogView *>(findWidget("blog"));
    if (blog)
    {
        if (moduleName == "blog")
        {
            if (language_ != newLanguage)
            {
                Wt::log("notice") << "WittyWizard::InternalPathChange() for blog " << languageName <<  " | newPath = " << newPath;
                blog->SetInternalBasePath("/" + languageName + "/blog/");
            }
        }
    }
    #endif // BLOGMAN
} // end CallPluginSetLanguage
#ifdef HITCOUNTERMAN
/* ****************************************************************************
 * Hit Counter Manager
 */
Wt::WWidget* WittyWizard::HitCounterMan()
{
    HitCounterManView* hitCounterMan = new HitCounterManView(*dbConnection, languages[GetDefaultLanguage()].code_);
    hitCounterMan->setObjectName("hitcounterman");
    //
    return hitCounterMan->Update();
} // end HitCounterMan
#endif // HITCOUNTERMAN
// --- End Of File ------------------------------------------------------------
