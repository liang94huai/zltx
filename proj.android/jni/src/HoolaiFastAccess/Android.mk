LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := HoolaiGameProxy
LOCAL_SRC_FILES :=  \
FastSdk.cpp \
GameProxy.cpp \

LOCAL_C_INCLUDES        := $(LOCAL_PATH)/..

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)