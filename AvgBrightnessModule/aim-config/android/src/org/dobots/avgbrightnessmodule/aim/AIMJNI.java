/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.avgbrightnessmodule.aim;

public class AIMJNI {
  public final static native long new_vector_int__SWIG_0();
  public final static native long new_vector_int__SWIG_1(long jarg1);
  public final static native long vector_int_size(long jarg1, vector_int jarg1_);
  public final static native long vector_int_capacity(long jarg1, vector_int jarg1_);
  public final static native void vector_int_reserve(long jarg1, vector_int jarg1_, long jarg2);
  public final static native boolean vector_int_isEmpty(long jarg1, vector_int jarg1_);
  public final static native void vector_int_clear(long jarg1, vector_int jarg1_);
  public final static native void vector_int_add(long jarg1, vector_int jarg1_, int jarg2);
  public final static native int vector_int_get(long jarg1, vector_int jarg1_, int jarg2);
  public final static native void vector_int_set(long jarg1, vector_int jarg1_, int jarg2, int jarg3);
  public final static native void delete_vector_int(long jarg1);
  public final static native long new_vector_long__SWIG_0();
  public final static native long new_vector_long__SWIG_1(long jarg1);
  public final static native long vector_long_size(long jarg1, vector_long jarg1_);
  public final static native long vector_long_capacity(long jarg1, vector_long jarg1_);
  public final static native void vector_long_reserve(long jarg1, vector_long jarg1_, long jarg2);
  public final static native boolean vector_long_isEmpty(long jarg1, vector_long jarg1_);
  public final static native void vector_long_clear(long jarg1, vector_long jarg1_);
  public final static native void vector_long_add(long jarg1, vector_long jarg1_, int jarg2);
  public final static native int vector_long_get(long jarg1, vector_long jarg1_, int jarg2);
  public final static native void vector_long_set(long jarg1, vector_long jarg1_, int jarg2, int jarg3);
  public final static native void delete_vector_long(long jarg1);
  public final static native long new_vector_float__SWIG_0();
  public final static native long new_vector_float__SWIG_1(long jarg1);
  public final static native long vector_float_size(long jarg1, vector_float jarg1_);
  public final static native long vector_float_capacity(long jarg1, vector_float jarg1_);
  public final static native void vector_float_reserve(long jarg1, vector_float jarg1_, long jarg2);
  public final static native boolean vector_float_isEmpty(long jarg1, vector_float jarg1_);
  public final static native void vector_float_clear(long jarg1, vector_float jarg1_);
  public final static native void vector_float_add(long jarg1, vector_float jarg1_, float jarg2);
  public final static native float vector_float_get(long jarg1, vector_float jarg1_, int jarg2);
  public final static native void vector_float_set(long jarg1, vector_float jarg1_, int jarg2, float jarg3);
  public final static native void delete_vector_float(long jarg1);
  public final static native long new_vector_double__SWIG_0();
  public final static native long new_vector_double__SWIG_1(long jarg1);
  public final static native long vector_double_size(long jarg1, vector_double jarg1_);
  public final static native long vector_double_capacity(long jarg1, vector_double jarg1_);
  public final static native void vector_double_reserve(long jarg1, vector_double jarg1_, long jarg2);
  public final static native boolean vector_double_isEmpty(long jarg1, vector_double jarg1_);
  public final static native void vector_double_clear(long jarg1, vector_double jarg1_);
  public final static native void vector_double_add(long jarg1, vector_double jarg1_, double jarg2);
  public final static native double vector_double_get(long jarg1, vector_double jarg1_, int jarg2);
  public final static native void vector_double_set(long jarg1, vector_double jarg1_, int jarg2, double jarg3);
  public final static native void delete_vector_double(long jarg1);
  public final static native void Param_module_id_set(long jarg1, Param jarg1_, String jarg2);
  public final static native String Param_module_id_get(long jarg1, Param jarg1_);
  public final static native long new_Param();
  public final static native void delete_Param(long jarg1);
  public final static native void AndroidBrightnessRead_t_success_set(long jarg1, AndroidBrightnessRead_t jarg1_, boolean jarg2);
  public final static native boolean AndroidBrightnessRead_t_success_get(long jarg1, AndroidBrightnessRead_t jarg1_);
  public final static native void AndroidBrightnessRead_t_val_set(long jarg1, AndroidBrightnessRead_t jarg1_, float jarg2);
  public final static native float AndroidBrightnessRead_t_val_get(long jarg1, AndroidBrightnessRead_t jarg1_);
  public final static native long new_AndroidBrightnessRead_t();
  public final static native void delete_AndroidBrightnessRead_t(long jarg1);
  public final static native void AndroidCommandOutRead_t_success_set(long jarg1, AndroidCommandOutRead_t jarg1_, boolean jarg2);
  public final static native boolean AndroidCommandOutRead_t_success_get(long jarg1, AndroidCommandOutRead_t jarg1_);
  public final static native void AndroidCommandOutRead_t_val_set(long jarg1, AndroidCommandOutRead_t jarg1_, String jarg2);
  public final static native String AndroidCommandOutRead_t_val_get(long jarg1, AndroidCommandOutRead_t jarg1_);
  public final static native long new_AndroidCommandOutRead_t();
  public final static native void delete_AndroidCommandOutRead_t(long jarg1);
  public final static native void delete_AvgBrightnessModule(long jarg1);
  public final static native void AvgBrightnessModule_Init(long jarg1, AvgBrightnessModule jarg1_, long jarg2);
  public final static native long AvgBrightnessModule_GetParam(long jarg1, AvgBrightnessModule jarg1_);
  public final static native void AvgBrightnessModule_Tick(long jarg1, AvgBrightnessModule jarg1_);
  public final static native boolean AvgBrightnessModule_Stop(long jarg1, AvgBrightnessModule jarg1_);
  public final static native long AvgBrightnessModule_readImage__SWIG_0(long jarg1, AvgBrightnessModule jarg1_, boolean jarg2);
  public final static native long AvgBrightnessModule_readImage__SWIG_1(long jarg1, AvgBrightnessModule jarg1_);
  public final static native void AvgBrightnessModule_androidWriteImage(long jarg1, AvgBrightnessModule jarg1_, String jarg2);
  public final static native boolean AvgBrightnessModule_writeBrightness(long jarg1, AvgBrightnessModule jarg1_, float jarg2);
  public final static native long AvgBrightnessModule_androidReadBrightness(long jarg1, AvgBrightnessModule jarg1_);
  public final static native boolean AvgBrightnessModule_writeCommandOut(long jarg1, AvgBrightnessModule jarg1_, String jarg2);
  public final static native long AvgBrightnessModule_androidReadCommandOut(long jarg1, AvgBrightnessModule jarg1_);
  public final static native long new_AvgBrightnessModuleExt();
  public final static native void delete_AvgBrightnessModuleExt(long jarg1);
  public final static native void AvgBrightnessModuleExt_Tick(long jarg1, AvgBrightnessModuleExt jarg1_);
  public final static native boolean AvgBrightnessModuleExt_Stop(long jarg1, AvgBrightnessModuleExt jarg1_);
  public final static native long AvgBrightnessModuleExt_SWIGUpcast(long jarg1);
}
