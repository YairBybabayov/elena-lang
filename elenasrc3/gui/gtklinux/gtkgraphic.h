//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Win32 Common header
//      GTK graphic tools header
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef GTKGRAPHIC_H
#define GTKGRAPHIC_H

#include "gtklinux/gtkcommon.h"

namespace elena_lang
{

// --- Font ---

struct Font
{
//   static _ELENA_::List<Font*> Cache;
//
//   static Font* createFont(const char* fontName, int size, bool bold, bool italic);
//   static void releaseFontCache();

public:
   text_str _fontName;

   Pango::FontDescription _font;

//   void release();

   Font(const char* fontName, int size, bool bold, bool italic);
//   ~Font() { release(); }
};

// --- FontFactory ---
class FontFactory
{
   List<Font*, freeobj> _cache;

public:
   Font* createFont(wstr_t fontName, int size, int characterSet, bool bold, bool italic);

   FontFactory()
      : _cache(nullptr)
   {

   }
};

// --- Style ---

struct Style
{
   bool   valid;

   Color  foreground;
   Color  background;
   Font*  font;
   int    lineHeight;
   int    avgCharWidth;

   void validate(Glib::RefPtr<Pango::Layout> layout);

   Style();
   Style(Colour foreground, Color background, Font* font);
   ~Style();
};

////// --- DoubleBuffer ---
////
////struct Canvas;
////
////struct DoubleBuffer
////{
////   bool initialized;
////
////   cairo_surface_t* cs;
////
////   void init(Canvas& canvas, size_t width, size_t height);
////
////   bool isReleased() { return !initialized; }
////
////   void release()
////   {
////      if (initialized) {
////         initialized = false;
////
////         cairo_surface_destroy(cs);
////      }
////   }
////
////   DoubleBuffer()
////   {
////      initialized = false;
////   }
////};

// --- Canvas ---

struct Canvas : public CanvasBase
{
   const Cairo::RefPtr<Cairo::Context> cr;
   Glib::RefPtr<Pango::Layout>         layout;

   void validateStyle(Style* style) override;

//   void drawText(int x, int y, const char* s, Style& style);
//   void drawCursor(int x, int y, Style& style);
//   void drawOverwriteCursor(int x, int y, Style& style);
//
////   void copy(DoubleBuffer& buffer, int x, int y);
//
////   Canvas(GdkDrawable* drawable);
//////   Canvas(DoubleBuffer& buffer);
////   ~Canvas();
////   PangoLayout* layout;
//
//   int TextWidth(Style* style, const char* s);

   void fillRectangle(int x, int y, int width, int height, Style* style);

////   void drawText(int x, int y, const TCHAR* s, int length, Style& style);
////   void drawCursor(GtkWidget* widget, int x, int y, Style& style);
////   void drawOverwriteCursor(GtkWidget* widget, int x, int y, Style& style);
////
////   void copy(DoubleBuffer& buffer, int x, int y);
//
   Canvas(const Cairo::RefPtr<Cairo::Context>& cr);
////   Canvas(DoubleBuffer& buffer);
//   ~Canvas();
};

} // elena_lang

#endif // gtkgraphicH

