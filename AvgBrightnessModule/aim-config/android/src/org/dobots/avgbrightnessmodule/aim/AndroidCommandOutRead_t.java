/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.avgbrightnessmodule.aim;

public class AndroidCommandOutRead_t {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected AndroidCommandOutRead_t(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AndroidCommandOutRead_t obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        AIMJNI.delete_AndroidCommandOutRead_t(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setSuccess(boolean value) {
    AIMJNI.AndroidCommandOutRead_t_success_set(swigCPtr, this, value);
  }

  public boolean getSuccess() {
    return AIMJNI.AndroidCommandOutRead_t_success_get(swigCPtr, this);
  }

  public void setVal(String value) {
    AIMJNI.AndroidCommandOutRead_t_val_set(swigCPtr, this, value);
  }

  public String getVal() {
    return AIMJNI.AndroidCommandOutRead_t_val_get(swigCPtr, this);
  }

  public AndroidCommandOutRead_t() {
    this(AIMJNI.new_AndroidCommandOutRead_t(), true);
  }

}
