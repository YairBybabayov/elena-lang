//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     IDE common classes header File
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef IDECOMMON_H
#define IDECOMMON_H

#include "guicommon.h"

#define IDE_REVISION_NUMBER                     5

namespace elena_lang
{
   // --- GUIFactory ---
   class GUIFactoryBase
   {
   public:
      virtual GUIApp* createApp() = 0;
      virtual GUIControlBase* createMainWindow() = 0;
   };
}

#endif
