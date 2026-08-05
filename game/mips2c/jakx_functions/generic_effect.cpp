//--------------------------MIPS2C---------------------
// clang-format off
#include "game/mips2c/mips2c_private.h"
#include "game/kernel/jakx/kscheme.h"
using ::jakx::intern_from_c;
namespace Mips2C::jakx {

// vcallms helpers: the generic VU0 microprogram's effect at each call address,
// ported verbatim from jak1_functions/generic_effect.cpp (the jak1-era proc
// family around them is instruction-identical; parity checked at generation).

void vcallms0(ExecutionContext* c) {
  // this function does lighting calculations for 4 vertices.
  // the input colors (u8's) are in vf5, vf6, vf7, vf8

//  fmt::print("nromal:\n {}\n {}\n {}\n {}\n\n", c->vf_src(vf1).vf.print(), c->vf_src(vf2).vf.print(), c->vf_src(vf3).vf.print(), c->vf_src(vf4).vf.print());
//  //fmt::print("dir:\n {}\n {}\n {}\n\n", c->vf_src(vf10).vf.print(), c->vf_src(vf11).vf.print(), c->vf_src(vf12).vf.print());
//
//  // hack to see normals as colors
//  c->vfs[vf21].vf.move(Mask::xyzw, c->vf_src(vf17).vf);
//  c->vfs[vf22].vf.move(Mask::xyzw, c->vf_src(vf18).vf);
//  c->vfs[vf23].vf.move(Mask::xyzw, c->vf_src(vf19).vf);
//  c->vfs[vf24].vf.move(Mask::xyzw, c->vf_src(vf20).vf);
//  for (int vec = 0; vec < 4; vec++) {
//    for (int cmp = 0; cmp < 3; cmp++) {
////      c->vfs[vf17 + vec].f[cmp] = vec * 16 + cmp * 4;
//      u32 val = 128 + 127 * c->vfs[vf1 + vec].f[cmp];
//      fmt::print("{} ", val);
//      memcpy(&c->vfs[vf17 + vec].f[cmp], &val, 4);
//    }
//  }
//  fmt::print("\n");
//  return;

  // move.xyzw vf21, vf17       |  mulax.xyzw ACC, vf10, vf01
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf10).vf, c->vf_src(vf01).vf.x());   c->vfs[vf21].vf.move(Mask::xyzw, c->vf_src(vf17).vf);
  // move.xyzw vf22, vf18       |  madday.xyzw ACC, vf11, vf01
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf11].vf, c->vfs[vf01].vf.y());   c->vfs[vf22].vf.move(Mask::xyzw, c->vf_src(vf18).vf);
  // move.xyzw vf23, vf19       |  maddz.xyzw vf01, vf12, vf01
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf01].vf, c->vf_src(vf12).vf, c->vf_src(vf01).vf.z());   c->vfs[vf23].vf.move(Mask::xyzw, c->vf_src(vf19).vf);
  // move.xyzw vf24, vf20       |  mulax.xyzw ACC, vf10, vf02
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf10).vf, c->vf_src(vf02).vf.x());   c->vfs[vf24].vf.move(Mask::xyzw, c->vf_src(vf20).vf);
  // nop                        |  itof0.xyzw vf17, vf05
  c->vfs[vf17].vf.itof0(Mask::xyzw, c->vf_src(vf05).vf);
  // nop                        |  itof0.xyzw vf18, vf06
  c->vfs[vf18].vf.itof0(Mask::xyzw, c->vf_src(vf06).vf);
  // nop                        |  itof0.xyzw vf19, vf07
  c->vfs[vf19].vf.itof0(Mask::xyzw, c->vf_src(vf07).vf);
  // nop                        |  itof0.xyzw vf20, vf08
  c->vfs[vf20].vf.itof0(Mask::xyzw, c->vf_src(vf08).vf);

  //fmt::print("light in:\n {}\n {}\n {}\n {}\n\n", c->vf_src(vf17).vf.print(), c->vf_src(vf18).vf.print(), c->vf_src(vf19).vf.print(), c->vf_src(vf20).vf.print());

  // nop                        |  madday.xyzw ACC, vf11, vf02
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf11].vf, c->vfs[vf02].vf.y());
  // nop                        |  maddz.xyzw vf02, vf12, vf02
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf02].vf, c->vf_src(vf12).vf, c->vf_src(vf02).vf.z());
  // nop                        |  mulax.xyzw ACC, vf10, vf03
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf10).vf, c->vf_src(vf03).vf.x());
  // nop                        |  madday.xyzw ACC, vf11, vf03
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf11].vf, c->vfs[vf03].vf.y());
  // nop                        |  maddz.xyzw vf03, vf12, vf03
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf03].vf, c->vf_src(vf12).vf, c->vf_src(vf03).vf.z());
  // nop                        |  mulax.xyzw ACC, vf10, vf04
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf10).vf, c->vf_src(vf04).vf.x());
  // nop                        |  madday.xyzw ACC, vf11, vf04
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf11].vf, c->vfs[vf04].vf.y());
  // nop                        |  maddz.xyzw vf04, vf12, vf04
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf04].vf, c->vf_src(vf12).vf, c->vf_src(vf04).vf.z());
  // nop                        |  maxx.xyzw vf01, vf01, vf00
  c->vfs[vf01].vf.max(Mask::xyzw, c->vf_src(vf01).vf, c->vf_src(vf00).vf.x());
  // nop                        |  maxx.xyzw vf02, vf02, vf00
  c->vfs[vf02].vf.max(Mask::xyzw, c->vf_src(vf02).vf, c->vf_src(vf00).vf.x());
  // nop                        |  maxx.xyzw vf03, vf03, vf00
  c->vfs[vf03].vf.max(Mask::xyzw, c->vf_src(vf03).vf, c->vf_src(vf00).vf.x());
  // nop                        |  maxx.xyzw vf04, vf04, vf00
  c->vfs[vf04].vf.max(Mask::xyzw, c->vf_src(vf04).vf, c->vf_src(vf00).vf.x());
  // nop                        |  mulaw.xyzw ACC, vf13, vf00
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf13).vf, c->vf_src(vf00).vf.w());
  // nop                        |  maddax.xyzw ACC, vf14, vf01
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf14].vf, c->vfs[vf01].vf.x());
  // nop                        |  madday.xyzw ACC, vf15, vf01
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf15].vf, c->vfs[vf01].vf.y());
  // nop                        |  maddz.xyzw vf01, vf16, vf01
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf01].vf, c->vf_src(vf16).vf, c->vf_src(vf01).vf.z());
  // nop                        |  mulaw.xyzw ACC, vf13, vf00
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf13).vf, c->vf_src(vf00).vf.w());
  // nop                        |  maddax.xyzw ACC, vf14, vf02
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf14].vf, c->vfs[vf02].vf.x());
  // nop                        |  madday.xyzw ACC, vf15, vf02
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf15].vf, c->vfs[vf02].vf.y());
  // nop                        |  maddz.xyzw vf02, vf16, vf02
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf02].vf, c->vf_src(vf16).vf, c->vf_src(vf02).vf.z());
  // nop                        |  mulaw.xyzw ACC, vf13, vf00
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf13).vf, c->vf_src(vf00).vf.w());
  // nop                        |  maddax.xyzw ACC, vf14, vf03
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf14].vf, c->vfs[vf03].vf.x());
  // nop                        |  madday.xyzw ACC, vf15, vf03
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf15].vf, c->vfs[vf03].vf.y());
  // nop                        |  maddz.xyzw vf03, vf16, vf03
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf03].vf, c->vf_src(vf16).vf, c->vf_src(vf03).vf.z());
  // nop                        |  mulaw.xyzw ACC, vf13, vf00
  c->acc.vf.mula(Mask::xyzw, c->vf_src(vf13).vf, c->vf_src(vf00).vf.w());
  // nop                        |  maddax.xyzw ACC, vf14, vf04
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf14].vf, c->vfs[vf04].vf.x());
  // nop                        |  madday.xyzw ACC, vf15, vf04
  c->acc.vf.madda(Mask::xyzw, c->vfs[vf15].vf, c->vfs[vf04].vf.y());
  // nop                        |  maddz.xyzw vf04, vf16, vf04
  c->acc.vf.madd(Mask::xyzw, c->vfs[vf04].vf, c->vf_src(vf16).vf, c->vf_src(vf04).vf.z());
  // nop                        |  mul.xyzw vf17, vf17, vf01
  c->vfs[vf17].vf.mul(Mask::xyzw, c->vf_src(vf17).vf, c->vf_src(vf01).vf);
  // nop                        |  mul.xyzw vf18, vf18, vf02
  c->vfs[vf18].vf.mul(Mask::xyzw, c->vf_src(vf18).vf, c->vf_src(vf02).vf);
  // nop                        |  mul.xyzw vf19, vf19, vf03
  c->vfs[vf19].vf.mul(Mask::xyzw, c->vf_src(vf19).vf, c->vf_src(vf03).vf);
  // nop                        |  mul.xyzw vf20, vf20, vf04
  c->vfs[vf20].vf.mul(Mask::xyzw, c->vf_src(vf20).vf, c->vf_src(vf04).vf);
  // nop                        |  minix.xyzw vf17, vf17, vf09
  c->vfs[vf17].vf.mini(Mask::xyzw, c->vf_src(vf17).vf, c->vf_src(vf09).vf.x());
  // nop                        |  minix.xyzw vf18, vf18, vf09
  c->vfs[vf18].vf.mini(Mask::xyzw, c->vf_src(vf18).vf, c->vf_src(vf09).vf.x());
  // nop                        |  minix.xyzw vf19, vf19, vf09
  c->vfs[vf19].vf.mini(Mask::xyzw, c->vf_src(vf19).vf, c->vf_src(vf09).vf.x());
  // nop                        |  minix.xyzw vf20, vf20, vf09
  c->vfs[vf20].vf.mini(Mask::xyzw, c->vf_src(vf20).vf, c->vf_src(vf09).vf.x());
  //fmt::print("light:\n {}\n {}\n {}\n {}\n\n", c->vf_src(vf17).vf.print(), c->vf_src(vf18).vf.print(), c->vf_src(vf19).vf.print(), c->vf_src(vf20).vf.print());



  // nop                        |  ftoi0.xyzw vf17, vf17
  c->vfs[vf17].vf.ftoi0(Mask::xyzw, c->vf_src(vf17).vf);
  // nop                        |  ftoi0.xyzw vf18, vf18
  c->vfs[vf18].vf.ftoi0(Mask::xyzw, c->vf_src(vf18).vf);
  // nop                        |  ftoi0.xyzw vf19, vf19 :e
  c->vfs[vf19].vf.ftoi0(Mask::xyzw, c->vf_src(vf19).vf);
  // nop                        |  ftoi0.xyzw vf20, vf20
  c->vfs[vf20].vf.ftoi0(Mask::xyzw, c->vf_src(vf20).vf);
}

void vcallms48(ExecutionContext* c) {
  // nop                        |  mulx.xyzw vf13, vf09, vf31
  c->vfs[vf13].vf.mul(Mask::xyzw, c->vf_src(vf09).vf, c->vf_src(vf31).vf.x());
  // nop                        |  subw.z vf21, vf21, vf00
  c->vfs[vf21].vf.sub(Mask::z, c->vf_src(vf21).vf, c->vf_src(vf00).vf.w());
  // nop                        |  addy.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.y());
  // nop                        |  mulx.xyz vf08, vf08, vf30
  c->vfs[vf08].vf.mul(Mask::xyz, c->vf_src(vf08).vf, c->vf_src(vf30).vf.x());
  // nop                        |  addw.xy vf05, vf05, vf31
  c->vfs[vf05].vf.add(Mask::xy, c->vf_src(vf05).vf, c->vf_src(vf31).vf.w());
  // nop                        |  mul.xyz vf30, vf21, vf13
  c->vfs[vf30].vf.mul(Mask::xyz, c->vf_src(vf21).vf, c->vf_src(vf13).vf);
  // nop                        |  addz.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.z());
  // nop                        |  add.xyz vf08, vf08, vf16
  c->vfs[vf08].vf.add(Mask::xyz, c->vf_src(vf08).vf, c->vf_src(vf16).vf);
  // move.xyzw vf28, vf27       |  ftoi12.xy vf17, vf05
  c->vfs[vf17].vf.ftoi12(Mask::xy, c->vf_src(vf05).vf);   c->vfs[vf28].vf.move(Mask::xyzw, c->vf_src(vf27).vf);
  // move.xyzw vf02, vf22       |  addy.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.y());   c->vfs[vf02].vf.move(Mask::xyzw, c->vf_src(vf22).vf);
  // rsqrt Q, vf31.z, vf29.x    |  mul.xyz vf06, vf06, Q
  c->vfs[vf06].vf.mul(Mask::xyz, c->vf_src(vf06).vf, c->Q);   c->Q = c->vf_src(vf31).vf.z() / std::sqrt(c->vf_src(vf29).vf.x());
  // nop                        |  mul.xyz vf29, vf08, vf08
  c->vfs[vf29].vf.mul(Mask::xyz, c->vf_src(vf08).vf, c->vf_src(vf08).vf);
  // nop                        |  mulx.xyz vf01, vf21, vf28
  c->vfs[vf01].vf.mul(Mask::xyz, c->vf_src(vf21).vf, c->vf_src(vf28).vf.x());
  // nop                        |  addz.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.z());
  // nop                        |  mulx.xyzw vf14, vf10, vf31
  c->vfs[vf14].vf.mul(Mask::xyzw, c->vf_src(vf10).vf, c->vf_src(vf31).vf.x());
  // nop                        |  subw.z vf02, vf02, vf00
  c->vfs[vf02].vf.sub(Mask::z, c->vf_src(vf02).vf, c->vf_src(vf00).vf.w());
  // nop                        |  addy.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.y());
  // nop                        |  mulx.xyz vf01, vf01, vf30
  c->vfs[vf01].vf.mul(Mask::xyz, c->vf_src(vf01).vf, c->vf_src(vf30).vf.x());
  // nop                        |  addw.xy vf06, vf06, vf31
  c->vfs[vf06].vf.add(Mask::xy, c->vf_src(vf06).vf, c->vf_src(vf31).vf.w());
  // nop                        |  mul.xyz vf30, vf02, vf14
  c->vfs[vf30].vf.mul(Mask::xyz, c->vf_src(vf02).vf, c->vf_src(vf14).vf);
  // nop                        |  addz.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.z());
  // nop                        |  add.xyz vf01, vf01, vf13
  c->vfs[vf01].vf.add(Mask::xyz, c->vf_src(vf01).vf, c->vf_src(vf13).vf);
  // nop                        |  ftoi12.xy vf18, vf06
  c->vfs[vf18].vf.ftoi12(Mask::xy, c->vf_src(vf06).vf);
  // nop                        |  addy.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.y());
  // rsqrt Q, vf31.z, vf29.x    |  mul.xyz vf07, vf07, Q
  c->vfs[vf07].vf.mul(Mask::xyz, c->vf_src(vf07).vf, c->Q);   c->Q = c->vf_src(vf31).vf.z() / std::sqrt(c->vf_src(vf29).vf.x());
  // move.xyzw vf03, vf23       |  mul.xyz vf29, vf01, vf01
  c->vfs[vf29].vf.mul(Mask::xyz, c->vf_src(vf01).vf, c->vf_src(vf01).vf);   c->vfs[vf03].vf.move(Mask::xyzw, c->vf_src(vf23).vf);
  // nop                        |  muly.xyz vf02, vf02, vf28
  c->vfs[vf02].vf.mul(Mask::xyz, c->vf_src(vf02).vf, c->vf_src(vf28).vf.y());
  // nop                        |  addz.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.z());
  // nop                        |  mulx.xyzw vf15, vf11, vf31
  c->vfs[vf15].vf.mul(Mask::xyzw, c->vf_src(vf11).vf, c->vf_src(vf31).vf.x());
  // nop                        |  subw.z vf03, vf03, vf00
  c->vfs[vf03].vf.sub(Mask::z, c->vf_src(vf03).vf, c->vf_src(vf00).vf.w());
  // nop                        |  addy.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.y());
  // nop                        |  mulx.xyz vf02, vf02, vf30
  c->vfs[vf02].vf.mul(Mask::xyz, c->vf_src(vf02).vf, c->vf_src(vf30).vf.x());
  // nop                        |  addw.xy vf07, vf07, vf31
  c->vfs[vf07].vf.add(Mask::xy, c->vf_src(vf07).vf, c->vf_src(vf31).vf.w());
  // nop                        |  mul.xyz vf30, vf03, vf15
  c->vfs[vf30].vf.mul(Mask::xyz, c->vf_src(vf03).vf, c->vf_src(vf15).vf);
  // nop                        |  addz.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.z());
  // nop                        |  add.xyz vf02, vf02, vf14
  c->vfs[vf02].vf.add(Mask::xyz, c->vf_src(vf02).vf, c->vf_src(vf14).vf);
  // nop                        |  ftoi12.xy vf19, vf07
  c->vfs[vf19].vf.ftoi12(Mask::xy, c->vf_src(vf07).vf);
  // nop                        |  addy.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.y());
  // rsqrt Q, vf31.z, vf29.x    |  mul.xyz vf08, vf08, Q
  c->vfs[vf08].vf.mul(Mask::xyz, c->vf_src(vf08).vf, c->Q);   c->Q = c->vf_src(vf31).vf.z() / std::sqrt(c->vf_src(vf29).vf.x());
  // move.xyzw vf04, vf24       |  mul.xyz vf29, vf02, vf02
  c->vfs[vf29].vf.mul(Mask::xyz, c->vf_src(vf02).vf, c->vf_src(vf02).vf);   c->vfs[vf04].vf.move(Mask::xyzw, c->vf_src(vf24).vf);
  // nop                        |  mulz.xyz vf03, vf03, vf28
  c->vfs[vf03].vf.mul(Mask::xyz, c->vf_src(vf03).vf, c->vf_src(vf28).vf.z());
  // nop                        |  addz.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.z());
  // nop                        |  mulx.xyzw vf16, vf12, vf31
  c->vfs[vf16].vf.mul(Mask::xyzw, c->vf_src(vf12).vf, c->vf_src(vf31).vf.x());
  // nop                        |  subw.z vf04, vf04, vf00
  c->vfs[vf04].vf.sub(Mask::z, c->vf_src(vf04).vf, c->vf_src(vf00).vf.w());
  // nop                        |  addy.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.y());
  // nop                        |  mulx.xyz vf03, vf03, vf30
  c->vfs[vf03].vf.mul(Mask::xyz, c->vf_src(vf03).vf, c->vf_src(vf30).vf.x());
  // nop                        |  addw.xy vf08, vf08, vf31
  c->vfs[vf08].vf.add(Mask::xy, c->vf_src(vf08).vf, c->vf_src(vf31).vf.w());
  // nop                        |  mul.xyz vf30, vf04, vf16
  c->vfs[vf30].vf.mul(Mask::xyz, c->vf_src(vf04).vf, c->vf_src(vf16).vf);
  // nop                        |  addz.x vf29, vf29, vf29
  c->vfs[vf29].vf.add(Mask::x, c->vf_src(vf29).vf, c->vf_src(vf29).vf.z());
  // nop                        |  add.xyz vf03, vf03, vf15
  c->vfs[vf03].vf.add(Mask::xyz, c->vf_src(vf03).vf, c->vf_src(vf15).vf);
  // nop                        |  ftoi12.xy vf20, vf08
  c->vfs[vf20].vf.ftoi12(Mask::xy, c->vf_src(vf08).vf);
  // nop                        |  addy.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.y());
  // rsqrt Q, vf31.z, vf29.x    |  mul.xyz vf05, vf01, Q
  c->vfs[vf05].vf.mul(Mask::xyz, c->vf_src(vf01).vf, c->Q);   c->Q = c->vf_src(vf31).vf.z() / std::sqrt(c->vf_src(vf29).vf.x());
  // move.xyzw vf06, vf02       |  mul.xyz vf29, vf03, vf03
  c->vfs[vf29].vf.mul(Mask::xyz, c->vf_src(vf03).vf, c->vf_src(vf03).vf);   c->vfs[vf06].vf.move(Mask::xyzw, c->vf_src(vf02).vf);
  // move.xyzw vf07, vf03       |  mulw.xyz vf08, vf04, vf28 :e
  c->vfs[vf08].vf.mul(Mask::xyz, c->vf_src(vf04).vf, c->vf_src(vf28).vf.w());   c->vfs[vf07].vf.move(Mask::xyzw, c->vf_src(vf03).vf);
  // nop                        |  addz.x vf30, vf30, vf30
  c->vfs[vf30].vf.add(Mask::x, c->vf_src(vf30).vf, c->vf_src(vf30).vf.z());

}

namespace generic_debug_light_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -80);                           // daddiu sp, sp, -80
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sd(fp, 12440, at);                             // sd fp, 12440(at)
  c->mov64(fp, t9);                                 // or fp, t9, r0
  c->sq(s3, 12448, at);                             // sq s3, 12448(at)
  c->sq(s4, 12464, at);                             // sq s4, 12464(at)
  c->sq(s5, 12480, at);                             // sq s5, 12480(at)
  c->sq(gp, 12496, at);                             // sq gp, 12496(at)
  // daddiu v1, fp, L109                          // daddiu v1, fp, L109
  // daddiu a0, fp, L108                          // daddiu a0, fp, L108
  // daddiu a1, fp, L107                          // daddiu a1, fp, L107
  // L107-L109 are 16-byte constants in the object data segment; the emitter
  // cannot address them, so the loads below write the proven bits directly
  // (decompiler_out/jakx/generic-effect_ir2.asm L107/L108/L109).
  // c->lqc2(vf5, 0, v1);                         // lqc2 vf5, 0(v1)   L109 = (255.0, 255.0, 255.0, 0.0)
  c->vfs[vf5].du32[0] = 0x437f0000;
  c->vfs[vf5].du32[1] = 0x437f0000;
  c->vfs[vf5].du32[2] = 0x437f0000;
  c->vfs[vf5].du32[3] = 0x0;
  // c->lqc2(vf6, 0, a0);                         // lqc2 vf6, 0(a0)   L108 = (0.0, 0.0, 0.0, 128.0)
  c->vfs[vf6].du32[0] = 0x0;
  c->vfs[vf6].du32[1] = 0x0;
  c->vfs[vf6].du32[2] = 0x0;
  c->vfs[vf6].du32[3] = 0x43000000;
  // c->lqc2(vf7, 0, a1);                         // lqc2 vf7, 0(a1)   L107 = (-1.0, 1.0, 0.0, 0.0)
  c->vfs[vf7].du32[0] = 0xbf800000;
  c->vfs[vf7].du32[1] = 0x3f800000;
  c->vfs[vf7].du32[2] = 0x0;
  c->vfs[vf7].du32[3] = 0x0;
  c->mov128_gpr_vf(v1, vf7);                        // qmfc2.i v1, vf7
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 44, at);                                // lw a0, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a3, 36, at);                                // lw a3, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 0, a0);                                 // lw v1, 0(a0)
  // nop                                            // sll r0, r0, 0
  c->lw(t1, 4, a0);                                 // lw t1, 4(a0)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 4, at);                                 // lw a0, 4(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a1, 8, at);                                 // lw a1, 8(at)
  c->addiu(t2, r0, 255);                            // addiu t2, r0, 255
  c->lw(a2, 12, at);                                // lw a2, 12(at)
  c->addiu(t3, r0, 256);                            // addiu t3, r0, 256
  c->lui(t0, -2);                                   // lui t0, -2
  c->daddiu(t4, a3, -4);                            // daddiu t4, a3, -4
  c->mov64(a3, a3);                                 // or a3, a3, r0
  // nop                                            // sll r0, r0, 0
  c->ori(t0, t0, 65534);                            // ori t0, t0, 65534
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t0, t0, t0);                            // pextlw t0, t0, t0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t0, t0, t0);                            // pextlw t0, t0, t0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t1, t1, t1);                            // pextlw t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t1, t1, t1);                            // pextlw t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyh(t2, t2);                                 // pcpyh t2, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t2, t2, t2);                            // pcpyld t2, t2, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyh(t3, t3);                                 // pcpyh t3, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t3, t3, t3);                            // pcpyld t3, t3, t3
  
block_1:
  c->daddiu(a3, a3, -4);                            // daddiu a3, a3, -4
  c->ldr(t4, 0, v1);                                // ldr t4, 0(v1)
  c->daddiu(a0, a0, 48);                            // daddiu a0, a0, 48
  c->ldl(t4, 7, v1);                                // ldl t4, 7(v1)
  c->daddiu(a2, a2, 16);                            // daddiu a2, a2, 16
  c->daddiu(v1, v1, 8);                             // daddiu v1, v1, 8
  c->pextlh(t4, r0, t4);                            // pextlh t4, r0, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t5, t4, t2);                              // pand t5, t4, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(s4, t5, t1);                             // paddw s4, t5, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s5, s4, 0);                             // dsrl32 s5, s4, 0
  c->lq(t6, 0, s4);                                 // lq t6, 0(s4)
  c->pcpyud(gp, s4, r0);                            // pcpyud gp, s4, r0
  c->lq(t7, 0, s5);                                 // lq t7, 0(s5)
  c->dsrl32(t9, gp, 0);                             // dsrl32 t9, gp, 0
  c->lq(t5, 0, gp);                                 // lq t5, 0(gp)
  c->pand(ra, t4, t3);                              // pand ra, t4, t3
  c->lq(t8, 0, t9);                                 // lq t8, 0(t9)
  c->psraw(ra, ra, 8);                              // psraw ra, ra, 8
  c->lqc2(vf1, 16, s4);                             // lqc2 vf1, 16(s4)
  c->pextuw(s4, t7, t6);                            // pextuw s4, t7, t6
  c->lqc2(vf2, 16, s5);                             // lqc2 vf2, 16(s5)
  c->pextuw(s5, t8, t5);                            // pextuw s5, t8, t5
  c->lqc2(vf3, 16, gp);                             // lqc2 vf3, 16(gp)
  c->pcpyud(gp, s4, s5);                            // pcpyud gp, s4, s5
  c->lqc2(vf4, 16, t9);                             // lqc2 vf4, 16(t9)
  c->pand(t9, gp, t0);                              // pand t9, gp, t0
  // nop                                            // sll r0, r0, 0
  c->por(t9, t9, ra);                               // por t9, t9, ra
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->vmax_bc(DEST::xyz, BC::x, vf1, vf1, vf7);      // vmaxx.xyz vf1, vf1, vf7
  // nop                                            // sll r0, r0, 0
  c->vmax_bc(DEST::xyz, BC::x, vf2, vf2, vf7);      // vmaxx.xyz vf2, vf2, vf7
  // nop                                            // sll r0, r0, 0
  c->vmax_bc(DEST::xyz, BC::x, vf3, vf3, vf7);      // vmaxx.xyz vf3, vf3, vf7
  // nop                                            // sll r0, r0, 0
  c->vmax_bc(DEST::xyz, BC::x, vf4, vf4, vf7);      // vmaxx.xyz vf4, vf4, vf7
  // nop                                            // sll r0, r0, 0
  c->vmini_bc(DEST::xyz, BC::y, vf1, vf1, vf7);     // vminiy.xyz vf1, vf1, vf7
  // nop                                            // sll r0, r0, 0
  c->vmini_bc(DEST::xyz, BC::y, vf2, vf2, vf7);     // vminiy.xyz vf2, vf2, vf7
  // nop                                            // sll r0, r0, 0
  c->vmini_bc(DEST::xyz, BC::y, vf3, vf3, vf7);     // vminiy.xyz vf3, vf3, vf7
  // nop                                            // sll r0, r0, 0
  c->vmini_bc(DEST::xyz, BC::y, vf4, vf4, vf7);     // vminiy.xyz vf4, vf4, vf7
  // nop                                            // sll r0, r0, 0
  c->vmul(DEST::xyzw, vf1, vf1, vf5);               // vmul.xyzw vf1, vf1, vf5
  // nop                                            // sll r0, r0, 0
  c->vmul(DEST::xyzw, vf2, vf2, vf5);               // vmul.xyzw vf2, vf2, vf5
  // nop                                            // sll r0, r0, 0
  c->vmul(DEST::xyzw, vf3, vf3, vf5);               // vmul.xyzw vf3, vf3, vf5
  // nop                                            // sll r0, r0, 0
  c->vmul(DEST::xyzw, vf4, vf4, vf5);               // vmul.xyzw vf4, vf4, vf5
  // nop                                            // sll r0, r0, 0
  c->vadd(DEST::xyzw, vf1, vf1, vf6);               // vadd.xyzw vf1, vf1, vf6
  // nop                                            // sll r0, r0, 0
  c->vadd(DEST::xyzw, vf2, vf2, vf6);               // vadd.xyzw vf2, vf2, vf6
  // nop                                            // sll r0, r0, 0
  c->vadd(DEST::xyzw, vf3, vf3, vf6);               // vadd.xyzw vf3, vf3, vf6
  // nop                                            // sll r0, r0, 0
  c->vadd(DEST::xyzw, vf4, vf4, vf6);               // vadd.xyzw vf4, vf4, vf6
  // nop                                            // sll r0, r0, 0
  c->vftoi0(DEST::xyzw, vf1, vf1);                  // vftoi0.xyzw vf1, vf1
  // nop                                            // sll r0, r0, 0
  c->vftoi0(DEST::xyzw, vf2, vf2);                  // vftoi0.xyzw vf2, vf2
  // nop                                            // sll r0, r0, 0
  c->vftoi0(DEST::xyzw, vf3, vf3);                  // vftoi0.xyzw vf3, vf3
  // nop                                            // sll r0, r0, 0
  c->vftoi0(DEST::xyzw, vf4, vf4);                  // vftoi0.xyzw vf4, vf4
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(gp, vf1);                        // qmfc2.i gp, vf1
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(s5, vf2);                        // qmfc2.i s5, vf2
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(s4, vf3);                        // qmfc2.i s4, vf3
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(ra, vf4);                        // qmfc2.i ra, vf4
  c->ppach(gp, r0, gp);                             // ppach gp, r0, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(s5, r0, s5);                             // ppach s5, r0, s5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(s4, r0, s4);                             // ppach s4, r0, s4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(ra, r0, ra);                             // ppach ra, r0, ra
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(gp, r0, gp);                             // ppacb gp, r0, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(s3, r0, s5);                             // ppacb s3, r0, s5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(s5, r0, s4);                             // ppacb s5, r0, s4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(ra, r0, ra);                             // ppacb ra, r0, ra
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(gp, s3, gp);                            // pextlw gp, s3, gp
  // nop                                            // sll r0, r0, 0
  c->daddiu(a1, a1, 16);                            // daddiu a1, a1, 16
  c->daddiu(s4, a3, -4);                            // daddiu s4, a3, -4
  c->pextlw(ra, ra, s5);                            // pextlw ra, ra, s5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(ra, ra, gp);                            // pcpyld ra, ra, gp
  c->sq(t9, -16, a2);                               // sq t9, -16(a2)
  c->prot3w(t8, t8);                                // prot3w t8, t8
  c->sq(ra, -16, a1);                               // sq ra, -16(a1)
  c->prot3w(t7, t7);                                // prot3w t7, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t9, t7, t6);                            // pextuw t9, t7, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t7, t5, t7);                            // pcpyld t7, t5, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t6, t9, t6);                            // pcpyld t6, t9, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t5, t8, t5);                            // pextuw t5, t8, t5
  c->sq(t6, -48, a0);                               // sq t6, -48(a0)
  c->pcpyld(t5, t8, t5);                            // pcpyld t5, t8, t5
  c->sq(t7, -32, a0);                               // sq t7, -32(a0)
  bc = ((s64)c->sgpr64(a3)) > 0;                    // bgtz a3, L2
  c->sq(t5, -16, a0);                               // sq t5, -16(a0)
  if (bc) {goto block_1;}                           // branch non-likely

  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->ld(fp, 12440, at);                             // ld fp, 12440(at)
  c->lq(gp, 12496, at);                             // lq gp, 12496(at)
  c->lq(s5, 12480, at);                             // lq s5, 12480(at)
  c->lq(s4, 12464, at);                             // lq s4, 12464(at)
  c->lq(s3, 12448, at);                             // lq s3, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 80);                            // daddiu sp, sp, 80
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-debug-light-proc", execute, 256);
}

} // namespace generic_debug_light_proc

namespace generic_warp_envmap_dest {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* viewport_array; // *viewport-array*
  void* generic_envmap_proc; // generic-envmap-proc
  void* generic_no_light_proc; // generic-no-light-proc
  void* generic_warp_dest_proc; // generic-warp-dest-proc
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  u32 qwc = 0;
  u32 madr = 0;
  u32 sadr = 0;
  c->daddiu(sp, sp, -32);                           // daddiu sp, sp, -32
  c->sd(ra, 0, sp);                                 // sd ra, 0(sp)
  c->sq(gp, 16, sp);                                // sq gp, 16(sp)
  c->daddiu(v1, s7, 4);                             // daddiu v1, s7, #t
  bc = c->sgpr64(s7) == c->sgpr64(v1);              // beq s7, v1, L9
  c->mov64(v0, s7);                                 // or v0, s7, r0
  if (bc) {goto block_11;}                          // branch non-likely

  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->mov64(v1, a0);                                 // or v1, a0, r0
  c->load_symbol2(a0, cache.viewport_array);        // lw a0, *viewport-array*(s7)
  c->addiu(a1, r0, 688);                            // addiu a1, r0, 688
  c->lwu(a2, 8, a0);                                // lwu a2, 8(a0)
  c->multu3(a1, a1, a2);                            // multu3 a1, a1, a2
  c->daddiu(a1, a1, 16);                            // daddiu a1, a1, 16
  c->daddu(a0, a1, a0);                             // daddu a0, a1, a0
  c->daddiu(gp, a0, 368);                           // daddiu gp, a0, 368
  c->mov64(a0, gp);                                 // or a0, gp, r0
  c->addiu(a0, r0, 14);                             // addiu a0, r0, 14
  c->sh(a0, 11984, at);                             // sh a0, 11984(at)
  c->sw(r0, 48, at);                                // sw r0, 48(at)
  c->sw(v1, 44, at);                                // sw v1, 44(at)
  c->addiu(v1, r0, 1);                              // addiu v1, r0, 1
  c->sh(v1, 56, at);                                // sh v1, 56(at)
  c->daddiu(v1, at, 12064);                         // daddiu v1, at, 12064
  get_fake_spad_addr2(a0, cache.fake_scratchpad_data, 0, c);// lui a0, 28672
  c->lbu(a0, 6842, a0);                             // lbu a0, 6842(a0)
  bc = c->sgpr64(a0) == 0;                          // beq a0, r0, L4
  c->mov64(a0, s7);                                 // or a0, s7, r0
  if (bc) {goto block_3;}                           // branch non-likely

  c->addiu(v1, r0, 6960);                           // addiu v1, r0, 6960
  get_fake_spad_addr2(a0, cache.fake_scratchpad_data, 0, c);// lui a0, 28672
  c->daddu(v1, v1, a0);                             // daddu v1, v1, a0
  c->mov64(v1, v1);                                 // or v1, v1, r0
  c->mov64(a0, v1);                                 // or a0, v1, r0
  
block_3:
  c->sw(v1, 52, at);                                // sw v1, 52(at)
  get_fake_spad_addr2(v1, cache.fake_scratchpad_data, 0, c);// lui v1, 28672
  c->lwu(t9, 7468, v1);                             // lwu t9, 7468(v1)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->ld(v1, 0, gp);                                 // ld v1, 0(gp)
  c->ld(a0, 16, gp);                                // ld a0, 16(gp)
  c->ld(a1, 32, gp);                                // ld a1, 32(gp)
  c->ld(a2, 48, gp);                                // ld a2, 48(gp)
  c->ld(t0, 64, gp);                                // ld t0, 64(gp)
  c->ld(a3, 11936, at);                             // ld a3, 11936(at)
  get_fake_spad_addr2(t1, cache.fake_scratchpad_data, 0, c);// lui t1, 28672
  c->lbu(t1, 6856, t1);                             // lbu t1, 6856(t1)
  c->addiu(t2, r0, 128);                            // addiu t2, r0, 128
  c->sltu(t2, t2, t1);                              // sltu t2, t2, t1
  bc = c->sgpr64(t2) != 0;                          // bne t2, r0, L5
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_5;}                           // branch non-likely

  c->addiu(t2, r0, 128);                            // addiu t2, r0, 128
  c->dsubu(t1, t2, t1);                             // dsubu t1, t2, t1
  c->dsll32(t1, t1, 24);                            // dsll32 t1, t1, 24
  c->dsrl(t1, t1, 24);                              // dsrl t1, t1, 24
  c->ori(t1, t1, 100);                              // ori t1, t1, 100
  c->mov64(t1, t1);                                 // or t1, t1, r0
  c->mov64(t2, t1);                                 // or t2, t1, r0
  //beq r0, r0, L6                                  // beq r0, r0, L6
  // nop                                            // sll r0, r0, 0
  goto block_6;                                     // branch always

  
block_5:
  c->ld(t1, 11904, at);                             // ld t1, 11904(at)
  c->mov64(t2, t1);                                 // or t2, t1, r0
  
block_6:
  c->lw(t2, 24, at);                                // lw t2, 24(at)
  c->sd(v1, 128, t2);                               // sd v1, 128(t2)
  c->sd(a0, 144, t2);                               // sd a0, 144(t2)
  c->sd(a1, 160, t2);                               // sd a1, 160(t2)
  c->sd(a2, 176, t2);                               // sd a2, 176(t2)
  c->sd(t0, 192, t2);                               // sd t0, 192(t2)
  c->addiu(v1, r0, 71);                             // addiu v1, r0, 71
  c->sd(a3, 96, t2);                                // sd a3, 96(t2)
  c->addiu(a0, r0, 66);                             // addiu a0, r0, 66
  c->sd(t1, 112, t2);                               // sd t1, 112(t2)
  c->sw(v1, 104, t2);                               // sw v1, 104(t2)
  c->sw(a0, 120, t2);                               // sw a0, 120(t2)
  c->load_symbol2(t9, cache.generic_warp_dest_proc);// lw t9, generic-warp-dest-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->load_symbol2(t9, cache.generic_no_light_proc); // lw t9, generic-no-light-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->load_symbol2(t9, cache.generic_envmap_proc);   // lw t9, generic-envmap-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->lw(v1, 24, at);                                // lw v1, 24(at)
  c->lw(a0, 40, at);                                // lw a0, 40(at)
  c->mov64(a3, v1);                                 // or a3, v1, r0
  // nop                                            // sll r0, r0, 0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(a2, 4096);                                 // lui a2, 4096
  c->lwu(a1, 60, at);                               // lwu a1, 60(at)
  c->ori(a2, a2, 53248);                            // ori a2, a2, 53248
  // c->lw(t1, 0, a2);                              // lw t1, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, at, 92);                            // daddiu t0, at, 92
  c->andi(a3, a3, 16383);                           // andi a3, a3, 16383
  c->andi(t1, t1, 256);                             // andi t1, t1, 256
  // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(t1) == 0;                          // beq t1, r0, L8
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_10;}                          // branch non-likely
//
//   c->mov64(t1, a2);                                 // or t1, a2, r0
//   // nop                                            // sll r0, r0, 0
//
// block_8:
//   c->lw(t2, 0, t0);                                 // lw t2, 0(t0)
//   // nop                                            // sll r0, r0, 0
//   c->lw(t3, 0, t1);                                 // lw t3, 0(t1)
//   // nop                                            // sll r0, r0, 0
//   c->andi(t3, t3, 256);                             // andi t3, t3, 256
//   c->daddiu(t2, t2, 1);                             // daddiu t2, t2, 1
//   bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L7
//   c->sw(t2, 0, t0);                                 // sw t2, 0(t0)
//   if (bc) {goto block_8;}                           // branch non-likely
//
//   c->gprs[t0].du64[0] = 0;                          // or t0, r0, r0
  
// block_10:
  c->dsll(t0, a0, 4);                               // dsll t0, a0, 4
  // c->sw(a3, 128, a2);                            // sw a3, 128(a2)
  sadr = c->sgpr64(a3);
  // nop                                            // sll r0, r0, 0
  // c->sw(a1, 16, a2);                             // sw a1, 16(a2)
  madr = c->sgpr64(a1);
  c->addiu(a3, r0, 256);                            // addiu a3, r0, 256
  // c->sw(a0, 32, a2);                             // sw a0, 32(a2)
  qwc = c->sgpr64(a0);
  c->daddu(a0, a1, t0);                             // daddu a0, a1, t0
  // c->sw(a3, 0, a2);                              // sw a3, 0(a2)
  spad_from_dma_no_sadr_off(cache.fake_scratchpad_data, madr, sadr, qwc);
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 60, at);                                // sw a0, 60(at)
  c->gprs[a0].du64[0] = 0;                          // or a0, r0, r0
  c->xori(v0, v1, 4608);                            // xori v0, v1, 4608
  c->sw(v0, 24, at);                                // sw v0, 24(at)
  
block_11:
  c->ld(ra, 0, sp);                                 // ld ra, 0(sp)
  c->lq(gp, 16, sp);                                // lq gp, 16(sp)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 32);                            // daddiu sp, sp, 32
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  cache.viewport_array = intern_from_c(-1, 0, "*viewport-array*").c();
  cache.generic_envmap_proc = intern_from_c(-1, 0, "generic-envmap-proc").c();
  cache.generic_no_light_proc = intern_from_c(-1, 0, "generic-no-light-proc").c();
  cache.generic_warp_dest_proc = intern_from_c(-1, 0, "generic-warp-dest-proc").c();
  gLinkedFunctionTable.reg("generic-warp-envmap-dest", execute, 128);
}

} // namespace generic_warp_envmap_dest

namespace generic_warp_dest {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* viewport_array; // *viewport-array*
  void* generic_no_light_proc; // generic-no-light-proc
  void* generic_warp_dest_proc; // generic-warp-dest-proc
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  u32 qwc = 0;
  u32 madr = 0;
  u32 sadr = 0;
  c->daddiu(sp, sp, -32);                           // daddiu sp, sp, -32
  c->sd(ra, 0, sp);                                 // sd ra, 0(sp)
  c->sq(gp, 16, sp);                                // sq gp, 16(sp)
  c->daddiu(v1, s7, 4);                             // daddiu v1, s7, #t
  bc = c->sgpr64(s7) == c->sgpr64(v1);              // beq s7, v1, L14
  c->mov64(v0, s7);                                 // or v0, s7, r0
  if (bc) {goto block_8;}                           // branch non-likely

  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->mov64(v1, a0);                                 // or v1, a0, r0
  c->load_symbol2(a0, cache.viewport_array);        // lw a0, *viewport-array*(s7)
  c->addiu(a1, r0, 688);                            // addiu a1, r0, 688
  c->lwu(a2, 8, a0);                                // lwu a2, 8(a0)
  c->multu3(a1, a1, a2);                            // multu3 a1, a1, a2
  c->daddiu(a1, a1, 16);                            // daddiu a1, a1, 16
  c->daddu(a0, a1, a0);                             // daddu a0, a1, a0
  c->daddiu(gp, a0, 368);                           // daddiu gp, a0, 368
  c->mov64(a0, gp);                                 // or a0, gp, r0
  c->addiu(a0, r0, 14);                             // addiu a0, r0, 14
  c->sh(a0, 11984, at);                             // sh a0, 11984(at)
  c->sw(r0, 48, at);                                // sw r0, 48(at)
  c->sw(v1, 44, at);                                // sw v1, 44(at)
  c->sh(r0, 56, at);                                // sh r0, 56(at)
  c->daddiu(v1, at, 12064);                         // daddiu v1, at, 12064
  get_fake_spad_addr2(a0, cache.fake_scratchpad_data, 0, c);// lui a0, 28672
  c->lbu(a0, 6842, a0);                             // lbu a0, 6842(a0)
  bc = c->sgpr64(a0) == 0;                          // beq a0, r0, L11
  c->mov64(a0, s7);                                 // or a0, s7, r0
  if (bc) {goto block_3;}                           // branch non-likely

  c->addiu(v1, r0, 6960);                           // addiu v1, r0, 6960
  get_fake_spad_addr2(a0, cache.fake_scratchpad_data, 0, c);// lui a0, 28672
  c->daddu(v1, v1, a0);                             // daddu v1, v1, a0
  c->mov64(v1, v1);                                 // or v1, v1, r0
  c->mov64(a0, v1);                                 // or a0, v1, r0
  
block_3:
  c->sw(v1, 52, at);                                // sw v1, 52(at)
  get_fake_spad_addr2(v1, cache.fake_scratchpad_data, 0, c);// lui v1, 28672
  c->lwu(t9, 7464, v1);                             // lwu t9, 7464(v1)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->ld(v1, 0, gp);                                 // ld v1, 0(gp)
  c->ld(a0, 16, gp);                                // ld a0, 16(gp)
  c->ld(a1, 32, gp);                                // ld a1, 32(gp)
  c->ld(a2, 48, gp);                                // ld a2, 48(gp)
  c->ld(a3, 64, gp);                                // ld a3, 64(gp)
  c->ld(t0, 11936, at);                             // ld t0, 11936(at)
  c->lw(t1, 24, at);                                // lw t1, 24(at)
  c->sd(v1, 128, t1);                               // sd v1, 128(t1)
  c->sd(a0, 144, t1);                               // sd a0, 144(t1)
  c->sd(a1, 160, t1);                               // sd a1, 160(t1)
  c->sd(a2, 176, t1);                               // sd a2, 176(t1)
  c->sd(a3, 192, t1);                               // sd a3, 192(t1)
  c->sd(t0, 96, t1);                                // sd t0, 96(t1)
  c->load_symbol2(t9, cache.generic_warp_dest_proc);// lw t9, generic-warp-dest-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->load_symbol2(t9, cache.generic_no_light_proc); // lw t9, generic-no-light-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->lw(v1, 24, at);                                // lw v1, 24(at)
  c->lw(a0, 40, at);                                // lw a0, 40(at)
  c->mov64(a3, v1);                                 // or a3, v1, r0
  // nop                                            // sll r0, r0, 0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(a2, 4096);                                 // lui a2, 4096
  c->lwu(a1, 60, at);                               // lwu a1, 60(at)
  c->ori(a2, a2, 53248);                            // ori a2, a2, 53248
  // c->lw(t1, 0, a2);                              // lw t1, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, at, 92);                            // daddiu t0, at, 92
  c->andi(a3, a3, 16383);                           // andi a3, a3, 16383
  c->andi(t1, t1, 256);                             // andi t1, t1, 256
  // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(t1) == 0;                          // beq t1, r0, L13
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_7;}                           // branch non-likely

  c->mov64(t1, a2);                                 // or t1, a2, r0
  // nop                                            // sll r0, r0, 0
  
// block_5:
//   c->lw(t2, 0, t0);                                 // lw t2, 0(t0)
//   // nop                                            // sll r0, r0, 0
//   c->lw(t3, 0, t1);                                 // lw t3, 0(t1)
//   // nop                                            // sll r0, r0, 0
//   c->andi(t3, t3, 256);                             // andi t3, t3, 256
//   c->daddiu(t2, t2, 1);                             // daddiu t2, t2, 1
//   bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L12
//   c->sw(t2, 0, t0);                                 // sw t2, 0(t0)
//   if (bc) {goto block_5;}                           // branch non-likely
//
//   c->gprs[t0].du64[0] = 0;                          // or t0, r0, r0
  
// block_7:
  c->dsll(t0, a0, 4);                               // dsll t0, a0, 4
  // c->sw(a3, 128, a2);                            // sw a3, 128(a2)
  sadr = c->sgpr64(a3);
  // nop                                            // sll r0, r0, 0
  // c->sw(a1, 16, a2);                             // sw a1, 16(a2)
  madr = c->sgpr64(a1);
  c->addiu(a3, r0, 256);                            // addiu a3, r0, 256
  // c->sw(a0, 32, a2);                             // sw a0, 32(a2)
  qwc = c->sgpr64(a0);
  c->daddu(a0, a1, t0);                             // daddu a0, a1, t0
  // c->sw(a3, 0, a2);                              // sw a3, 0(a2)
  spad_from_dma_no_sadr_off(cache.fake_scratchpad_data, madr, sadr, qwc);
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 60, at);                                // sw a0, 60(at)
  c->gprs[a0].du64[0] = 0;                          // or a0, r0, r0
  c->xori(v0, v1, 4608);                            // xori v0, v1, 4608
  c->sw(v0, 24, at);                                // sw v0, 24(at)
  
block_8:
  c->ld(ra, 0, sp);                                 // ld ra, 0(sp)
  c->lq(gp, 16, sp);                                // lq gp, 16(sp)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 32);                            // daddiu sp, sp, 32
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  cache.viewport_array = intern_from_c(-1, 0, "*viewport-array*").c();
  cache.generic_no_light_proc = intern_from_c(-1, 0, "generic-no-light-proc").c();
  cache.generic_warp_dest_proc = intern_from_c(-1, 0, "generic-warp-dest-proc").c();
  gLinkedFunctionTable.reg("generic-warp-dest", execute, 128);
}

} // namespace generic_warp_dest

namespace generic_warp_dest_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -16);                           // daddiu sp, sp, -16
  c->sd(fp, 12440, at);                             // sd fp, 12440(at)
  c->mov64(fp, t9);                                 // or fp, t9, r0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lw(a1, 44, at);                                // lw a1, 44(at)
  c->lw(v1, 8, a1);                                 // lw v1, 8(a1)
  c->lw(a0, 4, a1);                                 // lw a0, 4(a1)
  c->lbu(a1, 17, a1);                               // lbu a1, 17(a1)
  c->lhu(a3, 6820, at);                             // lhu a3, 6820(at)
  c->lhu(a2, 6822, at);                             // lhu a2, 6822(at)
  c->dsll(t0, a3, 16);                              // dsll t0, a3, 16
  c->or_(a3, a3, t0);                               // or a3, a3, t0
  // Unknown instr: ld t0, L111(fp)
  c->gprs[t0].du64[0] = 0xffffffffull; // L111 data: 0x00000000ffffffff
  
block_1:
  c->lwu(t1, 12, a0);                               // lwu t1, 12(a0)
  c->psubh(t1, t1, a3);                             // psubh t1, t1, a3
  c->dsrav(t1, t1, a2);                             // dsrav t1, t1, a2
  c->ppacb(t1, r0, t1);                             // ppacb t1, r0, t1
  c->and_(t1, t1, t0);                              // and t1, t1, t0
  c->sll(t1, t1, 2);                                // sll t1, t1, 2
  c->daddu(t1, v1, t1);                             // daddu t1, v1, t1
  c->lw(t1, 0, t1);                                 // lw t1, 0(t1)
  c->sw(t1, 12, a0);                                // sw t1, 12(a0)
  c->daddiu(a1, a1, -1);                            // daddiu a1, a1, -1
  bc = ((s64)c->sgpr64(a1)) > 0;                    // bgtz a1, L16
  c->daddiu(a0, a0, 32);                            // daddiu a0, a0, 32
  if (bc) {goto block_1;}                           // branch non-likely

  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(fp, 12440, at);                             // ld fp, 12440(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 16);                            // daddiu sp, sp, 16
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-warp-dest-proc", execute, 128);
}

} // namespace generic_warp_dest_proc

namespace generic_warp_source_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* view_get_active_math_camera; // view-get-active-math-camera
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -64);                           // daddiu sp, sp, -64
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sd(fp, 12440, at);                             // sd fp, 12440(at)
  c->mov64(fp, t9);                                 // or fp, t9, r0
  c->sq(s4, 12448, at);                             // sq s4, 12448(at)
  c->sq(s5, 12464, at);                             // sq s5, 12464(at)
  c->sq(gp, 12480, at);                             // sq gp, 12480(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lw(v1, 44, at);                                // lw v1, 44(at)
  c->lw(gp, 8, v1);                                 // lw gp, 8(v1)
  c->lw(s5, 4, v1);                                 // lw s5, 4(v1)
  c->lh(s4, 20, v1);                                // lh s4, 20(v1)
  c->load_symbol2(t9, cache.view_get_active_math_camera);// lw t9, view-get-active-math-camera(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->mov64(v1, v0);                                 // or v1, v0, r0
  c->lhu(a0, 6820, at);                             // lhu a0, 6820(at)
  c->lhu(v1, 6822, at);                             // lhu v1, 6822(at)
  c->dsll(a1, a0, 16);                              // dsll a1, a0, 16
  c->or_(a0, a0, a1);                               // or a0, a0, a1
  c->lqc2(vf5, 7520, at);                           // lqc2 vf5, 7520(at)
  c->lqc2(vf6, 7536, at);                           // lqc2 vf6, 7536(at)
  c->lqc2(vf1, 7136, at);                           // lqc2 vf1, 7136(at)
  c->lqc2(vf2, 7152, at);                           // lqc2 vf2, 7152(at)
  c->lqc2(vf3, 7168, at);                           // lqc2 vf3, 7168(at)
  c->lqc2(vf4, 7184, at);                           // lqc2 vf4, 7184(at)
  // Unknown instr: ld a1, L111(fp)
  c->gprs[a1].du64[0] = 0xffffffffull; // L111 data: 0x00000000ffffffff
  
block_1:
  c->lqc2(vf9, 0, s5);                              // lqc2 vf9, 0(s5)
  c->lwu(a2, 12, s5);                               // lwu a2, 12(s5)
  c->vmax_bc(DEST::z, BC::z, vf9, vf9, vf6);        // vmaxz.z vf9, vf9, vf6
  // nop                                            // vnop
  c->vmula_bc(DEST::xyzw, BC::w, vf4, vf0);         // vmulaw.xyzw acc, vf4, vf0
  c->vmadda_bc(DEST::xyzw, BC::x, vf1, vf9);        // vmaddax.xyzw acc, vf1, vf9
  c->vmadda_bc(DEST::xyzw, BC::y, vf2, vf9);        // vmadday.xyzw acc, vf2, vf9
  c->vmadd_bc(DEST::xyzw, BC::z, vf9, vf3, vf9);    // vmaddz.xyzw vf9, vf3, vf9
  // nop                                            // vnop
  c->vdiv(vf5, BC::w, vf9, BC::w);                  // vdiv Q, vf5.w, vf9.w
  // nop                                            // vnop
  c->vwaitq();                                      // vwaitq
  c->vmulq(DEST::xyz, vf9, vf9);                    // vmulq.xyz vf9, vf9, Q
  c->vmul(DEST::xy, vf9, vf9, vf5);                 // vmul.xy vf9, vf9, vf5
  c->vadd_bc(DEST::xy, BC::z, vf9, vf9, vf5);       // vaddz.xy vf9, vf9, vf5
  c->vadd(DEST::xy, vf9, vf9, vf6);                 // vadd.xy vf9, vf9, vf6
  c->vftoi12(DEST::xyzw, vf10, vf9);                // vftoi12.xyzw vf10, vf9
  c->psubh(a2, a2, a0);                             // psubh a2, a2, a0
  c->dsrav(a2, a2, v1);                             // dsrav a2, a2, v1
  c->ppacb(a2, r0, a2);                             // ppacb a2, r0, a2
  c->and_(a2, a2, a1);                              // and a2, a2, a1
  bc = ((s64)c->sgpr64(a2)) < 0;                    // bltz a2, L20
  c->daddiu(a3, a2, -394);                          // daddiu a3, a2, -394
  if (bc) {goto block_3;}                           // branch non-likely

  bc = ((s64)c->sgpr64(a3)) < 0;                    // bltz a3, L21
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_4;}                           // branch non-likely

  
block_3:
  // nop                                            // sll r0, r0, 0
  
block_4:
  c->sll(a2, a2, 2);                                // sll a2, a2, 2
  c->daddu(a2, gp, a2);                             // daddu a2, gp, a2
  c->mov128_gpr_vf(a3, vf10);                       // qmfc2.i a3, vf10
  c->ppach(a3, r0, a3);                             // ppach a3, r0, a3
  c->daddiu(s4, s4, -1);                            // daddiu s4, s4, -1
  c->sw(a3, 0, a2);                                 // sw a3, 0(a2)
  bc = ((s64)c->sgpr64(s4)) > 0;                    // bgtz s4, L19
  c->daddiu(s5, s5, 32);                            // daddiu s5, s5, 32
  if (bc) {goto block_1;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->ld(fp, 12440, at);                             // ld fp, 12440(at)
  c->lq(gp, 12480, at);                             // lq gp, 12480(at)
  c->lq(s5, 12464, at);                             // lq s5, 12464(at)
  c->lq(s4, 12448, at);                             // lq s4, 12448(at)
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
  gLinkedFunctionTable.reg("generic-warp-source-proc", execute, 128);
}

} // namespace generic_warp_source_proc

namespace generic_none_dma_wait {
u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  [[maybe_unused]] bool bc = false;
  [[maybe_unused]] u32 call_addr = 0;
  c->lui(v1, 4096);                                 // lui v1, 4096
  c->ori(v1, v1, 54272);                            // ori v1, v1, 54272
  
// block_1:
//   c->lw(a0, 0, v1);                                 // lw a0, 0(v1)
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   c->andi(a0, a0, 256);                             // andi a0, a0, 256
//   // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(a0) == 0;                          // beq a0, r0, L24
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_3;}                           // branch non-likely
//
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   // nop                                            // sll r0, r0, 0
//   //beq r0, r0, L23                                 // beq r0, r0, L23
//   // nop                                            // sll r0, r0, 0
//   goto block_1;                                     // branch always

  
// block_3:
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  //jr ra                                           // jr ra
  c->daddu(sp, sp, r0);                             // daddu sp, sp, r0
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  gLinkedFunctionTable.reg("generic-none-dma-wait", execute, 256);
}

} // namespace generic_none_dma_wait

namespace generic_copy_vtx_dclr_dtex {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -96);                           // daddiu sp, sp, -96
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s2, 12448, at);                             // sq s2, 12448(at)
  c->sq(s3, 12464, at);                             // sq s3, 12464(at)
  c->sq(s4, 12480, at);                             // sq s4, 12480(at)
  c->sq(s5, 12496, at);                             // sq s5, 12496(at)
  c->sq(gp, 12512, at);                             // sq gp, 12512(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 44, at);                                // lw a0, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a1, 36, at);                                // lw a1, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 0, a0);                                 // lw v1, 0(a0)
  c->daddiu(a1, a1, 3);                             // daddiu a1, a1, 3
  c->lw(a2, 4, a0);                                 // lw a2, 4(a0)
  c->dsra(a0, a1, 2);                               // dsra a0, a1, 2
  // nop                                            // sll r0, r0, 0
  c->dsll(a0, a0, 3);                               // dsll a0, a0, 3
  c->addiu(a3, r0, 255);                            // addiu a3, r0, 255
  c->lui(a1, -2);                                   // lui a1, -2
  c->addiu(t1, r0, 256);                            // addiu t1, r0, 256
  c->ori(a1, a1, 65534);                            // ori a1, a1, 65534
  c->daddu(a0, v1, a0);                             // daddu a0, v1, a0
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t2, 4, at);                                 // lw t2, 4(at)
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t3, 8, at);                                 // lw t3, 8(at)
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->lw(t4, 12, at);                                // lw t4, 12(at)
  c->pcpyld(a3, a3, a3);                            // pcpyld a3, a3, a3
  c->lq(t0, 12160, at);                             // lq t0, 12160(at)
  c->pcpyh(t1, t1);                                 // pcpyh t1, t1
  c->ld(t5, 0, v1);                                 // ld t5, 0(v1)
  c->pcpyld(t1, t1, t1);                            // pcpyld t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(t6, r0, t5);                            // pextlh t6, r0, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t5, t6, a3);                              // pand t5, t6, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t2, t2, -48);                           // daddiu t2, t2, -48
  c->daddiu(t3, t3, -16);                           // daddiu t3, t3, -16
  //beq r0, r0, L28                                 // beq r0, r0, L28
  c->daddiu(t4, t4, -16);                           // daddiu t4, t4, -16
  goto block_2;                                     // branch always

  
block_1:
  c->pextlh(t6, r0, ra);                            // pextlh t6, r0, ra
  c->sq(t5, 0, t2);                                 // sq t5, 0(t2)
  c->pand(t5, t6, a3);                              // pand t5, t6, a3
  c->sq(t9, 16, t2);                                // sq t9, 16(t2)
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->sq(t7, 32, t2);                                // sq t7, 32(t2)
  
block_2:
  c->paddw(s4, t5, a2);                             // paddw s4, t5, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(gp, s4, 0);                             // dsrl32 gp, s4, 0
  c->daddiu(t2, t2, 48);                            // daddiu t2, t2, 48
  c->pcpyud(ra, s4, r0);                            // pcpyud ra, s4, r0
  c->lq(t5, 0, s4);                                 // lq t5, 0(s4)
  c->dsrl32(t7, ra, 0);                             // dsrl32 t7, ra, 0
  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->pand(t6, t6, t1);                              // pand t6, t6, t1
  c->lw(t9, 16, s4);                                // lw t9, 16(s4)
  c->psraw(t8, t6, 8);                              // psraw t8, t6, 8
  c->lq(s2, 16, gp);                                // lq s2, 16(gp)
  c->daddiu(t4, t4, 16);                            // daddiu t4, t4, 16
  c->lq(s5, 16, gp);                                // lq s5, 16(gp)
  c->daddiu(v1, v1, 8);                             // daddiu v1, v1, 8
  c->lq(s3, 16, gp);                                // lq s3, 16(gp)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, ra);                                 // lq t6, 0(ra)
  c->pextlw(s2, s2, t9);                            // pextlw s2, s2, t9
  c->lq(t9, 0, gp);                                 // lq t9, 0(gp)
  c->pextlw(s3, s3, s5);                            // pextlw s3, s3, s5
  c->lq(s5, 0, t7);                                 // lq s5, 0(t7)
  c->pcpyld(s3, s3, s2);                            // pcpyld s3, s3, s2
  c->lw(s4, 20, s4);                                // lw s4, 20(s4)
  // nop                                            // sll r0, r0, 0
  c->lw(gp, 20, gp);                                // lw gp, 20(gp)
  c->paddh(s3, s3, t0);                             // paddh s3, s3, t0
  c->lw(ra, 20, ra);                                // lw ra, 20(ra)
  c->pand(s3, s3, a1);                              // pand s3, s3, a1
  c->lw(t7, 20, t7);                                // lw t7, 20(t7)
  c->por(s3, s3, t8);                               // por s3, s3, t8
  c->sw(s4, 0, t3);                                 // sw s4, 0(t3)
  c->prot3w(t8, s5);                                // prot3w t8, s5
  c->sq(s3, 0, t4);                                 // sq s3, 0(t4)
  c->prot3w(t9, t9);                                // prot3w t9, t9
  c->sw(gp, 4, t3);                                 // sw gp, 4(t3)
  c->pextuw(gp, t9, t5);                            // pextuw gp, t9, t5
  c->sw(ra, 8, t3);                                 // sw ra, 8(t3)
  c->pcpyld(t9, t6, t9);                            // pcpyld t9, t6, t9
  c->ld(ra, 0, v1);                                 // ld ra, 0(v1)
  c->pcpyld(t5, gp, t5);                            // pcpyld t5, gp, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t6, t8, t6);                            // pextuw t6, t8, t6
  c->sw(t7, 12, t3);                                // sw t7, 12(t3)
  bc = c->sgpr64(v1) != c->sgpr64(a0);              // bne v1, a0, L27
  c->pcpyld(t7, t8, t6);                            // pcpyld t7, t8, t6
  if (bc) {goto block_1;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->sq(t5, 0, t2);                                 // sq t5, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(t9, 16, t2);                                // sq t9, 16(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(t7, 32, t2);                                // sq t7, 32(t2)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12512, at);                             // lq gp, 12512(at)
  c->lq(s5, 12496, at);                             // lq s5, 12496(at)
  c->lq(s4, 12480, at);                             // lq s4, 12480(at)
  c->lq(s3, 12464, at);                             // lq s3, 12464(at)
  c->lq(s2, 12448, at);                             // lq s2, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 96);                            // daddiu sp, sp, 96
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-copy-vtx-dclr-dtex", execute, 256);
}

} // namespace generic_copy_vtx_dclr_dtex

namespace generic_light {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* generic_light_proc; // generic-light-proc
  void* generic_prepare_dma_single; // generic-prepare-dma-single
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  [[maybe_unused]] bool bc = false;
  u32 call_addr = 0;
  u32 qwc = 0;
  u32 madr = 0;
  u32 sadr = 0;
  c->daddiu(sp, sp, -16);                           // daddiu sp, sp, -16
  c->sd(ra, 12176, at);                             // sd ra, 12176(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->sw(a1, 48, at);                                // sw a1, 48(at)
  c->sw(a0, 44, at);                                // sw a0, 44(at)
  c->sh(r0, 56, at);                                // sh r0, 56(at)
  c->load_symbol2(t9, cache.generic_prepare_dma_single);// lw t9, generic-prepare-dma-single(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->load_symbol2(t9, cache.generic_light_proc);    // lw t9, generic-light-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->lw(v1, 24, at);                                // lw v1, 24(at)
  c->lw(a0, 40, at);                                // lw a0, 40(at)
  c->mov64(a3, v1);                                 // or a3, v1, r0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(a2, 4096);                                 // lui a2, 4096
  c->lwu(a1, 60, at);                               // lwu a1, 60(at)
  c->ori(a2, a2, 53248);                            // ori a2, a2, 53248
  // c->lw(t1, 0, a2);                              // lw t1, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, at, 92);                            // daddiu t0, at, 92
  c->andi(a3, a3, 16383);                           // andi a3, a3, 16383
  c->andi(t1, t1, 256);                             // andi t1, t1, 256
  // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(t1) == 0;                          // beq t1, r0, L31
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_4;}                           // branch non-likely

  c->mov64(t1, a2);                                 // or t1, a2, r0
  // nop                                            // sll r0, r0, 0
  
// block_2:
//   c->lw(t2, 0, t0);                                 // lw t2, 0(t0)
//   // nop                                            // sll r0, r0, 0
//   c->lw(t3, 0, t1);                                 // lw t3, 0(t1)
//   // nop                                            // sll r0, r0, 0
//   c->andi(t3, t3, 256);                             // andi t3, t3, 256
//   c->daddiu(t2, t2, 1);                             // daddiu t2, t2, 1
//   bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L30
//   c->sw(t2, 0, t0);                                 // sw t2, 0(t0)
//   if (bc) {goto block_2;}                           // branch non-likely
//
//   c->gprs[t0].du64[0] = 0;                          // or t0, r0, r0
  
// block_4:
  c->dsll(t0, a0, 4);                               // dsll t0, a0, 4
  // c->sw(a3, 128, a2);                            // sw a3, 128(a2)
  sadr = c->sgpr64(a3);
  // nop                                            // sll r0, r0, 0
  // c->sw(a1, 16, a2);                             // sw a1, 16(a2)
  madr = c->sgpr64(a1);
  c->addiu(a3, r0, 256);                            // addiu a3, r0, 256
  // c->sw(a0, 32, a2);                             // sw a0, 32(a2)
  qwc = c->sgpr64(a0);
  c->daddu(a0, a1, t0);                             // daddu a0, a1, t0
  // c->sw(a3, 0, a2);                              // sw a3, 0(a2)
  spad_from_dma_no_sadr_off(cache.fake_scratchpad_data, madr, sadr, qwc);
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 60, at);                                // sw a0, 60(at)
  c->gprs[a0].du64[0] = 0;                          // or a0, r0, r0
  c->xori(v1, v1, 4608);                            // xori v1, v1, 4608
  c->sw(v1, 24, at);                                // sw v1, 24(at)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12176, at);                             // ld ra, 12176(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 16);                            // daddiu sp, sp, 16
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  cache.generic_light_proc = intern_from_c(-1, 0, "generic-light-proc").c();
  cache.generic_prepare_dma_single = intern_from_c(-1, 0, "generic-prepare-dma-single").c();
  gLinkedFunctionTable.reg("generic-light", execute, 256);
}

} // namespace generic_light

namespace generic_envmap_only_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -128);                          // daddiu sp, sp, -128
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s0, 12448, at);                             // sq s0, 12448(at)
  c->sq(s1, 12464, at);                             // sq s1, 12464(at)
  c->sq(s2, 12480, at);                             // sq s2, 12480(at)
  c->sq(s3, 12496, at);                             // sq s3, 12496(at)
  c->sq(s4, 12512, at);                             // sq s4, 12512(at)
  c->sq(s5, 12528, at);                             // sq s5, 12528(at)
  c->sq(gp, 12544, at);                             // sq gp, 12544(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 44, at);                                // lw a0, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 36, at);                                // lw v1, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t1, 0, a0);                                 // lw t1, 0(a0)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 4, a0);                                 // lw a0, 4(a0)
  // nop                                            // sll r0, r0, 0
  c->lw(a3, 4, at);                                 // lw a3, 4(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t4, 8, at);                                 // lw t4, 8(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a2, 12, at);                                // lw a2, 12(at)
  // nop                                            // sll r0, r0, 0
  c->addiu(t2, r0, 255);                            // addiu t2, r0, 255
  c->addiu(a1, r0, 256);                            // addiu a1, r0, 256
  c->lui(t3, -2);                                   // lui t3, -2
  c->lui(t0, 16256);                                // lui t0, 16256
  c->ori(t3, t3, 65534);                            // ori t3, t3, 65534
  c->mtc1(f0, t0);                                  // mtc1 f0, t0
  c->daddiu(t0, v1, 3);                             // daddiu t0, v1, 3
  c->sra(t6, t0, 2);                                // sra t6, t0, 2
  c->lq(t0, 12048, at);                             // lq t0, 12048(at)
  c->sra(t5, t6, 2);                                // sra t5, t6, 2
  c->andi(t6, t6, 3);                               // andi t6, t6, 3
  bc = c->sgpr64(t5) == 0;                          // beq t5, r0, L34
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_2;}                           // branch non-likely

  
block_1:
  c->daddiu(t4, t4, 64);                            // daddiu t4, t4, 64
  c->sq(t0, -64, t4);                               // sq t0, -64(t4)
  // nop                                            // sll r0, r0, 0
  c->sq(t0, -48, t4);                               // sq t0, -48(t4)
  c->daddiu(t5, t5, -1);                            // daddiu t5, t5, -1
  c->sq(t0, -32, t4);                               // sq t0, -32(t4)
  bc = ((s64)c->sgpr64(t5)) > 0;                    // bgtz t5, L33
  c->sq(t0, -16, t4);                               // sq t0, -16(t4)
  if (bc) {goto block_1;}                           // branch non-likely

  
block_2:
  bc = c->sgpr64(t6) == 0;                          // beq t6, r0, L35
  c->daddiu(t5, t6, -1);                            // daddiu t5, t6, -1
  if (bc) {goto block_6;}                           // branch non-likely

  bc = c->sgpr64(t5) == 0;                          // beq t5, r0, L35
  c->sq(t0, 0, t4);                                 // sq t0, 0(t4)
  if (bc) {goto block_6;}                           // branch non-likely

  c->daddiu(t4, t4, 16);                            // daddiu t4, t4, 16
  c->daddiu(t5, t5, -1);                            // daddiu t5, t5, -1
  bc = c->sgpr64(t5) == 0;                          // beq t5, r0, L35
  c->sq(t0, 0, t4);                                 // sq t0, 0(t4)
  if (bc) {goto block_6;}                           // branch non-likely

  c->daddiu(t4, t4, 16);                            // daddiu t4, t4, 16
  c->daddiu(t5, t5, -1);                            // daddiu t5, t5, -1
  // nop                                            // sll r0, r0, 0
  c->sq(t0, 0, t4);                                 // sq t0, 0(t4)
  
block_6:
  c->daddiu(t0, v1, -4);                            // daddiu t0, v1, -4
  c->lqc2(vf31, 12016, at);                         // lqc2 vf31, 12016(at)
  c->pextlw(v1, t3, t3);                            // pextlw v1, t3, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(v1, v1, v1);                            // pextlw v1, v1, v1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a0, a0, a0);                            // pextlw a0, a0, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a0, a0, a0);                            // pextlw a0, a0, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyh(t3, t2);                                 // pcpyh t3, t2
  c->ldr(t2, 0, t1);                                // ldr t2, 0(t1)
  c->pcpyld(t3, t3, t3);                            // pcpyld t3, t3, t3
  c->ldl(t2, 7, t1);                                // ldl t2, 7(t1)
  c->pcpyh(a1, a1);                                 // pcpyh a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a1, a1, a1);                            // pcpyld a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t1, t1, 8);                             // daddiu t1, t1, 8
  c->sq(t3, 96, at);                                // sq t3, 96(at)
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t3, t2, t3);                              // pand t3, t2, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t3, t3, 5);                              // psllw t3, t3, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t7, t3, a0);                             // paddw t7, t3, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f4, 24, t7);                              // lwc1 f4, 24(t7)
  c->pcpyud(t9, t7, r0);                            // pcpyud t9, t7, r0
  c->lwc1(f3, 24, t8);                              // lwc1 f3, 24(t8)
  c->dsrl32(ra, t9, 0);                             // dsrl32 ra, t9, 0
  c->lwc1(f2, 24, t9);                              // lwc1 f2, 24(t9)
  c->pand(t3, t2, a1);                              // pand t3, t2, a1
  c->lwc1(f1, 24, ra);                              // lwc1 f1, 24(ra)
  c->psraw(gp, t3, 8);                              // psraw gp, t3, 8
  c->lq(t3, 16, t7);                                // lq t3, 16(t7)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t4, 16, t8);                                // lq t4, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 16, t9);                                // lq t5, 16(t9)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 16, ra);                                // lq t6, 16(ra)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t9, 0, t9);                                 // lq t9, 0(t9)
  // nop                                            // sll r0, r0, 0
  c->lq(ra, 0, ra);                                 // lq ra, 0(ra)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  c->sq(t7, 112, at);                               // sq t7, 112(at)
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->sq(t8, 128, at);                               // sq t8, 128(at)
  // nop                                            // sll r0, r0, 0
  c->sq(t9, 144, at);                               // sq t9, 144(at)
  // nop                                            // sll r0, r0, 0
  c->sq(ra, 160, at);                               // sq ra, 160(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f4);                                  // mfc1 s5, f4
  // nop                                            // sll r0, r0, 0
  c->sq(gp, 304, at);                               // sq gp, 304(at)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(gp, f3);                                  // mfc1 gp, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(gp, gp, s5);                            // pextlw gp, gp, s5
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f2);                                  // mfc1 s5, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s4, f1);                                  // mfc1 s4, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->pextlw(s5, s4, s5);                            // pextlw s5, s4, s5
  // nop                                            // sll r0, r0, 0
  c->pcpyld(gp, s5, gp);                            // pcpyld gp, s5, gp
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t3);                       // qmtc2.ni vf21, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t4);                       // qmtc2.ni vf22, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t5);                       // qmtc2.ni vf23, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t6);                       // qmtc2.ni vf24, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t7);                        // qmtc2.ni vf9, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t8);                       // qmtc2.ni vf10, t8
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t9);                       // qmtc2.ni vf11, t9
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, ra);                       // qmtc2.ni vf12, ra
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, gp);                       // qmtc2.ni vf27, gp
  c->lq(t3, 96, at);                                // lq t3, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->ldr(t2, 0, t1);                                // ldr t2, 0(t1)
  // nop                                            // sll r0, r0, 0
  c->ldl(t2, 7, t1);                                // ldl t2, 7(t1)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t1, t1, 8);                             // daddiu t1, t1, 8
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t3, t2, t3);                              // pand t3, t2, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t3, t3, 5);                              // psllw t3, t3, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t7, t3, a0);                             // paddw t7, t3, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f4, 24, t7);                              // lwc1 f4, 24(t7)
  c->pcpyud(ra, t7, r0);                            // pcpyud ra, t7, r0
  c->lwc1(f3, 24, t8);                              // lwc1 f3, 24(t8)
  c->dsrl32(gp, ra, 0);                             // dsrl32 gp, ra, 0
  c->lwc1(f2, 24, ra);                              // lwc1 f2, 24(ra)
  c->pand(t3, t2, a1);                              // pand t3, t2, a1
  c->lwc1(f1, 24, gp);                              // lwc1 f1, 24(gp)
  c->psraw(t9, t3, 8);                              // psraw t9, t3, 8
  c->lq(t3, 16, t7);                                // lq t3, 16(t7)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t4, 16, t8);                                // lq t4, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 16, ra);                                // lq t5, 16(ra)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 16, gp);                                // lq t6, 16(gp)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(ra, 0, ra);                                 // lq ra, 0(ra)
  // nop                                            // sll r0, r0, 0
  c->lq(gp, 0, gp);                                 // lq gp, 0(gp)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  c->sq(t7, 176, at);                               // sq t7, 176(at)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->sq(t8, 192, at);                               // sq t8, 192(at)
  // nop                                            // sll r0, r0, 0
  c->sq(ra, 208, at);                               // sq ra, 208(at)
  // nop                                            // sll r0, r0, 0
  c->sq(gp, 224, at);                               // sq gp, 224(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f4);                                  // mfc1 s5, f4
  // nop                                            // sll r0, r0, 0
  c->sq(t9, 320, at);                               // sq t9, 320(at)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f3);                                  // mfc1 t9, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t9, t9, s5);                            // pextlw t9, t9, s5
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f2);                                  // mfc1 s5, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s4, f1);                                  // mfc1 s4, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t3);                       // qmtc2.ni vf21, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t7);                        // qmtc2.ni vf9, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t8);                       // qmtc2.ni vf10, t8
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, ra);                       // qmtc2.ni vf11, ra
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, gp);                       // qmtc2.ni vf12, gp
  c->pextlw(t3, s4, s5);                            // pextlw t3, s4, s5
  // nop                                            // sll r0, r0, 0
  c->pextlw(t3, t3, t9);                            // pextlw t3, t3, t9
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t4);                       // qmtc2.ni vf22, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t5);                       // qmtc2.ni vf23, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t6);                       // qmtc2.ni vf24, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t3);                       // qmtc2.ni vf27, t3
  c->lq(t3, 96, at);                                // lq t3, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->ldr(t2, 0, t1);                                // ldr t2, 0(t1)
  // nop                                            // sll r0, r0, 0
  c->ldl(t2, 7, t1);                                // ldl t2, 7(t1)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t1, t1, 8);                             // daddiu t1, t1, 8
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t3, t2, t3);                              // pand t3, t2, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t3, t3, 5);                              // psllw t3, t3, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t5, t3, a0);                             // paddw t5, t3, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t6, t5, 0);                             // dsrl32 t6, t5, 0
  c->lwc1(f4, 24, t5);                              // lwc1 f4, 24(t5)
  c->pcpyud(t7, t5, r0);                            // pcpyud t7, t5, r0
  c->lwc1(f3, 24, t6);                              // lwc1 f3, 24(t6)
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f2, 24, t7);                              // lwc1 f2, 24(t7)
  c->pand(t3, t2, a1);                              // pand t3, t2, a1
  c->lwc1(f1, 24, t8);                              // lwc1 f1, 24(t8)
  c->psraw(t3, t3, 8);                              // psraw t3, t3, 8
  c->lq(t4, 16, t5);                                // lq t4, 16(t5)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t9, 16, t6);                                // lq t9, 16(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(ra, 16, t7);                                // lq ra, 16(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(gp, 16, t8);                                // lq gp, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 0, t5);                                 // lq t5, 0(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, t6);                                 // lq t6, 0(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  c->sq(t5, 240, at);                               // sq t5, 240(at)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->sq(t6, 256, at);                               // sq t6, 256(at)
  // nop                                            // sll r0, r0, 0
  c->sq(t7, 272, at);                               // sq t7, 272(at)
  // nop                                            // sll r0, r0, 0
  c->sq(t8, 288, at);                               // sq t8, 288(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f4);                                  // mfc1 s5, f4
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 336, at);                               // sq t3, 336(at)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s4, f3);                                  // mfc1 s4, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  c->lq(t3, 304, at);                               // lq t3, 304(at)
  c->pextlw(s5, s4, s5);                            // pextlw s5, s4, s5
  c->lq(s4, 128, at);                               // lq s4, 128(at)
  // nop                                            // sll r0, r0, 0
  c->lq(s2, 160, at);                               // lq s2, 160(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s3, f2);                                  // mfc1 s3, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t4);                       // qmtc2.ni vf21, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t9);                       // qmtc2.ni vf22, t9
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, ra);                       // qmtc2.ni vf23, ra
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, gp);                       // qmtc2.ni vf24, gp
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 144, at);                               // lq t4, 144(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s1, f1);                                  // mfc1 s1, f1
  c->prot3w(ra, s2);                                // prot3w ra, s2
  c->lq(t9, 112, at);                               // lq t9, 112(at)
  c->prot3w(gp, s4);                                // prot3w gp, s4
  c->mov128_vf_gpr(vf9, t5);                        // qmtc2.ni vf9, t5
  c->pextlw(t5, s1, s3);                            // pextlw t5, s1, s3
  c->mov128_vf_gpr(vf10, t6);                       // qmtc2.ni vf10, t6
  c->pcpyld(t5, t5, s5);                            // pcpyld t5, t5, s5
  c->mov128_vf_gpr(vf11, t7);                       // qmtc2.ni vf11, t7
  c->pextuw(t7, gp, t9);                            // pextuw t7, gp, t9
  c->mov128_vf_gpr(vf12, t8);                       // qmtc2.ni vf12, t8
  c->pcpyld(t6, t4, gp);                            // pcpyld t6, t4, gp
  c->mov128_vf_gpr(vf27, t5);                       // qmtc2.ni vf27, t5
  c->pcpyld(s5, t7, t9);                            // pcpyld s5, t7, t9
  c->mov128_gpr_vf(t9, vf17);                       // qmfc2.ni t9, vf17
  c->pextuw(t4, ra, t4);                            // pextuw t4, ra, t4
  c->mov128_gpr_vf(t7, vf18);                       // qmfc2.ni t7, vf18
  c->pcpyld(ra, ra, t4);                            // pcpyld ra, ra, t4
  c->mov128_gpr_vf(gp, vf19);                       // qmfc2.ni gp, vf19
  bc = ((s64)c->sgpr64(t0)) <= 0;                   // blez t0, L37
  c->mov128_gpr_vf(t8, vf20);                       // qmfc2.ni t8, vf20
  if (bc) {goto block_10;}                          // branch non-likely

  
block_7:
  c->lq(s4, 96, at);                                // lq s4, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  c->daddiu(t0, t0, -4);                            // daddiu t0, t0, -4
  c->ldr(t2, 0, t1);                                // ldr t2, 0(t1)
  c->daddiu(t4, a3, 48);                            // daddiu t4, a3, 48
  c->ldl(t2, 7, t1);                                // ldl t2, 7(t1)
  c->daddiu(a3, a2, 16);                            // daddiu a3, a2, 16
  c->daddiu(t5, t1, 8);                             // daddiu t5, t1, 8
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->sq(s5, -48, t4);                               // sq s5, -48(t4)
  c->pand(a2, t2, s4);                              // pand a2, t2, s4
  c->sq(t6, -32, t4);                               // sq t6, -32(t4)
  c->psllw(a2, a2, 5);                              // psllw a2, a2, 5
  c->sq(ra, -16, t4);                               // sq ra, -16(t4)
  c->paddw(s2, a2, a0);                             // paddw s2, a2, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s3, s2, 0);                             // dsrl32 s3, s2, 0
  c->lwc1(f1, 24, s2);                              // lwc1 f1, 24(s2)
  c->pcpyud(s4, s2, r0);                            // pcpyud s4, s2, r0
  c->lwc1(f4, 24, s3);                              // lwc1 f4, 24(s3)
  c->subs(f3, f1, f0);                              // sub.s f3, f1, f0
  // nop                                            // sll r0, r0, 0
  c->dsrl32(s5, s4, 0);                             // dsrl32 s5, s4, 0
  c->lwc1(f2, 24, s4);                              // lwc1 f2, 24(s4)
  c->pand(a2, t2, a1);                              // pand a2, t2, a1
  c->lwc1(f1, 24, s5);                              // lwc1 f1, 24(s5)
  c->psraw(a2, a2, 8);                              // psraw a2, a2, 8
  c->lq(t1, 16, s2);                                // lq t1, 16(s2)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->lq(t6, 16, s3);                                // lq t6, 16(s3)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->ppach(s1, r0, t9);                             // ppach s1, r0, t9
  c->lq(t9, 16, s4);                                // lq t9, 16(s4)
  c->ppach(v0, r0, t7);                             // ppach v0, r0, t7
  c->lq(ra, 16, s5);                                // lq ra, 16(s5)
  c->ppach(s0, r0, gp);                             // ppach s0, r0, gp
  c->lq(t7, 0, s2);                                 // lq t7, 0(s2)
  c->ppach(s2, r0, t8);                             // ppach s2, r0, t8
  c->lq(t8, 0, s3);                                 // lq t8, 0(s3)
  c->pextlw(s3, v0, s1);                            // pextlw s3, v0, s1
  c->lq(gp, 0, s4);                                 // lq gp, 0(s4)
  c->pextlw(s4, s2, s0);                            // pextlw s4, s2, s0
  c->lq(s5, 0, s5);                                 // lq s5, 0(s5)
  c->muls(f5, f3, f0);                              // mul.s f5, f3, f0
  c->sq(t7, 112, at);                               // sq t7, 112(at)
  c->divs(f3, f0, f4);                              // div.s f3, f0, f4
  c->sq(t8, 128, at);                               // sq t8, 128(at)
  c->pcpyld(s4, s4, s3);                            // pcpyld s4, s4, s3
  c->sq(gp, 144, at);                               // sq gp, 144(at)
  c->pand(s4, s4, v1);                              // pand s4, s4, v1
  c->sq(s5, 160, at);                               // sq s5, 160(at)
  c->por(s4, s4, t3);                               // por s4, s4, t3
  c->mfc1(t3, f5);                                  // mfc1 t3, f5
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  c->sq(a2, 304, at);                               // sq a2, 304(at)
  // nop                                            // sll r0, r0, 0
  c->sq(s4, -16, a3);                               // sq s4, -16(a3)
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(a2, f3);                                  // mfc1 a2, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->pextlw(t3, a2, t3);                            // pextlw t3, a2, t3
  c->lq(a2, 320, at);                               // lq a2, 320(at)
  // nop                                            // sll r0, r0, 0
  c->lq(s4, 192, at);                               // lq s4, 192(at)
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  c->lq(s2, 224, at);                               // lq s2, 224(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s3, f2);                                  // mfc1 s3, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t1);                       // qmtc2.ni vf21, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t6);                       // qmtc2.ni vf22, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t9);                       // qmtc2.ni vf23, t9
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, ra);                       // qmtc2.ni vf24, ra
  // nop                                            // sll r0, r0, 0
  c->lq(ra, 208, at);                               // lq ra, 208(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t9, 176, at);                               // lq t9, 176(at)
  c->prot3w(t1, s2);                                // prot3w t1, s2
  c->mfc1(s2, f1);                                  // mfc1 s2, f1
  c->prot3w(t6, s4);                                // prot3w t6, s4
  c->mov128_vf_gpr(vf9, t7);                        // qmtc2.ni vf9, t7
  c->pextlw(t7, s2, s3);                            // pextlw t7, s2, s3
  c->mov128_vf_gpr(vf10, t8);                       // qmtc2.ni vf10, t8
  c->pcpyld(t3, t7, t3);                            // pcpyld t3, t7, t3
  c->mov128_vf_gpr(vf11, gp);                       // qmtc2.ni vf11, gp
  c->pextuw(t7, t6, t9);                            // pextuw t7, t6, t9
  c->mov128_vf_gpr(vf12, s5);                       // qmtc2.ni vf12, s5
  c->pcpyld(t6, ra, t6);                            // pcpyld t6, ra, t6
  c->mov128_vf_gpr(vf27, t3);                       // qmtc2.ni vf27, t3
  c->pcpyld(gp, t7, t9);                            // pcpyld gp, t7, t9
  c->mov128_gpr_vf(t9, vf17);                       // qmfc2.ni t9, vf17
  c->pextuw(t3, t1, ra);                            // pextuw t3, t1, ra
  c->mov128_gpr_vf(t8, vf18);                       // qmfc2.ni t8, vf18
  c->pcpyld(ra, t1, t3);                            // pcpyld ra, t1, t3
  c->mov128_gpr_vf(t7, vf19);                       // qmfc2.ni t7, vf19
  bc = ((s64)c->sgpr64(t0)) <= 0;                   // blez t0, L38
  c->mov128_gpr_vf(t3, vf20);                       // qmfc2.ni t3, vf20
  if (bc) {goto block_11;}                          // branch non-likely

  c->lq(s5, 96, at);                                // lq s5, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  c->daddiu(t0, t0, -4);                            // daddiu t0, t0, -4
  c->ldr(t2, 0, t5);                                // ldr t2, 0(t5)
  c->daddiu(t4, t4, 48);                            // daddiu t4, t4, 48
  c->ldl(t2, 7, t5);                                // ldl t2, 7(t5)
  c->daddiu(t1, a3, 16);                            // daddiu t1, a3, 16
  c->daddiu(t5, t5, 8);                             // daddiu t5, t5, 8
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->sq(gp, -48, t4);                               // sq gp, -48(t4)
  c->pand(a3, t2, s5);                              // pand a3, t2, s5
  c->sq(t6, -32, t4);                               // sq t6, -32(t4)
  c->psllw(a3, a3, 5);                              // psllw a3, a3, 5
  c->sq(ra, -16, t4);                               // sq ra, -16(t4)
  c->paddw(s2, a3, a0);                             // paddw s2, a3, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s3, s2, 0);                             // dsrl32 s3, s2, 0
  c->lwc1(f1, 24, s2);                              // lwc1 f1, 24(s2)
  c->pcpyud(gp, s2, r0);                            // pcpyud gp, s2, r0
  c->lwc1(f4, 24, s3);                              // lwc1 f4, 24(s3)
  c->subs(f3, f1, f0);                              // sub.s f3, f1, f0
  // nop                                            // sll r0, r0, 0
  c->dsrl32(s5, gp, 0);                             // dsrl32 s5, gp, 0
  c->lwc1(f2, 24, gp);                              // lwc1 f2, 24(gp)
  c->pand(a3, t2, a1);                              // pand a3, t2, a1
  c->lwc1(f1, 24, s5);                              // lwc1 f1, 24(s5)
  c->psraw(s4, a3, 8);                              // psraw s4, a3, 8
  c->lq(a3, 16, s2);                                // lq a3, 16(s2)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->lq(t6, 16, s3);                                // lq t6, 16(s3)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->ppach(s1, r0, t9);                             // ppach s1, r0, t9
  c->lq(t9, 16, gp);                                // lq t9, 16(gp)
  c->ppach(v0, r0, t8);                             // ppach v0, r0, t8
  c->lq(ra, 16, s5);                                // lq ra, 16(s5)
  c->ppach(s0, r0, t7);                             // ppach s0, r0, t7
  c->lq(t7, 0, s2);                                 // lq t7, 0(s2)
  c->ppach(s2, r0, t3);                             // ppach s2, r0, t3
  c->lq(t8, 0, s3);                                 // lq t8, 0(s3)
  c->pextlw(t3, v0, s1);                            // pextlw t3, v0, s1
  c->lq(gp, 0, gp);                                 // lq gp, 0(gp)
  c->pextlw(s3, s2, s0);                            // pextlw s3, s2, s0
  c->lq(s5, 0, s5);                                 // lq s5, 0(s5)
  c->muls(f5, f3, f0);                              // mul.s f5, f3, f0
  c->sq(t7, 176, at);                               // sq t7, 176(at)
  c->divs(f3, f0, f4);                              // div.s f3, f0, f4
  c->sq(t8, 192, at);                               // sq t8, 192(at)
  c->pcpyld(t3, s3, t3);                            // pcpyld t3, s3, t3
  c->sq(gp, 208, at);                               // sq gp, 208(at)
  c->pand(t3, t3, v1);                              // pand t3, t3, v1
  c->sq(s5, 224, at);                               // sq s5, 224(at)
  c->por(t3, t3, a2);                               // por t3, t3, a2
  c->mfc1(a2, f5);                                  // mfc1 a2, f5
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  c->sq(s4, 320, at);                               // sq s4, 320(at)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, -16, t1);                               // sq t3, -16(t1)
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t3, f3);                                  // mfc1 t3, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->pextlw(a2, t3, a2);                            // pextlw a2, t3, a2
  c->lq(t3, 336, at);                               // lq t3, 336(at)
  // nop                                            // sll r0, r0, 0
  c->lq(s4, 256, at);                               // lq s4, 256(at)
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  c->lq(s2, 288, at);                               // lq s2, 288(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s3, f2);                                  // mfc1 s3, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, a3);                       // qmtc2.ni vf21, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t6);                       // qmtc2.ni vf22, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t9);                       // qmtc2.ni vf23, t9
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, ra);                       // qmtc2.ni vf24, ra
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 272, at);                               // lq t6, 272(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t9, 240, at);                               // lq t9, 240(at)
  c->prot3w(a3, s2);                                // prot3w a3, s2
  c->mfc1(s2, f1);                                  // mfc1 s2, f1
  c->prot3w(ra, s4);                                // prot3w ra, s4
  c->mov128_vf_gpr(vf9, t7);                        // qmtc2.ni vf9, t7
  c->pextlw(t7, s2, s3);                            // pextlw t7, s2, s3
  c->mov128_vf_gpr(vf10, t8);                       // qmtc2.ni vf10, t8
  c->pcpyld(a2, t7, a2);                            // pcpyld a2, t7, a2
  c->mov128_vf_gpr(vf11, gp);                       // qmtc2.ni vf11, gp
  c->pextuw(t7, ra, t9);                            // pextuw t7, ra, t9
  c->mov128_vf_gpr(vf12, s5);                       // qmtc2.ni vf12, s5
  c->pcpyld(ra, t6, ra);                            // pcpyld ra, t6, ra
  c->mov128_vf_gpr(vf27, a2);                       // qmtc2.ni vf27, a2
  c->pcpyld(s5, t7, t9);                            // pcpyld s5, t7, t9
  c->mov128_gpr_vf(t8, vf17);                       // qmfc2.ni t8, vf17
  c->pextuw(a2, a3, t6);                            // pextuw a2, a3, t6
  c->mov128_gpr_vf(t9, vf18);                       // qmfc2.ni t9, vf18
  c->pcpyld(gp, a3, a2);                            // pcpyld gp, a3, a2
  c->mov128_gpr_vf(t6, vf19);                       // qmfc2.ni t6, vf19
  bc = ((s64)c->sgpr64(t0)) <= 0;                   // blez t0, L39
  c->mov128_gpr_vf(t7, vf20);                       // qmfc2.ni t7, vf20
  if (bc) {goto block_12;}                          // branch non-likely

  c->lq(s4, 96, at);                                // lq s4, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  c->daddiu(t0, t0, -4);                            // daddiu t0, t0, -4
  c->ldr(t2, 0, t5);                                // ldr t2, 0(t5)
  c->daddiu(a3, t4, 48);                            // daddiu a3, t4, 48
  c->ldl(t2, 7, t5);                                // ldl t2, 7(t5)
  c->daddiu(a2, t1, 16);                            // daddiu a2, t1, 16
  c->daddiu(t1, t5, 8);                             // daddiu t1, t5, 8
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->sq(s5, -48, a3);                               // sq s5, -48(a3)
  c->pand(t4, t2, s4);                              // pand t4, t2, s4
  c->sq(ra, -32, a3);                               // sq ra, -32(a3)
  c->psllw(t4, t4, 5);                              // psllw t4, t4, 5
  c->sq(gp, -16, a3);                               // sq gp, -16(a3)
  c->paddw(s3, t4, a0);                             // paddw s3, t4, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s4, s3, 0);                             // dsrl32 s4, s3, 0
  c->lwc1(f1, 24, s3);                              // lwc1 f1, 24(s3)
  c->pcpyud(ra, s3, r0);                            // pcpyud ra, s3, r0
  c->lwc1(f4, 24, s4);                              // lwc1 f4, 24(s4)
  c->subs(f3, f1, f0);                              // sub.s f3, f1, f0
  // nop                                            // sll r0, r0, 0
  c->dsrl32(gp, ra, 0);                             // dsrl32 gp, ra, 0
  c->lwc1(f2, 24, ra);                              // lwc1 f2, 24(ra)
  c->pand(t4, t2, a1);                              // pand t4, t2, a1
  c->lwc1(f1, 24, gp);                              // lwc1 f1, 24(gp)
  c->psraw(s5, t4, 8);                              // psraw s5, t4, 8
  c->lq(t4, 16, s3);                                // lq t4, 16(s3)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->lq(t5, 16, s4);                                // lq t5, 16(s4)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->ppach(s2, r0, t8);                             // ppach s2, r0, t8
  c->lq(t8, 16, ra);                                // lq t8, 16(ra)
  c->ppach(s0, r0, t9);                             // ppach s0, r0, t9
  c->lq(t9, 16, gp);                                // lq t9, 16(gp)
  c->ppach(s1, r0, t6);                             // ppach s1, r0, t6
  c->lq(t6, 0, s3);                                 // lq t6, 0(s3)
  c->ppach(s3, r0, t7);                             // ppach s3, r0, t7
  c->lq(t7, 0, s4);                                 // lq t7, 0(s4)
  c->pextlw(s4, s0, s2);                            // pextlw s4, s0, s2
  c->lq(ra, 0, ra);                                 // lq ra, 0(ra)
  c->pextlw(s3, s3, s1);                            // pextlw s3, s3, s1
  c->lq(gp, 0, gp);                                 // lq gp, 0(gp)
  c->muls(f5, f3, f0);                              // mul.s f5, f3, f0
  c->sq(t6, 240, at);                               // sq t6, 240(at)
  c->divs(f3, f0, f4);                              // div.s f3, f0, f4
  c->sq(t7, 256, at);                               // sq t7, 256(at)
  c->pcpyld(s4, s3, s4);                            // pcpyld s4, s3, s4
  c->sq(ra, 272, at);                               // sq ra, 272(at)
  c->pand(s4, s4, v1);                              // pand s4, s4, v1
  c->sq(gp, 288, at);                               // sq gp, 288(at)
  c->por(s4, s4, t3);                               // por s4, s4, t3
  c->mfc1(t3, f5);                                  // mfc1 t3, f5
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  c->sq(s5, 336, at);                               // sq s5, 336(at)
  // nop                                            // sll r0, r0, 0
  c->sq(s4, -16, a2);                               // sq s4, -16(a2)
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(s5, f3);                                  // mfc1 s5, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->pextlw(s5, s5, t3);                            // pextlw s5, s5, t3
  c->lq(t3, 304, at);                               // lq t3, 304(at)
  // nop                                            // sll r0, r0, 0
  c->lq(s4, 128, at);                               // lq s4, 128(at)
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  c->lq(s2, 160, at);                               // lq s2, 160(at)
  // nop                                            // sll r0, r0, 0
  c->mfc1(s3, f2);                                  // mfc1 s3, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t4);                       // qmtc2.ni vf21, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t5);                       // qmtc2.ni vf22, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t8);                       // qmtc2.ni vf23, t8
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t9);                       // qmtc2.ni vf24, t9
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 144, at);                               // lq t5, 144(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 112, at);                               // lq t8, 112(at)
  c->prot3w(t4, s2);                                // prot3w t4, s2
  c->mfc1(s2, f1);                                  // mfc1 s2, f1
  c->prot3w(t9, s4);                                // prot3w t9, s4
  c->mov128_vf_gpr(vf9, t6);                        // qmtc2.ni vf9, t6
  c->pextlw(t6, s2, s3);                            // pextlw t6, s2, s3
  c->mov128_vf_gpr(vf10, t7);                       // qmtc2.ni vf10, t7
  c->pcpyld(t7, t6, s5);                            // pcpyld t7, t6, s5
  c->mov128_vf_gpr(vf11, ra);                       // qmtc2.ni vf11, ra
  c->pextuw(ra, t9, t8);                            // pextuw ra, t9, t8
  c->mov128_vf_gpr(vf12, gp);                       // qmtc2.ni vf12, gp
  c->pcpyld(t6, t5, t9);                            // pcpyld t6, t5, t9
  c->mov128_vf_gpr(vf27, t7);                       // qmtc2.ni vf27, t7
  c->pcpyld(s5, ra, t8);                            // pcpyld s5, ra, t8
  c->mov128_gpr_vf(t9, vf17);                       // qmfc2.ni t9, vf17
  c->pextuw(t5, t4, t5);                            // pextuw t5, t4, t5
  c->mov128_gpr_vf(t7, vf18);                       // qmfc2.ni t7, vf18
  c->pcpyld(ra, t4, t5);                            // pcpyld ra, t4, t5
  c->mov128_gpr_vf(gp, vf19);                       // qmfc2.ni gp, vf19
  bc = ((s64)c->sgpr64(t0)) > 0;                    // bgtz t0, L36
  c->mov128_gpr_vf(t8, vf20);                       // qmfc2.ni t8, vf20
  if (bc) {goto block_7;}                           // branch non-likely

  
block_10:
  c->daddiu(a1, a3, 48);                            // daddiu a1, a3, 48
  c->sq(s5, -48, a1);                               // sq s5, -48(a1)
  c->daddiu(a0, a2, 16);                            // daddiu a0, a2, 16
  c->sq(t6, -32, a1);                               // sq t6, -32(a1)
  c->ppach(a2, r0, t9);                             // ppach a2, r0, t9
  c->sq(ra, -16, a1);                               // sq ra, -16(a1)
  c->ppach(a1, r0, t7);                             // ppach a1, r0, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(a3, r0, gp);                             // ppach a3, r0, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t0, r0, t8);                             // ppach t0, r0, t8
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a2);                            // pextlw a1, a1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, t0, a3);                            // pextlw a2, t0, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a1, a2, a1);                            // pcpyld a1, a2, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(v1, a1, v1);                              // pand v1, a1, v1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->por(v1, v1, t3);                               // por v1, v1, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  //beq r0, r0, L40                                 // beq r0, r0, L40
  c->sq(v1, -16, a0);                               // sq v1, -16(a0)
  goto block_13;                                    // branch always

  
block_11:
  c->daddiu(a1, t4, 48);                            // daddiu a1, t4, 48
  c->sq(gp, -48, a1);                               // sq gp, -48(a1)
  c->daddiu(a0, a3, 16);                            // daddiu a0, a3, 16
  c->sq(t6, -32, a1);                               // sq t6, -32(a1)
  c->ppach(a3, r0, t9);                             // ppach a3, r0, t9
  c->sq(ra, -16, a1);                               // sq ra, -16(a1)
  c->ppach(a1, r0, t8);                             // ppach a1, r0, t8
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t0, r0, t7);                             // ppach t0, r0, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t1, r0, t3);                             // ppach t1, r0, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a3);                            // pextlw a1, a1, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a3, t1, t0);                            // pextlw a3, t1, t0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a1, a3, a1);                            // pcpyld a1, a3, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(v1, a1, v1);                              // pand v1, a1, v1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->por(v1, v1, a2);                               // por v1, v1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  //beq r0, r0, L40                                 // beq r0, r0, L40
  c->sq(v1, -16, a0);                               // sq v1, -16(a0)
  goto block_13;                                    // branch always

  
block_12:
  c->daddiu(a1, t4, 48);                            // daddiu a1, t4, 48
  c->sq(s5, -48, a1);                               // sq s5, -48(a1)
  c->daddiu(a0, t1, 16);                            // daddiu a0, t1, 16
  c->sq(ra, -32, a1);                               // sq ra, -32(a1)
  c->ppach(a2, r0, t8);                             // ppach a2, r0, t8
  c->sq(gp, -16, a1);                               // sq gp, -16(a1)
  c->ppach(a1, r0, t9);                             // ppach a1, r0, t9
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(a3, r0, t6);                             // ppach a3, r0, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t0, r0, t7);                             // ppach t0, r0, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a2);                            // pextlw a1, a1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, t0, a3);                            // pextlw a2, t0, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a1, a2, a1);                            // pcpyld a1, a2, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(v1, a1, v1);                              // pand v1, a1, v1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->por(v1, v1, t3);                               // por v1, v1, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->sq(v1, -16, a0);                               // sq v1, -16(a0)
  
block_13:
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12544, at);                             // lq gp, 12544(at)
  c->lq(s5, 12528, at);                             // lq s5, 12528(at)
  c->lq(s4, 12512, at);                             // lq s4, 12512(at)
  c->lq(s3, 12496, at);                             // lq s3, 12496(at)
  c->lq(s2, 12480, at);                             // lq s2, 12480(at)
  c->lq(s1, 12464, at);                             // lq s1, 12464(at)
  c->lq(s0, 12448, at);                             // lq s0, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 128);                           // daddiu sp, sp, 128
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-envmap-only-proc", execute, 256);
}

} // namespace generic_envmap_only_proc

namespace generic_no_light {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
  void* generic_no_light_proc; // generic-no-light-proc
  void* generic_prepare_dma_single; // generic-prepare-dma-single
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  [[maybe_unused]] bool bc = false;
  u32 call_addr = 0;
  u32 qwc = 0;
  u32 madr = 0;
  u32 sadr = 0;
  c->daddiu(sp, sp, -16);                           // daddiu sp, sp, -16
  c->sd(ra, 12176, at);                             // sd ra, 12176(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->sw(a1, 48, at);                                // sw a1, 48(at)
  c->sw(a0, 44, at);                                // sw a0, 44(at)
  c->sh(r0, 56, at);                                // sh r0, 56(at)
  c->load_symbol2(t9, cache.generic_prepare_dma_single);// lw t9, generic-prepare-dma-single(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->load_symbol2(t9, cache.generic_no_light_proc); // lw t9, generic-no-light-proc(s7)
  call_addr = c->gprs[t9].du32[0];                  // function call:
  c->sll(v0, ra, 0);                                // sll v0, ra, 0
  c->jalr(call_addr);                               // jalr ra, t9
  c->lw(v1, 24, at);                                // lw v1, 24(at)
  c->lw(a0, 40, at);                                // lw a0, 40(at)
  c->mov64(a3, v1);                                 // or a3, v1, r0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(a2, 4096);                                 // lui a2, 4096
  c->lwu(a1, 60, at);                               // lwu a1, 60(at)
  c->ori(a2, a2, 53248);                            // ori a2, a2, 53248
  // c->lw(t1, 0, a2);                              // lw t1, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, at, 92);                            // daddiu t0, at, 92
  c->andi(a3, a3, 16383);                           // andi a3, a3, 16383
  c->andi(t1, t1, 256);                             // andi t1, t1, 256
  // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(t1) == 0;                          // beq t1, r0, L43
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_4;}                           // branch non-likely

  c->mov64(t1, a2);                                 // or t1, a2, r0
  // nop                                            // sll r0, r0, 0
  
// block_2:
//   c->lw(t2, 0, t0);                                 // lw t2, 0(t0)
//   // nop                                            // sll r0, r0, 0
//   c->lw(t3, 0, t1);                                 // lw t3, 0(t1)
//   // nop                                            // sll r0, r0, 0
//   c->andi(t3, t3, 256);                             // andi t3, t3, 256
//   c->daddiu(t2, t2, 1);                             // daddiu t2, t2, 1
//   bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L42
//   c->sw(t2, 0, t0);                                 // sw t2, 0(t0)
//   if (bc) {goto block_2;}                           // branch non-likely
//
//   c->gprs[t0].du64[0] = 0;                          // or t0, r0, r0
  
// block_4:
  c->dsll(t0, a0, 4);                               // dsll t0, a0, 4
  // c->sw(a3, 128, a2);                            // sw a3, 128(a2)
  sadr = c->sgpr64(a3);
  // nop                                            // sll r0, r0, 0
  // c->sw(a1, 16, a2);                             // sw a1, 16(a2)
  madr = c->sgpr64(a1);
  c->addiu(a3, r0, 256);                            // addiu a3, r0, 256
  // c->sw(a0, 32, a2);                             // sw a0, 32(a2)
  qwc = c->sgpr64(a0);
  c->daddu(a0, a1, t0);                             // daddu a0, a1, t0
  // c->sw(a3, 0, a2);                              // sw a3, 0(a2)
  spad_from_dma_no_sadr_off(cache.fake_scratchpad_data, madr, sadr, qwc);
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 60, at);                                // sw a0, 60(at)
  c->gprs[a0].du64[0] = 0;                          // or a0, r0, r0
  c->xori(v1, v1, 4608);                            // xori v1, v1, 4608
  c->sw(v1, 24, at);                                // sw v1, 24(at)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12176, at);                             // ld ra, 12176(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 16);                            // daddiu sp, sp, 16
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  cache.generic_no_light_proc = intern_from_c(-1, 0, "generic-no-light-proc").c();
  cache.generic_prepare_dma_single = intern_from_c(-1, 0, "generic-prepare-dma-single").c();
  gLinkedFunctionTable.reg("generic-no-light", execute, 256);
}

} // namespace generic_no_light

namespace generic_no_light_dproc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -128);                          // daddiu sp, sp, -128
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s0, 12448, at);                             // sq s0, 12448(at)
  c->sq(s1, 12464, at);                             // sq s1, 12464(at)
  c->sq(s2, 12480, at);                             // sq s2, 12480(at)
  c->sq(s3, 12496, at);                             // sq s3, 12496(at)
  c->sq(s4, 12512, at);                             // sq s4, 12512(at)
  c->sq(s5, 12528, at);                             // sq s5, 12528(at)
  c->sq(gp, 12544, at);                             // sq gp, 12544(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->lw(a1, 44, at);                                // lw a1, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 36, at);                                // lw a0, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 0, a1);                                 // lw v1, 0(a1)
  // nop                                            // sll r0, r0, 0
  c->lw(a2, 4, a1);                                 // lw a2, 4(a1)
  c->daddiu(a0, a0, 3);                             // daddiu a0, a0, 3
  // nop                                            // sll r0, r0, 0
  c->dsra(a0, a0, 2);                               // dsra a0, a0, 2
  // nop                                            // sll r0, r0, 0
  c->dsll(a0, a0, 3);                               // dsll a0, a0, 3
  c->addiu(a3, r0, 255);                            // addiu a3, r0, 255
  c->lui(a1, -2);                                   // lui a1, -2
  c->addiu(t1, r0, 256);                            // addiu t1, r0, 256
  c->ori(a1, a1, 65534);                            // ori a1, a1, 65534
  c->daddu(a0, v1, a0);                             // daddu a0, v1, a0
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->lw(t2, 4, at);                                 // lw t2, 4(at)
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->lw(t3, 8, at);                                 // lw t3, 8(at)
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t5, 12, at);                                // lw t5, 12(at)
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t4, 16, at);                                // lw t4, 16(at)
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->lw(t6, 20, at);                                // lw t6, 20(at)
  c->pcpyld(a3, a3, a3);                            // pcpyld a3, a3, a3
  c->lq(t0, 12160, at);                             // lq t0, 12160(at)
  c->pcpyh(t1, t1);                                 // pcpyh t1, t1
  c->ld(t7, 0, v1);                                 // ld t7, 0(v1)
  c->pcpyld(t1, t1, t1);                            // pcpyld t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(t8, r0, t7);                            // pextlh t8, r0, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t7, t8, a3);                              // pand t7, t8, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t7, t7, 5);                              // psllw t7, t7, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t2, t2, -48);                           // daddiu t2, t2, -48
  c->daddiu(t3, t3, -16);                           // daddiu t3, t3, -16
  c->daddiu(t4, t4, -16);                           // daddiu t4, t4, -16
  c->daddiu(t5, t5, -16);                           // daddiu t5, t5, -16
  //beq r0, r0, L49                                 // beq r0, r0, L49
  c->daddiu(t6, t6, -16);                           // daddiu t6, t6, -16
  goto block_3;                                     // branch always

  // nop                                            // sll r0, r0, 0
  
block_2:
  c->pextlh(t8, r0, gp);                            // pextlh t8, r0, gp
  c->sq(t7, 0, t2);                                 // sq t7, 0(t2)
  c->pand(t7, t8, a3);                              // pand t7, t8, a3
  c->sq(t9, 16, t2);                                // sq t9, 16(t2)
  c->psllw(t7, t7, 5);                              // psllw t7, t7, 5
  c->sq(ra, 32, t2);                                // sq ra, 32(t2)
  
block_3:
  c->paddw(s3, t7, a2);                             // paddw s3, t7, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s2, s3, 0);                             // dsrl32 s2, s3, 0
  c->daddiu(t2, t2, 48);                            // daddiu t2, t2, 48
  c->pcpyud(s5, s3, r0);                            // pcpyud s5, s3, r0
  c->lq(t7, 0, s3);                                 // lq t7, 0(s3)
  c->dsrl32(s4, s5, 0);                             // dsrl32 s4, s5, 0
  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->pand(t8, t8, t1);                              // pand t8, t8, t1
  c->lq(t9, 0, s2);                                 // lq t9, 0(s2)
  c->psraw(gp, t8, 8);                              // psraw gp, t8, 8
  c->lq(t8, 0, s5);                                 // lq t8, 0(s5)
  c->pextuw(s1, t9, t7);                            // pextuw s1, t9, t7
  c->lq(ra, 0, s4);                                 // lq ra, 0(s4)
  c->daddiu(t5, t5, 16);                            // daddiu t5, t5, 16
  c->daddiu(v1, v1, 8);                             // daddiu v1, v1, 8
  c->daddiu(t4, t4, 16);                            // daddiu t4, t4, 16
  c->daddiu(t6, t6, 16);                            // daddiu t6, t6, 16
  c->pextuw(s0, ra, t8);                            // pextuw s0, ra, t8
  c->lq(s3, 16, s3);                                // lq s3, 16(s3)
  c->pcpyud(s1, s1, s0);                            // pcpyud s1, s1, s0
  c->lq(s2, 16, s2);                                // lq s2, 16(s2)
  c->paddh(s0, s1, t0);                             // paddh s0, s1, t0
  c->lq(s1, 16, s5);                                // lq s1, 16(s5)
  c->pand(s5, s0, a1);                              // pand s5, s0, a1
  c->lq(s0, 16, s4);                                // lq s0, 16(s4)
  c->pextlw(s4, s2, s3);                            // pextlw s4, s2, s3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(s3, s2, s3);                            // pextuw s3, s2, s3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(s2, s0, s1);                            // pextlw s2, s0, s1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(s0, s0, s1);                            // pextuw s0, s0, s1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(s1, s2, s4);                            // pcpyld s1, s2, s4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyud(s4, s4, s2);                            // pcpyud s4, s4, s2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyud(s3, s3, s0);                            // pcpyud s3, s3, s0
  c->sq(s4, 0, t4);                                 // sq s4, 0(t4)
  c->pand(s4, s1, a1);                              // pand s4, s1, a1
  c->sq(s3, 0, t3);                                 // sq s3, 0(t3)
  c->por(s4, s4, gp);                               // por s4, s4, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->por(gp, s5, gp);                               // por gp, s5, gp
  c->sq(s4, 0, t6);                                 // sq s4, 0(t6)
  c->prot3w(ra, ra);                                // prot3w ra, ra
  c->sq(gp, 0, t5);                                 // sq gp, 0(t5)
  c->prot3w(t9, t9);                                // prot3w t9, t9
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(s5, t9, t7);                            // pextuw s5, t9, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t9, t8, t9);                            // pcpyld t9, t8, t9
  c->ld(gp, 0, v1);                                 // ld gp, 0(v1)
  c->pcpyld(t7, s5, t7);                            // pcpyld t7, s5, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t8, ra, t8);                            // pextuw t8, ra, t8
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  bc = c->sgpr64(v1) != c->sgpr64(a0);              // bne v1, a0, L48
  c->pcpyld(ra, ra, t8);                            // pcpyld ra, ra, t8
  if (bc) {goto block_2;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->sq(t7, 0, t2);                                 // sq t7, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(t9, 16, t2);                                // sq t9, 16(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(ra, 32, t2);                                // sq ra, 32(t2)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12544, at);                             // lq gp, 12544(at)
  c->lq(s5, 12528, at);                             // lq s5, 12528(at)
  c->lq(s4, 12512, at);                             // lq s4, 12512(at)
  c->lq(s3, 12496, at);                             // lq s3, 12496(at)
  c->lq(s2, 12480, at);                             // lq s2, 12480(at)
  c->lq(s1, 12464, at);                             // lq s1, 12464(at)
  c->lq(s0, 12448, at);                             // lq s0, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 128);                           // daddiu sp, sp, 128
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-no-light-dproc", execute, 256);
}

} // namespace generic_no_light_dproc

namespace generic_no_light_dproc_only {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -32);                           // daddiu sp, sp, -32
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(gp, 12448, at);                             // sq gp, 12448(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lw(a0, 44, at);                                // lw a0, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a1, 36, at);                                // lw a1, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 0, a0);                                 // lw v1, 0(a0)
  c->daddiu(a1, a1, 3);                             // daddiu a1, a1, 3
  c->lw(a2, 4, a0);                                 // lw a2, 4(a0)
  // nop                                            // sll r0, r0, 0
  c->dsra(a0, a1, 2);                               // dsra a0, a1, 2
  c->dsll(a0, a0, 3);                               // dsll a0, a0, 3
  c->addiu(a3, r0, 255);                            // addiu a3, r0, 255
  c->lui(a1, -2);                                   // lui a1, -2
  c->addiu(t1, r0, 256);                            // addiu t1, r0, 256
  c->ori(a1, a1, 65534);                            // ori a1, a1, 65534
  c->daddu(a0, v1, a0);                             // daddu a0, v1, a0
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t2, 16, at);                                // lw t2, 16(at)
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->lw(t3, 20, at);                                // lw t3, 20(at)
  c->pcpyld(a3, a3, a3);                            // pcpyld a3, a3, a3
  c->lq(t0, 12160, at);                             // lq t0, 12160(at)
  c->pcpyh(t1, t1);                                 // pcpyh t1, t1
  c->ld(t4, 0, v1);                                 // ld t4, 0(v1)
  c->pcpyld(t1, t1, t1);                            // pcpyld t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(t5, r0, t4);                            // pextlh t5, r0, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t4, t5, a3);                              // pand t4, t5, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t4, t4, 5);                              // psllw t4, t4, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->daddiu(t2, t2, -16);                           // daddiu t2, t2, -16
  //beq r0, r0, L52                                 // beq r0, r0, L52
  c->daddiu(t3, t3, -16);                           // daddiu t3, t3, -16
  goto block_2;                                     // branch always

  
block_1:
  c->pextlh(t5, r0, t5);                            // pextlh t5, r0, t5
  c->sd(t7, 0, t2);                                 // sd t7, 0(t2)
  c->pand(t6, t5, a3);                              // pand t6, t5, a3
  c->sd(t4, 8, t2);                                 // sd t4, 8(t2)
  c->psllw(t4, t6, 5);                              // psllw t4, t6, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  
block_2:
  c->paddw(t8, t4, a2);                             // paddw t8, t4, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t7, t8, 0);                             // dsrl32 t7, t8, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyud(t6, t8, r0);                            // pcpyud t6, t8, r0
  c->lwu(t9, 16, t8);                               // lwu t9, 16(t8)
  c->dsrl32(t4, t6, 0);                             // dsrl32 t4, t6, 0
  c->daddiu(t2, t2, 16);                            // daddiu t2, t2, 16
  c->pand(t5, t5, t1);                              // pand t5, t5, t1
  c->lwu(gp, 16, t7);                               // lwu gp, 16(t7)
  c->psraw(t5, t5, 8);                              // psraw t5, t5, 8
  c->lwu(ra, 16, t6);                               // lwu ra, 16(t6)
  c->pextlw(t9, gp, t9);                            // pextlw t9, gp, t9
  c->lwu(gp, 16, t4);                               // lwu gp, 16(t4)
  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->daddiu(v1, v1, 8);                             // daddiu v1, v1, 8
  c->pextlw(ra, gp, ra);                            // pextlw ra, gp, ra
  c->lwu(t8, 20, t8);                               // lwu t8, 20(t8)
  c->pcpyld(t9, ra, t9);                            // pcpyld t9, ra, t9
  c->lwu(t7, 20, t7);                               // lwu t7, 20(t7)
  c->paddh(t9, t9, t0);                             // paddh t9, t9, t0
  c->lwu(t6, 20, t6);                               // lwu t6, 20(t6)
  c->pand(t9, t9, a1);                              // pand t9, t9, a1
  c->lwu(t4, 20, t4);                               // lwu t4, 20(t4)
  c->por(t9, t9, t5);                               // por t9, t9, t5
  c->ld(t5, 0, v1);                                 // ld t5, 0(v1)
  c->pextlw(t7, t7, t8);                            // pextlw t7, t7, t8
  c->sq(t9, 0, t3);                                 // sq t9, 0(t3)
  bc = c->sgpr64(v1) != c->sgpr64(a0);              // bne v1, a0, L51
  c->pextlw(t4, t4, t6);                            // pextlw t4, t4, t6
  if (bc) {goto block_1;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->sd(t7, 0, t2);                                 // sd t7, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->sd(t4, 8, t2);                                 // sd t4, 8(t2)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12448, at);                             // lq gp, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 32);                            // daddiu sp, sp, 32
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-no-light-dproc-only", execute, 256);
}

} // namespace generic_no_light_dproc_only

namespace generic_no_light_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -96);                           // daddiu sp, sp, -96
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s2, 12448, at);                             // sq s2, 12448(at)
  c->sq(s3, 12464, at);                             // sq s3, 12464(at)
  c->sq(s4, 12480, at);                             // sq s4, 12480(at)
  c->sq(s5, 12496, at);                             // sq s5, 12496(at)
  c->sq(gp, 12512, at);                             // sq gp, 12512(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->addiu(t1, r0, 256);                            // addiu t1, r0, 256
  c->lw(v1, 36, at);                                // lw v1, 36(at)
  c->lui(a0, -2);                                   // lui a0, -2
  c->lw(a1, 44, at);                                // lw a1, 44(at)
  c->daddiu(v1, v1, 3);                             // daddiu v1, v1, 3
  c->addiu(a3, r0, 255);                            // addiu a3, r0, 255
  c->dsra(a2, v1, 2);                               // dsra a2, v1, 2
  c->lw(v1, 0, a1);                                 // lw v1, 0(a1)
  c->dsll(t0, a2, 3);                               // dsll t0, a2, 3
  c->lw(a2, 4, a1);                                 // lw a2, 4(a1)
  c->ori(a1, a0, 65534);                            // ori a1, a0, 65534
  c->daddu(a0, v1, t0);                             // daddu a0, v1, t0
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t2, 4, at);                                 // lw t2, 4(at)
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->lw(t3, 8, at);                                 // lw t3, 8(at)
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->lw(t4, 12, at);                                // lw t4, 12(at)
  c->pcpyld(a3, a3, a3);                            // pcpyld a3, a3, a3
  c->lq(t0, 12160, at);                             // lq t0, 12160(at)
  c->pcpyh(t1, t1);                                 // pcpyh t1, t1
  c->ld(t5, 0, v1);                                 // ld t5, 0(v1)
  c->pcpyld(t1, t1, t1);                            // pcpyld t1, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(t6, r0, t5);                            // pextlh t6, r0, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t5, t6, a3);                              // pand t5, t6, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t2, t2, -48);                           // daddiu t2, t2, -48
  c->daddiu(t3, t3, -16);                           // daddiu t3, t3, -16
  //beq r0, r0, L55                                 // beq r0, r0, L55
  c->daddiu(t4, t4, -16);                           // daddiu t4, t4, -16
  goto block_2;                                     // branch always

  
block_1:
  c->pextlh(t6, r0, ra);                            // pextlh t6, r0, ra
  c->sq(t5, 0, t2);                                 // sq t5, 0(t2)
  c->pand(t5, t6, a3);                              // pand t5, t6, a3
  c->sq(t7, 16, t2);                                // sq t7, 16(t2)
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->sq(t8, 32, t2);                                // sq t8, 32(t2)
  
block_2:
  c->paddw(gp, t5, a2);                             // paddw gp, t5, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(s4, gp, 0);                             // dsrl32 s4, gp, 0
  c->daddiu(t2, t2, 48);                            // daddiu t2, t2, 48
  c->pcpyud(s5, gp, r0);                            // pcpyud s5, gp, r0
  c->lq(t5, 0, gp);                                 // lq t5, 0(gp)
  c->dsrl32(t8, s5, 0);                             // dsrl32 t8, s5, 0
  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->pand(t6, t6, t1);                              // pand t6, t6, t1
  c->lq(t7, 0, s4);                                 // lq t7, 0(s4)
  c->psraw(ra, t6, 8);                              // psraw ra, t6, 8
  c->lq(t6, 0, s5);                                 // lq t6, 0(s5)
  c->pextuw(s3, t7, t5);                            // pextuw s3, t7, t5
  c->lq(t9, 0, t8);                                 // lq t9, 0(t8)
  c->daddiu(t4, t4, 16);                            // daddiu t4, t4, 16
  c->daddiu(v1, v1, 8);                             // daddiu v1, v1, 8
  c->pextuw(s2, t9, t6);                            // pextuw s2, t9, t6
  c->lw(gp, 28, gp);                                // lw gp, 28(gp)
  c->pcpyud(s3, s3, s2);                            // pcpyud s3, s3, s2
  c->lw(s4, 28, s4);                                // lw s4, 28(s4)
  c->paddh(s3, s3, t0);                             // paddh s3, s3, t0
  c->lw(s5, 28, s5);                                // lw s5, 28(s5)
  c->pand(s3, s3, a1);                              // pand s3, s3, a1
  c->lw(t8, 28, t8);                                // lw t8, 28(t8)
  c->por(ra, s3, ra);                               // por ra, s3, ra
  c->sw(gp, 0, t3);                                 // sw gp, 0(t3)
  c->prot3w(t9, t9);                                // prot3w t9, t9
  c->sq(ra, 0, t4);                                 // sq ra, 0(t4)
  c->prot3w(t7, t7);                                // prot3w t7, t7
  c->sw(s4, 4, t3);                                 // sw s4, 4(t3)
  c->pextuw(gp, t7, t5);                            // pextuw gp, t7, t5
  c->sw(s5, 8, t3);                                 // sw s5, 8(t3)
  c->pcpyld(t7, t6, t7);                            // pcpyld t7, t6, t7
  c->ld(ra, 0, v1);                                 // ld ra, 0(v1)
  c->pcpyld(t5, gp, t5);                            // pcpyld t5, gp, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t6, t9, t6);                            // pextuw t6, t9, t6
  c->sw(t8, 12, t3);                                // sw t8, 12(t3)
  bc = c->sgpr64(v1) != c->sgpr64(a0);              // bne v1, a0, L54
  c->pcpyld(t8, t9, t6);                            // pcpyld t8, t9, t6
  if (bc) {goto block_1;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->sq(t5, 0, t2);                                 // sq t5, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(t7, 16, t2);                                // sq t7, 16(t2)
  // nop                                            // sll r0, r0, 0
  c->sq(t8, 32, t2);                                // sq t8, 32(t2)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12512, at);                             // lq gp, 12512(at)
  c->lq(s5, 12496, at);                             // lq s5, 12496(at)
  c->lq(s4, 12480, at);                             // lq s4, 12480(at)
  c->lq(s3, 12464, at);                             // lq s3, 12464(at)
  c->lq(s2, 12448, at);                             // lq s2, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 96);                            // daddiu sp, sp, 96
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-no-light-proc", execute, 128);
}

} // namespace generic_no_light_proc

namespace generic_interp_dproc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 64, at);                                // lw v1, 64(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 44, at);                                // lw a0, 44(at)
  bc = c->sgpr64(v1) == 0;                          // beq v1, r0, L59
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_7;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->lw(t0, 8, v1);                                 // lw t0, 8(v1)
  // nop                                            // sll r0, r0, 0
  c->lw(a2, 4, a0);                                 // lw a2, 4(a0)
  // nop                                            // sll r0, r0, 0
  c->lh(a0, 0, v1);                                 // lh a0, 0(v1)
  // nop                                            // sll r0, r0, 0
  c->lh(a1, 2, v1);                                 // lh a1, 2(v1)
  bc = c->sgpr64(a0) != 0;                          // bne a0, r0, L59
  c->lh(a0, 4, v1);                                 // lh a0, 4(v1)
  if (bc) {goto block_7;}                           // branch non-likely

  c->dsll(t1, a0, 5);                               // dsll t1, a0, 5
  c->lh(a0, 12, v1);                                // lh a0, 12(v1)
  c->daddiu(a3, a1, 7);                             // daddiu a3, a1, 7
  c->lh(a1, 14, v1);                                // lh a1, 14(v1)
  bc = c->sgpr64(a1) == 0;                          // beq a1, r0, L59
  c->daddu(v1, t1, a2);                             // daddu v1, t1, a2
  if (bc) {goto block_7;}                           // branch non-likely

  c->pextlh(a0, a0, a0);                            // pextlh a0, a0, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a0, a0, a0);                            // pextlw a0, a0, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a0, a0, a0);                            // pcpyld a0, a0, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(a1, a1, a1);                            // pextlh a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a1, a1, a1);                            // pcpyld a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a2, a2, a2);                            // pcpyld a2, a2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsra(a3, a3, 3);                               // dsra a3, a3, 3
  // nop                                            // sll r0, r0, 0
  c->dsll(a3, a3, 4);                               // dsll a3, a3, 4
  c->ld(t1, 0, t0);                                 // ld t1, 0(t0)
  c->daddu(a3, t0, a3);                             // daddu a3, t0, a3
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlb(t1, r0, t1);                            // pextlb t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllh(t2, t1, 5);                              // psllh t2, t1, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuh(t1, r0, t2);                            // pextuh t1, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t2, t2, a2);                             // paddw t2, t2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  //beq r0, r0, L58                                 // beq r0, r0, L58
  c->pcpyud(t5, t2, r0);                            // pcpyud t5, t2, r0
  goto block_5;                                     // branch always

  
block_4:
  c->dsrl32(t5, t6, 0);                             // dsrl32 t5, t6, 0
  c->dsrl32(t4, t3, 0);                             // dsrl32 t4, t3, 0
  c->pextuh(t1, r0, t2);                            // pextuh t1, r0, t2
  c->sw(t6, 16, v1);                                // sw t6, 16(v1)
  c->pextlh(t2, r0, t2);                            // pextlh t2, r0, t2
  c->sw(t5, 48, v1);                                // sw t5, 48(v1)
  c->paddw(t2, t2, a2);                             // paddw t2, t2, a2
  c->sw(t3, 80, v1);                                // sw t3, 80(v1)
  c->pcpyud(t5, t2, r0);                            // pcpyud t5, t2, r0
  c->sw(t4, 112, v1);                               // sw t4, 112(v1)
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->daddiu(v1, v1, 128);                           // daddiu v1, v1, 128
  
block_5:
  c->paddw(t1, t1, a2);                             // paddw t1, t1, a2
  c->lwu(t3, 16, t2);                               // lwu t3, 16(t2)
  c->pcpyud(t6, t1, r0);                            // pcpyud t6, t1, r0
  c->lwu(t4, 16, t5);                               // lwu t4, 16(t5)
  c->dsrl32(t8, t2, 0);                             // dsrl32 t8, t2, 0
  c->lwu(t2, 16, t1);                               // lwu t2, 16(t1)
  c->dsrl32(t9, t5, 0);                             // dsrl32 t9, t5, 0
  c->lwu(t5, 16, t6);                               // lwu t5, 16(t6)
  c->dsrl32(t7, t1, 0);                             // dsrl32 t7, t1, 0
  c->lwu(t1, 16, t8);                               // lwu t1, 16(t8)
  c->dsrl32(t8, t6, 0);                             // dsrl32 t8, t6, 0
  c->lwu(t6, 16, t9);                               // lwu t6, 16(t9)
  c->pextlw(t4, t4, t3);                            // pextlw t4, t4, t3
  c->lwu(t3, 16, t7);                               // lwu t3, 16(t7)
  c->pextlw(t2, t5, t2);                            // pextlw t2, t5, t2
  c->lwu(t5, 16, t8);                               // lwu t5, 16(t8)
  c->pcpyld(t2, t2, t4);                            // pcpyld t2, t2, t4
  c->lwu(t4, 16, v1);                               // lwu t4, 16(v1)
  c->pextlw(t6, t6, t1);                            // pextlw t6, t6, t1
  c->lwu(t1, 48, v1);                               // lwu t1, 48(v1)
  c->pextlw(t3, t5, t3);                            // pextlw t3, t5, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t3, t3, t6);                            // pcpyld t3, t3, t6
  c->lwu(t5, 80, v1);                               // lwu t5, 80(v1)
  c->pextlw(t1, t1, t4);                            // pextlw t1, t1, t4
  c->lwu(t4, 112, v1);                              // lwu t4, 112(v1)
  c->pmulth(r0, t2, a1);                            // pmulth r0, t2, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t2, t4, t5);                            // pextlw t2, t4, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pmaddh(r0, t3, a1);                            // pmaddh r0, t3, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t1, t2, t1);                            // pcpyld t1, t2, t1
  c->ld(t2, 0, t0);                                 // ld t2, 0(t0)
  c->pmaddh(r0, t1, a0);                            // pmaddh r0, t1, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlb(t1, r0, t2);                            // pextlb t1, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllh(t2, t1, 5);                              // psllh t2, t1, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // Unknown instr: pmfhl.lw t3
  c->pmfhl_lw(t3);
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // Unknown instr: pmfhl.uw t1
  c->pmfhl_uw(t1);
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psraw(t3, t3, 8);                              // psraw t3, t3, 8
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psraw(t1, t1, 8);                              // psraw t1, t1, 8
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pinteh(t6, t1, t3);                            // pinteh t6, t1, t3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  bc = c->sgpr64(t0) != c->sgpr64(a3);              // bne t0, a3, L57
  c->pcpyud(t3, t6, r0);                            // pcpyud t3, t6, r0
  if (bc) {goto block_4;}                           // branch non-likely

  c->dsrl32(a0, t6, 0);                             // dsrl32 a0, t6, 0
  c->sw(t6, 16, v1);                                // sw t6, 16(v1)
  c->dsrl32(a1, t3, 0);                             // dsrl32 a1, t3, 0
  c->sw(a0, 48, v1);                                // sw a0, 48(v1)
  // nop                                            // sll r0, r0, 0
  c->sw(t3, 80, v1);                                // sw t3, 80(v1)
  // nop                                            // sll r0, r0, 0
  c->sw(a1, 112, v1);                               // sw a1, 112(v1)
  
block_7:
  //jr ra                                           // jr ra
  c->daddu(sp, sp, r0);                             // daddu sp, sp, r0
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-interp-dproc", execute, 128);
}

} // namespace generic_interp_dproc

namespace generic_envmap_dproc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  // nop                                            // sll r0, r0, 0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(v1, 16256);                                // lui v1, 16256
  c->mtc1(f0, v1);                                  // mtc1 f0, v1
  c->lwu(a1, 44, at);                               // lwu a1, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 12048, at);                             // lw v1, 12048(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a2, 4, a1);                                 // lw a2, 4(a1)
  c->mov64(a0, a2);                                 // or a0, a2, r0
  c->lhu(a1, 20, a1);                               // lhu a1, 20(a1)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f4, 24, a2);                              // lwc1 f4, 24(a2)
  c->daddiu(a1, a1, -4);                            // daddiu a1, a1, -4
  c->lwc1(f3, 56, a2);                              // lwc1 f3, 56(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f2, 88, a2);                              // lwc1 f2, 88(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f1, 120, a2);                             // lwc1 f1, 120(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 16, a2);                                // lq t2, 16(a2)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  c->lq(t3, 48, a2);                                // lq t3, 48(a2)
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t4, 80, a2);                                // lq t4, 80(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 112, a2);                               // lq t5, 112(a2)
  // nop                                            // sll r0, r0, 0
  c->lqc2(vf31, 12016, at);                         // lqc2 vf31, 12016(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, a2);                                 // lq t6, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 32, a2);                                // lq a3, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t0, 64, a2);                                // lq t0, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 96, a2);                                // lq t1, 96(a2)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  c->mov128_vf_gpr(vf21, t2);                       // qmtc2.i vf21, t2
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  c->mov128_vf_gpr(vf22, t3);                       // qmtc2.ni vf22, t3
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->mov128_vf_gpr(vf23, t4);                       // qmtc2.ni vf23, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t5);                       // qmtc2.ni vf24, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t6);                        // qmtc2.ni vf9, t6
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  c->mfc1(t2, f4);                                  // mfc1 t2, f4
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  c->mov128_vf_gpr(vf10, a3);                       // qmtc2.ni vf10, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t0);                       // qmtc2.ni vf11, t0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t1);                       // qmtc2.ni vf12, t1
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(a3, f3);                                  // mfc1 a3, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(a3, a3, t2);                            // pextlw a3, a3, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t0, f2);                                  // mfc1 t0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->daddiu(a2, a2, 128);                           // daddiu a2, a2, 128
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t1, f1);                                  // mfc1 t1, f1
  c->pextlw(t0, t1, t0);                            // pextlw t0, t1, t0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a3, t0, a3);                            // pcpyld a3, t0, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, a3);                       // qmtc2.ni vf27, a3
  // nop                                            // sll r0, r0, 0
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->lwc1(f4, 24, a2);                              // lwc1 f4, 24(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f3, 56, a2);                              // lwc1 f3, 56(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f2, 88, a2);                              // lwc1 f2, 88(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f1, 120, a2);                             // lwc1 f1, 120(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 16, a2);                                // lq a3, 16(a2)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t0, 48, a2);                                // lq t0, 48(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 80, a2);                                // lq t1, 80(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 112, a2);                               // lq t2, 112(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 0, a2);                                 // lq t3, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 32, a2);                                // lq t4, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 64, a2);                                // lq t5, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 96, a2);                                // lq t6, 96(a2)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t7, f4);                                  // mfc1 t7, f4
  // nop                                            // sll r0, r0, 0
  c->daddiu(a2, a2, 128);                           // daddiu a2, a2, 128
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f3);                                  // mfc1 t8, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t7, t8, t7);                            // pextlw t7, t8, t7
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f2);                                  // mfc1 t8, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f1);                                  // mfc1 t9, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, a3);                       // qmtc2.ni vf21, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t3);                        // qmtc2.ni vf9, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t4);                       // qmtc2.ni vf10, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t5);                       // qmtc2.ni vf11, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t6);                       // qmtc2.ni vf12, t6
  c->pextlw(a3, t9, t8);                            // pextlw a3, t9, t8
  // nop                                            // sll r0, r0, 0
  c->pcpyld(a3, a3, t7);                            // pcpyld a3, a3, t7
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t0);                       // qmtc2.ni vf22, t0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t1);                       // qmtc2.ni vf23, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t2);                       // qmtc2.ni vf24, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, a3);                       // qmtc2.ni vf27, a3
  // nop                                            // sll r0, r0, 0
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->lwc1(f1, 24, a2);                              // lwc1 f1, 24(a2)
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  c->lwc1(f2, 56, a2);                              // lwc1 f2, 56(a2)
  c->divs(f3, f0, f1);                              // div.s f3, f0, f1
  c->lwc1(f5, 88, a2);                              // lwc1 f5, 88(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f1, 120, a2);                             // lwc1 f1, 120(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 16, a2);                                // lq a3, 16(a2)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->subs(f4, f2, f0);                              // sub.s f4, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f5, f0);                              // sub.s f2, f5, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  c->lq(t0, 48, a2);                                // lq t0, 48(a2)
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t1, 80, a2);                                // lq t1, 80(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 112, a2);                               // lq t2, 112(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 0, a2);                                 // lq t3, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 32, a2);                                // lq t4, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 64, a2);                                // lq t5, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 96, a2);                                // lq t6, 96(a2)
  // nop                                            // sll r0, r0, 0
  c->mfc1(t7, f3);                                  // mfc1 t7, f3
  c->muls(f3, f4, f0);                              // mul.s f3, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->daddiu(a2, a2, 128);                           // daddiu a2, a2, 128
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f3);                                  // mfc1 t8, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f2);                                  // mfc1 t9, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->pextlw(t7, t8, t7);                            // pextlw t7, t8, t7
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f1);                                  // mfc1 t8, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->pextlw(t8, t8, t9);                            // pextlw t8, t8, t9
  // nop                                            // sll r0, r0, 0
  c->pcpyld(t7, t8, t7);                            // pcpyld t7, t8, t7
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, a3);                       // qmtc2.ni vf21, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t0);                       // qmtc2.ni vf22, t0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t1);                       // qmtc2.ni vf23, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t2);                       // qmtc2.ni vf24, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t3);                        // qmtc2.ni vf9, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t4);                       // qmtc2.ni vf10, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t5);                       // qmtc2.ni vf11, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t6);                       // qmtc2.ni vf12, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t7);                       // qmtc2.ni vf27, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t1, vf17);                       // qmfc2.ni t1, vf17
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t2, vf18);                       // qmfc2.ni t2, vf18
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t0, vf19);                       // qmfc2.ni t0, vf19
  bc = ((s64)c->sgpr64(a1)) <= 0;                   // blez a1, L62
  c->mov128_gpr_vf(a3, vf20);                       // qmfc2.ni a3, vf20
  if (bc) {goto block_2;}                           // branch non-likely

  
block_1:
  c->ppach(t1, r0, t1);                             // ppach t1, r0, t1
  // Unknown instr: vcallms 48
  vcallms48(c);
  c->ppach(t2, r0, t2);                             // ppach t2, r0, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t0, r0, t0);                             // ppach t0, r0, t0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(a3, r0, a3);                             // ppach a3, r0, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->sw(t1, 16, a0);                                // sw t1, 16(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(t2, 48, a0);                                // sw t2, 48(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(t0, 80, a0);                                // sw t0, 80(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(a3, 112, a0);                               // sw a3, 112(a0)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f4, 24, a2);                              // lwc1 f4, 24(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f3, 56, a2);                              // lwc1 f3, 56(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f2, 88, a2);                              // lwc1 f2, 88(a2)
  // nop                                            // sll r0, r0, 0
  c->lwc1(f1, 120, a2);                             // lwc1 f1, 120(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 16, a2);                                // lq a3, 16(a2)
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  c->sw(v1, 20, a0);                                // sw v1, 20(a0)
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  c->sw(v1, 52, a0);                                // sw v1, 52(a0)
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  c->sw(v1, 84, a0);                                // sw v1, 84(a0)
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  c->sw(v1, 116, a0);                               // sw v1, 116(a0)
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t3, 48, a2);                                // lq t3, 48(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 80, a2);                                // lq t4, 80(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 112, a2);                               // lq t5, 112(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, a2);                                 // lq t6, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 32, a2);                                // lq t2, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t0, 64, a2);                                // lq t0, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 96, a2);                                // lq t1, 96(a2)
  c->daddiu(a1, a1, -4);                            // daddiu a1, a1, -4
  c->daddiu(a0, a0, 128);                           // daddiu a0, a0, 128
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t7, f4);                                  // mfc1 t7, f4
  // nop                                            // sll r0, r0, 0
  c->daddiu(a2, a2, 128);                           // daddiu a2, a2, 128
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f3);                                  // mfc1 t8, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t7, t8, t7);                            // pextlw t7, t8, t7
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t8, f2);                                  // mfc1 t8, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f1);                                  // mfc1 t9, f1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, a3);                       // qmtc2.ni vf21, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t3);                       // qmtc2.ni vf22, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t4);                       // qmtc2.ni vf23, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t5);                       // qmtc2.ni vf24, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t6);                        // qmtc2.ni vf9, t6
  c->pextlw(a3, t9, t8);                            // pextlw a3, t9, t8
  c->mov128_vf_gpr(vf10, t2);                       // qmtc2.ni vf10, t2
  c->pcpyld(a3, a3, t7);                            // pcpyld a3, a3, t7
  c->mov128_vf_gpr(vf11, t0);                       // qmtc2.ni vf11, t0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t1);                       // qmtc2.ni vf12, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, a3);                       // qmtc2.ni vf27, a3
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t1, vf17);                       // qmfc2.ni t1, vf17
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t2, vf18);                       // qmfc2.ni t2, vf18
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t0, vf19);                       // qmfc2.ni t0, vf19
  bc = ((s64)c->sgpr64(a1)) > 0;                    // bgtz a1, L61
  c->mov128_gpr_vf(a3, vf20);                       // qmfc2.ni a3, vf20
  if (bc) {goto block_1;}                           // branch non-likely

  
block_2:
  c->ppach(a1, r0, t1);                             // ppach a1, r0, t1
  c->sw(v1, 20, a0);                                // sw v1, 20(a0)
  c->ppach(a2, r0, t2);                             // ppach a2, r0, t2
  c->sw(v1, 52, a0);                                // sw v1, 52(a0)
  c->ppach(t0, r0, t0);                             // ppach t0, r0, t0
  c->sw(a1, 16, a0);                                // sw a1, 16(a0)
  c->ppach(a1, r0, a3);                             // ppach a1, r0, a3
  c->sw(a2, 48, a0);                                // sw a2, 48(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(t0, 80, a0);                                // sw t0, 80(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(a1, 112, a0);                               // sw a1, 112(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 84, a0);                                // sw v1, 84(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 116, a0);                               // sw v1, 116(a0)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  //jr ra                                           // jr ra
  c->daddu(sp, sp, r0);                             // daddu sp, sp, r0
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-envmap-dproc", execute, 256);
}

} // namespace generic_envmap_dproc

namespace generic_prepare_dma_single {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

// jak3 parity: the body reads the caller's at register (sq gp, 12448(at)) before
// loading it, so it must run on the caller's ExecutionContext. The linked symbol
// gets this assert stub; mips2c callers invoke execute_real directly.
u64 execute(void*) {
  ASSERT(false);
  return 0;
}

u64 execute_real(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -32);                           // daddiu sp, sp, -32
  c->sq(gp, 12448, at);                             // sq gp, 12448(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->lw(t1, 44, at);                                // lw t1, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t8, 48, at);                                // lw t8, 48(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 24, at);                                // lw v1, 24(at)
  // nop                                            // sll r0, r0, 0
  c->lh(t3, 56, at);                                // lh t3, 56(at)
  // nop                                            // sll r0, r0, 0
  c->lh(a1, 18, t1);                                // lh a1, 18(t1)
  c->mov64(a0, v1);                                 // or a0, v1, r0
  c->lbu(a2, 16, t1);                               // lbu a2, 16(t1)
  bc = c->sgpr64(t8) == 0;                          // beq t8, r0, L70
  c->lq(t4, 11744, at);                             // lq t4, 11744(at)
  if (bc) {goto block_10;}                          // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->lq(t9, 11808, at);                             // lq t9, 11808(at)
  // nop                                            // sll r0, r0, 0
  c->lq(gp, 11824, at);                             // lq gp, 11824(at)
  c->mov64(a3, a2);                                 // or a3, a2, r0
  c->lq(t7, 11840, at);                             // lq t7, 11840(at)
  bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L64
  c->lq(t2, 11856, at);                             // lq t2, 11856(at)
  if (bc) {goto block_3;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->lq(t0, 11872, at);                             // lq t0, 11872(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 11888, at);                             // lq t5, 11888(at)
  //beq r0, r0, L65                                 // beq r0, r0, L65
  c->lq(t6, 11936, at);                             // lq t6, 11936(at)
  goto block_4;                                     // branch always

  
block_3:
  // nop                                            // sll r0, r0, 0
  c->lq(t0, 12032, at);                             // lq t0, 12032(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 11936, at);                             // lq t5, 11936(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 11936, at);                             // lq t6, 11936(at)
  
block_4:
  // nop                                            // sll r0, r0, 0
  c->sq(t4, 0, a0);                                 // sq t4, 0(a0)
  // nop                                            // sll r0, r0, 0
  c->sq(t9, 16, a0);                                // sq t9, 16(a0)
  // nop                                            // sll r0, r0, 0
  c->sq(gp, 32, a0);                                // sq gp, 32(a0)
  c->mov64(t4, t8);                                 // or t4, t8, r0
  c->sq(t7, 48, a0);                                // sq t7, 48(a0)
  c->daddiu(t1, t1, 22);                            // daddiu t1, t1, 22
  c->sq(t2, 64, a0);                                // sq t2, 64(a0)
  c->addiu(t2, r0, 0);                              // addiu t2, r0, 0
  c->sq(t0, 80, a0);                                // sq t0, 80(a0)
  c->addiu(t0, r0, 128);                            // addiu t0, r0, 128
  c->sq(t5, 96, a0);                                // sq t5, 96(a0)
  bc = c->sgpr64(t3) != 0;                          // bne t3, r0, L67
  c->sq(t6, 112, a0);                               // sq t6, 112(a0)
  if (bc) {goto block_7;}                           // branch non-likely

  
block_5:
  c->daddu(a0, a0, t0);                             // daddu a0, a0, t0
  c->lq(t0, 0, t4);                                 // lq t0, 0(t4)
  c->daddiu(a3, a3, -1);                            // daddiu a3, a3, -1
  c->lbu(t3, 0, t1);                                // lbu t3, 0(t1)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 16, t4);                                // lq t5, 16(t4)
  c->daddu(t7, t3, t3);                             // daddu t7, t3, t3
  c->lq(t6, 32, t4);                                // lq t6, 32(t4)
  c->daddu(t8, t7, t3);                             // daddu t8, t7, t3
  c->lq(t7, 48, t4);                                // lq t7, 48(t4)
  c->daddiu(t9, t8, 9);                             // daddiu t9, t8, 9
  c->lq(t8, 64, t4);                                // lq t8, 64(t4)
  c->daddiu(t4, t4, 80);                            // daddiu t4, t4, 80
  c->sq(t0, 0, a0);                                 // sq t0, 0(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(t2, 12, a0);                                // sw t2, 12(a0)
  c->daddiu(t1, t1, 1);                             // daddiu t1, t1, 1
  c->sq(t5, 16, a0);                                // sq t5, 16(a0)
  c->daddu(t2, t2, t9);                             // daddu t2, t2, t9
  c->sw(t3, 28, a0);                                // sw t3, 28(a0)
  // nop                                            // sll r0, r0, 0
  c->sq(t6, 32, a0);                                // sq t6, 32(a0)
  c->addiu(t0, r0, 80);                             // addiu t0, r0, 80
  c->sq(t7, 48, a0);                                // sq t7, 48(a0)
  bc = ((s64)c->sgpr64(a3)) > 0;                    // bgtz a3, L66
  c->sq(t8, 64, a0);                                // sq t8, 64(a0)
  if (bc) {goto block_5;}                           // branch non-likely

  //beq r0, r0, L69                                 // beq r0, r0, L69
  // nop                                            // sll r0, r0, 0
  goto block_9;                                     // branch always

  
block_7:
  // nop                                            // sll r0, r0, 0
  c->lw(t3, 52, at);                                // lw t3, 52(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 0, t3);                                 // lq t4, 0(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 16, t3);                                // lq t5, 16(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 32, t3);                                // lq t6, 32(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 48, t3);                                // lq t7, 48(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 64, t3);                                // lq t8, 64(t3)
  
block_8:
  c->daddu(a0, a0, t0);                             // daddu a0, a0, t0
  c->lbu(t3, 0, t1);                                // lbu t3, 0(t1)
  c->daddiu(a3, a3, -1);                            // daddiu a3, a3, -1
  c->sq(t4, 0, a0);                                 // sq t4, 0(a0)
  c->daddu(t0, t3, t3);                             // daddu t0, t3, t3
  c->sw(t2, 12, a0);                                // sw t2, 12(a0)
  c->daddu(t0, t0, t3);                             // daddu t0, t0, t3
  c->sq(t5, 16, a0);                                // sq t5, 16(a0)
  c->daddiu(t0, t0, 9);                             // daddiu t0, t0, 9
  c->sw(t3, 28, a0);                                // sw t3, 28(a0)
  c->daddiu(t1, t1, 1);                             // daddiu t1, t1, 1
  // nop                                            // sll r0, r0, 0
  c->daddu(t2, t2, t0);                             // daddu t2, t2, t0
  c->sq(t6, 32, a0);                                // sq t6, 32(a0)
  c->addiu(t0, r0, 80);                             // addiu t0, r0, 80
  c->sq(t7, 48, a0);                                // sq t7, 48(a0)
  bc = ((s64)c->sgpr64(a3)) > 0;                    // bgtz a3, L68
  c->sq(t8, 64, a0);                                // sq t8, 64(a0)
  if (bc) {goto block_8;}                           // branch non-likely

  
block_9:
  c->ori(a3, t3, 32768);                            // ori a3, t3, 32768
  c->sw(a1, 36, at);                                // sw a1, 36(at)
  // nop                                            // sll r0, r0, 0
  c->sw(a3, 28, a0);                                // sw a3, 28(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(a2, 92, a0);                                // sw a2, 92(a0)
  // nop                                            // sll r0, r0, 0
  c->sw(a1, 108, v1);                               // sw a1, 108(v1)
  //beq r0, r0, L71                                 // beq r0, r0, L71
  c->sw(r0, 124, v1);                               // sw r0, 124(v1)
  goto block_11;                                    // branch always

  
block_10:
  c->dsll(a3, a2, 2);                               // dsll a3, a2, 2
  c->sq(t4, 0, a0);                                 // sq t4, 0(a0)
  c->daddu(a3, a3, a2);                             // daddu a3, a3, a2
  c->sw(a1, 108, v1);                               // sw a1, 108(v1)
  c->dsll(a3, a3, 4);                               // dsll a3, a3, 4
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, a3, 128);                           // daddiu t0, a3, 128
  c->sw(a1, 36, at);                                // sw a1, 36(at)
  
block_11:
  c->dsll(t1, a2, 2);                               // dsll t1, a2, 2
  c->lw(a3, 12, v1);                                // lw a3, 12(v1)
  c->daddu(a2, t1, a2);                             // daddu a2, t1, a2
  c->lw(t1, 68, at);                                // lw t1, 68(at)
  c->daddiu(a2, a2, 7);                             // daddiu a2, a2, 7
  // nop                                            // sll r0, r0, 0
  c->or_(t2, a3, t1);                               // or t2, a3, t1
  // nop                                            // sll r0, r0, 0
  c->sll(t3, a2, 16);                               // sll t3, a2, 16
  c->xori(a3, t1, 38);                              // xori a3, t1, 38
  c->or_(t1, t2, t3);                               // or t1, t2, t3
  c->daddiu(a2, a2, 1);                             // daddiu a2, a2, 1
  // nop                                            // sll r0, r0, 0
  c->sw(t1, 12, v1);                                // sw t1, 12(v1)
  // nop                                            // sll r0, r0, 0
  c->sw(a3, 68, at);                                // sw a3, 68(at)
  c->daddiu(a1, a1, 3);                             // daddiu a1, a1, 3
  c->daddu(a0, a0, t0);                             // daddu a0, a0, t0
  c->dsra(a1, a1, 2);                               // dsra a1, a1, 2
  c->daddiu(a2, a0, 32);                            // daddiu a2, a0, 32
  c->dsll(t0, a1, 2);                               // dsll t0, a1, 2
  // nop                                            // sll r0, r0, 0
  c->daddu(a3, t0, t0);                             // daddu a3, t0, t0
  c->dsll(a1, t0, 2);                               // dsll a1, t0, 2
  c->daddu(a3, a3, t0);                             // daddu a3, a3, t0
  c->daddiu(a1, a1, 15);                            // daddiu a1, a1, 15
  c->dsll(a3, a3, 2);                               // dsll a3, a3, 2
  c->dsra(a1, a1, 4);                               // dsra a1, a1, 4
  c->daddiu(a3, a3, 15);                            // daddiu a3, a3, 15
  c->dsll(t1, a1, 4);                               // dsll t1, a1, 4
  c->dsra(a3, a3, 4);                               // dsra a3, a3, 4
  c->lw(a1, 72, at);                                // lw a1, 72(at)
  c->dsll(a3, a3, 4);                               // dsll a3, a3, 4
  // nop                                            // sll r0, r0, 0
  c->daddu(a3, a2, a3);                             // daddu a3, a2, a3
  c->lw(t2, 11968, at);                             // lw t2, 11968(at)
  c->daddu(a2, a3, t1);                             // daddu a2, a3, t1
  c->sq(r0, 0, a3);                                 // sq r0, 0(a3)
  c->daddiu(a2, a2, 16);                            // daddiu a2, a2, 16
  c->sq(r0, -16, a3);                               // sq r0, -16(a3)
  c->daddu(t1, a2, t1);                             // daddu t1, a2, t1
  c->sq(r0, -32, a3);                               // sq r0, -32(a3)
  c->daddiu(t1, t1, 16);                            // daddiu t1, t1, 16
  c->sq(r0, 0, a2);                                 // sq r0, 0(a2)
  c->subu(t3, t1, v1);                              // subu t3, t1, v1
  c->sq(r0, -16, a2);                               // sq r0, -16(a2)
  c->sra(t3, t3, 4);                                // sra t3, t3, 4
  c->sq(r0, 0, a0);                                 // sq r0, 0(a0)
  // nop                                            // sll r0, r0, 0
  c->sh(t3, 0, v1);                                 // sh t3, 0(v1)
  c->daddiu(v1, t3, 1);                             // daddiu v1, t3, 1
  c->sq(r0, 0, t1);                                 // sq r0, 0(t1)
  // nop                                            // sll r0, r0, 0
  c->sq(r0, -16, t1);                               // sq r0, -16(t1)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t5, a1, 1);                             // daddiu t5, a1, 1
  c->daddiu(t4, a1, 2);                             // daddiu t4, a1, 2
  c->lw(t3, 11988, at);                             // lw t3, 11988(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t7, 11972, at);                             // lw t7, 11972(at)
  c->dsll(t0, t0, 16);                              // dsll t0, t0, 16
  c->lw(t6, 11976, at);                             // lw t6, 11976(at)
  c->or_(t4, t7, t4);                               // or t4, t7, t4
  c->lw(t7, 11980, at);                             // lw t7, 11980(at)
  c->or_(t5, t6, t5);                               // or t5, t6, t5
  c->lw(t6, 11984, at);                             // lw t6, 11984(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t8, 11992, at);                             // lw t8, 11992(at)
  c->mov64(t9, a1);                                 // or t9, a1, r0
  c->sw(t2, 8, a0);                                 // sw t2, 8(a0)
  c->or_(t2, t7, t9);                               // or t2, t7, t9
  c->sw(t8, 0, t1);                                 // sw t8, 0(t1)
  c->daddiu(a0, a0, 16);                            // daddiu a0, a0, 16
  c->sw(r0, 4, t1);                                 // sw r0, 4(t1)
  c->or_(t4, t4, t0);                               // or t4, t4, t0
  c->sw(t6, 8, t1);                                 // sw t6, 8(t1)
  c->daddiu(a3, a3, 16);                            // daddiu a3, a3, 16
  c->sw(t3, 12, t1);                                // sw t3, 12(t1)
  c->or_(t1, t5, t0);                               // or t1, t5, t0
  c->sw(t4, -4, a0);                                // sw t4, -4(a0)
  c->or_(t0, t2, t0);                               // or t0, t2, t0
  c->sw(t1, -4, a3);                                // sw t1, -4(a3)
  c->daddiu(a2, a2, 16);                            // daddiu a2, a2, 16
  c->sw(t0, -4, a2);                                // sw t0, -4(a2)
  c->addiu(t0, r0, 567);                            // addiu t0, r0, 567
  c->sw(v1, 40, at);                                // sw v1, 40(at)
  bc = c->sgpr64(a1) != c->sgpr64(t0);              // bne a1, t0, L72
  c->daddiu(v1, a1, 279);                           // daddiu v1, a1, 279
  if (bc) {goto block_13;}                          // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->addiu(v1, r0, 9);                              // addiu v1, r0, 9
  
block_13:
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 72, at);                                // sw v1, 72(at)
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 4, at);                                 // sw a0, 4(at)
  // nop                                            // sll r0, r0, 0
  c->sw(a3, 8, at);                                 // sw a3, 8(at)
  // nop                                            // sll r0, r0, 0
  c->sw(a2, 12, at);                                // sw a2, 12(at)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->lq(gp, 12448, at);                             // lq gp, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 32);                            // daddiu sp, sp, 32
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-prepare-dma-single", execute, 128);
}

} // namespace generic_prepare_dma_single

namespace generic_prepare_dma_double {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -128);                          // daddiu sp, sp, -128
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s0, 12448, at);                             // sq s0, 12448(at)
  c->sq(s1, 12464, at);                             // sq s1, 12464(at)
  c->sq(s2, 12480, at);                             // sq s2, 12480(at)
  c->sq(s3, 12496, at);                             // sq s3, 12496(at)
  c->sq(s4, 12512, at);                             // sq s4, 12512(at)
  c->sq(s5, 12528, at);                             // sq s5, 12528(at)
  c->sq(gp, 12544, at);                             // sq gp, 12544(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->lw(a3, 44, at);                                // lw a3, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lbu(v1, 16, a3);                               // lbu v1, 16(a3)
  // nop                                            // sll r0, r0, 0
  c->lh(a0, 18, a3);                                // lh a0, 18(a3)
  // nop                                            // sll r0, r0, 0
  c->lw(t7, 24, at);                                // lw t7, 24(at)
  c->sll(a2, v1, 2);                                // sll a2, v1, 2
  c->daddiu(a1, a0, 3);                             // daddiu a1, a0, 3
  c->daddu(a2, a2, v1);                             // daddu a2, a2, v1
  c->addiu(t0, r0, -4);                             // addiu t0, r0, -4
  c->and_(t6, a1, t0);                              // and t6, a1, t0
  c->sll(a1, a2, 4);                                // sll a1, a2, 4
  c->daddu(a2, t6, t6);                             // daddu a2, t6, t6
  c->daddiu(a1, a1, 112);                           // daddiu a1, a1, 112
  c->daddu(a2, a2, t6);                             // daddu a2, a2, t6
  c->dsll(t0, t6, 2);                               // dsll t0, t6, 2
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, t0, 15);                            // daddiu t0, t0, 15
  c->dsll(a2, a2, 2);                               // dsll a2, a2, 2
  c->dsra(t0, t0, 4);                               // dsra t0, t0, 4
  c->daddiu(a2, a2, 15);                            // daddiu a2, a2, 15
  c->dsll(t4, t0, 4);                               // dsll t4, t0, 4
  c->dsra(t9, a2, 4);                               // dsra t9, a2, 4
  c->dsll(t1, t9, 4);                               // dsll t1, t9, 4
  c->mov64(a2, t7);                                 // or a2, t7, r0
  c->dsra(t8, a1, 4);                               // dsra t8, a1, 4
  c->lq(t0, 11744, at);                             // lq t0, 11744(at)
  c->daddu(t2, a2, a1);                             // daddu t2, a2, a1
  c->lq(t5, 11760, at);                             // lq t5, 11760(at)
  c->daddu(t1, t2, t1);                             // daddu t1, t2, t1
  c->sq(t0, 0, a2);                                 // sq t0, 0(a2)
  c->daddiu(t0, t2, 32);                            // daddiu t0, t2, 32
  c->lq(ra, 11776, at);                             // lq ra, 11776(at)
  c->daddu(t2, t1, t4);                             // daddu t2, t1, t4
  c->sq(r0, -16, t0);                               // sq r0, -16(t0)
  c->daddiu(t1, t1, 64);                            // daddiu t1, t1, 64
  c->lq(gp, 11792, at);                             // lq gp, 11792(at)
  c->daddu(t3, t2, t4);                             // daddu t3, t2, t4
  c->sq(r0, -32, t1);                               // sq r0, -32(t1)
  c->daddiu(t2, t2, 80);                            // daddiu t2, t2, 80
  c->sq(r0, -16, t1);                               // sq r0, -16(t1)
  c->daddu(s5, t3, a1);                             // daddu s5, t3, a1
  c->sq(r0, -32, t2);                               // sq r0, -32(t2)
  c->daddiu(a1, t3, 80);                            // daddiu a1, t3, 80
  c->sq(r0, -16, t2);                               // sq r0, -16(t2)
  c->daddu(s4, s5, t4);                             // daddu s4, s5, t4
  c->sq(r0, -16, a1);                               // sq r0, -16(a1)
  c->daddiu(t3, s5, 112);                           // daddiu t3, s5, 112
  c->lw(s5, 11984, at);                             // lw s5, 11984(at)
  c->daddu(s3, s4, t4);                             // daddu s3, s4, t4
  c->sq(r0, -16, t3);                               // sq r0, -16(t3)
  c->daddiu(t4, s4, 128);                           // daddiu t4, s4, 128
  c->sq(t5, 0, a1);                                 // sq t5, 0(a1)
  c->daddiu(t5, s3, 128);                           // daddiu t5, s3, 128
  c->sq(r0, -32, t4);                               // sq r0, -32(t4)
  c->subu(s4, t5, t7);                              // subu s4, t5, t7
  c->sq(r0, -16, t4);                               // sq r0, -16(t4)
  c->sra(s4, s4, 4);                                // sra s4, s4, 4
  c->sq(r0, -16, t5);                               // sq r0, -16(t5)
  c->daddiu(s4, s4, -1);                            // daddiu s4, s4, -1
  c->sq(ra, 0, t5);                                 // sq ra, 0(t5)
  c->sh(t9, 0, t5);                                 // sh t9, 0(t5)
  c->sq(gp, 16, t5);                                // sq gp, 16(t5)
  c->sw(s5, 24, t5);                                // sw s5, 24(t5)
  c->sh(s4, 0, t7);                                 // sh s4, 0(t7)
  c->daddiu(t7, s4, 3);                             // daddiu t7, s4, 3
  c->sw(t7, 40, at);                                // sw t7, 40(at)
  c->lw(t7, 60, at);                                // lw t7, 60(at)
  c->dsubu(t9, t0, a2);                             // dsubu t9, t0, a2
  c->daddu(t7, t7, t9);                             // daddu t7, t7, t9
  c->lw(t9, 12, a2);                                // lw t9, 12(a2)
  c->sll(ra, t8, 16);                               // sll ra, t8, 16
  c->lw(t8, 68, at);                                // lw t8, 68(at)
  // nop                                            // sll r0, r0, 0
  c->lw(gp, 12, a1);                                // lw gp, 12(a1)
  c->or_(s4, t9, t8);                               // or s4, t9, t8
  c->lw(t9, 72, at);                                // lw t9, 72(at)
  c->xori(s5, t8, 38);                              // xori s5, t8, 38
  // nop                                            // sll r0, r0, 0
  c->or_(s4, s4, ra);                               // or s4, s4, ra
  c->lw(t8, 11968, at);                             // lw t8, 11968(at)
  // nop                                            // sll r0, r0, 0
  c->lw(s3, 11988, at);                             // lw s3, 11988(at)
  c->or_(gp, gp, s5);                               // or gp, gp, s5
  c->sw(s4, 12, a2);                                // sw s4, 12(a2)
  c->or_(s5, gp, ra);                               // or s5, gp, ra
  c->lw(ra, 11972, at);                             // lw ra, 11972(at)
  c->daddiu(gp, t9, 1);                             // daddiu gp, t9, 1
  c->sw(s5, 12, a1);                                // sw s5, 12(a1)
  c->daddiu(s4, t9, 2);                             // daddiu s4, t9, 2
  c->lw(s3, 11976, at);                             // lw s3, 11976(at)
  c->dsll(t6, t6, 16);                              // dsll t6, t6, 16
  c->lw(s5, 11980, at);                             // lw s5, 11980(at)
  c->or_(s4, ra, s4);                               // or s4, ra, s4
  c->lw(ra, 11984, at);                             // lw ra, 11984(at)
  c->or_(s3, s3, gp);                               // or s3, s3, gp
  c->lw(gp, 11992, at);                             // lw gp, 11992(at)
  c->mov64(s2, t9);                                 // or s2, t9, r0
  c->sw(t8, -8, t0);                                // sw t8, -8(t0)
  c->or_(s5, s5, s2);                               // or s5, s5, s2
  c->sw(gp, 4, a1);                                 // sw gp, 4(a1)
  c->or_(s4, s4, t6);                               // or s4, s4, t6
  c->sw(ra, 0, a1);                                 // sw ra, 0(a1)
  c->or_(s3, s3, t6);                               // or s3, s3, t6
  c->sw(s4, -4, t0);                                // sw s4, -4(t0)
  c->or_(s5, s5, t6);                               // or s5, s5, t6
  c->sw(s3, -4, t1);                                // sw s3, -4(t1)
  c->addiu(s4, r0, 567);                            // addiu s4, r0, 567
  c->sw(s5, -4, t2);                                // sw s5, -4(t2)
  bc = c->sgpr64(t9) != c->sgpr64(s4);              // bne t9, s4, L74
  c->daddiu(t9, t9, 279);                           // daddiu t9, t9, 279
  if (bc) {goto block_2;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->addiu(t9, r0, 9);                              // addiu t9, r0, 9
  
block_2:
  c->daddiu(s1, t9, 1);                             // daddiu s1, t9, 1
  c->lw(s0, 11976, at);                             // lw s0, 11976(at)
  c->mov64(s3, t9);                                 // or s3, t9, r0
  c->lw(s2, 11980, at);                             // lw s2, 11980(at)
  c->daddiu(s5, t9, 2);                             // daddiu s5, t9, 2
  c->lw(s4, 11972, at);                             // lw s4, 11972(at)
  c->or_(s1, s0, s1);                               // or s1, s0, s1
  c->sw(t8, -8, t3);                                // sw t8, -8(t3)
  c->or_(t8, s2, s3);                               // or t8, s2, s3
  c->sw(gp, 24, t5);                                // sw gp, 24(t5)
  c->or_(gp, s4, s5);                               // or gp, s4, s5
  // nop                                            // sll r0, r0, 0
  c->or_(s5, s1, t6);                               // or s5, s1, t6
  c->sw(ra, 28, t5);                                // sw ra, 28(t5)
  c->or_(t8, t8, t6);                               // or t8, t8, t6
  c->sw(s5, -4, t3);                                // sw s5, -4(t3)
  c->or_(ra, gp, t6);                               // or ra, gp, t6
  c->sw(t8, -4, t4);                                // sw t8, -4(t4)
  c->addiu(t6, r0, 567);                            // addiu t6, r0, 567
  c->sw(t7, 4, t5);                                 // sw t7, 4(t5)
  // nop                                            // sll r0, r0, 0
  c->sw(ra, 12, t5);                                // sw ra, 12(t5)
  bc = c->sgpr64(t9) != c->sgpr64(t6);              // bne t9, t6, L75
  c->daddiu(t5, t9, 279);                           // daddiu t5, t9, 279
  if (bc) {goto block_4;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->addiu(t5, r0, 9);                              // addiu t5, r0, 9
  
block_4:
  // nop                                            // sll r0, r0, 0
  c->sw(t5, 72, at);                                // sw t5, 72(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t0, 4, at);                                 // sw t0, 4(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t1, 8, at);                                 // sw t1, 8(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t2, 12, at);                                // sw t2, 12(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t3, 16, at);                                // sw t3, 16(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t4, 20, at);                                // sw t4, 20(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t0, 48, at);                                // lw t0, 48(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 11808, at);                             // lq t1, 11808(at)
  bc = c->sgpr64(t0) == 0;                          // beq t0, r0, L80
  c->lq(t2, 11824, at);                             // lq t2, 11824(at)
  if (bc) {goto block_13;}                          // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->lq(t3, 11840, at);                             // lq t3, 11840(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 11856, at);                             // lq t4, 11856(at)
  // nop                                            // sll r0, r0, 0
  c->lh(t5, 58, at);                                // lh t5, 58(at)
  // nop                                            // sll r0, r0, 0
  c->sq(t1, 16, a2);                                // sq t1, 16(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 32, a2);                                // sq t2, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 48, a2);                                // sq t3, 48(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t4, 64, a2);                                // sq t4, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t1, 16, a1);                                // sq t1, 16(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 32, a1);                                // sq t2, 32(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 48, a1);                                // sq t3, 48(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t4, 64, a1);                                // sq t4, 64(a1)
  bc = c->sgpr64(t5) == 0;                          // beq t5, r0, L76
  c->lq(t1, 11872, at);                             // lq t1, 11872(at)
  if (bc) {goto block_7;}                           // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->lq(t2, 11904, at);                             // lq t2, 11904(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 11936, at);                             // lq t3, 11936(at)
  //beq r0, r0, L77                                 // beq r0, r0, L77
  c->lq(t4, 12032, at);                             // lq t4, 12032(at)
  goto block_8;                                     // branch always

  
block_7:
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 11888, at);                             // lq t2, 11888(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 11936, at);                             // lq t3, 11936(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 12032, at);                             // lq t4, 12032(at)
  
block_8:
  // nop                                            // sll r0, r0, 0
  c->sq(t1, 80, a2);                                // sq t1, 80(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 96, a2);                                // sq t2, 96(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 112, a2);                               // sq t3, 112(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(t4, 80, a1);                                // sq t4, 80(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 96, a1);                                // sq t3, 96(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 112, a1);                               // sq t3, 112(a1)
  c->daddiu(t2, a3, 22);                            // daddiu t2, a3, 22
  c->addiu(t3, r0, 0);                              // addiu t3, r0, 0
  c->mov64(t4, v1);                                 // or t4, v1, r0
  c->addiu(t6, r0, 128);                            // addiu t6, r0, 128
  c->mov64(t1, a2);                                 // or t1, a2, r0
  
block_9:
  c->daddu(t1, t1, t6);                             // daddu t1, t1, t6
  c->lq(t6, 0, t0);                                 // lq t6, 0(t0)
  c->daddiu(t4, t4, -1);                            // daddiu t4, t4, -1
  c->lbu(t5, 0, t2);                                // lbu t5, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 16, t0);                                // lq t7, 16(t0)
  c->daddu(t9, t5, t5);                             // daddu t9, t5, t5
  c->lq(t8, 32, t0);                                // lq t8, 32(t0)
  c->daddu(ra, t9, t5);                             // daddu ra, t9, t5
  c->lq(t9, 48, t0);                                // lq t9, 48(t0)
  c->daddiu(gp, ra, 9);                             // daddiu gp, ra, 9
  c->lq(ra, 64, t0);                                // lq ra, 64(t0)
  c->daddiu(t0, t0, 80);                            // daddiu t0, t0, 80
  c->sq(t6, 0, t1);                                 // sq t6, 0(t1)
  // nop                                            // sll r0, r0, 0
  c->sw(t3, 12, t1);                                // sw t3, 12(t1)
  c->daddiu(t2, t2, 1);                             // daddiu t2, t2, 1
  c->sq(t7, 16, t1);                                // sq t7, 16(t1)
  c->daddu(t3, t3, gp);                             // daddu t3, t3, gp
  c->sw(t5, 28, t1);                                // sw t5, 28(t1)
  // nop                                            // sll r0, r0, 0
  c->sq(t8, 32, t1);                                // sq t8, 32(t1)
  c->addiu(t6, r0, 80);                             // addiu t6, r0, 80
  c->sq(t9, 48, t1);                                // sq t9, 48(t1)
  bc = ((s64)c->sgpr64(t4)) > 0;                    // bgtz t4, L78
  c->sq(ra, 64, t1);                                // sq ra, 64(t1)
  if (bc) {goto block_9;}                           // branch non-likely

  c->ori(t0, t5, 32768);                            // ori t0, t5, 32768
  c->sw(a0, 36, at);                                // sw a0, 36(at)
  // nop                                            // sll r0, r0, 0
  c->sw(t0, 28, t1);                                // sw t0, 28(t1)
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 92, a2);                                // sw v1, 92(a2)
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 108, a2);                               // sw a0, 108(a2)
  // nop                                            // sll r0, r0, 0
  c->sw(r0, 124, a2);                               // sw r0, 124(a2)
  c->daddiu(a3, a3, 22);                            // daddiu a3, a3, 22
  c->addiu(t0, r0, 0);                              // addiu t0, r0, 0
  c->mov64(t1, v1);                                 // or t1, v1, r0
  c->lw(t6, 52, at);                                // lw t6, 52(at)
  c->mov64(a2, a1);                                 // or a2, a1, r0
  c->addiu(t8, r0, 128);                            // addiu t8, r0, 128
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 0, t6);                                 // lq t2, 0(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 16, t6);                                // lq t3, 16(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 32, t6);                                // lq t4, 32(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 48, t6);                                // lq t5, 48(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 64, t6);                                // lq t6, 64(t6)
  
block_11:
  c->daddu(a2, a2, t8);                             // daddu a2, a2, t8
  c->lbu(t7, 0, a3);                                // lbu t7, 0(a3)
  c->daddiu(t1, t1, -1);                            // daddiu t1, t1, -1
  c->sq(t2, 0, a2);                                 // sq t2, 0(a2)
  c->daddu(t8, t7, t7);                             // daddu t8, t7, t7
  c->sw(t0, 12, a2);                                // sw t0, 12(a2)
  c->daddu(t8, t8, t7);                             // daddu t8, t8, t7
  c->sq(t3, 16, a2);                                // sq t3, 16(a2)
  c->daddiu(t8, t8, 9);                             // daddiu t8, t8, 9
  c->sw(t7, 28, a2);                                // sw t7, 28(a2)
  c->daddiu(a3, a3, 1);                             // daddiu a3, a3, 1
  // nop                                            // sll r0, r0, 0
  c->daddu(t0, t0, t8);                             // daddu t0, t0, t8
  c->sq(t4, 32, a2);                                // sq t4, 32(a2)
  c->addiu(t8, r0, 80);                             // addiu t8, r0, 80
  c->sq(t5, 48, a2);                                // sq t5, 48(a2)
  bc = ((s64)c->sgpr64(t1)) > 0;                    // bgtz t1, L79
  c->sq(t6, 64, a2);                                // sq t6, 64(a2)
  if (bc) {goto block_11;}                          // branch non-likely

  c->ori(a3, t7, 32768);                            // ori a3, t7, 32768
  c->sw(a0, 36, at);                                // sw a0, 36(at)
  // nop                                            // sll r0, r0, 0
  c->sw(a3, 28, a2);                                // sw a3, 28(a2)
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 92, a1);                                // sw v1, 92(a1)
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 108, a1);                               // sw a0, 108(a1)
  //beq r0, r0, L82                                 // beq r0, r0, L82
  c->sw(r0, 124, a1);                               // sw r0, 124(a1)
  goto block_16;                                    // branch always

  
block_13:
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 16, a2);                                // lq a3, 16(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t0, 32, a2);                                // lq t0, 32(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 48, a2);                                // lq t1, 48(a2)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 64, a2);                                // lq t2, 64(a2)
  // nop                                            // sll r0, r0, 0
  c->sq(a3, 16, a1);                                // sq a3, 16(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t0, 32, a1);                                // sq t0, 32(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t1, 48, a1);                                // sq t1, 48(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 64, a1);                                // sq t2, 64(a1)
  // nop                                            // sll r0, r0, 0
  c->lq(a3, 12032, at);                             // lq a3, 12032(at)
  // nop                                            // sll r0, r0, 0
  c->lq(t0, 11936, at);                             // lq t0, 11936(at)
  // nop                                            // sll r0, r0, 0
  c->sq(a3, 80, a1);                                // sq a3, 80(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t0, 96, a1);                                // sq t0, 96(a1)
  // nop                                            // sll r0, r0, 0
  c->sq(t0, 112, a1);                               // sq t0, 112(a1)
  c->mov64(a3, v1);                                 // or a3, v1, r0
  c->lw(t5, 52, at);                                // lw t5, 52(at)
  c->mov64(t0, a1);                                 // or t0, a1, r0
  c->addiu(t7, r0, 128);                            // addiu t7, r0, 128
  // nop                                            // sll r0, r0, 0
  c->lq(t1, 0, t5);                                 // lq t1, 0(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 16, t5);                                // lq t2, 16(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 32, t5);                                // lq t3, 32(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 48, t5);                                // lq t4, 48(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 64, t5);                                // lq t5, 64(t5)
  c->daddu(a2, a2, t7);                             // daddu a2, a2, t7
  // nop                                            // sll r0, r0, 0
  
block_14:
  c->daddu(t0, t0, t7);                             // daddu t0, t0, t7
  c->lwu(t8, 12, a2);                               // lwu t8, 12(a2)
  c->daddiu(a3, a3, -1);                            // daddiu a3, a3, -1
  c->lwu(t7, 28, a2);                               // lwu t7, 28(a2)
  c->daddiu(a2, a2, 80);                            // daddiu a2, a2, 80
  c->sq(t1, 0, t0);                                 // sq t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  c->sw(t8, 12, t0);                                // sw t8, 12(t0)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 16, t0);                                // sq t2, 16(t0)
  c->daddu(t8, t8, t6);                             // daddu t8, t8, t6
  c->sw(t7, 28, t0);                                // sw t7, 28(t0)
  // nop                                            // sll r0, r0, 0
  c->sq(t3, 32, t0);                                // sq t3, 32(t0)
  c->addiu(t7, r0, 80);                             // addiu t7, r0, 80
  c->sq(t4, 48, t0);                                // sq t4, 48(t0)
  bc = ((s64)c->sgpr64(a3)) > 0;                    // bgtz a3, L81
  c->sq(t5, 64, t0);                                // sq t5, 64(t0)
  if (bc) {goto block_14;}                          // branch non-likely

  // nop                                            // sll r0, r0, 0
  c->sw(a0, 36, at);                                // sw a0, 36(at)
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 92, a1);                                // sw v1, 92(a1)
  // nop                                            // sll r0, r0, 0
  c->sw(a0, 108, a1);                               // sw a0, 108(a1)
  // nop                                            // sll r0, r0, 0
  c->sw(r0, 124, a1);                               // sw r0, 124(a1)
  
block_16:
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12544, at);                             // lq gp, 12544(at)
  c->lq(s5, 12528, at);                             // lq s5, 12528(at)
  c->lq(s4, 12512, at);                             // lq s4, 12512(at)
  c->lq(s3, 12496, at);                             // lq s3, 12496(at)
  c->lq(s2, 12480, at);                             // lq s2, 12480(at)
  c->lq(s1, 12464, at);                             // lq s1, 12464(at)
  c->lq(s0, 12448, at);                             // lq s0, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 128);                           // daddiu sp, sp, 128
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-prepare-dma-double", execute, 256);
}

} // namespace generic_prepare_dma_double

namespace generic_envmap_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -128);                          // daddiu sp, sp, -128
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s0, 12448, at);                             // sq s0, 12448(at)
  c->sq(s1, 12464, at);                             // sq s1, 12464(at)
  c->sq(s2, 12480, at);                             // sq s2, 12480(at)
  c->sq(s3, 12496, at);                             // sq s3, 12496(at)
  c->sq(s4, 12512, at);                             // sq s4, 12512(at)
  c->sq(s5, 12528, at);                             // sq s5, 12528(at)
  c->sq(gp, 12544, at);                             // sq gp, 12544(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 44, at);                                // lw v1, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 36, at);                                // lw a0, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(t0, 0, v1);                                 // lw t0, 0(v1)
  // nop                                            // sll r0, r0, 0
  c->lw(a2, 4, v1);                                 // lw a2, 4(v1)
  // nop                                            // sll r0, r0, 0
  c->lw(t3, 16, at);                                // lw t3, 16(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 20, at);                                // lw v1, 20(at)
  // nop                                            // sll r0, r0, 0
  c->addiu(t1, r0, 255);                            // addiu t1, r0, 255
  c->addiu(a3, r0, 256);                            // addiu a3, r0, 256
  c->lui(a1, -2);                                   // lui a1, -2
  c->lui(t2, 16256);                                // lui t2, 16256
  c->ori(a1, a1, 65534);                            // ori a1, a1, 65534
  c->mtc1(f0, t2);                                  // mtc1 f0, t2
  c->daddiu(t2, a0, 3);                             // daddiu t2, a0, 3
  c->sra(t5, t2, 2);                                // sra t5, t2, 2
  c->lq(t2, 12048, at);                             // lq t2, 12048(at)
  c->sra(t4, t5, 2);                                // sra t4, t5, 2
  c->andi(t5, t5, 3);                               // andi t5, t5, 3
  bc = c->sgpr64(t4) == 0;                          // beq t4, r0, L85
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_2;}                           // branch non-likely

  
block_1:
  c->daddiu(t3, t3, 64);                            // daddiu t3, t3, 64
  c->sq(t2, -64, t3);                               // sq t2, -64(t3)
  // nop                                            // sll r0, r0, 0
  c->sq(t2, -48, t3);                               // sq t2, -48(t3)
  c->daddiu(t4, t4, -1);                            // daddiu t4, t4, -1
  c->sq(t2, -32, t3);                               // sq t2, -32(t3)
  bc = ((s64)c->sgpr64(t4)) > 0;                    // bgtz t4, L84
  c->sq(t2, -16, t3);                               // sq t2, -16(t3)
  if (bc) {goto block_1;}                           // branch non-likely

  
block_2:
  bc = c->sgpr64(t5) == 0;                          // beq t5, r0, L86
  c->daddiu(t4, t5, -1);                            // daddiu t4, t5, -1
  if (bc) {goto block_6;}                           // branch non-likely

  bc = c->sgpr64(t4) == 0;                          // beq t4, r0, L86
  c->sq(t2, 0, t3);                                 // sq t2, 0(t3)
  if (bc) {goto block_6;}                           // branch non-likely

  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->daddiu(t4, t4, -1);                            // daddiu t4, t4, -1
  bc = c->sgpr64(t4) == 0;                          // beq t4, r0, L86
  c->sq(t2, 0, t3);                                 // sq t2, 0(t3)
  if (bc) {goto block_6;}                           // branch non-likely

  c->daddiu(t3, t3, 16);                            // daddiu t3, t3, 16
  c->daddiu(t4, t4, -1);                            // daddiu t4, t4, -1
  // nop                                            // sll r0, r0, 0
  c->sq(t2, 0, t3);                                 // sq t2, 0(t3)
  
block_6:
  c->daddiu(a0, a0, -4);                            // daddiu a0, a0, -4
  c->lqc2(vf31, 12016, at);                         // lqc2 vf31, 12016(at)
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(a2, a2, a2);                            // pextlw a2, a2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyh(t2, t1);                                 // pcpyh t2, t1
  c->ld(t1, 0, t0);                                 // ld t1, 0(t0)
  c->pcpyld(t2, t2, t2);                            // pcpyld t2, t2, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a3, a3, a3);                            // pcpyld a3, a3, a3
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->sq(t2, 96, at);                                // sq t2, 96(at)
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t2, t1, t2);                              // pand t2, t1, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t2, t2, 5);                              // psllw t2, t2, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t5, t2, a2);                             // paddw t5, t2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t6, t5, 0);                             // dsrl32 t6, t5, 0
  c->lwc1(f4, 24, t5);                              // lwc1 f4, 24(t5)
  c->pcpyud(t7, t5, r0);                            // pcpyud t7, t5, r0
  c->lwc1(f3, 24, t6);                              // lwc1 f3, 24(t6)
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f2, 24, t7);                              // lwc1 f2, 24(t7)
  c->pand(t1, t1, a3);                              // pand t1, t1, a3
  c->lwc1(f1, 24, t8);                              // lwc1 f1, 24(t8)
  c->psraw(t2, t1, 8);                              // psraw t2, t1, 8
  c->lq(t1, 16, t5);                                // lq t1, 16(t5)
  c->mov128_gpr_gpr(s0, t2);                        // por s0, t2, r0
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  c->divs(f4, f0, f4);                              // div.s f4, f0, f4
  c->lq(t2, 16, t6);                                // lq t2, 16(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 16, t7);                                // lq t3, 16(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 16, t8);                                // lq t4, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 0, t5);                                 // lq t5, 0(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, t6);                                 // lq t6, 0(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f3, f3, f0);                              // sub.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f4);                                  // mfc1 t9, f4
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f2, f0);                              // sub.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f3);                                  // mfc1 ra, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f1, f0);                              // sub.s f1, f1, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t9, ra, t9);                            // pextlw t9, ra, t9
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f2);                                  // mfc1 ra, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(gp, f1);                                  // mfc1 gp, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->pextlw(ra, gp, ra);                            // pextlw ra, gp, ra
  // nop                                            // sll r0, r0, 0
  c->pcpyld(t9, ra, t9);                            // pcpyld t9, ra, t9
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t1);                       // qmtc2.ni vf21, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t2);                       // qmtc2.ni vf22, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t3);                       // qmtc2.ni vf23, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t4);                       // qmtc2.ni vf24, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t5);                        // qmtc2.ni vf9, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t6);                       // qmtc2.ni vf10, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t7);                       // qmtc2.ni vf11, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t8);                       // qmtc2.ni vf12, t8
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t9);                       // qmtc2.ni vf27, t9
  c->lq(t2, 96, at);                                // lq t2, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->ld(t1, 0, t0);                                 // ld t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t2, t1, t2);                              // pand t2, t1, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t2, t2, 5);                              // psllw t2, t2, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t5, t2, a2);                             // paddw t5, t2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t6, t5, 0);                             // dsrl32 t6, t5, 0
  c->lwc1(f3, 24, t5);                              // lwc1 f3, 24(t5)
  c->pcpyud(t7, t5, r0);                            // pcpyud t7, t5, r0
  c->lwc1(f2, 24, t6);                              // lwc1 f2, 24(t6)
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f1, 24, t7);                              // lwc1 f1, 24(t7)
  c->pand(t1, t1, a3);                              // pand t1, t1, a3
  c->lwc1(f4, 24, t8);                              // lwc1 f4, 24(t8)
  c->psraw(t2, t1, 8);                              // psraw t2, t1, 8
  c->lq(t1, 16, t5);                                // lq t1, 16(t5)
  c->mov128_gpr_gpr(s1, t2);                        // por s1, t2, r0
  c->subs(f5, f3, f0);                              // sub.s f5, f3, f0
  c->subs(f3, f2, f0);                              // sub.s f3, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f1, f0);                              // sub.s f2, f1, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f4, f0);                              // sub.s f1, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f5);                              // div.s f4, f0, f5
  c->lq(t2, 16, t6);                                // lq t2, 16(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 16, t7);                                // lq t3, 16(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t4, 16, t8);                                // lq t4, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 0, t5);                                 // lq t5, 0(t5)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 0, t6);                                 // lq t6, 0(t6)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f4);                                  // mfc1 t9, f4
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f3);                                  // mfc1 ra, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t9, ra, t9);                            // pextlw t9, ra, t9
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f2);                                  // mfc1 ra, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(gp, f1);                                  // mfc1 gp, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t1);                       // qmtc2.ni vf21, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t5);                        // qmtc2.ni vf9, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t6);                       // qmtc2.ni vf10, t6
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t7);                       // qmtc2.ni vf11, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t8);                       // qmtc2.ni vf12, t8
  c->pextlw(t1, gp, ra);                            // pextlw t1, gp, ra
  // nop                                            // sll r0, r0, 0
  c->pcpyld(t1, t1, t9);                            // pcpyld t1, t1, t9
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t2);                       // qmtc2.ni vf22, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t3);                       // qmtc2.ni vf23, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t4);                       // qmtc2.ni vf24, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t1);                       // qmtc2.ni vf27, t1
  c->lq(t2, 96, at);                                // lq t2, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  // nop                                            // sll r0, r0, 0
  c->ld(t1, 0, t0);                                 // ld t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t2, t1, t2);                              // pand t2, t1, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t2, t2, 5);                              // psllw t2, t2, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t2, t2, a2);                             // paddw t2, t2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t3, t2, 0);                             // dsrl32 t3, t2, 0
  c->lwc1(f3, 24, t2);                              // lwc1 f3, 24(t2)
  c->pcpyud(t7, t2, r0);                            // pcpyud t7, t2, r0
  c->lwc1(f2, 24, t3);                              // lwc1 f2, 24(t3)
  c->dsrl32(t8, t7, 0);                             // dsrl32 t8, t7, 0
  c->lwc1(f1, 24, t7);                              // lwc1 f1, 24(t7)
  c->pand(t1, t1, a3);                              // pand t1, t1, a3
  c->lwc1(f4, 24, t8);                              // lwc1 f4, 24(t8)
  c->psraw(t4, t1, 8);                              // psraw t4, t1, 8
  c->lq(t1, 16, t2);                                // lq t1, 16(t2)
  c->mov128_gpr_gpr(s2, t4);                        // por s2, t4, r0
  c->subs(f5, f3, f0);                              // sub.s f5, f3, f0
  c->subs(f3, f2, f0);                              // sub.s f3, f2, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f2, f1, f0);                              // sub.s f2, f1, f0
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f4, f0);                              // sub.s f1, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f4, f0, f5);                              // div.s f4, f0, f5
  c->lq(t4, 16, t3);                                // lq t4, 16(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t5, 16, t7);                                // lq t5, 16(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t6, 16, t8);                                // lq t6, 16(t8)
  // nop                                            // sll r0, r0, 0
  c->lq(t2, 0, t2);                                 // lq t2, 0(t2)
  // nop                                            // sll r0, r0, 0
  c->lq(t3, 0, t3);                                 // lq t3, 0(t3)
  // nop                                            // sll r0, r0, 0
  c->lq(t7, 0, t7);                                 // lq t7, 0(t7)
  // nop                                            // sll r0, r0, 0
  c->lq(t8, 0, t8);                                 // lq t8, 0(t8)
  c->muls(f4, f4, f0);                              // mul.s f4, f4, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t9, f4);                                  // mfc1 t9, f4
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f3, f3, f0);                              // mul.s f3, f3, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f2, f0, f2);                              // div.s f2, f0, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f3);                                  // mfc1 ra, f3
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f2, f0);                              // mul.s f2, f2, f0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  c->pextlw(t9, ra, t9);                            // pextlw t9, ra, t9
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(ra, f2);                                  // mfc1 ra, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t1);                       // qmtc2.ni vf21, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t4);                       // qmtc2.ni vf22, t4
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t5);                       // qmtc2.ni vf23, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t6);                       // qmtc2.ni vf24, t6
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t1, f1);                                  // mfc1 t1, f1
  c->pextlw(t1, t1, ra);                            // pextlw t1, t1, ra
  // nop                                            // sll r0, r0, 0
  c->pcpyld(t1, t1, t9);                            // pcpyld t1, t1, t9
  c->mov128_vf_gpr(vf9, t2);                        // qmtc2.ni vf9, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf10, t3);                       // qmtc2.ni vf10, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf11, t7);                       // qmtc2.ni vf11, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t8);                       // qmtc2.ni vf12, t8
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t1);                       // qmtc2.ni vf27, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t4, vf17);                       // qmfc2.ni t4, vf17
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t5, vf18);                       // qmfc2.ni t5, vf18
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t6, vf19);                       // qmfc2.ni t6, vf19
  bc = ((s64)c->sgpr64(a0)) <= 0;                   // blez a0, L88
  c->mov128_gpr_vf(t7, vf20);                       // qmfc2.ni t7, vf20
  if (bc) {goto block_8;}                           // branch non-likely

  
block_7:
  c->lq(t2, 96, at);                                // lq t2, 96(at)
  // Unknown instr: vcallms 48
  vcallms48(c);
  c->daddiu(a0, a0, -4);                            // daddiu a0, a0, -4
  c->ld(t1, 0, t0);                                 // ld t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->daddiu(v1, v1, 16);                            // daddiu v1, v1, 16
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t2, t1, t2);                              // pand t2, t1, t2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t2, t2, 5);                              // psllw t2, t2, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(t9, t2, a2);                             // paddw t9, t2, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(ra, t9, 0);                             // dsrl32 ra, t9, 0
  c->lwc1(f1, 24, t9);                              // lwc1 f1, 24(t9)
  c->pcpyud(s4, t9, r0);                            // pcpyud s4, t9, r0
  c->lwc1(f4, 24, ra);                              // lwc1 f4, 24(ra)
  c->subs(f3, f1, f0);                              // sub.s f3, f1, f0
  // nop                                            // sll r0, r0, 0
  c->dsrl32(s3, s4, 0);                             // dsrl32 s3, s4, 0
  c->lwc1(f1, 24, s4);                              // lwc1 f1, 24(s4)
  c->pand(t1, t1, a3);                              // pand t1, t1, a3
  c->lwc1(f2, 24, s3);                              // lwc1 f2, 24(s3)
  c->psraw(s5, t1, 8);                              // psraw s5, t1, 8
  c->lq(t1, 16, t9);                                // lq t1, 16(t9)
  c->divs(f3, f0, f3);                              // div.s f3, f0, f3
  c->lq(t2, 16, ra);                                // lq t2, 16(ra)
  c->mov128_gpr_gpr(gp, s0);                        // por gp, s0, r0
  c->subs(f4, f4, f0);                              // sub.s f4, f4, f0
  c->ppach(t4, r0, t4);                             // ppach t4, r0, t4
  c->lq(t3, 16, s4);                                // lq t3, 16(s4)
  c->ppach(t5, r0, t5);                             // ppach t5, r0, t5
  c->lq(t8, 16, s3);                                // lq t8, 16(s3)
  c->ppach(t6, r0, t6);                             // ppach t6, r0, t6
  c->lq(t9, 0, t9);                                 // lq t9, 0(t9)
  c->ppach(t7, r0, t7);                             // ppach t7, r0, t7
  c->lq(ra, 0, ra);                                 // lq ra, 0(ra)
  c->pextlw(t4, t5, t4);                            // pextlw t4, t5, t4
  c->lq(t5, 0, s4);                                 // lq t5, 0(s4)
  c->pextlw(t6, t7, t6);                            // pextlw t6, t7, t6
  c->lq(t7, 0, s3);                                 // lq t7, 0(s3)
  c->mov128_gpr_gpr(s0, s1);                        // por s0, s1, r0
  c->muls(f5, f3, f0);                              // mul.s f5, f3, f0
  c->mov128_gpr_gpr(s1, s2);                        // por s1, s2, r0
  c->divs(f3, f0, f4);                              // div.s f3, f0, f4
  c->pcpyld(t4, t6, t4);                            // pcpyld t4, t6, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t4, t4, a1);                              // pand t4, t4, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->mov128_gpr_gpr(s2, s5);                        // por s2, s5, r0
  c->mfc1(t6, f5);                                  // mfc1 t6, f5
  c->subs(f4, f1, f0);                              // sub.s f4, f1, f0
  // nop                                            // sll r0, r0, 0
  c->por(t4, t4, gp);                               // por t4, t4, gp
  // nop                                            // sll r0, r0, 0
  c->subs(f1, f2, f0);                              // sub.s f1, f2, f0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f3, f0);                              // mul.s f2, f3, f0
  c->sq(t4, -16, v1);                               // sq t4, -16(v1)
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->divs(f3, f0, f4);                              // div.s f3, f0, f4
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t4, f2);                                  // mfc1 t4, f2
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->muls(f2, f3, f0);                              // mul.s f2, f3, f0
  // nop                                            // sll r0, r0, 0
  c->pextlw(t4, t4, t6);                            // pextlw t4, t4, t6
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->divs(f1, f0, f1);                              // div.s f1, f0, f1
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t6, f2);                                  // mfc1 t6, f2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf21, t1);                       // qmtc2.ni vf21, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf22, t2);                       // qmtc2.ni vf22, t2
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf23, t3);                       // qmtc2.ni vf23, t3
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf24, t8);                       // qmtc2.ni vf24, t8
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->mfc1(t1, f1);                                  // mfc1 t1, f1
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf9, t9);                        // qmtc2.ni vf9, t9
  c->pextlw(t1, t1, t6);                            // pextlw t1, t1, t6
  c->mov128_vf_gpr(vf10, ra);                       // qmtc2.ni vf10, ra
  c->pcpyld(t1, t1, t4);                            // pcpyld t1, t1, t4
  c->mov128_vf_gpr(vf11, t5);                       // qmtc2.ni vf11, t5
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf12, t7);                       // qmtc2.ni vf12, t7
  // nop                                            // sll r0, r0, 0
  c->mov128_vf_gpr(vf27, t1);                       // qmtc2.ni vf27, t1
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t4, vf17);                       // qmfc2.ni t4, vf17
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t5, vf18);                       // qmfc2.ni t5, vf18
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t6, vf19);                       // qmfc2.ni t6, vf19
  bc = ((s64)c->sgpr64(a0)) > 0;                    // bgtz a0, L87
  c->mov128_gpr_vf(t7, vf20);                       // qmfc2.ni t7, vf20
  if (bc) {goto block_7;}                           // branch non-likely

  
block_8:
  c->daddiu(v1, v1, 16);                            // daddiu v1, v1, 16
  // nop                                            // sll r0, r0, 0
  c->ppach(t4, r0, t4);                             // ppach t4, r0, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t5, r0, t5);                             // ppach t5, r0, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t6, r0, t6);                             // ppach t6, r0, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t7, r0, t7);                             // ppach t7, r0, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t4, t5, t4);                            // pextlw t4, t5, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextlw(t6, t7, t6);                            // pextlw t6, t7, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t4, t6, t4);                            // pcpyld t4, t6, t4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t4, t4, a1);                              // pand t4, t4, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->por(t4, t4, s0);                               // por t4, t4, s0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->sq(t4, -16, v1);                               // sq t4, -16(v1)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12544, at);                             // lq gp, 12544(at)
  c->lq(s5, 12528, at);                             // lq s5, 12528(at)
  c->lq(s4, 12512, at);                             // lq s4, 12512(at)
  c->lq(s3, 12496, at);                             // lq s3, 12496(at)
  c->lq(s2, 12480, at);                             // lq s2, 12480(at)
  c->lq(s1, 12464, at);                             // lq s1, 12464(at)
  c->lq(s0, 12448, at);                             // lq s0, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 128);                           // daddiu sp, sp, 128
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-envmap-proc", execute, 256);
}

} // namespace generic_envmap_proc

namespace generic_light_proc {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  bool bc = false;
  u32 call_addr = 0;
  c->daddiu(sp, sp, -96);                           // daddiu sp, sp, -96
  c->sd(ra, 12432, at);                             // sd ra, 12432(at)
  c->sq(s2, 12448, at);                             // sq s2, 12448(at)
  c->sq(s3, 12464, at);                             // sq s3, 12464(at)
  c->sq(s4, 12480, at);                             // sq s4, 12480(at)
  c->sq(s5, 12496, at);                             // sq s5, 12496(at)
  c->sq(gp, 12512, at);                             // sq gp, 12512(at)
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 44, at);                                // lw v1, 44(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a1, 36, at);                                // lw a1, 36(at)
  // nop                                            // sll r0, r0, 0
  c->lw(a0, 4, v1);                                 // lw a0, 4(v1)
  // nop                                            // sll r0, r0, 0
  c->lw(t0, 0, v1);                                 // lw t0, 0(v1)
  // nop                                            // sll r0, r0, 0
  c->lw(t3, 4, at);                                 // lw t3, 4(at)
  // nop                                            // sll r0, r0, 0
  c->lw(v1, 8, at);                                 // lw v1, 8(at)
  c->addiu(a3, r0, 255);                            // addiu a3, r0, 255
  c->lw(t2, 12, at);                                // lw t2, 12(at)
  c->addiu(a2, r0, 256);                            // addiu a2, r0, 256
  c->lui(t1, -2);                                   // lui t1, -2
  c->mov64(t4, a1);                                 // or t4, a1, r0
  c->lqc2(vf10, 12688, at);                         // lqc2 vf10, 12688(at)
  c->ori(a1, t1, 65534);                            // ori a1, t1, 65534
  c->lqc2(vf11, 12704, at);                         // lqc2 vf11, 12704(at)
  c->pextlw(a1, a1, a1);                            // pextlw a1, a1, a1
  c->lqc2(vf12, 12720, at);                         // lqc2 vf12, 12720(at)
  c->pextlw(t1, a0, a0);                            // pextlw t1, a0, a0
  c->lqc2(vf15, 12752, at);                         // lqc2 vf15, 12752(at)
  c->pextlw(a0, a1, a1);                            // pextlw a0, a1, a1
  c->lqc2(vf14, 12736, at);                         // lqc2 vf14, 12736(at)
  c->pextlw(a1, t1, t1);                            // pextlw a1, t1, t1
  c->lqc2(vf16, 12768, at);                         // lqc2 vf16, 12768(at)
  c->pcpyh(a3, a3);                                 // pcpyh a3, a3
  c->lqc2(vf13, 12784, at);                         // lqc2 vf13, 12784(at)
  c->pcpyh(t1, a2);                                 // pcpyh t1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(a2, a3, a3);                            // pcpyld a2, a3, a3
  c->lqc2(vf9, 12144, at);                          // lqc2 vf9, 12144(at)
  c->pcpyld(a3, t1, t1);                            // pcpyld a3, t1, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->ldr(t1, 0, t0);                                // ldr t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  c->ldl(t1, 7, t0);                                // ldl t1, 7(t0)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t5, t1, a2);                              // pand t5, t1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(s5, t5, a1);                             // paddw s5, t5, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t9, s5, 0);                             // dsrl32 t9, s5, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyud(s4, s5, r0);                            // pcpyud s4, s5, r0
  c->lq(t6, 0, s5);                                 // lq t6, 0(s5)
  c->dsrl32(ra, s4, 0);                             // dsrl32 ra, s4, 0
  c->lq(t7, 0, t9);                                 // lq t7, 0(t9)
  c->pand(t8, t1, a3);                              // pand t8, t1, a3
  c->lq(t5, 0, s4);                                 // lq t5, 0(s4)
  c->psraw(gp, t8, 8);                              // psraw gp, t8, 8
  c->lq(t8, 0, ra);                                 // lq t8, 0(ra)
  c->pextuw(s3, t7, t6);                            // pextuw s3, t7, t6
  c->lq(s5, 16, s5);                                // lq s5, 16(s5)
  c->pextuw(s2, t8, t5);                            // pextuw s2, t8, t5
  c->lq(t9, 16, t9);                                // lq t9, 16(t9)
  c->pcpyud(s3, s3, s2);                            // pcpyud s3, s3, s2
  c->lq(s4, 16, s4);                                // lq s4, 16(s4)
  c->pand(s3, s3, a0);                              // pand s3, s3, a0
  c->lq(ra, 16, ra);                                // lq ra, 16(ra)
  c->por(s3, s3, gp);                               // por s3, s3, gp
  c->mov128_vf_gpr(vf1, s5);                        // qmtc2.ni vf1, s5
  c->pextub(gp, r0, s5);                            // pextub gp, r0, s5
  c->sq(s3, 0, t2);                                 // sq s3, 0(t2)
  c->pextub(s5, r0, t9);                            // pextub s5, r0, t9
  c->mov128_vf_gpr(vf2, t9);                        // qmtc2.ni vf2, t9
  c->pextub(t9, r0, s4);                            // pextub t9, r0, s4
  c->mov128_vf_gpr(vf3, s4);                        // qmtc2.ni vf3, s4
  c->pextub(s4, r0, ra);                            // pextub s4, r0, ra
  c->mov128_vf_gpr(vf4, ra);                        // qmtc2.ni vf4, ra
  c->pextuh(ra, r0, gp);                            // pextuh ra, r0, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuh(gp, r0, s5);                            // pextuh gp, r0, s5
  c->mov128_vf_gpr(vf5, ra);                        // qmtc2.ni vf5, ra
  c->pextuh(t9, r0, t9);                            // pextuh t9, r0, t9
  c->mov128_vf_gpr(vf6, gp);                        // qmtc2.ni vf6, gp
  c->pextuh(ra, r0, s4);                            // pextuh ra, r0, s4
  c->mov128_vf_gpr(vf7, t9);                        // qmtc2.ni vf7, t9
  c->prot3w(t8, t8);                                // prot3w t8, t8
  c->mov128_vf_gpr(vf8, ra);                        // qmtc2.ni vf8, ra
  c->prot3w(t7, t7);                                // prot3w t7, t7
  // Unknown instr: vcallms 0
  vcallms0(c);
  c->pextuw(t9, t7, t6);                            // pextuw t9, t7, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t7, t5, t7);                            // pcpyld t7, t5, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t6, t9, t6);                            // pcpyld t6, t9, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t2, t2, 16);                            // daddiu t2, t2, 16
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t5, t8, t5);                            // pextuw t5, t8, t5
  c->sq(t7, 16, t3);                                // sq t7, 16(t3)
  c->pcpyld(t5, t8, t5);                            // pcpyld t5, t8, t5
  c->sq(t6, 0, t3);                                 // sq t6, 0(t3)
  // nop                                            // sll r0, r0, 0
  c->sq(t5, 32, t3);                                // sq t5, 32(t3)
  c->daddiu(t3, t3, 48);                            // daddiu t3, t3, 48
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t4, t4, -4);                            // daddiu t4, t4, -4
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  bc = ((s64)c->sgpr64(t4)) <= 0;                   // blez t4, L91
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  if (bc) {goto block_2;}                           // branch non-likely

  
block_1:
  // nop                                            // sll r0, r0, 0
  c->ldr(t1, 0, t0);                                // ldr t1, 0(t0)
  // nop                                            // sll r0, r0, 0
  c->ldl(t1, 7, t0);                                // ldl t1, 7(t0)
  // nop                                            // sll r0, r0, 0
  c->daddiu(t0, t0, 8);                             // daddiu t0, t0, 8
  c->pextlh(t1, r0, t1);                            // pextlh t1, r0, t1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pand(t5, t1, a2);                              // pand t5, t1, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->psllw(t5, t5, 5);                              // psllw t5, t5, 5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->paddw(s5, t5, a1);                             // paddw s5, t5, a1
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->dsrl32(t9, s5, 0);                             // dsrl32 t9, s5, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyud(s4, s5, r0);                            // pcpyud s4, s5, r0
  c->lq(t6, 0, s5);                                 // lq t6, 0(s5)
  c->dsrl32(ra, s4, 0);                             // dsrl32 ra, s4, 0
  c->lq(t7, 0, t9);                                 // lq t7, 0(t9)
  c->pand(t8, t1, a3);                              // pand t8, t1, a3
  c->lq(t5, 0, s4);                                 // lq t5, 0(s4)
  c->psraw(gp, t8, 8);                              // psraw gp, t8, 8
  c->lq(t8, 0, ra);                                 // lq t8, 0(ra)
  c->pextuw(s3, t7, t6);                            // pextuw s3, t7, t6
  c->lq(s5, 16, s5);                                // lq s5, 16(s5)
  c->pextuw(s2, t8, t5);                            // pextuw s2, t8, t5
  c->lq(t9, 16, t9);                                // lq t9, 16(t9)
  c->pcpyud(s3, s3, s2);                            // pcpyud s3, s3, s2
  c->lq(s4, 16, s4);                                // lq s4, 16(s4)
  c->pand(s3, s3, a0);                              // pand s3, s3, a0
  c->lq(ra, 16, ra);                                // lq ra, 16(ra)
  c->por(s3, s3, gp);                               // por s3, s3, gp
  c->mov128_vf_gpr(vf1, s5);                        // qmtc2.ni vf1, s5
  c->pextub(gp, r0, s5);                            // pextub gp, r0, s5
  c->sq(s3, 0, t2);                                 // sq s3, 0(t2)
  c->pextub(s5, r0, t9);                            // pextub s5, r0, t9
  c->mov128_vf_gpr(vf2, t9);                        // qmtc2.ni vf2, t9
  c->pextub(t9, r0, s4);                            // pextub t9, r0, s4
  c->mov128_vf_gpr(vf3, s4);                        // qmtc2.ni vf3, s4
  c->pextub(s4, r0, ra);                            // pextub s4, r0, ra
  c->mov128_vf_gpr(vf4, ra);                        // qmtc2.ni vf4, ra
  c->pextuh(ra, r0, gp);                            // pextuh ra, r0, gp
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuh(gp, r0, s5);                            // pextuh gp, r0, s5
  c->mov128_vf_gpr(vf5, ra);                        // qmtc2.ni vf5, ra
  c->pextuh(t9, r0, t9);                            // pextuh t9, r0, t9
  c->mov128_vf_gpr(vf6, gp);                        // qmtc2.ni vf6, gp
  c->pextuh(ra, r0, s4);                            // pextuh ra, r0, s4
  c->mov128_vf_gpr(vf7, t9);                        // qmtc2.ni vf7, t9
  c->prot3w(t8, t8);                                // prot3w t8, t8
  c->mov128_vf_gpr(vf8, ra);                        // qmtc2.ni vf8, ra
  c->prot3w(t7, t7);                                // prot3w t7, t7
  // Unknown instr: vcallms 0
  vcallms0(c);
  c->pextuw(t9, t7, t6);                            // pextuw t9, t7, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t7, t5, t7);                            // pcpyld t7, t5, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t6, t9, t6);                            // pcpyld t6, t9, t6
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pextuw(t5, t8, t5);                            // pextuw t5, t8, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->pcpyld(t5, t8, t5);                            // pcpyld t5, t8, t5
  c->sq(t6, 0, t3);                                 // sq t6, 0(t3)
  // nop                                            // sll r0, r0, 0
  c->sq(t7, 16, t3);                                // sq t7, 16(t3)
  // nop                                            // sll r0, r0, 0
  c->sq(t5, 32, t3);                                // sq t5, 32(t3)
  c->daddiu(t2, t2, 16);                            // daddiu t2, t2, 16
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->daddiu(t3, t3, 48);                            // daddiu t3, t3, 48
  c->mov128_gpr_vf(t7, vf21);                       // qmfc2.ni t7, vf21
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t8, vf22);                       // qmfc2.ni t8, vf22
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t5, vf23);                       // qmfc2.ni t5, vf23
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(t6, vf24);                       // qmfc2.ni t6, vf24
  c->ppach(t7, t8, t7);                             // ppach t7, t8, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(t5, t6, t5);                             // ppach t5, t6, t5
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(t5, t5, t7);                             // ppacb t5, t5, t7
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->sq(t5, 0, v1);                                 // sq t5, 0(v1)
  c->daddiu(t4, t4, -4);                            // daddiu t4, t4, -4
  c->daddiu(v1, v1, 16);                            // daddiu v1, v1, 16
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  bc = ((s64)c->sgpr64(t4)) > 0;                    // bgtz t4, L90
  // nop                                            // sll r0, r0, 0
  if (bc) {goto block_1;}                           // branch non-likely

  
block_2:
  // nop                                            // sll r0, r0, 0
  // nop                                            // vnop
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(a2, vf17);                       // qmfc2.ni a2, vf17
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(a3, vf18);                       // qmfc2.ni a3, vf18
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(a0, vf19);                       // qmfc2.ni a0, vf19
  // nop                                            // sll r0, r0, 0
  c->mov128_gpr_vf(a1, vf20);                       // qmfc2.ni a1, vf20
  c->ppach(a2, a3, a2);                             // ppach a2, a3, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppach(a0, a1, a0);                             // ppach a0, a1, a0
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  c->ppacb(a0, a0, a2);                             // ppacb a0, a0, a2
  c->mfc1(r0, f31);                                 // mfc1 r0, f31
  // nop                                            // sll r0, r0, 0
  c->sq(a0, 0, v1);                                 // sq a0, 0(v1)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  c->ld(ra, 12432, at);                             // ld ra, 12432(at)
  c->lq(gp, 12512, at);                             // lq gp, 12512(at)
  c->lq(s5, 12496, at);                             // lq s5, 12496(at)
  c->lq(s4, 12480, at);                             // lq s4, 12480(at)
  c->lq(s3, 12464, at);                             // lq s3, 12464(at)
  c->lq(s2, 12448, at);                             // lq s2, 12448(at)
  //jr ra                                           // jr ra
  c->daddiu(sp, sp, 96);                            // daddiu sp, sp, 96
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-light-proc", execute, 128);
}

} // namespace generic_light_proc

namespace generic_dma_from_spr {
struct Cache {
  void* fake_scratchpad_data; // *fake-scratchpad-data*
} cache;

u64 execute(void* ctxt) {
  auto* c = (ExecutionContext*)ctxt;
  [[maybe_unused]] bool bc = false;
  [[maybe_unused]] u32 call_addr = 0;
  u32 qwc = 0;
  u32 madr = 0;
  u32 sadr = 0;
  // nop                                            // sll r0, r0, 0
  get_fake_spad_addr2(at, cache.fake_scratchpad_data, 0, c);// lui at, 28672
  c->lui(a2, 4096);                                 // lui a2, 4096
  c->lwu(v1, 60, at);                               // lwu v1, 60(at)
  c->ori(a2, a2, 53248);                            // ori a2, a2, 53248
  // c->lw(t0, 0, a2);                              // lw t0, 0(a2)
  // nop                                            // sll r0, r0, 0
  c->daddiu(a3, at, 92);                            // daddiu a3, at, 92
  c->andi(a0, a0, 16383);                           // andi a0, a0, 16383
  c->andi(t0, t0, 256);                             // andi t0, t0, 256
  // nop                                            // sll r0, r0, 0
//   bc = c->sgpr64(t0) == 0;                          // beq t0, r0, L94
//   // nop                                            // sll r0, r0, 0
//   if (bc) {goto block_4;}                           // branch non-likely

  c->mov64(t0, a2);                                 // or t0, a2, r0
  // nop                                            // sll r0, r0, 0
  
// block_2:
//   c->lw(t1, 0, a3);                                 // lw t1, 0(a3)
//   // nop                                            // sll r0, r0, 0
//   c->lw(t2, 0, t0);                                 // lw t2, 0(t0)
//   // nop                                            // sll r0, r0, 0
//   c->andi(t2, t2, 256);                             // andi t2, t2, 256
//   c->daddiu(t1, t1, 1);                             // daddiu t1, t1, 1
//   bc = c->sgpr64(t2) != 0;                          // bne t2, r0, L93
//   c->sw(t1, 0, a3);                                 // sw t1, 0(a3)
//   if (bc) {goto block_2;}                           // branch non-likely
//
//   c->gprs[a3].du64[0] = 0;                          // or a3, r0, r0
  
// block_4:
  c->dsll(a3, a1, 4);                               // dsll a3, a1, 4
  // c->sw(a0, 128, a2);                            // sw a0, 128(a2)
  sadr = c->sgpr64(a0);
  // nop                                            // sll r0, r0, 0
  // c->sw(v1, 16, a2);                             // sw v1, 16(a2)
  madr = c->sgpr64(v1);
  c->addiu(a0, r0, 256);                            // addiu a0, r0, 256
  // c->sw(a1, 32, a2);                             // sw a1, 32(a2)
  qwc = c->sgpr64(a1);
  c->daddu(v1, v1, a3);                             // daddu v1, v1, a3
  // c->sw(a0, 0, a2);                              // sw a0, 0(a2)
  spad_from_dma_no_sadr_off(cache.fake_scratchpad_data, madr, sadr, qwc);
  // nop                                            // sll r0, r0, 0
  c->sw(v1, 60, at);                                // sw v1, 60(at)
  c->gprs[v0].du64[0] = 0;                          // or v0, r0, r0
  //jr ra                                           // jr ra
  c->daddu(sp, sp, r0);                             // daddu sp, sp, r0
  goto end_of_function;                             // return

  // nop                                            // sll r0, r0, 0
  // nop                                            // sll r0, r0, 0
end_of_function:
  return c->gprs[v0].du64[0];
}

void link() {
  cache.fake_scratchpad_data = intern_from_c(-1, 0, "*fake-scratchpad-data*").c();
  gLinkedFunctionTable.reg("generic-dma-from-spr", execute, 256);
}

} // namespace generic_dma_from_spr

} // namespace Mips2C
