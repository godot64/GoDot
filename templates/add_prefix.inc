; ----------------------------------------- Namen verlängern

addprefix	pha		; Länge merken
	ldy #0
ag4	lda ls_nambuf,y	; beginnt mit "PI."?
	cmp prefix,y
	bne ag2		; nein
	iny
	cpy #3		; (### anpassen)
	bcc ag4
	pla		; ja, fertig
	rts
	
ag2	pla		; verlängern um 3 Zeichen
	clc
	adc #3		; (### anpassen)
	tay		; Name nun länger als 16 Zeichen?
	cpy #16
	bcs ag0		; ja, behandeln

ag5	ldx #12		; nein, "pi." vor den Namen (### anpassen)
ag1	lda ls_nambuf,x
	sta ls_nambuf+3,x
	lda ls_lastname,x
	sta ls_lastname+3,x
	dex
	bpl ag1
	ldx #2		; Länge (### anpassen)
ag3	lda prefix,x
	sta ls_nambuf,x
	lda prefixs,x
	sta ls_lastname,x
	dex
	bpl ag3
	sty wflen		; neue Länge merken, fertig
	sty ls_flen
	rts

ag0	dey		; auf 16 verkürzen
	cpy #16		; ### anpassen
	bne ag0
	beq ag5		; unbed. Sprung

; ----------------------------------------- 

prefix	!pet "pi."	; Länge: plus 3, max 13
prefixs	!scr "pi."

