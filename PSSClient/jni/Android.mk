# A simple test for the minimal standard C++ library
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -std=c++11
# LOCAL_CPPFLAGS += -D__cplusplus=201103L
LOCAL_MODULE := test-libstl
LOCAL_SRC_FILES := main.cpp
include $(BUILD_EXECUTABLE)
