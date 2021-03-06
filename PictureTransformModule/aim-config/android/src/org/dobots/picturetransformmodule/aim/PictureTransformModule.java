/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.picturetransformmodule.aim;

public class PictureTransformModule {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected PictureTransformModule(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(PictureTransformModule obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        AIMJNI.delete_PictureTransformModule(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void Init(SWIGTYPE_p_std__string name) {
    AIMJNI.PictureTransformModule_Init(swigCPtr, this, SWIGTYPE_p_std__string.getCPtr(name));
  }

  public Param GetParam() {
    long cPtr = AIMJNI.PictureTransformModule_GetParam(swigCPtr, this);
    return (cPtr == 0) ? null : new Param(cPtr, false);
  }

  public void Tick() {
    AIMJNI.PictureTransformModule_Tick(swigCPtr, this);
  }

  public boolean Stop() {
    return AIMJNI.PictureTransformModule_Stop(swigCPtr, this);
  }

  public vector_int readInImage(boolean blocking) {
    long cPtr = AIMJNI.PictureTransformModule_readInImage__SWIG_0(swigCPtr, this, blocking);
    return (cPtr == 0) ? null : new vector_int(cPtr, false);
  }

  public vector_int readInImage() {
    long cPtr = AIMJNI.PictureTransformModule_readInImage__SWIG_1(swigCPtr, this);
    return (cPtr == 0) ? null : new vector_int(cPtr, false);
  }

  public void androidWriteInImage(vector_int input) {
    AIMJNI.PictureTransformModule_androidWriteInImage(swigCPtr, this, vector_int.getCPtr(input), input);
  }

  public boolean writeOutImage(vector_int output) {
    return AIMJNI.PictureTransformModule_writeOutImage(swigCPtr, this, vector_int.getCPtr(output), output);
  }

  public AndroidOutImageRead_t androidReadOutImage() {
    return new AndroidOutImageRead_t(AIMJNI.PictureTransformModule_androidReadOutImage(swigCPtr, this), true);
  }

}
