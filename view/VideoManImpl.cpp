#ifdef VIDEOMAN
/* ****************************************************************************
 * Video Manager Implementaion
 * Witty Wizard
 * Version: 1.0.0
 * Last Date Modified: 19 Aug 2014
 *
 */
#include "view/VideoManImpl.h"
/* ****************************************************************************
 * Video Man Impl
 * This gets called every time the page is refreshed
 * appPath: appRoot() + "home/" + domainName + "/videoman/" ~ /full-path/app_root/home/domainName/videoman/
 * basePath: "/" + langPath + "/video/" ~ /en/video/
 * connectionPool: dbo
 * lang: Name: en, cn, ru ...
 */
VideoManImpl::VideoManImpl(const std::string& appPath, const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool, const std::string& lang, const std::string& useDb, const std::string& domainName) : appPath_(appPath), basePath_(basePath), session_(appPath, useDb, connectionPool), lang_(lang), useDb_(useDb), domainName_(domainName)
{
    items_ = new Wt::WContainerWidget(this);
    items_->setId("videocan");
    //
    thePath = Wt::WString::tr("VideoManPath").toUTF8();
    //
    bindItems = new Wt::WContainerWidget(this);
    bindItems->setId("videomanbinder");
    //Wt::WApplication *app = Wt::WApplication::instance();
    app = wApp;
    // FIXIT Do we want to use our own Template or use a common Template?
    // /full-path/app_root/home/domainName/videoman.xml
    if (1 && !Crystallball::IsFile(appPath + "videoman.xml")) { Wt::log("error") << " *** VideoManImpl::VideoManImpl() Themeplate Not Found: " << appPath + "videoman.xml" << " *** "; }
    std::string myLang = "_" + lang;
    if (lang == "en") { myLang = ""; }
    app->messageResourceBundle().use(appPath + "videoman" + myLang); // ./app_root/ Wt::WApplication::appRoot()
    if (useDb_ == "1")
    {
        videoManPage_ = videoManTemplate = new Wt::WTemplate(Wt::WString::tr("videoman-template"), this); //  <message id="videoman-template">
        filePath = appPath + "videoman-db-import" + myLang + ".xml";
    }
    else
    {
        filePath = appPath + "videoman-xml-import" + myLang + ".xml";
    }
    //
    myTheme = Crystallball::GetCookie("theme");
    if (myTheme.empty())
    {
        myTheme = Crystallball::DefaultTheme[domainName];
    }
    //Wt::WLocalizedStrings thisString;    thisString.resolveKey()

    Wt::log("start") << " *** VideoManImpl::VideoManImpl() lang = " << lang << " | domainName = " << domainName << " | appPath = " << appPath << " | filePath = " << filePath << " | basePath = " << basePath << " *** ";

    Init();
} // end VideoManImpl
/* ****************************************************************************
 * ~Video Man Impl
 */
VideoManImpl::~VideoManImpl()
{
    clear();
} // end ~VideoManImpl
/* ****************************************************************************
 * init
 */
void VideoManImpl::Init()
{
    Wt::log("start") << " *** VideoManImpl::Init() ***";
    MakeVideo();
    //addWidget(videoManPage_);
    // Now add in the event handler
    app->internalPathChanged().connect(this, &VideoManImpl::HandlePathChange);
} // end void Init()
/* ****************************************************************************
 * make Video
 */
void VideoManImpl::MakeVideo()
{
    Wt::log("start") << " *** VideoManImpl::MakeVideo( ) *** ";
    // Set Category and Video from Internal Path, ComboBox or Cookie
    GetCategoriesPath();
    if (videoText == "updatexml")
    {
        videoText == "";
    }
    if (videoText == "edit")
    {
        Edit();
        videoText == "";
    }
    // Clear all the WContainerWidget Items
    items_->clear();
    //
    CreateCategoryCombo();
    //
    CreateVideoCombobox();
    //
    Wt::log("end") << " ** VideoManImpl::MakeVideo() ** ";
} // end void MakeVideo()
/* ****************************************************************************
 * Create Category Combo Db
 */
bool VideoManImpl::CreateCategoryComboDb()
{
    Wt::log("start") << " *** VideoManImpl::CreateCategoryComboDb() ***";
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);
        typedef Wt::Dbo::collection< Wt::Dbo::ptr<VideoMan> > VideoMans;
        // Get database list of all Videos
        VideoMans videos = session_.find<VideoMan>();
        for (VideoMans::const_iterator i = videos.begin(); i != videos.end(); ++i)
        {
            //Wt::log("notice") << "VideoManImpl::CreateCategoryCombo:  const_iterator = " << (*i)->categories;
            // Category List: cat-0|cat-1|cat-2|cat-3...
            // Currently only testing 1 category
            // @FIXIT test
            std::string myCategoriesSchema = (*i)->categories;
            std::vector<std::string> categoryFields;
            boost::split(categoryFields, myCategoriesSchema, boost::is_any_of("|"));
            //
            if (myCategoriesSchema.empty())
            {
                numberCateories = 0;
            }
            else
            {
                //numberCats = myCategoriesSchema.contains('|') + 1;
                numberCateories = std::count(myCategoriesSchema.begin(), myCategoriesSchema.end(), '_') + 1;
            }
            //Wt::log("notice") << "VideoManImpl::CreateCategoryCombo:  numberCats = " << numberCats;
            if (numberCateories > 1)
            {
                    int catSize = categoryFields.size();
                    if (catSize > 1)
                    {
                        // if I use lambda (name of javascript concept where you name a variable like category_$x = 0; not a c concept) a categoryText_x where x is i, it becomes: categoryText_0 and assigns the correct value to it
                        for (int catIndex = 0; catIndex < catSize; catIndex++)
                        {
                            switch(catIndex)
                            {
                                case 0:
                                    //categoryText_0 = categoryFields.at(catIndex).toStdString();
                                    categoryText_0 = categoryFields[catIndex];
                                    break;
                                case 1:
                                    //categoryText_1 = categoryFields.at(catIndex).toStdString();
                                    categoryText_1 = categoryFields[catIndex];
                                    break;
                                case 2:
                                    //categoryText_2 = categoryFields.at(catIndex).toStdString();
                                    categoryText_2 = categoryFields[catIndex];
                                    break;
                                case 3:
                                    //categoryText_3 = categoryFields.at(catIndex).toStdString();
                                    categoryText_3 = categoryFields[catIndex];
                                    break;
                                case 4:
                                    //categoryText_4 = categoryFields.at(catIndex).toStdString();
                                    categoryText_4 = categoryFields[catIndex];
                                    break;
                                case 5:
                                    //categoryText_5 = categoryFields.at(catIndex).toStdString();
                                    categoryText_5 = categoryFields[catIndex];
                                    break;
                            }
                        } // end for (int catIndex=0;catIndex<catSize;catIndex++)
                    } // end  if (catSize > 1)
            } // end if (numberCats > 1)
            switch (numberCateories)
            {
                case 0:
                    // FIXIT for video list only
                    break;
                case 1:
                    if (ComboCategory_0 == NULL)
                    {
                        Wt::log("notice") << "VideoManImpl::CreateCategoryCombo:  new ComboCategory_0 WComboBox";
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(myCategoriesSchema) == -1)
                    {
                        ComboCategory_0->addItem(myCategoriesSchema); // it did not exist, add it
                    }
                    if (myCategoriesSchema == categoryQuery)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryQuery));
                    }
                    break;
                case 2:
                    if (ComboCategory_0 == NULL)
                    {
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_1 == NULL)
                    {
                        ComboCategory_1 = new Wt::WComboBox(items_);
                        ComboCategory_1->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(categoryText_0) == -1)
                    {
                        ComboCategory_0->addItem(categoryText_0); // it did not exist, add it
                    }
                    if (categoryFields[0] == categoryText_0)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryText_0));
                    }
                    if (ComboCategory_1->findText(categoryText_1) == -1)
                    {
                        ComboCategory_1->addItem(categoryText_1); // it did not exist, add it
                    }
                    if (categoryFields[1] == categoryText_1)
                    {
                        ComboCategory_1->setCurrentIndex(ComboCategory_1->findText(categoryText_1));
                    }
                    break;
                case 3:
                    if (ComboCategory_0 == NULL)
                    {
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_1 == NULL)
                    {
                        ComboCategory_1 = new Wt::WComboBox(items_);
                        ComboCategory_1->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_2 == NULL)
                    {
                        ComboCategory_2 = new Wt::WComboBox(items_);
                        ComboCategory_2->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(categoryText_0) == -1)
                    {
                        ComboCategory_0->addItem(categoryText_0); // it did not exist, add it
                    }
                    if (categoryFields[0] == categoryText_0)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryText_0));
                    }
                    if (ComboCategory_1->findText(categoryText_1) == -1)
                    {
                        ComboCategory_1->addItem(categoryText_1); // it did not exist, add it
                    }
                    if (categoryFields[1] == categoryText_1)
                    {
                        ComboCategory_1->setCurrentIndex(ComboCategory_1->findText(categoryText_1));
                    }
                    if (ComboCategory_2->findText(categoryText_2) == -1)
                    {
                        ComboCategory_2->addItem(categoryText_2); // it did not exist, add it
                    }
                    if (categoryFields[2] == categoryText_2)
                    {
                        ComboCategory_2->setCurrentIndex(ComboCategory_2->findText(categoryText_2));
                    }
                    break;
                case 4:
                    if (ComboCategory_0 == NULL)
                    {
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_1 == NULL)
                    {
                        ComboCategory_1 = new Wt::WComboBox(items_);
                        ComboCategory_1->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_2 == NULL)
                    {
                        ComboCategory_2 = new Wt::WComboBox(items_);
                        ComboCategory_2->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_3 == NULL)
                    {
                        ComboCategory_3 = new Wt::WComboBox(items_);
                        ComboCategory_3->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(categoryText_0) == -1)
                    {
                        ComboCategory_0->addItem(categoryText_0); // it did not exist, add it
                    }
                    if (categoryFields[0] == categoryText_0)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryText_0));
                    }
                    if (ComboCategory_1->findText(categoryText_1) == -1)
                    {
                        ComboCategory_1->addItem(categoryText_1); // it did not exist, add it
                    }
                    if (categoryFields[1] == categoryText_1)
                    {
                        ComboCategory_1->setCurrentIndex(ComboCategory_1->findText(categoryText_1));
                    }
                    if (ComboCategory_2->findText(categoryText_2) == -1)
                    {
                        ComboCategory_2->addItem(categoryText_2); // it did not exist, add it
                    }
                    if (categoryFields[2] == categoryText_2)
                    {
                        ComboCategory_2->setCurrentIndex(ComboCategory_2->findText(categoryText_2));
                    }
                    if (ComboCategory_3->findText(categoryText_3) == -1)
                    {
                        ComboCategory_3->addItem(categoryText_3); // it did not exist, add it
                    }
                    if (categoryFields[3] == categoryText_3)
                    {
                        ComboCategory_3->setCurrentIndex(ComboCategory_3->findText(categoryText_3));
                    }
                    break;
                case 5:
                    if (ComboCategory_0 == NULL)
                    {
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_1 == NULL)
                    {
                        ComboCategory_1 = new Wt::WComboBox(items_);
                        ComboCategory_1->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_2 == NULL)
                    {
                        ComboCategory_2 = new Wt::WComboBox(items_);
                        ComboCategory_2->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_3 == NULL)
                    {
                        ComboCategory_3 = new Wt::WComboBox(items_);
                        ComboCategory_3->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_4 == NULL)
                    {
                        ComboCategory_4 = new Wt::WComboBox(items_);
                        ComboCategory_4->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(categoryText_0) == -1)
                    {
                        ComboCategory_0->addItem(categoryText_0); // it did not exist, add it
                    }
                    if (categoryFields[0] == categoryText_0)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryText_0));
                    }
                    if (ComboCategory_1->findText(categoryText_1) == -1)
                    {
                        ComboCategory_1->addItem(categoryText_1); // it did not exist, add it
                    }
                    if (categoryFields[1] == categoryText_1)
                    {
                        ComboCategory_1->setCurrentIndex(ComboCategory_1->findText(categoryText_1));
                    }
                    if (ComboCategory_2->findText(categoryText_2) == -1)
                    {
                        ComboCategory_2->addItem(categoryText_2); // it did not exist, add it
                    }
                    if (categoryFields[2] == categoryText_2)
                    {
                        ComboCategory_2->setCurrentIndex(ComboCategory_2->findText(categoryText_2));
                    }
                    if (ComboCategory_3->findText(categoryText_3) == -1)
                    {
                        ComboCategory_3->addItem(categoryText_3); // it did not exist, add it
                    }
                    if (categoryFields[3] == categoryText_3)
                    {
                        ComboCategory_3->setCurrentIndex(ComboCategory_3->findText(categoryText_3));
                    }
                    if (ComboCategory_4->findText(categoryText_4) == -1)
                    {
                        ComboCategory_4->addItem(categoryText_4); // it did not exist, add it
                    }
                    if (categoryFields[4] == categoryText_4)
                    {
                        ComboCategory_4->setCurrentIndex(ComboCategory_4->findText(categoryText_4));
                    }
                    break;
                case 6:
                    if (ComboCategory_0 == NULL)
                    {
                        ComboCategory_0 = new Wt::WComboBox(items_);
                        ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_1 == NULL)
                    {
                        ComboCategory_1 = new Wt::WComboBox(items_);
                        ComboCategory_1->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_2 == NULL)
                    {
                        ComboCategory_2 = new Wt::WComboBox(items_);
                        ComboCategory_2->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_3 == NULL)
                    {
                        ComboCategory_3 = new Wt::WComboBox(items_);
                        ComboCategory_3->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_4 == NULL)
                    {
                        ComboCategory_4 = new Wt::WComboBox(items_);
                        ComboCategory_4->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    if (ComboCategory_5 == NULL)
                    {
                        ComboCategory_5 = new Wt::WComboBox(items_);
                        ComboCategory_5->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
                    }
                    // Find it
                    if (ComboCategory_0->findText(categoryText_0) == -1)
                    {
                        ComboCategory_0->addItem(categoryText_0); // it did not exist, add it
                    }
                    if (categoryFields[0] == categoryText_0)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(categoryText_0));
                    }
                    if (ComboCategory_1->findText(categoryText_1) == -1)
                    {
                        ComboCategory_1->addItem(categoryText_1); // it did not exist, add it
                    }
                    if (categoryFields[1] == categoryText_1)
                    {
                        ComboCategory_1->setCurrentIndex(ComboCategory_1->findText(categoryText_1));
                    }
                    if (ComboCategory_2->findText(categoryText_2) == -1)
                    {
                        ComboCategory_2->addItem(categoryText_2); // it did not exist, add it
                    }
                    if (categoryFields[2] == categoryText_2)
                    {
                        ComboCategory_2->setCurrentIndex(ComboCategory_2->findText(categoryText_2));
                    }
                    if (ComboCategory_3->findText(categoryText_3) == -1)
                    {
                        ComboCategory_3->addItem(categoryText_3); // it did not exist, add it
                    }
                    if (categoryFields[3] == categoryText_3)
                    {
                        ComboCategory_3->setCurrentIndex(ComboCategory_3->findText(categoryText_3));
                    }
                    if (ComboCategory_4->findText(categoryText_4) == -1)
                    {
                        ComboCategory_4->addItem(categoryText_4); // it did not exist, add it
                    }
                    if (categoryFields[4] == categoryText_4)
                    {
                        ComboCategory_4->setCurrentIndex(ComboCategory_4->findText(categoryText_4));
                    }
                    if (ComboCategory_5->findText(categoryText_5) == -1)
                    {
                        ComboCategory_5->addItem(categoryText_5); // it did not exist, add it
                    }
                    if (categoryFields[5] == categoryText_5)
                    {
                        ComboCategory_5->setCurrentIndex(ComboCategory_5->findText(categoryText_5));
                    }
                    break;
            } // end switch (numberCats)
            //Wt::log("notice") << " VideoManImpl::CreateCategoryCombo() numberCats = " << numberCats << " : name = " << mySchema.toStdString() << " ";
        } // end for (VideoMans::const_iterator i = videos.begin(); i != videos.end(); ++i)
        // Commit Transaction
        t.commit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManImpl::CreateCategoryCombo: Failed reading from videoman database.";
        Wt::log("error") << "VideoManImpl::CreateCategoryCombo()  Failed reading from videoman database.";
        return false;
    }
    Wt::log("end") << " ** VideoManImpl::CreateCategoryComboDb() **";
    return true;
} // end CreateCategoryComboDb
/* ****************************************************************************
 * Create Category Combo XML
 */
bool VideoManImpl::CreateCategoryComboXml()
{
    Wt::log("start") << " *** VideoManImpl::CreateCategoryComboXml() ***";
    if (Crystallball::IsFile(filePath))
    {
        Wt::log("info") << "CreateCategoryComboXml:  XML Configuration File " << filePath;
    }
    else
    {
        Wt::log("error") << "VideoManImpl::CreateCategoryComboXml Missing XML Configuration File " << filePath;
        return false;
    }
    // Open XML File
    rapidxml::file<> xmlFile(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    // Find our root node
    rapidxml::xml_node<> * root_node = doc.first_node("category");
    // define xml item
    rapidxml::xml_attribute<> *x_item;
    //
    x_item = root_node->first_attribute("number");
    if (!x_item)
    {
        Wt::log("error") << "-> CreateCategoryComboXml() Missing XML Element: number";
        return false;
    }
    numberCateories = std::stoi(x_item->value());
    //
    x_item = root_node->first_attribute("categories");
    if (!x_item)
    {
        Wt::log("error") << "-> CreateCategoryComboXml() Missing XML Element: categories";
        return false;
    }
    std::string myCategoriesSchema = x_item->value();
    std::vector<std::string> categoryFields;
    switch (numberCateories)
    {
        case 0:
            break;
        case 1:
            boost::split(categoryFields, myCategoriesSchema, boost::is_any_of("|"));
            if (ComboCategory_0 == NULL)
            {
                Wt::log("notice") << "VideoManImpl::CreateCategoryComboXml:  new ComboCategory_0 WComboBox";
                ComboCategory_0 = new Wt::WComboBox(items_);
                ComboCategory_0->activated().connect(this, &VideoManImpl::ComboCategoryChanged);
            }
            for (std::vector<std::string>::const_iterator it = categoryFields.begin() ; it != categoryFields.end(); ++it)
            {
                // Find it
                if (ComboCategory_0->findText(*it) == -1)
                {
                    Wt::log("notice") << "VideoManImpl::CreateCategoryComboXml:  addItem = " << *it;
                    ComboCategory_0->addItem(*it); // it did not exist, add it
                }
                if (categoryQuery.empty())
                {
                    ComboCategory_0->setCurrentIndex(0);
                    categoryQuery = *it;
                }
                else
                {
                    if (*it == categoryQuery)
                    {
                        ComboCategory_0->setCurrentIndex(ComboCategory_0->findText(*it));
                    }
                }
            }
            break;
        case 2:
            boost::split(categoryFields, myCategoriesSchema, boost::is_any_of("^"));
            break;
    } // end switch (numberCateories)
    Wt::log("end") << " ** VideoManImpl::CreateCategoryComboXml() **";
    return true;
} // end CreateCategoryComboXml
/* ****************************************************************************
 * Create Category Combo
 * First time Internal Path will not be set
 * Combobox will not be set
 */
bool VideoManImpl::CreateCategoryCombo()
{
    Wt::log("start") << " *** VideoManImpl::CreateCategoryCombo() ***";
    // Create an instance of app to access Internal Paths
    Wt::log("notice") << "VideoManImpl::CreateCategoryCombo() read internal Path: categoryPath = " << categoryPath  << " | categoryQuery = " << categoryQuery << " | videoText = __" << videoText << "__";
    if (categoryPath.empty())
    {
        categoryPath = GetCategories("/");
    }
    if (categoryQuery.empty())
    {
        categoryQuery = GetCategories("|");
    }
    //
    if (useDb_ == "1")
    {
        Wt::log("notice") << "VideoManImpl::CreateCategoryCombo() DB";
        CreateCategoryComboDb();
    }
    else
    {
        if (numberCateories == -1)
        {
            Wt::log("notice") << "VideoManImpl::CreateCategoryCombo() XML = " << categoryQuery;
            CreateCategoryComboXml();
        }
    }
    //
    Wt::log("end") << " ** VideoManImpl::CreateCategoryCombo() **";
    return true;
} // end bool CreateCategoryCombo()
/* ****************************************************************************
 * Create Video Combobox
 * categoryPath is set in program
 * Called From:
 * ComboCategoryChanged
 *
 */
bool VideoManImpl::CreateVideoComboboxDb()
{
    Wt::log("start") << "VideoManImpl::CreateVideoComboboxDb()";
    try
    {
        Wt::Dbo::QueryModel< Wt::Dbo::ptr<VideoMan> > *model = new Wt::Dbo::QueryModel< Wt::Dbo::ptr<VideoMan> >();
        // FIXIT language  AND language = " + lang_
        //model->setQuery(session_.query< Wt::Dbo::ptr<VideoMan> >("select u from videoman u").where("categories = ? AND language = ?").bind(categoryQuery).bind(lang_), false);
        model->setQuery(session_.query< Wt::Dbo::ptr<VideoMan> >("select u from videoman u").where("categories = ?").bind(categoryQuery).where("language = ?").bind(lang_), false);
        //model->setQuery(session_.query< Wt::Dbo::ptr<VideoMan> >("select u from videoman u").where("categories = ?").bind(categoryQuery), false);
        model->addColumn("name");
        // ComboVideo has not been set
        if (ComboVideo == NULL)
        {
            ComboVideo = new Wt::WComboBox(items_);
            isChanged = true;
            ComboVideo->activated().connect(this, &VideoManImpl::ComboVideoChanged);
            isChanged = false;
            Wt::log("notice") << "VideoManImpl::CreateVideoComboboxDb()  new ComboBox";
        }
        else
        {
            // ComboVideo = new Wt::WComboBox(items_); // Creates Duplicates
            Wt::log("notice") << "VideoManImpl::CreateVideoComboboxDb()  reuse ComboBox";
        }
        // ComboVideo->clear(); do not do this, it deletes the database files
        Wt::log("notice") << "VideoManImpl::CreateVideoComboboxDb() Query = " << categoryQuery << " | Model row count = " << model->rowCount() << " ";
        ComboVideo->setModel(model);
        ComboVideo->refresh();
        ComboVideo->setCurrentIndex(0);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManImpl::CreateVideoComboboxDb: Failed reading record from videoman database category = " << categoryQuery << "";
        Wt::log("error") << "VideoManImpl::CreateVideoComboboxDb() Failed reading record from videoman database category = " << categoryQuery << " ";
        return false;
    }
    Wt::log("start") << "VideoManImpl::CreateVideoComboboxDb()";
    return true;
} // end CreateVideoComboboxDb()
/* ****************************************************************************
 * Create Video Combobox
 * categoryPath is set in program
 * Called From:
 * ComboCategoryChanged
 *
 */
bool VideoManImpl::CreateVideoComboboxXml()
{
    Wt::log("start") << "VideoManImpl::CreateVideoComboboxXml() ~ categoryQuery = " << categoryQuery << " | numberCateories = " << numberCateories;
    if (ComboVideo != NULL)
    {
        ComboVideo->clear();
        Wt::log("notice") << "VideoManImpl::CreateVideoComboboxXml()  clear ComboBox";
    }

    if (Crystallball::IsFile(filePath))
    {
        Wt::log("info") << "CreateVideoComboboxXml:  XML Configuration File " << filePath;
    }
    else
    {
        Wt::log("error") << "VideoManImpl::CreateVideoComboboxXml Missing XML Configuration File " << filePath;
        return false;
    }
    std::string Name;
    std::string Categories;
    std::string Language;
    // Open XML File
    rapidxml::file<> xmlFile(filePath.c_str());
    rapidxml::xml_document<> xmlDoc;
    xmlDoc.parse<0>(xmlFile.data());
    // Find our root node
    rapidxml::xml_node<> * root_node = xmlDoc.first_node("category");
    // define xml item
    rapidxml::xml_attribute<> *nodeAttrib;
    //
    std::string myCategoriesSchema = "";
    std::vector<std::string> categoryFields;
    boost::split(categoryFields, myCategoriesSchema, boost::is_any_of("|"));

    for (rapidxml::xml_node<> * domain_node = root_node->first_node("videoman"); domain_node; domain_node = domain_node->next_sibling("videoman"))
    {
        // name
        nodeAttrib = domain_node->first_attribute("name");
        if (!nodeAttrib)
        {
            Wt::log("error") << "-> CreateVideoComboboxXml() Missing XML Element: name = " << domain_node->name();
            return false;
        }
        Name = nodeAttrib->value();
        // categories
        nodeAttrib = domain_node->first_attribute("categories");
        if (!nodeAttrib)
        {
            Wt::log("error") << "-> CreateVideoComboboxXml() Missing XML Element: categories";
            return false;
        }
        Categories = nodeAttrib->value();
        // language
        nodeAttrib = domain_node->first_attribute("language");
        if (!nodeAttrib)
        {
            Wt::log("error") << "-> CreateVideoComboboxXml() Missing XML Element: language";
            return false;
        }
        Language = nodeAttrib->value();
        //
        //Wt::log("info") << "-> CreateVideoComboboxXml() Name = " << Name << " | Categories = " << Categories << " | Language = " << Language;
        switch (numberCateories)
        {
            case 0:
                break;
            case 1:
                //Wt::log("notice") << "VideoManImpl::CreateVideoComboboxXml:  case 1 = " << Name;
                if (ComboVideo == NULL)
                {
                    ComboVideo = new Wt::WComboBox(items_);
                    isChanged = true;
                    ComboVideo->activated().connect(this, &VideoManImpl::ComboVideoChanged);
                    isChanged = false;
                    Wt::log("notice") << "VideoManImpl::CreateVideoComboboxXml()  new ComboBox";
                }
                if (Categories == categoryQuery)
                {
                    // Find it
                    if (ComboVideo->findText(Name) == -1)
                    {
                        Wt::log("notice") << "VideoManImpl::CreateVideoComboboxXml:  addItem = " << Name;
                        ComboVideo->addItem(Name); // it did not exist, add it
                    }
                    if (videoText.empty())
                    {
                        ComboVideo->setCurrentIndex(0);
                        videoText = Name;
                    }
                    else
                    {
                        if (Name == videoText)
                        {
                            ComboVideo->setCurrentIndex(ComboCategory_0->findText(Name));
                        }
                    }
                }
                break;
        } // end switch (numberCateories)
    } // end for (rapidxml::xml_node<> * domain_node = root_node->first_node("domain"); domain_node; domain_node = domain_node->next_sibling("domain"))
    Wt::log("end") << "VideoManImpl::CreateVideoComboboxXml()";
    return true;
} // end CreateVideoComboboxXml()
/* ****************************************************************************
 * Create Video Combobox
 * categoryPath is set in program
 * Called From:
 * ComboCategoryChanged
 *
 */
bool VideoManImpl::CreateVideoCombobox()
{
    if (categoryQuery.empty())
    {
        categoryQuery = GetCategories("/");
        Wt::log("error") << " *** VideoManImpl::CreateVideoCombobox() empty categoryQuery = __" << categoryQuery << "__ | lang_ = " << lang_ << " | numberCateories = " << numberCateories << " *** ";
    }
    else
    {
        Wt::log("start") << " *** VideoManImpl::CreateVideoCombobox() categoryQuery = __" << categoryQuery << "__  lang_ = " << lang_ << " | numberCateories = " << numberCateories << " *** ";
    }
    //
    if (useDb_ == "1")
    {
        CreateVideoComboboxDb();
    }
    else // if (useDb_ != "1")
    {
        CreateVideoComboboxXml();
    } // end if (useDb_ == "1")
    Wt::log("notice") << " ** VideoManImpl::CreateVideoCombobox() ** ";
    GetVideo();
    Wt::log("end") << " ** VideoManImpl::CreateVideoCombobox() ** ";
    return true;
} // end bool CreateVideoCombobox()
/* ****************************************************************************
 * Get Video Db
 */
bool VideoManImpl::GetVideoDb()
{
    Wt::log("start") << " *** VideoManImpl::GetVideoDb()";
    // clear all variables
    mp4Video = "";
    ogvVideo = "";
    poster = "";
    title = "";
    try
    {
        // Start a Transaction
        Wt::Dbo::Transaction t(session_);
        //Wt::Dbo::QueryModel< Wt::Dbo::ptr<VideoMan> > *model = new Wt::Dbo::QueryModel< Wt::Dbo::ptr<VideoMan> >();
        //model->setQuery(session_.query< Wt::Dbo::ptr<VideoMan> >("select u from videoman u").where("name = ?").bind(videoCombo->currentText().toUTF8()), false);
        //Wt::Dbo::ptr<VideoMan> playVideo = session_.find<VideoMan>().where("name = ? AND language = ?").bind(ComboVideo->currentText().toUTF8()).bind(lang_);
        Wt::Dbo::ptr<VideoMan> playVideo = session_.find<VideoMan>().where("name = ?").bind(ComboVideo->currentText().toUTF8()).where("language = ?").bind(lang_);
        //Wt::Dbo::ptr<VideoMan> playVideo = session_.find<VideoMan>().where("name = ?").bind(ComboVideo->currentText().toUTF8());
        // Set Text for Top iFrame
        if (playVideo->pagetop.empty())
        {
            TextPageTopIframe = Wt::WString::Empty;
        }
        else
        {
            TextPageTopIframe = "<div id='vpagetop' class='vpagetop' style='width:" + (playVideo->pagetopwidth.empty() ? defaultPageTopWidth : playVideo->pagetopwidth) + ";height:" + (playVideo->pagetopheight.empty() ? defaultPageTopHeight : playVideo->pagetopheight) + ";'>" + playVideo->pagetop + "</div>";
        }

        /*
        if (!playVideo->pagebottomlink.empty())
        {

            //bottomPage = new Wt::WText("<div id='vpagebottomlink' class='vpagebottomlink' style='width:" + playVideo->pagebottomwidth + "; height:" + playVideo->pagebottomheight + ";'><iframe id='pagebottomframe' theme='" + myTheme + "' src='" + playVideo->pagebottomlink + "' style='width:" + playVideo->pagebottomwidth + "; height:" + playVideo->pagebottomheight + ";' frameBorder='1' scrolling='auto' ></iframe></div>", Wt::XHTMLUnsafeText, bindItems);
            //bottomPage = new Wt::WText("<div id='vpagebottomlink' class='vpagebottomlink' style='width:" + playVideo->pagebottomwidth + "; height:" + playVideo->pagebottomheight + ";'><iframe id='pagebottomframe' theme='" + myTheme + "' style='width:" + playVideo->pagebottomwidth + "; height:" + playVideo->pagebottomheight + ";' frameBorder='1' scrolling='auto' ></iframe></div>", Wt::XHTMLUnsafeText, bindItems);
            //bottomPage->setStyleClass("videomanbottompage");
        }
        */
        //
        if (playVideo->pagebottom.empty())
        {
            TextPageBottomDiv = Wt::WString::Empty;
        }
        else
        {
            //Wt::WString tempPage = Wt::WString::fromUTF8("<div id='vpagebottom' class='vpagebottom' style='width:" + playVideo->pagebottomwidth + ";height:" + playVideo->pagebottomheight + ";'>" + playVideo->pagebottom + "</div>");
            //Wt::log("error") << "VideoManImpl::GetVideoDb: set tempPage size = " << tempPage.size();
            //bottomPage =  new Wt::WText(Wt::WString::fromUTF8("<div id='vpagebottom' class='vpagebottom' style='width:" + playVideo->pagebottomwidth + ";height:" + playVideo->pagebottomheight + ";'>" + playVideo->pagebottom + "</div>"), Wt::XHTMLUnsafeText, bindItems);
            // bottomPage->setStyleClass("videomanbottompage");
            //pageBottom = playVideo->pagebottom;
            //TextPageBottomIframe = "<div id='vpagebottom' class='vpagebottom' style='width:" + pageBottomWidth + ";height:" + pageBottomHeight + ";'>" + playVideo->pagebottom + "</div>";
            TextPageBottomDiv = "<div id='vpagebottom' class='vpagebottom' style='width:" + (playVideo->pagebottomwidth.empty() ? defaultPageBottomWidth : playVideo->pagebottomwidth) + ";height:" + (playVideo->pagebottomheight.empty() ? defaultPageBottomHeight : playVideo->pagebottomheight) + ";'>" + playVideo->pagebottom + "</div>";
        }

        //
        if (!playVideo->isYouTube)
        {

            /* sizes="1080,720"
             *
             */
            std::vector <std::string> sizesFields;
            boost::split( sizesFields, playVideo->sizes, boost::is_any_of( "," ) );
            //
            size = sizesFields[0]; // FIXIT read from combobox
            int sizeCount = sizesFields.size();
            if (sizeCount > 1)
            {
                // Create a dropdown box
                if (!ComboSizes) // FIXIT test: does this work?
                {
                    ComboSizes = new Wt::WComboBox(items_);
                    Wt::log("notice") << "VideoManImpl::GetVideoDb()  new ComboBox ComboSizes";
                }
                else
                {
                    size = ComboSizes->currentText().toUTF8();
                }
                ComboSizes->clear();
                for (int sizecnt = 0; sizecnt < sizeCount;sizecnt++)
                {
                    if (ComboSizes->findText(sizesFields[sizecnt]) == -1)
                    {
                        ComboSizes->addItem(sizesFields[sizecnt]); // it did not exist, add it
                    }
                    ComboSizes->setCurrentIndex(0);
                }
                size = sizesFields[0]; // FIXIT read from combobox
            }
            //  quality="hd,lq"
            std::vector <std::string> qualityFields;
            boost::split( qualityFields, playVideo->quality, boost::is_any_of( "," ) );
            int qualityCount = qualityFields.size();
            quality = qualityFields[0]; // FIXME read from combobox
            if (qualityCount > 1)
            {
                // Create a dropdown box
                if (!ComboQuality)
                {
                    ComboQuality = new Wt::WComboBox(items_);
                    Wt::log("notice") << "VideoManImpl::GetVideoDb()  new ComboBox ComboQuality";
                }
                else
                {
                    quality = ComboQuality->currentText().toUTF8();
                }
                ComboQuality->clear();
                for (int qualitycnt=0;qualitycnt<sizeCount;qualitycnt++)
                {
                    if (ComboQuality->findText(qualityFields[qualitycnt]) == -1)
                    {
                        ComboQuality->addItem(qualityFields[qualitycnt]); // it did not exist, add it
                    }
                    ComboQuality->setCurrentIndex(0);
                } // end for (int qualitycnt=0;qualitycnt<sizeCount;qualitycnt++)
            } // end if (qualityCount > 1)
            //
            if (playVideo->isM4v)
            {
                mp4Video = playVideo->path + "-" + quality + "-" + size + ".m4v";
            }
            if (playVideo->isOgv)
            {
                ogvVideo = playVideo->path + "-" + quality + "-" + size + ".ogv";
            }
            poster = playVideo->poster;

        } // end if (playVideo->isutube)
        //
        //
        if (playVideo->isYouTube)
        {
            /* FIXME:
             * Add field for aspectRatio
             */
            std::string aspectRatio;
            if (playVideo->aspectRatio == "16by9")
            {
                aspectRatio = "embed-responsive embed-responsive-16by9";
            }
            else
            {
                aspectRatio = "embed-responsive embed-responsive-4by3";
            }
            TextYouTubeIframe = Wt::WString::fromUTF8("<div id='vplayer' class='vplayer'><br /><div id='yewtube' class='" + aspectRatio + "'><iframe id='vframe' class='embed-responsive-item' src='" + playVideo->path + "' width='" + playVideo->width + "' height='" + playVideo->height + "' style='' frameBorder='0' scrolling='no' allowfullscreen='true'></iframe></div><br /></div>");
        }
        else
        {
            TextYouTubeIframe = Wt::WString::Empty;
        #ifdef WVIDEO
            if (!player)
            {
                // Create Video Player
                player = new Wt::WVideo(items_);
            }
            else
            {
                player->clearSources();
            }
            if (!mp4Video.empty())
            {
                player->addSource(mp4Video);
            }
            if (!ogvVideo.empty())
            {
                player->addSource(ogvVideo);
            }
            if (!poster.empty())
            {
                player->setPoster(poster);
                player->setAlternativeContent(new Wt::WImage(poster));
            }
            player->resize(std::stoi(playVideo->width), std::stoi(playVideo->height));
        #else
            //
            //Wt::WApplication::instance()-> useStyleSheet(Wt::WApplication::resourcesUrl() + "jPlayer/skin/blue.monday/jplayer.blue.monday.css");
            //Wt::WApplication::instance()-> useStyleSheet(Wt::WApplication::resourcesUrl() + "jPlayer/skin/pink.flag/jplayer.pink.flag.css");
            //
            // Create Video Player
            if (!player)
            {
                player = new Wt::WMediaPlayer(Wt::WMediaPlayer::Video, items_);
            }
            else
            {
                player->clearSources();
            }
            if (!mp4Video.empty())
            {
                player->addSource(Wt::WMediaPlayer::M4V, Wt::WLink(mp4Video));
            }
            if (!ogvVideo.empty())
            {
                player->addSource(Wt::WMediaPlayer::OGV, Wt::WLink(ogvVideo));
            }
            if (!poster.empty())
            {
                player->addSource(Wt::WMediaPlayer::PosterImage, Wt::WLink(poster));
            }
            if (!title.empty())
            {
                player->setTitle(title);
            }
            player->setVideoSize(std::stoi(playVideo->width), std::stoi(playVideo->height));
        #endif
        } // end if (playVideo->isutube)
        // FIXME
        videoManTemplate->bindWidget("videomanpagetop", WrapView(&VideoManImpl::PageContentTop));

        videoManTemplate->bindWidget("videoman", WrapView(&VideoManImpl::YouTubeContent));

        // Chech to see if an audio widget is in text
        if (TextPageBottomDiv.toUTF8().find("widget:audio") != std::string::npos)
        {
            videoManTemplate->bindWidget("videomanpagebottom", WrapView(&VideoManImpl::GetTemplate));
        }
        else
        {
            videoManTemplate->bindWidget("videomanpagebottom", WrapView(&VideoManImpl::PageContentBottom));
        }
        /*
        std::string jsPageBottom = "document.getElementById('pagebottomframe').src='" + playVideo->pagebottomlink + "';";
        Wt::log("notice") << " VideoManImpl::GetVideoDb() jsPageBottom = " << jsPageBottom << " ";
        this->doJavaScript(jsPageBottom);
        */

        // Commit Transaction
        t.commit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManImpl::GetVideoDb: Failed reading from videoman database";
        Wt::log("error") << "VideoManImpl::GetVideoDb()  Failed reading from videoman database";
        return false;
    }
    return true;
} // end GetVideoDb()
/* ****************************************************************************
 * GetTemplate
 *
 */
Wt::WWidget* VideoManImpl::GetTemplate()
{
    Wt::WTemplate* myTemplate = new Wt::WTemplate(TextPageBottomDiv);
    Wt::log("notice") << " VideoManImpl::GetVideoDb() found audio tag";
    WidgetFunction widgetFunction;

    widgetFunction.registerType("audio", WidgetFunction::createAudio);

    myTemplate->addFunction("widget", widgetFunction);
    return myTemplate;
} // end GetTemplate
/* ****************************************************************************
 * get Video XML
 */
bool VideoManImpl::GetVideoXml()
{
    Wt::log("start") << " *** VideoManImpl::GetVideoXml() videoText = " << videoText << " ***";
    std::string myLang = + "_" + lang_;
    bool isTemplate = false;
    if (lang_ == "en")
    {
        myLang = "";
    }
    isTemplate = BindTemplate(appPath_ + videoText, appPath_ + videoText + myLang + ".xml", videoText);
    /*
    Wt::log("info") << "VideoManImpl::GetVideoXml() videoTemplate->children() = " << videoTemplate->children().size();

    for(std::vector<Wt::WWidget *>::const_iterator it = videoTemplate->children().begin(); it != videoTemplate->children().end(); ++it)
    {
        Wt::log("info") << "VideoManImpl::GetVideoXml() videoTemplate->children() = " << *it;
    Wt::WContainerWidget *container = new Wt::WContainerWidget();
        Wt::WWidget* myTag;
        switch(*it.value_type("type"))
        {
            case "audio/mpeg":
                myTag = *it;
                videoTemplate->bindWidget(*it.value_type("id"), WrapView(&VideoManImpl::GetAudioPlayer));
                break;
        }
    }
        */
    // patch messageResourceBundle, to look for wittywizard tag

    Wt::log("end") << " ** VideoManImpl::GetVideoXml() **";
    return isTemplate;
} // end GetVideoXml
/* ****************************************************************************
 * Bind Template
 */
bool VideoManImpl::BindTemplate(std::string resourceFile, std::string filePath, std::string templateName)
{
    if (!Crystallball::IsFile(filePath))
    {
        Wt::log("error") << "**** VideoManImpl::BindTemplate() missing file: " <<  filePath << " ****";
        return false;
    }
    app->messageResourceBundle().use(resourceFile, false);
    videoManPage_ = videoManTemplate = new Wt::WTemplate(Wt::WString::tr(templateName + "-template"), this); //  <message id="?-template">
    // Open XML File
    rapidxml::file<> xmlFile(filePath.c_str());
    rapidxml::xml_document<> xmlDoc;
    xmlDoc.parse<0>(xmlFile.data()); // rapidxml::parse_non_destructive
    // Find our root node
    rapidxml::xml_node<> * root_node = xmlDoc.first_node("messages");
    // define xml item
    rapidxml::xml_attribute<> *x_item;
    //
    for (rapidxml::xml_node<> * messages_node = root_node->first_node("message"); messages_node; messages_node = messages_node->next_sibling("message"))
    {
        // id
        x_item = messages_node->first_attribute("id");
        if (!x_item)
        {
            Wt::log("error") << "-> BindTemplate() Missing XML Element: id = " << messages_node->name();
            return false;
        }
        if (x_item->value() == templateName + "-template")
        {
            BindTags(messages_node);
        } // end if (x_item->value() == templateName + "-template")
    } // end for (rapidxml::xml_node<> * domain_node = root_node->first_node("domain"); domain_node; domain_node = domain_node->next_sibling("domain"))
    return true;
} // end BindTemplate
/* ****************************************************************************
 * BindTags
 */
void VideoManImpl::BindTags(const rapidxml::xml_node<>* node)
{
    const rapidxml::node_type nodeType = node->type();
    switch(nodeType)
    {
        case rapidxml::node_element:
            //Wt::log("info") << "BindTags() node_element -> node->name() = " << node->name();
            if(strcmp(node->name(), "wt") == 0)
            {
                // Clear tags
                wtTag.id = "";
                wtTag.type = "";
                wtTag.myClass = "";
                wtTag.width = "";
                wtTag.height = "";
                wtTag.src = "";
                for(const rapidxml::xml_attribute<>* a = node->first_attribute(); a; a = a->next_attribute())
                {
                    // Wt::log("info") << "-> BindTags() node_element attribute a->name() = " << a->name();
                    //
                    if(strcmp(a->name(), "type") == 0)
                    {
                        wtTag.type = a->value();
                    } // end if(strcmp(node->name(), "type") == 0)
                    if(strcmp(a->name(), "id") == 0)
                    {
                        wtTag.id = a->value();
                    }
                    if(strcmp(a->name(), "class") == 0)
                    {
                        wtTag.myClass = a->value();
                    }
                    if(strcmp(a->name(), "width") == 0)
                    {
                        wtTag.width = a->value();
                    }
                    if(strcmp(a->name(), "height") == 0)
                    {
                        wtTag.height = a->value();
                    }
                    if(strcmp(a->name(), "src") == 0)
                    {
                        wtTag.src = a->value();
                    }
                } // end for(const rapidxml::xml_attribute<>* a = node->first_attribute(); a; a = a->next_attribute())
                //
                if(wtTag.type == "audio/mpeg")
                {
                    //Wt::log("info") << "BindTags() node_element attribute wtTag.type = " << wtTag.type << "  wtTag.id = " << wtTag.id << "  wtTag.myClass = " << wtTag.myClass << "  wtTag.width = " << wtTag.width << "  wtTag.height = " << wtTag.height << "  wtTag.src = " << wtTag.src;
                    //videoManTemplate->bindWidget(wtTag.id, WrapView(&VideoManImpl::GetAudioPlayer));
                    //videoManTemplate->bindWidget(wtTag.id, VideoManImpl::GetAudioPlayer());
                    videoManTemplate->bindWidget(wtTag.id, new Wt::WText(""));
                } // end if(strcmp(node->value(), "audio/mpeg") == 0)
            } // end if(strcmp(node->name(), "wt") == 0)
            for(const rapidxml::xml_node<>* n = node->first_node(); n; n = n->next_sibling())
            {
                BindTags(n);
            }
            break;

        case rapidxml::node_data:
            //Wt::log("info") << "-> BindTags() node_data node->value() = " << node->value();
            break;

        default:
            //Wt::log("info") << "-> BindTags() NODE-TYPE = " << t;
            break;
    } // end switch(nodeType)
} // end BindTags
/* ****************************************************************************
 * Get Audio Player
 *
 */
Wt::WWidget* VideoManImpl::GetAudioPlayer()
{
    Wt::WContainerWidget *result = new Wt::WContainerWidget();
    Wt::WAudio *audio = new Wt::WAudio(result);
    result->setStyleClass(wtTag.myClass);
    //audio->setStyleClass(wtTag.myClass);
    audio->setId(wtTag.id);
    audio->setWidth(std::stoi(wtTag.width));
    audio->setHeight(std::stoi(wtTag.height));
    audio->addSource(Wt::WLink(wtTag.src));
    //audio->addSource(Wt::WLink(wtTag.altsrc));
    audio->setOptions(Wt::WAudio::Controls);
    audio->setAlternativeContent(new Wt::WText(Wt::WString::tr("videoman-audio-support")));
    // Wt::log("info") << "VideoManImpl::GetAudioPlayer wtTag.type = " << wtTag.type << "  wtTag.id = " << wtTag.id << "  wtTag.myClass = " << wtTag.myClass << "  wtTag.width = " << wtTag.width << "  wtTag.height = " << wtTag.height << "  wtTag.src = " << wtTag.src;
    return result;
} // end GetAudioPlayer
/* ****************************************************************************
 * Get Video
 */
void VideoManImpl::GetVideo()
{
    // check cookie
    Wt::log("start") << " *** VideoManImpl::GetVideo() ~ videoText = {" << videoText << "} | numberCats = " << numberCateories << " | lang_ = " << lang_  << " *** ";
    try
    {
        if (ComboVideo != NULL)
        {
            if (videoText.empty())
            {
                videoText = ComboVideo->currentText().toUTF8();
                Wt::log("warning") << "VideoManImpl::getVideo()  set empty videoText = " <<  videoText  << " | count = " << ComboVideo->count() << " ";
            }
            else
            {
                isChanged = true;
                ComboVideo->setCurrentIndex(ComboVideo->findText(videoText));
                if (ComboVideo->currentIndex() == -1)
                {
                    ComboVideo->setCurrentIndex(0); // Not found, set it to the first Video
                    videoText = ComboVideo->currentText().toUTF8(); // Reset the videoText to new text, this should be an old video link, missing, renamed or deleted video
                }
                Wt::log("info") << "VideoManImpl::getVideo() Find videoText {"  << videoText << "} | ComboVideo = " << " | " << ComboVideo->currentText().toUTF8()  << " | count = " << ComboVideo->count() <<  " ";
                isChanged = false;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManImpl::getVideo: videoText.empty";
        Wt::log("error") << "VideoManImpl::getVideo:  videoText.empty";
    }

    //
    if (useDb_ == "1")
    {
        GetVideoDb();
    }
    else
    {
        GetVideoXml();
    }
    //

    Crystallball::SetCookie("videomancat", categoryPath);
    Crystallball::SetCookie("videomanvideo", videoText);
    Crystallball::SetCookie("videomanquery", categoryQuery);
    // Set Internal Path to Video
    // FIXIT add set Cookie support
    isChanged = true;
    SetPath();
    isChanged = false;
    currentVideoIndex = oldVideoIndex = ComboVideo->currentIndex();
    Wt::log("end") << " ** VideoManImpl::getVideo() ** ";
} // end GetVideo()
/* ****************************************************************************
 * Set Path
 */
void VideoManImpl::SetPath()
{
    switch (numberCateories)
    {
        case 0:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboVideo->currentText().toUTF8(), true);
            break;
        case 1:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8() + "/" + ComboVideo->currentText().toUTF8(), true);
            Wt::log("notice") << "VideoManImpl::SetPath() set Internal Path = " << ComboCategory_0->currentText().toUTF8() << "/" << ComboVideo->currentText().toUTF8() << " >>>>>>>";
            break;
        case 2:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8()  + "/" + ComboCategory_1->currentText().toUTF8() + "/" + ComboVideo->currentText().toUTF8(), true);
            break;
        case 3:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8() + "/" + ComboCategory_1->currentText().toUTF8() + "/" + ComboCategory_2->currentText().toUTF8() + "/" + ComboVideo->currentText().toUTF8(), true);
            break;
        case 4:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8() + "/" + ComboCategory_1->currentText().toUTF8() + "/" + ComboCategory_2->currentText().toUTF8() + "/" + ComboCategory_3->currentText().toUTF8() + "/" + ComboVideo->currentText().toUTF8(), true);
            break;
        case 5:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8() + "/" + ComboCategory_1->currentText().toUTF8() + "/" + ComboCategory_2->currentText().toUTF8() + "/" + ComboCategory_3->currentText().toUTF8() + "/" + ComboCategory_4->currentText().toUTF8() + "/" + ComboVideo->currentText().toUTF8(), true);
            break;
        case 6:
            Wt::WApplication::instance()->setInternalPath(basePath_ + ComboCategory_0->currentText().toUTF8() + "/" + ComboCategory_1->currentText().toUTF8() + "/" + ComboCategory_2->currentText().toUTF8() + "/" + ComboCategory_3->currentText().toUTF8() + "/" + ComboCategory_4->currentText().toUTF8() + "/" + ComboCategory_5->currentText().toUTF8()+ "/" + ComboVideo->currentText().toUTF8(), true);
            break;
    }
} // end SetPath
/* ****************************************************************************
 * Page Content Top
 */
Wt::WWidget* VideoManImpl::PageContentTop()
{
    WContainerWidget *result = new WContainerWidget();
    if (!TextPageTopIframe.empty())
    {
        Wt::WText *w = new Wt::WText(TextPageTopIframe, Wt::XHTMLUnsafeText, result);
        w->setInternalPathEncoding(true);
    }
    return result;
} // end PageContentTop
/* ****************************************************************************
 * Page Content Bottom
 */
Wt::WWidget* VideoManImpl::PageContentBottom()
{
    WContainerWidget *result = new WContainerWidget();
    if (!TextPageBottomDiv.empty())
    {
        Wt::WText *w = new Wt::WText(TextPageBottomDiv, Wt::XHTMLUnsafeText, result);
        w->setInternalPathEncoding(true);
    }
    return result;
} // end PageContentBottom
/* ****************************************************************************
 * You Tube Content
 */
Wt::WWidget* VideoManImpl::YouTubeContent()
{
    WContainerWidget *result = new WContainerWidget();
    if (!TextYouTubeIframe.empty())
    {
        Wt::WText *w = new Wt::WText(TextYouTubeIframe, Wt::XHTMLUnsafeText, result);
        w->setInternalPathEncoding(true);
    }
    return result;
} // end YouTubeContent
/* ****************************************************************************
 * Wrap View
 */
Wt::WWidget* VideoManImpl::WrapView(Wt::WWidget *(VideoManImpl::*createWidget)())
{
    return makeStaticModel(boost::bind(createWidget, this));
} // end WrapView
/* ****************************************************************************
 * Edit
 */
void VideoManImpl::Edit()
{
    Wt::WTemplate *result = new Wt::WTemplate(Wt::WString::tr("videoman-edit-form-template"));

    Wt::WLineEdit *name = new Wt::WLineEdit();
    result->bindWidget("name", name);
    name->setEmptyText("first name");

    Wt::WPushButton *button = new Wt::WPushButton("OK");
    result->bindWidget("button", button);

    Wt::WText *out = new Wt::WText("");
    result->bindWidget("out", out);

    button->clicked().connect(std::bind([=] () {
        out->setText("Hello, " + name->text() + "! I just want to help you... You"
                     + " could complete this simple form by adding validation.");
    }));

    Wt::WCheckBox *cb = new Wt::WCheckBox("Check me!"); // , result
    cb->setChecked(true);

} // end Edit
/* ****************************************************************************
 * Category Combo Changed
 */
void VideoManImpl::ComboCategoryChanged()
{
    //videoCombo->setCurrentIndex(0);
    // Make sure we want to update URL
    if (!isChanged)
    {
        categoryQuery = GetCategories("|");
        videoText = ""; // delete video, set index 0
        Wt::log("notice") << "-> VideoManImpl::ComboCategoryChanged() # Categories: " << std::to_string(numberCateories) << " : categoryQuery = __" << categoryQuery << "__";
        CreateVideoCombobox();
    }
} // end void ComboCategoryChanged()
/* ****************************************************************************
 * Combo Video Changed
 */
void VideoManImpl::ComboVideoChanged()
{
    Wt::log("start!!!!!!!!") << " *** VideoManImpl::ComboVideoChanged() *** currentVideoIndex = " << currentVideoIndex << " | oldVideoIndex = " << oldVideoIndex;
    // videoIndex = videoCombo->currentIndex();
    // Make sure we want to update URL
    currentVideoIndex = ComboVideo->currentIndex();
    if (!isChanged && currentVideoIndex != oldVideoIndex)
    {
        Wt::log("notice") << "-> VideoManImpl::ComboVideoChanged() currentIndex = " << std::to_string(ComboVideo->currentIndex()) << " ";
        videoText = ComboVideo->currentText().toUTF8();
        GetVideo();
    }
} // end void ComboVideoChanged()
/* ****************************************************************************
 * Get Categories from Catergory Combo
 * delimitor: | = categoryQuery, / category Path
 * categoryType catType,
 */
std::string VideoManImpl::GetCategories(std::string delimitor)
{
    if (ComboCategory_0 == NULL)
    {
        // FIXIT if number of categories = 0 set to ""
        categoryText_0 = Crystallball::GetCookie("videomanquery");
        if (categoryText_0.empty())
        {
            categoryText_0 = "";
        }
    }
    std::string newCategory = "";
    try
    {
        switch (numberCateories)
        {
            case 0:
                // FIXIT only uses video combobox
                newCategory = "";
                break;
            case 1:
                if (ComboCategory_0 != NULL)
                {
                    newCategory = categoryText_0 = ComboCategory_0->currentText().toUTF8(); // Change categoryPath
                }
                else
                {
                    newCategory = categoryText_0 = Crystallball::GetCookie("videomanquery");
                    if (newCategory.empty())
                    {
                        newCategory = categoryText_0 = "";
                    }
                }
                break;
            case 2:
                if (ComboCategory_0 != NULL && ComboCategory_1 != NULL)
                {
                    newCategory = ComboCategory_0->currentText().toUTF8() + delimitor + ComboCategory_1->currentText().toUTF8();
                }
                break;
            case 3:
                if (ComboCategory_0 != NULL && ComboCategory_1 != NULL && ComboCategory_2 != NULL)
                {
                    newCategory = ComboCategory_0->currentText().toUTF8() + delimitor + ComboCategory_1->currentText().toUTF8() + delimitor + ComboCategory_2->currentText().toUTF8();
                }
                break;
            case 4:
                if (ComboCategory_0 != NULL && ComboCategory_1 != NULL && ComboCategory_2 != NULL && ComboCategory_3 != NULL)
                {
                    newCategory = ComboCategory_0->currentText().toUTF8() + delimitor + ComboCategory_1->currentText().toUTF8() + delimitor + ComboCategory_2->currentText().toUTF8() + delimitor + ComboCategory_3->currentText().toUTF8();
                }
                break;
            case 5:
                if (ComboCategory_0 != NULL && ComboCategory_1 != NULL && ComboCategory_2 != NULL && ComboCategory_3 != NULL && ComboCategory_4 != NULL)
                {
                    newCategory = ComboCategory_0->currentText().toUTF8() + delimitor + ComboCategory_1->currentText().toUTF8() + delimitor + ComboCategory_2->currentText().toUTF8() + delimitor + ComboCategory_3->currentText().toUTF8() + delimitor + ComboCategory_4->currentText().toUTF8();
                }
                break;
            case 6:
                if (ComboCategory_0 != NULL && ComboCategory_1 != NULL && ComboCategory_2 != NULL && ComboCategory_3 != NULL && ComboCategory_4 != NULL && ComboCategory_5 != NULL)
                {
                    newCategory = ComboCategory_0->currentText().toUTF8() + delimitor + ComboCategory_1->currentText().toUTF8() + delimitor + ComboCategory_2->currentText().toUTF8() + delimitor + ComboCategory_3->currentText().toUTF8() + delimitor + ComboCategory_4->currentText().toUTF8() + delimitor + ComboCategory_5->currentText().toUTF8();
                }
                break;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "VideoManImpl::CreateCategoryCombo: Failed reading Categories: ";
        Wt::log("error") << "VideoManImpl::GetCategories()  Failed reading Categories: ";
    }
    return newCategory;
} // end std::string GetCategories(std::string delimitor)
/* ****************************************************************************
 * Get Categories Path from internal Path
 * Contains first Category or Video
 * /video/cat-1/cat-2/cat-3/video-1
 * or
 * /video/video-1
 */
bool VideoManImpl::GetCategoriesPath()
{
    Wt::log("start") << " *** VideoManImpl::GetCategoriesPath() ***";
    // Create an instance of app to access Internal Paths
    Wt::WApplication* app = Wt::WApplication::instance();
    if (!app->internalPathMatches(basePath_))
    {
        return false;
    }
    ClearCategories();
    std::string path = app->internalPath(); // /en/video/
    std::vector<std::string> parts;
    boost::split(parts, path, boost::is_any_of("/"));
    // path = /en/video/IAM/00-02-N-IAM | parts.size()=5 | parts[0]= | parts[1]=en | parts[2]=video | parts[3]=IAM | parts[4]=00-02-N-IAM
    // Wt::log("notice") << " @@@@@@@@@@ VideoManImpl::GetCategoriesPath() path = " << path << " | parts.size()=" << parts.size() << " | parts[0]=" << parts[0] << " | parts[1]=" << parts[1] << " | parts[2]=" << parts[2] << " | parts[3]=" << parts[3] << " | parts[4]=" << parts[4];
    // 0 Categories 1 Video
    if (parts.size() == 4)
    {
        categoryText_0 = "";
        videoText      = parts[3];
        categoryQuery = "";
        categoryPath  = "";
    }
    // 1 Categories 1 Video
    if (parts.size() == 5)
    {
        categoryText_0 = parts[3];
        videoText      = parts[4];
        categoryQuery = categoryText_0;
        categoryPath  = categoryText_0;
    }
    // 2 Categories 1 Video
    if (parts.size() == 6)
    {
        categoryText_0 = parts[3];
        categoryText_1 = parts[4];
        videoText      = parts[5];
        categoryQuery = categoryText_0 + "|" + categoryText_1;
        categoryPath  = categoryText_0 + "/" + categoryText_1;
    }
    // 3 Categories 1 Video
    if (parts.size() == 7)
    {
        categoryText_0 = parts[3];
        categoryText_1 = parts[4];
        categoryText_2 = parts[5];
        videoText      = parts[6];
        categoryQuery = categoryText_0 + "|" + categoryText_1 + "|" + categoryText_2;
        categoryPath  = categoryText_0 + "/" + categoryText_1 + "/" + categoryText_2;
    }
    // 4 Categories 1 Video
    if (parts.size() == 8)
    {
        categoryText_0 = parts[3];
        categoryText_1 = parts[4];
        categoryText_2 = parts[5];
        categoryText_3 = parts[6];
        videoText      = parts[7];
        categoryQuery = categoryText_0 + "|" + categoryText_1 + "|" + categoryText_2 + "|" + categoryText_3;
        categoryPath  = categoryText_0 + "/" + categoryText_1 + "/" + categoryText_2 + "/" + categoryText_3;
    }
    // 5 Categories 1 Video
    if (parts.size() == 9)
    {
        categoryText_0 = parts[3];
        categoryText_1 = parts[4];
        categoryText_2 = parts[5];
        categoryText_3 = parts[6];
        categoryText_4 = parts[7];
        videoText      = parts[8];
        categoryQuery = categoryText_0 + "|" + categoryText_1 + "|" + categoryText_2 + "|" + categoryText_3 + "|" + categoryText_4;
        categoryPath  = categoryText_0 + "/" + categoryText_1 + "/" + categoryText_2 + "/" + categoryText_3 + "/" + categoryText_4;
    }
    // 6 Categories 1 Video
    if (parts.size() == 10)
    {
        categoryText_0 = parts[3];
        categoryText_1 = parts[4];
        categoryText_2 = parts[5];
        categoryText_3 = parts[6];
        categoryText_4 = parts[7];
        categoryText_5 = parts[8];
        videoText      = parts[9];
        categoryQuery = categoryText_0 + "|" + categoryText_1 + "|" + categoryText_2 + "|" + categoryText_3 + "|" + categoryText_4 + "|" + categoryText_5;
        categoryPath  = categoryText_0 + "/" + categoryText_1 + "/" + categoryText_2 + "/" + categoryText_3 + "/" + categoryText_4 + "/" + categoryText_5;
    }
    bool isInternalPathLegal=false;
    if (!videoText.empty())
    {
        isInternalPathLegal = true;
    }
    Wt::log("end") << "->>>>>>> VideoManImpl::GetCategoriesPath() categoryPath = " << categoryPath << " | categoryQuery = " << categoryQuery << " | videoText = " << videoText << " <<<<<-";
    return isInternalPathLegal;
} // end bool GetCategoriesPath()
/* ****************************************************************************
 * handle Path Change
 */
void VideoManImpl::HandlePathChange(const std::string& path)
{
    if (isChanged) { return; }
    Wt::log("start") << " *** VideoManImpl::HandlePathChange(path: " << path << ") | isChanged = " << isChanged << " | basePath_ = " << basePath_ << " *** "; // /video/1/1
    // set categoryQuery
    GetCategoriesPath();
    // get app for internal paths
    Wt::WApplication *app = wApp;
    if (app->internalPathMatches(basePath_)) // /en/video/
    {
        Wt::log("notice") << "<<<<<<<<<<<<<<<<<<<<<<< VideoManImpl::HandlePathChange() basePath_ match ";
        if (!isChanged) // if we are not changing path in another function
        {
            std::string newCategory = GetCategories("|"); // check comboboxes
            if (categoryQuery == newCategory) // Has Path Changed -
            {
                if (videoText != ComboVideo->currentText().toUTF8())
                {
                    videoText = ComboVideo->currentText().toUTF8();
                    GetVideo();
                } // end if (videoText != ComboVideo->currentText().toUTF8())
            } // end if (categoryQuery == newCategory)
            newCategory = GetCategories("|"); // check comboboxes
            if (path != newCategory)
            {
                Wt::log("notice") << "<<<<<<<<<<<<<<<<<<<<<<< VideoManImpl::HandlePathChange() Change Internal Path ";
                if (ComboVideo != NULL) // not working (ComboVideo)
                {
                    videoText = ComboVideo->currentText().toUTF8();
                    Wt::WApplication::instance()->setInternalPath(basePath_ + newCategory + "/" + videoText, true);
                }
            }
        } // end if (!isChanged)
    }
    Wt::log("end") << " ** VideoManImpl::HandlePathChange() ** ";
} // end void HandlePathChange(const std::string& path)
/* ****************************************************************************
 * set Internal Base Path
 */
void VideoManImpl::SetInternalBasePath(const std::string& basePath)
{
    basePath_ = basePath;
    refresh();
} // end void SetInternalBasePath
/* ****************************************************************************
 * refresh
 */
void VideoManImpl::refresh()
{
    //HandlePathChange(wApp->internalPath());
} // end void refresh()
/* ****************************************************************************
 * Clear Categories
 */
void VideoManImpl::ClearCategories()
{
    videoText      = "";
    categoryPath   = "";
    categoryQuery  = "";
    categoryText_0 = "";
    categoryText_1 = "";
    categoryText_2 = "";
    categoryText_3 = "";
    categoryText_4 = "";
    categoryText_5 = "";
} // end void ClearCategories()
#endif // VIDEOMAN
// --- End Of File ------------------------------------------------------------
