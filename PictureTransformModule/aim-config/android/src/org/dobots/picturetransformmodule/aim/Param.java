/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.picturetransformmodule.aim;

public class Param {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Param(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Param obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        AIMJNI.delete_Param(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setModule_id(String value) {
    AIMJNI.Param_module_id_set(swigCPtr, this, value);
  }

  public String getModule_id() {
    return AIMJNI.Param_module_id_get(swigCPtr, this);
  }

  public Param() {
    this(AIMJNI.new_Param(), true);
  }

}
