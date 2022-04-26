//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//                     IDE Controller header File
//                                             (C)2005-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef IDECONTROLLER_H
#define IDECONTROLLER_H

#include "controller.h"
#include "debugcontroller.h"
#include "ideproject.h"

namespace elena_lang
{
   // --- SourceViewController ---
   class SourceViewController : public TextViewController
   {
   public:
      SourceViewController(EOLMode eolMode)
         : TextViewController(eolMode)
      {
      }
   };

   // --- DebugAction ---
   enum class DebugAction
   {
      None,
      Run,
      StepInto,
      StepOver
   };

   // --- ProjectController ---
   class ProjectController : public NotifierBase
   {
      DebugController      _debugController;
      NotifierBase*        _notifier;

      bool onDebugAction(ProjectModel& model, DebugAction action);
      bool isOutaged(bool noWarning);

      bool startDebugger(ProjectModel& model/*, bool stepMode*/);

   public:
      void doNewFile(ProjectModel& model);

      bool doCompileProject(ProjectModel& model, DebugAction postponedAction);

      void doDebugAction(ProjectModel& model, DebugAction action);

      void setNotifier(NotifierBase* notifier)
      {
         _notifier = notifier;
      }

      void notifyMessage(int messageCode) override
      {
         if (_notifier)
            _notifier->notifyMessage(messageCode);
      }
      void notifyModelChange(int modelCode) override
      {
         if (_notifier)
            _notifier->notifyModelChange(modelCode);
      }

      ProjectController(DebugProcessBase* process, ProjectModel* model, SourceViewModel* sourceModel)
         : _debugController(process, model, sourceModel, this)
      {
         _notifier = nullptr;
      }
   };

   // --- IDEController ---
   class IDEController
   {
      NotifierBase* _notifier;

   public:
      SourceViewController sourceController;
      ProjectController    projectController;

      void setNotifier(NotifierBase* notifier)
      {
         _notifier = notifier;

         projectController.setNotifier(notifier);
      }

      IDEController(DebugProcessBase* process, IDEModel* model, EOLMode eolMode) :
         sourceController(eolMode),
         projectController(process, &model->projectModel, &model->sourceViewModel)
      {
         _notifier = nullptr;
      }
   };

} // elena:lang

#endif // IDECONTROLLER_H
