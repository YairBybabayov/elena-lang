//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA VM Script Engine
//
//                                             (C)2011-2015, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef cfparserH
#define cfparserH 1

//////#include "elena.h"
#include "session.h"

namespace _ELENA_
{

// --- CFPrarser ---

class CFParser : public _Parser
{
public:
   // --- TokenInfo ---

   struct TokenInfo
   {
      CFParser*        parser;
      ScriptLog*       buffer;

      char             state;
      const wchar16_t* value;
      size_t           row, column;

      void read(_ScriptReader& reader)
      {
         value = reader.read();
         state = reader.info.state;
         row = reader.info.row;
         column = reader.info.column;
      }

//      void copyTo(Terminal* terminal)
//      {
//         size_t length = getlength(value);
//         StringHelper::copy(terminal->value, value, length);
//         terminal->value[length] = 0;
//
//         terminal->col = column;
//         terminal->row = row;
//         terminal->state = state;
//      }
//
//      bool empty()
//      {
//         return emptystr(value);
//      }
   
      bool compare(const wchar16_t* param)
      {
         return StringHelper::compare(value, param);
      }
   
      bool compare(const char* param)
      {
         return ConstantIdentifier::compare(value, param);
      }
   
      bool compare(size_t param)
      {
         const wchar_t* s = parser->getBodyText(param);

         return StringHelper::compare(value, s);
      }
   
//      const wchar16_t* resolvePtr(size_t param)
//      {
//         return parser->getBodyText(param);
//      }

      void writeLog()
      {
         if (state == _ELENA_TOOL_::dfaQuote) {
            buffer->write('\"');
            for (size_t i = 0 ; i < getlength(value) ; i++) {
               buffer->write(value[i]);
               if (value[i]=='"') {
                  buffer->write(value[i]);
               }
            }
            buffer->write('\"');
         }
         else buffer->write(value);
      }

      const wchar16_t* getLog()
      {
         if (buffer->Length() > 0) {            
            buffer->write((wchar16_t)0);

            return (const wchar16_t*)buffer->getBody();
         }
         else return NULL;
      }

      void clearLog()
      {
         buffer->clear();
      }

      void trimLog(size_t position)
      {
         buffer->trim(position);
      }

      size_t LogPosition() const
      {
         return buffer->Position();
      }

      void save(TokenInfo& token)
      {
         token.parser = parser;
         token.buffer = buffer;
         token.row = row;
         token.column = column;
         token.value = value;
         token.state = state;
      }

      TokenInfo(CFParser* parser, ScriptLog* log)
      {
         this->parser = parser;
         this->buffer = log;
         row = column = 0;
         value = NULL;
         state = 0;
      }
      TokenInfo(TokenInfo& token)
      {
         parser = token.parser;
         this->buffer = token.buffer;
         row = token.row;
         column = token.column;
         value = token.value;
         state = token.state;
      }
   };
   
   enum RuleType
   {
      rtNormal,
      rtChomski,
      rtLiteral,
      rtNumeric,
      rtReference,
      rtIdentifier,
//      rtAny,
      rtEps,
//      rtEof
   };

   // --- Rule ---
   struct Rule
   {
      size_t terminal;    // in chomski form it could be additional nonterminal as well
      size_t nonterminal;

      size_t prefixPtr;
      size_t postfixPtr;

      bool(*apply)(CFParser::Rule& rule, CFParser::TokenInfo& token, _ScriptReader& reader);

      void applyPrefixDSARule(CFParser::TokenInfo& token)
      {
         token.parser->writeDSARule(token, prefixPtr);
      }

      void applyPostfixDSARule(CFParser::TokenInfo& token)
      {
         token.parser->writeDSARule(token, postfixPtr);
      }

      Rule()
      {
         terminal = 0;
         nonterminal = 0;
         prefixPtr = 0;
         postfixPtr = 0;
      }
   };

   friend struct TokenInfo;

   typedef MemoryMap<size_t, Rule>             RuleMap;
   typedef MemoryMap<const wchar16_t*, size_t> NameMap;

protected:
   bool       _symbolMode;
   NameMap    _names;
   RuleMap    _rules;
   MemoryDump _body;

   size_t mapRuleId(const wchar16_t* name)
   {
      return mapKey(_names, name, _names.Count() + 1);
   }

   void defineApplyRule(Rule& rule, RuleType type);

   size_t writeBodyText(const wchar16_t* text);
   const wchar16_t* getBodyText(size_t ptr);

   size_t defineDSARule(TokenInfo& token, _ScriptReader& reader);

   void writeDSARule(TokenInfo& token, size_t ptr);

//   void compile(TokenInfo& token, CachedScriptReader& reader, _ScriptCompiler* compiler);

   size_t defineGrammarRule(TokenInfo& token, _ScriptReader& reader, size_t nonterminal = 0);
   void defineGrammarRule(TokenInfo& token, _ScriptReader& reader, Rule& rule);

   void saveScript(TokenInfo& token, _ScriptReader& reader, Rule& rule);

public:
   bool applyRule(Rule& rule, TokenInfo& token, _ScriptReader& reader);
   bool applyRule(size_t ruleId, TokenInfo& token, _ScriptReader& reader);

   virtual void parse(_ScriptReader& reader, ScriptLog& log);
   virtual void parseGrammarRule(_ScriptReader& reader);
   virtual void parseDirective(_ScriptReader& reader);

   CFParser()
      : _rules(Rule())
   {
      // all body pointers should be greater than zero
      _symbolMode = false;
      _body.writeDWord(0, 0);
   }
};

} // _ELENA_

#endif // cfparserH
