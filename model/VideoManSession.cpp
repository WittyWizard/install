/* ****************************************************************************
 * Video Man Session
 */
#ifdef VIDEOMAN
#include "VideoManSession.h"
/* ****************************************************************************
 * Video Man Session
 * appPath: Internal Path ~ /lang/video/...
 * connectionPool:
 */
VideoManSession::VideoManSession(const std::string& appPath, const std::string& useDb, Wt::Dbo::SqlConnectionPool& connectionPool) : appPath_(appPath), useDb_(useDb), connectionPool_(connectionPool)
{
    Wt::log("start") << " *** VideoManSession::VideoManSession() *** ";
    if (useDb == "1")
    {
        setConnectionPool(connectionPool_);
        mapClass<VideoMan>("videoman");
        Wt::WApplication* app = Wt::WApplication::instance();
        std::string path = app->internalPath(); // /lang/video/
        bool doXmlUpdate = false; // FIXME add security for logon or certificate
        if (path.find("/updatexml") != std::string::npos)
        {
            doXmlUpdate = true;
        }
        // FIXIT check for admin login
        if (Crystallball::InitDb || doXmlUpdate)
        {
            try
            {
                Wt::Dbo::Transaction t(*this);
                // Note: you must drop tables to do VideoManSession::ImportXML, FIXIT make it a url in the backend with credentials
                if (doXmlUpdate)
                {
                    Wt::log("warning") << " *** VideoManSession::VideoManSession() dropTables *** ";
                    dropTables();
                }
                t.commit();
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Using existing video database";
                Wt::log("error") << " *** VideoManSession::VideoManSession() Using existing video database *** ";
            }
            try
            {
                Wt::Dbo::Transaction t(*this);
                createTables();
                Wt::log("error") << " *** VideoManSession::VideoManSession() Created database: video *** ";
                t.commit();
                if (!ImportXML())
                {
                    Wt::log("error") << " *** VideoManSession::VideoManSession() ImportXML failed! *** ";
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Using existing video database";
                Wt::log("error") << " *** VideoManSession::VideoManSession() Using existing video database *** ";
            }
        }
    } // end if (useDb == "1")
    Wt::log("end") << " *** VideoManSession::VideoManSession() *** ";
} // end VideoManSession::VideoManSession
/* ****************************************************************************
 * Video Man Session
 * Check Database for existing records and delete them
 * Read in XML file and populate Database
 */
bool VideoManSession::ImportXML()
{
    Wt::log("start") << " *** VideoManSession::ImportXML()  *** ";
    std::string fullFilePath = appPath_ + "videoman-db-import.xml";
    if (Crystallball::IsFile(fullFilePath))
    {
        Wt::log("info") << "VideoManSession::ImportXML: " << fullFilePath;
    }
    else
    {
        Wt::log("error") << "-> Missing XML Configuration File VideoManSession::ImportXML: " << fullFilePath;
        return false;
    }
    try
    {
        // Open XML File
        const char *filePath = fullFilePath.c_str();
        rapidxml::file<> xmlFile(filePath);
        rapidxml::xml_document<> xmlDoc;
        xmlDoc.parse<0>(xmlFile.data());
        // Find our root node
        /*
         *
<?xml version="1.0" encoding="ISO-8859-1" ?>
<videos>
    <!-- 3 Comboboxes -->
    <category name="Make|Model|Year" scheme="make|model|year">
        <video name="66 Dodge Cuda"        categories="Dodge|Cudu|1966"        title="426 Hemi" path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="77 Dodge Power Wagon" categories="Dodge|Power Wagon|1977" title="318"      path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="69 Plymoth Firebird"  categories="Plymoth|Firebird|1969"  title="350"      path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
    </category>
    <!-- 1 Comboboxes -->
    <category name="Color" scheme="color">
        <video name="Blue"  categories="Blue"  title="The Color Blue"  path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="Red"   categories="Red"   title="The Color Red"   path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="White" categories="White" title="The Color White" path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
    </category>
    <!-- 0 Comboboxes -->
    <category name="" scheme="">
        <video name="Video 1" categories="" title="First Video"  path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="Video 2" categories="" title="Second Video" path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
        <video name="Video 3" categories="" title="Third Video"  path="http://domain.tdl/path/filename" ism4v="1" isogv="0" isutube="0" poster="http://domain.tdl/path/fileName.png" width="640" height="360" isautoplay="0" sizes="1080,720" quality="hd,lq" pages=""></video>
    </category>
</videos>
        */
        rapidxml::xml_node<> * root_node = xmlDoc.first_node("category");
        rapidxml::xml_attribute<> *nodeAttrib;
        for (rapidxml::xml_node<> * domain_node = root_node->first_node("videoman"); domain_node; domain_node = domain_node->next_sibling("videoman"))
        {
            Wt::log("progress") << "VideoManSession::ImportXML: Start Loop = " << domain_node->name();
            // Start a Transaction
            Wt::Dbo::Transaction t(*this);
            // Create a new Video Instance
            Wt::Dbo::ptr<VideoMan> thisVideo = add(new VideoMan());
            // Set object to Modify
            VideoMan *videoDb = thisVideo.modify();
            // name of combo box
            nodeAttrib = domain_node->first_attribute("name");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: name = " << domain_node->name();
                return false;
            }
            videoDb->name = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: name = " << nodeAttrib->value();
            // categories
            nodeAttrib = domain_node->first_attribute("categories");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: categories = " << domain_node->name();
                return false;
            }
            videoDb->categories = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: categories = " << nodeAttrib->value();
            // title
            nodeAttrib = domain_node->first_attribute("title");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: title = " << domain_node->name();
                return false;
            }
            videoDb->title = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: title = " << nodeAttrib->value();
            // language
            nodeAttrib = domain_node->first_attribute("language");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: language = " << domain_node->name();
                return false;
            }
            videoDb->language = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: language = " << nodeAttrib->value();
            // path
            nodeAttrib = domain_node->first_attribute("path");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: path = " << domain_node->name();
                return false;
            }
            videoDb->path = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: path = " << nodeAttrib->value();
            // aspectratio
            nodeAttrib = domain_node->first_attribute("aspectratio");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: aspectratio = " << domain_node->name();
                return false;
            }
            videoDb->aspectRatio = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: aspectratio = " << nodeAttrib->value();
            // ism4v
            nodeAttrib = domain_node->first_attribute("ism4v");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: ism4v = " << domain_node->name();
                return false;
            }
            videoDb->isM4v = std::stoi(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: ism4v = " << nodeAttrib->value();
            // isogv
            nodeAttrib = domain_node->first_attribute("isogv");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: isogv = " << domain_node->name();
                return false;
            }
            videoDb->isOgv = std::stoi(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: isogv = " << nodeAttrib->value();
            // isutube
            nodeAttrib = domain_node->first_attribute("isutube");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: isutube = " << domain_node->name();
                return false;
            }
            videoDb->isYouTube = std::stoi(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: isutube = " << nodeAttrib->value();
            // poster
            nodeAttrib = domain_node->first_attribute("poster");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: poster = " << domain_node->name();
                return false;
            }
            videoDb->poster = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: poster = " << nodeAttrib->value();
            // width
            nodeAttrib = domain_node->first_attribute("width");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: width = " << domain_node->name();
                return false;
            }
            videoDb->width = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: width = " << nodeAttrib->value();
            // height
            nodeAttrib = domain_node->first_attribute("height");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: height = " << domain_node->name();
                return false;
            }
            videoDb->height = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: height = " << nodeAttrib->value();
            // isautoplay stored as a 1=true, 0=false
            nodeAttrib = domain_node->first_attribute("isautoplay");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: isautoplay = " << domain_node->name();
                return false;
            }
            videoDb->isAutoPlay = std::stoi(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: isautoplay = " << nodeAttrib->value();
            // sizes
            nodeAttrib = domain_node->first_attribute("sizes");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: sizes = " << domain_node->name();
                return false;
            }
            videoDb->sizes = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: sizes = " << nodeAttrib->value();
            // quality
            nodeAttrib = domain_node->first_attribute("quality");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: quality = " << domain_node->name();
                return false;
            }
            videoDb->quality = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: quality = " << nodeAttrib->value();
            // pagetop
            nodeAttrib = domain_node->first_attribute("pagetop");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagetop = " << domain_node->name();
                return false;
            }
            videoDb->pagetop = Wt::WString::fromUTF8(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: pagetop"; // << mypagetop;
            // pagetopwidth
            nodeAttrib = domain_node->first_attribute("pagetopwidth");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagetopwidth = " << domain_node->name();
                return false;
            }
            videoDb->pagetopwidth = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: pagetopwidth = " << nodeAttrib->value();
            // pagetopheight
            nodeAttrib = domain_node->first_attribute("pagetopheight");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagetopheight = " << domain_node->name();
                return false;
            }
            videoDb->pagetopheight = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: pagetopheight = " << nodeAttrib->value();
            // pagebottom
            nodeAttrib = domain_node->first_attribute("pagebottom");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagebottom = " << domain_node->name();
                return false;
            }
            videoDb->pagebottom = Wt::WString::fromUTF8(nodeAttrib->value());
            Wt::log("progress") << "VideoManSession::ImportXML: pagebottom"; // << mypagebottom;
            // pagebottomwidth
            nodeAttrib = domain_node->first_attribute("pagebottomwidth");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagebottomwidth = " << domain_node->name();
                return false;
            }
            videoDb->pagebottomwidth = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: pagebottomwidth = " << nodeAttrib->value();
            // pagebottomheight
            nodeAttrib = domain_node->first_attribute("pagebottomheight");
            if (!nodeAttrib)
            {
                Wt::log("error") << "VideoManSession::ImportXML: Missing XML Element: pagebottomheight = " << domain_node->name();
                return false;
            }
            videoDb->pagebottomheight = nodeAttrib->value();
            Wt::log("progress") << "VideoManSession::ImportXML: pagebottomheight = " << nodeAttrib->value();
            // Commit Transaction
            t.commit();
        } // end for
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManSession::ImportXML(): Failed writting to videoman database";
        Wt::log("error") << "-> VideoManSession::ImportXML()  Failed writting to videoman database: " << e.what();
        return false;
    }
    Wt::log("end") << "VideoManSession::ImportXML()";
    return true;
} // end void VideoManSession::VideoManSession::ImportXML
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
