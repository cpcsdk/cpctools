Under Windows, it is not necessary to rebuild AntTweakBar since a precompiled
version is provided in the /lib directory.

Under GNU/Linux, a precompiled version of the library (libAntTweakBar.so) is
provided in the /lib directory. But depending on your system, you may need 
to rebuild the library. To do so, type make in this /src directory.

To recompile AntTweakBar you also need the headers of the following external
libraries (note that you only need the headers, since AntTweakBar does not
link with them):
- GLFW : http://glfw.sourceforge.net
- GLUT : http://opengl.org/resources/libraries/glut
- SDL  : http://www.libsdl.org
(and the DirectX SDK if you want to recompile the Windows version)
