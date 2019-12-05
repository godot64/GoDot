; ----------------------------------------- Counters
; 
count 	inc sc_vekt20	; increment sc_vekt20 (or any other) by 1
	bne cou5
	inc sc_vekt20+1
cou5 	inc sc_texttab	; increment sc_texttab (or any other) by 1
	bne cou6
	inc sc_texttab+1
cou6 	lda ls_vekta8	; decrement ls_vekta8/9 by 1
	bne cou7
	dec ls_vekta8+1
cou7 	dec ls_vekta8
	lda ls_vekta8	; and check if 0 (sets .e)
	ora ls_vekta8+1
	rts


