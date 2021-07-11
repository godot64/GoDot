; ----------------------------------------- Re-Color Area

recolor	ldx #3		; get first carea (7, 11...)

recolor2	ldy #3		; .x=index in carea (3,7,11..)
;	sta sc_chgcol	; if you want to set your individual color, pass it over in .a
-	lda carea,x
	sta sc_descriptor,y
	dex
	dey
	bpl -
; 	ldx #4		; 4: offset to sc_chgcol (if you passed over your own color in .a)
	ldx #2		; 2: offset to sc_normtext (system ink color)
	jmp gd_fcol

carea	!by 12,11,12,3	; sample areas:
	!by 10,15, 7,1	; row, column, width, height
	!by 10,11,12,1	; all recolored to system ink color

