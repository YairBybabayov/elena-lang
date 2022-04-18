//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler Engine
//
//		This file contains ELENA JIT compiler class.
//
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef JITCOMPILER_H
#define JITCOMPILER_H

#include "elena.h"
#include "bytecode.h"

namespace elena_lang
{
   constexpr auto NumberOfInlines = 7;

   // --- JITCompilerScope ---
   class JITCompiler;

   // --- JITConstants ---
   struct JITConstants
   {
      // an offset to raw stack data
      int          dataOffset;
      int          dataHeader;
      int          indexPower;
      ref_t        inlineMask;
      int          alignmentVA;
      int          structMask;
   };

   struct JITCompilerScope
   {
      ReferenceHelperBase* helper;
      JITCompiler*         compiler;
      LabelHelperBase*     lh;
      MemoryWriter*        codeWriter;
      ByteCommand          command;
      JITConstants*        constants;

      bool                 withDebugInfo;
      ref_t                frameOffset;

      unsigned char code() const
      {
         return (unsigned char)command.code;
      }

      JITCompilerScope(ReferenceHelperBase* helper, JITCompiler* compiler, LabelHelperBase* lh, MemoryWriter* writer,
         JITConstants* constants);
   };

   typedef void(*CodeGenerator)(JITCompilerScope*);

   // --- JITCompiler ---
   class JITCompiler : public JITCompilerBase
   {
   public:
      typedef FixedMemoryMap<ref_t, void*> PreloadedMap;

   protected:
      void*        _inlines[NumberOfInlines][0x100];
      PreloadedMap _preloaded;

      JITConstants _constants;

      CodeGenerator* codeGenerators();

      virtual int calcFrameOffset(int argument) = 0;
      virtual int calcTotalSize(int numberOfFields) = 0;
      virtual int calcTotalStructSize(int size) = 0;

      void writeArgAddress(JITCompilerScope* scope, arg_t arg, pos_t offset, ref_t addressMask);

      virtual void compileTape(ReferenceHelperBase* helper, MemoryReader& bcReader, pos_t endPos, 
         MemoryWriter& codeWriter, LabelHelperBase* lh);

      friend void writeCoreReference(JITCompilerScope* scope, ref_t reference/*, pos_t position*/, 
         pos_t disp, void* code);
      friend void allocateCode(JITCompilerScope* scope, void* code);
      friend void loadCode(JITCompilerScope* scope, void* code);

      friend void* retrieveCode(JITCompilerScope* scope);
      friend void* retrieveCodeWithNegative(JITCompilerScope* scope);

      friend void loadOp(JITCompilerScope* scope);
      friend void loadLOp(JITCompilerScope* scope);
      friend void loadIndexOp(JITCompilerScope* scope);
      friend void loadVMTIndexOp(JITCompilerScope* scope);
      friend void loadFrameIndexOp(JITCompilerScope* scope);
      friend void loadFrameDispOp(JITCompilerScope* scope);
      friend void loadNOp(JITCompilerScope* scope);
      friend void loadROp(JITCompilerScope* scope);
      friend void loadMOp(JITCompilerScope* scope);
      friend void loadCallOp(JITCompilerScope* scope);
      friend void loadCallROp(JITCompilerScope* scope);
      friend void loadIndexROp(JITCompilerScope* scope);
      friend void loadFrameIndexROp(JITCompilerScope* scope);
      friend void loadIndexNOp(JITCompilerScope* scope);
      friend void loadIndexIndexOp(JITCompilerScope* scope);
      friend void loadNewOp(JITCompilerScope* scope);
      friend void loadNewNOp(JITCompilerScope* scope);
      friend void loadMROp(JITCompilerScope* scope);
      friend void loadVMTROp(JITCompilerScope* scope);

      friend void compileBreakpoint(JITCompilerScope* scope);
      friend void compileClose(JITCompilerScope* scope);
      friend void compileOpen(JITCompilerScope* scope);
      friend void compileJump(JITCompilerScope* scope);

      void loadCoreRoutines(
         LibraryLoaderBase* loader,
         ImageProviderBase* imageProvider,
         ReferenceHelperBase* helper,
         LabelHelperBase* lh,
         JITSettings settings,
         Map<ref_t, pos_t>& positions, bool declareMode);

   public:
      bool isWithDebugInfo() override
      {
         // in the current implementation, debug info (i.e. debug section)
         // is always generated (to be used by RTManager)
         return true;
      }

      void prepare(
         LibraryLoaderBase* loader, 
         ImageProviderBase* imageProvider, 
         ReferenceHelperBase* helper,
         LabelHelperBase* lh,
         JITSettings settings) override;

      void compileProcedure(ReferenceHelperBase* helper, MemoryReader& bcReader, 
         MemoryWriter& codeWriter, LabelHelperBase* lh) override;
      void compileSymbol(ReferenceHelperBase* helper, MemoryReader& bcReader, MemoryWriter& codeWriter
         , LabelHelperBase* lh) override;

      void writeImm16(MemoryWriter* writer, int value, int type) override
      {
         switch (type) {
            case INV_ARG:
               writer->writeWord(-value);
               break;
            default:
               writer->writeWord(value);
               break;
         }         
      }

      void writeImm32(MemoryWriter* writer, int value) override
      {
         writer->writeDWord(value);
      }

      JITCompiler()
         : _inlines{}, _preloaded(nullptr)
      {
         _constants.indexPower = 0;
         _constants.dataHeader = _constants.dataOffset = 0;
         _constants.inlineMask = 0;
         _constants.alignmentVA = 8;
      }
   };

   // --- JITCompiler32 ---
   class JITCompiler32 : public JITCompiler
   {
   protected:
      int calcFrameOffset(int argument) override
      {
         return 4 + (argument > 0 ? align(argument + 8, 4) : 0);
      }

      int calcTotalSize(int numberOfFields) override;
      int calcTotalStructSize(int size) override;

   public:
      void prepare(
         LibraryLoaderBase* loader, 
         ImageProviderBase* imageProvider, 
         ReferenceHelperBase* helper,
         LabelHelperBase* lh,
         JITSettings settings) override;

      void compileMetaList(ReferenceHelperBase* helper, MemoryReader& reader, MemoryWriter& writer, pos_t length) override;

      pos_t getVMTLength(void* targetVMT) override;
      addr_t findMethodAddress(void* entries, mssg_t message) override;
      pos_t findMethodOffset(void* entries, mssg_t message) override;

      void allocateVMT(MemoryWriter& vmtWriter, pos_t flags, pos_t vmtLength) override;
      void addVMTEntry(mssg_t message, addr_t codeAddress, void* targetVMT, pos_t& entryCount) override;
      void updateVMTHeader(MemoryWriter& vmtWriter, addr_t parentAddress, addr_t classClassAddress, 
         ref_t flags, pos_t count, bool virtualMode) override;
      pos_t copyParentVMT(void* parentVMT, void* targetVMT) override;

      void allocateHeader(MemoryWriter& writer, addr_t vmtAddress, int length, 
         bool structMode, bool virtualMode) override;

      pos_t addActionEntry(MemoryWriter& messageWriter, MemoryWriter& messageBodyWriter, ustr_t actionName, 
         ref_t weakActionRef, ref_t signature) override;
      pos_t addSignatureEntry(MemoryWriter& writer, addr_t vmtAddress, bool virtualMode) override;

      void addBreakpoint(MemoryWriter& writer, MemoryWriter& codeWriter, bool virtualMode) override;
      void addBreakpoint(MemoryWriter& writer, addr_t vaddress, bool virtualMode) override;

      void writeInt32(MemoryWriter& writer, unsigned value) override;
      void writeLiteral(MemoryWriter& writer, ustr_t value) override;

      JITCompiler32()
         : JITCompiler()
      {
      }
   };

   // --- JITCompiler64 ---
   class JITCompiler64 : public JITCompiler
   {
   protected:
      int calcFrameOffset(int argument) override
      {
         return 8 + (argument > 0 ? align(argument + 16, 16) : 0);
      }

      int calcTotalSize(int numberOfFields) override;
      int calcTotalStructSize(int size) override;

   public:
      void prepare(
         LibraryLoaderBase* loader, 
         ImageProviderBase* imageProvider, 
         ReferenceHelperBase* helper,
         LabelHelperBase* lh,
         JITSettings settings) override;

      void compileMetaList(ReferenceHelperBase* helper, MemoryReader& reader, MemoryWriter& writer, pos_t length) override;

      pos_t getVMTLength(void* targetVMT) override;
      addr_t findMethodAddress(void* entries, mssg_t message) override;
      pos_t findMethodOffset(void* entries, mssg_t message) override;

      void allocateVMT(MemoryWriter& vmtWriter, pos_t flags, pos_t vmtLength) override;
      pos_t copyParentVMT(void* parentVMT, void* targetVMT) override;
      void addVMTEntry(mssg_t message, addr_t codeAddress, void* targetVMT, pos_t& entryCount) override;
      void updateVMTHeader(MemoryWriter& vmtWriter, addr_t parentAddress, addr_t classClassAddress, 
         ref_t flags, pos_t count, bool virtualMode) override;

      void allocateHeader(MemoryWriter& writer, addr_t vmtAddress, int length, 
         bool structMode, bool virtualMode) override;

      pos_t addActionEntry(MemoryWriter& messageWriter, MemoryWriter& messageBodyWriter, ustr_t actionName, ref_t weakActionRef, 
         ref_t signature) override;
      pos_t addSignatureEntry(MemoryWriter& writer, addr_t vmtAddress, bool virtualMode) override;

      void addBreakpoint(MemoryWriter& writer, MemoryWriter& codeWriter, bool virtualMode) override;
      void addBreakpoint(MemoryWriter& writer, addr_t vaddress, bool virtualMode) override;

      void writeInt32(MemoryWriter& writer, unsigned value) override;
      void writeLiteral(MemoryWriter& writer, ustr_t value) override;

      JITCompiler64()
         : JITCompiler()
      {
      }
   };

   void writeCoreReference(JITCompilerScope* scope, ref_t reference/*, pos_t position*/, pos_t disp, void* code);
   void loadCode(JITCompilerScope* scope, void* code);
   void allocateCode(JITCompilerScope* scope, void* code);

   inline void* retrieveCode(JITCompilerScope* scope);
   inline void* retrieveCodeWithNegative(JITCompilerScope* scope);

   void loadNop(JITCompilerScope*);
   void loadOp(JITCompilerScope* scope);
   void loadLOp(JITCompilerScope* scope);
   void loadIndexOp(JITCompilerScope* scope);
   void loadVMTIndexOp(JITCompilerScope* scope);
   void loadFrameIndexOp(JITCompilerScope* scope);
   void loadFrameDispOp(JITCompilerScope* scope);
   void loadNOp(JITCompilerScope* scope);
   void loadROp(JITCompilerScope* scope);
   void loadMOp(JITCompilerScope* scope);
   void loadCallOp(JITCompilerScope* scope);
   void loadCallROp(JITCompilerScope* scope);
   void loadIndexROp(JITCompilerScope* scope);
   void loadFrameIndexROp(JITCompilerScope* scope);
   void loadIndexNOp(JITCompilerScope* scope);
   void loadIndexIndexOp(JITCompilerScope* scope);
   void loadNewOp(JITCompilerScope* scope);
   void loadNewNOp(JITCompilerScope* scope);
   void loadMROp(JITCompilerScope* scope);
   void loadVMTROp(JITCompilerScope* scope);

   void compileClose(JITCompilerScope* scope);
   void compileOpen(JITCompilerScope* scope);
   void compileBreakpoint(JITCompilerScope* scope);
   void compileJump(JITCompilerScope* scope);
}

#endif