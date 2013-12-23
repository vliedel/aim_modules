/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.avgbrightnessmodule.aim;

public class vector_float {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected vector_float(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(vector_float obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        AIMJNI.delete_vector_float(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public vector_float() {
    this(AIMJNI.new_vector_float__SWIG_0(), true);
  }

  public vector_float(long n) {
    this(AIMJNI.new_vector_float__SWIG_1(n), true);
  }

  public long size() {
    return AIMJNI.vector_float_size(swigCPtr, this);
  }

  public long capacity() {
    return AIMJNI.vector_float_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    AIMJNI.vector_float_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return AIMJNI.vector_float_isEmpty(swigCPtr, this);
  }

  public void clear() {
    AIMJNI.vector_float_clear(swigCPtr, this);
  }

  public void add(float x) {
    AIMJNI.vector_float_add(swigCPtr, this, x);
  }

  public float get(int i) {
    return AIMJNI.vector_float_get(swigCPtr, this, i);
  }

  public void set(int i, float val) {
    AIMJNI.vector_float_set(swigCPtr, this, i, val);
  }

}