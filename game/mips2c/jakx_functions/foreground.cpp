//--------------------------MIPS2C---------------------
// clang-format off
#include "game/mips2c/mips2c_private.h"
#include "game/kernel/jakx/kscheme.h"
using ::jakx::intern_from_c;
namespace Mips2C::jakx {
namespace foreground_check_longest_edge_asm {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* view_get_active_math_camera; // view-get-active-math-camera
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  bool cop1_bc = false;
  float acc;
  c->daddiu(sp, sp, -64);                           // daddiu sp, sp, -64
  c->sd(ra, 0, sp);                                 // sd ra, 0(sp)
  c->sq(s4, 16, sp);                                // sq s4, 16(sp)
  c->sq(s5, 32, sp);                                // sq s5, 32(sp)
  c->sq(gp, 48, sp);                                // sq gp, 48(sp)
  c->mov64(s4, a0);                                 // or s4, a0, r0
  c->mov64(s5, a1);                                 // or s5, a1, r0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->mov64(gp, s7);                                 // or gp, s7, r0
  c->load_symbol2(t9, cache.view_get_active_math_camera);// lw t9, view-get-active-math-camera(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->mov64(v1, v0);                                 // or v1, v0, r0
  c->lwc1(f0, 124, s4);                             // lwc1 f0, 124(s4)
  c->lwc1(f7, 9376, at);                            // lwc1 f7, 9376(at)
  c->lwc1(f2, 9380, at);                            // lwc1 f2, 9380(at)
  c->lwc1(f5, 9384, at);                            // lwc1 f5, 9384(at)
  c->lwc1(f4, 184, s4);                             // lwc1 f4, 184(s4)
  c->lwc1(f12, 0, v1);                              // lwc1 f12, 0(v1)
  c->lwc1(f11, 64, v1);                             // lwc1 f11, 64(v1)
  c->mtc1(f1, s5);                                  // mtc1 f1, s5
  c->mtc1(f9, r0);                                  // mtc1 f9, r0
  c->mtc1(f10, r0);                                 // mtc1 f10, r0
  c->mtc1(f3, r0);                                  // mtc1 f3, r0
  c->mtc1(f3, r0);                                  // mtc1 f3, r0
  c->mtc1(f6, r0);                                  // mtc1 f6, r0
  c->mtc1(f8, r0);                                  // mtc1 f8, r0
  c->lui(a0, 16256);                                // lui a0, 16256
  c->mtc1(f6, a0);                                  // mtc1 f6, a0
  cop1_bc = c->fprs[f1] < c->fprs[f11];             // c.lt.s f1, f11
  bc = cop1_bc;                                     // bc1t L102
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_37;}                          // branch non-likely

  c->lwc1(f11, 12, v1);                             // lwc1 f11, 12(v1)
  c->lwc1(f13, 16, v1);                             // lwc1 f13, 16(v1)
  c->muls(f11, f11, f12);                           // mul.s f11, f11, f12
  c->muls(f13, f13, f12);                           // mul.s f13, f13, f12
  c->subs(f14, f5, f4);                             // sub.s f14, f5, f4
  cop1_bc = c->fprs[f12] < c->fprs[f14];            // c.lt.s f12, f14
  bc = !cop1_bc;                                    // bc1f L86
  c->lwc1(f12, 60, v1);                             // lwc1 f12, 60(v1)
  if (bc) {goto block_3;}                           // branch non-likely

  c->muls(f12, f14, f12);                           // mul.s f12, f14, f12
  cop1_bc = c->fprs[f0] < c->fprs[f12];             // c.lt.s f0, f12
  bc = cop1_bc;                                     // bc1t L101
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_36;}                          // branch non-likely

  
block_3:
  c->subs(f14, f2, f4);                             // sub.s f14, f2, f4
  cop1_bc = c->fprs[f13] < c->fprs[f14];            // c.lt.s f13, f14
  bc = !cop1_bc;                                    // bc1f L87
  c->lwc1(f12, 56, v1);                             // lwc1 f12, 56(v1)
  if (bc) {goto block_5;}                           // branch non-likely

  //beq r0, r0, L88                                 // beq r0, r0, L88
  c->muls(f10, f14, f12);                           // mul.s f10, f14, f12
  goto block_7;                                     // branch always

  
block_5:
  c->adds(f14, f2, f4);                             // add.s f14, f2, f4
  c->negs(f13, f13);                                // neg.s f13, f13
  cop1_bc = c->fprs[f14] < c->fprs[f13];            // c.lt.s f14, f13
  bc = !cop1_bc;                                    // bc1f L88
  c->negs(f13, f14);                                // neg.s f13, f14
  if (bc) {goto block_7;}                           // branch non-likely

  c->muls(f10, f13, f12);                           // mul.s f10, f13, f12
  
block_7:
  cop1_bc = c->fprs[f0] < c->fprs[f10];             // c.lt.s f0, f10
  bc = cop1_bc;                                     // bc1t L101
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_36;}                          // branch non-likely

  c->subs(f12, f7, f4);                             // sub.s f12, f7, f4
  cop1_bc = c->fprs[f11] < c->fprs[f12];            // c.lt.s f11, f12
  bc = !cop1_bc;                                    // bc1f L89
  c->lwc1(f10, 52, v1);                             // lwc1 f10, 52(v1)
  if (bc) {goto block_10;}                          // branch non-likely

  //beq r0, r0, L90                                 // beq r0, r0, L90
  c->muls(f9, f12, f10);                            // mul.s f9, f12, f10
  goto block_12;                                    // branch always

  
block_10:
  c->adds(f12, f7, f4);                             // add.s f12, f7, f4
  c->negs(f11, f11);                                // neg.s f11, f11
  cop1_bc = c->fprs[f12] < c->fprs[f11];            // c.lt.s f12, f11
  bc = !cop1_bc;                                    // bc1f L90
  c->negs(f11, f12);                                // neg.s f11, f12
  if (bc) {goto block_12;}                          // branch non-likely

  c->muls(f9, f11, f10);                            // mul.s f9, f11, f10
  
block_12:
  cop1_bc = c->fprs[f0] < c->fprs[f9];              // c.lt.s f0, f9
  bc = cop1_bc;                                     // bc1t L101
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_36;}                          // branch non-likely

  c->abss(f14, f7);                                 // abs.s f14, f7
  c->movs(f12, f5);                                 // mov.s f12, f5
  acc = c->fprs[f14] * c->fprs[f14];                // mula.s f14, f14
  c->fprs[f15] = acc + c->fprs[f12] * c->fprs[f12]; // madd.s f15, f12, f12
  c->lwc1(f9, 76, v1);                              // lwc1 f9, 76(v1)
  c->lwc1(f13, 80, v1);                             // lwc1 f13, 80(v1)
  c->lwc1(f10, 84, v1);                             // lwc1 f10, 84(v1)
  c->lwc1(f11, 88, v1);                             // lwc1 f11, 88(v1)
  c->fprs[f16] = c->fprs[f6] / (std::sqrt(std::abs(c->fprs[f15]))); // rsqrt.s f16, f6, f15
  c->muls(f15, f14, f16);                           // mul.s f15, f14, f16
  c->muls(f16, f12, f16);                           // mul.s f16, f12, f16
  acc = c->fprs[f9] * c->fprs[f16];                // mula.s f9, f16
  c->fprs[f12] = acc - c->fprs[f13] * c->fprs[f15]; // msub.s f12, f13, f15
  acc = c->fprs[f10] * c->fprs[f16];                // mula.s f10, f16
  c->fprs[f14] = acc - c->fprs[f11] * c->fprs[f15]; // msub.s f14, f11, f15
  acc = c->fprs[f9] * c->fprs[f15];                // mula.s f9, f15
  c->fprs[f9] = acc + c->fprs[f13] * c->fprs[f16]; // madd.s f9, f13, f16
  acc = c->fprs[f10] * c->fprs[f15];                // mula.s f10, f15
  c->fprs[f10] = acc + c->fprs[f11] * c->fprs[f16]; // madd.s f10, f11, f16
  cop1_bc = c->fprs[f8] < c->fprs[f12];             // c.lt.s f8, f12
  bc = cop1_bc;                                     // bc1t L91
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_17;}                          // branch non-likely

  cop1_bc = c->fprs[f8] < c->fprs[f14];             // c.lt.s f8, f14
  bc = cop1_bc;                                     // bc1t L92
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_18;}                          // branch non-likely

  cop1_bc = c->fprs[f8] < c->fprs[f9];              // c.lt.s f8, f9
  bc = cop1_bc;                                     // bc1t L93
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_19;}                          // branch non-likely

  //beq r0, r0, L95                                 // beq r0, r0, L95
  // nop                                            // sll r0, r0, 0
  goto block_23;                                    // branch always

  
block_17:
  //beq r0, r0, L95                                 // beq r0, r0, L95
  c->divs(f3, f1, f10);                             // div.s f3, f1, f10
  goto block_23;                                    // branch always

  
block_18:
  c->negs(f3, f12);                                 // neg.s f3, f12
  c->divs(f3, f3, f9);                              // div.s f3, f3, f9
  c->divs(f7, f14, f10);                            // div.s f7, f14, f10
  c->adds(f3, f7, f3);                              // add.s f3, f7, f3
  //beq r0, r0, L95                                 // beq r0, r0, L95
  c->muls(f3, f3, f1);                              // mul.s f3, f3, f1
  goto block_23;                                    // branch always

  
block_19:
  c->subs(f8, f7, f4);                              // sub.s f8, f7, f4
  c->adds(f10, f7, f4);                             // add.s f10, f7, f4
  c->negs(f11, f7);                                 // neg.s f11, f7
  cop1_bc = c->fprs[f7] < c->fprs[f8];              // c.lt.s f7, f8
  bc = cop1_bc;                                     // bc1t L94
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_22;}                          // branch non-likely

  cop1_bc = c->fprs[f10] < c->fprs[f11];            // c.lt.s f10, f11
  bc = cop1_bc;                                     // bc1t L94
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_22;}                          // branch non-likely

  //beq r0, r0, L95                                 // beq r0, r0, L95
  // nop                                            // sll r0, r0, 0
  goto block_23;                                    // branch always

  
block_22:
  c->negs(f3, f12);                                 // neg.s f3, f12
  c->muls(f3, f1, f3);                              // mul.s f3, f1, f3
  c->divs(f3, f3, f9);                              // div.s f3, f3, f9
  
block_23:
  cop1_bc = c->fprs[f0] < c->fprs[f3];              // c.lt.s f0, f3
  bc = cop1_bc;                                     // bc1t L101
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_36;}                          // branch non-likely

  c->abss(f10, f2);                                 // abs.s f10, f2
  c->movs(f12, f5);                                 // mov.s f12, f5
  acc = c->fprs[f10] * c->fprs[f10];                // mula.s f10, f10
  c->fprs[f9] = acc + c->fprs[f12] * c->fprs[f12]; // madd.s f9, f12, f12
  c->lwc1(f7, 96, v1);                              // lwc1 f7, 96(v1)
  c->lwc1(f8, 100, v1);                             // lwc1 f8, 100(v1)
  c->lwc1(f5, 104, v1);                             // lwc1 f5, 104(v1)
  c->fprs[f6] = c->fprs[f6] / (std::sqrt(std::abs(c->fprs[f9]))); // rsqrt.s f6, f6, f9
  c->lwc1(f11, 108, v1);                            // lwc1 f11, 108(v1)
  c->mtc1(f9, r0);                                  // mtc1 f9, r0
  c->muls(f13, f10, f6);                            // mul.s f13, f10, f6
  c->muls(f14, f12, f6);                            // mul.s f14, f12, f6
  acc = c->fprs[f7] * c->fprs[f14];                // mula.s f7, f14
  c->fprs[f10] = acc - c->fprs[f8] * c->fprs[f13]; // msub.s f10, f8, f13
  acc = c->fprs[f5] * c->fprs[f14];                // mula.s f5, f14
  c->fprs[f12] = acc - c->fprs[f11] * c->fprs[f13]; // msub.s f12, f11, f13
  acc = c->fprs[f7] * c->fprs[f13];                // mula.s f7, f13
  c->fprs[f6] = acc + c->fprs[f8] * c->fprs[f14]; // madd.s f6, f8, f14
  acc = c->fprs[f5] * c->fprs[f13];                // mula.s f5, f13
  c->fprs[f5] = acc + c->fprs[f11] * c->fprs[f14]; // madd.s f5, f11, f14
  cop1_bc = c->fprs[f9] < c->fprs[f10];             // c.lt.s f9, f10
  bc = cop1_bc;                                     // bc1t L96
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_28;}                          // branch non-likely

  cop1_bc = c->fprs[f9] < c->fprs[f12];             // c.lt.s f9, f12
  bc = cop1_bc;                                     // bc1t L97
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_29;}                          // branch non-likely

  cop1_bc = c->fprs[f9] < c->fprs[f6];              // c.lt.s f9, f6
  bc = cop1_bc;                                     // bc1t L98
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_30;}                          // branch non-likely

  //beq r0, r0, L100                                // beq r0, r0, L100
  // nop                                            // sll r0, r0, 0
  goto block_34;                                    // branch always

  
block_28:
  //beq r0, r0, L100                                // beq r0, r0, L100
  c->divs(f1, f1, f5);                              // div.s f1, f1, f5
  goto block_34;                                    // branch always

  
block_29:
  c->negs(f2, f10);                                 // neg.s f2, f10
  c->divs(f2, f2, f6);                              // div.s f2, f2, f6
  c->divs(f4, f12, f5);                             // div.s f4, f12, f5
  c->adds(f2, f4, f2);                              // add.s f2, f4, f2
  //beq r0, r0, L100                                // beq r0, r0, L100
  c->muls(f1, f2, f1);                              // mul.s f1, f2, f1
  goto block_34;                                    // branch always

  
block_30:
  c->subs(f5, f2, f4);                              // sub.s f5, f2, f4
  c->adds(f4, f2, f4);                              // add.s f4, f2, f4
  c->negs(f7, f2);                                  // neg.s f7, f2
  cop1_bc = c->fprs[f2] < c->fprs[f5];              // c.lt.s f2, f5
  bc = cop1_bc;                                     // bc1t L99
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_33;}                          // branch non-likely

  cop1_bc = c->fprs[f4] < c->fprs[f7];              // c.lt.s f4, f7
  bc = cop1_bc;                                     // bc1t L99
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_33;}                          // branch non-likely

  //beq r0, r0, L100                                // beq r0, r0, L100
  // nop                                            // sll r0, r0, 0
  goto block_34;                                    // branch always

  
block_33:
  c->negs(f2, f10);                                 // neg.s f2, f10
  c->muls(f1, f1, f2);                              // mul.s f1, f1, f2
  c->divs(f1, f1, f6);                              // div.s f1, f1, f6
  
block_34:
  cop1_bc = c->fprs[f0] < c->fprs[f3];              // c.lt.s f0, f3
  bc = cop1_bc;                                     // bc1t L101
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_36;}                          // branch non-likely

  //beq r0, r0, L102                                // beq r0, r0, L102
  // nop                                            // sll r0, r0, 0
  goto block_37;                                    // branch always

  
block_36:
  c->daddiu(gp, s7, 4);                             // daddiu gp, s7, #t
  
block_37:
  c->mov64(v0, gp);                                 // or v0, gp, r0
  c->ld(ra, 0, sp);                                 // ld ra, 0(sp)
  c->lq(gp, 48, sp);                                // lq gp, 48(sp)
  c->lq(s5, 32, sp);                                // lq s5, 32(sp)
  c->lq(s4, 16, sp);                                // lq s4, 16(sp)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 64);                            // daddiu sp, sp, 64
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  cache.view_get_active_math_camera = intern_from_c(-1, 0, "view-get-active-math-camera").c();
  gLinkedFunctionTable.reg("foreground-check-longest-edge-asm", execute, 128);
}

} // namespace foreground_check_longest_edge_asm
} // namespace Mips2C
// add foreground_check_longest_edge_asm::link to the link callback table for the object file.
