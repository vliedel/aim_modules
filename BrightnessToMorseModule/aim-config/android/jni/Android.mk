# File: Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Name of the lib
LOCAL_MODULE    := BrightnessToMorseModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc $(LOCAL_PATH)/../../../aim-core/inc
LOCAL_SRC_FILES := BrightnessToMorseModule_wrap.cpp ../../../src/BrightnessToMorseModuleExt.cpp ../../../aim-core/src/BrightnessToMorseModule.cpp
LOCAL_CFLAGS    := -frtti

include $(BUILD_SHARED_LIBRARY)
