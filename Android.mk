# Marco Zavatta
# TELECOM Bretagne
# Android.mk build file for libzertp

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libzertp
LOCAL_SRC_FILES := rtp_net.c rtppacket.c rtcppacket.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../ZeSenseServer
LOCAL_LDLIBS  := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_CFLAGS :=  -Wall -Wextra -std=c99 -pedantic -g -O2
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
