//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  Amd64 ELENA System Routines
//
//                                              (C)2020, by Alexei Rakov
//---------------------------------------------------------------------------

#include "elena.h"
// --------------------------------------------------------------------------
#include "elenamachine.h"
//#include <windows.h>
//#include <errhandlingapi.h>

using namespace _ELENA_;

const pos_t page_size = 0x10;
const pos_t page_size_order = 0x04;
const pos_t page_size_order_minus2 = 0x02;
const pos_t page_mask = 0x0FFFFFFF0;
const pos_t page_ceil = 0x17;
const pos_t page_align_mask = 0x000FFFF0;

void SystemRoutineProvider :: Init(SystemEnv* env)
{
   // ; initialize static roots
   memset(env->StatRoots, 0, env->StatLength);

   // set the GC root counter
   env->Table->gc_rootcount = env->StatLength;
   env->Table->gc_roots = (uintptr_t)env->StatRoots;

   // ; allocate memory heap
   env->Table->gc_header = NewHeap(0x8000000, align(env->GCMGSize, 128) >> page_size_order_minus2);

   uintptr_t mg_ptr = NewHeap(0x20000000, align(env->GCMGSize, 128));
   env->Table->gc_start = mg_ptr;

   // ; initialize yg
   env->Table->gc_yg_current = env->Table->gc_yg_start = mg_ptr;

   // ; initialize gc end
   env->Table->gc_end = mg_ptr + align(env->GCMGSize, 128);

   // ; initialize gc shadow
   mg_ptr += (env->GCYGSize & page_mask);
   env->Table->gc_shadow = env->Table->gc_yg_end = mg_ptr;

   // ; initialize gc mg
 //  add  eax, ecx
 //  mov  [data : %CORE_GC_TABLE + gc_shadow_end], eax
 //  mov  [data : %CORE_GC_TABLE + gc_mg_start], eax
 //  mov  [data : %CORE_GC_TABLE + gc_mg_current], eax
   mg_ptr += (env->GCYGSize & page_mask);
   env->Table->gc_shadow_end = env->Table->gc_mg_start = env->Table->gc_mg_current = mg_ptr;

   // ; initialize wbar start
   env->Table->gc_mg_wbar = ((mg_ptr - env->Table->gc_start) >> page_size_order) + env->Table->gc_header;

   env->Table->gc_signal = 0;
}

void SystemRoutineProvider::ExitThread(SystemEnv* env, pos_t exitCode, bool withExit)
{
   //entryCriticalSection(&env->Table->tt_lock);

   //TLSEntry* entry = GetTLSEntry(*env->TLSIndex);

   //env->ThreadTable[entry->tls_threadindex] = 0;

   //leaveCriticalSection(&env->Table->tt_lock);

   //CloseThreadHandle(entry, withExit, exitCode);
}

bool SystemRoutineProvider::NewThread(SystemEnv* env, ProgramHeader* frameHeader)
{
   //entryCriticalSection(&env->Table->tt_lock);

   bool valid = false;
   //pos_t threadIndex = 0;
   //for (pos_t i = 0; i < env->MaxThread; i++) {
   //   if (env->ThreadTable[i] == 0) {
   //      threadIndex = i;
   //      valid = true;
   //      break;
   //   }
   //}

   //if (valid) {
   //   InitTLSEntry(threadIndex, *env->TLSIndex, frameHeader, env->ThreadTable);
   //}

   //leaveCriticalSection(&env->Table->tt_lock);

   return valid;
}
