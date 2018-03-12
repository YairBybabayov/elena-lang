//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//      Settings class header
//                                              (C)2005-2015, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef settingsH
#define settingsH

#include "elena.h"
#include "config.h"
#include "idecommon.h"

#define SRCPATH_SECTION          "srcpath"     // template source paths
#define LIBPATH_SECTION          "libpath"     // template library paths
#define RECENTFILES_SECTION      "recent_files"
#define RECENTRPOJECTS_SECTION   "recent_projects"

#define TAB_SIZE_SETTING         "configuration/settings/tabsize"
#define ENCODING_SETTING         "encoding"
#define TAB_USING_SETTING        "tabusing"
#define MAXIMIZED_SETTING        "configuration/settings/app_maximized"
#define PATH_REMEMBER_SETTING    "configuration/settings/remeber_path"
#define PROJECT_REMEMBER_SETTING "configuration/settings/remeber_project"
#define AUTO_DETECTING_SETTING   "configuration/settings/encoding_detecting"
#define LINE_NUMBERS_SETTING     "linenumbers"
#define HIGHLIGHT_SETTING        "highlightsyntax"
#define BRACKETS_SETTING         "highlightbrackets"
#define SCHEME_SETTING           "scheme"
#define TABSCORE_SETTING         "tabscore"
#define OUTPUT_SETTING           "configuration/settings/compileroutput"
#define PROJECTVIEW_SETTING      "configuration/settings/projectview"
#define CALLSTACK_SETTING        "configuration/settings/callstack"
#define MESSAGES_SETTING         "configuration/settings/messages"
#define AUTO_RECOMPILE_SETTING   "configuration/settings/autocomp"
#define AUTO_PROJECT_LOAD_SETTING "configuration/settings/autoload"
#define DEBUG_TAPE_SETTING       "debugtape"
#define FONTSIZE_SETTING         "font_size"

#define DEFAULT_PROJECT_SETTING  "configuration/settings/defaultproject"

namespace _GUI_
{

// --- Paths ---

class Paths
{
public:
   static void init(Model* model, _ELENA_::path_t appPath, _ELENA_::path_t defaultPath);

   static void setLibraryRoot(Model* model, _ELENA_::path_t libraryPath);

   static void resolveRelativePath(_ELENA_::Path& path, _ELENA_::path_t rootPath);

   static void makeRelativePath(_ELENA_::Path& path, _ELENA_::path_t rootPath);
};

// --- Settings ---

struct Settings
{
   static void init(Model* model, _ELENA_::path_t packagePath, _ELENA_::path_t libraryPath);
   static void load(Model* model, _ELENA_::XmlConfigFile& config);
   static void save(Model* model, _ELENA_::XmlConfigFile& config);

   static void onNewProjectTemplate(Model* model, _GUI_::_ProjectManager* project);

   static void addSearchHistory(Model* model, text_t line);
   static void addReplaceHistory(Model* model, text_t line);

   static void addPackagePath(Model* model, const char* projectTemplate, _ELENA_::path_t path)
   {
      model->packageRoots.erase(projectTemplate);
      model->packageRoots.add(projectTemplate, path.clone());
   }

   static void addLibraryPath(Model* model, const char* projectTemplate, _ELENA_::path_t path)
   {
      model->libraryRoots.erase(projectTemplate);
      model->libraryRoots.add(projectTemplate, path.clone());
   }
};

} // _GUI_

#endif // idesettingsH
