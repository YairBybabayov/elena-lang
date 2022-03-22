//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This file contains ELENA compiler logic class.
//
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef COMPILERLOGIC_H
#define COMPILERLOGIC_H

#include "buildtree.h"
#include "clicommon.h"

namespace elena_lang
{
   // --- CompilerLogic ---
   class CompilerLogic
   {
   public:
      BuildKey resolveOp(ref_t* arguments, size_t length);

      bool validateTemplateAttribute(ref_t attribute, Visibility& visibility, TemplateType& type);
      bool validateSymbolAttribute(ref_t attribute, Visibility& visibility);
      bool validateClassAttribute(ref_t attribute, Visibility& visibility);
      bool validateMethodAttribute(ref_t attribute, MethodHint& hint, bool& explicitMode);
      bool validateImplicitMethodAttribute(ref_t attribute, MethodHint& hint);
      bool validateDictionaryAttribute(ref_t attribute, ref_t& dictionaryType);
      bool validateExpressionAttribute(ref_t attrValue, ExpressionAttributes& attrs);

      bool validateMessage(mssg_t message);
      void validateClassDeclaration();

      void writeDictionaryEntry(MemoryBase* section, ustr_t key, int value);
      bool readDictionary(MemoryBase* section, ReferenceMap& map);

      void writeArrayEntry(MemoryBase* section, ref_t reference);

      //void injectVirtualCode(ClassInfo& classInfo);

      static CompilerLogic* getInstance()
      {
         static CompilerLogic instance;

         return &instance;
      }
   };

}

#endif // COMPILERLOGIC_H