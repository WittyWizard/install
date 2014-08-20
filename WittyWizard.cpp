/* ****************************************************************************
 * Witty Wizard
 */
#include "WittyWizard.h"
/* ****************************************************************************
 * Witty Wizard Contructor
 */
WittyWizard::WittyWizard(const Wt::WEnvironment& env) : Wt::WApplication(env)
{
    Wt::log("start") << " *** WittyWizard::WittyWizard() env.hostName() = " << env.hostName().c_str() << " *** ";
    myHost = env.hostName().c_str();       // localhost:8088
    myUrlScheme = env.urlScheme().c_str(); // http or https
    myBaseUrl = myUrlScheme + "://" + myHost + "/"; // FIXIT

    domainName = env.hostName().c_str();
    unsigned pos = domainName.find(":");
    if (pos > 0)
    {
        domainName = domainName.substr(0, pos);
    }
    // this is just for testing multiple sites in a localhost nated network
    if (0) domainName = "wittywizard.org";
    if (0) domainName = "lightwizzard.com";
    if (0) domainName = "vetshelpcenter.com";
    //
    const std::string jqueryMin = docRoot() + "/resources/js/jquery.js"; // resourcesUrl()
    if (Crystallball::IsFile(jqueryMin))
    {
        Wt::log("info") << "**** WittyWizard::WittyWizard() Using Jquery file: " <<  jqueryMin << " ****";
        //Wt::WApplication::instance()->requireJQuery(jqueryMin);
    }
    else
    {
        Wt::log("error") << "**** WittyWizard::WittyWizard() missing file: " <<  jqueryMin << "****";
    }
    //
    // Note: this is in the root with executable
    // ./app_root/domains.xml
    if (!Crystallball::GetConnectionPoolInfo(appRoot().append("domains.xml").c_str()))
    {
        Wt::log("error") << " *** WittyWizard::WittyWizard() domains.xml file not found:" << appRoot().append("domains.xml").c_str();
        return; // Fix 404
    }
    // Set Global Variables
    if (!Crystallball::SetSqlConnectionPool(domainName))
    {
        Wt::log("error") << "(WittyWizard::WittyWizard: SetSqlConnectionPool failed for domain: " << domainName << ")";
        // FIXIT make this a error page
        return;
    }
    // Connect to Connection Pool
    if(Crystallball::ConnectionPool.find(domainName) == Crystallball::ConnectionPool.end())
    {
        // element not found;
        Wt::log("error") << "WittyWizard::WittyWizard() myConnectionPool element not found " << domainName << "";
        return; // FIXME make and error page
    }
    try
    {
        dbConnection_ = boost::any_cast<Wt::Dbo::SqlConnectionPool *>(Crystallball::ConnectionPool[domainName]);
    }
    catch (...)
    {
        Wt::log("error") << "->>> WittyWizard::WittyWizard() Failed Connection Pool " << domainName << "<<<-";
        return; // FIXME make and error page
    }
    //
    if (!Crystallball::IsFile(appRoot() + "home/" + domainName + "/WittyWizard.xml"))
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
        Wt::log("notice") << "WittyWizard::WittyWizard: (resourcesUrl(): " << resourcesUrl() << ")"; // /resources/
        Wt::log("notice") << "WittyWizard::WittyWizard: (appRoot(): " << appRoot() << ")";           // ./app_root/
        Wt::log("notice") << "WittyWizard::WittyWizard: (docRoot(): " << docRoot() << ")";           // ./doc_root
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
    //               name, code,    shortDescription, longDescription
    AddLanguage(Lang("en", "en_US", "en",             "English"));
    AddLanguage(Lang("cn", "zh_CN", "汉语",            "中文 (Chinese)"));
    AddLanguage(Lang("ru", "ru_RU", "ру",             "Русский (Russian)"));
    // Add Theme
    AddTheme(Theme("red"));
    AddTheme(Theme("white"));
    AddTheme(Theme("blue"));
    AddTheme(Theme("green"));
    AddTheme(Theme("tan"));
    AddTheme(Theme("default"));
    //
    CreateMenu();
    internalPathChanged().connect(this, &WittyWizard::InternalPathChange);
    InternalPathChange(internalPath());
    Wt::log("end") << "WittyWizard::WittyWizard()";
} // WittyWizard
/* ****************************************************************************
 * Create Menu
 */
void WittyWizard::CreateMenu()
{
    homeTemplate_ = new Wt::WTemplate(Wt::WString::tr("ww-template"), root()); //  <message id="template">
    homePage_ = homeTemplate_; // not sure why I need this
    //
    Wt::WStackedWidget* contents = new Wt::WStackedWidget();
    Wt::WAnimation fade(Wt::WAnimation::Fade, Wt::WAnimation::Linear, 250);
    contents->setTransitionAnimation(fade);
    contents->setId("main_page");
    // Create a navigation bar with a link to a web page.
    Wt::WNavigationBar *navigation = new Wt::WNavigationBar(contents);
    //navigation->setId("navigation");
    //navigation->setTitle("Witty Wizard", "http://www.google.com/search?q=witty+wizard");
    navigation->setTitle(Wt::WString::tr("title"));
    //navigation->setResponsive(true); // caused it to be collapsable

    Wt::WStackedWidget *contentsStack = new Wt::WStackedWidget(contents);
    //contentsStack->setId("contents");
    //contentsStack->addStyleClass("contents");

    // Setup a Left-aligned menu.
    mainMenu_ = new Wt::WMenu(contentsStack, contents);
    //mainMenu_->setId("mainmenu");
    //mainMenu_->setStyleClass("mainmenu");
    navigation->addMenu(mainMenu_);
    // FIXIT
    Wt::WText *searchResult = new Wt::WText(Wt::WString::tr("search"));
    // FIXIT
    //QString includeThis = myIncludes[domainName];
    //includeThis.split("|")
    // "|home|chat|blog|about|contact|video|"
    mainMenu_->addItem(Wt::WString::tr("home"),  Home())->setPathComponent("");
    theIncludes = Crystallball::ModuleIncludes[domainName];
    Wt::log("notice") << "WittyWizard::CreateHome:  theIncludes=" << theIncludes;
    // Call Menu Plugin
    CallMenuPlugin();
    #ifdef MENUMAN
        // query menu: lang,
        const Lang& l = GetLanguage(language_);
        thisMenu = new MenuManView(appRoot() + "home/" + domainName + "/menuman/", "/" + l.name_ + "/menuman/", *dbConnection_, l.name_, Crystallball::UseDb[domainName], domainName);

        //mainMenu_->addItem(Wt::WString::tr("contact"), deferCreate(boost::bind(&WittyWizard::Contact, this)));
        // thisMenu
    #endif
    if (theIncludes.find("|contact|") != std::string::npos)
    {
        mainMenu_->addItem(Wt::WString::tr("contact"), deferCreate(boost::bind(&WittyWizard::Contact, this)));
    }
    if (theIncludes.find("|about|") != std::string::npos)
    {
        mainMenu_->addItem(Wt::WString::tr("about"),   deferCreate(boost::bind(&WittyWizard::About, this)));
    }
    //mainMenu_->addItem("Search", searchResult);

    // Setup a Right-aligned menu.
    Wt::WMenu *rightMenu = new Wt::WMenu();
    //rightMenu->setId("rightmenu");
    navigation->addMenu(rightMenu, Wt::AlignRight);

    // Create a Language popup submenu for the Language Menu.
    Wt::WPopupMenu *languagePopup = new Wt::WPopupMenu();
    for (unsigned i = 0; i < languages.size(); ++i)
    {
        // Get Language
        const Lang& l = languages[i];
        // Add Popup Item with Description.
        Wt::WMenuItem *mi = languagePopup->addItem(Wt::WString::fromUTF8(l.longDescription_));
        mi->triggered().connect(boost::bind(&WittyWizard::HandleLanguagePopup, this, i));
        Wt::log("info") << " <<< WittyWizard::CreateHome() set Language -> " << Wt::WString::fromUTF8(l.longDescription_);
    }
    // Language Popdown
    Wt::WMenuItem *item = new Wt::WMenuItem(Wt::WString::tr("language"));
    // Add Language Popup to Menu
    item->setMenu(languagePopup);
    rightMenu->addItem(item);

    // Create a Theme popup submenu for the Theme Menu.
    Wt::WPopupMenu *themePopup = new Wt::WPopupMenu();
    for (unsigned i = 0; i < themes.size(); ++i)
    {
        // Get Theme
        const Theme& t = themes[i];
        // Add Popup Item with Description.
        Wt::WMenuItem *mit = themePopup->addItem(Wt::WString::fromUTF8(t.name_));
        mit->triggered().connect(boost::bind(&WittyWizard::HandleThemePopup, this, i));
        // Wt::log("notice") << " <<<<<<<<<<<<<<< WittyWizard::CreateHome() themes " << t.name_ << " >>>>>>>>>>>>>>>>>>>>> ";
    }
    // Theme Popdown
    Wt::WMenuItem *themeItem = new Wt::WMenuItem(Wt::WString::tr("theme"));
    // Add Theme Popup to Menu
    themeItem->setMenu(themePopup);
    rightMenu->addItem(themeItem);

    // Add a Search control.
    Wt::WLineEdit *searchText = new Wt::WLineEdit();
    searchText->setEmptyText(Wt::WString::tr("search"));

    searchText->enterPressed().connect(std::bind([=] ()
    {
        // FIXIT add a real search feature
        mainMenu_->select(4); // is the index a random menu item
        searchResult->setText(Wt::WString("Nothing found for {1}.").arg(searchText->text()));
    }));

    navigation->addSearch(searchText, Wt::AlignRight);

    contents->addWidget(contentsStack);

    // On Select
    mainMenu_->itemSelectRendered().connect(this, &WittyWizard::UpdateTitle);
    mainMenu_->itemSelected().connect(this, &WittyWizard::GoogleAnalyticsLogger);
    // Make the menu be internal-path aware.
    mainMenu_->setInternalPathEnabled("/"); // Not sure about this, should it be /en/ ?
    // Bind to Template
    //result->bindWidget("menu", mainMenu_);
    homeTemplate_->bindWidget("menu", navigation);
    homeTemplate_->bindWidget("contents", contents);
    // Banner
    std::string myBannerSource = Wt::WString::tr("banner-source").toUTF8();
    std::string myBannerAlt = Wt::WString::tr("banner-alt").toUTF8();
    Wt::WText* banner;
    if (!myBannerSource.empty())
    {
        banner = new Wt::WText("<img src='" + myBannerSource + "' alt='" + myBannerAlt + "'>", Wt::XHTMLUnsafeText);
    }
    else
    {
        banner = new Wt::WText("");
    }
    // CopyRight
    Wt::WText* copyright = new Wt::WText("<a href='" + myBaseUrl + "'>" + Wt::WString::tr("copyright") + "</a>", Wt::XHTMLUnsafeText); // FIXIT add copyright page to CMS
    // Footer Menu
    Wt::WText* footermenu = new Wt::WText("<a href='" + myBaseUrl + "'>" + Wt::WString::tr("home") + "</a> | <a href='" + myBaseUrl + "contact'>" + Wt::WString::tr("contact") + "</a>", Wt::XHTMLUnsafeText);
    // Google Analytics
    Wt::WText* ga = new Wt::WText("<script>/*<![CDATA[*/(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,'script','//www.google-analytics.com/analytics.js','ga');ga('create', '" + Crystallball::GaAccount[domainName] + "', '" + myHost + "');ga('send', 'pageview');/* ]]> */</script>", Wt::XHTMLUnsafeText);
    //
    homeTemplate_->bindWidget("banner", banner);
    homeTemplate_->bindWidget("copyright", copyright);
    homeTemplate_->bindWidget("footermenu", footermenu);
    homeTemplate_->bindWidget("ga", ga);
    /*
    sideBarContent_ = new WContainerWidget();
    result->bindWidget("sidebar", sideBarContent_);
    */
} // end CreateMenu
/* ****************************************************************************
 * Re init
 */
void WittyWizard::ReInit()
{
    Wt::log("start") << " *** WittyWizard::ReInit() *** ";
    // Set Base URL
    SetBaseURL();
    // Clear the Screen
    root()->clear();
    // Create Home Page, we just cleared it
    CreateMenu();
    // Add Home Page to root
    //root()->addWidget(homePage_); // FIXIT do I need to do this?
} // end ReInit()
/* ****************************************************************************
 * Set Base URL
 * URL Schema: http or https + Root Prefix defined in deploy-path + Language Code
 */
void WittyWizard::SetBaseURL()
{
    myBaseUrl = myUrlScheme + "://" + myHost + "/" + Crystallball::RootPathPrefix + "/" + languages[GetDefaultLanguage()].name_ + "/";
} // end SetBaseURL
/* ****************************************************************************
 * Home
 */
Wt::WWidget* WittyWizard::Home()
{
    return GetTemplate(Wt::WString::tr("page.home"));
} // end Home
/* ****************************************************************************
 * Contact
 */
Wt::WWidget* WittyWizard::Contact()
{
    return GetTemplate(Wt::WString::tr("page.contact"));
} // end Contact
/* ****************************************************************************
 * About
 */
Wt::WWidget* WittyWizard::About()
{
    return GetTemplate(Wt::WString::tr("page.about"));
} // end About
/* ****************************************************************************
 * Get Template
 *
 */
Wt::WWidget* WittyWizard::GetTemplate(Wt::WString content)
{
    int useWidgetFun = 0;
    if (useWidgetFun == 0)
    {
        //
        bool tagFound = false;
        Wt::WTemplate* myTemplate = new Wt::WTemplate(content);
        if (content.toUTF8().find("widget:audio") != std::string::npos)
        {
            Wt::log("notice") << " AudioManImpl::GetTemplate() found audio tag: useWidgetFun == 0";
            widgetFunction.registerType("audio", WidgetFunction::createAudio);
            // FIXME: Define Default theme
            Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::resourcesUrl() + "jPlayer/skin/jplayer.blue.monday.css");
            tagFound = true;
        }
        if (content.toUTF8().find("widget:line-edit") != std::string::npos)
        {
            Wt::log("notice") << " AudioManImpl::GetTemplate() found line-edit tag: useWidgetFun == 0";
            widgetFunction.registerType("line-edit", WidgetFunction::createLineEdit);
            tagFound = true;
        }
        if (tagFound)
        {
            myTemplate->addFunction("widget", widgetFunction);
        }
        return myTemplate;
    }
    else if (useWidgetFun == 1)
    {
        // This is so much cleaner, since we need to add more controls
        // I pass in referance to widgetFunction, this is not writen as a singleton
        widgetFunction.setTemplate(content, widgetFunction);

        if (widgetFunction.doAddFunction())
        {
            widgetFunction.getTemplate()->addFunction("widget", widgetFunction);
        }
        return widgetFunction.getTemplate();
    }
    else
    {
        Wt::log("notice") << " AudioManImpl::GetTemplate() found audio tag: useWidgetFun > 1";
        //
        Wt::WContainerWidget* result = new Wt::WContainerWidget();
        if (!content.empty())
        {
            Wt::WText *w = new Wt::WText(content, Wt::XHTMLUnsafeText, result);
            w->setInternalPathEncoding(true);
        }
        return result;
    }
} // end GetTemplate
/* ****************************************************************************
 * Internal Path Change
 */
void WittyWizard::InternalPathChange(const std::string& thePath)
{
    if (isPathChanging)
    {
        Wt::log("restart") << " ~~~~ WittyWizard::InternalPathChange() returning nothing done ~~~~ ";
        return;
    }
    //
    std::string languageName = internalPathNextPart("/"); // Checks First Argument
    //
    // Get a Valid Language, returns default if not found
    const Lang& theLanguage = GetLanguage(languageName);
    languageName = theLanguage.name_;
    std::string newPath = "/" + languageName;
    //
    int newLanguage = GetLanguageIndex(languageName); // Set to default Language Index if not set
    //
    std::vector<std::string> parts;
    boost::split(parts, thePath, boost::is_any_of("/"));

    Wt::log("start") << " WittyWizard::InternalPathChange() language_ = " << language_ << " | newLanguage = " << newLanguage;

    if (language_ != newLanguage)
    {
        Wt::log("start") << " WittyWizard::InternalPathChange() Language Change ~ path = " << thePath << " | parts.size()=" << parts.size() << " | parts[0]=" << parts[0] << " | parts[1]=" << parts[1];
    }
    else
    {
        Wt::log("start") << " WittyWizard::InternalPathChange() No Language Change ~ path = " << thePath << " | parts.size()=" << parts.size() << " | parts[0]=" << parts[0] << " | parts[1]=" << parts[1];
    }

    // path = /                      | parts.size()=2 | parts[0]= | parts[1]=
    // path = /en                    | parts.size()=2 | parts[0]= | parts[1]=en
    // path = /en/video/Series/Video | parts.size()=5 | parts[0]= | parts[1]=en | parts[2]=video | parts[3]=Series | parts[4]=Video

    std::string moduleName = "";
    if (parts.size() > 2)
    {
        moduleName = parts[2];
    }
    // If Language changed, make new Path
    if (language_ == newLanguage)
    {
        newPath = thePath;
    }
    else
    {
        for (unsigned i = 2; i < parts.size(); ++i)
        {
            newPath = newPath + "/" + parts[i];
        }
    }
    //
    isPathChanging = true;
    if (!PluginHandlePathChange(moduleName, newLanguage))
    {
        return;
    }
    if (language_ != newLanguage)
    {
        Wt::log("local") << " *** WittyWizard::InternalPathChange() language Code set to = " << languageName << " theLanguage.name_ = " << theLanguage.name_ << " | internalPathNextPart = " << internalPathNextPart("/")  << " | newPath = " << newPath << " *** ";
        // Set Local
        setLocale(theLanguage.name_);
        //setLocale(theLanguage.code_);
    }
    //
    CallPluginSetLanguage(moduleName, languageName, newLanguage);
    //
    UpdateTitle();
    if (language_ != newLanguage)
    {
        language_ = newLanguage; // Set language_ to current Language
        ReInit();
    }
    // Change Menu Base Path
    mainMenu_->setInternalBasePath(theLanguage.name_);
    // Change Path
    Wt::WApplication::instance()->setInternalPath(newPath, true);
    //
    CallPlugin();
    // Set Theme
    SetWizardTheme(true, 0);
    Wt::log("end") << "WittyWizard::InternalPathChange()";
    isPathChanging = false; // Set isPathChanging so InternalPathChange will fire
} // end InternalPathChange
/* ****************************************************************************
 * Is Path Language
 * langPath: pass in first path string
 * return -1 if not found, else return index of Language
 */
int WittyWizard::IsPathLanguage(std::string langPath)
{
    int foundLanguageIndex = -1;
    for (unsigned i = 0; i < languages.size(); ++i)
    {
        if (languages[i].name_ == langPath)
        {
            foundLanguageIndex = i;
            Wt::log("notice") << " *** WittyWizard::IsPathLanguage() langPath = " << langPath << " found at " << i << " ***"; // en, cn, ru ...
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
    int newLanguageIndex = language_;
    if (newLanguageIndex == -1)
    {
        // FIXIT: Option to use Users Default Language, how do you get the default users language?
        newLanguageIndex = std::stoi(Crystallball::DefaultLanguageIndex[domainName]);
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
    {
        // FIXIT: Option to use Users Default Language, how do you get the default users language?
        newLanguageIndex = std::stoi(Crystallball::DefaultLanguageIndex[domainName]);
    }
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
 */
const Lang& WittyWizard::GetLanguage(int index)
{
    if (language_ == -1)
        { return languages[std::stoi(Crystallball::DefaultLanguageIndex[domainName])]; }
    else
        { return languages[index]; }
} // end GetLanguage
/* ****************************************************************************
 * Handle Language Popup
 * data: passed in as a index into languages
 * assumes path: /lang
 */
void WittyWizard::HandleLanguagePopup(int data)
{
    Wt::log("start") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") language_ = " << language_ << " *** ";
    if (language_ == data) { return; } // Nothing to do
    std::string languagePath = internalPathNextPart("/"); // Checks First path element, its allows the Language: en, ru, cn
    std::string thePath = internalPath(); // will always be /lang...
    // We must replace the Old Lanuage First
    if (!Crystallball::StringReplace(thePath, languagePath, languages[data].name_))
    {
        Wt::log("error") << "WittyWizard::HandleLanguagePopup(Error in String Replace " << ")";
    }
    Wt::log("notice") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") set thePath = " << thePath << " *** ";
    //
    Wt::WApplication::instance()->setInternalPath(thePath,  true);
    //InternalPathChange();
    Wt::log("end") << " *** WittyWizard::HandleLanguagePopup(data: " << data << ") *** ";
} // end HandleLanguagePopup
/* ****************************************************************************
 * set Theme
 */
void WittyWizard::HandleThemePopup(int data)
{
    Wt::log("start") << " *** WittyWizard::HandleThemePopup(data: " << data << ") *** ";
    SetWizardTheme(false, data);
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
        myTheme = Crystallball::GetCookie("theme");
        if (myTheme.empty())
        {
            myTheme = Crystallball::DefaultTheme[domainName];
        }
    }
    else
    {
        myTheme = themes[index].name_;
    }
    if (!myTheme.empty())
    {
        // page or Wt-inline-css document.getElementById('wittywizardstylesheet').href='new_css.css';  style.property
        // + myBaseUrl
        // FIXIT check for legal path
        std::string jsCss = "document.getElementById('wittywizardstylesheet').href='" + Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/ww-" + myTheme + ".css';";
        this->doJavaScript(jsCss);
        Crystallball::SetCookie("theme", myTheme);
        Crystallball::SetCookie("themepath", Wt::WApplication::resourcesUrl() + "themes/wittywizard/");
        useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/wittywizard/" + myTheme + "/ww-" + myTheme + ".css");
    }
} // end SetWizardTheme
/* ****************************************************************************
 * Update Title
 */
void WittyWizard::UpdateTitle()
{
    if (mainMenu_->currentItem())
    {
        setTitle(Wt::WString::tr("page.title") + " - " + mainMenu_->currentItem()->text());
    }
} // end UpdateTitle
/* ****************************************************************************
 * Google Analytics Logger
 */
void WittyWizard::GoogleAnalyticsLogger()
{
    std::string googleCmd = "if (window.pageTracker) { try { setTimeout(function() { window.pageTracker._trackPageview('" + environment().deploymentPath() + internalPath() + "'); }, 1000); } catch (e) { } }";
    doJavaScript(googleCmd);
} // end googleAnalyticsLogger
// --- End Of File ------------------------------------------------------------
