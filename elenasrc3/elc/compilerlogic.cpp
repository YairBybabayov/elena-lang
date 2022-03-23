//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This file contains ELENA compiler logic class implementation.
//
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#include "elena.h"
// --------------------------------------------------------------------------
#include "langcommon.h"
#include "compilerlogic.h"

using namespace elena_lang;

struct Op
{
   BuildKey operation;

   ref_t    loperand;
   ref_t    roperand;
   ref_t    ioperand;
};

constexpr auto OperationLength = 2;
constexpr Op Operations[OperationLength] =
{
   { BuildKey::StrDictionaryOp, V_DICTIONARY, V_INT32, V_STRING },
   { BuildKey::ObjArrayOp, V_OBJARRAY, V_OBJECT, 0 }
};

// --- CompilerLogic ---

BuildKey CompilerLogic :: resolveOp(ref_t* arguments, size_t length)
{
   for(size_t i = 0; i < OperationLength; i++) {
      if (arguments[0] == Operations[i].loperand && arguments[1] == Operations[i].roperand) {
         if ((length == 2) || (arguments[2] == Operations[i].ioperand))
            return Operations[i].operation;
      }
   }

   return BuildKey::None;
}

bool CompilerLogic :: validateTemplateAttribute(ref_t attribute, Visibility& visibility, TemplateType& type)
{
   switch (attribute) {
      case V_PUBLIC:
         visibility = Visibility::Public;
         break;
      case V_PRIVATE:
         visibility = Visibility::Private;
         break;
      case V_INLINE:
         type = TemplateType::Inline;
         break;
      default:
         return false;
   }

   return true;
}

bool CompilerLogic :: validateSymbolAttribute(ref_t attribute, Visibility& visibility)
{
   switch (attribute) {
      case V_PUBLIC:
         visibility = Visibility::Public;
         break;
      case V_PRIVATE:
         visibility = Visibility::Private;
         break;
      case V_SYMBOLEXPR:
         break;
      default:
         return false;
   }

   return true;
}

bool CompilerLogic :: validateClassAttribute(ref_t attribute, ref_t& flags, Visibility& visibility)
{
   switch (attribute) {
      case V_PUBLIC:
         visibility = Visibility::Public;
         break;
      case V_PRIVATE:
         visibility = Visibility::Private;
         break;
      case V_CLASS:
         break;
      case V_SINGLETON:
         flags = elRole | elSealed | elStateless;
         break;
      default:
         return false;
   }

   return true;
}

bool CompilerLogic :: validateMethodAttribute(ref_t attribute, MethodHint& hint, bool& explicitMode)
{
   switch (attribute) {
      case 0:
         break;
      case V_METHOD:
         explicitMode = true;
         break;
      case V_STATIC:
         hint = MethodHint::Static;
         break;
      case V_DISPATCHER:
         explicitMode = true;
         hint = MethodHint::Dispatcher;
         break;
      case V_CONSTRUCTOR:
         explicitMode = true;
         hint = MethodHint::Constructor;
         break;
      default:
         return false;
   }

   return true;
}

bool CompilerLogic::validateImplicitMethodAttribute(ref_t attribute, MethodHint& hint)
{
   bool dummy = false;
   switch (attribute) {
      case V_METHOD:
      case V_DISPATCHER:
      case V_CONSTRUCTOR:
         return validateMethodAttribute(attribute, hint, dummy);
      default:
         return false;
   }
}

bool CompilerLogic :: validateDictionaryAttribute(ref_t attribute, ref_t& dictionaryType)
{
   if (attribute == V_STRINGOBJ) {
      dictionaryType = V_STRINGOBJ;

      return true;
   }
   else return false;
}

bool CompilerLogic :: validateExpressionAttribute(ref_t attrValue, ExpressionAttributes& attrs)
{
   switch(attrValue) {
      case V_FORWARD:
         attrs |= ExpressionAttribute::Forward;
         return true;
      case V_INTERN:
         attrs |= ExpressionAttribute::Intern;
         return true;
      default:
         return false;
   }
}

bool CompilerLogic :: validateMessage(mssg_t message)
{
   return true;
}

void CompilerLogic :: validateClassDeclaration()
{
   
}

bool CompilerLogic :: isRole(ClassInfo& info)
{
   return test(info.header.flags, elRole);
}

void CompilerLogic :: tweakClassFlags(ClassInfo& info, bool classClassMode)
{
   if (classClassMode) {
      // class class is always stateless and final
      info.header.flags |= elStateless;
      info.header.flags |= elSealed;
   }
}

void CompilerLogic :: writeDictionaryEntry(MemoryBase* section, ustr_t key, int value)
{
   MemoryWriter writer(section);
   writer.writeString(key);
   writer.writeDWord(1);
   writer.writeDWord(value);
}

bool CompilerLogic :: readDictionary(MemoryBase* section, ReferenceMap& map)
{
   IdentifierString key;

   MemoryReader reader(section);
   while (!reader.eof()) {
      reader.readString(key);
      int type = reader.getDWord();

      if (type == 1) {
         ref_t value = reader.getRef();

         map.add(*key, value);
      }
      else return false;
   }

   return true;
}

void CompilerLogic :: writeArrayEntry(MemoryBase* section, ref_t reference)
{
   MemoryWriter writer(section);
   writer.writeRef(reference);
}

//void CompilerLogic :: injectVirtualCode(ClassInfo& classInfo)
//{
//   if (test(classInfo.header.flags, elClassClass)) {
//      
//   }
//   else {
//      
//   }
//}
