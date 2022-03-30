//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//      Linux-GTK+ program entry
//                                             (C)2021/2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#include "gtklinux/gtkcommon.h"
#include "factory.h"

int main(int argc, char* argv[])
{
   Gtk::Main kit(argc, argv);

   IDEFactory    factory;

   //GUIApp* app = factory.createApp();

   int retVal = 0; // !! temporal

   Gtk::Main::run(*factory.createMainWindow());

   //delete app;

   return retVal;
}
