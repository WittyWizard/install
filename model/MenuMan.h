#ifndef MENUMAN_H
#define MENUMAN_H
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
/* ****************************************************************************
 * Menu Man
 */
class MenuMan : public Wt::Dbo::Dbo<MenuMan>
{
    public:
        MenuMan();
        Wt::WString name;     // name: MenuName
        Wt::WString path;     // path: menuname
        Wt::WString language; // language: en, cn, ru...
        Wt::WString content;  // content: XHTML
        //
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, name,     "name");
            Wt::Dbo::field(a, path,     "path");
            Wt::Dbo::field(a, language, "language");
            Wt::Dbo::field(a, content,  "content");
        }}; // end class MenuMan
#endif // MENUMAN_H
// --- End Of File ------------------------------------------------------------
