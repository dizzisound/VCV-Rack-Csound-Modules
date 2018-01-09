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
 - etc...

Important:

Csound should be installed in usr/local/ (it is the default in "make install" from sources)

The cloned directory needs to be named Csound, i.e. 'git clone https://github.com/Djack13/VCV-Rack-Csound-Modules.git Csound'.

This Csound directory is to be copied in /Rack/plugins/

Then cd to /Rack/plugins/Csound and compile with make



* The module CsoundReverb use the Csound script CsoundReverb.csd

