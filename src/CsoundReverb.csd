
/*
Reverbsc:
8 delay line Feedback Delay Network reverb, with feedback matrix based upon physical modeling scattering junction of 8 lossless waveguides of equal characteristic impedance. (Sean Costello, October 1999)
*/
<CsoundSynthesizer>
<CsOptions>
-n -dm0 -odac -+rtaudio=null -+rtmidi=null -b1024 -B16384
</CsOptions>
<CsInstruments>
sr      = 44100
ksmps   = 16
nchnls  = 2     ;2 in + 2 out
0dbfs   = 20    ;full amplitude range is 20

turnon  1       ;start instr 1

instr   1   ;Reverb
    kfblvl  chnget  "feedback"
    kfco    chnget  "cutoff"

    ainL, ainR  ins

	;kfblvl	= 0.87
	;kfco	= 5000
	israte	= sr
	ipitchm	= 0.99

	aLeft, aRight	reverbsc	ainL, ainR, kfblvl, kfco, israte, ipitchm
                    outs        aLeft, aRight
endin
</CsInstruments>
<CsScore>
</CsScore>
</CsoundSynthesizer>
