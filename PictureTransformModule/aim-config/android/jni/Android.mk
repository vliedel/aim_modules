# File: Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Name of the lib
LOCAL_MODULE    := PictureTransformModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc $(LOCAL_PATH)/../../../aim-core/inc
LOCAL_SRC_FILES := PictureTransformModule_wrap.cpp ../../../src/PictureTransformModuleExt.cpp ../../../aim-core/src/PictureTransformModule.cpp
LOCAL_CFLAGS    := -frtti

include $(BUILD_SHARED_LIBRARY)
