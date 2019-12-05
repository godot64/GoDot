; --------------------------- Kringel definieren

chschr	!by $00,$3a,$46,$4e,$40,$40,$3c,$00

makechr	ldx #7
-	lda chschr,x
	sta sc_undochar,x
	dex
	bpl -
	rts


