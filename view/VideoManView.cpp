#ifdef VIDEOMAN
/* ****************************************************************************
 * Video Manager View
 */
#include "VideoManView.h"
/* ****************************************************************************
 * Video Man View
 * appPath: appRoot() + "home/" + domainName + "/"
 * basePath: "/" + langPath + "/video/"
 * connectionPool: SQL Database Connection Pool
 * lang: Language
 */
VideoManView::VideoManView(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName)
{
    impl_ = new VideoManImpl(appPath, basePath, connectionPool, lang, useDb, domainName);
    setImplementation(impl_);
} // end VideoManView
/* ****************************************************************************
 * Set Internal Base Path
 * basePath:
 */
void VideoManView::SetInternalBasePath(const std::string& basePath)
{
    impl_->SetInternalBasePath(basePath);
} // end SetInternalBasePath
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
