//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This file contains ELENA Engine Byte code Build Tree classes
//
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef BUILDTREE_H
#define BUILDTREE_H

#include "elena.h"
#include "tree.h"

namespace elena_lang
{
   enum class BuildKey
   {
      None              = 0x0000,

      CollectionMask    = 0x1000,

      Root              = 0x1000,
      Symbol            = 0x1001,
      Class             = 0x1002,
      Method            = 0x1003,
      Tape              = 0x1004,

      OpenFrame         = 0x0001,
      CloseFrame        = 0x0002,
      NilReference      = 0x0003,
      SymbolCall        = 0x0004,
      ClassReference    = 0x0005,
      CallOp            = 0x0006,      // NOTE : the stack for the argument list should be preallocated
      Exit              = 0x0007,
      SavingInStack     = 0x0008,
      Assigning         = 0x0009,
      Local             = 0x000A,
      CreatingClass     = 0x000B,

      Import            = 0x0028,
      StrDictionaryOp   = 0x0029,
      ObjArrayOp        = 0x002A,

      //Value             = 0x8001,
      Reserved          = 0x8002,      // reserved managed
      ReservedN         = 0x8003,      // reserved unmanaged
      Index             = 0x8004,
      Type              = 0x8005,

      //IntLiteral        = 0x0020,
      //StringLiteral     = 0x0021,
      //MetaDictionary    = 0x0022,
      //MetaArray         = 0x1023,

   };

   // --- BuildTree ---

   class BuildTree : public Tree<BuildKey, BuildKey::None>
   {
   public:
      static pos_t countChildren(Tree<BuildKey, BuildKey::None>::Node node, BuildKey mask)
      {
         pos_t counter = 0;
         auto current = node.firstChild(mask);
         while (current != BuildKey::None) {
            counter++;

            current = current.nextNode(mask);
         }

         return counter;
      }
   };

   // --- BuildTreeWriter ---
   typedef Tree<BuildKey, BuildKey::None>::Writer BuildTreeWriter;
   typedef Tree<BuildKey, BuildKey::None>::Node   BuildNode;

}

#endif
