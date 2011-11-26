from setuptools import setup, find_packages, Extension

setup(name='pyprime',
        version='0.1',
        packages=find_packages(),
        ext_modules=[
            Extension('prime', ['prime.c']),
            ],
        )
