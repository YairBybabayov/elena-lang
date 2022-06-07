//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     GTK SDI Control Implementation File
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#include "gtksdi.h"

using namespace elena_lang;

// --- SDIWindow ---

SDIWindow :: SDIWindow()
   : Gtk::Window(Gtk::WINDOW_TOPLEVEL), _box(Gtk::ORIENTATION_VERTICAL),
     _hbox(Gtk::ORIENTATION_HORIZONTAL), _vbox(Gtk::ORIENTATION_VERTICAL)
{
   _children = nullptr;
   _childCounter = 0;

   add(_box);
}

void SDIWindow :: populate(int counter, Gtk::Widget** children)
{
   _children = children;
   _childCounter = counter;
}

void SDIWindow::setLayout(int client, int top, int bottom, int right, int left)
{
   _box.pack_start(_hbox, TRUE, TRUE, 0);

   if (left != -1)
      _hbox.pack_start(*_children[left], Gtk::PACK_SHRINK);

   _hbox.pack_start(_vbox, Gtk::PACK_EXPAND_WIDGET);

   if (client != -1)
      _vbox.pack_start(*_children[client], TRUE, TRUE, 0);

   if (bottom != -1)
      _vbox.pack_start(*_children[bottom], Gtk::PACK_SHRINK);

//   if (statusbar)
//      _box.pack_start(*statusbar, Gtk::PACK_SHRINK);

   show_all_children(); // !!temporal

}
