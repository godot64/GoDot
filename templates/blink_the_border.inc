	rahmen		= $d020

; ----------------------------------------------------- Blink the Border

graphon	lda rahmen	; save border color
	pha
	sty rahmen	; set new color (in .y, e.g. "2" for red)
	ldx #$40
	jsr gd_dl2	; delay
	pla
	sta rahmen	; reset old border color
	stx sy_tbuffer	; clear key strokes
stay	clc
	rts


