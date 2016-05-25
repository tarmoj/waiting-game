<CsoundSynthesizer>
<CsOptions>
-odac:system:playback_ -+rtaudio=jack -d
</CsOptions>
<CsInstruments>

sr = 44100 
ksmps = 8
nchnls = 2
0dbfs = 1

chnset 0.9, "volume"

ga1 init 0
ga2 init 0

alwayson "reverb_"
; scoreline_i  {{ i "play" 0 0.5 "sounds/stones/stone01.wav" 0.5 }}
instr play
	
	
	Sfile strget p4
	p3 filelen Sfile
	ipan = p5
	asig diskin2 Sfile, random:i(0.95,1.05)
	asig *= linen:a(1,0.02,p3,0.05) 
	aL, aR pan2 asig , ipan
	;outs aL, aR
	ga1 += aL
	ga2 += aR
	

endin

; schedule "flute",0,5
;giFluteFreqs ftgen 0,0,16,-2,1,2,3,4,5,6,7,8
;giFluteAmps ftgen 0,0,16,-2, 1,0.5,0.3,0.2,0.1,0.05,0.03,0.02
instr flute
	ipan = p4
	iamp = 0.2 + birnd:i(0.1)

	kline line 1,p3,0
	
	iattack random 0.05,0.1
	idecay = iattack * random:i(0.7,1.5)
	isustain random 0.5,0.7 ; for flute sound
	inoisesustain random 0.01, 0.1	
	irelease = p3/2	
	anoiseenv adsr iattack,idecay*2, inoisesustain, irelease
	; it is a big mess with envelopes, amplitudes etc...
	;anoiseenv adsr 0.05,0.1, inoisesustain, irelease
	;asoundenv linen iamp, (iattack + idecay)*3, p3, irelease
	asoundenv adsr random:i(0.1,0.5), random:i(0.1,0.5), 0.6, irelease
	asoundenv *=iamp
	;anoise2env adsr iattack, idecay,iamp*0.8,idecay;
	anoise2env = asoundenv*8
	;asoundenv = anoise2env 
	anoiseenv linen iamp, (iattack + idecay)/2, p3, irelease
 
	
	ioct = int(random:i(1,2.99))
	ideviation = cent(random:i(-20,20)) ; allow higher since in a=440
	ifreq = (cpspch(5.05)+1)*ioct*ideviation  ; fis, with some deviation; +2 to bring up a bit
	print ioct, ideviation, ipan, ifreq
	
	; flute tone
	asig poscil 1, ifreq+rnd(0.005);jspline:k(0.1,0.2,2) ; some variation to sound
	kexpdecay expon 1,p3,0.0001
	kh1 = 0.7+rnd:i(0.2) ;
	kh2 = 0.6+rnd:i(0.3) ;0.5 + jspline:k(0.2,0.5,2) ; add some variation to specter
	;printk2 kh2  
	kh3 =(0.1+rnd:i(0.1))*kexpdecay ;bring down upper harmonics
	kh4 =rnd:i(0.1)*kexpdecay 
	kh5 =rnd:i(0.08)*kexpdecay
	kh6 =rnd:i(0.04)*kexpdecay

		
	asig chebyshevpoly asig, 0, kh1, kh2, kh3, kh4, kh5,kh6
	
	; noise
	
	anoise1 = pinkish(anoiseenv*0.01) ;random:i(0.01,0.03)
	anoise1 butterhp anoise1, 200
	;anoise butterlp anoise, linseg:k(10000,(iattack + idecay),ifreq*2)
		
	anoise2 pinkish anoise2env *random:i(0.4,0.8)
	anoise3 butterbp anoise2, ifreq, ifreq/4 ; noise around th pitch
	anoise4  butterbp anoise2, ifreq*4, ifreq ; noise above th pitch
	anoise = (anoise3+anoise4)*0.5 ; + anoise1
	
	aout = (anoise + asig*asoundenv ) *iamp
	;aout = anoise *iamp
	;dispfft aout*10, .1, 1024
	aL, aR pan2 aout,ipan
	;outs aL, aR
	ga1 += aL
	ga2 += aR
endin

instr reverb_
	irvbtime = 4.5
	irvbamount = 0.01
	ihdif = 0.6
	idrywet = 0.5
	iroomsize = 0.4
	kvolume chnget "volume"
	kvolume port kvolume, 0.02, chnget:i("volume")
	arvbL, arvbR freeverb ga1*irvbamount, ga2*irvbamount,iroomsize, ihdif 
	;arvbL reverb2 ga1*irvbamount, irvbtime, ihdif
	;arvbR reverb2 ga2*irvbamount, irvbtime, ihdif
	
	outs ntrpol(arvbL,ga1,idrywet)*kvolume, ntrpol(arvbR,ga2,idrywet)*kvolume
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
 <x>0</x>
 <y>0</y>
 <width>445</width>
 <height>660</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
 <bsbObject type="BSBButton" version="2">
  <objectName>flute</objectName>
  <x>41</x>
  <y>34</y>
  <width>100</width>
  <height>30</height>
  <uuid>{8f5fb1c9-3ecf-4a97-abcf-bb71ee2b02d8}</uuid>
  <visible>true</visible>
  <midichan>0</midichan>
  <midicc>0</midicc>
  <type>event</type>
  <pressedValue>1.00000000</pressedValue>
  <stringvalue/>
  <text>flute</text>
  <image>/</image>
  <eventLine>i "flute" 0  5 0.5</eventLine>
  <latch>false</latch>
  <latched>true</latched>
 </bsbObject>
 <bsbObject type="BSBScope" version="2">
  <objectName/>
  <x>66</x>
  <y>221</y>
  <width>350</width>
  <height>150</height>
  <uuid>{9c529ccd-dafa-4c68-8c0f-918727580334}</uuid>
  <visible>true</visible>
  <midichan>0</midichan>
  <midicc>-3</midicc>
  <value>-255.00000000</value>
  <type>scope</type>
  <zoomx>2.00000000</zoomx>
  <zoomy>2.00000000</zoomy>
  <dispx>1.00000000</dispx>
  <dispy>1.00000000</dispy>
  <mode>0.00000000</mode>
 </bsbObject>
 <bsbObject type="BSBGraph" version="2">
  <objectName/>
  <x>95</x>
  <y>510</y>
  <width>350</width>
  <height>150</height>
  <uuid>{548ba2f4-40d8-4535-9fcc-bc0aa699e27b}</uuid>
  <visible>true</visible>
  <midichan>0</midichan>
  <midicc>-3</midicc>
  <value>0</value>
  <objectName2/>
  <zoomx>1.00000000</zoomx>
  <zoomy>1.00000000</zoomy>
  <dispx>1.00000000</dispx>
  <dispy>1.00000000</dispy>
  <modex>lin</modex>
  <modey>lin</modey>
  <all>true</all>
 </bsbObject>
</bsbPanel>
<bsbPresets>
</bsbPresets>
