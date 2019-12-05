
; ------------------------------------------------ Variables to be declared
;
	adcnt		= $30 	; (activity counter, sample location)
;
; ------------------------------------------------ 

	sprptr		= $07f8
	spr255		= $3fc0

	spr2x		= $d004
	spr2y		= $d005
	msbx		= $d010
	spren		= $d015
	sprxe		= $d01d
	border		= $d020
	sprcol2		= $d029

; ------------------------------------------------ Activity

adinc	dec adcnt		; advance after adcnt passes
	bne +
	inc spr2y
	inc spr2y+2
	jmp setadcnt	; reset adcnt to 5

; ------------------------------------------------ Abschluss Activity

clearad	lda spren		; Wanderbalken aus
	and #$f3
	sta spren
	lda msbx
	and #$f3
	sta msbx
	lda #$ff		; Grafik verändert,
	sta gr_redisp	; also Flag für System setzen
	clc 		; fertig
+	rts 

; ------------------------------------------------ Init Activity

initad	ldy #60		; Spriteblock 255 vorbereiten
	lda #0
-	sta spr255+3,y
	dey 
	bpl -
	sty spr255	; dünner Strich
	sty spr255+1
	sty spr255+2
	lda #15		; Farbe hellgrau
	sta sprcol2
	sta sprcol2+1
	lda sprxe		; doppelt breit
	ora #$0c
	sta sprxe

	lda msbx		; x über 255
	ora #$0c
	sta msbx
	lda #8		; x-Ordinate: 8 (plus 256)
	sta spr2x
	lda #$20
	sta spr2x+2
	lda #$92		; y-Ordinate: 146
	sta spr2y		; (im Image Information Fenster)
	sta spr2y+2

	sty sprptr+2	; Sprite 255
	sty sprptr+3
	lda spren
	ora #$0c		; Activity-Balken aktivieren
	sta spren
setadcnt	lda #5		; Startzähler (5 mal 40 = 200)
	sta adcnt
	rts


