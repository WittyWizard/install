/* ****************************************************************************
 * Poor mans Plugin System
 */
#include "WittyWizard.h"
/* ****************************************************************************
 * Call Menu Plugin
 */
void WittyWizard::CallMenuPlugin()
{
    // FIXIT: Make sure you can completly remove any module, make this a plugin
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
    #ifdef VIDEOMAN
        if (theIncludes.find("|VideoMan|") != std::string::npos)
        {
            mainMenu_->addItem(Wt::WString::tr("VideoMan").toUTF8(), deferCreate(boost::bind(&WittyWizard::VideoMan, this)));
        }
        // FIXIT add Menu Options for all videos
    #endif // VIDEOMAN
} // end CallMenuPlugin
/* ****************************************************************************
 * Call Plugin
 */
void WittyWizard::CallPlugin()
{
    #ifdef HITCOUNTERMAN
        if (theIncludes.find("|HitManCounter|") != std::string::npos)
        {
            homeTemplate_->bindWidget("hitcounterman", HitCounterMan());
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
    #ifdef VIDEOMAN
    VideoManView *video;
    if (moduleName == Wt::WString::tr("VideoManPath"))
    {
        video = dynamic_cast<VideoManView *>(findWidget("videoman"));
        if (video)
        {
            if (language_ == newLanguage)
            {
                Wt::log("notice") << " <<<<<<< WittyWizard::InternalPathChange() menu is video do return.";
                isPathChanging = false; // Set isPathChanging so InternalPathChange will fire
                // FIXIT add a callback from video to update hit counter
                #ifdef HITCOUNTERMAN
                    if (theIncludes.find("|HitManCounter|") != std::string::npos)
                    {
                        homeTemplate_->bindWidget("hitcounterman", HitCounterMan());
                    }
                #else
                    homeTemplate_->bindWidget("hitcounterman", new Wt::WText(""));
                #endif // HITCOUNTERMAN
                return false; // we do not want to handle changes: FIXIT find a way to make this generic
                // maybe make a call to video handle path change, return true to continue, false to return without handling change
            }
        }
    }
    #endif
    return true;
} // end PluginHandlePathChange
/* ****************************************************************************
 * Call Plugin Set Language
 */
void WittyWizard::CallPluginSetLanguage(std::string moduleName, std::string languageName, int newLanguage)
{
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
    #ifdef VIDEOMAN
    VideoManView *video = dynamic_cast<VideoManView *>(findWidget("video"));
    if (video)
    {
        if (moduleName == Wt::WString::tr("VideoManPath"))
        {
            if (language_ != newLanguage)
            {
                Wt::log("notice") << "WittyWizard::InternalPathChange() for videoman " << languageName;
                video->SetInternalBasePath("/" + languageName + "/" + Wt::WString::tr("VideoManPath").toUTF8() + "/");
            }
        }
    }
    #endif
} // end CallPluginSetLanguage
#ifdef HITCOUNTERMAN
/* ****************************************************************************
 * Hit Counter Manager
 */
Wt::WWidget* WittyWizard::HitCounterMan()
{
    HitCounterManView* hitManCounter = new HitCounterManView(*dbConnection_, languages[GetDefaultLanguage()].code_);
    hitManCounter->setObjectName("hitcounterman");
    //
    return hitManCounter->Update();
} // end HitCounterMan
#endif // HITCOUNTERMAN
#ifdef VIDEOMAN
/* ****************************************************************************
 * Video Manager
 * FIXIT: this needs to be done with a software plug in, this is for testing only
 * Given the current state with plugins, this is a long term project
 * myAppRoot: Path to video.xml
 * Internal Path
 * debase Connection
 */
Wt::WWidget* WittyWizard::VideoMan()
{
    //
    Wt::log("start") << " *** WittyWizard::VideoMan() internalPath = " << internalPath();
    const Lang& l = languages[language_];
    std::string langPath = l.name_;
    std::string theVideoManPath = Wt::WString::tr("VideoMan").toUTF8();
    boost::to_lower(theVideoManPath);
    VideoManView* thisVideo = new VideoManView(appRoot() + "home/" + domainName + "/videoman/", "/" + langPath + "/" + theVideoManPath + "/", *dbConnection_, l.name_, Crystallball::UseDb[domainName], domainName);
    thisVideo->setObjectName("videoman");
    Wt::log("end") << " *** WittyWizard::VideoMan()";
    return thisVideo;
} // end VideoMan
#endif // VIDEOMAN
