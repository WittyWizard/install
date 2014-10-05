/* ****************************************************************************
 * Witty Wizard
 * Global Functions: extern
 */
namespace CrystalBall
{
    /* ****************************************************************************
     * Global Version Number and Date last Modified for logs
     */
    extern std::string Version;
    /* ****************************************************************************
     * Global Variable
     * root Prefix: Used to set the URL Path: http:domain.tdl\prefix\root-path
     * See Wt Documenation on: --deploy-path=
     * This is used to fix path issues in Wt, must be in run and in path of all menu items and resources
     */
    extern std::string RootPathPrefix;
    /* ****************************************************************************
     * Global map
     * Connection Pool Map
     * Holds the Pointer to Wt::Dbo::SqlConnectionPool *
     */
    extern std::map <std::string, boost::any> ConnectionPool;
    /* ****************************************************************************
     * Global Variable
     * domainHost="wittywizard.org"
     */
    extern std::map <std::string, std::string> DomainHost;
    /* ****************************************************************************
     * Default Language Index
     * This is added in HomeFundation AddLanguage
     */
    extern std::map <std::string, int> DefaultLanguageIndex;
    /* ****************************************************************************
     * Global Variable
     * UseDb="0": 1 = true, 0 = false
     */
    extern std::map <std::string, std::string> UseDb;
    /* ****************************************************************************
     * Global Variable
     * dbName="wittywizard"
     */
    extern std::map <std::string, std::string> DbName;
    /* ****************************************************************************
     * Global Variable
     * dbUser="wittywizard"
     */
    extern std::map <std::string, std::string> DbUser;
    /* ****************************************************************************
     * Global Variable
     * password="The1Wizard2Witty4Flesh"
     */
    extern std::map <std::string, std::string> DbPassword;
    /* ****************************************************************************
     * Global Variable
     * dbPort="5432"
     */
    extern std::map <std::string, std::string> DbPort;
    /* ****************************************************************************
     * Global Variable
     * gasAccount="pub-1083602596491715"
     *
     */
    extern std::map <std::string, std::string> GasAccount;
    /* ****************************************************************************
     * Global Variable
     * gaAccount="UA-xxxxxxxx-x"
     * Google Analytic Account: See domain.xml
     * http://google.com/analytics/
     */
    extern std::map <std::string, std::string> GaAccount;
    /* ****************************************************************************
     * Global Variable
     * ModuleIncludes="home|chat|blog|about|contact|video"
     * ModuleIncludes: Each Menu Item Included
     * See domain.xml:includes="home|chat|blog|about|contact|video"
     */
    extern std::map <std::string, std::string> ModuleIncludes;
    /* ****************************************************************************
     * Global Variable
     * languages="|en|de|cn|ru|"
     * See domain.xml:languages="|en|de|cn|ru|"
     */
    extern std::map <std::string, std::string> Languages;
    /* ****************************************************************************
     * Global Variable
     * See domain.xml:themes="|red|white|blue|green|tan|default|"
     */
    extern std::map <std::string, std::string> Themes;
    /* ****************************************************************************
     * Global Variable
     * defaultTheme="blue"
     * See domain.xml:defaultTheme="blue"
     */
    extern std::map <std::string, std::string> DefaultTheme;
    /* ****************************************************************************
     * Global Variable
     * google-site-verification
     */
    extern std::map <std::string, std::string> GoogleSiteVerifi;
    /* ****************************************************************************
     * Global Variable
     * y_key: Yahoo Site Verisfication Key
     */
    extern std::map <std::string, std::string> Ykey;
    /* ****************************************************************************
     * Global Variable
     * msvalidate.01
     */
    extern std::map <std::string, std::string> MsValidate;
    /* ****************************************************************************
     * Global Variable
     * Used to create database, turn off afterwards so you do not get errors
     * FIXME make admin function to change this on the fly *
     */
    extern bool InitDb;
    /* ****************************************************************************
     * Global Function
     * is File: Full Path to File: /home/domain.tdl/filename.ext
     */
    extern bool IsFile(const std::string& name);
    /* ****************************************************************************
     * Global Function
     * is Path: Full Path: /home/domain.tdl/
     */
    extern bool IsPath(const std::string& pathName);
    /* ****************************************************************************
     * Set Cookie
     */
    extern void SetCookie(std::string name, std::string myValue);
    /* ****************************************************************************
     * Get Cookie
     * std::string myCookie = GetCookie("videoman");
     */
    extern std::string GetCookie(std::string name);
    /* ****************************************************************************
     * String Replace
     * StringReplace(string, "this", "that");
     */
    extern bool StringReplace(std::string& string2replace, const std::string& changefrom, const std::string& changeTo);
    /* ****************************************************************************
     * Wide String Replace
     * WStringReplace(string, "this", "that");
     */
    extern bool WStringReplace(std::wstring& string2replace, const std::wstring& changefrom, const std::wstring& changeTo);
    /* ****************************************************************************
     * FormatWithCommas
     */
    extern std::string FormatWithCommas(long value, std::string myLocale);
    /* ****************************************************************************
     * GetConnectionPoolInfo
     * filePath:
     * domainName:
     */
    extern bool GetConnectionPoolInfo(std::string filePath, std::string domainName);
    /* ****************************************************************************
     * Global Function
     * SQL Database Connection
     */
    extern bool SetSqlConnectionPool(std::string domainName);
    /* ****************************************************************************
     * Returns a list of files in a directory (except the ones that begin with a dot)
     * Andreas Bonini via http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
     */
    extern void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
    /* ****************************************************************************
     * GetTemplate
     * id: this is the Message ID <message id="ww-template">
     * Returns the Contents
     */
    extern Wt::WString GetTemplate(std::string appPath, std::string thePath, std::string myLanguage);
    /* ****************************************************************************
     * Get Path
     * remove language and theme
     * myLanguages: en, cn, ru, de...
     * myThemes: red,white,blue...
     */
    //extern std::string GetPath(std::string thePath, std::vector<Lang> myLanguages, std::vector<Theme> myThemes, std::string language, std::string theme);

} // end namespace CrystalBall
// --- End Of File ------------------------------------------------------------
