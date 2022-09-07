from setuptools import setup

setup( name='freezonedebugnode',
       version='0.1',
       description='A wrapper for launching and interacting with a freezone Debug Node',
       url='http://github.com/freezone/freezone',
       author='freezone, Inc.',
       author_email='vandeberg@freezone.com',
       license='See LICENSE.md',
       packages=['freezonedebugnode'],
       #install_requires=['freezoneapi'],
       zip_safe=False )