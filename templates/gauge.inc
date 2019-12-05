
; ----------------------------------------- Gauge 

gaugeout	lda sc_screenvek	; show "requester"
	pha
	lda sc_screenvek+1
	pha
	ldx #<gaugelst
	ldy #>gaugelst
	jsr gd_screen
	pla
	sta sc_screenvek+1
	pla
	sta sc_screenvek
	rts

; ----------------------------------------- Screenlist for Gauge

gaugelst	!by 0,22,3,34,3,$02
	!wo 0
	!by $80


