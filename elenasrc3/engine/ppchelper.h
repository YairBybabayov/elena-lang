//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Engine
//
//		This file contains CPU native helpers
//		Supported platforms: PPC64
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef PPCHELPER_H
#define PPCHELPER_H

namespace elena_lang
{
   // --- PPCOperand ---
   enum class PPCOperandType : unsigned int
   {
      None,

      GPR0     = 0x0100,
      GPR1     = 0x0101,
      GPR2     = 0x0102,
      GPR3     = 0x0103,
      GPR4     = 0x0104,
      GPR5     = 0x0105,
      GPR6     = 0x0106,
      GPR7     = 0x0107,
      GPR8     = 0x0108,
      GPR9     = 0x0109,
      GPR10    = 0x010A,
      GPR11    = 0x010B,
      GPR12    = 0x010C,
      GPR13    = 0x010D,
      GPR14    = 0x010E,
      GPR15    = 0x010F,
      GPR16    = 0x0110,
      GPR17    = 0x0111,
      GPR18    = 0x0112,
      GPR19    = 0x0113,
      GPR20    = 0x0114,
      GPR21    = 0x0115,
      GPR22    = 0x0116,
      GPR23    = 0x0117,
      GPR24    = 0x0118,
      GPR25    = 0x0119,
      GPR26    = 0x011A,
      GPR27    = 0x011B,
      GPR28    = 0x011C,
      GPR29    = 0x011D,
      GPR30    = 0x011E,
      GPR31    = 0x011F,

      GPR      = 0x0100,
   };

   inline bool test(PPCOperandType type, PPCOperandType mask)
   {
      return (((unsigned int)type & (unsigned int)mask) == (unsigned int)mask);
   }

   // --- PPCOperand ---
   struct PPCOperand
   {
      PPCOperandType type;
      bool           rc;

      bool isGPR()
      {
         return test(type, PPCOperandType::GPR);
      }

      bool isRX()
      {
         return (type >= PPCOperandType::GPR0 && type <= PPCOperandType::GPR15);
      }

      PPCOperand()
      {
         type = PPCOperandType::None;
         rc = false;
      }

      PPCOperand(PPCOperandType type)
      {
         this->type = type;
         rc = false;
      }
   };

   // --- PPCHelper ---
   class PPCHelper
   {
   public:
      static unsigned int makeDSCommand(unsigned int opcode, PPCOperandType rs, PPCOperandType ra, int d)
      {
         return (opcode << 26) | (((unsigned int)rs & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16) | ((unsigned int)d & 0xFFFC);
      }
      static unsigned int makeDSCommandU(unsigned int opcode, PPCOperandType rs, PPCOperandType ra, int d)
      {
         return (opcode << 26) | (((unsigned int)rs & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16) 
            | ((d & 0x3FFF) << 2) | 1;
      }

      static unsigned int makeXFXCommand(unsigned int opcode, PPCOperandType rt, int form, int xopcode)
      {
         return (opcode << 26) | (((unsigned int)rt & 0x1F) << 21) | (form << 16) | (xopcode << 1);
      }

      static unsigned int makeACommand(unsigned int opcode, PPCOperandType rt, PPCOperandType ra, PPCOperandType rb,
         int bc, int xo)
      {
         return (opcode << 26) | (((unsigned int)rt & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16)
            | (((unsigned int)rb & 0x1F) << 11) | (((unsigned int)bc & 0x1F) << 6) | (xo << 1) | 0;
      }

      static unsigned int makeXCommand(unsigned int opcode, PPCOperandType rt, PPCOperandType ra, PPCOperandType rb,
         int xo, int rc)
      {
         return (opcode << 26) | (((unsigned int)rt & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16)
            | (((unsigned int)rb & 0x1F) << 11) | (xo << 1) | rc;
      }

      static unsigned int makeDCommand(unsigned int opcode, PPCOperandType rs, PPCOperandType ra, int d)
      {
         return (opcode << 26) | (((unsigned int)rs & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16)
            | ((unsigned int)d & 0xFFFF);
      }
      static unsigned int makeDCommand(unsigned int opcode, int bf, int l, PPCOperandType ra, int d)
      {
         return (opcode << 26) | (bf << 23) | (l << 21) | (((unsigned int)ra & 0x1F) << 16)
            | ((unsigned int)d & 0xFFFF);
      }

      static unsigned int makeMDCommand(unsigned int opcode, PPCOperandType rs, PPCOperandType ra, int sh, int mb,
         int xo, int sh_hi, int rc)
      {
         return (opcode << 26) | (((unsigned int)rs & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16)
            | (sh << 11) | (mb << 5) | (xo << 2) | (sh_hi << 1) | rc;
      }

      static unsigned int makeXLCommand(unsigned int opcode, int bo, int bi, int bh, int xo, int lk)
      {
         return (opcode << 26) | (bo << 21) | (bi << 16) | (bh << 11) | (xo << 1) | lk;
      }
      static unsigned int makeBCommand(unsigned int opcode, int bo, int bi, int bd, int aa, int lk)
      {
         return (opcode << 26) | (bo << 21) | (bi << 16) | ((bd & 0xFFFF) << 2) | (aa << 1) | lk;
      }
      static unsigned int makeXOCommand(unsigned int opcode, PPCOperandType rt, PPCOperandType ra, PPCOperandType rb, 
         int oe, int xo, int rc)
      {
         return (opcode << 26) | (((unsigned int)rt & 0x1F) << 21) | (((unsigned int)ra & 0x1F) << 16)
            | (((unsigned int)rb & 0x1F) << 11) | (oe << 10) | (xo << 1) | rc;
      }
      static unsigned int makeICommand(unsigned int opcode, int li, int aa, int lk)
      {
         return (opcode << 26) | ((li & 0xFFFFFF) << 2) | (aa << 1) | lk;
      }
      static unsigned int makeXCommand(unsigned int opcode, int bc, int l, PPCOperandType ra, PPCOperandType rb,
         int xo)
      {
         return (opcode << 26) | ((bc & 0x1F) << 23) | (l << 22) | (((unsigned int)ra & 0x1F) << 16)
            | (((unsigned int)rb & 0x1F) << 11) | (xo << 1) | 0;
      }

      static void fixBCommand(void* opcode, int bd)
      {
         *(unsigned int*)opcode = *(unsigned int*)opcode | bd;
      }
   };

}

#endif
