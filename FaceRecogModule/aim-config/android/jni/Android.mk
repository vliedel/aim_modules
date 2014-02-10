# File: Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Name of the lib
LOCAL_MODULE    := FaceRecogModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc $(LOCAL_PATH)/../../../aim-core/inc
LOCAL_SRC_FILES := FaceRecogModule_wrap.cpp ../../../src/FaceRecogModuleExt.cpp ../../../aim-core/src/FaceRecogModule.cpp
LOCAL_CFLAGS    := -frtti

include $(BUILD_SHARED_LIBRARY)
