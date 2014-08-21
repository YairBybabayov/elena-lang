//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler Engine
//
//		This file contains ELENA JIT-X linker class.
//		Supported platforms: x86
//                                              (C)2005-2014, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef x86jitcompilerH
#define x86jitcompilerH 1

#include "jitcompiler.h"
#include "x86helper.h"

namespace _ELENA_
{

//#define JIT_TYPE_X 1

class x86JITCompiler;

// --- x86JITScope ---

struct x86JITScope
{
   _Module*          module;
   x86JITCompiler*   compiler;
   MemoryWriter*     code;
   _ReferenceHelper* helper;
   MemoryReader*     tape;
   x86LabelHelper    lh;

   bool              embeddedSymbols;
   bool              debugMode;
   int               objectSize;

   // byte code command argument
   int            argument;

   void writeReference(MemoryWriter& writer, ref_t reference, size_t disp)
   {
      helper->writeReference(writer, reference, disp, module);
   }

   ref_t resolveMessage(ref_t reference)
   {
      return helper->resolveMessage(reference, module);
   }

   SectionInfo getSection(ref_t reference)
   {
      return helper->getSection(reference, module);
   }

   x86JITScope(MemoryReader* tape, MemoryWriter* code, _ReferenceHelper* helper, x86JITCompiler* compiler, bool embeddedSymbols);
};

// --- x86JITCompiler ---

class x86JITCompiler : public JITCompiler32
{
protected:
   bool _debugMode;
   bool _embeddedSymbolMode;

   friend void writePreloadedReference(x86JITScope& scope, ref_t reference, int position, int offset, char* code);
   friend void loadCoreOp(x86JITScope& scope, char* code);

   friend void loadFunction(int opcode, x86JITScope& scope);
   //friend void loadExtensions(int opcode, x86JITScope& scope);
   friend void loadCode(int opcode, x86JITScope& scope);
   friend void loadOneByteLOp(int opcode, x86JITScope& scope);
   friend void loadOneByteOp(int opcode, x86JITScope& scope);
   friend void loadIndexOp(int opcode, x86JITScope& scope);
   friend void loadVMTIndexOp(int opcode, x86JITScope& scope);
   friend void loadVMTMIndexOp(int opcode, x86JITScope& scope);
   friend void loadNOp(int opcode, x86JITScope& scope);
   friend void loadFPOp(int opcode, x86JITScope& scope);
   friend void loadROp(int opcode, x86JITScope& scope);

   // commands
   friend void compileNop(int opcode, x86JITScope& scope);
   friend void compileBreakpoint(int opcode, x86JITScope& scope);
   friend void compileAElse(int opcode, x86JITScope& scope);
   friend void compileAThen(int opcode, x86JITScope& scope);
//   friend void compileTest(int opcode, x86JITScope& scope);
//   friend void compileTestType(int opcode, x86JITScope& scope);
//   friend void compileDElse(int opcode, x86JITScope& scope);
//   friend void compileDThen(int opcode, x86JITScope& scope);
   friend void compileIfM(int opcode, x86JITScope& scope);
   friend void compileElseM(int opcode, x86JITScope& scope);
//   friend void compileMElseVerb(int opcode, x86JITScope& scope);
//   friend void compileMThenVerb(int opcode, x86JITScope& scope);
////   //friend void compileMElseSI(int opcode, x86JITScope& scope);
////   //friend void compileMThenSI(int opcode, x86JITScope& scope);
//   friend void compileMElseAI(int opcode, x86JITScope& scope);
//   friend void compileMThenAI(int opcode, x86JITScope& scope);
   friend void compilePop(int opcode, x86JITScope& scope);
   friend void compilePopA(int opcode, x86JITScope& scope);
   friend void compilePopE(int opcode, x86JITScope& scope);
   friend void compilePopD(int opcode, x86JITScope& scope);
   friend void compilePopN(int opcode, x86JITScope& scope);
   friend void compileOpen(int opcode, x86JITScope& scope);
   friend void compilePush(int opcode, x86JITScope& scope);
   friend void compilePushFI(int opcode, x86JITScope& scope);
////   friend void compileXPushFPI(int opcode, x86JITScope& scope);
   friend void compilePushF(int opcode, x86JITScope& scope);
   friend void compilePushS(int opcode, x86JITScope& scope);
   friend void compilePushB(int opcode, x86JITScope& scope);
   friend void compilePushE(int opcode, x86JITScope& scope);
   friend void compilePushD(int opcode, x86JITScope& scope);
   friend void compilePushBI(int opcode, x86JITScope& scope);
   friend void compileCallR(int opcode, x86JITScope& scope);
////   //friend void compileSendVMTR(int opcode, x86JITScope& scope);
   friend void compileMCopy(int opcode, x86JITScope& scope);
//   friend void compileMOp(int opcode, x86JITScope& scope);
//   friend void compileMReset(int opcode, x86JITScope& scope);
//   friend void compileMSetSubj(int opcode, x86JITScope& scope);
   friend void compileASaveR(int opcode, x86JITScope& scope);
//   friend void compilePopFI(int opcode, x86JITScope& scope);
//   friend void compilePopSI(int opcode, x86JITScope& scope);
   friend void compileSCopyF(int opcode, x86JITScope& scope);
   friend void compileJump(int opcode, x86JITScope& scope);
   friend void compileHook(int opcode, x86JITScope& scope);
   friend void compileCreate(int opcode, x86JITScope& scope);
   friend void compileCreateN(int opcode, x86JITScope& scope);
   friend void compileSelectR(int opcode, x86JITScope& scope);
//   friend void compileBoxN(int opcode, x86JITScope& scope);
////   //friend void compileAccCreate(int opcode, x86JITScope& scope);
   friend void compileACopyR(int opcode, x86JITScope& scope);
   friend void compileBCopyR(int opcode, x86JITScope& scope);
////   friend void compileAccSetN(int opcode, x86JITScope& scope);
   friend void compileALoadR(int opcode, x86JITScope& scope);
   friend void compilePushA(int opcode, x86JITScope& scope);
   friend void compileACopyB(int opcode, x86JITScope& scope);
   friend void compileInvokeVMT(int opcode, x86JITScope& scope);
////   //friend void compileCallVMT(int opcode, x86JITScope& scope);
//   friend void compileIAXCopyR(int opcode, x86JITScope& scope);
//   friend void compileIAccCopyFI(int opcode, x86JITScope& scope);
//   friend void compileIAccCopySI(int opcode, x86JITScope& scope);
//   friend void compileIAccCopyBI(int opcode, x86JITScope& scope);
   friend void compileALoadAI(int opcode, x86JITScope& scope);
   friend void compileACopyS(int opcode, x86JITScope& scope);
//   friend void compileACopyBI(int opcode, x86JITScope& scope);
   friend void compileNext(int opcode, x86JITScope& scope);
   friend void compileIfE(int opcode, x86JITScope& scope);
   friend void compileElseE(int opcode, x86JITScope& scope);
   friend void compileLessE(int opcode, x86JITScope& scope);
   friend void compileNotLessE(int opcode, x86JITScope& scope);
   friend void compileIfB(int opcode, x86JITScope& scope);
   friend void compileElseB(int opcode, x86JITScope& scope);
//   friend void compileNextI(int opcode, x86JITScope& scope);
   friend void compileIfR(int opcode, x86JITScope& scope);
   friend void compileElseR(int opcode, x86JITScope& scope);
   friend void compileIfN(int opcode, x86JITScope& scope);
   friend void compileElseN(int opcode, x86JITScope& scope);
   friend void compileLessN(int opcode, x86JITScope& scope);
//   friend void compileElseSI(int opcode, x86JITScope& scope);
//   friend void compileThenSI(int opcode, x86JITScope& scope);
//   friend void compileElseFlag(int opcode, x86JITScope& scope);
//   friend void compileThenFlag(int opcode, x86JITScope& scope);
   friend void compileIndexInc(int opcode, x86JITScope& scope);
   friend void compileIndexDec(int opcode, x86JITScope& scope);
   friend void compileQuit(int opcode, x86JITScope& scope);
   friend void compileQuitN(int opcode, x86JITScope& scope);
   friend void compileSetVerb(int opcode, x86JITScope& scope);
   friend void compileSetSubj(int opcode, x86JITScope& scope);
   friend void compileOr(int opcode, x86JITScope& scope);
   friend void compilePopB(int opcode, x86JITScope& scope);
//   friend void compileCallSI(int opcode, x86JITScope& scope);
   friend void compileECopyD(int opcode, x86JITScope& scope);
   friend void compileDCopyE(int opcode, x86JITScope& scope);
   friend void compileBCopyF(int opcode, x86JITScope& scope);
   friend void compileACopyF(int opcode, x86JITScope& scope);
   friend void compileDCopy(int opcode, x86JITScope& scope);
   friend void compileECopy(int opcode, x86JITScope& scope);
   friend void compileDAdd(int opcode, x86JITScope& scope);
   friend void compileDSub(int opcode, x86JITScope& scope);
   friend void compileDAndN(int opcode, x86JITScope& scope);
   friend void compileDAddN(int opcode, x86JITScope& scope);
   friend void compileDCopyVerb(int opcode, x86JITScope& scope);
   friend void compileDCopyCount(int opcode, x86JITScope& scope);
   friend void compileDCopySubj(int opcode, x86JITScope& scope);
//   friend void compileDLoadAI(int opcode, x86JITScope& scope);
//   friend void compileDAddAI(int opcode, x86JITScope& scope);
//   friend void compileDSaveAI(int opcode, x86JITScope& scope);
   friend void compileALoad(int opcode, x86JITScope& scope);
////   friend void compileDCopyA(int opcode, x86JITScope& scope);

   // preloaded command set
   void* _inlines[0x100];
   //void* _extensions[0x100];

   // preloaded references
   IntFixedMap<void*> _preloaded;

   void writePreloadedReference(x86JITScope& scope, ref_t reference, int position, int offset, char* code);

public:
   bool isDebugMode() const { return _debugMode; }

   virtual void alignCode(MemoryWriter* writer, int alignment, bool code);

   virtual void* getPreloadedReference(ref_t reference);

   virtual void prepareCoreData(_ReferenceHelper& helper, _Memory* data, _Memory* rdata, _Memory* sdata);
   virtual void prepareCommandSet(_ReferenceHelper& helper, _Memory* code);
   virtual void prepareVMData(_ReferenceHelper& helper, _Memory* data);

   virtual void compileThreadTable(_JITLoader* loader, int maxThreadNumber);
   virtual void compileTLS(_JITLoader* loader);

   virtual void embedSymbol(_ReferenceHelper& helper, MemoryReader& tapeReader, MemoryWriter& codeWriter, _Module* module);
   virtual void compileSymbol(_ReferenceHelper& helper, MemoryReader& reader, MemoryWriter& codeWriter);
   virtual void compileProcedure(_ReferenceHelper& helper, MemoryReader& reader, MemoryWriter& codeWriter);

   virtual void loadNativeCode(_BinaryHelper& helper, MemoryWriter& writer, _Module* binary, _Memory* section);

   virtual void setStaticRootCounter(_JITLoader* loader, int counter, bool virtualMode);

   x86JITCompiler(bool debugMode, bool embeddedSymbolMode);
};

// --- compiler friend functions---
inline void writePreloadedReference(x86JITScope& scope, ref_t reference, int position, int offset, char* code);
void loadCoreOp(x86JITScope& scope, char* code);
void loadFunction(int opcode, x86JITScope& scope);
void loadExtensions(int opcode, x86JITScope& scope);
void loadCode(int opcode, x86JITScope& scope);
void loadOneByteLOp(int opcode, x86JITScope& scope);
void loadOneByteOp(int opcode, x86JITScope& scope);
void loadIndexOp(int opcode, x86JITScope& scope);
void loadVMTIndexOp(int opcode, x86JITScope& scope);
void loadVMTMIndexOp(int opcode, x86JITScope& scope);
void loadNOp(int opcode, x86JITScope& scope);
void loadFPOp(int opcode, x86JITScope& scope);
void loadROp(int opcode, x86JITScope& scope);
void compileNop(int opcode, x86JITScope& scope);
void compileBreakpoint(int opcode, x86JITScope& scope);
//void compileAElse(int opcode, x86JITScope& scope);
//void compileAThen(int opcode, x86JITScope& scope);
//void compileTest(int opcode, x86JITScope& scope);
//void compileTestType(int opcode, x86JITScope& scope);
//void compileDElse(int opcode, x86JITScope& scope);
//void compileDThen(int opcode, x86JITScope& scope);
void compilePop(int opcode, x86JITScope& scope);
void compilePopA(int opcode, x86JITScope& scope);
void compilePopN(int opcode, x86JITScope& scope);
void compileOpen(int opcode, x86JITScope& scope);
void compilePush(int opcode, x86JITScope& scope);
void compilePushF(int opcode, x86JITScope& scope);
////void compileXPushFPI(int opcode, x86JITScope& scope);
void compilePushFI(int opcode, x86JITScope& scope);
void compilePushS(int opcode, x86JITScope& scope);
void compilePushB(int opcode, x86JITScope& scope);
void compilePushE(int opcode, x86JITScope& scope);
void compilePushD(int opcode, x86JITScope& scope);
void compilePushBI(int opcode, x86JITScope& scope);
void compileCallR(int opcode, x86JITScope& scope);
//////void compileSendVMTR(int opcode, x86JITScope& scope);
void compileMCopy(int opcode, x86JITScope& scope);
//void compileMReset(int opcode, x86JITScope& scope);
void compileASaveR(int opcode, x86JITScope& scope);
//void compilePopFI(int opcode, x86JITScope& scope);
//void compilePopSI(int opcode, x86JITScope& scope);
void compileSCopyF(int opcode, x86JITScope& scope);
void compileJump(int opcode, x86JITScope& scope);
void compileHook(int opcode, x86JITScope& scope);
////////void compileAccLoadField(int opcode, x86JITScope& scope);
void compileCreate(int opcode, x86JITScope& scope);
void compileCreateN(int opcode, x86JITScope& scope);
void compileSelectR(int opcode, x86JITScope& scope);
//void compileBoxN(int opcode, x86JITScope& scope);
//////void compileAccCreate(int opcode, x86JITScope& scope);
void compileACopyR(int opcode, x86JITScope& scope);
void compileBCopyR(int opcode, x86JITScope& scope);
////void compileAccSetN(int opcode, x86JITScope& scope);
////////void compileAccSet(int opcode, x86JITScope& scope);
void compileALoadR(int opcode, x86JITScope& scope);
void compilePushA(int opcode, x86JITScope& scope);
void compileACopyB(int opcode, x86JITScope& scope);
void compileInvokeVMT(int opcode, x86JITScope& scope);
//////void compileCallVMT(int opcode, x86JITScope& scope);
//void compileMSetSubj(int opcode, x86JITScope& scope);
////void compileAAdd(int opcode, x86JITScope& scope);
////void compileAMul(int opcode, x86JITScope& scope);
void compileIfM(int opcode, x86JITScope& scope);
void compileElseM(int opcode, x86JITScope& scope);
//void compileMElseVerb(int opcode, x86JITScope& scope);
//void compileMThenVerb(int opcode, x86JITScope& scope);
//void compileIAXCopyR(int opcode, x86JITScope& scope);
//void compileIAccCopyFI(int opcode, x86JITScope& scope);
//void compileIAccCopySI(int opcode, x86JITScope& scope);
//void compileIAccCopyBI(int opcode, x86JITScope& scope);
void compileALoadAI(int opcode, x86JITScope& scope);
void compileACopyS(int opcode, x86JITScope& scope);
//void compileACopyBI(int opcode, x86JITScope& scope);
void compileNext(int opcode, x86JITScope& scope);
void compileIfE(int opcode, x86JITScope& scope);
void compileElseE(int opcode, x86JITScope& scope);
void compileLessE(int opcode, x86JITScope& scope);
void compileNotLessE(int opcode, x86JITScope& scope);
void compileIfB(int opcode, x86JITScope& scope);
void compileElseB(int opcode, x86JITScope& scope);
//void compileNextI(int opcode, x86JITScope& scope);
void compileIfR(int opcode, x86JITScope& scope);
void compileElseR(int opcode, x86JITScope& scope);
void compileIfN(int opcode, x86JITScope& scope);
void compileElseN(int opcode, x86JITScope& scope);
void compileLessN(int opcode, x86JITScope& scope);
void compileIndexInc(int opcode, x86JITScope& scope);
void compileIndexDec(int opcode, x86JITScope& scope);
//////void compileAccCopyM(int opcode, x86JITScope& scope);
//void compileElseSI(int opcode, x86JITScope& scope);
//void compileThenSI(int opcode, x86JITScope& scope);
//void compileElseFlag(int opcode, x86JITScope& scope);
//void compileThenFlag(int opcode, x86JITScope& scope);
void compileQuit(int opcode, x86JITScope& scope);
void compileQuitN(int opcode, x86JITScope& scope);
//void compileMPush(int opcode, x86JITScope& scope);
void compilePopE(int opcode, x86JITScope& scope);
void compilePopD(int opcode, x86JITScope& scope);
void compileSetVerb(int opcode, x86JITScope& scope);
void compileSetSubj(int opcode, x86JITScope& scope);
void compileOr(int opcode, x86JITScope& scope);
void compilePopB(int opcode, x86JITScope& scope);
//void compileCallSI(int opcode, x86JITScope& scope);
void compileECopyD(int opcode, x86JITScope& scope);
void compileDCopyE(int opcode, x86JITScope& scope);
void compileBCopyF(int opcode, x86JITScope& scope);
void compileACopyF(int opcode, x86JITScope& scope);
//void compileMElseAI(int opcode, x86JITScope& scope);
//void compileMThenAI(int opcode, x86JITScope& scope);
void compileDCopy(int opcode, x86JITScope& scope);
void compileECopy(int opcode, x86JITScope& scope);
void compileDAdd(int opcode, x86JITScope& scope);
void compileDSub(int opcode, x86JITScope& scope);
void compileDAndN(int opcode, x86JITScope& scope);
void compileDAddN(int opcode, x86JITScope& scope);
void compileDCopyVerb(int opcode, x86JITScope& scope);
void compileDCopyCount(int opcode, x86JITScope& scope);
void compileDCopyCount(int opcode, x86JITScope& scope);
//void compileDLoadAI(int opcode, x86JITScope& scope);
//void compileDAddAI(int opcode, x86JITScope& scope);
//void compileDSaveAI(int opcode, x86JITScope& scope);
void compileALoad(int opcode, x86JITScope& scope);
////void compileDCopyA(int opcode, x86JITScope& scope);
//void compileMOp(int opcode, x86JITScope& scope);

} // _ELENA_

#endif // x86jitcompilerH
