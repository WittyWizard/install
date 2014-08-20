#ifdef VIDEOMAN
#ifndef VIDEOMANIMPL_H
#define VIDEOMANIMPL_H
#include <Wt/WApplication>
//#include <Wt/WEnvironment>
//#include <Wt/WLogger>
#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WViewWidget>
#include <Wt/WImage>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
//#include <Wt/WIntValidator>
#include <Wt/WAudio>

#ifdef WVIDEO
    #include <Wt/WVideo>
#else
    #include <Wt/WMediaPlayer>
#endif
// Database
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/QueryModel>
//
#include <boost/algorithm/string.hpp>
//
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "model/VideoManSession.h"
#include "WidgetFunction.h"
#include "Crystallball.h"
/* ****************************************************************************
 * Video Man Implement
 * appPath:
 * basePath:
 * connectionPool:
 * lang:
 */
class VideoManImpl : public Wt::WContainerWidget
{
    public:
        VideoManImpl(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName);
        void SetInternalBasePath(const std::string& basePath);
        virtual ~VideoManImpl();
        /* --------------------------------------------------------------------
         * session
         */
        VideoManSession& session()  { return session_;  }

    private:
        // Init in constructor
        std::string appPath_;
        std::string basePath_;
        VideoManSession session_;
        std::string lang_;
        std::string useDb_;
        std::string domainName_;
        //
        void refresh();
        void Init();
        void MakeVideo();
        void HandlePathChange(const std::string& path);

        void ComboCategoryChanged();
        void ComboVideoChanged();
        bool CreateCategoryCombo();
        bool CreateCategoryComboDb();
        bool CreateCategoryComboXml();
        bool CreateVideoCombobox();
        bool CreateVideoComboboxDb();
        bool CreateVideoComboboxXml();
        void GetVideo();
        std::string GetCategories(std::string delimitor);
        bool GetCategoriesPath();
        void ClearCategories();
        Wt::WWidget* GetPage(Wt::WString src);
        Wt::WWidget* WrapView(Wt::WWidget *(VideoManImpl::*createWidget)());
        Wt::WWidget* PageContentTop();
        Wt::WWidget* PageContentBottom();
        Wt::WWidget* YouTubeContent();
        Wt::WWidget* GetAudioPlayer();
        bool GetVideoDb();
        bool GetVideoXml();
        void Edit();
        bool BindTemplate(std::string resourceFile, std::string filePath, std::string templateName);
        void BindTags(const rapidxml::xml_node<>* node);
        void SetPath();
        Wt::WWidget* GetTemplate();
        //
        std::string thePath = "";
        //
        Wt::WApplication *app = NULL;
        std::string myTheme = "blue";
        std::string defaultPageTopHeight = "800px";
        std::string defaultPageTopWidth = "790px";
        std::string defaultPageBottomHeight = "800px";
        std::string defaultPageBottomWidth = "790px";
        //
        Wt::WString TextPageTopIframe;     // Top Page iFrame
        Wt::WString TextPageBottomDiv;  // Bottom Page iFrame
        Wt::WString TextYouTubeIframe;     // You-Tube iFrame
        //
        Wt::WTemplate* videoManTemplate = NULL;
        Wt::WWidget* videoManPage_ = NULL;
        bool isVideo = false;
        bool bindVideo = false;
        bool isChanged = false;
        Wt::WContainerWidget* items_ = NULL;
        Wt::WContainerWidget* bindItems = NULL;
        std::string filePath = "";
        // ComboBoxs
        int numberCateories = -1;
        Wt::WComboBox* ComboCategory_0 = NULL;  // Category 0
        Wt::WComboBox* ComboCategory_1 = NULL;  // Category 1
        Wt::WComboBox* ComboCategory_2 = NULL;  // Category 2
        Wt::WComboBox* ComboCategory_3 = NULL;  // Category 3
        Wt::WComboBox* ComboCategory_4 = NULL;  // Category 4
        Wt::WComboBox* ComboCategory_5 = NULL;  // Category 5
        Wt::WComboBox* ComboVideo = NULL;       // Video Name
        Wt::WComboBox* ComboSizes = NULL;       // Sizes: sizes="1080,720"
        Wt::WComboBox* ComboQuality = NULL;     // quality="hd,lq"
        //
        std::string mp4Video = "";
        std::string ogvVideo = "";
        std::string poster = "";
        std::string title = "";
        std::string size = "";
        std::string quality = "";
        bool isutube = false;
        std::string categoryQuery = "";  // Full Path from Database: Cat-1|Cat-2
        std::string categoryPath = "";   // Full Path from Database: Cat-1/Cat-2
        std::string categoryText_0 = ""; // First Category:  Cat-1
        std::string categoryText_1 = ""; // Second Category: Cat-2
        std::string categoryText_2 = ""; //
        std::string categoryText_3 = ""; //
        std::string categoryText_4 = ""; //
        std::string categoryText_5 = ""; //
        std::string videoText = "";      // Video Name
        int currentVideoIndex = -1;
        int oldVideoIndex = -2;
        //
        std::string myInternalPath = "";
        //
        struct wtTags
        {
            std::string type = "";
            std::string id = "";
            std::string myClass = "";
            std::string width = "";
            std::string height = "";
            std::string src = "";
        } wtTag;
        //
        #ifdef WVIDEO
            Wt::WVideo* player;
        #else
            Wt::WMediaPlayer* player = NULL;
        #endif
        #ifdef USE_TEMPLATE
            Wt::WTemplate* videoTemplate = NULL;
        #endif
};
#endif // VIDEOMANIMPL_H
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
