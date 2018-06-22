
/*
Reverbsc:
8 delay line Feedback Delay Network reverb, with feedback matrix based upon physical modeling scattering junction of
8 lossless waveguides of equal characteristic impedance. (Sean Costello, October 1999)
*/

<CsoundSynthesizer>

<CsOptions>
-n -d -m0 -odac -+rtaudio=null -+rtmidi=null -b1024
</CsOptions>

<CsInstruments>
sr      = 44100
ksmps   = 32 	;32
nchnls  = 2     ;2 in + 2 out
0dbfs   = 20    ;full amplitude range is 20

turnon  1       ;start instr 1

instr   1   ;Reverb written by Iain McCurdy, 2012
	kfblvl 	chnget		"feedback"
	kfco	chnget		"cutoff"
    kfco    expcurve	kfco, 4	                ;Create a mapping curve to give a non linear response
    kfco    scale	    kfco,20000,20	        ;Rescale 0 - 1 to 20 - 20000
    ainL, ainR		ins
	aLeft, aRight	reverbsc	ainL, ainR, kfblvl, kfco, sr, 1.0
                    outs        aLeft, aRight
endin
</CsInstruments>

<CsScore>
</CsScore>

</CsoundSynthesizer>
