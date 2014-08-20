/* ****************************************************************************
 * Widget Function
 */
#include "WidgetFunction.h"
/* ****************************************************************************
 * get TypeFound
 */
bool WidgetFunction::doAddFunction()
{
    return typeFound;
} // end getTemplate
/* ****************************************************************************
 * set Template
 * widgets:
 * widgetFunction:
 */
void WidgetFunction::setTemplate(Wt::WString widgets, WidgetFunction widgetFunction)
{
    if (debugLog) { Wt::log("start") << " WidgetFunction::WidgetFunction()"; }
    myTemplate_ = new Wt::WTemplate(widgets);
    if (widgets.toUTF8().find("widget:audio") != std::string::npos)
    {
        if (debugLog) { Wt::log("notice") << " WidgetFunction::WidgetFunction() found audio tag useWidgetFun = 1"; }
        registerType("audio", WidgetFunction::createAudio);
        // if I comment out above line registeredTypes_['audio'] = 0 --- if not it = 1, so it is getting registed
        if (debugLog) { Wt::log("notice") << " WidgetFunction::WidgetFunction() registeredTypes_['audio'] = " << registeredTypes_["audio"]; }
        typeFound = true;
    }
    if (widgets.toUTF8().find("widget:line-edit") != std::string::npos)
    {
        if (debugLog) { Wt::log("notice") << " WidgetFunction::WidgetFunction() found line-edit tag"; }
        registerType("line-edit", WidgetFunction::createLineEdit);
        typeFound = true;
    }
    if (typeFound)
    {
        // My guess is that passing in the pointer to the widgetFunction is not pulling up the same instance,
        // which makes no sence, this is not writen as a singleton
        // FIXME myTemplate_->addFunction("widget", widgetFunction);
    }

    if (debugLog) { Wt::log("end") << " WidgetFunction::WidgetFunction()"; }
} // end setTemplate
/* ****************************************************************************
 * get Template
 */
Wt::WTemplate* WidgetFunction::getTemplate()
{
    return myTemplate_;
} // end getTemplate
/* ****************************************************************************
 * operator
 * t: Template
 * args: from registerType
 * result:
 */
bool WidgetFunction::operator()(Wt::WTemplate* t, const std::vector<Wt::WString>& args, std::ostream& result)
{
    std::string name = args[0].toUTF8();

    RegistryMap::const_iterator i = registeredTypes_.find(name);
    if (i == registeredTypes_.end())
    {
        result << "?? WidgetFunction: no type registered: " << name << " ??";
    }
    else
    {
        std::string id = getArg("id", args);

        Wt::WWidget* w = 0;
        if (!id.empty())
        {
            w = t->resolveWidget(id);
        }
        if (!w)
        {
            w = i->second(args);

            std::string cl = getArg("class", args);
            if (!cl.empty())
            {
                w->addStyleClass(cl);
            }
        }

        if (!w)
        {
            result << "?? WidgetFunction: could not create instance of type " << name << " ??";
        }
        else
        {
            if (id.empty())
            {
                id = w->id();
            }
        }

        t->bindWidget(id, w);

        Wt::WString text = Wt::WString::fromUTF8("${" + id + "}");
        t->renderTemplateText(result, text);
    }
    return true;
} // end operator
/* ****************************************************************************
 * registerType
 * static
 * name: Name of Control type: audio, line-edit, ...
 * instantiate: Function that provides the controls for this tag
 */
void WidgetFunction::registerType(const std::string& name, InstatiateWidget instantiate)
{
    registeredTypes_[name] = instantiate;
} // end registerType
/* ****************************************************************************
 * get Arg
 * static
 * name: tag -> ${widget:audio id='audio'}, will return arg for tag, getArg("id" will return audio
 * args: vector
 */
std::string WidgetFunction::getArg(const std::string& name, const std::vector<Wt::WString>& args)
{
    for (unsigned i = 0; i < args.size(); ++i)
    {
        std::string s = args[i].toUTF8();
        if (boost::starts_with(s, name + "="))
        {
            return s.substr(name.length()+1);
        }
    }
    return std::string();
} // end getArg
/* ****************************************************************************
 * is File
 * static
 * fileName: URL or physical file
 */
bool WidgetFunction::isFile(std::string fileName)
{
    if (fileName.substr(0,4).compare("http:") || fileName.substr(0,4).compare("https:"))
    {
        if (0) // FIXME: how do I Ping file in C++
        {
            return false;
        }
    }
    else
    {
        if (!boost::filesystem::exists(fileName)) { return false; }
    }
    return true;
} // end isFile
/* ****************************************************************************
 * Create Line Edit
 * static
 * args: vector
 * tag: ${widget:line-edit id='edit' class='wteditor' type='editor' width='580' height='33' maxlength='16' emptytext='Change Text' }
 */
Wt::WWidget* WidgetFunction::createLineEdit(const std::vector<Wt::WString>& args)
{
    Wt::WLineEdit* myLineEditor = new Wt::WLineEdit();
    // ID
    std::string myId = getArg("id", args);
    if (!myId.empty()) { myLineEditor->setId(myId); }
    // class
    std::string myClass = getArg("class", args);
    if (!myClass.empty()) { myLineEditor->setStyleClass(myClass); }
    // width
    std::string myWidth = getArg("width", args);
    if (!myWidth.empty()) { myLineEditor->setWidth(std::stoi(myWidth)); }
    // height
    std::string myHeight = getArg("height", args);
    if (!myHeight.empty()) { myLineEditor->setHeight(std::stoi(myHeight)); }
    // emptytext
    std::string myEmptyText = getArg("emptytext", args);
    if (!myEmptyText.empty()) { myLineEditor->setEmptyText(myEmptyText); }
    // maxlength
    std::string myMaxLength = getArg("maxlength", args);
    if (!myMaxLength.empty()) { myLineEditor->setMaxLength(std::stoi(myMaxLength)); }
    return myLineEditor;
} // end createLineEdit
/* ****************************************************************************
 * Create Audio
 * static
 * args: vector
 * tag: ${widget:audio id='audio' align='center' class='wtaudio' type='audio/mpeg' width='580' height='33' src='/resources/LaSera-NeverComeAround.mp3' altsrc='/resources/LaSera-NeverComeAround.ogg' title='La Sera - Never Come Around'}
 */
Wt::WWidget* WidgetFunction::createAudio(const std::vector<Wt::WString>& args)
{
    bool debugging = false;
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    Wt::WAudio *audio = new Wt::WAudio(result);
    // 0: id     "audio1"
    // 1: class  "wtaudio"
    // 2: type   "audio/mpeg" (could be used to pick Flash Player if no audio support by Browser)
    // 3: width  "580"
    // 4: height "33"
    // 5: src    "http://domain.tdl/audio.mp3 or /resources/audio.mp3"
    std::string tagType = getArg("type", args);
    if(!tagType.empty())
    {
        // Enhance for no html5 audio support in browser
        // tag is not used, I had an idea for fall back support, will finish some day
    }
    // Id
    std::string tagId = getArg("id", args);
    // FIXME may be a bug: if(!tagId.empty()) { audio->setId(tagId); }
    // Class
    std::string tagClass = getArg("class", args);
    if(!tagClass.empty()) { audio->setStyleClass(tagClass); }
    // Width
    std::string tagWidth = getArg("width", args);
    if(!tagWidth.empty()) { audio->setWidth(std::stoi(tagWidth)); }
    // Height
    std::string tagHeight = getArg("height", args);
    if(!tagHeight.empty()) { audio->setHeight(std::stoi(tagHeight)); }
    // Source
    std::string tagSrc = getArg("src", args);
    if(!tagSrc.empty())
    {
        // if debugging, do this if you need to check to see if file exist
        if (debugging) { if (!isFile(tagSrc)) { Wt::log("error") << " WidgetFunction::createAudio() File Not Found src: " << tagSrc; } }
        audio->addSource(Wt::WLink(tagSrc));
    }
    // Alternet Source
    std::string tagAltSrc = getArg("altsrc", args);
    if(!tagAltSrc.empty())
    {
        // if debugging, do this if you need to check to see if file exist
        if (debugging) { if (!isFile(tagAltSrc)) { Wt::log("error") << " WidgetFunction::createAudio() File Not Found altsrc: " << tagAltSrc; } }
        audio->addSource(Wt::WLink(tagAltSrc));
    }
    // Title
    std::string tagTitle = getArg("title", args);
    // WAudio has no title, but WMediaPlayer does, its used for flash fall back support
    // align
    std::string tagAlign = getArg("align", args);
    // Controls
    audio->setOptions(Wt::WAudio::Controls);
    // Need browser detection or just stuff a fall back in every request
    audio->setAlternativeContent(getFlashBack(result, tagId, tagSrc, tagAltSrc, tagClass, tagWidth, tagHeight, tagTitle, tagAlign));
    // FIXME: Define Default theme, if use alternet we do not need this
    Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::resourcesUrl() + "jPlayer/skin/jplayer.blue.monday.css");
    return result;
} // end createAudio
/* ****************************************************************************
 * get Flash Back
 * static
 * result: Container to add this to
 * tagId: Id
 * tagSrc: Source
 * tagAltSrc: Alternet Source
 * tagClass: class
 * tagWidth: Width
 * tagHeight: Height
 * tagTitle: Title
 */
Wt::WWidget* WidgetFunction::getFlashBack(Wt::WContainerWidget *result, std::string tagId, std::string tagSrc, std::string tagAltSrc, std::string tagClass, std::string tagWidth, std::string tagHeight, std::string tagTitle, std::string tagAlign)
{
    // FIXME: this will not center unless you center it,
    Wt::WMediaPlayer *player = new Wt::WMediaPlayer(Wt::WMediaPlayer::Audio, result);
    if (tagAlign == "center")
    {
        player->setStyleClass("centerit");
    }
    if(!tagId.empty())     { /* FIXME may be a bug: player->setId(tagId); */ }
    if(!tagClass.empty())  { player->setStyleClass(tagClass); }
    if(!tagWidth.empty())  { player->setWidth(std::stoi(tagWidth)); }
    if(!tagHeight.empty()) { player->setHeight(std::stoi(tagHeight)); }
    if(!tagSrc.empty())    { player->addSource(Wt::WMediaPlayer::MP3, Wt::WLink(tagSrc)); }
    if(!tagAltSrc.empty()) { player->addSource(Wt::WMediaPlayer::OGA, Wt::WLink(tagAltSrc)); }
    if(!tagTitle.empty())  { player->setTitle(tagTitle); }
    // FIXME, what if they do not have Flash support? player->setAlternativeContent(new Wt::WText(Wt::WString::tr("Browser-No-Audio-Support")));
    return player;
} // end getFlashBack
// --- End Of File ------------------------------------------------------------
