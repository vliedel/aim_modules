# File: Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Name of the lib
LOCAL_MODULE    := ImageDisplayModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc $(LOCAL_PATH)/../../../aim-core/inc
LOCAL_SRC_FILES := ImageDisplayModule_wrap.cpp ../../../src/ImageDisplayModuleExt.cpp ../../../aim-core/src/ImageDisplayModule.cpp
LOCAL_CFLAGS    := -frtti

include $(BUILD_SHARED_LIBRARY)
