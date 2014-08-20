#ifdef VIDEOMAN
#ifndef VIDEOMAN_H
#define VIDEOMAN_H
//
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
/* ****************************************************************************
 * Video Man
 * categories:    coma delimited array of Categories for Categories Combo box, each in its own Combo Box
 *                  Make|Model|Year
 * name:             Name for Combo box
 * title:            Title for Video Player
 * language:         en, cn, ru...
 * path:             Full Path minus video extension and quality
 * aspectratio:      Aspect Ratio:
 * ism4v:            is video m4v extension
 * isogv:            is video ogv extension
 * isUtube:          is Yew Tube video
 * isautoplay:       is autoplay
 * poster:           Full path to image
 * width:            Width of Video in pixils
 * height:           Height of Video in pixils
 * pagetop:          page XHTML Source
 * pagetopwidth:     page Width: 40px, 100%
 * pagetopheight:    page Height: 40px, 100%
 * pagebottom:       page XHTML Source
 * pagebottomwidth:  page Width: 40px, 100%
 * pagebottomheight: page Height: 40px, 100%
 */
class VideoMan : public Wt::Dbo::Dbo<VideoMan>
{
    public:
        VideoMan();
        //
        std::string categories;         // categories for Combo box
        std::string name;               // Name for Combo box
        std::string title;              // Title for Video Player
        std::string language;           // language: en, cn, ru...
        std::string path;               // Full Path minus video extension and quality
        std::string aspectRatio;        // Aspect Ratio
        int isM4v;                      // is video m4v extension
        int isOgv;                      // is video ogv extension
        int isYouTube;                  // is You Tube video
        int isAutoPlay;                 // is autoplay
        std::string poster;             // Full path to image
        std::string width;              // Width of Video
        std::string height;             // Height of Video
        std::string sizes;              // coma delimited array of sizes: 1080,720 | Create check box
        std::string quality;            // coma delimited array of quality: hd,lq | Create check box
        Wt::WString pagetop;            // page Location: iframe = fully qualified URL path to html file, xml = message ID
        std::string pagetopwidth;       // page Width: 40px, 100%
        std::string pagetopheight;      // page Height: 40px, 100%
        Wt::WString pagebottom;         // page HTML in 1 line, no returns, no quotes
        std::string pagebottomwidth;    // page Width: 40px, 100%
        std::string pagebottomheight;   // page Height: 40px, 100%
        //
        template<class Action>
        void persist(Action &a)
        {
            Wt::Dbo::field(a, categories,       "categories");
            Wt::Dbo::field(a, name,             "name");
            Wt::Dbo::field(a, title,            "title");
            Wt::Dbo::field(a, language,         "language");
            Wt::Dbo::field(a, path,             "path");
            Wt::Dbo::field(a, aspectRatio,      "aspectratio");
            Wt::Dbo::field(a, isM4v,            "ism4v");
            Wt::Dbo::field(a, isOgv,            "isogv");
            Wt::Dbo::field(a, isYouTube,        "isutube");
            Wt::Dbo::field(a, isAutoPlay,       "isautoplay");
            Wt::Dbo::field(a, poster,           "poster");
            Wt::Dbo::field(a, width,            "width");
            Wt::Dbo::field(a, height,           "height");
            Wt::Dbo::field(a, sizes,            "sizes");
            Wt::Dbo::field(a, quality,          "quality");
            Wt::Dbo::field(a, pagetop,          "pagetop");
            Wt::Dbo::field(a, pagetopwidth,     "pagetopwidth");
            Wt::Dbo::field(a, pagetopheight,    "pagetopheight");
            Wt::Dbo::field(a, pagebottom,       "pagebottom");
            Wt::Dbo::field(a, pagebottomwidth,  "pagebottomwidth");
            Wt::Dbo::field(a, pagebottomheight, "pagebottomheight");
        }
}; // end class VideoMan
#endif // VIDEOMAN_H
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
