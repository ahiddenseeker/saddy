g++ tools/hbuild.cpp -o bin/hbuild.exe
bin\hbuild.exe  preprocess\Makefile_library_debug_mingw.hbd -o makefiles\Makefile_Library_Debug.mingw
bin\hbuild.exe  preprocess\Makefile_library_release_mingw.hbd -o makefiles\Makefile_Library_Release.mingw
bin\hbuild.exe  preprocess\Makefile_library_debug_linux.hbd -o makefiles\Makefile_Library_Debug.linux
bin\hbuild.exe  preprocess\Makefile_library_release_linux.hbd -o makefiles\Makefile_Library_Release.linux