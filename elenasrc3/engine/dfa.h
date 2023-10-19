//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Compiler
//
//		This header contains DFA tables
//
//                                             (C)2022-2023, by Aleksey Rakov
//---------------------------------------------------------------------------

#ifndef DFA_H
#define DFA_H

namespace elena_lang
{
   const char* token_dfa[17] =
   {
        ".????????BB??B??????????????????BDFLND??QQDDDDDHEEEEEEEEEEDDDDDDDCCCCCCCCCCCCCCCCCCCCCCCCCCQ?DDC?CCCCCCCCCCCCCCCCCCCCCCCCCCDDDD?",
        "*********BB*********************B***********************************************************************************************",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMAAAAAAAACCCCCCCCCCAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCAAAACACCCCCCCCCCCCCCCCCCCCCCCCCCAAAAA",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAADDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEEEEEEEEEEAAAAAAAKKKKKKAAAAAAAAAAAAAAAAAAAAAAAAAAEEEEEEEJAAAAAAAAAAAAAAAAAAAAAAA",
        "?FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFGFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "*IIIIIIIII*II*IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "????????????????????????????????????????????????KKKKKKKKKK???????KKKKKK?????????????????????????????????J???????????????????????",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAALLLLLLLLLLAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCAAAACACCCCCCCCCCCCCCCCCCCCCCCCCCAAAAA",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMAAAAAAAA???????????A?????MMMMMMMMMMMMMMMMMMMMMMMMMM??????MMMMMMMMMMMMMMMMMMMMMMMMMM?????",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAANNNNNNNNNN?A?????NNNNNNNNNNNNNNNNNNNNNNNNNN??????NNNNNNNNNNNNNNNNNNNNNNNNNN?????",
        "????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????",
        "????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
   };

   const char* symbolic_dfa[4] =
   {
        ".????????DD??D??????????????????BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????",
        "*********DD**D******************************************************************************************************************"
   };

}
#endif