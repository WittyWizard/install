/* ****************************************************************************
 * Witty Wizard CrystalBall is a set of Global Functions and Variables.
 */
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WLogger>
//
#include <locale>
#include <iomanip>
#include <locale>
#include <sys/stat.h>
#include <map>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <dirent.h>
// Database
#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Dbo>
#ifdef POSTGRES
    #include <Wt/Dbo/backend/Postgres>
#elif SQLITE3
    #include <Wt/Dbo/backend/Sqlite3>
#elif MYSQL
    #include <Wt/Dbo/backend/MySQL>
#elif FIREBIRD
    #include <Wt/Dbo/backend/Firebird>
#endif // FIREBIRD
#include <Wt/Dbo/FixedSqlConnectionPool>
#ifdef WWUSEBOOST
    #include <boost/filesystem.hpp>
#endif
// DO NOT INCLUDE #include "WittyWizard.h" (its defined as External, this is like C, its Global)
namespace CrystalBall
{
    /* ****************************************************************************
     * Global Version Number and Date last Modified for logs, make sure its this version.
     */
    std::string Version = "Date: 20 Sep 2014 - Version: 1.0.3";
    /* ****************************************************************************
     * Global Variable
     * root Prefix: Used to set the URL Path: http:domain.tdl\prefix\root-path
     * See Wt Documenation on: --deploy-path=
     * This is used to fix path issues in Wt, must be in run and in path of all menu items and resources
     */
    std::string RootPathPrefix="ww";
    /* ****************************************************************************
     * Global Variable
     * Used to create database, turn off afterwards so you do not get errors
     * FIXME make admin function to change this on the fly * /admin/init
     */
    bool InitDb = false;
    /* ****************************************************************************
     * Global map
     * Connection Pool Map
     * Holds the Pointer to Wt::Dbo::SqlConnectionPool *
     */
    std::map <std::string, boost::any> ConnectionPool;
    /* ****************************************************************************
     * Global Variable
     * domainHost="wittywizard.org"
     */
    std::map <std::string, std::string> DomainHost;
    /* ****************************************************************************
     * Default Language Index
     * This is added in HomeFundation AddLanguage
     * You need this when you edit a pages content, save as docName_lang.xml
     */
    std::map <std::string, int> DefaultLanguageIndex;
    /* ****************************************************************************
     * Global Variable
     * UseDb="0": 1 = true, 0 = false
     */
    std::map <std::string, std::string> UseDb;
    /* ****************************************************************************
     * Global Variable
     * dbName="wittywizard"
     */
    std::map <std::string, std::string> DbName;
    /* ****************************************************************************
     * Global Variable
     * dbUser="wittywizard"
     */
    std::map <std::string, std::string> DbUser;
    /* ****************************************************************************
     * Global Variable
     * password="The1Wizard2Witty4Flesh"
     */
    std::map <std::string, std::string> DbPassword;
    /* ****************************************************************************
     * Global Variable
     * dbPort="5432"
     */
    std::map <std::string, std::string> DbPort;
    /* ****************************************************************************
     * Global Variable
     * gasAccount="pub-1083602596491715"
     *
     */
    std::map <std::string, std::string> GasAccount;
    /* ****************************************************************************
     * Global Variable
     * gaAccount="UA-xxxxxxxx-x"
     * Google Analytic Account: See domain.xml
     * http://google.com/analytics/
     */
    std::map <std::string, std::string> GaAccount;
    /* ****************************************************************************
     * Global Variable
     * moduleIncludes="home|chat|blog|about|contact|video"
     * moduleIncludes: Each Menu Item Included
     * See domain.xml:includes="home|chat|blog|about|contact|video"
     */
    std::map <std::string, std::string> ModuleIncludes;
    /* ****************************************************************************
     * Global Variable
     * languages="|en|de|cn|ru|"
     * See domain.xml:languages="|en|de|cn|ru|"
     */
    std::map <std::string, std::string> Languages;
    /* ****************************************************************************
     * Global Variable
     * See domain.xml:themes="|red|white|blue|green|tan|default|"
     */
    std::map <std::string, std::string> Themes;
    /* ****************************************************************************
     * Global Variable
     * defaultTheme="blue"
     * See domain.xml:defaultTheme="blue"
     */
    std::map <std::string, std::string> DefaultTheme;
    /* ****************************************************************************
     * Global Variable
     * google-site-verification
     */
    std::map <std::string, std::string> GoogleSiteVerifi;
    /* ****************************************************************************
     * Global Variable
     * y_key: Yahoo Site Verisfication Key
     */
    std::map <std::string, std::string> Ykey;
    /* ****************************************************************************
     * Global Variable
     * msvalidate.01
     */
    std::map <std::string, std::string> MsValidate;
    /* ****************************************************************************
     * Global Function
     * is File: Full Path to File: /home/domain.tdl/filename.ext
     */
    bool IsFile(const std::string& name)
    {
        #ifdef WWUSEBOOST
        // which is faster?
        return boost::filesystem::exists(name);
        #else
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
        #endif
    }
    /* ****************************************************************************
     * Global Function
     * is Path: Full Path: /home/domain.tdl/
     */
    bool IsPath(const std::string& pathName)
    {
        struct stat sb;
        if (stat(pathName.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            { return true; }
        return false;
    }
    /* ****************************************************************************
     * Set Cookie
     */
    void SetCookie(std::string name, std::string myValue)
    {
        Wt::WApplication *app = Wt::WApplication::instance();
        try
        {
            app->setCookie(name, myValue, 150000, "", "/", false);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "CrystalBall::SetCookie: Failed writting cookie: " << name;
            Wt::log("error") << "CrystalBall::SetCookie()  Failed writting cookie: " << name;
        }
    } // end SetCookie
    /* ****************************************************************************
     * Get Cookie
     * std::string myCookie = GetCookie("videoman");
     */
    std::string GetCookie(std::string name)
    {
        std::string myCookie = "";
        try
        {
            myCookie = Wt::WApplication::instance()->environment().getCookie(name);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "CrystalBall::GetCookie: Failed reading cookie: " << name;
            Wt::log("error") << "CrystalBall::GetCookie()  Failed reading cookie: " << name;
        }
        return myCookie;
    } // end std::string CrystalBall::GetCookie
    /* ****************************************************************************
     * String Replace
     * StringReplace(string, "this", "that");
     * string2replace: Actual string to be replaced
     * changefrom: None Empty string to do replacement with
     * changeTo: Can be Empty
     */
    bool StringReplace(std::string& string2replace, const std::string& changefrom, const std::string& changeTo)
    {
        if (string2replace.empty()) { return false; }
        if (changefrom.empty()) { return false; }
        size_t start_pos = string2replace.find(changefrom);
        if(start_pos == std::string::npos) { return false; }
        string2replace.replace(start_pos, changefrom.length(), changeTo);
        return true;
    } // end StringReplace
    /* ****************************************************************************
     * String Replace
     * StringReplace(string, "this", "that");
     */
    bool WStringReplace(std::wstring& string2replace, const std::wstring& changefrom, const std::wstring& changeTo)
    {
        size_t start_pos = string2replace.find(changefrom);
        if(start_pos == std::wstring::npos) { return false; }
        string2replace.replace(start_pos, changefrom.length(), changeTo);
        return true;
    } // end StringReplace
    /* ****************************************************************************
     * FormatWithCommas
     */
    //template<class T>
    std::string FormatWithCommas(long value, std::string myLocale) // T value
    {
        std::stringstream ss;
        ss.imbue(std::locale(myLocale.c_str()));
        ss << std::fixed << value;
        return ss.str();
    } // end FormatWithCommas
    /* ****************************************************************************
     * GetConnectionPoolInfo
     * FIXME: Would it be faster to save this to a database? That was a Joke.
     */
    bool GetConnectionPoolInfo(const std::string filePath, std::string domainName)
    {
        bool showDebug = false;
        if (IsFile(filePath))
        {
            if (showDebug) { Wt::log("start") << "Starting version: " << Version << " XML Configuration File CrystalBall::GetConnectionPoolInfo(" << filePath << ")"; }
        }
        else
        {
            if (showDebug) { Wt::log("error") << "-> Missing XML Configuration File GetConnectionPoolInfo(" << filePath << ")"; }
            return false;
        }
        // Open XML File
        rapidxml::file<> xmlFile(filePath.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        // Find our root node
        rapidxml::xml_node<> * root_node = doc.first_node("domains");
        // define xml item
        rapidxml::xml_attribute<> *x_item;
        for (rapidxml::xml_node<> * domain_node = root_node->first_node("domain"); domain_node; domain_node = domain_node->next_sibling("domain"))
        {
            // domainHost
            x_item = domain_node->first_attribute("domainHost");
            if (!x_item)
            {
                Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: domainHost = " << domain_node->name();
                return false;
            }
            std::string myDomainHost(x_item->value(), x_item->value_size());
            if (domainName == myDomainHost)
            {
                DomainHost[myDomainHost] = myDomainHost;
                // dbName
                x_item = domain_node->first_attribute("dbName");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: dbName";
                    return false;
                }
                DbName[myDomainHost] = x_item->value();
                // defaultLanguageIndex
                x_item = domain_node->first_attribute("defaultLanguageIndex");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: defaultLanguageIndex";
                    return false;
                }
                DefaultLanguageIndex[myDomainHost] = std::stoi(x_item->value());
                // useDb
                x_item = domain_node->first_attribute("useDb");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: useDb";
                    return false;
                }
                UseDb[myDomainHost] = x_item->value();
                // dbUser
                x_item = domain_node->first_attribute("dbUser");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: dbUser";
                    return false;
                }
                DbUser[myDomainHost] = x_item->value();
                // dbPassword
                x_item = domain_node->first_attribute("dbPassword");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: dbPassword";
                    return false;
                }
                DbPassword[myDomainHost] = x_item->value();
                // dbPort
                x_item = domain_node->first_attribute("dbPort");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: dbPort";
                    return false;
                }
                DbPort[myDomainHost] = x_item->value();
                // gaAccount
                x_item = domain_node->first_attribute("gaAccount");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: gaAccount";
                    return false;
                }
                GaAccount[myDomainHost] = x_item->value();
                // gasAccount
                x_item = domain_node->first_attribute("gasAccount");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: gasAccount";
                    return false;
                }
                GasAccount[myDomainHost] = x_item->value();
                // moduleIncludes
                x_item = domain_node->first_attribute("moduleIncludes");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: moduleIncludes";
                    return false;
                }
                ModuleIncludes[myDomainHost] = x_item->value();
                // languages
                x_item = domain_node->first_attribute("languages");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: languages";
                    return false;
                }
                Languages[myDomainHost] = x_item->value();
                // themes
                x_item = domain_node->first_attribute("themes");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: themes";
                    return false;
                }
                Themes[myDomainHost] = x_item->value();
                // defaultTheme
                x_item = domain_node->first_attribute("defaultTheme");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: defaultTheme";
                    return false;
                }
                DefaultTheme[myDomainHost] = x_item->value();
                // googlesiteverifi
                x_item = domain_node->first_attribute("googlesiteverifi");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: googlesiteverifi";
                    return false;
                }
                GoogleSiteVerifi[myDomainHost] = x_item->value();
                // ykey
                x_item = domain_node->first_attribute("ykey");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: ykey";
                    return false;
                }
                Ykey[myDomainHost] = x_item->value();
                // msvalidate
                x_item = domain_node->first_attribute("msvalidate");
                if (!x_item)
                {
                    Wt::log("error") << "-> GetConnectionPoolInfo(" << filePath << ") Missing XML Element: msvalidate";
                    return false;
                }
                MsValidate[myDomainHost] = x_item->value();
                break;
            } // end if (domainName == myDomainHost)
        } // end for (rapidxml::xml_node<> * domain_node = root_node->first_node("domain"); domain_node; domain_node = domain_node->next_sibling("domain"))
        if (showDebug) { Wt::log("end") << "CrystalBall::GetConnectionPoolInfo()"; }
        return true;
    } // end bool GetConnectionPoolInfo
    /* ****************************************************************************
     * Global Function
     * SQL Database Connection
     */
    bool SetSqlConnectionPool(std::string domainName)
    {
        // myConnectionPool.find(domainName) == myConnectionPool.end() ~ this is defined inside the function: myConnectionPool[domainName] = dbConnection_;
        // myDomainHost.find(domainName) != myDomainHost.end() ~ make sure the connection is defined
        if (ConnectionPool.find(domainName) == ConnectionPool.end() && DomainHost.find(domainName) != DomainHost.end())
        {
            Wt::log("start") << " *** SetSqlConnectionPool(" << domainName << ")  myDbUser = " << DbUser[domainName] << " | myDbPort = " << DbPort[domainName] << " |  myDbName = "  <<  DbName[domainName];
            try
            {
                Wt::Dbo::SqlConnection *dbConnection;
                #ifdef POSTGRES
                    dbConnection = new Wt::Dbo::backend::Postgres("user=" + DbUser[domainName] + " password=" + DbPassword[domainName] + " port=" + DbPort[domainName] + " dbname=" + DbName[domainName]);
                #elif SQLITE3
                    Wt::Dbo::backend::Sqlite3 *sqlite3 = new Wt::Dbo::backend::Sqlite3(std::string(path.c_str()) + "app_root/" + std::string(myDbName[domainName]));
                    sqlite3->setDateTimeStorage(Wt::Dbo::SqlDateTime, Wt::Dbo::backend::Sqlite3::PseudoISO8601AsText);
                    dbConnection = sqlite3;
                #elif MYSQL
                    dbConnection = new Wt::Dbo::backend::MySQL(myDbName[domainName], myDbUser[domainName], myDbPassword[domainName], "localhost");
                #elif FIREBIRD
                    // FIXME Hard coded paths for firebird
                    #ifdef WIN32
                        myFile = "C:\\opt\\db\\firebird\\" + myDbName[domainName];
                    #else
                        myFile = "/opt/db/firebird/" + myDbName[domainName];
                    #endif
                    dbConnection = new Wt::Dbo::backend::Firebird("localhost", myFile.c_str(), myDbUser[domainName], myDbPassword[domainName], "", "", ""); // Server:localhost, Path:File, user, password
                #endif // FIREBIRD
                dbConnection->setProperty("show-queries", "true");
                // We need to convert it FixedSqlConnectionPool to SqlConnectionPool, not sure if I should just refactor to use FixedSqlConnectionPool, or do not use Fixed pool size
                Wt::Dbo::SqlConnectionPool* dbConnection_ = new Wt::Dbo::FixedSqlConnectionPool(dbConnection, 10); // FIXME, size of pool
                ConnectionPool[domainName] = dbConnection_;
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Error Connecting to domains database: " << domainName;
                Wt::log("error") << "CrystalBall::SetSqlConnectionPool()  Failed Connecting to domains database: " << domainName;
                return false;
            }
        }
        else
        {
            Wt::log("start") << " *** SetSqlConnectionPool(" << domainName << ") reuse Connection";
        }
        return true;
    } // end SetSqlConnectionPool
    /* ****************************************************************************
     * Returns a list of files in a directory (except the ones that begin with a dot)
     * Andreas Bonini via http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
     */
    void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
    {
    #ifdef WINDOWS
        HANDLE dir;
        WIN32_FIND_DATA file_data;
        if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
            { return; /* No files found */ }
        do
        {
            const string file_name = file_data.cFileName;
            const string full_file_name = directory + "/" + file_name;
            const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
            if (file_name[0] == '.')
                { continue; }
            if (is_directory)
                { continue;}
            out.push_back(full_file_name);
        } while (FindNextFile(dir, &file_data));

        FindClose(dir);
    #else
        DIR *dir;
        class dirent *ent;
        class stat st;
        dir = opendir(directory.c_str());
        while ((ent = readdir(dir)) != NULL)
        {
            const std::string file_name = ent->d_name;
            const std::string full_file_name = directory + "/" + file_name;
            if (file_name[0] == '.')
                { continue; }
            if (stat(full_file_name.c_str(), &st) == -1)
                { continue; }
            const bool is_directory = (st.st_mode & S_IFDIR) != 0;
            if (is_directory)
                { continue; }
            out.push_back(full_file_name);
        }
        closedir(dir);
    #endif
    } // GetFilesInDirectory
    /* ****************************************************************************
     * CheckErrorBits
     */
    bool CheckErrorBits(std::wifstream* f)
    {
        bool stop = false;
        if (f->eof())
        {
            Wt::log("error") << "CrystalBall::CheckErrorBits stream eofbit. error state";
            // EOF after std::getline() is not the criterion to stop processing
            // data: In case there is data between the last delimiter and EOF,
            // getline() extracts it and sets the eofbit.
            stop = true;
        }
        if (f->fail())
        {
            Wt::log("error") << "CrystalBall::CheckErrorBits stream failbit (or badbit). error state";
        }
        if (f->bad())
        {
            Wt::log("error") << "CrystalBall::CheckErrorBits stream badbit. error state";
        }
        return stop;
    } // end CheckErrorBits
    /* ****************************************************************************
     * GetTemplate
     * id: this is the Message ID <message id="ww-template">
     * Returns the Contents
     */
    Wt::WString GetTemplate(std::string appPath, std::string thePath, std::string myLanguage)
    {
        //if (id.empty()) { Wt::log("error") << " *** MenuManSession::GetTemplate() empty content id"; return ""; }
    //#define STATIC_DATA
    #ifdef STATIC_DATA
        //return "<div><p>This is a Test</p><p>And Only a Test</p></div>";
        //return "test";
        std::locale::global(std::locale("")); // FIXME: this fixs issue with not reading in file chinese, but this makes an ugly page, had to start app over

        std::wstring line;        
        std::wifstream myfile (thePath);
        std::wstring myTemplate;
        int counter = 0;
        //myfile.imbue(locale(myfile.getloc(), new std::codecvt<wchar_t, 0x10ffff, consume_header>()));
        if (myfile.is_open())
        {
          while (1)
          {
              getline (myfile, line);
              if (CheckErrorBits(&myfile))
              {
                  Wt::log("error") << "CrystalBall::GetTemplate skip operation on data, break loop";
                  break;
              }
              if (counter++ < 3) { continue; }
              if (line.find(L"</message>") != std::string::npos)
              {
                  break;
              }
              line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
              line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
              WStringReplace(line, L"“", L"\"");
              WStringReplace(line, L"”", L"\"");
              myTemplate.append(line);
          }
          myfile.close();
        }
        else
        {
            Wt::log("error") << " *** MenuManSession::GetTemplate() Can not Read File " << id;
        }
        return myTemplate.c_str();
    #else
        std::string myPath = thePath;
        if (myPath == "/")
            { myPath = "/home"; } // FIXME: home is hard corded
        std::string fileSufix = "";
        if (myLanguage != "en") // FIXME: Is en the default language?
            { fileSufix = "_" + myLanguage; }
        if (IsFile(appPath + "xml" + myPath + fileSufix + ".xml"))
        {
            std::string templateName = myPath;
            size_t found = templateName.find_last_of("/\\"); // /path/fileName
            if (found != std::string::npos)
                { templateName = templateName.substr(found + 1); }
            Wt::WApplication::instance()->setLocale(myLanguage);
            Wt::WApplication::instance()->messageResourceBundle().use(appPath + "xml" + myPath, false);

            std::wstring myContent = Wt::WString::tr(templateName + "-template");

            myContent.erase(std::remove(myContent.begin(), myContent.end(), '\t'), myContent.end());
            myContent.erase(std::remove(myContent.begin(), myContent.end(), '\n'), myContent.end());

            WStringReplace(myContent, L"“", L"\"");
            WStringReplace(myContent, L"”", L"\"");

            return myContent;
        }
        else
        {
            Wt::log("error") << " *** CrystalBall::GetTemplate() Could Not find xml file " << thePath;
        }
        //std::wstring myTemplate = L"<div><p>This is a Test</p><p>And Only a Test</p></div>";
    #endif
        //const std::string temp( myTemplate.begin(), myTemplate.end() );
        //Wt::log("notice") << " *** MenuManSession::GetTemplate() Read " << temp;
        return "";
    } // end GetTemplate
} // end namespace CrystalBall
// --- End Of File ------------------------------------------------------------
