# VCV-Rack-Csound-Modules
VCV Rack modules with Csound engine

Csound is a user-programmable and user-extensible sound processing language and software synthesizer. (www.csound.com)
Csound is copyright (c) 1991 Barry Vercoe, John ffitch, and other contributors.

VCVRack v0.5.1, Csound v6.10.0 and Csound modules are compiled and tested on linux

*** NOT TESTED ON OTHER PLATFORM ***

Thanks to:
 - Tutorial plugin 
 - Fundamentals plugins by Andrew Belt http://www.vcvrack.com
 - ML Modules plugins by Martin Lueders https://github.com/martin-lueders/ML_modules (ML knob...)
 - Csound API at http://csound.com/docs/api/index.html
 - Csound realtime examples by Iain McCurdy at http://iainmccurdy.org/csound.html
 etc...

Important:

1) Install the sound and music system Csound, You will found it at www.csound.com.

   It should be installed in usr/local/ (it is the default in "make install" from sources).

2) Clone from git the vcv-Rack-Csound-Modules 

   The cloned module directory needs to be named Csound, i.e.
   
   'git clone https://github.com/Djack13/VCV-Rack-Csound-Modules.git Csound'.

3) Copie the Csound module directory in /Rack/plugins/

4) Then cd to /Rack/plugins/Csound and compile with make



* The module CsoundReverb use the Csound script CsoundReverb.csd

