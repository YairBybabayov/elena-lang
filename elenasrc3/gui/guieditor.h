//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     GUI common editor header File
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef GUIEDITOR_H
#define GUIEDITOR_H

#include "guicommon.h"
#include "document.h"

namespace elena_lang
{
   // --- ELENA IDE Styles ---
   constexpr auto STYLE_DEFAULT = 0;
   constexpr auto STYLE_MARGIN = 1;
   //#define SCHEME_COUNT                            2

   //#define STYLE_KEYWORD                           3
   //#define STYLE_COMMENT                           4
   //#define STYLE_OPERATOR                          5
   //#define STYLE_MESSAGE                           6
   //#define STYLE_NUMBER                            7
   //#define STYLE_STRING                            8
   //#define STYLE_HINT                              9  // !! not used
   //#define STYLE_ERROR_LINE                        10
   //#define STYLE_TRACE_LINE                        11
   //#define STYLE_TRACE                             12
   //#define STYLE_BREAKPOINT                        13
   //#define STYLE_HIGHLIGHTED_BRACKET               14
   constexpr auto STYLE_MAX = 1;

   // --- TextViewBase ---
   class TextViewModelBase
   {
   public:
      DocumentView* docView;
      bool          lineNumbersVisible;

      virtual Style* getStyle(pos_t styleKey) const = 0;
      virtual int getMarginWidth() const = 0;
      virtual int getLineHeight() const = 0;
      virtual bool isAssigned() const = 0;

      virtual void resize(Point size) = 0;

      virtual void validate(CanvasBase* canvas) = 0;

      TextViewModelBase()
      {
         lineNumbersVisible = false;
         docView = nullptr;
      }
   };

   // --- TextViewControllerBase ---
   class TextViewControllerBase
   {
   public:
      virtual void moveCaretLeft(TextViewModelBase* model, bool kbShift, bool kbCtrl) = 0;
      virtual void moveCaretRight(TextViewModelBase* model, bool kbShift, bool kbCtrl) = 0;
      virtual void moveCaretUp(TextViewModelBase* model, bool kbShift, bool kbCtrl) = 0;
      virtual void moveCaretDown(TextViewModelBase* model, bool kbShift, bool kbCtrl) = 0;

   };

}

#endif
