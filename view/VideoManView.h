/* ****************************************************************************
 * Video Man View
 */
#ifdef VIDEOMAN
#ifndef VIDEOMANVIEW_H
#define VIDEOMANVIEW_H
//
#include <Wt/WCompositeWidget>
#include "VideoManImpl.h"
/* ****************************************************************************
 * Prototype VideoManImpl
 */
class VideoManImpl;
/* ****************************************************************************
 * Video Man View
 */
class VideoManView : public Wt::WCompositeWidget
{
    public:
        VideoManView(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& db, const std::string& lang, const std::string& useDb, const std::string& domainName);
        void SetInternalBasePath(const std::string& basePath);
    private:
        VideoManImpl* impl_;
}; // end class VideoManView
#endif // VIDEOMANVIEW_H
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
