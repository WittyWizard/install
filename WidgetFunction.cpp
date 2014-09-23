/* ****************************************************************************
 * Widget Function
 * Provided by Koen Deforche
 *
 */
#include "WidgetFunction.h"
/* ****************************************************************************
 * MyWidgetFunction
 * Do not use: &ldquo; &rdquo; &colon; &dollar; &lcub; &rcub; &amp; & “” ‘’ ¾ – <>
 */
namespace MyWidgetFunction
{
    std::map <std::string, Wt::WComboBox *> ComboSizes;
    std::map <std::string, Wt::WComboBox *> ComboQuality;
}
/* ****************************************************************************
 * get TypeFound
 */
bool WidgetFunction::doAddFunction()
{
    return typeFound;
} // end getTemplate
/* ****************************************************************************
 * set Template
 * widgets: this is the template or content
 * widgetFunction: this does not work, not sure how to fix this
 */
void WidgetFunction::setTemplate(Wt::WString widgets, WidgetFunction widgetFunction)
{
    (void)widgetFunction;
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
    if (widgets.toUTF8().find("widget:video") != std::string::npos)
    {
        Wt::log("notice") << " WidgetFunction::WidgetFunction() found video tag: useWidgetFun == 1";
        registerType("video", WidgetFunction::createVideo);
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
 *
 * tag: ${widget:video id='edit' class='wteditor' type='editor' width='580' height='33' maxlength='16' emptytext='Change Text' }
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
 * Create Video
 * static
 * args: vector of WString
 *
 * You-Tube Video
 * tag: ${widget:video id='video1' class='wtvideo' player='youtube' src='//www.youtube.com/embed/VKFfkBc0s1o' title='Title' next='' previous='' nplocation='top' align='center' posterimage='' mp4video='0' ogvvideo='0' sizes='1080,720' quality='hd,lq' aspectratio='16by9' width='640' height='360'}
 * WVideo
 * tag: ${widget:video id='video1' class='wtvideo' player='WVideo' src='//www.youtube.com/embed/VKFfkBc0s1o' aspectRatio='16by9' width='640' height='360'}
 * WMediaPlayer
 * tag: ${widget:video id='video1' class='wtvideo' player='WMediaPlayer' src='//www.youtube.com/embed/VKFfkBc0s1o' aspectRatio='16by9' width='640' height='360'}
 */
Wt::WWidget* WidgetFunction::createVideo(const std::vector<Wt::WString>& args)
{
    // ID
    std::string myId = getArg("id", args);
    if (myId.empty()) { new Wt::WText("Missing ID"); } // FIXME: Wt::WString::tr("no-id")
    // Source
    std::string mySource = getArg("src", args);
    if (!mySource.empty()) { new Wt::WText("Missing Source"); } // FIXME: Wt::WString::tr("no-source")
    // next
    std::string myNext = getArg("next", args);
    // previous
    std::string myPrevious = getArg("previous", args);
    // nplocation
    std::string myNpLocation = getArg("nplocation", args);
    if (myNpLocation.empty()) { myNpLocation = "top"; }
    // align
    std::string myAlign= getArg("align", args);
    if (myAlign.empty()) { myAlign = "center"; }
    // Create a Container
    Wt::WContainerWidget* container = new Wt::WContainerWidget();
    Wt::WContainerWidget *npContainer = new Wt::WContainerWidget();
    // FIXME: defaults for each attribute
    // class
    std::string myClass = getArg("class", args);
    if (myClass.empty()) { myClass = "wtvideo"; }
    // width
    std::string myWidth = getArg("width", args);
    if (myWidth.empty()) { myWidth = "640"; }
    // height
    std::string myHeight = getArg("height", args);
    if (myHeight.empty()) { myHeight = "360"; }
    // Poster Image
    std::string myPosterImage = getArg("posterimage", args);
    if (myPosterImage.empty()) { myPosterImage = ""; }
    // Title
    std::string myTitle = getArg("title", args);
    if (myTitle.empty()) { myTitle = ""; }
    // Mp4 Video
    std::string myMp4Video = getArg("mp4video", args);
    if (myMp4Video.empty()) { myMp4Video = "0"; }
    // Ogv Video
    std::string myOgvVideo = getArg("ogvvideo", args);
    if (myOgvVideo.empty()) { myOgvVideo = "0"; }
    // Sizes
    std::string mySizes = getArg("sizes", args);
    if (mySizes.empty()) { mySizes = "1080,720"; }
    std::vector <std::string> sizesFields;
    boost::split( sizesFields, mySizes, boost::is_any_of( "," ) );
    // Quality
    std::string myQuality = getArg("quality", args);
    if (myQuality.empty()) { myQuality = "hd,lq"; }
    // Aspect Ratio
    std::string myAspectRatio = getArg("aspectratio", args);
    if (myAspectRatio.empty()) { myAspectRatio = "16by9"; }
    // Player
    std::string myPlayer = getArg("player", args);
    if (myPlayer.empty()) { myPlayer = "WVideo"; }
    //
    if (!myNext.empty() || !myPrevious.empty())
    {
        Wt::WToolBar* toolBar = new Wt::WToolBar(npContainer);
        toolBar->setStyleClass("centerit");
        if (!myPrevious.empty())
        {
            toolBar->addButton(WidgetFunction::createButton(Wt::WString::tr("previous"), "", myPrevious));
        }
        if (!myNext.empty())
        {
            toolBar->addButton(WidgetFunction::createButton(Wt::WString::tr("next"), "", myNext));
        }
    }
    if (myNpLocation == "top" && (!myNext.empty() || !myPrevious.empty()))
    {
        container->addWidget(npContainer);
    }
    //
    if (myPlayer == "WVideo" || myPlayer == "WMediaPlayer")
    {
        /* sizes="1080,720"
         *
         */
        //
        std::string size = sizesFields[0]; // FIXIT read from combobox
        int sizeCount = sizesFields.size();
        if (sizeCount > 1)
        {
            // Create a dropdown box
            if(MyWidgetFunction::ComboSizes.find(myId) == MyWidgetFunction::ComboSizes.end())
            {
                MyWidgetFunction::ComboSizes[myId] = new Wt::WComboBox(container);
                for (int sizecnt = 0; sizecnt < sizeCount; sizecnt++)
                {
                    if (MyWidgetFunction::ComboSizes[myId]->findText(sizesFields[sizecnt]) == -1)
                    {
                        MyWidgetFunction::ComboSizes[myId]->addItem(sizesFields[sizecnt]); // it did not exist, add it
                    }
                    MyWidgetFunction::ComboSizes[myId]->setCurrentIndex(0);
                }
            }
            size = MyWidgetFunction::ComboSizes[myId]->currentText().toUTF8();
        }
        //  quality="hd,lq"
        std::vector <std::string> qualityFields;
        boost::split( qualityFields, myQuality, boost::is_any_of( "," ) );
        int qualityCount = qualityFields.size();
        myQuality = qualityFields[0]; // FIXME read from combobox
        if (qualityCount > 1)
        {
            // Create a dropdown box
            if(MyWidgetFunction::ComboQuality.find(myId) == MyWidgetFunction::ComboQuality.end())
            {
                MyWidgetFunction::ComboQuality[myId] = new Wt::WComboBox(container);
                Wt::log("notice") << "VideoManImpl::GetVideoDb()  new ComboBox MyWidgetFunction::ComboQuality";
                for (int qualitycnt = 0; qualitycnt < sizeCount; qualitycnt++)
                {
                    if (MyWidgetFunction::ComboQuality[myId]->findText(qualityFields[qualitycnt]) == -1)
                    {
                        MyWidgetFunction::ComboQuality[myId]->addItem(qualityFields[qualitycnt]); // it did not exist, add it
                    }
                    MyWidgetFunction::ComboQuality[myId]->setCurrentIndex(0);
                } // end for (int qualitycnt=0;qualitycnt<sizeCount;qualitycnt++)
            }
            myQuality = MyWidgetFunction::ComboQuality[myId]->currentText().toUTF8();
        } // end if (qualityCount > 1)
        //
        std::string mp4Video = "";
        std::string ogvVideo = "";
        if (myMp4Video == "1")
        {
            mp4Video = mySource + "-" + myQuality + "-" + size + ".m4v";
        }
        if (myOgvVideo == "1")
        {
            ogvVideo = mySource + "-" + myQuality + "-" + size + ".ogv";
        }
        if (myPlayer == "WVideo")
        {
            Wt::WVideo* videoPlayer = new Wt::WVideo(container);


            if (!mp4Video.empty())
            {
                videoPlayer->addSource(mp4Video);
            }
            if (!ogvVideo.empty())
            {
                videoPlayer->addSource(ogvVideo);
            }
            if (!myPosterImage.empty())
            {
                videoPlayer->setPoster(myPosterImage);
                videoPlayer->setAlternativeContent(new Wt::WImage(myPosterImage)); // FIXME: Do I need to do both?
            }
            videoPlayer->resize(std::stoi(myWidth), std::stoi(myHeight));

        }
        else if (myPlayer == "WMediaPlayer")
        {
            Wt::WMediaPlayer* mediaPlayer = new Wt::WMediaPlayer(Wt::WMediaPlayer::Video, container);
            if (!mp4Video.empty())
            {
                mediaPlayer->addSource(Wt::WMediaPlayer::M4V, Wt::WLink(mp4Video));
            }
            if (!ogvVideo.empty())
            {
                mediaPlayer->addSource(Wt::WMediaPlayer::OGV, Wt::WLink(ogvVideo));
            }
            if (!myPosterImage.empty())
            {
                mediaPlayer->addSource(Wt::WMediaPlayer::PosterImage, Wt::WLink(myPosterImage));
            }
            if (!myTitle.empty())
            {
                mediaPlayer->setTitle(myTitle);
            }
            mediaPlayer->setVideoSize(std::stoi(myWidth), std::stoi(myHeight));
        }
    }
    else if (myPlayer == "youtube")
    {
        std::string aspectRatio;
        if (myAspectRatio == "16by9")
        {
            aspectRatio = "embed-responsive embed-responsive-16by9";
        }
        else
        {
            aspectRatio = "embed-responsive embed-responsive-4by3";
        }
        Wt::WText *w = new Wt::WText("<div id='vp-" + myId + "' class='vplayer" + myAlign + "'><div id='yewtube' class='" + aspectRatio + "'><iframe id='vframe' class='embed-responsive-item' src='" + mySource + "' width='" + myWidth + "' height='" + myHeight + "' style='' frameBorder='0' scrolling='no' allowfullscreen='true'></iframe></div></div>", Wt::XHTMLUnsafeText, container);
        w->setInternalPathEncoding(true);
    }
    if (myNpLocation == "bottom" && (!myNext.empty() || !myPrevious.empty()))
    {
        container->addWidget(npContainer);
    }
    return container;
} // end createLineEdit
/* ****************************************************************************
 * Create Audio
 * static
 * args: vector
 * tag: ${widget:audio id='widget_ID' align='center,left or right' class='wtaudio' type='Enchancments' width='580' height='33' src='/resources/source_path.mp3' altsrc='/resources/source.ogg' title='Title'}
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
/* ****************************************************************************
 * createButton
 */
Wt::WPushButton* WidgetFunction::createButton(const Wt::WString& text, const std::string className, const std::string link)
{
    Wt::WPushButton *button = new Wt::WPushButton();
    button->setTextFormat(Wt::XHTMLText);
    button->setText(text);
    if (!className.empty()) { button->addStyleClass(className); }
    button->setLink(Wt::WLink(Wt::WLink::InternalPath, link));
    return button;
} // end createButton
/* ****************************************************************************
 * String Replace
 * StringReplace(string, "this", "that");
 */
bool WidgetFunction::StringReplace(std::string& string2replace, const std::string& changefrom, const std::string& changeTo)
{
    size_t start_pos = string2replace.find(changefrom);
    if(start_pos == std::string::npos) { return false; }
    string2replace.replace(start_pos, changefrom.length(), changeTo);
    return true;
} // end StringReplace
// --- End Of File ------------------------------------------------------------
