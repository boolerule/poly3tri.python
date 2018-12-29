import os
from setuptools import setup
from distutils.extension import Extension
import Cython
import Cython.Build

import sys

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

from Cython.Distutils import build_ext as build_ext_c

CYTHON_SOURCES =  """src/p3t.pyx""".split("\n")

CPP_SOURCES = """poly2tri/common/shapes3.cc
poly2tri/sweep/advancing_front.cc
poly2tri/sweep/cdt.cc
poly2tri/sweep/sweep.cc
poly2tri/sweep/sweep_context.cc""".split("\n")

ext = Extension(
    'p3t',
    sources= CYTHON_SOURCES + CPP_SOURCES,
    language='c++'
)
extensions = Cython.Build.cythonize(ext)

setup(
    name = "poly3tri",
    version = "0.0.1",
    author = "twosilly",
    description = "A 2D 3D constrained Delaunay triangulation library",
    long_description = read('README'),
    url = "http://code.google.com/p/poly2tri/",

    ext_modules = extensions,
)
