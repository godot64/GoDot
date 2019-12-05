; --------------------------- Activity Gauge

action	dec adcnt
	bne ld4
	lda cntwert
	sta adcnt
	ldy offy
	cpy #20		; !!! this value must be the same as at CLRMESS
	bne act0
	jsr clrmess
act0	ldx offx
	lda filltab,x
	sta mess,y
	jsr messout
	dec offx
	bpl ld4
	inc offy
	lda #7
	sta offx
ld4	rts

; --------------------------- Message Display Handler

tcopy	ldy #0		; Select message
tc0	lda txt,x		; in .x: offset to message
	beq clrmess
	sta message,y
	inx
	iny
	bne tc0

clrmess   ldx #20		; clear gauge bar
	lda #32
cl0	sta mess,x
	dex
	bpl cl0
	ldy #0		; init counters
	ldx #7	
	sty offy
	stx offx
	rts

messout   ldx #<message	; display message and gauge
	ldy #>message
	jmp gd_xtxout2

filltab   !by 160,93,103,127,126,124,105,109

cntwert   !by 50		; activity counter (advance after cntwert passes)

; sample texts:

txt	!scr " Colors @"	; 0 (to: .x)
	!scr " Convert@"	; 9
	!scr " Bitmap @"	; 18
	!scr " EnTile @"	; 27
	!scr " Move   @"	; 36

message   !fill 11,32	; gauge bar: 32 bytes screencode
mess	!fill 21,32	; value at MESS must be one more than at CLRMESS
	!by 0


