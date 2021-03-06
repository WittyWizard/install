#ifndef WIDGETFUNCTION_H
#define WIDGETFUNCTION_H
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WLineEdit>
#include <Wt/WAudio>
#include <Wt/WMediaPlayer>
#include <Wt/WVideo>
#include <Wt/WComboBox>
#include <Wt/WText>
#include <Wt/WLink>
#include <Wt/WImage>
#include <Wt/WCheckBox>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WHBoxLayout>
#include <Wt/WToolBar>
//#include <Wt/WIntValidator>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/tokenizer.hpp>
#include <map>
/* ****************************************************************************
 * Widget Function
 */
class WidgetFunction
{
    public:
        //WidgetFunction();
        typedef boost::function<Wt::WWidget* (const std::vector<Wt::WString>&)> InstatiateWidget;

        bool operator()(Wt::WTemplate* t, const std::vector<Wt::WString>& args, std::ostream& result);

        void registerType(const std::string& name, InstatiateWidget instatiate);
        bool doAddFunction();
        void setTemplate(Wt::WString& widgets);
        Wt::WTemplate* getTemplate();
        bool typeFound = false;

        static Wt::WWidget* createAudio(const std::vector<Wt::WString>& args);
        static Wt::WWidget* createVideo(const std::vector<Wt::WString>& args);
        static Wt::WWidget* createLineEdit(const std::vector<Wt::WString>& args);
        static Wt::WWidget* getFlashBack(Wt::WContainerWidget *result, std::string tagId, std::string tagSrc, std::string tagAltSrc, std::string tagClass, std::string tagWidth, std::string tagHeight, std::string tagTitle, std::string tagAlign);
        static Wt::WPushButton* createButton(const Wt::WString& text, const std::string className, const std::string link);

    private:
        typedef std::map<std::string, InstatiateWidget> RegistryMap;
        RegistryMap registeredTypes_;
        Wt::WTemplate* myTemplate_;
        bool debugLog = true;

        //
        static bool isFile(std::string fileName);
        static std::string getArg(const std::string& name, const std::vector<Wt::WString>& args);
        static bool StringReplace(std::string& string2replace, const std::string& changefrom, const std::string& changeTo);
}; // end class WidgetFunction
#endif // WIDGETFUNCTION_H
// --- End Of File ------------------------------------------------------------
