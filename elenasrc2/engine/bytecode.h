   //		E L E N A   P r o j e c t:  ELENA Engine
//
//		This file contains common ELENA byte code classes and constants
//
//                                              (C)2009-2020, by Alexei Rakov
//------------------------------------------------------------------------------

#ifndef bytecodeH
#define bytecodeH 1

namespace _ELENA_
{

// --- Byte code command set ---
enum ByteCode
{
   // commands:
   bcNop             = 0x00,
   bcBreakpoint      = 0x01,
   bcCoalesce        = 0x02,
//   bcPop             = 0x03,
   bcSNop            = 0x04,
   bcPushVerb        = 0x05,
   bcLoadVerb        = 0x06,
   bcThrow           = 0x07,
   bcMCount          = 0x08,
   bcPush            = 0x09,
   bcPushA           = 0x0A,
   bcPopA            = 0x0B,
//   bcACopyB          = 0x0C,
//   bcPopE            = 0x0D,
   bcBSRedirect      = 0x0E,
   bcSetVerb         = 0x0F,

   bcNot             = 0x10,
//   bcLen             = 0x11,
//   bcBCopyA          = 0x12,
//   bcDec             = 0x13,
//   bcPopB            = 0x14,
   bcClose           = 0x15,
//   bcSub             = 0x16,
   bcQuit            = 0x17,
   bcGet             = 0x18,
   bcSet             = 0x19,
//   bcInc             = 0x1A,
   bcMQuit           = 0x1B,
   bcCount           = 0x1C,
   bcUnhook          = 0x1D,
//   bcAdd             = 0x1E,
//   bcCreate          = 0x1F,
//
//   bcECopyD          = 0x20,
//   bcDCopyE          = 0x21,
   bcPushD           = 0x22,
   bcPopD            = 0x23,
//   bcXCopy           = 0x24,
   bcInclude         = 0x25,     // should immediately follow exclude (after callextr)
   bcExclude         = 0x26,   
   bcTryLock         = 0x27,
   bcFreeLock        = 0x28,
//   bcRethrow         = 0x29,
   bcLoadEnv         = 0x2A,
//   bcSelect          = 0x2B,
//   bcESwap           = 0x2C,
   bcRead            = 0x2D,
   bcClone           = 0x2E,           
//   bcXSet            = 0x2F,
//
//   bcXLen            = 0x30,
   bcLen             = 0x31,
   bcRLoad           = 0x32,
   bcFlag            = 0x33,
//   bcNLen            = 0x34,
//   bcParent          = 0x35,
   bcClass           = 0x36,
//   bcMIndex          = 0x37,
//   bcCheck           = 0x38,
//   bcACallVD         = 0x39,
//   bcValidate        = 0x3A,
//   bcAndE            = 0x3B,
//   bcDMoveVerb       = 0x3C,
   bcEqual           = 0x3E,

   bcNEqual          = 0x40,
   bcNLess           = 0x41,
//   bcNCopy           = 0x42,
   bcLEqual          = 0x43,
   bcLLess           = 0x44,
   bcRSet            = 0x45,
   bcRSave           = 0x46,
   bcSave            = 0x47,
   bcLoad            = 0x48,
   bcRGet            = 0x49,
//   bcNAnd            = 0x4A,
//   bcNOr             = 0x4B,
//   bcNXor            = 0x4C,
//   bcNShiftL         = 0x4D,
//   bcNNot            = 0x4E,
//   bcNCreate         = 0x4F,
//


   bcAddF            = 0x50,
   bcSubF            = 0x51,
   bcNXorF           = 0x52,
   bcNOrF            = 0x53,
   bcNAndF           = 0x54,
//   bcWRead           = 0x59,
//   bcWWrite          = 0x5A,
   bcDiv             = 0x5B,
   bcXWrite          = 0x5C,
   bcCopyTo          = 0x5D,
   bcNShlF           = 0x5E,
   bcNShrF           = 0x5F,

//   bcBRead           = 0x61,
//   bcBReadB          = 0x65,
//   bcRSin            = 0x66,
//   bcRCos            = 0x67,
//   bcRArcTan         = 0x68,
//   bcBWrite          = 0x69,
//   bcBWriteB         = 0x6C,
//   bcBWriteW         = 0x6D,
//   bcBCreate         = 0x6F,
//
//   bcLCopy           = 0x70,
//   bcLSave           = 0x71,
//   bcLEqual          = 0x72,
//   bcLLess           = 0x73,
   bcLAddF           = 0x74,
   bcLSubF           = 0x75,
   bcLMulF           = 0x76,
   bcLDivF           = 0x77,
   bcLAndF           = 0x78,
   bcLOrF            = 0x79,
   bcLXorF           = 0x7A,
   bcLShlF           = 0x7B,
//   bcLNot            = 0x7C,
   bcLShrF           = 0x7D,

   bcREqual          = 0x83,
   bcRLess           = 0x84,
   bcRAddF           = 0x85,
   bcRSubF           = 0x86,
   bcRMulF           = 0x87,
   bcRDivF           = 0x88,
//   bcRExp            = 0x8A,
//   bcRLn             = 0x8B,
//   bcRAbs            = 0x8C,
//   bcRRound          = 0x8D,
   bcRIntF           = 0x8E,

   bcDec             = 0x90,
   bcGetI            = 0x91,
   bcRestore         = 0x92,
   bcPeekR           = 0x93,
   bcPeekFI          = 0x94,
   bcPeekSI          = 0x95,
//   bcIfHeap          = 0x96,
   bcXSetI           = 0x97,
   bcOpen            = 0x98,
   bcQuitN           = 0x99,
   bcCreate          = 0x9A,
   bcFillR           = 0x9B,
   bcMovF            = 0x9C,
   bcMovS            = 0x9D,
   bcMovR            = 0x9E,
   bcMovM            = 0x9F,

   bcJump            = 0xA0,
   bcJumpVI          = 0xA1,
   bcCallVI          = 0xA2,
   bcCallR           = 0xA3,
   bcJumpI           = 0xA4,
   bcCallExtR        = 0xA5,
   bcHook            = 0xA6,
   bcAddress         = 0xA7,
//   bcACallI          = 0xA8,
//   bcLess            = 0xA9,
   bcNotLess         = 0xAA,
   bcNotGreater      = 0xAB,
//   bcElseB           = 0xAC,
//   bcIf              = 0xAD,
   bcElse            = 0xAE,
//   bcNext            = 0xAF,

   bcPushN           = 0xB0,
   bcMovN            = 0xB1,
   bcPushR           = 0xB2,
//   bcBSaveSI         = 0xB3,
   bcPushAI          = 0xB4,
//   bcESaveFI         = 0xB5,
   bcPushFI          = 0xB6,
   bcLoadFI          = 0xB7,
   bcLoadSI          = 0xB8,
   bcSaveF           = 0xB9,
   bcPushSI          = 0xBA,
   bcSaveSI          = 0xBB,
//   bcELoadSI         = 0xBC,
   bcPushF           = 0xBD,
   bcPushS           = 0xBE,
   bcReserve         = 0xBF,   // should be used only for unmanaged stack (stack may contains old references, which may break GC)

   bcSetI            = 0xC0,
//   bcNWriteI         = 0xC1,
//   bcASwapSI         = 0xC2,
   bcStoreSI         = 0xC3,
   bcStoreFI         = 0xC4,
   bcNAddF           = 0xC5,
   bcNMulF           = 0xC6,
//   bcDSwapSI         = 0xC7,
   bcNSubF           = 0xC8,
   bcNDivF           = 0xC9,
   bcLoadI           = 0xCA,
   bcSaveI           = 0xCB,
   bcStoreR          = 0xCC,
//   bcALoadAI         = 0xCD,
   bcCloneF          = 0xCE,
//   bcAXSaveBI        = 0xCF,

   bcFreeI           = 0xD0,
   bcAllocI          = 0xD1, 
//   bcNReadI          = 0xD1,
//   bcSCopyF          = 0xD2,
   bcMovV            = 0xD3,
   bcShl             = 0xD4,
   bcAnd             = 0xD5,
   bcInc             = 0xD6,
   bcOr              = 0xD7,
   bcCoalesceR       = 0xD8,
   bcShr             = 0xD9,



//   bcMulN            = 0xDA,
//   bcBLoadR          = 0xDC,
//   bcInit            = 0xDD,
//   bcEqualR          = 0xDE,
//   bcBLoadAI         = 0xDF,

   bcReadToF         = 0xE0,
   bcCreateN         = 0xE1,
   bcXSetFI          = 0xE2,
   bcCopyToAI        = 0xE3,
   bcCopyToFI        = 0xE4,
   bcCopyToF         = 0xE5,
   bcCopyFI          = 0xE6,
   bcCopyF           = 0xE7,
   bcMTRedirect      = 0xE8,
   bcXMTRedirect     = 0xE9,
//   bcGreaterN        = 0xEA,   // note that for code simplicity reverse order is used for jump parameters (jump label, arg)
//   bcNotGreaterN     = 0xEB,   // note that for code simplicity reverse order is used for jump parameters (jump label, arg)
//   bcNotLessN        = 0xEC,   // note that for code simplicity reverse order is used for jump parameters (jump label, arg)
//   bcSubFI           = 0xED,
//   bcAddFI           = 0xEE,
   bcXSaveF          = 0xEF,

   bcNew             = 0xF0,
   bcNewN            = 0xF1,
   bcFillRI          = 0xF2,
   bcXSelectR        = 0xF3,
   bcVCallRM         = 0xF4,
   bcJumpRM          = 0xF5,
   bcSelect          = 0xF6,
   bcLessN           = 0xF7,   // note that for code simplicity reverse order is used for jump parameters (jump label, arg)
//   bcIfM             = 0xF8,   // note that for code simplicity reverse order is used for jump parameters (jump label, arg)
//   bcElseM           = 0xF9,   // though in bytecode section they saved in the correct order (jump arg, label)
   bcIfR             = 0xFA,
   bcElseR           = 0xFB,   
   bcIfN             = 0xFC,
   bcElseN           = 0xFD,   
   bcCallRM          = 0xFE,
   bcReserved        = 0xFF,

   // labels
   blLabelMask       = 0xC000,  // tape label mask
   blBegin           = 0xC001,  // meta command, declaring the structure
   blEnd             = 0xC002,  // meta command, closing the structure
   blLabel           = 0xC003,  // meta command, declaring the label
   blBreakLabel      = 0xC004,  // meta command, breaking the optimization rules

   // meta commands:
   //bcAllocStack     = 0x8101,  // meta command, used to indicate that the previous command allocate number of items in the stack; used only for exec
   //bcFreeStack      = 0x8102,  // meta command, used to indicate that the previous command release number of items from stack; used only for exec
   //bcResetStack     = 0x8103,  // meta command, used to indicate that the previous command release number of items from stack; used only for exec

   bcMatch          = 0x8FFE,  // used in optimization engine
   bcNone           = 0x8FFF,  // used in optimization engine

   //blDeclare        = 0x8120,  // meta command, closing the structure
   blStatement      = 0x8121,  // meta command, declaring statement
   blBlock          = 0x8122,  // meta command, declaring sub code

   // debug info
   //bdDebugInfo      = 0x8400,
   bdBreakpoint     = 0x8401,
   bdBreakcoord     = 0x8402,
   bdLocal          = 0x8403,
   bdSelf           = 0x8404,
   bdMessage        = 0x8405,
   bdLocalInfo      = 0x8406,
   bdSourcePath     = 0x8407,

   bdIntLocal       = 0x8413,
   bdLongLocal      = 0x8423,
   bdRealLocal      = 0x8433,
   bdParamsLocal    = 0x8443,
   bdByteArrayLocal = 0x8453,
   bdShortArrayLocal= 0x8463,
   bdIntArrayLocal  = 0x8473,
   bdStruct         = 0x8486,
   bdStructSelf     = 0x8484,
};

#define MAX_SINGLE_ECODE 0x4F
#define MAX_DOUBLE_ECODE 0xDF

enum PseudoArg
{
   baNone          = 0,
   baFirstLabel    = 1,
   baCurrentLabel  = 2,
   baPreviousLabel = 3,
   baPrev2Label    = 4, // before previous
};

enum Predicate
{
   bpNone  = 0,
   bpFrame = 1,
//   bpBlock = 2
};

enum TapeStructure
{
   bsNone        = 0x0,
   bsSymbol      = 0x1,
   bsClass       = 0x2,
   bsMethod      = 0x3,
   //bsBranch      = 0x5,
   bsImport      = 0x6,
   bsInitializer = 0x7,
};

struct ByteCommand
{
   ByteCode  code;
   int       argument;
   int       additional;
   Predicate predicate;

   int Argument() const { return argument; }

   operator ByteCode() const { return code; }

   ByteCommand()
   {
      code = bcNop;
      argument = 0;
      additional = 0;
      predicate = bpNone;
   }
   ByteCommand(ByteCode code)
   {
      this->code = code;
      this->argument = 0;
      this->additional = 0;
      this->predicate = bpNone;
   }
   ByteCommand(ByteCode code, int argument)
   {
      this->code = code;
      this->argument = argument;
      this->additional = 0;
      this->predicate = bpNone;
   }
   ByteCommand(ByteCode code, int argument, int additional)
   {
      this->code = code;
      this->argument = argument;
      this->additional = additional;
      this->predicate = bpNone;
   }
   ByteCommand(ByteCode code, int argument, int additional, Predicate predicate)
   {
      this->code = code;
      this->argument = argument;
      this->additional = additional;
      this->predicate = predicate;
   }

   void save(MemoryWriter* writer, bool commandOnly = false)
   {
      writer->writeByte((unsigned char)code);
      if (!commandOnly && (code > MAX_SINGLE_ECODE)) {
         writer->writeDWord(argument);
      }
      if (!commandOnly && (code > MAX_DOUBLE_ECODE)) {
         writer->writeDWord(additional);
      }
   }

   // save additional argument if required
   void saveAditional(MemoryWriter* writer)
   {
      if (code > MAX_DOUBLE_ECODE) {
         writer->writeDWord(additional);
      }
   }
};

// --- ByteCodeCompiler ---

class ByteCodeCompiler
{
public:
   //static void loadVerbs(MessageMap& verbs);
   static void loadOperators(MessageMap& operators);

   static ByteCode code(ident_t s);

   static ident_t decode(ByteCode code, char* s);

   static bool IsJump(ByteCode code)
   {
      switch(code) {
         case bcJump:
         case bcIfR:
         case bcElseR:
         //case bcIfB:
         //case bcElseB:
         //case bcIf:
         case bcElse:
         //case bcLess:
         case bcNotLess:
         case bcNotGreater:
         case bcIfN:
         case bcElseN:
         case bcLessN:
         //case bcNotLessN:
         //case bcGreaterN:
         //case bcNotGreaterN:
         //case bcIfM:
         //case bcElseM:
         //case bcNext:
         //case bcIfHeap:
         case bcHook:
         case bcAddress:
         case bcJumpRM:
         case bcJumpI:
            return true;
         default:
            return false;
         }
   }

   static bool IsRCode(ByteCode code)
   {
      switch(code) {
         case bcPushR:
         ////case bcEvalR:
         case bcCallR:
         case bcPeekR:
         case bcStoreR:
         case bcMovR:
         case bcNew:
         case bcFillRI:
         case bcFillR:
         case bcNewN:
         //case bcBCopyR:
         case bcCallRM:
         case bcCallExtR:
         case bcSelect:
         case bcJumpRM:
         case bcJumpI:
         case bcVCallRM:
         //case bcBLoadR:
         case bcCreate:
         case bcCreateN:
         case bcCoalesceR:
         case bcXSelectR:
            return true;
         default:
            return false;
      }
   }

   static bool IsR2Code(ByteCode code)
   {
      switch(code) {
         case bcIfR:
         case bcElseR:
         case bcSelect:
         case bcXSelectR:
            return true;
         default:
            return false;
      }
   }

   static bool IsM2Code(ByteCode code)
   {
      switch (code) {
         case bcVCallRM:
         case bcCallRM:
         //case bcIfM:
         //case bcElseM:
         //   return true;
         default:
            return false;
      }
   }

   static bool IsMCode(ByteCode code)
   {
      switch (code) {
         case bcMovM:
            return true;
         default:
            return false;
      }
   }

   static bool IsMNCode(ByteCode code)
   {
      switch (code) {
         case bcMovV:
            return true;
         default:
            return false;
      }
   }

   static bool IsPush(ByteCode code)
   {
      switch(code) {
         case bcPushA:
         //case bcPushB:
         case bcPushFI:
         case bcPushN:
         case bcPushR:
         case bcAllocI:
         case bcPushSI:
         case bcPushS:
         case bcPushAI:
         case bcPushF:
         //case bcPushE:
         case bcPushD:
            return true;
         default:
            return false;
      }
   }

   static bool IsPop(ByteCode code)
   {
      switch(code) {
         //case bcPop:
         case bcPopA:
         case bcFreeI:
         //case bcPopB:
         //case bcPopE:
         case bcPopD:
            return true;
         default:
            return false;
      }
   }

   static bool resolveMessageName(IdentifierString& messageName, _Module* module, size_t messageRef);
};

// --- CommandTape ---
typedef BList<ByteCommand>::Iterator ByteCodeIterator;

struct CommandTape
{
   BList<ByteCommand> tape;   // !! should we better use an array?

   int            labelSeed;
   Stack<int>     labels;
   
   ByteCodeIterator start() { return tape.start(); }

   ByteCodeIterator end() { return tape.end(); }

   int newLabel()
   {
      labelSeed++;

      labels.push(labelSeed);

      return labelSeed;
   }

   void setLabel(bool persist = false)
   {
      if (persist) {
         write(blLabel, labels.peek());
      }
      else write(blLabel, labels.pop());
   }

   void setPreviousLabel()
   {
      int lastLabel = labels.pop();

      write(blLabel, labels.pop());

      labels.push(lastLabel);
   }

   // to resolve possible conflicts the predefined labels should be negative
   void setPredefinedLabel(int label)
   {
      write(blLabel, label);
   }

   void releaseLabel()
   {
      labels.pop();
   }

   int exchangeFirstsLabel(int newLabel)
   {
      auto it = labels.end();

      int oldLabel = *it;
      *it = newLabel;

      return oldLabel;
   }

   ByteCodeIterator find(ByteCode code);
   ByteCodeIterator find(ByteCode code, int argument);

   int resolvePseudoArg(PseudoArg argument);

   void write(ByteCode code);
   void write(ByteCode code, int argument);
   void write(ByteCode code, PseudoArg argument);
   void write(ByteCode code, int argument, int additional);
   void write(ByteCode code, PseudoArg argument, int additional);
   void write(ByteCode code, TapeStructure argument, int additional);
   void write(ByteCode code, int argument, int additional, Predicate predicate);
   void write(ByteCode code, int argument, Predicate predicate);
   void write(ByteCommand command);
   void insert(ByteCodeIterator& it, ByteCommand command);

//   ByteCommand extract()
//   {
//      ByteCommand command = *tape.end();
//      tape.cut(tape.end());
//
//      return command;
//   }

   void import(_Memory* section, bool withHeader = false, bool withBreakpoints = false);

   static bool optimizeIdleBreakpoints(CommandTape& tape);
   static bool optimizeJumps(CommandTape& tape);
   static bool importReference(ByteCommand& command, _Module* sour, _Module* dest);

   CommandTape()
   {
      labelSeed = 0;
   }

   void clear()
   {
      tape.clear();
      labelSeed = 0;
      labels.clear();
   }
};

// --- ByteRule ---

enum PatternArgument
{
   braNone = 0,
   braValue,
   braAditionalValue,
   braAdd,
   braCopy,
   braMatch,
   braSame,          // TransformTape should perform xor operation with the argument (1 if same, 0 if different)
   braAdditionalSame // TransformTape should perform xor operation with the argument (1 if same, 0 if different)
};

struct ByteCodePattern
{
   ByteCode        code;
   PatternArgument argumentType;
   int             argument;

   bool operator ==(ByteCode code) const
   {
      return (this->code == code);
   }

   bool operator !=(ByteCode code) const
   {
      return (this->code != code);
   }

   bool operator ==(ByteCommand command) const
   {
      if (this->code == command.code) {
         if (argumentType == braSame || argumentType == braAdditionalSame) {
            return argument == 0;
         }
         else return argumentType != braMatch || argument == command.argument;
      }
      else return false;
   }

   bool operator !=(ByteCommand command) const
   {
      return !(*this == command);
   }

   bool operator ==(ByteCodePattern pattern)
   {
      return (code == pattern.code && argumentType == pattern.argumentType && argument == pattern.argument);
   }

   bool operator !=(ByteCodePattern pattern)
   {
      return !(*this == pattern);
   }

   ByteCodePattern()
   {
      code = bcNone;
      argumentType = braNone;
      argument = 0;
   }
   ByteCodePattern(ByteCode code)
   {
      this->code = code;
      this->argumentType = braNone;
      this->argument = 0;
   }
};

// --- TransformTape ---

struct TransformTape
{
   typedef MemoryTrie<ByteCodePattern>     MemoryByteTrie;
   typedef MemoryTrieNode<ByteCodePattern> Node;

   MemoryByteTrie trie;
   bool           loaded;

   bool apply(CommandTape& tape);
   void transform(ByteCodeIterator& trans_it, Node replacement);

   bool makeStep(Node& step, ByteCommand& command, int previousArg);

   void load(StreamReader* optimization)
   {
      loaded = true;
      trie.load(optimization);
   }

   TransformTape()
      : trie(ByteCodePattern(bcNone))
   {
      loaded = false;
   }
};

} // _ELENA_

#endif // bytecodeH
