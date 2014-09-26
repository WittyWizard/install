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
#include <Wt/WBootstrapTheme>
#include <Wt/WGlobal>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <map>
#include "CrystalBall.h"
#include "view/MenuManView.h"
#include <WidgetFunction.h>
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
            if (count() == 0) { addWidget(f_()); }
        }
        Function f_;
}; // end class DeferredWidget
/* ****************************************************************************
 * defer Create
 */
template <typename Function>
DeferredWidget<Function>* deferCreate(Function f)
{
    return new DeferredWidget<Function>(f);
} // end deferCreate
/* ****************************************************************************
 * Lang
 * name: en, cn, ru
 * code: en_US, zh_CN, ru_RU
 * short Description: en, 汉语, ру
 * long Description: English, 中文 (Chinese), Русский (Russian)
 * FIXME: [warning] "WString: narrow(): loss of detail: ?? (Chinese)"
 */
struct Lang
{
    Lang(const std::string& name, const std::string& code, const Wt::WString& shortDescription, const Wt::WString& longDescription) : name_(name), code_(code), shortDescription_(shortDescription), longDescription_(longDescription) { }
    std::string name_, code_;
    Wt::WString shortDescription_, longDescription_;

}; // end struct Lang
/* ****************************************************************************
 * Theme
 */
struct Theme
{
    Theme(const Wt::WString& name, const std::string& path) : name_(name), path_(path) { }
    Wt::WString name_;
    std::string path_;
}; // end struct Theme
/* ****************************************************************************
 * App
 */
class WittyWizard : public Wt::WApplication
{
    public:
        WittyWizard(const Wt::WEnvironment& e);
        Wt::WWidget* MenuMan();
    private:
        void ReInit();
        void CreateMenu();
        void InternalPathChange(const std::string& path);
        int GetDefaultLanguage();
        void HandleLanguagePopup(int data);
        void HandleThemePopup(int data);
        void SetWizardTheme(bool fromCookie, int index);
        void GoogleAnalyticsLogger();
        const Lang& GetLanguage(std::string languageName);
        const Lang& GetLanguage(int index);
        int GetLanguageIndex(std::string languageName);
        std::string GetLanguageName();
        int IsPathLanguage(std::string langPath);
        void SetBaseURL();
        void CallPluginSetLanguage(std::string moduleName, std::string languageName, int newLanguage);
        bool PluginHandlePathChange(std::string moduleName, int newLanguage);
        void CallPlugin();
        void CallMenuPlugin();
        std::string GetPath();
        Wt::WWidget* GetTemplate(std::string thePath);
        // FIXME Plugin System - how do I fix this?
        #ifdef HITCOUNTERMAN
            Wt::WWidget* HitCounterMan();
        #endif
        //
        Wt::WTemplate* homeTemplate = NULL;
        Wt::WContainerWidget* container = NULL;
        int myLanguageIndex = -1; // Language Index: -1 means uninitilized
        // Language Vector Array
        std::vector<Lang> languages;
        // Add Language
        void AddLanguage(const Lang& l) { languages.push_back(l); }
        // Theme Vector Array
        std::vector<Theme> themes;
        // Add Theme
        void AddTheme(const Theme& l) { themes.push_back(l); }
        //
        std::string myHost;
        std::string myUrlScheme;
        std::string myBaseUrl;
        std::string myLanguage = "";
        std::string domainName = "";       
        Wt::Dbo::SqlConnectionPool* dbConnection;
        std::string theIncludes = "";
        Wt::WMenu* mainMenu_;
        // Used to prevent internalPath changes
        bool isPathChanging = false;
        std::string path = "";
        void SetMyLocale();
        std::string myLocale = "";
        std::string myPath = "";
};
//Wt::WApplication* CreateApp(const Wt::WEnvironment& env);

#endif // APP_H
// --- End Of File ------------------------------------------------------------
