#!/usr/bin/python3
#-*- coding: utf-8 -*-

from distutils.core import setup, Extension

mdl = Extension('Buffer', sources = ['buffer.cpp'])

setup(name = 'Buffer',
        version = '1.0',
        description = 'Python C API Simplest Module',
        ext_modules = [mdl])
