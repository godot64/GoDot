; ----------------------------------------- Namen verl�ngern

addsuffix	pha		; L�nge merken
	tay
	dey
	ldx #0
ag4	lda ls_nambuf,y	; endet auf ".abc"? (z.B. "GIF")
	cmp suftx,x
	bne ag2		; nein
	dey
	inx
	cpx #4
	bcc ag4
	pla		; ja, fertig
	rts
	
ag2	pla
	tay		; Name l�nger als 12 Zeichen?
	cpy #13
	bcs ag0		; ja, behandeln

ag5	ldx #3		; nein, ".abc" anh�ngen
ag1	lda suftx,x
ag3	sta ls_nambuf,y
	lda suftxs,x	; auch f�r Image Info
	sta ls_lastname,y
	iny
	dex
	bpl ag1
	sty wflen		; neue L�nge merken, fertig
	sty ls_flen
	rts

ag0	dey		; auf 12 verk�rzen
	cpy #12
	bne ag0
	beq ag5		; unbed. Sprung

; ----------------------------------------- 

suftx	!pet "cba."
suftxs	!scr "cba."

