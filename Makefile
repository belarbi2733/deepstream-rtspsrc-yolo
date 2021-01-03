################################################################################
# Copyright (c) 2019-2020, NVIDIA CORPORATION. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
################################################################################

CUDA_VER?=
ifeq ($(CUDA_VER),)
  $(error "CUDA_VER is not set")
endif

APP:= deepstream-rtspsrc-yolo-app
CC = g++
TARGET_DEVICE = $(shell gcc -dumpmachine | cut -f1 -d -)

NVDS_VERSION:=5.0

LIB_INSTALL_DIR?=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/lib/
APP_INSTALL_DIR?=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/bin/

SRCS:= $(wildcard src/*.cpp)
INCS:= $(wildcard *.h)
INCS:= $(wildcard common/includes/*.h)

ifeq ($(TARGET_DEVICE),aarch64)
	PKGS:= gstreamer-1.0 opencv4 gstreamer-video-1.0 x11 gstreamer-rtsp-1.0
else
	PKGS:= gstreamer-1.0 opencv gstreamer-video-1.0 x11 gstreamer-rtsp-1.0
endif

CFLAGS:= -fPIC -std=c++11    \
         -I /opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/sources/includes \
         -I /usr/local/cuda-$(CUDA_VER)/include

CFLAGS+= `pkg-config --cflags $(PKGS)`
LIBS:= `pkg-config --libs $(PKGS)`

LIBS+= -L$(LIB_INSTALL_DIR) -lnvdsgst_meta -lnvds_meta -lnvds_inferutils -lnvds_utils -lnvdsgst_helper -lm \
       -lgstrtspserver-1.0 -ldl -L/usr/local/cuda-$(CUDA_VER)/lib64/ -lcudart     \
       -Wl,-rpath,$(LIB_INSTALL_DIR)

ifeq ($(TARGET_DEVICE),aarch64)
  CFLAGS+= -DPLATFORM_TEGRA
  SRCS+=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/sources/libs/nvdsinfer_customparser/nvdsinfer_custombboxparser.cpp
else
  LIBS+= -L$(LIB_INSTALL_DIR) -lnvds_infercustomparser
endif

OBJS:= $(SRCS:.cpp=.o)

all: $(APP)

%.o: %.cpp $(INCS) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

$(APP): $(OBJS) Makefile
	$(CC) -o $(APP) $(OBJS) $(LIBS)

install: $(APP)
	cp -rv $(APP) $(APP_INSTALL_DIR)

clean:
	rm -rf $(OBJS) $(APP)
