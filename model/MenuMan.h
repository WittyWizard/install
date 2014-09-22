#ifndef MENUMAN_H
#define MENUMAN_H
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
/* ****************************************************************************
 * Menu Manager
 * name: MenuName
 * path: menuname ~
 *       /moduleName/subpath/
 * language: en, cn, ru...
 * content: XHTML
 */
class MenuMan : public Wt::Dbo::Dbo<MenuMan>
{
    public:
        MenuMan();
        Wt::WString name;             // name: MenuName
        std::string path;             // path: menuname
        std::string type;             // type: sub
        std::string parent;           // parent
        std::string grandparent;      // grandparent
        std::string language;         // language: en, cn, ru...
        Wt::WString content;          // content: XHTML
        Wt::WString title;            // title:
        Wt::WString description;      // description:
        Wt::WString keywords;         // keywords:
        Wt::WString copyright;        // copyright:
        std::string rating;           // rating: general, mature, restricted, 14 years, safe for kids
        //
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, name,             "name");
            Wt::Dbo::field(a, path,             "path");
            Wt::Dbo::field(a, type,             "type");
            Wt::Dbo::field(a, parent,           "parent");
            Wt::Dbo::field(a, grandparent,      "grandparent");
            Wt::Dbo::field(a, language,         "language");
            Wt::Dbo::field(a, content,          "content");
            Wt::Dbo::field(a, title,            "title");
            Wt::Dbo::field(a, description,      "description");
            Wt::Dbo::field(a, keywords,         "keywords");
            Wt::Dbo::field(a, copyright,        "copyright");
            Wt::Dbo::field(a, rating,           "rating");
        }}; // end class MenuMan
#endif // MENUMAN_H
// --- End Of File ------------------------------------------------------------
