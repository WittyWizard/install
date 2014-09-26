/* ****************************************************************************
 * Witty Wizard
 */
#include "WittyWizard.h"
/* ****************************************************************************
 * Witty Wizard Contructor
 */
WittyWizard::WittyWizard(const Wt::WEnvironment& env) : Wt::WApplication(env)
{
    bool showDebug = false;
    if (showDebug) { Wt::log("start") << " *** WittyWizard::WittyWizard() env.hostName() = " << env.hostName().c_str() << " *** "; }
    myHost = env.hostName().c_str();       // localhost:8088
    myUrlScheme = env.urlScheme().c_str(); // http or https
    myBaseUrl = myUrlScheme + "://" + myHost + "/"; // FIXME
    domainName = env.hostName().c_str();
    unsigned pos = domainName.find(":");
    if (pos > 0)
    {
        domainName = domainName.substr(0, pos);
    }
    // this is just for testing multiple sites in a localhost nated network
    if (0) domainName = "wittywizard.org";
    //
    const std::string jqueryMin = docRoot() + "/resources/js/jquery.js"; // resourcesUrl()
    if (CrystalBall::IsFile(jqueryMin))
    {
        if (showDebug) { Wt::log("info") << "**** WittyWizard::WittyWizard() Using Jquery file: " <<  jqueryMin << " ****"; }
        //Wt::WApplication::instance()->requireJQuery(jqueryMin);
    }
    else
    {
        Wt::log("error") << "**** WittyWizard::WittyWizard() missing file: " <<  jqueryMin << "****";
    }
    //
    // Note: this is in the root with executable
    // ./app_root/domains.xml
    if (!CrystalBall::GetConnectionPoolInfo(appRoot().append("domains.xml").c_str(), domainName))
    {
        Wt::log("error") << " *** WittyWizard::WittyWizard() domains.xml file not found:" << appRoot().append("domains.xml").c_str();
        return; // Fix 404
    }
    // Set Global Variables
    if (!CrystalBall::SetSqlConnectionPool(domainName))
    {
        Wt::log("error") << "(WittyWizard::WittyWizard: SetSqlConnectionPool failed for domain: " << domainName << ")";
        // FIXME make this a error page
        return;
    }
    // Connect to Connection Pool
    if(CrystalBall::ConnectionPool.find(domainName) == CrystalBall::ConnectionPool.end())
    {
        // element not found;
        Wt::log("error") << "WittyWizard::WittyWizard() myConnectionPool element not found " << domainName << "";
        return; // FIXME make and error page
    }
    try
    {
        dbConnection = boost::any_cast<Wt::Dbo::SqlConnectionPool *>(CrystalBall::ConnectionPool[domainName]);
    }
    catch (...)
    {
        Wt::log("error") << "->>> WittyWizard::WittyWizard() Failed Connection Pool " << domainName << "<<<-";
        return; // FIXME make and error page
    }
    //
    if (!CrystalBall::IsFile(appRoot() + "home/" + domainName + "/WittyWizard.xml"))
    {
        Wt::log("error") << "WittyWizard::WittyWizard() missing file: " <<  appRoot() + "home/" + domainName + "/WittyWizard.xml";
    }
    messageResourceBundle().use(appRoot() + "home/" + domainName + "/WittyWizard", false);
    // Fix if it should use local copy of themes
    useStyleSheet(Wt::WApplication::resourcesUrl() + "css/wittywizard.css");
    useStyleSheet(Wt::WApplication::resourcesUrl() + "css/wittywizard_ie.css", "lt IE 7"); // "." +
    // Debug to see what path is returned in settings
    if (0)
    {
        if (showDebug) { Wt::log("notice") << "WittyWizard::WittyWizard: (resourcesUrl(): " << resourcesUrl() << ")"; } // /resources/
        if (showDebug) { Wt::log("notice") << "WittyWizard::WittyWizard: (appRoot(): " << appRoot() << ")";           } // ./app_root/
        if (showDebug) { Wt::log("notice") << "WittyWizard::WittyWizard: (docRoot(): " << docRoot() << ")";           } // ./doc_root
    }
    //useStyleSheet("css/chatwidget.css");
    //useStyleSheet("css/chatwidget_ie6.css", "lt IE 7");
    //setCssTheme("polished");
    // Theme: Not to be confused with wittywizard theme
    #ifdef THEME3
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme();
        bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
        setTheme(bootstrapTheme);
        // load the default bootstrap3 (sub-)theme
        // Fix if it should use local copy of themes
        //useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/3/bootstrap-theme.min.css");
    #elif THEME2
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme();
        bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version2);
        setTheme(bootstrapTheme);
        // load the default bootstrap2 (sub-)theme
        // Fix if it should use local copy of themes
        useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/2/bootstrap-theme.min.css");
    #else
        setTheme(new Wt::WBootstrapTheme());
    #endif
    // Fix if it should use local copy of themes, std::string cssPath = "";
    useStyleSheet(Wt::WApplication::resourcesUrl() + "style/everywidget.css");
    useStyleSheet(Wt::WApplication::resourcesUrl() + "style/dragdrop.css");
    useStyleSheet(Wt::WApplication::resourcesUrl() + "style/combostyle.css");
    useStyleSheet(Wt::WApplication::resourcesUrl() + "style/pygments.css");
    // Set Title
    // Fix needs to be per page
    //setTitle(MyCmsDomain.MyTitle);
    // Set Locale
    setLocale("");
    // Add Language: Do not forget to change WittyWizard::DefaultLanguageIndex
    //               name, code,    shortDescription,             longDescription
    // FIXME: Make all Languages
    std::string myLanguages = CrystalBall::Languages[domainName];
    if (myLanguages.find("|en|") != std::string::npos)
        { AddLanguage(Lang("en", "en_US", "en",  "English")); }
    if (myLanguages.find("|cn|") != std::string::npos)
        { AddLanguage(Lang("cn", "zh_CN", Wt::WString::fromUTF8("汉语"), Wt::WString::fromUTF8("中文 (Chinese)"))); }
    if (myLanguages.find("|ru|") != std::string::npos)
        { AddLanguage(Lang("ru", "ru_RU", "ру",  Wt::WString::fromUTF8("Русский (Russian)"))); }
    if (myLanguages.find("|de|") != std::string::npos)
        { AddLanguage(Lang("de", "de_DE", "von", "Keim (Germen)")); }
    // After you add all your Languages, Set Locale
    myLocale = env.locale().name().c_str();
    SetMyLocale();
    // Add Theme
    std::string myThemes = CrystalBall::Themes[domainName];
    if (myThemes.find("|red|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("red")   , "red")); }
    if (myThemes.find("|white|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("white") , "white")); }
    if (myThemes.find("|blue|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("blue")  , "blue")); }
    if (myThemes.find("|green|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("green") , "green")); }
    if (myThemes.find("|tan|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("tan")   , "tan")); }
    if (myThemes.find("|default|") != std::string::npos)
        { AddTheme(Theme(Wt::WString::tr("default"), "default")); }
    // Listen for path change
    internalPathChanged().connect(this, &WittyWizard::InternalPathChange);
    // Make Path change manually to call ReInit
    InternalPathChange(internalPath());
    if (showDebug) { Wt::log("end") << "WittyWizard::WittyWizard()"; }
} // WittyWizard
/* ****************************************************************************
 * Internal Path Change
 */
void WittyWizard::InternalPathChange(const std::string& thePath)
{
    bool showDebug = false;
    if (isPathChanging)
    {
        if (showDebug) { Wt::log("restart") << " ~~~~ WittyWizard::InternalPathChange() returning nothing done ~~~~ "; }
        return;
    } // end if (isPathChanging)
    //
    int oldLanguageIndex = myLanguageIndex;
    std::string moduleName = "";
    myLanguage = internalPathNextPart("/"); // Checks First Argument
    if (IsPathLanguage(myLanguage) == -1)
    {
        myLanguageIndex = GetDefaultLanguage();
    }
    else // if (IsPathLanguage(myLanguage) != -1)
    {
        if (!myLanguage.empty())
            { moduleName = internalPathNextPart(myLanguage + "/"); }  // Checks Second Argument
    } // if (IsPathLanguage(myLanguage) == -1)
    myLanguage = GetLanguageName(); // en, cn, ru...
    //
    myPath = CrystalBall::GetPath(internalPath(), GetLanguageName());
    // Get a Valid Language, returns default if not found
    std::string newPath = "/" + myLanguage + "/" + myPath;

    if (showDebug) { Wt::log("start") << " WittyWizard::InternalPathChange() myLanguage = " << myLanguage << " | myLanguageIndex = " << oldLanguageIndex << " | new Language Index = " << myLanguageIndex; }

    //
    isPathChanging = true;
    PluginHandlePathChange(moduleName, myLanguageIndex);
    if (myLanguageIndex != oldLanguageIndex)
    {
        if (showDebug) { Wt::log("local") << " *** WittyWizard::InternalPathChange() language Code set to = " << myLanguage << " myLanguage = " << myLanguage << " | thePath = " << thePath << " | newPath = " << newPath << " *** "; }
        // Set Local
        setLocale(myLanguage);
        //setLocale(theLanguage.code_);
    }
    //
    CallPluginSetLanguage(moduleName, myLanguage, myLanguageIndex);
    //
    if (myLanguageIndex != oldLanguageIndex)
    {
        // FIXME: do I need to see if lanuage changed?
        // Change Menu Base Path
        //mainMenu_->setInternalBasePath(myLanguage);
    } // end if (myLanguageIndex != oldLanguageIndex)
    // FIXME: do I need to test thePath == newPath
    // Change Path
    Wt::WApplication::instance()->setInternalPath(newPath, true);
    // Set Theme
    SetWizardTheme(true, 0);
    //
    ReInit();
    if (showDebug) { Wt::log("end") << "WittyWizard::InternalPathChange()"; }
    isPathChanging = false; // Set isPathChanging so InternalPathChange will fire
} // end InternalPathChange
/* ****************************************************************************
 * Re init
 */
void WittyWizard::ReInit()
{
    bool showDebug = false;
    if (showDebug) { Wt::log("start") << " *** WittyWizard::ReInit() *** "; }
    // Set Base URL
    SetBaseURL();
    // Clear the Screen
    root()->clear();
    // Create Home Page, we just cleared it
    CreateMenu();
    //
    CallPlugin();
    // do meta data
    std::string myMetaData = CrystalBall::GoogleSiteVerifi[domainName];
    if (!myMetaData.empty())
        { addMetaHeader(Wt::MetaName, "google-site-verification", myMetaData, myLanguage); }
    myMetaData = CrystalBall::Ykey[domainName];
    if (!myMetaData.empty())
        { addMetaHeader(Wt::MetaName, "y_key", myMetaData, myLanguage); }
    myMetaData = CrystalBall::MsValidate[domainName];
    if (!myMetaData.empty())
        { addMetaHeader(Wt::MetaName, "msvalidate.01", myMetaData, myLanguage); }
    MenuManView* thisMenu = new MenuManView(appRoot() + "home/" + domainName + "/menuman/", *dbConnection, myLanguage, CrystalBall::UseDb[domainName], domainName, Wt::Horizontal, internalPath());
    MetaData* metaData = thisMenu->GetMetaData(CrystalBall::GetPath(internalPath(), GetLanguageName()));
    if (metaData == NULL)
    {
        if (showDebug) { Wt::log("notice") << " <<<<<<<<<<<<<<< WittyWizard::ReInit() internalPath = " << internalPath() << " path = " << path  << " | metaData == NULL >>>>>>>>>>>>>>>>>>>>> "; }
    }
    else
    {
        if (showDebug) { Wt::log("notice") << " <<<<<<<<<<<<<<< WittyWizard::ReInit() internalPath = " << internalPath() << " path = " << path  << " >>>>>>>>>>>>>>>>>>>>> "; }
        if (!metaData->title.empty())
        {
            if (mainMenu_->currentItem())
                { setTitle(mainMenu_->currentItem()->text() + " - " + metaData->title); }
        }
        if (!metaData->description.empty())
        {
            if (showDebug) { Wt::log("notice") << " <<<<<<<<<<<<<<< WittyWizard::ReInit() metaData->description = " << metaData->description << " path = " << path  << " >>>>>>>>>>>>>>>>>>>>> "; }
            addMetaHeader(Wt::MetaName, "description", metaData->description, myLanguage);
        }
        if (!metaData->keywords.empty())
            { addMetaHeader(Wt::MetaName, "keywords", metaData->keywords, myLanguage); }
        if (!metaData->rating.empty())
            { addMetaHeader(Wt::MetaName, "rating", metaData->rating, myLanguage); }
    }
} // end ReInit()
/* ****************************************************************************
 * Create Menu
 */
void WittyWizard::CreateMenu()
{
    bool showDebug = true;
    if (showDebug) { Wt::log("start") << "CreateMenu : thePath = ->" << myPath << "<-"; }
    //
    // This is our template
    homeTemplate = new Wt::WTemplate(Wt::WString::tr("ww-template"), root()); //  <message id="template">
    // Create a Container
    container = new Wt::WContainerWidget();
    // Create the stack where the contents will be located
    Wt::WStackedWidget* contents = new Wt::WStackedWidget();
    contents->setStyleClass("content");
    // FIXME
    // "|chat|blog|video|"
    theIncludes = CrystalBall::ModuleIncludes[domainName];
    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome:  theIncludes=" << theIncludes; }
    // create a top-level menu
    // Setup a Left-aligned menu.
    mainMenu_ = new Wt::WMenu(contents, container);
    mainMenu_->setStyleClass("sfhmenu");
    //mainMenu_->removeStyleClass("nav", true); // top level li of sfhmenu with children
    // ------------------------------------------------------------------------
    // query menu: lang,
    // FIXME: I do not like hard coding directories
    // FIXME: how do I know domainName is Clean
    MenuManView* thisMenu = new MenuManView(appRoot() + "home/" + domainName + "/menuman/", *dbConnection, myLanguage, CrystalBall::UseDb[domainName], domainName, Wt::Horizontal, internalPath());
    // FIXME: The Menu Session Crash was not from GetMenus using db or xml
#define DO_MENU_MAN
#ifdef DO_MENU_MAN
    thisMenu->GetMenus();
#else
    /*
<?xml version="1.0" encoding="UTF-8" ?>
<menusman>
    <menuman name="Home"       path="/home"                    type=""        parent=""               grandparent=""          language="en" content="home" description="Witty Wizard Content Management System (CMS)" keywords="Witty,Wizard,Content,Management,System,CMS,C++,Wt, Witty Wizard,Content Management System,Document,PDF,Video,Audio,Widget,Qt" title="The Witty Wizard Content Management System (CMS)" copyright="Witty Wizard" rating="general"></menuman>
    <menuman name="Features"   path="/features"                type="topsub"  parent="/features"      grandparent=""          language="en" content="" description="" keywords="" title="" copyright="" rating="general"></menuman>
    <menuman name="Themes"     path="/features/theme"          type="submenu" parent="/features"      grandparent=""          language="en" content="features/theme" description="Theme Manager" keywords="Witty,Wizard,Content,Management,System,CMS,C++,Wt, Witty Wizard,Content Management System,Document,PDF,Video,Audio,Widget,Qt,Menu,Manager" title="Witty Wizard Theme Manager" copyright="Witty Wizard" rating="general"></menuman>
    <menuman name="Menu"       path="/features/menu"           type="subsub"  parent="/features/menu" grandparent="/features" language="en" content="" description="" keywords="" title="" copyright="" rating="general"></menuman>
    <menuman name="Sub Menu"   path="/features/menu"           type="subsub"  parent="/features/menu" grandparent="/features" language="en" content="" description="" keywords="" title="" copyright="" rating="general"></menuman>
    <menuman name="Sub Menu 1" path="/features/menu/sub-menu1" type="submenu" parent="/features/menu" grandparent="/features" language="en" content="features/menu/sub-menu1" description="Sub Menu 1" keywords="Witty,Wizard,Content,Management,System,CMS" title="Sub Menu 1" copyright="Light Wizzard" rating="general"></menuman>
    <menuman name="Sub Menu"   path="/features/menu"           type="subend"  parent="/features/menu" grandparent="/features" language="en" content="" description="" keywords="" title="" copyright="" rating=""></menuman>
    <menuman name="Menu"       path="/features/menu"           type="subend"  parent="/features/menu" grandparent="/features" language="en" content="" description="" keywords="" title="" copyright="" rating=""></menuman>
    <menuman name="Features"   path="/features"                type="topend"  parent="/features"      grandparent=""          language="en" content="" description="" keywords="" title="" copyright="" rating=""></menuman>
    <menuman name="About"      path="/about"                   type=""        parent=""               grandparent=""          language="en" content="about" description="Witty Wizard Content Management System (CMS)" keywords="Witty,Wizard,Content,Management,System,CMS,C++,Wt, Witty Wizard,Content Management System,Document,PDF,Video,Audio,Widget,Qt" title="About Witty Wizard Content Management System (CMS)" copyright="Witty Wizard" rating="general"></menuman>
</menusman>
     */
    // name_, path_, type_, parent_, grandparent_, isSelected_;
    thisMenu->AddMenu(MasterMenu("home"       , "/home"                   , ""       , ""               , ""         , false)); // myMainMenu
    thisMenu->AddMenu(MasterMenu("Features"   , "/features"               , "topsub" , "/features"      , ""         , false)); // myMainMenu
    thisMenu->AddMenu(MasterMenu("Themes"     , "/features/theme"         , "submenu", "/features"      , ""         , false)); // myPopupMenu[l.path_], myMenuItem[l.path_]
    thisMenu->AddMenu(MasterMenu("Menu"       , "/features/menu"          , "subsub" , "/features/menu" , "/features", false)); // myMenuItem[l.path_]
    thisMenu->AddMenu(MasterMenu("Sub Menu"   , "/features/menu"          , "subsub" , "/features/menu" , "/features", false)); // myMenuItem[l.path_]
    thisMenu->AddMenu(MasterMenu("Sub Menu 1" , "/features/menu/sub-menu1", "submenu", "/features/menu" , "/features", false)); // myPopupMenu[l.path_]
    thisMenu->AddMenu(MasterMenu("Sub Menu"   , "/features/menu"          , "subend" , "/features/menu" , "/features", false)); // myMenuItem[l.path_]
    thisMenu->AddMenu(MasterMenu("Menu"       , "/features/menu"          , "subend" , "/features/menu" , "/features", false)); // myMenuItem[l.path_]
    thisMenu->AddMenu(MasterMenu("Features"   , "/features"               , "topend" , "/features"      , ""         , false)); // myMainMenu
    thisMenu->AddMenu(MasterMenu("About"      , "/about"                  , ""       , ""               , ""         , false)); // myMainMenu->addItem(myPopupMenu[l.path_]->setSubMenu(myMenuItem[l.path_]));
#endif
    // FIXME: This Menu system is causeing the Theme not to work, click on Options -> Theme, change, then refresh page, if it remembers the theme, then it works
#define MENU
#ifdef MENU
    std::vector<std::string> parts;
    boost::split(parts, myPath, boost::is_any_of("/"));
    // FIXME: Add Authentication
    if (myPath == "/admin/menuman/updatexml")
    {
        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome:  Admin Mode"; }
    }
    // This tracks menu items
    std::map <std::string, int> menuItemCounter;
    std::map <std::string, Wt::WSubMenuItem *> myPopupMenu;
    std::map <std::string, Wt::WMenu *> myMenuItem;
    std::string selectedMenuItem = "";
    //
    for (unsigned i = 0; i < parts.size(); i++)
        { if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: parts size = " << parts.size() << " | parts[" << i << "]=" << parts[i]; } }
    // Clear Selected
    for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
        { thisMenu->mastermenus.at(i).isSelected_ = false; }
    // Set Selected Menu Item
    for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
    {
        MasterMenu& menu = thisMenu->mastermenus[i];
        if (menu.type_ == "submenu" || menu.type_ == "")
        {
            if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: submenu: thePath = " << myPath << " ==  menu.path_ = " << menu.path_; }
            if (myPath == menu.path_)
            {
                if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ submenu = " << menu.path_; }
                selectedMenuItem = menu.path_;
                thisMenu->mastermenus.at(i).isSelected_ = true;
                break;
            } // end if (myPath == menu.path_)
        } // end if (menu.type_ == "submenu" || menu.type_ == "")
    } // end for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
    // Set all Popups Selected in path
    if (!selectedMenuItem.empty())
    {
        for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
        {
            MasterMenu& menu = thisMenu->mastermenus[i];
            if (menu.type_ == "")
            {
                if (std::find(parts.begin(), parts.end(), menu.path_) != parts.end())
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ = " << menu.name_; }
                    thisMenu->mastermenus.at(i).isSelected_ = true;
                }
            } // end if (menu.type_ == "")
            if (menu.type_ == "topsub")
            {
                if (std::find(parts.begin(), parts.end(), menu.path_) != parts.end())
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: topsub isSelected_ = " << menu.name_; }
                    thisMenu->mastermenus.at(i).isSelected_ = true;
                }
            } // end if (menu.type_ == "topsub")
            if (menu.type_ == "subsub")
            {
                if (std::find(parts.begin(), parts.end(), menu.path_) != parts.end())
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: subsub isSelected_ = " << menu.name_; }
                    thisMenu->mastermenus.at(i).isSelected_ = true;
                }
            } // end if (menu.type_ == "subsub")
            if (menu.type_ == "submenu")
            {
                if (myPath == menu.path_)
                {
                    if (std::find(parts.begin(), parts.end(), menu.path_) != parts.end())
                    {
                        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: submenu isSelected_ = " << menu.name_; }
                        thisMenu->mastermenus.at(i).isSelected_ = true;
                    }
                    break;
                } // end if (myPath == menu.path_)
            } // end if (menu.type_ == "submenu")
        } // end for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
    } // end if (!selectedMenuItem.empty())
    // Now lets Build the Menu
    for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
    {
        MasterMenu& menu = thisMenu->mastermenus[i];
        if (menu.type_ == "")
        {
            // Add to Main Menu
            // Note: I could not move this section into MenuManView due to boost bind done here
            mainMenu_->addItem(menu.name_, deferCreate(boost::bind(&WittyWizard::MenuMan, this)))->setLink(Wt::WLink(Wt::WLink::InternalPath, "/" + GetLanguageName() + menu.path_));
            if (menu.isSelected_)
            {
                if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ myMainMenu->addItem(" << menu.name_; }
                mainMenu_->itemAt(menuItemCounter["rootmenu"]++)->setStyleClass("selectedmm"); // top level li of sfhmenu selected
            }
            else
            {
                if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: myMainMenu->addItem(" << menu.name_; }
                mainMenu_->itemAt(menuItemCounter["rootmenu"]++)->setStyleClass("mainmenumm"); // top level li of sfhmenu
            } // end if (menu.isSelected_)
        }
        else // if (menu.type_ != "")
        {
            if (menu.type_ == "topsub")
            {
                if(myPopupMenu.find(menu.path_) == myPopupMenu.end())
                {
                    myPopupMenu[menu.path_] = new Wt::WSubMenuItem(menu.name_); // , "/" + l.path_ , container
                    //myPopupMenu[menu.path_]->removeStyleClass("dropdown", true); // top level li of sfhmenu with children
                    if (menu.isSelected_)
                    {
                        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ topsub myPopupMenu[" << menu.path_ << "] |  new WSubMenuItem(" << menu.name_; }
                        myPopupMenu[menu.path_]->setStyleClass("activemmwc"); // top level li of sfhmenu with children selected
                    }
                    else
                    {
                        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: topsub myPopupMenu[" << menu.path_ << "] |  new WSubMenuItem(" << menu.name_; }
                        myPopupMenu[menu.path_]->setStyleClass("mmwc"); // top level li of sfhmenu with children
                    } // end if (menu.isSelected_)
                    menuItemCounter["rootmenu"]++;
                } // end if(myPopupMenu.find(menu.path_) == myPopupMenu.end())
                if(myMenuItem.find(menu.path_) == myMenuItem.end())
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: topsub new WMenu myMenuItem[" << menu.path_; }
                    myMenuItem[menu.path_] = new Wt::WMenu(contents, container);
                    myMenuItem[menu.path_]->setStyleClass("sfhmenu"); // top level ul under sfhmenu, container
                } // end if(myMenuItem.find(menu.path_) == myMenuItem.end())
            } // end if (l.type_ == "topsub")
            if (menu.type_ == "subsub")
            {
                if(myPopupMenu.find(menu.parent_) == myPopupMenu.end())
                {
                    myPopupMenu[menu.parent_] = new Wt::WSubMenuItem(menu.name_); // , "/" + l.path_ , container
                    if (menu.isSelected_)
                    {
                        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ subsub myPopupMenu[" << menu.parent_ << "] |  new WSubMenuItem(" << menu.name_; }
                        myPopupMenu[menu.parent_]->setStyleClass("selectedsm"); // sub level li of sfhmenu selected
                    }
                    else
                    {
                        if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: subsub myPopupMenu[" << menu.parent_ << "] |  new WSubMenuItem(" << menu.name_; }
                        myPopupMenu[menu.parent_]->setStyleClass("smwc"); // sub level li of sfhmenu
                    } // end if (menu.isSelected_)
                } // end if(myPopupMenu.find(menu.parent_) == myPopupMenu.end())
                if(myMenuItem.find(menu.path_) == myMenuItem.end())
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: subsub new WMenu myMenuItem[" << menu.parent_; }
                    myMenuItem[menu.parent_] = new Wt::WMenu(contents, container);
                    myMenuItem[menu.parent_]->setStyleClass("sfhmenu"); // sub level ul of sfhmenu
                } // end if(myMenuItem.find(menu.path_) == myMenuItem.end())
            } // end if (l.type_ == "subsub")
            if (menu.type_ == "submenu")
            {
                myMenuItem[menu.parent_]->addItem(menu.name_, deferCreate(boost::bind(&WittyWizard::MenuMan, this)))->setLink(Wt::WLink(Wt::WLink::InternalPath, "/" + GetLanguageName() + menu.path_));
                if (menu.isSelected_)
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: isSelected_ submenu myMenuItem[" << menu.parent_ << "]->addItem(" << menu.name_; }
                    myMenuItem[menu.parent_]->itemAt(menuItemCounter[menu.parent_]++)->setStyleClass("selectedsm"); // li of sfhmenu
                }
                else
                {
                    if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: submenu myMenuItem[" << menu.parent_ << "]->addItem(" << menu.name_; }
                    myMenuItem[menu.parent_]->itemAt(menuItemCounter[menu.parent_]++)->setStyleClass("submenumm"); // li of sfhmenu
                } // end if (menu.isSelected_)
                //myMenuItem.erase(menu.parent_);
            } // end if (l.type_ == "submenu")
            if (menu.type_ == "topend")
            {
                if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: topend addItem myMainMenu->addItem(myPopupMenu[" << menu.parent_ << "]->setSubMenu(myMenuItem[" << menu.parent_ << "]"; }
                myPopupMenu[menu.parent_]->setSubMenu(myMenuItem[menu.parent_]);
                mainMenu_->addItem(myPopupMenu[menu.parent_]);
                //myPopupMenu.erase(menu.parent_);
            } // end if (l.type_ == "topend")
        } // end if (l.type_ != "")
        if (menu.type_ == "subend")
        {
            if (showDebug) { Wt::log("notice") << "WittyWizard::CreateHome: subend myMainMenu->addItem(myMenuItem[" << menu.grandparent_ << "]->addItem(myPopupMenu[" << menu.parent_ << "])"; }
            myPopupMenu[menu.parent_]->setSubMenu(myMenuItem[menu.parent_]);
            myMenuItem[menu.grandparent_]->addItem(myPopupMenu[menu.parent_]);
            mainMenu_->addItem(myPopupMenu[menu.grandparent_]);
        } // end if (l.type_ == "subend")
    } // end for (unsigned i = 0; i < thisMenu->mastermenus.size(); i++)
#else
    mainMenu_ = new Wt::WMenu(contents, container);
    mainMenu_->setStyleClass("sfhmenu");
    //mainMenu_->removeStyleClass("nav", true); // top level li of sfhmenu with children
#endif
    // ------------------------------------------------------------------------
    /*
    // FIXME: make search work
    Wt::WText *searchResult = new Wt::WText(Wt::WString::tr("search"));
    //
    myMainMenu->addItem("Search", searchResult);
    // Add a Search control.
    Wt::WLineEdit *searchText = new Wt::WLineEdit();
    searchText->setEmptyText(Wt::WString::tr("search"));
    searchText->enterPressed().connect(std::bind([=] ()
    {
        // FIXME add a real search feature
        mainMenu_->select(4); // is the index a random menu item
        searchResult->setText(Wt::WString("Nothing found for {1}.").arg(searchText->text()));
    }));
    myMainMenu->addSearch(searchText, Wt::AlignRight);
    */
    // Setup a Right-aligned menu.
    Wt::WMenu* rightMenu = new Wt::WMenu(contents, container);
    //rightMenu->setId("rightmenu");
    rightMenu->setStyleClass("popright"); // ul
    // Create a Language popup submenu for the Language Menu.
    Wt::WSubMenuItem* languagePopup = new Wt::WSubMenuItem(Wt::WString::tr("language"));
    languagePopup->setStyleClass("popright");
    Wt::WMenu* langItems = new Wt::WMenu(contents, container);
    langItems->setStyleClass("poprightsub"); // ul
    for (unsigned i = 0; i < languages.size(); ++i)
    {
        // Get Language
        const Lang& l = languages[i];
        // Add Popup Item with Description.
        // FIXME: [warning] "WString: narrow(): loss of detail: ?? (Chinese)"
        Wt::WMenuItem* mi = langItems->addItem(l.longDescription_);
        // FIXME: [error] "WString: widen(): could not widen string: 中文 (Chinese)"
        // Wt::WMenuItem* mi = langItems->addItem(l.longDescription_.toUTF8());
        // FIXME: [warning] "WString: narrow(): loss of detail: ?? (Chinese)"
        //Wt::WMenuItem* mi = langItems->addItem(Wt::WString::fromUTF8(l.longDescription_.toUTF8()));
        //langItems->itemAt(i)->setStyleClass("poprightsub");
        mi->triggered().connect(boost::bind(&WittyWizard::HandleLanguagePopup, this, i));
        //if (showDebug) { Wt::log("info") << " <<< WittyWizard::CreateHome() set Language -> " << Wt::WString::fromUTF8(l.longDescription_.toUTF8()); }
    }
    // Language Popdown
    // Add Language Popup to Menu
    languagePopup->setMenu(langItems);
    rightMenu->addItem(languagePopup);
    // Create a Theme popup submenu for the Theme Menu.
    Wt::WSubMenuItem* themePopup = new Wt::WSubMenuItem(Wt::WString::tr("theme"));
    themePopup->setStyleClass("popright");
    Wt::WMenu* themeItems = new Wt::WMenu(contents, container);
    themeItems->setStyleClass("poprightsub");
    for (unsigned i = 0; i < themes.size(); ++i)
    {
        // Get Theme
        const Theme& t = themes[i];
        // Add Popup Item with Description.
        Wt::WMenuItem* mit = themeItems->addItem(t.name_);
        //themeItems->itemAt(i)->setStyleClass("poprightsub");
        mit->triggered().connect(boost::bind(&WittyWizard::HandleThemePopup, this, i));
        // if (showDebug) { Wt::log("notice") << " <<<<<<<<<<<<<<< WittyWizard::CreateHome() themes " << t.name_ << " >>>>>>>>>>>>>>>>>>>>> "; }
    }
    // Theme Popdown
    themePopup->setMenu(themeItems);
    rightMenu->addItem(themePopup);
    // Now add Menu to Main Menu
    Wt::WMenuItem *moit = mainMenu_->addMenu(Wt::WString::tr("options"), rightMenu);
    moit->setStyleClass("navright"); // ul
    container->addWidget(contents);
    // On Select
    //mainMenu_->itemSelected().connect(this, &WittyWizard::GoogleAnalyticsLogger);
    // Make the menu be internal-path aware.
    //mainMenu_->setInternalPathEnabled("/"); // Not sure about this, should it be /en/ ?
    // Bind to Template
    homeTemplate->bindWidget("menu", mainMenu_);
    homeTemplate->bindWidget("contents", container);
    // Banner
    std::string myBannerSource = Wt::WString::tr("banner-source").toUTF8();
    std::string myBannerAlt = Wt::WString::tr("banner-alt").toUTF8();
    Wt::WText* banner;
    if (myBannerSource.empty())
        { banner = new Wt::WText(""); }
    else
        { banner = new Wt::WText("<img src='" + myBannerSource + "' alt='" + myBannerAlt + "'>", Wt::XHTMLUnsafeText); }
    // CopyRight
    Wt::WText* copyright = new Wt::WText("<a href='" + myBaseUrl + "'>" + Wt::WString::tr("copyright") + "</a>", Wt::XHTMLUnsafeText); // FIXME add copyright page to CMS
    // Footer Menu
    //
    std::string menuExtras = "";
    // FIXME: Make this an xml item <message id="contacts">name1|path1|name2|path2</message> replace with contact below, split
    if (theIncludes.find("|Contact|") != std::string::npos)
        { menuExtras = "| <a href='" + myBaseUrl + "contact'>" + Wt::WString::tr("contact").toUTF8() + "</a>"; }
    Wt::WText* footermenu = new Wt::WText("<a href='" + myBaseUrl + "'>" + Wt::WString::tr("home") + "</a> " + menuExtras, Wt::XHTMLUnsafeText);
    // Google Analytics
    Wt::WText* ga = new Wt::WText("<script>/*<![CDATA[*/(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,'script','//www.google-analytics.com/analytics.js','ga');ga('create', '" + CrystalBall::GaAccount[domainName] + "', '" + myHost + "');ga('send', 'pageview');/* ]]> */</script>", Wt::XHTMLUnsafeText);
    //
    homeTemplate->bindWidget("banner", banner);
    homeTemplate->bindWidget("copyright", copyright);
    homeTemplate->bindWidget("footermenu", footermenu);
    homeTemplate->bindWidget("ga", ga);
    /*
    sideBarContent_ = new WContainerWidget();
    result->bindWidget("sidebar", sideBarContent_);
    */
    // Add Home Page to root
} // end CreateMenu
/* ****************************************************************************
 * Set Base URL
 * URL Schema: http or https + Root Prefix defined in deploy-path + Language Code
 */
void WittyWizard::SetBaseURL()
{
    myBaseUrl = myUrlScheme + "://" + myHost + "/" + CrystalBall::RootPathPrefix + "/" + GetLanguageName() + "/";
} // end SetBaseURL
/* ****************************************************************************
 * Menu Man
 */
Wt::WWidget* WittyWizard::MenuMan()
{
    std::string thePath = CrystalBall::GetPath(internalPath(), GetLanguageName()); // FIXME: make sure this is clean
    //if (showDebug) { Wt::log("start") << " WittyWizard::MenuMan() thePath = " << thePath; }
    if (thePath.empty()) { thePath = "home"; } // FIXME: home is not hard coded
    //if (showDebug) { Wt::log("start") << " WittyWizard::MenuMan() moduleName = " << thePath; }
    return GetTemplate(thePath);
    //return new Wt::WText("<h1>Test</h1>", Wt::XHTMLUnsafeText);
} // end MenuMan
/* ****************************************************************************
 * Get Template
 */
Wt::WWidget* WittyWizard::GetTemplate(std::string thePath)
{
    bool showDebug = true;
    Wt::WString content;
    //
    if (CrystalBall::UseDb[domainName] == "1")
    {
        MenuManView* myMenu = new MenuManView(appRoot() + "home/" + domainName + "/menuman/", *dbConnection, myLanguage, CrystalBall::UseDb[domainName], domainName, Wt::Horizontal, internalPath());
        content = myMenu->GetMenu("/" + thePath);
        if (content.empty())
            { return new Wt::WText("Page Not Found"); }
    }
    else
    {
        if (!CrystalBall::IsFile(appRoot() + "home/" + domainName + "/menuman/xml/" + thePath + ".xml"))
            { if (showDebug) { Wt::log("notice") << " WittyWizard::GetTemplate() Template not found " << appRoot() + "home/" + domainName + "/menuman/xml/" + thePath + ".xml"; } }
        messageResourceBundle().use(appRoot() + "home/" + domainName + "/menuman/xml/" + thePath, false);
        size_t found = thePath.find_last_of("/\\");
        if (found != std::string::npos)
            { thePath = thePath.substr(found + 1); }
        content = Wt::WString::tr(thePath + "-template");
    } // end if (CrystalBall::UseDb[domainName] == "0")
    /*
    Wt::log("notice") << " WittyWizard::GetTemplate() found audio tag: useWidgetFun > 1";
    //
    Wt::WContainerWidget* result = new Wt::WContainerWidget();
    if (!content.empty())
    {
        Wt::WText *w = new Wt::WText(content, Wt::XHTMLUnsafeText, result);
        w->setInternalPathEncoding(true);
    }
    return result;
    */

#define FUNCT1x
#ifdef FUNCT1
    WidgetFunction widgetFunction;
    //
    Wt::WTemplate* myTemplate;
    myTemplate = new Wt::WTemplate(content);
    bool tagFound = false;
    if (content.toUTF8().find("widget:audio") != std::string::npos)
    {
        if (showDebug) { Wt::log("notice") << " WittyWizard::GetTemplate() found audio tag: useWidgetFun == 0"; }
        widgetFunction.registerType("audio", WidgetFunction::createAudio);
        // FIXME: Define Default theme
        Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::resourcesUrl() + "jPlayer/skin/jplayer.blue.monday.css");
        tagFound = true;
    }
    if (content.toUTF8().find("widget:video") != std::string::npos)
    {
        if (showDebug) { Wt::log("notice") << " WittyWizard::GetTemplate() found video tag: useWidgetFun == 0"; }
        widgetFunction.registerType("video", WidgetFunction::createVideo);
        tagFound = true;
    }
    if (content.toUTF8().find("widget:line-edit") != std::string::npos)
    {
        if (showDebug) { Wt::log("notice") << " WittyWizard::GetTemplate() found line-edit tag: useWidgetFun == 0"; }
        widgetFunction.registerType("line-edit", WidgetFunction::createLineEdit);
        tagFound = true;
    }
    if (tagFound)
    {
        myTemplate->addFunction("widget", widgetFunction);
    }
    return myTemplate;
    //return new Wt::WText(myTemplate, Wt::XHTMLUnsafeText);
#else
    // This is so much cleaner then above, since we need to add more controls
    WidgetFunction widgetFunction;
    widgetFunction.setTemplate(content);
    // I wish I knew a way to put this function pointer inside the widgetFunction
    if (widgetFunction.doAddFunction()) { widgetFunction.getTemplate()->addFunction("widget", widgetFunction); }
    return widgetFunction.getTemplate();
#endif
} // end GetTemplate
/* ****************************************************************************
 * Is Path Language
 * langPath: pass in first path string
 * return -1 if not found, else return index of Language
 */
int WittyWizard::IsPathLanguage(std::string langPath)
{
    if (langPath.empty()) { return -1; }
    int foundLanguageIndex = -1;
    for (unsigned i = 0; i < languages.size(); ++i)
    {
        if (languages[i].name_ == langPath)
        {
            foundLanguageIndex = i;
            //Wt::log("notice") << " *** WittyWizard::IsPathLanguage() langPath = " << langPath << " found at " << i << " ***"; // en, cn, ru ...
            break;
        }
    }
    return foundLanguageIndex;
} // end IsPathLanguage
/* ****************************************************************************
 * Get Default Language
 * returns the language index based on the current int language_ index -1 means uninitilized
 */
int WittyWizard::GetDefaultLanguage()
{
    int newLanguageIndex = myLanguageIndex;
    if (newLanguageIndex == -1)
    {
        std::string languageName = internalPathNextPart("/"); // Checks First Argument
        newLanguageIndex = IsPathLanguage(languageName);
        if (newLanguageIndex == -1)
        {
            // FIXME: Option to use Users Default Language, how do you get the default users language?
            newLanguageIndex = IsPathLanguage(myLocale);
            if (newLanguageIndex == -1)
                { newLanguageIndex = CrystalBall::DefaultLanguageIndex[domainName]; }
        }
    }
    return newLanguageIndex;
} // end WittyWizard::GetDefaultLanguage
/* ****************************************************************************
 * Get Language Index
 * languageName: name in struct Lang
 */
int WittyWizard::GetLanguageIndex(std::string languageName)
{
    int newLanguageIndex = IsPathLanguage(languageName);
    if (newLanguageIndex == -1)
        { newLanguageIndex = GetDefaultLanguage(); }
    return newLanguageIndex;
} // end GetLanguageIndex
/* ****************************************************************************
 * Get Language
 * string languageName
 */
const Lang& WittyWizard::GetLanguage(std::string languageName)
{
    return languages[GetLanguageIndex(languageName)];
} // end GetLanguage
/* ****************************************************************************
 * Get Language
 * int index
 * FIXME: Get Browsers Default Language and convert it to name, i.e. en_US = en, then find index
 */
const Lang& WittyWizard::GetLanguage(int index)
{
    if (myLanguageIndex == -1)
        { return languages[GetDefaultLanguage()]; }
    else
        { return languages[index]; }
} // end GetLanguage
/* ****************************************************************************
 * Get Language Name
 */
std::string WittyWizard::GetLanguageName()
{
    return languages[GetDefaultLanguage()].name_;
} // end GetLanguageName
/* ****************************************************************************
 * Get Locale
 */
void WittyWizard::SetMyLocale()
{
    //Wt::log("info") << "-> WittyWizard::LookInCyrstalball(): defaultLanguageIndex = " << CrystalBall::DefaultLanguageIndex[domainName];
    std::string thisLocale = myLocale.substr(0, myLocale.find("-"));
    if (IsPathLanguage(thisLocale) == -1)
        { thisLocale = GetLanguageName(); }
    myLocale = thisLocale;
} // end GetLocale
/* ****************************************************************************
 * Handle Language Popup
 * data: passed in as a index into languages
 * assumes path: /lang
 */
void WittyWizard::HandleLanguagePopup(int data)
{
    //Wt::log("start") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") language_ = " << myLanguageIndex << " *** ";
    if (myLanguageIndex == data) { return; } // Nothing to do
    std::string languagePath = internalPathNextPart("/"); // Checks First path element, its allows the Language: en, ru, cn
    std::string thePath = internalPath(); // will always be /lang...
    // We must replace the Old Lanuage First
    if (!CrystalBall::StringReplace(thePath, languagePath, languages[data].name_))
        { Wt::log("error") << "WittyWizard::HandleLanguagePopup(Error in String Replace " << ")"; }
    //Wt::log("notice") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") set thePath = " << thePath << " *** ";
    //
    Wt::WApplication::instance()->setInternalPath(thePath,  true);
    //InternalPathChange();
    //Wt::log("end") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") *** ";
} // end HandleLanguagePopup
/* ****************************************************************************
 * Handle Theme Popup
 */
void WittyWizard::HandleThemePopup(int data)
{
    Wt::log("start") << " *** WittyWizard::HandleThemePopup(data: " << data << ") *** ";
    SetWizardTheme(false, data);
    ReInit();
} // end HandleThemePopup
/* ****************************************************************************
 * Set Theme
 */
void WittyWizard::SetWizardTheme(bool fromCookie, int index)
{
    Wt::log("start") << " *** WittyWizard::SetWizardTheme(FromCookie: " << "data: " << index << ") *** ";
    std::string myTheme;
    if (fromCookie)
    {
        myTheme = CrystalBall::GetCookie("wwtheme");
        if (myTheme.empty())
            { myTheme = CrystalBall::DefaultTheme[domainName]; }
    }
    else
        { myTheme = themes[index].path_; }
    if (!myTheme.empty())
    {
        // FIXME check for legal path - Note: even if they change the cookie, its still only going to change the file name, worse case it does not work
        std::string jsCss = "document.getElementById('wittywizardstylesheet').href='" + Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/ww-" + myTheme + ".css';";
        this->doJavaScript(jsCss);
        jsCss = "document.getElementById('wittywizardmenustylesheet').href='" + Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/SfMenuHoriz.css';";
        this->doJavaScript(jsCss);
        //useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/ww-" + myTheme + ".css");
        //useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/SfMenuHoriz.css");
        CrystalBall::SetCookie("wwtheme", myTheme);
    } // end if (!myTheme.empty())
} // end SetWizardTheme
/* ****************************************************************************
 * Google Analytics Logger
 */
void WittyWizard::GoogleAnalyticsLogger()
{
    std::string googleCmd = "if (window.pageTracker) { try { setTimeout(function() { window.pageTracker._trackPageview('" + environment().deploymentPath() + internalPath() + "'); }, 1000); } catch (e) { } }";
    doJavaScript(googleCmd);
} // end googleAnalyticsLogger
// --- End Of File ------------------------------------------------------------
