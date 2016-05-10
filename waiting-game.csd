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

; schedule "flute",0,5
giFluteFreqs ftgen 0,0,16,-2,1,2,3,4,5,6,7,8
giFluteAmps ftgen 0,0,16,-2, 1,0.5,0.3,0.2,0.1,0.05,0.03,0.02
instr flute
	ipan = p4
	; TODO: tämber muutus ajas -  chebyshev?
	kline line 1,p3,0
	anoise = pinkish(rnd(0.05))
	anoise butterlp anoise*kline, random:i(1000,8000) ; somewhat different noise, decay during note
	ioct = int(random:i(1,2.99))
	ideviation = cent(random:i(-20,20))
	iattack random 0.05,1
	idecay = iattack * random:i(0.7,1.5)
	isustain random 0.3,0.8
	print ioct, ideviation, ipan
	ifreq = cpspch(5.05)*ioct*ideviation ; fis, with some deviation
	aenv adsr iattack,idecay,isustain,p3/2
	;asig adsynt2 0.3,ifreq, -1, giFluteFreqs, giFluteAmps, 8
	
	asig poscil 1, ifreq
	kh1 = 0.2+rnd:i(0.8) ;
	kh2 = rnd:i(0.5) ;0.5 + jspline:k(0.2,0.5,2) ; add some variation to specter
	;printk2 kh2  
	kh3 =rnd:i(0.4);0.3 + jspline:k(0.1,0.5,2)
	kh4 =rnd:i(0.3)*kline ;0.2 + jspline:k(0.05,0.5,2)
	kh5 =rnd:i(0.2)*kline
	kh6 =rnd:i(0.1)*kline

		
	asig chebyshevpoly asig, 0, kh1, kh2, kh3, kh4, kh5,kh6
	
	
	aout = (anoise+asig*0.3) * aenv
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
 <x>0</x>
 <y>0</y>
 <width>0</width>
 <height>0</height>
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
  <eventLine>i "flute" 0 15</eventLine>
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
</bsbPanel>
<bsbPresets>
</bsbPresets>
