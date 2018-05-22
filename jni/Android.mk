LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDFLAGS := 

LOCAL_CFLAGS := 

LOCAL_CFLAGS  += -Werror

LOCAL_CFLAGS  += -D_ANDROID_

LOCAL_SRC_FILES:= OpenGL_Hello_World.c

LOCAL_C_INCLUDES:=$(LOCAL_PATH)
LOCAL_C_INCLUDES+= $(LOCAL_PATH)/includes/
LOCAL_C_INCLUDES+= $(LOCAL_PATH)/../../includes/
LOCAL_C_INCLUDES+= $(LOCAL_PATH)/../../export_includes/

LOCAL_SHARED_LIBRARIES:= 

LOCAL_32_BIT_ONLY := true

LOCAL_MODULE:= OpenGL_Hello_World
LOCAL_CLANG := false
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS := -llog -lGLESv2 -lEGL


include $(BUILD_EXECUTABLE)
