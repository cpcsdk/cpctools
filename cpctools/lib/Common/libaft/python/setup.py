from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
    Extension("cpctools.libaft",
              ["cpctools/libaft.pyx"],
              language="c++",
              include_dir=["../../libcpcfs"],
              libraries=["cpcfs","tools","dsk"]), # Unix-like specific,
    Extension("cpctools.cpcbooster",
              ["cpctools/cpcbooster.pyx"],
              ) # Unix-like specific

]

setup(
  name = "CPCTools",
  packages = ['cpctools'],
  cmdclass = {"build_ext": build_ext},
  ext_modules = ext_modules
)

