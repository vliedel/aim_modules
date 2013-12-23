/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.dobots.brightnesstomorsemodule.aim;

public class BrightnessToMorseModule {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected BrightnessToMorseModule(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(BrightnessToMorseModule obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        AIMJNI.delete_BrightnessToMorseModule(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void Init(SWIGTYPE_p_std__string name) {
    AIMJNI.BrightnessToMorseModule_Init(swigCPtr, this, SWIGTYPE_p_std__string.getCPtr(name));
  }

  public Param GetParam() {
    long cPtr = AIMJNI.BrightnessToMorseModule_GetParam(swigCPtr, this);
    return (cPtr == 0) ? null : new Param(cPtr, false);
  }

  public void Tick() {
    AIMJNI.BrightnessToMorseModule_Tick(swigCPtr, this);
  }

  public boolean Stop() {
    return AIMJNI.BrightnessToMorseModule_Stop(swigCPtr, this);
  }

  public SWIGTYPE_p_float readBrightness(boolean blocking) {
    long cPtr = AIMJNI.BrightnessToMorseModule_readBrightness__SWIG_0(swigCPtr, this, blocking);
    return (cPtr == 0) ? null : new SWIGTYPE_p_float(cPtr, false);
  }

  public SWIGTYPE_p_float readBrightness() {
    long cPtr = AIMJNI.BrightnessToMorseModule_readBrightness__SWIG_1(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_float(cPtr, false);
  }

  public void androidWriteBrightness(float input) {
    AIMJNI.BrightnessToMorseModule_androidWriteBrightness(swigCPtr, this, input);
  }

  public boolean writeMorse(String output) {
    return AIMJNI.BrightnessToMorseModule_writeMorse(swigCPtr, this, output);
  }

  public AndroidMorseRead_t androidReadMorse() {
    return new AndroidMorseRead_t(AIMJNI.BrightnessToMorseModule_androidReadMorse(swigCPtr, this), true);
  }

}
