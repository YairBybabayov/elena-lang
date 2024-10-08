//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Engine
//               
//		This file contains the Win32 Debugger class and its helpers header
//                                             (C)2021-2024, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef WIN32DEBUGPROCESS_H
#define WIN32DEBUGPROCESS_H

#include "idecommon.h"
#include "windows/winevents.h"

namespace elena_lang
{
   // --- DebugEventManager ---
   typedef EventManager<int, MAX_DEBUG_EVENT> DebugEventManager;

   class Win32DebugProcess;
   struct Win32BreakpointContext;

   // --- Win32ThreadBreakpoint ---
   struct Win32ThreadBreakpoint
   {
      bool   software;
      bool   hardware;
      addr_t next;
      addr_t stackLevel;

      void clearSoftware()
      {
         software = false;
         next = 0;
      }

      Win32ThreadBreakpoint()
      {
         hardware = software = false;
         stackLevel = next = 0;
      }
   };

   // --- Win32ThreadContext ---
   struct Win32ThreadContext
   {
      friend class Win32DebugProcess;
      friend struct Win32BreakpointContext;

   protected:
      void*   state;
      HANDLE  hProcess;
      HANDLE  hThread;
      CONTEXT context;

   public:
      Win32ThreadBreakpoint breakpoint;

      bool readDump(addr_t address, char* dump, size_t length);
      void writeDump(addr_t address, char* dump, size_t length);

      void refresh();

      void setTrapFlag();
      void resetTrapFlag();
      void setHardwareBreakpoint(addr_t breakpoint);
      void clearHardwareBreakpoint();

      unsigned char setSoftwareBreakpoint(addr_t breakpoint);
      void clearSoftwareBreakpoint(addr_t breakpoint, unsigned char substitute);

      void setIP(addr_t address);

      Win32ThreadContext(HANDLE hProcess, HANDLE hThread);
   };

   // --- BreakpointContext ---
   struct Win32BreakpointContext
   {
      Map<size_t, char> breakpoints;

      void addBreakpoint(addr_t address, Win32ThreadContext* context, bool started);
      void removeBreakpoint(addr_t address, Win32ThreadContext* context, bool started);
      void setSoftwareBreakpoints(Win32ThreadContext* context);
      void setHardwareBreakpoint(addr_t address, Win32ThreadContext* context, bool withStackLevelControl);

      bool processStep(Win32ThreadContext* context, bool stepMode);
      bool processBreakpoint(Win32ThreadContext* context);

      void clear();

      Win32BreakpointContext();
   };

   // --- Win32DebugProcessException ---
   struct Win32DebugProcessException : DebugProcessException
   {
      
   };

   // --- Win32DebugProcess ---
   class Win32DebugProcess : public DebugProcessBase
   {
      typedef Map<int, Win32ThreadContext*, nullptr, nullptr, freeobj>  ThreadContextes;
      typedef MemoryMap<addr_t, void*, Map_StoreAddr, Map_GetAddr>      StepMap;

      class ConsoleHelper
      {
      public:
         void printText(const char* s);
         void waitForAnyKey();
      };

   protected:
      DebugEventManager          _events;
      Win32BreakpointContext     _breakpoints;
      ThreadContextes            _threads;
      Win32ThreadContext*        _current;

      addr_t                     init_breakpoint;
      addr_t                     minAddress, maxAddress;
      addr_t                     baseAddress;

      DWORD                      threadId;
      bool                       started;
      bool                       trapped;
      bool                       stepMode;
      bool                       needToHandle;
      bool                       needToFreeConsole;

      DWORD                      dwCurrentProcessId;
      DWORD                      dwCurrentThreadId;
      DWORD                      dwDebugeeProcessId;

      StepMap                    steps;

      Win32DebugProcessException exception;

      bool startProcess(const wchar_t* exePath, const wchar_t* cmdLine, bool withPersistentConsole);

      void continueProcess();
      void processEvent(DWORD timeout);
      void processException(EXCEPTION_DEBUG_INFO* exception);
      void processStep();
      void processEnd();

   public:
      void initEvents() override
      {
         _events.init(DEBUG_ACTIVE);
      }
      void setEvent(int event) override
      {
         _events.setEvent(event);
      }
      void resetEvent(int event) override
      {
         _events.resetEvent(event);
      }
      int waitForAnyEvent() override
      {
         return _events.waitForAnyEvent();
      }
      bool waitForEvent(int event, int timeout) override
      {
         return _events.waitForEvent(event, timeout);
      }
      void clearEvents() override
      {
         _events.close();
      }

      void resetException() override;

      DebugProcessException* Exception() override
      {
         return exception.code == 0 ? nullptr : &exception;
      }

      bool isStarted() override
      {
         return started;
      }

      bool isTrapped() override
      {
         return trapped;
      }

      bool isInitBreakpoint() override;

      void initHook() override { init_breakpoint = INVALID_ADDR; }

      addr_t findEntryPoint(path_t programPath) override;

      bool findSignature(StreamReader& reader, char* signature, pos_t length) override;

      bool startProgram(path_t exePath, path_t cmdLine, bool withPersistentConsole) override;

      bool proceed(int timeout) override;
      void run() override;
      void stop() override;
      void reset() override;
      void activate() override;

      void setStepMode() override;

      bool startThread(DebugControllerBase* controller) override;

      int getDataOffset() override;

      addr_t getBaseAddress() override;
      void* getState() override;

      ref_t getClassFlags(addr_t vmtAddress) override;

      addr_t getClassVMT(addr_t address) override;
      addr_t getStackItem(int index, disp_t offset = 0) override;
      addr_t getStackItemAddress(disp_t disp) override;

      addr_t getField(addr_t address, int index) override;
      addr_t getFieldAddress(addr_t address, disp_t disp) override;

      addr_t getMemoryPtr(addr_t address) override;
      char getBYTE(addr_t address) override;
      unsigned short getWORD(addr_t address) override;
      unsigned getDWORD(addr_t address) override;
      unsigned long long getQWORD(addr_t address) override;
      double getFLOAT64(addr_t address) override;

      size_t getArrayLength(addr_t address) override;

      void setBreakpoint(addr_t address, bool withStackLevelControl) override;
      void addBreakpoint(addr_t address) override;
      void removeBreakpoint(addr_t address) override;

      void addStep(addr_t address, void* current) override;

      bool readDump(addr_t address, char* s, pos_t length) override
      {
         return _current->readDump(address, s, length);
      }

      Win32DebugProcess();
   };

}

#endif
