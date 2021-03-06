// REQUIRES: powerpc-registered-target
// RUN: %clang_cc1 -faltivec -target-feature +power8-vector -triple powerpc64-unknown-unknown -emit-llvm %s -o - | FileCheck %s
// RUN: %clang_cc1 -faltivec -target-feature +power8-vector -triple powerpc64le-unknown-unknown -emit-llvm %s -o - | FileCheck %s -check-prefix=CHECK-LE
// RUN: not %clang_cc1 -faltivec -target-feature +vsx -triple powerpc64-unknown-unknown -emit-llvm %s -o - 2>&1 | FileCheck %s -check-prefix=CHECK-PPC
// Added -target-feature +vsx above to avoid errors about "vector double" and to
// generate the correct errors for functions that are only overloaded with VSX
// (vec_cmpge, vec_cmple). Without this option, there is only one overload so
// it is selected.

vector signed char vsc = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 };
vector unsigned char vuc = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 };
vector bool char vbc = { 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1 };

vector signed short vss = { 0, 1, 2, 3, 4, 5, 6, 7 };
vector unsigned short vus = { 0, 1, 2, 3, 4, 5, 6, 7 };
vector bool short vbs = { 1, 1, 0, 0, 0, 0, 1, 1 };

vector signed int vsi = { -1, 2, -3, 4 };
vector unsigned int vui = { 1, 2, 3, 4 };
vector bool int vbi = {0, -1, -1, 0};

vector signed long long vsll = { 1, 2 };
vector unsigned long long vull = { 1, 2 };
vector bool long long vbll = { 1, 0 };

vector float vfa = { 1.e-4f, -132.23f, -22.1, 32.00f };
vector double vda = { 1.e-11, -132.23e10 };

int res_i;
vector signed char res_vsc;
vector unsigned char res_vuc;
vector bool char res_vbc;

vector signed short res_vss;
vector unsigned short res_vus;
vector bool short res_vbs;

vector int res_vsi;
vector unsigned int res_vui;
vector bool int res_vbi;

vector signed long long res_vsll;
vector unsigned long long res_vull;
vector bool long long res_vbll;

vector double res_vf;
vector double res_vd;

// CHECK-LABEL: define void @test1
void test1() {

  /* vec_abs */
  res_vsll = vec_abs(vsll);
// CHECK: call <2 x i64> @llvm.ppc.altivec.vmaxsd(<2 x i64> %{{[0-9]*}}, <2 x i64>
// CHECK-LE: call <2 x i64> @llvm.ppc.altivec.vmaxsd(<2 x i64> %{{[0-9]*}}, <2 x i64>
// CHECK-PPC: error: call to 'vec_abs' is ambiguous

  res_vd = vec_abs(vda);
// CHECK: store <2 x i64> <i64 9223372036854775807, i64 9223372036854775807>, <2 x i64>*
// CHECK: and <2 x i64>
// CHECK-LE: store <2 x i64> <i64 9223372036854775807, i64 9223372036854775807>, <2 x i64>*
// CHECK-LE: and <2 x i64>
// CHECK-PPC: error: call to 'vec_abs' is ambiguous

  /* vec_add */
  res_vsll = vec_add(vsll, vsll);
// CHECK: add <2 x i64>
// CHECK-LE: add <2 x i64>
// CHECK-PPC: error: call to 'vec_add' is ambiguous

  res_vull = vec_add(vull, vull);
// CHECK: add <2 x i64>
// CHECK-LE: add <2 x i64>
// CHECK-PPC: error: call to 'vec_add' is ambiguous

  /* vec_mergee */  
  res_vbi = vec_mergee(vbi, vbi);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm
  
  res_vsi = vec_mergee(vsi, vsi);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm

  res_vui = vec_mergee(vui, vui);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm
// CHECK-PPC: warning: implicit declaration of function 'vec_mergee'

  /* vec_mergeo */
  res_vbi = vec_mergeo(vbi, vbi);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm

  res_vsi = vec_mergeo(vsi, vsi);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm

  res_vui = vec_mergeo(vui, vui);
// CHECK: @llvm.ppc.altivec.vperm
// CHECK-LE: @llvm.ppc.altivec.vperm
// CHECK-PPC: warning: implicit declaration of function 'vec_mergeo'
  
  /* vec_cmpeq */
  res_vbll = vec_cmpeq(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd
// CHECK-LE: @llvm.ppc.altivec.vcmpequd
// CHECK-PPC: error: call to 'vec_cmpeq' is ambiguous

  res_vbll = vec_cmpeq(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd
// CHECK-LE: @llvm.ppc.altivec.vcmpequd
// CHECK-PPC: error: call to 'vec_cmpeq' is ambiguous

  /* vec_cmpge */
  res_vbll = vec_cmpge(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd
// CHECK-PPC: error: call to 'vec_cmpge' is ambiguous

  res_vbll = vec_cmpge(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud
// CHECK-PPC: error: call to 'vec_cmpge' is ambiguous

  /* vec_cmple */
  res_vbll = vec_cmple(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd
// CHECK-PPC: error: call to 'vec_cmple' is ambiguous

  res_vbll = vec_cmple(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud
// CHECK-PPC: error: call to 'vec_cmple' is ambiguous

  /* vec_cmpgt */
  res_vbll = vec_cmpgt(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd
// CHECK-PPC: error: call to 'vec_cmpgt' is ambiguous

  res_vbll = vec_cmpgt(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud
// CHECK-PPC: error: call to 'vec_cmpgt' is ambiguous

  /* vec_cmplt */
  res_vbll = vec_cmplt(vsll, vsll);
// CHECK: call <2 x i64> @llvm.ppc.altivec.vcmpgtsd(<2 x i64> %{{[0-9]*}}, <2 x i64> %{{[0-9]*}})
// CHECK-LE: call <2 x i64> @llvm.ppc.altivec.vcmpgtsd(<2 x i64> %{{[0-9]*}}, <2 x i64> %{{[0-9]*}})
// CHECK-PPC: error: call to 'vec_cmplt' is ambiguous

  res_vbll = vec_cmplt(vull, vull);
// CHECK: call <2 x i64> @llvm.ppc.altivec.vcmpgtud(<2 x i64> %{{[0-9]*}}, <2 x i64> %{{[0-9]*}})
// CHECK-LE: call <2 x i64> @llvm.ppc.altivec.vcmpgtud(<2 x i64> %{{[0-9]*}}, <2 x i64> %{{[0-9]*}})
// CHECK-PPC: error: call to 'vec_cmplt' is ambiguous

  /* vec_eqv */
  res_vsc =  vec_eqv(vsc, vsc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vsc =  vec_eqv(vbc, vsc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vsc =  vec_eqv(vsc, vbc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vuc =  vec_eqv(vuc, vuc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vuc =  vec_eqv(vbc, vuc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vuc =  vec_eqv(vuc, vbc);
// CHECK: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-LE: [[T1:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <16 x i8> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <16 x i8>
// CHECK-PPC: error: assigning to

  res_vss =  vec_eqv(vss, vss);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vss =  vec_eqv(vbs, vss);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vss =  vec_eqv(vss, vbs);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vus =  vec_eqv(vus, vus);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vus =  vec_eqv(vbs, vus);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vus =  vec_eqv(vus, vbs);
// CHECK: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-LE: [[T1:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <8 x i16> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <8 x i16>
// CHECK-PPC: error: assigning to

  res_vsi =  vec_eqv(vsi, vsi);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vsi =  vec_eqv(vbi, vsi);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vsi =  vec_eqv(vsi, vbi);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vui =  vec_eqv(vui, vui);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vui =  vec_eqv(vbi, vui);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vui =  vec_eqv(vui, vbi);
// CHECK: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-LE: call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.*}}, <4 x i32> {{.+}})
// CHECK-PPC: error: assigning to

  res_vsll =  vec_eqv(vsll, vsll);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vsll =  vec_eqv(vbll, vsll);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vsll =  vec_eqv(vsll, vbll);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vull =  vec_eqv(vull, vull);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vull =  vec_eqv(vbll, vull);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vull =  vec_eqv(vull, vbll);
// CHECK: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x i64> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x i64>
// CHECK-PPC: error: assigning to

  res_vf = vec_eqv(vfa, vfa);
// CHECK: [[T1:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <4 x float>
// CHECK-LE: [[T1:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <4 x float>
// CHECK-PPC: error: assigning to

  res_vf = vec_eqv(vbi, vfa);
// CHECK: [[T2:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.+}}, <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <4 x float>
// CHECK-LE: [[T2:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.+}}, <4 x i32> [[T2]])
// CHECK-PPC: error: assigning to

  res_vf = vec_eqv(vfa, vbi);
// CHECK: [[T1:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32>
// CHECK: bitcast <4 x i32> [[T3]] to <4 x float>
// CHECK-LE: [[T1:%.+]] = bitcast <4 x float> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32>
// CHECK-LE: bitcast <4 x i32> [[T3]] to <4 x float>
// CHECK-PPC: error: assigning to

  res_vd = vec_eqv(vda, vda);
// CHECK: [[T1:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK: [[T2:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-PPC: error: assigning to

  res_vd = vec_eqv(vbll, vda);
// CHECK: [[T2:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.+}}, <4 x i32> [[T2]])
// CHECK: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-LE: [[T2:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> {{.+}}, <4 x i32> [[T2]])
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-PPC: error: assigning to

  res_vd = vec_eqv(vda, vbll);
// CHECK: [[T1:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32>
// CHECK: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-LE: [[T1:%.+]] = bitcast <2 x double> {{.+}} to <4 x i32>
// CHECK-LE: [[T3:%.+]] = call <4 x i32> @llvm.ppc.vsx.xxleqv(<4 x i32> [[T1]], <4 x i32>
// CHECK-LE: bitcast <4 x i32> [[T3]] to <2 x double>
// CHECK-PPC: error: assigning to

  /* ----------------------- predicates --------------------------- */
  /* vec_all_eq */
  res_i = vec_all_eq(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  res_i = vec_all_eq(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_eq' is ambiguous

  /* vec_all_ne */
  res_i = vec_all_ne(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  res_i = vec_all_ne(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_all_ne' is ambiguous

  /* vec_any_eq */
  res_i = vec_any_eq(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  res_i = vec_any_eq(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_eq' is ambiguous

  /* vec_any_ne */
  res_i = vec_any_ne(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  res_i = vec_any_ne(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpequd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpequd.p
// CHECK-PPC: error: call to 'vec_any_ne' is ambiguous

  /* vec_all_ge */
  res_i = vec_all_ge(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  res_i = vec_all_ge(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_ge' is ambiguous

  /* vec_all_gt */
  res_i = vec_all_gt(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  res_i = vec_all_gt(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_gt' is ambiguous

  /* vec_all_le */
  res_i = vec_all_le(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  res_i = vec_all_le(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_le' is ambiguous

  /* vec_all_lt */
  res_i = vec_all_lt(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  res_i = vec_all_lt(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_all_lt' is ambiguous

  /* vec_any_ge */
  res_i = vec_any_ge(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  res_i = vec_any_ge(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_ge' is ambiguous

  /* vec_any_gt */
  res_i = vec_any_gt(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  res_i = vec_any_gt(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_gt' is ambiguous

  /* vec_any_le */
  res_i = vec_any_le(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  res_i = vec_any_le(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_le' is ambiguous

  /* vec_any_lt */
  res_i = vec_any_lt(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtsd.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vull, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vull, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vbll, vull);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  res_i = vec_any_lt(vbll, vbll);
// CHECK: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-LE: @llvm.ppc.altivec.vcmpgtud.p
// CHECK-PPC: error: call to 'vec_any_lt' is ambiguous

  /* vec_max */
  res_vsll = vec_max(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  res_vsll = vec_max(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  res_vsll = vec_max(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  res_vull = vec_max(vull, vull);
// CHECK: @llvm.ppc.altivec.vmaxud
// CHECK-LE: @llvm.ppc.altivec.vmaxud
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  res_vull = vec_max(vbll, vull);
// CHECK: @llvm.ppc.altivec.vmaxud
// CHECK-LE: @llvm.ppc.altivec.vmaxud
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  res_vull = vec_max(vull, vbll);
// CHECK: @llvm.ppc.altivec.vmaxud
// CHECK-LE: @llvm.ppc.altivec.vmaxud
// CHECK-PPC: error: call to 'vec_max' is ambiguous

  /* vec_min */
  res_vsll = vec_min(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  res_vsll = vec_min(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  res_vsll = vec_min(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  res_vull = vec_min(vull, vull);
// CHECK: @llvm.ppc.altivec.vminud
// CHECK-LE: @llvm.ppc.altivec.vminud
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  res_vull = vec_min(vbll, vull);
// CHECK: @llvm.ppc.altivec.vminud
// CHECK-LE: @llvm.ppc.altivec.vminud
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  res_vull = vec_min(vull, vbll);
// CHECK: @llvm.ppc.altivec.vminud
// CHECK-LE: @llvm.ppc.altivec.vminud
// CHECK-PPC: error: call to 'vec_min' is ambiguous

  /* vec_mule */
  res_vsll = vec_mule(vsi, vsi);
// CHECK: @llvm.ppc.altivec.vmulesw
// CHECK-LE: @llvm.ppc.altivec.vmulosw
// CHECK-PPC: error: call to 'vec_mule' is ambiguous

  res_vull = vec_mule(vui , vui);
// CHECK: @llvm.ppc.altivec.vmuleuw
// CHECK-LE: @llvm.ppc.altivec.vmulouw
// CHECK-PPC: error: call to 'vec_mule' is ambiguous

  /* vec_mulo */
  res_vsll = vec_mulo(vsi, vsi);
// CHECK: @llvm.ppc.altivec.vmulosw
// CHECK-LE: @llvm.ppc.altivec.vmulesw
// CHECK-PPC: error: call to 'vec_mulo' is ambiguous

  res_vull = vec_mulo(vui, vui);
// CHECK: @llvm.ppc.altivec.vmulouw
// CHECK-LE: @llvm.ppc.altivec.vmuleuw
// CHECK-PPC: error: call to 'vec_mulo' is ambiguous

  /* vec_packs */
  res_vsi = vec_packs(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vpksdss
// CHECK-LE: @llvm.ppc.altivec.vpksdss
// CHECK-PPC: error: call to 'vec_packs' is ambiguous

  res_vui = vec_packs(vull, vull);
// CHECK: @llvm.ppc.altivec.vpkudus
// CHECK-LE: @llvm.ppc.altivec.vpkudus
// CHECK-PPC: error: call to 'vec_packs' is ambiguous

  /* vec_packsu */
  res_vui = vec_packsu(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vpksdus
// CHECK-LE: @llvm.ppc.altivec.vpksdus
// CHECK-PPC: error: call to 'vec_packsu' is ambiguous

  res_vui = vec_packsu(vull, vull);
// CHECK: @llvm.ppc.altivec.vpkudus
// CHECK-LE: @llvm.ppc.altivec.vpkudus
// CHECK-PPC: error: call to 'vec_packsu' is ambiguous

  /* vec_rl */
  res_vsll = vec_rl(vsll, vull);
// CHECK: @llvm.ppc.altivec.vrld
// CHECK-LE: @llvm.ppc.altivec.vrld
// CHECK-PPC: error: call to 'vec_rl' is ambiguous

  res_vull = vec_rl(vull, vull);
// CHECK: @llvm.ppc.altivec.vrld
// CHECK-LE: @llvm.ppc.altivec.vrld
// CHECK-PPC: error: call to 'vec_rl' is ambiguous

  /* vec_sl */
  res_vsll = vec_sl(vsll, vull);
// CHECK: shl <2 x i64>
// CHECK-LE: shl <2 x i64>
// CHECK-PPC: error: call to 'vec_sl' is ambiguous

  res_vull = vec_sl(vull, vull);
// CHECK: shl <2 x i64>
// CHECK-LE: shl <2 x i64>
// CHECK-PPC: error: call to 'vec_sl' is ambiguous

  /* vec_sr */
  res_vsll = vec_sr(vsll, vull);
// CHECK: lshr <2 x i64>
// CHECK-LE: lshr <2 x i64>
// CHECK-PPC: error: call to 'vec_sr' is ambiguous

  res_vull = vec_sr(vull, vull);
// CHECK: lshr <2 x i64>
// CHECK-LE: lshr <2 x i64>
// CHECK-PPC: error: call to 'vec_sr' is ambiguous

  /* vec_sra */
  res_vsll = vec_sra(vsll, vull);
// CHECK: ashr <2 x i64>
// CHECK-LE: ashr <2 x i64>
// CHECK-PPC: error: call to 'vec_sra' is ambiguous

  res_vull = vec_sra(vull, vull);
// CHECK: ashr <2 x i64>
// CHECK-LE: ashr <2 x i64>
// CHECK-PPC: error: call to 'vec_sra' is ambiguous

  /* vec_unpackh */
  res_vsll = vec_unpackh(vsi);
// CHECK: llvm.ppc.altivec.vupkhsw
// CHECK-LE: llvm.ppc.altivec.vupklsw
// CHECK-PPC: error: call to 'vec_unpackh' is ambiguous

  res_vbll = vec_unpackh(vbi);
// CHECK: llvm.ppc.altivec.vupkhsw
// CHECK-LE: llvm.ppc.altivec.vupklsw
// CHECK-PPC: error: call to 'vec_unpackh' is ambiguous

  /* vec_unpackl */
  res_vsll = vec_unpackl(vsi);
// CHECK: llvm.ppc.altivec.vupklsw
// CHECK-LE: llvm.ppc.altivec.vupkhsw
// CHECK-PPC: error: call to 'vec_unpackl' is ambiguous

  res_vbll = vec_unpackl(vbi);
// CHECK: llvm.ppc.altivec.vupklsw
// CHECK-LE: llvm.ppc.altivec.vupkhsw
// CHECK-PPC: error: call to 'vec_unpackl' is ambiguous

  /* vec_vpksdss */
  res_vsi = vec_vpksdss(vsll, vsll);
// CHECK: llvm.ppc.altivec.vpksdss
// CHECK-LE: llvm.ppc.altivec.vpksdss
// CHECK-PPC: warning: implicit declaration of function 'vec_vpksdss'

  /* vec_vpksdus */
  res_vui = vec_vpksdus(vsll, vsll);
// CHECK: llvm.ppc.altivec.vpksdus
// CHECK-LE: llvm.ppc.altivec.vpksdus
// CHECK-PPC: warning: implicit declaration of function 'vec_vpksdus'

  /* vec_vpkudum */
  res_vsi = vec_vpkudum(vsll, vsll);
// CHECK: vperm
// CHECK-LE: vperm
// CHECK-PPC: warning: implicit declaration of function 'vec_vpkudum'

  res_vui = vec_vpkudum(vull, vull);
// CHECK: vperm
// CHECK-LE: vperm

  res_vui = vec_vpkudus(vull, vull);
// CHECK: llvm.ppc.altivec.vpkudus
// CHECK-LE: llvm.ppc.altivec.vpkudus
// CHECK-PPC: warning: implicit declaration of function 'vec_vpkudus'

  /* vec_vupkhsw */
  res_vsll = vec_vupkhsw(vsi);
// CHECK: llvm.ppc.altivec.vupkhsw
// CHECK-LE: llvm.ppc.altivec.vupklsw
// CHECK-PPC: warning: implicit declaration of function 'vec_vupkhsw'

  res_vbll = vec_vupkhsw(vbi);
// CHECK: llvm.ppc.altivec.vupkhsw
// CHECK-LE: llvm.ppc.altivec.vupklsw

  /* vec_vupklsw */
  res_vsll = vec_vupklsw(vsi);
// CHECK: llvm.ppc.altivec.vupklsw
// CHECK-LE: llvm.ppc.altivec.vupkhsw
// CHECK-PPC: warning: implicit declaration of function 'vec_vupklsw'

  res_vbll = vec_vupklsw(vbi);
// CHECK: llvm.ppc.altivec.vupklsw
// CHECK-LE: llvm.ppc.altivec.vupkhsw

  /* vec_max */
  res_vsll = vec_max(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd

  res_vsll = vec_max(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd

  res_vsll = vec_max(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vmaxsd
// CHECK-LE: @llvm.ppc.altivec.vmaxsd

  res_vull = vec_max(vull, vull);
// CHECK: @llvm.ppc.altivec.vmaxud
// CHECK-LE: @llvm.ppc.altivec.vmaxud

  res_vull = vec_max(vbll, vull);
// CHECK: @llvm.ppc.altivec.vmaxud
// CHECK-LE: @llvm.ppc.altivec.vmaxud

  /* vec_min */
  res_vsll = vec_min(vsll, vsll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd

  res_vsll = vec_min(vbll, vsll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd

  res_vsll = vec_min(vsll, vbll);
// CHECK: @llvm.ppc.altivec.vminsd
// CHECK-LE: @llvm.ppc.altivec.vminsd

  res_vull = vec_min(vull, vull);
// CHECK: @llvm.ppc.altivec.vminud
// CHECK-LE: @llvm.ppc.altivec.vminud

  res_vull = vec_min(vbll, vull);
// CHECK: @llvm.ppc.altivec.vminud
// CHECK-LE: @llvm.ppc.altivec.vminud

  /* vec_vbpermq */
  res_vsll = vec_vbpermq(vsc, vsc);
// CHECK: llvm.ppc.altivec.vbpermq
// CHECK-LE: llvm.ppc.altivec.vbpermq

  res_vull = vec_vbpermq(vuc, vuc);
// CHECK: llvm.ppc.altivec.vbpermq
// CHECK-LE: llvm.ppc.altivec.vbpermq
// CHECK-PPC: warning: implicit declaration of function 'vec_vbpermq'

  /* vec_vgbbd */
  res_vsc = vec_vgbbd(vsc);
// CHECK: llvm.ppc.altivec.vgbbd
// CHECK-LE: llvm.ppc.altivec.vgbbd

  res_vuc = vec_vgbbd(vuc);
// CHECK: llvm.ppc.altivec.vgbbd
// CHECK-LE: llvm.ppc.altivec.vgbbd
// CHECK-PPC: warning: implicit declaration of function 'vec_vgbbd'
}
