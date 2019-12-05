; ----------------------------------------------------- Reset Balancing

	ldy #16		; finished, reset Balancing
	sty gr_ctr
	sty gr_brt
dc8	dey
	tya
	sta gr_btab,y
	bne dc8
	sec
	rts


