if [ -v $ROOTDIR ]; then
    ROOTDIR=".."
fi

echo "wtf"

CC=g++
# OGL_CPPFLAGS="$CPPFLAGS -I$ROOTDIR/Include -I$ROOTDIR/Common/FreetypeGL -I$ROOTDIR/Common/3rdparty/ImGui/GLFW/ -std=c++20"
OGL_CPPFLAGS="$CPPFLAGS -I$ROOTDIR/include -std=c++20"
OGL_LDFLAGS=`pkg-config --libs glew assimp`
OGL_LDFLAGS="$OGL_LDFLAGS -lglfw -lX11 -lmeshoptimizer -lglut"
# OGL_LDFLAGS="$OGL_LDFLAGS -lglfw -lEGL -lmeshoptimizer -lglut"

build_ogldev() {
	cd $ROOTDIR/Common
	./build.sh
	cd -
}

build_demolition() {
	cd $ROOTDIR/Common
	./build.sh
	cd -
}


