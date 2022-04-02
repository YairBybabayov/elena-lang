// ; --- Predefined References  --
define INVOKER               10001h
define GC_ALLOC	             10002h

define CORE_TOC              20001h
define SYSTEM_ENV            20002h
define CORE_GC_TABLE         20003h

// ; --- Object header fields ---
define elSizeOffset          0004h
define elVMTOffset           0010h 
define elObjectOffset        0010h

// ; --- VMT header fields ---
define elVMTSizeOffset       0008h
define elVMTFlagOffset       0018h
define elPackageOffset       0020h

// ; --- GC TABLE OFFSETS ---
define gc_header             0000h
define gc_start              0008h
define gc_yg_current         0010h
define gc_yg_current         0018h
define gc_yg_end             0020h
define gc_mg_start           0038h
define gc_mg_current         0040h
define gc_end                0048h
define gc_mg_wbar            0050h

// ; --- System Core Preloaded Routines --

structure % CORE_TOC

  dq 0         // ; address of import section

end
 
structure %CORE_GC_TABLE

  dq 0 // ; gc_header             : +00h
  dq 0 // ; gc_start              : +08h
  dq 0 // ; gc_yg_start           : +10h
  dq 0 // ; gc_yg_current         : +18h
  dq 0 // ; gc_yg_end             : +20h
  dq 0 // ; gc_shadow             : +28h
  dq 0 // ; gc_shadow_end         : +30h
  dq 0 // ; gc_mg_start           : +38h
  dq 0 // ; gc_mg_current         : +40h
  dq 0 // ; gc_end                : +48h
  dq 0 // ; gc_mg_wbar            : +50h

end

// ; NOTE : the table is tailed with GCMGSize,GCYGSize and MaxThread fields
structure %SYSTEM_ENV

  dq data : %CORE_GC_TABLE
  dq code : %INVOKER

end

// ; --- GC_ALLOC ---
// ; in: x11 - size ; out: x10 - created object
inline % GC_ALLOC

  movz    x12,  data_ptr32lo : %CORE_GC_TABLE
  movk    x12,  data_ptr32hi : %CORE_GC_TABLE, lsl #16
  add     x13, x12, gc_yg_current
  ldr     x15, [x13]
  add     x14, x12, gc_yg_end
  ldr     x14, [x14]
  add     x11, x11, x15
  cmp     x11, x14
  bge     labYGCollect
  str     x14, [x13]
  add     x10, x15, elObjectOffset
  ret     x30

labYGCollect:
  mov     x10, #0                 //; ecx
  ret     x30

end
 
// ; ==== Command Set ==

// ; redirect
inline % 03h //; (r15 - object, r14 - message)

  sub     x14, x10, elVMTOffset              
  ldr     x11, [x14]              //; edi
  mov     x12, #0                 //; ecx
  sub     x15, x11, elVMTSizeOffset
  ldr     x13, [x15]              //; esi

labSplit:
  cmp     x13, #0 
  beq     labEnd

labStart:
  tst     x13, #1
  lsr     x13, x13, #1
  cset    x12, eq

  lsl     x14, x13, #4
  add     x14, x14, x11 

  ldr     x15, [x14]         //; edx
  cmp     x9, x15
  beq     labFound
  add     x11, x14, #16
  ble     labSplit
  sub     x13, x13, x12
  b       labSplit

labFound:
  add     x14, x14, #8 
  ldr     x15, [x14] 
  br      x15

labEnd:
                               
end

// ; quit
inline %4

  ret x30

end

// ; movenv
inline %5

  movz    x9,  rdata_ptr32lo : %SYSTEM_ENV
  movk    x9,  rdata_ptr32hi : %SYSTEM_ENV, lsl #16

end

// ; load
inline %6

  ldr    x9, [x10]

end

// ; setr
inline %80h

  movz    x10,  __ptr32lo_1
  movk    x10,  __ptr32hi_1, lsl #16

end 

// ; setr 0
inline %180h

  mov     x10, #0

end 

// ; setddisp
inline %81h

  add     x10, x29, __arg12_1

end 

// ; movm
inline %88h

  movz    x9,  __arg32lo_1
  movk    x9,  __arg32hi_1, lsl #16

end

// ; closen
inline %91h

  add     x29, x29, __arg12_1
  mov     sp, x29
  ldp     x29, x30, [sp], #16
  
end

// ; closen 0
inline %191h

  mov     sp, x29
  ldp     x29, x30, [sp], #16
  
end

// ; alloci
// ; NOTE : __arg1 should be aligned to 16
inline %92h

  sub     sp, sp,  __arg12_1   // ; allocate stack
  mov     x11, __arg12_1
  mov     x12, 0
  mov     x13, sp

labLoop:
  cmp     x11, 0
  beq     labEnd
  sub     x11, x11, 8
  str     x12, [x13], #8
  b       labLoop

labEnd:

end

// ; alloci 0
inline %192h

end

// ; alloci 1
inline %292h

  mov     x12, #0
  str     x12, [sp, #-8]!

end

// ; alloci 2
inline %392h

  mov     x12, #0
  stp     x12, x12, [sp, #-16]!

end

// ; alloci 3
inline %492h

  mov     x12, #0
  stp     x12, x12, [sp], #8
  str     x12, [sp], #16

end

// ; alloci 4
inline %592h

  stp     x12,x12, [sp], #16
  stp     x12,x12, [sp], #16

end

// ; freei
// ; NOTE : __arg1 should be aligned to 16
inline %93h

  add     sp, sp, __arg12_1

end

// ; saveddisp
inline %0A0h

  add     x11, x29, __arg12_1
  str     x9, [x11]        

end

// ; storefi
// ; NOTE : it is presumed that arg1 < 0 (it is inverted in jitcompiler)
inline %0A1h

  sub     x11, x29, -__arg12_1
  str     x10, [x11]

end

// ; storefi
// ; NOTE : it is presumed that arg1 > 0 (it is inverted in jitcompiler)
inline %05A1h

  add     x11, x29, __arg12_1
  str     x10, [x11]

end

// ; savesi
inline %0A2h

  add     x11, sp, __arg12_1
  str     x9, [x11]

end 

// ; savesi 0
inline %1A2h

  mov     x0, x9

end 

// ; savesi 1
inline %2A2h

  mov     x1, x9

end 

// ; storesi
inline %0A3h

  add     x11, sp, __arg12_1
  str     x10, [x11]

end 

// ; storesi 0
inline %1A3h

  mov     x0, x10

end 

// ; storesi 1
inline %2A3h

  mov     x1, x10

end 

// ; xflushsi i
inline %0A4h

end 

// ; xflushsi 0
inline %1A4h

  add     x11, sp, 8
  str     x0, [x11]

end 

// ; xflushsi 1
inline %2A4h

  add     x11, sp, 16
  str     x1, [x11]

end 

// ; peekfi
// ; NOTE : it is presumed that arg1 < 0 (it is inverted in jitcompiler)
inline %0A8h

  sub     x11, x29, -__arg12_1
  ldr     x10, [x11]

end 

// ; peekfi
// ; NOTE : it is presumed that arg1 > 0 (it is inverted in jitcompiler)
inline %5A8h

  add     x11, x29, __arg12_1
  ldr     x10, [x11]

end 

// ; callr
inline %0B0h

  movz    x17,  __ptr32lo_1
  movk    x17,  __ptr32hi_1, lsl #16
  blr     x17

end

// ; callvi
inline %0B1h

  sub     x14, x10, elVMTOffset              
  ldr     x17, [x14]
  add     x17, x17, __arg12_1
  ldr     x17, [x17]
  blr     x17

end

// ; openin
inline %0F0h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp, __n12_2 // ; allocate raw stack

  mov     x11, #0
  stp     x11, x29, [sp, #-16]! 

  mov     x29, sp              // ; set frame pointer

  sub     sp, sp,  __arg12_1   // ; allocate stack
  mov     x11, __arg16_1
  mov     x12, #0
  mov     x13, sp

labLoop:
  cmp     x11, #0
  beq     labEnd
  sub     x11, x11, #8
  str     x12, [x13], #8
  b       labLoop

labEnd:

end 

// ; openin 0, 0
inline %1F0h

   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer

end 

// ; openin 1, 0
inline %2F0h

   mov     x11, 0
   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer
   stp     x11, x11, [sp, #-16]! 

end 

// ; openin 2, 0
inline %3F0h

   mov     x11, 0
   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer
   stp     x11, x11, [sp, #-16]! 

end 

// ; openin 3, 0
inline %4F0h

  mov     x11, 0
  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp              // ; set frame pointer
  stp     x11, x11, [sp, #-16]! 
  stp     x11, x11, [sp, #-16]! 

end 

// ; openin 0, n
inline %5F0h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp, __n12_2 // ; allocate raw stack

  mov     x11, #0
  stp     x11, x29, [sp, #-16]! 

  mov     x29, sp              // ; set frame pointer

end 

// ; openin i, 0
inline %6F0h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp,  __arg12_1   // ; allocate stack
  mov     x11, __arg16_1
  mov     x12, #0
  mov     x13, sp

labLoop:
  cmp     x11, #0
  beq     labEnd
  sub     x11, x11, #8
  str     x12, [x13], #8
  b       labLoop

labEnd:

end 

// ; xstoresir
inline %0F1h

  movz    x11,  __ptr32lo_2
  movk    x11,  __ptr32hi_2, lsl #16

  add     x12, sp, __arg12_1
  str     x11, [x12]

end

// ; xstoresir :0, ...
inline %1F1h

  movz    x0,  __ptr32lo_2
  movk    x0,  __ptr32hi_2, lsl #16

end

// ; xstoresir :1, ...
inline %2F1h

  movz    x1,  __ptr32lo_2
  movk    x1,  __ptr32hi_2, lsl #16

end

// ; openheaderin
inline %0F2h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp, __n12_2 // ; allocate raw stack

  mov     x11, #0
  stp     x11, x29, [sp, #-16]! 

  mov     x29, sp              // ; set frame pointer

  sub     sp, sp,  __arg12_1   // ; allocate stack
  mov     x11, __arg16_1
  mov     x12, #0
  mov     x13, sp

labLoop:
  cmp     x11, #0
  beq     labEnd
  sub     x11, x11, #8
  str     x12, [x13], #8
  b       labLoop

labEnd:

end 

// ; openheaderin 0, 0
inline %1F2h

   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer

end 

// ; openheaderin 1, 0
inline %2F2h

   mov     x11, #0
   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer
   stp     x11, x11, [sp, #-16]! 

end 

// ; openheaderin 2, 0
inline %3F2h

   mov     x11, #0
   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer
   stp     x11, x11, [sp, #-16]! 

end 

// ; openheaderin 3, 0
inline %4F2h

   mov     x11, #0
   stp     x29, x30, [sp, #-16]! 
   mov     x29, sp              // ; set frame pointer
   stp     x11, x11, [sp, #-16]! 
   stp     x11, x11, [sp, #-16]! 

end 

// ; openheaderin 0, n
inline %5F2h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp, __n12_2 // ; allocate raw stack

  mov     x11, #0
  stp     x11, x29, [sp, #-16]! 

  mov     x29, sp              // ; set frame pointer

end 

// ; openheaderin i, 0
inline %6F2h

  stp     x29, x30, [sp, #-16]! 
  mov     x29, sp

  sub     sp, sp,  __arg12_1   // ; allocate stack
  mov     x11, __arg16_1
  mov     x12, #0
  mov     x13, sp

labLoop:
  cmp     x11, #0
  beq     labEnd
  sub     x11, x11, #8
  str     x12, [x13], #8
  b       labLoop

labEnd:

end 

// ; movsifi
inline %0F3h

  add     x12, x29, __arg12_1
  add     x13, sp, __arg12_2

  ldr     x11, [x12]
  str     x11, [x13]

end

// ; movsifi sp:0, fp:i2
inline %1F3h

  add     x12, x29, __arg12_1
  ldr     x0, [x12]

end

// ; movsifi sp:1, fp:i2
inline %2F3h

  add     x12, x29, __arg12_1
  ldr     x1, [x12]

end

// ; newir i, r
inline %0F4h

  mov     x11, __arg16_1
  movz    x17,  code_ptr32lo : %GC_ALLOC
  movk    x17,  code_ptr32hi : %GC_ALLOC, lsl #16
  blr     x17
  mov     x18, __n16_1
  movz    x19,  __ptr32lo_2
  movk    x19,  __ptr32hi_2, lsl #16
  sub     x20, x10, elVMTOffset
  str     x19, [x20]
  str     w18, [x20, #12]!

end

// ; newnr i, r
inline %0F5h

  mov     x11, __arg16_1
  movz    x17,  code_ptr32lo : %GC_ALLOC
  movk    x17,  code_ptr32hi : %GC_ALLOC, lsl #16
  blr     x17
  movz    x18, __n16_1
  movk    x18, __n16hi_1, lsl #16
  movz    x19,  __ptr32lo_2
  movk    x19,  __ptr32hi_2, lsl #16
  sub     x20, x10, elVMTOffset
  str     x19, [x20]
  str     w18, [x20, #12]!

end

// ; xstorefir
inline %0F9h

  movz    x11,  __ptr32lo_2
  movk    x11,  __ptr32hi_2, lsl #16
  add     x12, x29, __arg12_1
  str     x11, [x12]

end

// ; callext
inline %0FEh

  movz    x16,  __ptr32lo_1
  movk    x16,  __ptr32hi_1, lsl #16
  ldr     x17, [x16]
  blr     x17

end
