#ifndef APP_H
#define APP_H
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WPopupMenu>
#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WBootstrapTheme>
#include <Wt/WTemplate>
#include <Wt/WNavigationBar>
#include <Wt/WComboBox>
#include <Wt/WVideo>
#include <Wt/WImage>
#include <Wt/WLineEdit>
#include <Wt/WAudio>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <map>
#include <Wt/WBootstrapTheme>
#include "Crystallball.h"
#ifdef MENUMAN
    #include "view/MenuManView.h"
#endif
// Plugin System
#include "WittyWizardPlugins.h"
/* ****************************************************************************
 * DeferredWidget
 * A utility container widget which defers creation of its single
 * child widget until the container is loaded (which is done on-demand by a WMenu).
 * The constructor takes the create function for the widget as a parameter.
 *
 * We use this to defer widget creation until needed.
 */
template <typename Function>
class DeferredWidget : public Wt::WContainerWidget
{
    public:
        DeferredWidget(Function f) : f_(f) { }

    private:
        void load()
        {
            Wt::WContainerWidget::load();
            if (count() == 0)
            {
                addWidget(f_());
            }
        }

        Function f_;
};
/* ****************************************************************************
 * deferCreate
 */
template <typename Function>
DeferredWidget<Function>* deferCreate(Function f)
{
    return new DeferredWidget<Function>(f);
}
/* ****************************************************************************
 * Lang
 * name: en, cn, ru
 * code: en_US, zh_CN, ru_RU
 * short Description: en, 汉语, ру
 * long Description: English, 中文 (Chinese), Русский (Russian)
 * FIXIT: [warning] "WString: narrow(): loss of detail: ?? (Chinese)"
 */
struct Lang
{
    Lang(const std::string& name, const std::string& code, const std::string& shortDescription, const std::string& longDescription) : name_(name), code_(code), shortDescription_(shortDescription), longDescription_(longDescription) { }
    std::string name_, code_, shortDescription_, longDescription_;
};
/* ****************************************************************************
 * Theme
 */
struct Theme
{
    Theme(const std::string& name) : name_(name) { }
    std::string name_;
};
/* ****************************************************************************
 * App
 */
class WittyWizard : public Wt::WApplication
{
    public:
        WittyWizard(const Wt::WEnvironment& e);
    private:
        void ReInit();
        void CreateMenu();
        void InternalPathChange(const std::string& path);
        Wt::WWidget* Home();
        Wt::WWidget* Contact();
        Wt::WWidget* About();
        void UpdateTitle();
        int GetDefaultLanguage();
        void HandleLanguagePopup(int data);
        void HandleThemePopup(int data);
        void SetWizardTheme(bool fromCookie, int index);
        void GoogleAnalyticsLogger();
        const Lang& GetLanguage(std::string languageName);
        const Lang& GetLanguage(int index);
        int GetLanguageIndex(std::string languageName);
        int IsPathLanguage(std::string langPath);
        void SetBaseURL();
        void CallPluginSetLanguage(std::string moduleName, std::string languageName, int newLanguage);
        bool PluginHandlePathChange(std::string moduleName, int newLanguage);
        void CallPlugin();
        void CallMenuPlugin();
        Wt::WWidget* GetTemplate(Wt::WString content);
        WidgetFunction widgetFunction;
        // FIXME Plugin System - how do I fix this?
        #ifdef VIDEOMAN
            Wt::WWidget* VideoMan();
        #endif
        #ifdef HITCOUNTERMAN
            Wt::WWidget* HitCounterMan();
        #endif
        #ifdef MENUMAN
            MenuManView* thisMenu;
        #endif
        // Add Language
        void AddLanguage(const Lang& l) { languages.push_back(l); }
        // Add Theme
        void AddTheme(const Theme& l) { themes.push_back(l); }
        //
        Wt::WTemplate* homeTemplate_ = NULL;
        Wt::WWidget* homePage_;
        //
        std::string myHost;
        std::string myUrlScheme;
        std::string myBaseUrl;
        std::string domainName = "";
        Wt::Dbo::SqlConnectionPool *dbConnection_;
        std::string theIncludes = "";
        // Language Vector Array
        std::vector<Lang> languages;
        int language_ = -1; // Language Index: -1 means uninitilized
        // Theme Vector Array
        std::vector<Theme> themes;
        Wt::WMenu* mainMenu_;
        // Used to prevent internalPath changes
        bool isPathChanging = false;

};
Wt::WApplication* create_app(const Wt::WEnvironment& env);

#endif // APP_H
// --- End Of File ------------------------------------------------------------
