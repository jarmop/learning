#!/bin/bash

source ../build_base.sh

SOURCES="terrain_demo1.cpp terrain.cpp triangle_list.cpp terrain_technique.cpp $ROOTDIR/common/ogldev_util.cpp $ROOTDIR/common/math_3d.cpp $ROOTDIR/common/ogldev_basic_glfw_camera.cpp $ROOTDIR/common/ogldev_glfw.cpp $ROOTDIR/common/technique.cpp"

$CC $SOURCES $OGL_CPPFLAGS $OGL_LDFLAGS -o terrain_demo1
# $CC $SOURCES $OGL_LDFLAGS -o terrain_demo1
