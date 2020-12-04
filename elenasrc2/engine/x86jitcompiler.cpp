//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler Engine
//
//		This file contains ELENA JIT-X linker class.
//		Supported platforms: x86
//                                              (C)2005-2020, by Alexei Rakov
//---------------------------------------------------------------------------

#include "elena.h"
// --------------------------------------------------------------------------
#include "x86jitcompiler.h"
#include "bytecode.h"
#include "core.h"

using namespace _ELENA_;

// --- ELENA CORE built-in routines
constexpr int GC_ALLOC           = 0x10001;
constexpr int HOOK               = 0x10010;
constexpr int INVOKER            = 0x10011;
constexpr int INIT_RND           = 0x10012;
constexpr int ENDFRAME           = 0x10016;
constexpr int CALC_SIZE          = 0x1001F;
constexpr int GET_COUNT          = 0x10020;
constexpr int THREAD_WAIT        = 0x10021;
constexpr int BREAK              = 0x10026;
constexpr int EXPAND_HEAP        = 0x10028;

constexpr int CORE_GC_TABLE      = 0x20002;
constexpr int CORE_STATICROOT    = 0x20005;
constexpr int CORE_TLS_INDEX     = 0x20007;
constexpr int CORE_THREADTABLE   = 0x20008;
constexpr int CORE_MESSAGE_TABLE = 0x2000A;
constexpr int CORE_EH_TABLE      = 0x2000B;
constexpr int SYSTEM_ENV         = 0x2000C;
constexpr int VOID               = 0x2000D;
constexpr int VOIDPTR            = 0x2000E;

// preloaded gc routines
const int coreVariableNumber = 2;
const int coreVariables[coreVariableNumber] =
{
   CORE_GC_TABLE, CORE_EH_TABLE
};

const int coreStaticNumber = 2;
const int coreStatics[coreStaticNumber] =
{
   VOID, VOIDPTR
};

// preloaded env routines
const int envFunctionNumber = 1;
const int envFunctions[envFunctionNumber] =
{
   INVOKER
};

// preloaded gc routines
const int coreFunctionNumber = 9;
const int coreFunctions[coreFunctionNumber] =
{
   BREAK, EXPAND_HEAP, GC_ALLOC, HOOK, INIT_RND, ENDFRAME,
   CALC_SIZE, GET_COUNT,
   THREAD_WAIT
};

// preloaded gc commands
const int gcCommandNumber = 158;
const int gcCommands[gcCommandNumber] =
{
   bcLoadEnv, bcCallExtR, bcSaveSI, bcBSRedirect, bcOpen,
   bcReserve, bcPushSIP, bcStoreSI, bcPeekSI, bcThrow,
   bcCallVI, bcClose, bcNew, bcFillRI, bcCallRM,
   bcPeekFI, bcStoreFI, bcAllocI, bcJumpRM, bcVCallRM,
   bcMTRedirect, bcJumpVI, bcXMTRedirect, bcRestore, bcPushF,
   bcCopyF, bcCopyFI, bcNAddF, bcCopyToF, bcCopyToFI,
   bcNSubF, bcNMulF, bcNDivF, bcPushAI, bcGetI,
   bcSetI, bcCopyToAI, bcCreate, bcFillR, bcXSetI,
   bcXSetFI, bcClass, bcXSaveF, bcLen, bcSave,
   bcSelect, bcNEqual, bcNLess, bcSNop, bcCreateN,
   bcSaveF, bcTryLock, bcLoad, bcHook, bcUnhook,
   bcFlag, bcFreeLock, bcGet, bcNShlF, bcNShrF,
   bcMovN, bcCloneF, bcInc, bcRead, bcExclude,
   bcInclude, bcCopyTo, bcReadToF, bcXWrite, bcDiv,
   bcLoadFI, bcEqual, bcNAndF, bcNOrF, bcNXorF,
   bcCoalesce, bcCoalesceR, bcXSelectR, bcLAddF, bcLSubF,
   bcLMulF, bcLDivF, bcLShlF, bcLShrF, bcLAndF,
   bcLOrF, bcLXorF, bcLEqual, bcLLess, bcSaveI,
   bcLoadI, bcRAddF, bcRSubF, bcRMulF, bcRDivF,
   bcREqual, bcRLess, bcRSet, bcRSave, bcRSaveN,
   bcRIntF, bcRLoad, bcClone, bcAddF, bcSubF,
   bcAddress, bcLoadSI, bcLoadVerb, bcSetV, bcCount,
   bcSet, bcPushVerb, bcPush, bcMQuit, bcStoreV,
   bcRExp, bcRSin, bcRArcTan, bcRLn, bcRRound,
   bcRInt, bcRCos, bcMove, bcMoveTo, bcCopyAI,
   bcXSaveAI, bcXSave, bcSaveFI, bcXLoad, bcXAddF,
   bcMul, bcXOr, bcPeek, bcSwap, bcXCreate,
   bcIfHeap, bcEqualFI, bcLoadF, bcRSaveL, bcRAbs,
   bcCallI, bcIfCount, bcSub, bcSwapD, bcXSet,
   bcMIndex, bcParent, bcCheckSI, bcLSave, bcLCallExtR,
   bcRAddNF, bcRSubNF, bcRMulNF, bcRDivNF, bcXRSaveF,
   bcXRedirect, bcXVRedirect, bcVJumpRM,
};

const int gcCommandExNumber = 54;
const int gcCommandExs[gcCommandExNumber] =
{
   bcMTRedirect + 0x100, bcXMTRedirect + 0x100,
   bcMTRedirect + 0x200, bcXMTRedirect + 0x200,
   bcMTRedirect + 0xC00, bcXMTRedirect + 0xC00,
   bcCreateN + 0x100, bcCreateN + 0x200, bcCreateN + 0x300, bcCreateN + 0x400,
   bcXWrite + 0x100, bcXWrite + 0x200, bcXWrite + 0x300, bcXWrite + 0x400,
   bcReadToF + 0x100, bcReadToF + 0x200, bcReadToF + 0x300, bcReadToF + 0x400,
   bcCopyTo + 0x100, bcCopyTo + 0x200, bcCopyTo + 0x300, bcCopyTo + 0x400,
   bcCopyToF + 0x100, bcCopyToF + 0x200, bcCopyToF + 0x300, bcCopyToF + 0x400,
   bcCopyToFI + 0x100, bcCopyToFI + 0x200, bcCopyToFI + 0x300, bcCopyToFI + 0x400,
   bcCopyFI + 0x100, bcCopyFI + 0x200, bcCopyFI + 0x300, bcCopyFI + 0x400,
   bcCopyF + 0x100, bcCopyF + 0x200, bcCopyF + 0x300, bcCopyF + 0x400,
   bcCopyAI + 0x100, bcCopyAI + 0x200, bcCopyAI + 0x300, bcCopyAI + 0x400,
   bcCopyToAI + 0x100, bcCopyToAI + 0x200, bcCopyToAI + 0x300, bcCopyToAI + 0x400,
   bcMove + 0x100, bcMove + 0x200, bcMove + 0x300, bcMove + 0x400,
   bcMoveTo + 0x100, bcMoveTo + 0x200, bcMoveTo + 0x300, bcMoveTo + 0x400,
};

// command table
void (*commands[0x100])(int opcode, x86JITScope& scope) =
{
   &compileNop, &compileBreakpoint, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp,
   &compileDCopyCount, &loadOneByteOp, &compilePushA, &compilePopA, &compileNop, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp,

   &compileNot, &compileNop, &compileNop, &loadOneByteOp, &loadOneByteOp, &loadOneByteLOp, &loadOneByteOp, &compileQuit,
   &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &compileNop,

   &loadOneByteOp, &loadOneByteOp, &compilePushD, &compilePopD, &compileNop, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp,
   &loadOneByteOp, &compileNop, &loadOneByteOp, &compileNop, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp, &loadOneByteOp,

   &loadOneByteLOp, &loadOneByteLOp, &loadOneByteOp, &loadOneByteLOp, &compileNop, &loadOneByteLOp, &loadOneByteLOp, &loadOneByteOp,
   &compileNop, &compileNop, &compileNop, &compileNop, &compileNop, &loadOneByteLOp, &loadOneByteLOp, &compileNop,

   &loadOneByteLOp, &loadOneByteLOp, &compileNop, &loadOneByteLOp, &loadOneByteLOp, &loadOneByteLOp, &loadOneByteLOp, &loadOneByteLOp,
   &loadOneByteLOp, &loadOneByteLOp, &loadOneByteLOp, &loadOneByteLOp, &compileNop, &compileNop, &compileNop, &loadOneByteOp,

   &loadFOp, &loadFOp, &loadFOp, &loadFOp, &loadFOp, &compileNop, &compileNop, &compileNop,
   &compileNop, &compileNop, &loadNOp, &loadNOp, &loadNOpX, &loadN4OpX, &loadFOp, &loadFOp,

   &loadNOp, &loadIndexOp, &compileXRedirect, &compileXRedirect, &compileNop, &compileNop, &compileNop, &compileNop,
   &compileNop, &compileNop, &compileNop, &compileNop, &compileNop, &compileNop, &compileNop, &compileNop,

   &compileNop, &compileNop, &compileNop, &compileNop, &loadFOp, &loadFOp, &loadFOp, &loadFOp,
   &loadFOp, &loadFOp, &loadFOp, &loadFOp, &compileNop, &loadFOp, &compileNop, &compileNop,

   &loadFOp, &loadFOp, &loadFOp, &loadFPOp, &loadFPOp, &loadFOp, &loadFOp, &loadFOp,
   &loadFOp, &loadFOp, &compileNop, &compileNop, &compileNop, &compileNop, &loadFOp, &compileNop,

   &compileDec, &loadIndexOp, &compileRestore, &compileALoadR, &loadFPOp, &loadIndexOp, &compileIfHeap, &loadIndexOp,
   &compileOpen, &compileQuitN, &loadROp, &loadROp, &compileACopyF, &compileACopyS, &compileSetR, &compileMCopy,

   &compileJump, &loadVMTIndexOp, &loadVMTIndexOp, &compileCallR, &compileJumpN, &loadFunction, &compileHook, &compileHook,
   &loadIndexOp, &compileNop, &compileNotLessE, &compileNotGreaterE, &compileElseD, &compileIfE, &compileElseE, &compileIfCount,

   &compilePush, &loadNOp, &compilePush, &loadFPOp, &loadIndexOp, &loadFOp, &compilePushFI, &loadFPOp,
   &loadIndexOp, &loadFOp, &compilePushSI, &loadIndexOp, &loadFPOp, &compilePushF, &loadFPOp, &loadNOp,

   &loadIndexOp, &compileNop, &compilePushF, &loadIndexOp, &loadFPOp, &loadFOp, &loadFOp, &compileNop,
   &loadFOp, &loadFOp, &loadIndexOp, &loadIndexOp, &compileASaveR, &loadFunction, &loadFOp, &loadNOp,

   &compilePopN, &compileAllocI, &loadROp, &compileMovV, &compileDShiftN, &compileDAndN, &loadNOp, &compileDOrN,
   &loadROp, &compileDShiftN, &loadNOp, &compileInvokeVMTOffset, &loadIndexNOp, &loadIndexN4OpX, &loadNNOpX, &loadNNOpX,

   &loadFN4OpX, &compileDynamicCreateN, &loadFPIndexOp, &loadIndexN4OpX, &loadFPN4OpX, &loadFN4OpX, &loadFPN4OpX, &loadFN4OpX,
   &compileMTRedirect, &compileMTRedirect, &compileGreaterN, &compileGreaterN, &compileLessN, &loadFNOp, &loadFNOp, &loadFNOp,

   &compileCreate, &compileCreateN, &compileFill, &compileSelectR, &compileInvokeVMTOffset, &compileInvokeVMT, &compileSelectR, &compileLessN,
   &compileNop, &compileNop, &compileIfR, &compileElseR, &compileIfN, &compileElseN, &compileInvokeVMT, &compileNop,
};

// --- x86JITCompiler commands ---

inline void compileJump(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jmp   lbEnding
   if (!forwardJump) {
      scope.lh.writeJmpBack(label);
   }
   else {
      // if it is forward jump, try to predict if it is short
      if (shortJump) {
         scope.lh.writeShortJmpForward(label);
      }
      else scope.lh.writeJmpForward(label);
   }
}

inline void compileJumpX(x86JITScope& scope, int label, bool forwardJump, bool shortJump, x86Helper::x86JumpType prefix)
{
   if (!forwardJump) {
      scope.lh.writeJxxBack(prefix, label);
   }
   else {
      // if it is forward jump, try to predict if it is short
      if (shortJump) {
         scope.lh.writeShortJxxForward(label, prefix);
      }
      else scope.lh.writeJxxForward(label, prefix);
   }
}

inline void compileJumpIf(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jnz   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JNZ);
}

inline void compileJumpIfNot(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jz   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JZ);
}

//inline void compileJumpAbove(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
//{
//   // ja   lbEnding
//   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JA);
//}
//
//inline void compileJumpBelow(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
//{
//   // jb   lbEnding
//   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JB);
//}

inline void compileJumpLess(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jl   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JL);
}

inline void compileJumpNotLess(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jnl   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JGE);
}

inline void compileJumpGreater(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jl   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JG);
}

inline void compileJumpNotGreater(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jl   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JLE);
}

inline void compileJumpLessOrEqual(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jle   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JLE);
}

inline void compileJumpGreaterOrEqual(x86JITScope& scope, int label, bool forwardJump, bool shortJump)
{
   // jge   lbEnding
   compileJumpX(scope, label, forwardJump, shortJump, x86Helper::JUMP_TYPE_JGE);
}

void _ELENA_::loadCoreOp(x86JITScope& scope, char* code)
{
   MemoryWriter* writer = scope.code;

   if (code==NULL)
      throw InternalError("Cannot load core command");

   pos_t position = writer->Position();
   size_t length = *(size_t*)(code - 4);

   writer->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   int key, offset;
   while (count > 0) {
      key = relocation[0];
      offset = relocation[1];

      // locate relocation position
      writer->seek(position + offset);

      if ((key & mskTypeMask) == mskPreloaded) {
         scope.compiler->writeCoreReference(scope, key, position, offset, code);
      }
      else {
         scope.helper->writeReference(*writer, key, *(int*)(code + offset), scope.module);
      }

      relocation += 2;
      count--;
   }
   writer->seekEOF();
}

inline void _ELENA_::writeCoreReference(x86JITScope& scope, ref_t reference, int position, int offset, char* code)
{
   // references should be already preloaded
   if ((reference & mskAnyRef) == mskPreloadRelCodeRef) {
      scope.helper->writeReference(*scope.code,
         scope.compiler->_preloaded.get(reference & ~mskAnyRef), true, *(int*)(code + offset));

      scope.lh.addFixableJump(offset + position, (*scope.code->Memory())[offset + position]);
   }
   else scope.helper->writeReference(*scope.code,
      scope.compiler->_preloaded.get(reference & ~mskAnyRef), false, *(int*)(code + offset));
}

void _ELENA_::loadOneByteOp(int opcode, x86JITScope& scope)
{
   MemoryWriter* writer = scope.code;

   char* code = (char*)scope.compiler->_inlines[opcode];
   size_t position = writer->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   writer->write(code, *(int*)(code - 4));

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   int key, offset;
   while (count > 0) {
      key = relocation[0];
      offset = relocation[1];

      // locate relocation position
      writer->seek(position + relocation[1]);

      if ((key & mskTypeMask) == mskPreloaded) {
         scope.compiler->writeCoreReference(scope, key, position, offset, code);
      }
      else scope.writeReference(*writer, key, *(int*)(code + offset));

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadNOp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         scope.code->writeDWord(scope.argument);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(scope.argument << 2);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

inline int getFPOffset(int argument, int argOffset)
{
   return -(argument - (argument < 0 ? argOffset : 0));
}

void _ELENA_::loadFNOp(int opcode, x86JITScope& scope)
{
   int arg2 = scope.tape->getDWord();

   char* code = (char*)scope.compiler->_inlines[opcode];
   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(getFPOffset(scope.argument, 8));
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFPN4OpX(int opcode, x86JITScope& scope, int prefix)
{
   int arg2 = scope.tape->getDWord();
   char* code = nullptr;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(getFPOffset(scope.argument << 2, scope.frameOffset));
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFN4OpX(int opcode, x86JITScope& scope, int prefix)
{
   int arg2 = scope.tape->getDWord();
   char* code = nullptr;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(getFPOffset(scope.argument, 8));
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}
void _ELENA_::loadFPIndexOpX(int opcode, x86JITScope& scope, int prefix)
{
   char* code = nullptr;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   int arg2 = scope.tape->getDWord();

   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(getFPOffset(scope.argument << 2, scope.frameOffset));
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2 << 2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFPIndexOpX(int opcode, x86JITScope& scope)
{
   pos_t pos = scope.tape->Position();
   int arg2 = scope.tape->getDWord();
   scope.tape->seek(pos);

   switch (arg2) {
      case 1:
         loadFPIndexOpX(opcode, scope, 0x100);
         break;
      case 2:
         loadFPIndexOpX(opcode, scope, 0x200);
         break;
      case 4:
         loadFPIndexOpX(opcode, scope, 0x300);
         break;
      case 8:
         loadFPIndexOpX(opcode, scope, 0x400);
         break;
      default:
         loadFPIndexOp(opcode, scope);
         break;
   }
}

void _ELENA_::loadFN4OpX(int opcode, x86JITScope& scope)
{
   pos_t pos = scope.tape->Position();
   int arg2 = scope.tape->getDWord();
   scope.tape->seek(pos);

   switch (arg2) {
      case 1:
         loadFN4OpX(opcode, scope, 0x100);
         break;
      case 2:
         loadFN4OpX(opcode, scope, 0x200);
         break;
      case 3:
         loadFN4OpX(opcode, scope, 0x300);
         break;
      case 4:
         loadFN4OpX(opcode, scope, 0x400);
         break;
      default:
         loadFNOp(opcode, scope);
         break;
   }
}

void _ELENA_::loadFPN4OpX(int opcode, x86JITScope& scope)
{
   pos_t pos = scope.tape->Position();
   int arg2 = scope.tape->getDWord();
   scope.tape->seek(pos);

   switch (arg2) {
   case 1:
      loadFPN4OpX(opcode, scope, 0x100);
      break;
   case 2:
      loadFPN4OpX(opcode, scope, 0x200);
      break;
   case 3:
      loadFPN4OpX(opcode, scope, 0x300);
      break;
   case 4:
      loadFN4OpX(opcode, scope, 0x400);
      break;
   default:
      loadFNOp(opcode, scope);
      break;
   }
}

void _ELENA_::loadFPIndexOp(int opcode, x86JITScope& scope)
{
   int arg2 = scope.tape->getDWord();

   char* code = (char*)scope.compiler->_inlines[opcode];
   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(-(scope.argument << 2));
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2 << 2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadIndexNOp(int opcode, x86JITScope& scope)
{
   int arg2 = scope.tape->getDWord();

   char* code = (char*)scope.compiler->_inlines[opcode];
   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(scope.argument << 2);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadIndexN4OpX(int opcode, x86JITScope& scope, int prefix)
{
   int arg2 = scope.tape->getDWord();
   char* code = NULL;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(scope.argument << 2);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadIndexN4OpX(int opcode, x86JITScope& scope)
{
   pos_t pos = scope.tape->Position();
   int arg2 = scope.tape->getDWord();
   scope.tape->seek(pos);

   switch (arg2) {
      case 1:
         loadIndexN4OpX(opcode, scope, 0x100);
         break;
      case 2:
         loadIndexN4OpX(opcode, scope, 0x200);
         break;
      case 3:
         loadIndexN4OpX(opcode, scope, 0x300);
         break;
      case 4:
         loadIndexN4OpX(opcode, scope, 0x400);
         break;
      default:
         loadIndexNOp(opcode, scope);
         break;
   }
}

void _ELENA_::loadNNOp(int opcode, x86JITScope& scope)
{
   int arg2 = scope.tape->getDWord();

   char* code = (char*)scope.compiler->_inlines[opcode];
   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(scope.argument);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadNNOpX(int opcode, x86JITScope& scope, int prefix)
{
   int arg2 = scope.tape->getDWord();

   char* code = NULL;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   size_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(scope.argument);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(arg2);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadNNOpX(int opcode, x86JITScope& scope)
{
   pos_t pos = scope.tape->Position();
   int arg2 = scope.tape->getDWord();
   scope.tape->seek(pos);

   switch (arg2) {
      case 1:
         loadNNOpX(opcode, scope, 0x100);
         break;
      case 2:
         loadNNOpX(opcode, scope, 0x200);
         break;
      case 4:
         loadNNOpX(opcode, scope, 0x300);
         break;
      case 8:
         loadNNOpX(opcode, scope, 0x400);
         break;
      default:
         loadNNOp(opcode, scope);
         break;
   }
}

void _ELENA_::loadOneByteLOp(int opcode, x86JITScope& scope)
{
   char* code = (char*)scope.compiler->_inlines[opcode];

   // simply copy correspondent inline code
   scope.code->write(code, *(int*)(code - 4));
}

void _ELENA_::loadROp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         if (scope.argument == 0) {
            scope.code->writeDWord(0);
         }
         else scope.writeReference(*scope.code, scope.argument, 0);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadMTOp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.writeReference(*scope.code, scope.argument, 0);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(scope.extra_arg);
      }
      else if (relocation[0] == -3) {
         scope.code->writeDWord(scope.extra_arg2);
      }
      else if (relocation[0] == (CORE_MESSAGE_TABLE | mskPreloadDataRef)) {
         scope.helper->writeMTReference(*scope.code);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadMTOpX(int opcode, x86JITScope& scope, int prefix)
{
   char* code = NULL;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.writeReference(*scope.code, scope.argument, 0);
      }
      else if (relocation[0] == -2) {
         scope.code->writeDWord(scope.extra_arg);
      }
      else if (relocation[0] == -3) {
         scope.code->writeDWord(scope.extra_arg2);
      }
      else if (relocation[0] == (CORE_MESSAGE_TABLE | mskPreloadDataRef)) {
         scope.helper->writeMTReference(*scope.code);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadNOpX(int opcode, x86JITScope& scope, int prefix)
{
   char* code = nullptr;
   for (int i = 0; i < gcCommandExNumber; i++) {
      if (gcCommandExs[i] == opcode + prefix) {
         code = (char*)scope.compiler->_inlineExs[i];
         break;
      }
   }

   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.writeReference(*scope.code, scope.argument, 0);
      }
      else if (relocation[0] == (CORE_MESSAGE_TABLE | mskPreloadDataRef)) {
         scope.helper->writeMTReference(*scope.code);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadIndexOp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         scope.code->writeDWord(scope.argument << 2);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadVMTIndexOp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         scope.code->writeDWord((scope.argument << 3) + 4);
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFPOp(int opcode, x86JITScope& scope)
{
   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         scope.code->writeDWord(getFPOffset(scope.argument << 2, scope.frameOffset));
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFOp(int opcode, x86JITScope& scope)
{
   char* code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0] == -1) {
         scope.code->writeDWord(getFPOffset(scope.argument, 8));
      }
      else writeCoreReference(scope, relocation[0], position, relocation[1], code);

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::loadFunction(int opcode, x86JITScope& scope)
{
   // if it is internal native call
   switch (scope.argument & mskAnyRef) {
      case mskNativeCodeRef:
      case mskPreloadCodeRef:
         compileCallR(opcode, scope);
         return;
   }

   MemoryWriter* writer = scope.code;

   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   writer->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   int key, offset;
   while (count > 0) {
      key = relocation[0];
      offset = relocation[1];

      // locate relocation position
      writer->seek(position + relocation[1]);

      // !! temporal, more optimal way should be used
      if (relocation[0]==-1) {
         scope.writeReference(*writer, scope.argument, *(int*)(code + offset));
      }
      //else if ((key & mskTypeMask) == mskPreloaded) {
      //   scope.compiler->writePreloadedReference(scope, key, position, offset, code);
      //}
      else scope.writeReference(*writer, key, *(int*)(code + offset));

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::compileNop(int, x86JITScope& scope)
{
   // nop command is used to indicate possible label
   // fix the label if it exists
   if (scope.lh.checkLabel(scope.tape->Position() - 1)) {
      scope.lh.fixLabel(scope.tape->Position() - 1);
   }
   // or add the label
   else scope.lh.setLabel(scope.tape->Position() - 1);
}

void _ELENA_::compileBreakpoint(int, x86JITScope& scope)
{
   if (scope.withDebugInfo)
      scope.helper->addBreakpoint(scope.code->Position());
}

void _ELENA_::compilePush(int opcode, x86JITScope& scope)
{
   // push constant | reference
   scope.code->writeByte(0x68);
   if (opcode == bcPushR && scope.argument != 0) {
      scope.writeReference(*scope.code, scope.argument, 0);
   }
   else scope.code->writeDWord(scope.argument);
}

void _ELENA_::compilePopD(int, x86JITScope& scope)
{
   // pop edx
   scope.code->writeByte(0x5A);
}

void _ELENA_::compilePushSI(int, x86JITScope& scope)
{
   // push [esp+offset]
   scope.code->writeWord(0xB4FF);
   scope.code->writeByte(0x24);
   scope.code->writeDWord(scope.argument << 2);
}

void _ELENA_::compileJump(int, x86JITScope& scope)
{
   ::compileJump(scope, scope.tape->Position() + scope.argument, (scope.argument > 0), (__abs(scope.argument) < 0x10));
}

void _ELENA_::compileHook(int opcode, x86JITScope& scope)
{
   if (scope.argument < 0) {
      scope.lh.writeLoadBack(scope.tape->Position() + scope.argument);
   }
   else scope.lh.writeLoadForward(scope.tape->Position() + scope.argument);

   loadOneByteOp(opcode, scope);
}

void _ELENA_::compileOpen(int opcode, x86JITScope& scope)
{
   loadOneByteLOp(opcode, scope);

   // include current frame and FrameHeader if required
   if (scope.argument) {
      scope.frameOffset = (scope.argument << 2) + 12;
   }
   else scope.frameOffset = 4;
}

void _ELENA_::compileQuit(int, x86JITScope& scope)
{
   scope.code->writeByte(0xC3);
}

void _ELENA_::compileQuitN(int, x86JITScope& scope)
{
   scope.code->writeByte(0xC2);
   scope.code->writeWord((unsigned short)(scope.argument << 2));
}

//void _ELENA_::compileNext(int opcode, x86JITScope& scope)
//{
//   int jumpOffset = scope.argument;
//
//  // test upper boundary
//   loadOneByteLOp(opcode, scope);
//
//  // try to use short jump if offset small (< 0x10?)
//   compileJumpLess(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
//}

void _ELENA_::compileIfE(int, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // cmp dword ptr[ebx], edx
   scope.code->writeWord(0x1339);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

void _ELENA_ :: compileIfCount(int opcode, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   loadOneByteOp(opcode, scope);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

void _ELENA_::compileElseE(int, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // cmp dwotd ptr[ebx], edx
   scope.code->writeWord(0x1339);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpIf(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

//void _ELENA_::compileLessE(int, x86JITScope& scope)
//{
//   int jumpOffset = scope.argument;
//
//   // cmp ebx, ecx
//   scope.code->writeWord(0xD93B);
//
//   // try to use short jump if offset small (< 0x10?)
//   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
//   compileJumpLess(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
//}

void _ELENA_::compileNotLessE(int, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // cmp dwotd ptr[ebx], edx
   scope.code->writeWord(0x1339);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpGreaterOrEqual(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

void _ELENA_::compileNotGreaterE(int, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // cmp dwotd ptr[ebx], edx
   scope.code->writeWord(0x1339);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpLessOrEqual(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

//void _ELENA_::compileIfB(int, x86JITScope& scope)
//{
//   int jumpOffset = scope.argument;
//
//   // cmp eax, edi
//   scope.code->writeWord(0xC73B);
//
//   // try to use short jump if offset small (< 0x10?)
//   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
//   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
//}

void _ELENA_::compileElseD(int, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // cmp edx, [esp]
   scope.code->writeByte(0x3B);
   scope.code->writeWord(0x2414);

   // try to use short jump if offset small (< 0x10?)
   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpIf(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

//void _ELENA_::compileIfM(int, x86JITScope& scope)
//{
//   int jumpOffset = scope.tape->getDWord();
//   int message = scope.resolveMessage(scope.argument);
//
//   // cmp ecx, message
//   scope.code->writeWord(0xF981);
//   scope.code->writeDWord(message);
//
//   // try to use short jump if offset small (< 0x10?)
//   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
//   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
//}
//
//void _ELENA_::compileElseM(int, x86JITScope& scope)
//{
//   int jumpOffset = scope.tape->getDWord();
//   int message = scope.resolveMessage(scope.argument);
//
//   // cmp ecx, message
//   scope.code->writeWord(0xF981);
//   scope.code->writeDWord(message);
//
//  // try to use short jump if offset small (< 0x10?)
//   //NOTE: due to compileJumpX implementation - compileJumpIf is called
//   compileJumpIf(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
//}

void _ELENA_::compileIfHeap(int opcode, x86JITScope& scope)
{
   int jumpOffset = scope.argument;

   // load bottom boundary
   loadOneByteOp(opcode, scope);

   // jz short label
   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (__abs(jumpOffset) < 0x10));
}

void _ELENA_::compileFill(int opcode, x86JITScope& scope)
{
   // HOT FIX : reverse the argument order
   ref_t r = scope.argument;
   scope.argument = scope.tape->getDWord();

   if (r == 0) {
      // xor eax, eax
      scope.code->writeWord(0xC033);
   }
   else {
      // mov eax, r
      scope.code->writeByte(0xB8);
      scope.writeReference(*scope.code, r, 0);
   }

   if (scope.argument == 1) {
      // mov [ebx], eax
      scope.code->writeWord(0x0389);
   }
   else if (scope.argument == 2) {
      // mov [ebx], eax
      scope.code->writeWord(0x0389);
      // mov [ebx+4], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(4);
   }
   else if (scope.argument == 3) {
      // mov [ebx], eax
      scope.code->writeWord(0x0389);
      // mov [ebx+4], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(4);
      // mov [ebx+8], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(8);
   }
   else if (scope.argument == 4) {
      // mov [ebx], eax
      scope.code->writeWord(0x0389);
      // mov [ebx+4], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(4);
      // mov [ebx+8], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(8);
      // mov [ebx+12], eax
      scope.code->writeWord(0x4389);
      scope.code->writeByte(8);
   }
   else loadNOp(opcode, scope);
}

void _ELENA_::compileCreate(int opcode, x86JITScope& scope)
{
   // HOT FIX : reverse the argument order
   ref_t vmtRef = scope.argument;
   scope.argument = scope.tape->getDWord();

   scope.argument <<= 2;
   int length = scope.argument;

   // __arg1 = #gc_page + (length - 1)
   scope.argument = align(scope.argument + scope.objectSize, gcPageSize32);

   loadNOp(opcode, scope);

   // NOTE : empty length should be equal to 800000h
   // due to current GC algorithm
   if (length == 0)
      length = 0x800000;

   // mov [ebx-elPageSizeOffset], length
   scope.code->writeWord(0x43C7);
   scope.code->writeByte((unsigned char)-elPageSizeOffset32);
   scope.code->writeDWord(length);

   if (vmtRef) {
      // ; set vmt reference
      // mov [ebx-elPageVMTOffset], vmt
      scope.code->writeWord(0x43C7);
      scope.code->writeByte((unsigned char)-elPageVMTOffset32);
      scope.writeReference(*scope.code, vmtRef, 0);
   }
}

void _ELENA_::compileCreateN(int, x86JITScope& scope)
{
   // HOT FIX : reverse the argument order
   ref_t vmtRef = scope.argument;
   scope.argument = scope.tape->getDWord();

   int length = scope.argument | 0x800000; // mark object as a binary structure

   // __arg1 = #gc_page + (length - 1)
   scope.argument = align(scope.argument + scope.objectSize, gcPageSize32);

   loadNOp(bcNew, scope);

   // mov [ebx-elPageSizeOffset], length
   scope.code->writeWord(0x43C7);
   scope.code->writeByte((unsigned char)-elPageSizeOffset32);
   scope.code->writeDWord(length);

   if (vmtRef) {
      // ; set vmt reference
      // mov [ebx-elPageVMTOffset], vmt
      scope.code->writeWord(0x43C7);
      scope.code->writeByte((unsigned char)-elPageVMTOffset32);
      scope.writeReference(*scope.code, vmtRef, 0);
   }
}

void _ELENA_::loadNOpX(int opcode, x86JITScope& scope)
{
   switch (scope.argument) {
      case 1:
         loadNOpX(opcode, scope, 0x100);
         break;
      case 2:
         loadNOpX(opcode, scope, 0x200);
         break;
      case 4:
         loadNOpX(opcode, scope, 0x300);
         break;
      case 8:
         loadNOpX(opcode, scope, 0x400);
         break;
      default:
         loadNOp(opcode, scope);
         break;
   }
}

void _ELENA_::loadN4OpX(int opcode, x86JITScope& scope)
{
   switch (scope.argument) {
      case 1:
         loadNOpX(opcode, scope, 0x100);
         break;
      case 2:
         loadNOpX(opcode, scope, 0x200);
         break;
      case 3:
         loadNOpX(opcode, scope, 0x300);
         break;
      case 4:
         loadNOpX(opcode, scope, 0x400);
         break;
      default:
         loadNOp(opcode, scope);
         break;
   }
}

void _ELENA_::compileDynamicCreateN(int opcode, x86JITScope& scope)
{
   // HOT FIX : reverse the argument order
   ref_t vmtRef = scope.argument;
   scope.argument = scope.tape->getDWord(); // item size

   switch (scope.argument) {
      case 1:
         loadNOpX(opcode, scope, 0x100);
         break;
      case 2:
         loadNOpX(opcode, scope, 0x200);
         break;
      case 4:
         loadNOpX(opcode, scope, 0x300);
         break;
      case 8:
         loadNOpX(opcode, scope, 0x400);
         break;
      default:
         loadNOp(opcode, scope);
         break;
   }

   if (vmtRef) {
      // ; set vmt reference
      // mov [ebx-elVMT], vmt
      scope.code->writeWord(0x43C7);
      scope.code->writeByte((unsigned char)(-elPageVMTOffset32));
      scope.writeReference(*scope.code, vmtRef, 0);
   }
}

void _ELENA_::compileSelectR(int opcode, x86JITScope& scope)
{
   // HOT FIX : reverse the argument order
   ref_t r1 = scope.argument;
   scope.argument = scope.tape->getDWord();

   if (opcode == bcSelect) {
      // mov ebx, r1
      scope.code->writeByte(0xBB);
      scope.writeReference(*scope.code, r1, 0);
   }
   else {
      // mov  eax, r1
      scope.code->writeByte(0xB8);
      scope.writeReference(*scope.code, r1, 0);
   }

   loadROp(opcode, scope);
}

void _ELENA_::compileSetR(int, x86JITScope& scope)
{
   // mov ebx, r
   scope.code->writeByte(0xBB);
   if (scope.argument == 0) {
      scope.code->writeDWord(0);
   }
   else if (scope.argument == -1) {
      scope.code->writeDWord((pos_t)-1);
   }
   else scope.writeReference(*scope.code, scope.argument, 0);
}

void _ELENA_::compileDAndN(int, x86JITScope& scope)
{
   // and edx, mask
   scope.code->writeWord(0xE281);
   scope.code->writeDWord(scope.argument);
}

void _ELENA_::compileDOrN(int, x86JITScope& scope)
{
   // or edx, mask
   scope.code->writeWord(0xCA81);
   scope.code->writeDWord(scope.argument);
}

void _ELENA_::compileDec(int, x86JITScope& scope)
{
   // sub edx, n
   scope.code->writeWord(0xEA81);
   scope.code->writeDWord(scope.argument);
}

void _ELENA_::compileDCopyCount(int, x86JITScope& scope)
{
   // and edx, ARG_MASK
   scope.code->writeWord(0xE281);
   scope.code->writeDWord(ARG_MASK);
}

void _ELENA_::compileALoadR(int, x86JITScope& scope)
{
   // mov ebx, [r]
   scope.code->writeWord(0x1D8B);
   scope.writeReference(*scope.code, scope.argument, 0);
}

void _ELENA_::compilePushA(int, x86JITScope& scope)
{
   // push ebx
   scope.code->writeByte(0x53);
}

void _ELENA_::compilePushFI(int, x86JITScope& scope)
{
   scope.code->writeWord(0xB5FF);
   // push [ebp-level*4]
   scope.code->writeDWord(getFPOffset(scope.argument << 2, scope.frameOffset));
}

void _ELENA_:: compilePushF(int op, x86JITScope& scope)
{
   scope.argument = getFPOffset(scope.argument, op == bcPushFIP ? scope.frameOffset : 8);

   loadNOp(bcPushF, scope);
}

void _ELENA_::compilePushD(int, x86JITScope& scope)
{
   // push edx
   scope.code->writeByte(0x52);
}

void _ELENA_::compileCallR(int, x86JITScope& scope)
{
   // call symbol
   scope.code->writeByte(0xE8);
   scope.writeReference(*scope.code, scope.argument | mskRelCodeRef, 0);
}

void _ELENA_::compileJumpN(int, x86JITScope& scope)
{
   // jmp [ebx+i]
   scope.code->writeWord(0x63FF);
   scope.code->writeByte(scope.argument << 2);
}

void _ELENA_::compilePopA(int, x86JITScope& scope)
{
   // pop ebx
   scope.code->writeByte(0x5B);
}

void _ELENA_::compileMCopy(int, x86JITScope& scope)
{
   // mov edx, message
   scope.code->writeByte(0xBA);
   scope.code->writeDWord(scope.resolveMessage(scope.argument));
}

void _ELENA_::compilePopN(int, x86JITScope& scope)
{
   // add esp, arg
   int arg = scope.argument << 2;
   if (arg < 0x80) {
      scope.code->writeWord(0xC483);
      scope.code->writeByte(scope.argument << 2);
   }
   else {
      scope.code->writeWord(0xC481);
      scope.code->writeDWord(scope.argument << 2);
   }
}

void _ELENA_ :: compileAllocI(int opcode, x86JITScope& scope)
{
   // sub esp, __arg1 * 4
   int arg = scope.argument << 2;
   if (arg < 0x80) {
      scope.code->writeWord(0xEC83);
      scope.code->writeByte(scope.argument << 2);
   }
   else {
      scope.code->writeWord(0xEC81);
      scope.code->writeDWord(scope.argument << 2);
   }

   loadNOp(opcode, scope);
}

void _ELENA_::compileASaveR(int, x86JITScope& scope)
{
   // mov [ref], ebx

   scope.code->writeWord(0x1D89);
   scope.writeReference(*scope.code, scope.argument, 0);
}

void _ELENA_::compileInvokeVMTOffset(int opcode, x86JITScope& scope)
{
   mssg_t message = scope.resolveMessage(scope.tape->getDWord());

   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         // resolve message offset
         scope.writeReference(*scope.code, scope.argument | mskVMTEntryOffset, message);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::compileInvokeVMT(int opcode, x86JITScope& scope)
{
   mssg_t message = scope.resolveMessage(scope.tape->getDWord());

   char*  code = (char*)scope.compiler->_inlines[opcode];
   pos_t position = scope.code->Position();
   size_t length = *(size_t*)(code - 4);

   // simply copy correspondent inline code
   scope.code->write(code, length);

   // resolve section references
   int count = *(int*)(code + length);
   int* relocation = (int*)(code + length + 4);
   while (count > 0) {
      // locate relocation position
      scope.code->seek(position + relocation[1]);

      if (relocation[0]==-1) {
         // resolve message offset
         scope.writeReference(*scope.code, scope.argument | mskVMTMethodAddress, message);
      }

      relocation += 2;
      count--;
   }
   scope.code->seekEOF();
}

void _ELENA_::compileACopyS(int, x86JITScope& scope)
{
   // lea ebx, [esp + index]
   x86Helper::leaRM32disp(
      scope.code, x86Helper::otEBX, x86Helper::otESP, scope.argument << 2);
}

void _ELENA_::compileIfR(int, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp ebx, r
   // jz lab

   scope.code->writeWord(0xFB81);
   // HOTFIX : support zero references
   if (scope.argument == 0) {
      scope.code->writeDWord(0);
   }
   else if (scope.argument == -1) {
      scope.code->writeDWord((pos_t)-1);
   }
   else scope.writeReference(*scope.code, scope.argument, 0);
   //NOTE: due to compileJumpX implementation - compileJumpIf is called
   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileElseR(int, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp ebx, r
   // jz lab

   scope.code->writeWord(0xFB81);
   // HOTFIX : support zero references
   if (scope.argument == -1) {
      scope.code->writeDWord((pos_t)-1);
   }
   else if (scope.argument == 0) {
      scope.code->writeDWord(0);
   }
   else scope.writeReference(*scope.code, scope.argument, 0);

   //NOTE: due to compileJumpX implementation - compileJumpIfNot is called
   compileJumpIf(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileIfN(int, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp edx, n
   // jz lab

   scope.code->writeWord(0xFA81);
   scope.code->writeDWord(scope.argument);
   compileJumpIfNot(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileElseN(int, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp edx, n
   // jnz lab

   scope.code->writeWord(0xFA81);
   scope.code->writeDWord(scope.argument);
   compileJumpIf(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileLessN(int op, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp edx, n
   // jz lab
   scope.code->writeWord(0xFA81);
   scope.code->writeDWord(scope.argument);
   if (op == bcLessN) {
      compileJumpLess(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
   }
   else compileJumpNotLess(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileGreaterN(int op, x86JITScope& scope)
{
   int jumpOffset = scope.tape->getDWord();

   // cmp edx, n
   // jz lab
   scope.code->writeWord(0xFA81);
   scope.code->writeDWord(scope.argument);
   if (op == bcGreaterN) {
      compileJumpGreater(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
   }
   else compileJumpNotGreater(scope, scope.tape->Position() + jumpOffset, (jumpOffset > 0), (jumpOffset < 0x10));
}

void _ELENA_::compileMTRedirect(int op, x86JITScope& scope)
{
   scope.extra_arg = scope.tape->getDWord();
   scope.extra_arg2 = getArgCount(scope.extra_arg);

   int startArg = 1;
   if (test(scope.extra_arg, FUNCTION_MESSAGE)) {
      startArg = 0;
      scope.extra_arg2++;
   }
//   else scope.extra_arg = 4;

   // ; lea  eax, [esp + offs]
   x86Helper::leaRM32disp(scope.code, x86Helper::otEAX, x86Helper::otESP, startArg << 2);

   if (test(scope.extra_arg, VARIADIC_MESSAGE)) {
      loadMTOpX(op, scope, 0xC00);
   }
   else {
      switch (getArgCount(scope.extra_arg) - startArg) {
         case 1:
            loadMTOpX(op, scope, 0x100);
            break;
         case 2:
            loadMTOpX(op, scope, 0x200);
            break;
         default:
            loadMTOp(op, scope);
            break;
      }
   }
}

void _ELENA_::compileXRedirect(int op, x86JITScope& scope)
{
   // should be always used for a function message
   int startArg = 0;

      // ; lea  eax, [esp + offs]
   x86Helper::leaRM32disp(scope.code, x86Helper::otEAX, x86Helper::otESP, startArg << 2);

   // HOTFIX : to adjust the command for loadMTOp
   scope.extra_arg = scope.argument << 2;

   loadMTOp(op, scope);
}

void _ELENA_::compileMovV(int, x86JITScope& scope)
{
   // and edx, ARG_MASK | ACTION_MASK
   // or  edx, m
   scope.code->writeWord(0xE281);
   scope.code->writeDWord(ARG_MASK | ACTION_MASK);
   scope.code->writeWord(0xCA81);
   scope.code->writeDWord(scope.resolveMessage(encodeAction(scope.argument)));
}

void _ELENA_::compileACopyF(int op, x86JITScope& scope)
{
   // lea ebx, [ebp+nn]
   scope.code->writeWord(0x9D8D);
   scope.code->writeDWord(getFPOffset(scope.argument, op == bcMovFIP ? scope.frameOffset : 8));
}

void _ELENA_ :: compileNot(int, x86JITScope& scope)
{
   // not edx
   scope.code->writeWord(0xD2F7);
}

void _ELENA_ :: compileDShiftN(int op, x86JITScope& scope)
{
   if (op == bcShl) {
      // shl edx, n
      scope.code->writeWord(0xE2C1);
      scope.code->writeByte((unsigned char)scope.argument);
   }
   else {
      // shr edx, n
      scope.code->writeWord(0xEAC1);
      scope.code->writeByte((unsigned char)scope.argument);
   }
}

void _ELENA_ :: compileRestore(int op, x86JITScope& scope)
{
   scope.argument += 8; // include EIP & EBP 
   loadNOp(op, scope);
}

// --- x86JITScope ---

x86JITScope :: x86JITScope(MemoryReader* tape, MemoryWriter* code, _ReferenceHelper* helper, x86JITCompiler* compiler)
   : lh(code)
{
   this->tape = tape;
   this->code = code;
   this->helper = helper;
   this->compiler = compiler;
   this->withDebugInfo = compiler->isWithDebugInfo();
   this->objectSize = helper ? helper->getLinkerConstant(lnObjectSize) : 0;
   this->module = NULL;
   this->argument = 0;
   this->extra_arg = 0;
   this->extra_arg2 = 0;

   this->frameOffset = 0;
}

void x86JITScope :: writeReference(MemoryWriter& writer, ref_t reference, size_t disp)
{
   // HOTFIX : mskLockVariable used to fool trylock opcode, adding virtual offset
   if ((reference & mskAnyRef) == mskLockVariable) {
      disp += compiler->getObjectHeaderSize();
   }

   helper->writeReference(writer, reference, disp, module);
}

// --- x86JITCompiler ---

x86JITCompiler :: x86JITCompiler(bool debugMode)
   : _inlineExs(NULL, gcCommandExNumber << 2)
{
   _debugMode = debugMode;
}

size_t x86JITCompiler :: getObjectHeaderSize() const
{
   return elObjectOffset32;
}

bool x86JITCompiler :: isWithDebugInfo() const
{
   // in the current implementation, debug info (i.e. debug section)
   // is always generated (to be used by RTManager)
   return true;
}

void x86JITCompiler :: alignCode(MemoryWriter* writer, int alignment, bool code)
{
   writer->align(alignment, code ? 0x90 : 0x00);
}

void x86JITCompiler :: writeCoreReference(x86JITScope& scope, ref_t reference, int position, int offset, char* code)
{
   if (!_preloaded.exist(reference& ~mskAnyRef)) {
      MemoryWriter writer(scope.code->Memory());

      _preloaded.add(reference & ~mskAnyRef, scope.helper->getVAddress(writer, mskCodeRef));

      // due to optimization section must be ROModule::ROSection instance
      SectionInfo info = scope.helper->getCoreSection(reference & ~mskAnyRef);
      // separate scope should be used to prevent overlapping
      x86JITScope newScope(NULL, &writer, scope.helper, this);
      newScope.module = info.module;

      loadCoreOp(newScope, info.section ? (char*)info.section->get(0) : NULL);
   }
   _ELENA_::writeCoreReference(scope, reference, position, offset, code);
}

inline void loadCoreData(_ReferenceHelper& helper, x86JITScope& dataScope, ref_t reference)
{
   // due to optimization section must be ROModule::ROSection instance
   SectionInfo info = helper.getCoreSection(reference);
   dataScope.module = info.module;

   loadCoreOp(dataScope, info.section ? (char*)info.section->get(0) : NULL);
}

inline void loadRoutines(int functionNumber, const int* functions, x86JITScope& scope,
   IntFixedMap<vaddr_t>& preloaded)
{
   for (int i = 0; i < functionNumber; i++) {
      if (!preloaded.exist(functions[i])) {

         preloaded.add(functions[i], scope.helper->getVAddress(*scope.code, mskCodeRef));

         // due to optimization section must be ROModule::ROSection instance
         SectionInfo info = scope.helper->getCoreSection(functions[i]);
         scope.module = info.module;

         loadCoreOp(scope, info.section ? (char*)info.section->get(0) : NULL);
      }
   }
}

void x86JITCompiler :: prepareCore(_ReferenceHelper& helper, _JITLoader* loader)
{
   // preload core data
   _Memory* data = loader->getTargetSection((ref_t)mskDataRef);
   _Memory* rdata = loader->getTargetSection((ref_t)mskRDataRef);
   _Memory* sdata = loader->getTargetSection((ref_t)mskStatRef);
   _Memory* code = loader->getTargetSection((ref_t)mskCodeRef);

   MemoryWriter dataWriter(data);
   MemoryWriter rdataWriter(rdata);
   MemoryWriter sdataWriter(sdata);
   MemoryWriter codeWriter(code);

   // preloaded variables
    x86JITScope dataScope(NULL, &dataWriter, &helper, this);
   for (int i = 0; i < coreVariableNumber; i++) {
      if (!_preloaded.exist(coreVariables[i])) {
         _preloaded.add(coreVariables[i], helper.getVAddress(dataWriter, mskDataRef));

         loadCoreData(helper, dataScope, coreVariables[i]);
      }
   }

   // MESSAGE TABLE POINTER
   _preloaded.add(CORE_MESSAGE_TABLE, helper.getVAddress(rdataWriter, mskRDataRef));
   rdataWriter.writeDWord(0);

   // load GC static root
   _preloaded.add(CORE_STATICROOT, helper.getVAddress(sdataWriter, mskStatRef));

   // HOTFIX : preload invoker
   x86JITScope scope(NULL, &codeWriter, &helper, this);
   loadRoutines(envFunctionNumber, envFunctions, scope, _preloaded);

   // SYSTEM_ENV
   x86JITScope rdataScope(NULL, &rdataWriter, &helper, this);
   _preloaded.add(SYSTEM_ENV, helper.getVAddress(rdataWriter, mskRDataRef));
   loadCoreData(helper, rdataScope, SYSTEM_ENV);
   // NOTE : the table is tailed with GCMGSize,GCYGSize,MaxThread
   rdataWriter.writeDWord(helper.getLinkerConstant(lnGCMGSize));
   rdataWriter.writeDWord(helper.getLinkerConstant(lnGCYGSize));
   rdataWriter.writeDWord(helper.getLinkerConstant(lnThreadCount));

   // resolve reference to SYSTEM_ENV at rdata header
   rdata->addReference((ref_t)_preloaded.get(SYSTEM_ENV), 0);

   // lnVMAPI_Instance
   dataWriter.writeDWord(helper.getLinkerConstant(lnVMAPI_Instance)); // ??

   // preloaded core static variables
   for (int i = 0; i < coreStaticNumber; i++) {
      if (!_preloaded.exist(coreStatics[i])) {
         _preloaded.add(coreStatics[i], helper.getVAddress(rdataWriter, mskRDataRef));

         // due to optimization section must be ROModule::ROSection instance
         SectionInfo info = helper.getCoreSection(coreStatics[i]);
         rdataScope.module = info.module;

         loadCoreOp(rdataScope, info.section ? (char*)info.section->get(0) : NULL);
      }
   }

   // preloaded core functions
   loadRoutines(coreFunctionNumber, coreFunctions, scope, _preloaded);

   // preload vm commands
   for (int i = 0; i < gcCommandNumber; i++) {
      SectionInfo info = helper.getCoreSection(gcCommands[i]);

      // due to optimization section must be ROModule::ROSection instance
      _inlines[gcCommands[i]] = (char*)info.section->get(0);
   }

   // preload vm exended commmands
   for (int i = 0; i < gcCommandExNumber; i++) {
      SectionInfo info = helper.getCoreSection(gcCommandExs[i]);
      _inlineExs.add(gcCommandExs[i], info.section->get(0));
   }
}

void x86JITCompiler :: setStaticRootCounter(_JITLoader* loader, size_t counter, bool virtualMode)
{
   if (virtualMode) {
      _Memory* data = loader->getTargetSection(mskRDataRef);

      size_t offset = ((size_t)_preloaded.get(SYSTEM_ENV) & ~mskAnyRef);
      (*data)[offset] = (counter << 2);
   }
   else {
 	   size_t offset = (size_t)_preloaded.get(SYSTEM_ENV);
 	   *(int*)offset = (counter << 2);
   }
}

void x86JITCompiler :: setTLSKey(pos_t ptr)
{
   _preloaded.add(CORE_TLS_INDEX, ptr);
}

void x86JITCompiler :: setThreadTable(vaddr_t ptr)
{
   _preloaded.add(CORE_THREADTABLE, ptr);
}

void x86JITCompiler :: setEHTable(vaddr_t ptr)
{
   _preloaded.add(CORE_EH_TABLE, ptr);
}

void x86JITCompiler :: setGCTable(vaddr_t ptr)
{
   _preloaded.add(CORE_GC_TABLE, ptr);
}

vaddr_t x86JITCompiler :: getPreloadedReference(ref_t reference)
{
   return _preloaded.get(reference);
}

vaddr_t x86JITCompiler :: getInvoker()
{
   return _preloaded.get(INVOKER);
}

void x86JITCompiler :: setVoidParent(_JITLoader* loader, vaddr_t ptr, bool virtualMode)
{
   if (virtualMode) {
      pos_t offset = ((size_t)_preloaded.get(VOID) & ~mskAnyRef);

      _Memory* rdata = loader->getTargetSection((ref_t)mskRDataRef);

      rdata->addReference((ref_t)ptr, offset);
   }
   else {
      size_t offset = (size_t)_preloaded.get(VOID);
      *(int*)offset = (int)ptr;
      offset += 0;
   }
}

void x86JITCompiler :: allocateThreadTable(_JITLoader* loader, int maxThreadNumber)
{
   // get target image & resolve virtual address
   MemoryWriter dataWriter(loader->getTargetSection((ref_t)mskDataRef));

   // size place holder
   dataWriter.writeDWord(0);
   int position = dataWriter.Position();
   if (maxThreadNumber > 0) {
      // reserve space for the thread table
      allocateArray(dataWriter, maxThreadNumber);
   }

   // map thread table
   loader->mapReference(ReferenceInfo(GC_THREADTABLE), (position | mskDataRef), (ref_t)mskDataRef);
   _preloaded.add(CORE_THREADTABLE, position | mskDataRef);
}

int x86JITCompiler :: allocateTLSVariable(_JITLoader* loader)
{
   MemoryWriter dataWriter(loader->getTargetSection((ref_t)mskDataRef));

   // reserve space for TLS index
   int position = dataWriter.Position();
   allocateVariable(dataWriter);

   // map TLS index
   loader->mapReference(ReferenceInfo(TLS_KEY), position | mskDataRef, (ref_t)mskDataRef);
   _preloaded.add(CORE_TLS_INDEX, position | mskDataRef);

   return position;
}

int x86JITCompiler :: allocateVMTape(_JITLoader* loader, void* tape, pos_t length)
{
   MemoryWriter dataWriter(loader->getTargetSection((ref_t)mskRDataRef));

   // reserve space for TLS index
   int position = dataWriter.Position();

   dataWriter.write(tape, length);

   // map VMTape
   loader->mapReference(ReferenceInfo(TAPE_KEY), position | mskRDataRef, (ref_t)mskRDataRef);

   return position;
}

inline void compileTape(MemoryReader& tapeReader, size_t endPos, x86JITScope& scope)
{
   unsigned char code = 0;
   while(tapeReader.Position() < endPos) {
      // read bytecode + arguments
      code = tapeReader.getByte();
      // preload an argument if a command requires it
      if (code > MAX_SINGLE_ECODE) {
         scope.argument = tapeReader.getDWord();
      }
      commands[code](code, scope);
   }
}

void x86JITCompiler :: compileSymbol(_ReferenceHelper& helper, MemoryReader& tapeReader, MemoryWriter& codeWriter)
{
   x86JITScope scope(&tapeReader, &codeWriter, &helper, this);

   size_t codeSize = tapeReader.getDWord();
   size_t endPos = tapeReader.Position() + codeSize;

   compileTape(tapeReader, endPos, scope);

   // ; copy the parameter to the accumulator to simulate embedded symbol
   // ; exit the procedure
   // ret
   codeWriter.writeByte(0xC3);

   alignCode(&codeWriter, 0x04, true);
}

void x86JITCompiler :: compileProcedure(_ReferenceHelper& helper, MemoryReader& tapeReader, MemoryWriter& codeWriter)
{
   x86JITScope scope(&tapeReader, &codeWriter, &helper, this);
//   scope.prevFSPOffs = 4;

   size_t codeSize = tapeReader.getDWord();
   size_t endPos = tapeReader.Position() + codeSize;

   compileTape(tapeReader, endPos, scope);

   alignCode(&codeWriter, 0x04, true);
}

void x86JITCompiler :: generateSymbolCall(MemoryDump& tape, vaddr_t address)
{
   MemoryWriter ecodes(&tape);

   ecodes.writeByte(bcCallR);
   ecodes.writeDWord(address | mskCodeRef);
}

void x86JITCompiler :: generateProgramEnd(MemoryDump& tape)
{
   MemoryWriter ecodes(&tape);

   ecodes.writeByte(bcQuit);

   JITCompiler32::generateProgramEnd(tape);
}
