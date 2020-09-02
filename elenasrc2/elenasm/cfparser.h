//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA VM Script Engine
//
//                                             (C)2011-2020, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef cfparserH
#define cfparserH 1

#include "scriptengine.h"

namespace _ELENA_
{

// --- CFPrarser ---

class CFParser : public _Parser
{
public:
   typedef Map<ident_t, int> Mapping;

   enum RuleType
   {
      rtTypeMask      = 0x0F,

      rtNone          = 0x00,
      rtNormal        = 0x01,
      rtChomski       = 0x02,
      rtNonterminal   = 0x03,
      rtEps           = 0x04,
      rtWithForward   = 0x10,
      rtWithBookmark  = 0x20,
   };
   
   typedef void (*SaveToSign)(_ScriptReader& scriptReader, CFParser* parser, ref_t ptr, ScriptLog& log);

   struct Rule
   {
      RuleType type;
      size_t   terminal;    // in chomski form it is additional nonterminal as well
      size_t   nonterminal;
      size_t   prefix1Ptr;
      size_t   prefix2Ptr;
      size_t   postfix1Ptr;
      size_t   postfix2Ptr;

      bool(*apply)(Rule& rule, ScriptBookmark& bm, _ScriptReader& reader, CFParser* parser);
      SaveToSign saveTo;

      Rule()
      {
         type = rtNone;
         terminal = nonterminal = 0;
         apply = NULL;
         saveTo = NULL;
         postfix1Ptr = prefix1Ptr = 0;
         postfix2Ptr = prefix2Ptr = 0;
      }
   };

   struct DerivationItem
   {
      size_t ruleId;
      int    trace;    // derivation trace
      int    next;     // tailing nonterminal

      DerivationItem()
      {
         ruleId = 0;
         trace = 0;
         next = 0;
      }
      DerivationItem(int ruleId)
      {
         this->ruleId = ruleId;
         this->trace = 0;
         this->next = 0;
      }
      DerivationItem(int ruleId, int trace, int next)
      {
         this->ruleId = ruleId;
         this->trace = trace;
         this->next = next;
      }
   };

   struct TraceItem
   {
      int ruleKey;
      int terminal;
      int previous;

      TraceItem()
      {
         ruleKey = terminal = previous = 0;
      }
      TraceItem(int key)
      {
         ruleKey = key;
         terminal = previous = 0;
      }
   };

   typedef Queue<DerivationItem> DerivationQueue;
   typedef MemoryHashTable<size_t, Rule, syntaxRule, cnHashSize> SyntaxTable;
   typedef MemoryMap<ident_t, size_t> NameMap;

protected:
   _Parser*          _baseParser;
   SyntaxTable       _table;
   NameMap           _names;
   MemoryDump        _body;

   bool              _symbolMode;

   size_t mapRuleId(ident_t name)
   {
      return mapKey(_names, name, _names.Count() + 1);
   }

   void setScriptPtr(ScriptBookmark& bm, Rule& rule, bool prefixMode);

   size_t autonameRule(size_t parentRuleId);

   void defineApplyRule(Rule& rule, int terminalType, bool forwardMode, bool bookmarkMode);

   size_t writeBodyText(ident_t text);
   size_t writeRegExprBodyText(_ScriptReader& reader, int& mode);
   const char* getBodyText(size_t ptr);

   void addRule(int id, Rule& rule);

   void saveScript(_ScriptReader& reader, Rule& rule, int& mode, bool forwardMode);
   void defineIdleGrammarRule(ref_t ruleId);
   size_t defineOptionalGrammarRule(ref_t ruleId, size_t nonterminal);
   size_t defineChomskiGrammarRule(ref_t ruleId, size_t nonterminal, size_t nonterminal2);
   size_t defineStarGrammarRule(ref_t ruleId, size_t nonterminal);
   size_t definePlusGrammarRule(ref_t ruleId, size_t nonterminal);
   size_t defineGrammarBrackets(_ScriptReader& reader, ScriptBookmark& bm, ref_t ruleId);
   size_t defineGrammarRuleMember(_ScriptReader& reader, ScriptBookmark& bm, ref_t ruleId, size_t nonterminal = 0, size_t terminal = 0);

   void defineGrammarRuleMemberPostfix(_ScriptReader& reader, ScriptBookmark& bm, Rule& rule, ref_t ruleId);
   void defineGrammarRuleMember(_ScriptReader& reader, ScriptBookmark& bm, Rule& rule, ref_t ruleId, int& applyMode);
   void defineGrammarRule(_ScriptReader& reader, ScriptBookmark& bm, Rule& rule, ref_t ruleId);

   void predict(DerivationQueue& queue, DerivationItem item, _ScriptReader& reader, ScriptBookmark& bm, int terminalOffset, MemoryWriter& writer);
   int buildDerivationTree(_ScriptReader& reader, size_t startRuleId, MemoryWriter& writer);
   void generateOutput(int offset, _ScriptReader& reader, ScriptLog& log);

   void insertForwards(Stack<Pair<int, int>>& forwards, int level, ScriptLog& log);

public:
   void readScriptBookmark(size_t ptr, ScriptBookmark& bm);

   bool compareToken(_ScriptReader& reader, ScriptBookmark& bm, int rule);
   bool compareTokenWithAny(_ScriptReader& reader, ScriptBookmark& bm, int rule);

   virtual bool setPostfix(ident_t)
   {
      return false;
   }

   virtual bool parseGrammarRule(_ScriptReader& reader);

   virtual void parse(_ScriptReader& reader, MemoryDump* output);

   virtual bool parseGrammarMode(_ScriptReader& reader);

   CFParser(_Parser* baseParser)
      : _table(Rule())
   {
      _baseParser = baseParser;

      // all body pointers should be greater than zero
      _body.writeDWord(0, 0);

      _symbolMode = false;
   }

   virtual ~CFParser()
   {
      freeobj(_baseParser);
   }
};

} // _ELENA_

#endif // cfparserH
