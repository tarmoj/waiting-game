<CsoundSynthesizer>
<CsOptions>
-odac:system:playback_ -+rtaudio=jack -d
</CsOptions>
<CsInstruments>

sr = 44100 
ksmps = 8
nchnls = 2
0dbfs = 1

chnset 0.6, "volume"

ga1 init 0
ga2 init 0

alwayson "reverb_"
 scoreline_i  {{ i "play" 0 0.5 "sounds/stones/stone01.wav" 0.5 }}
instr play
	
	
	Sfile strget p4
	p3 filelen Sfile
	ipan = p5
	asig diskin2 Sfile, random:i(0.95,1.05)
	asig *= linen:a(1,0.02,p3,0.05)
	kvolume chnget "volume"
	kvolume port kvolume, 0.02, chnget:i("volume") 
	aL, aR pan2 asig*kvolume , ipan
	;outs aL, aR
	ga1 += aL
	ga2 += aR
	

endin

instr reverb_
	irvbtime = 4.5
	irvbamount = 0.01
	ihdif = 0.6
	idrywet = 0.5
	arvbL reverb2 ga1*irvbamount, irvbtime, ihdif
	arvbR reverb2 ga2*irvbamount, irvbtime, ihdif
	
	outs ntrpol(arvbL,ga1,idrywet), ntrpol(arvbR,ga2,idrywet)
	ga1=0
	ga2=0
endin
</CsInstruments>
<CsScore>

</CsScore>
</CsoundSynthesizer>
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>100</x>
 <y>100</y>
 <width>320</width>
 <height>240</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
