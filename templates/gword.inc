	int		= $14

; -------------------------------------- Convert Digits to Word (Byte in INT)
; address of digits in ls_vekta8, digit strings like " 654"

gword	ldy #0
	sty int
	sty int+1
	beq +

-	jsr inca8
+	lda (ls_vekta8),y
	cmp #$20
	beq -

	cmp #$3a
	bcs +
	sec 
	sbc #$30
	sec 
	sbc #$d0
	bcs +
	sbc #$2f
	sta sc_merk
	lda int+1
	sta ls_temp
	cmp #$19		; delimit value to max. 63999 (next pass would exceed 64K)
	bcs +

	lda int		; value times 10:
	asl
	rol ls_temp
	asl
	rol ls_temp
	adc int
	sta int
	lda ls_temp
	adc int+1
	sta int+1
	asl int
	rol int+1
	lda int
	adc sc_merk
	sta int
	bcc -
	inc int+1
	bne -

+	lda int		; result in INT and INT+1
	rts

; -------------------------------------- 

inca8	inc ls_vekta8
	bne +
	inc ls_vekta8+1
+	rts 


