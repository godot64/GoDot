; ---------------------------
;
;   godotinput.inc
;   : GoDot Input System
;
;   1,00, first release
;   1.01, 20.08.93, bugfix: what if input length is 1?
;         14.08.17, changed internal input labels for convenience
;
;
;   This Include File must be set next to the process section of
;   each saver. Without this include there's no keyboard input.
;   First entry is *always* "initinp".
;   (In goDos it's part of ui_slot4 respectively.)
;
;   Every saver starts with this code:
;   
;   start	ldx nr		; check function number
;	dex 		; is func #1?
;	bne st0
;	jmp initinp	; yes: input
;
;   st0	...		; otherwise saver
;
; ---------------------------
;
; --------------------------- Equations

	in_mlen		= $fd
	in_cpos		= $fe
	
; ---------------------------
; --------------------------- Input Routines
; ---------------------------
;
; --------------------------- Output keystrokes on screen & mark cursor

in_cursor	ldy sc_br		; move in_buffer to out_buffer
	dey 
-	lda ls_nambuf,y
	sta sc_movetab,y
	dey 
	bpl -
	jsr gd_cnvbc	; convert to screencode
	sta sc_loop	; care for color mode (.a is >0)
	jsr gd_xtxout3	; set text colors
	stx sc_loop	; care for text mode (.x is 0)
	jsr gd_initmove	; compute position in text screen ram
	ldy in_cpos	; get cursor position
	lda (sc_vekt20),y	; invert this position
	eor #$80
	sta (sc_vekt20),y
	rts 

; --------------------------- Compute text cursor position

in_setcur	dec ls_len
	jsr gd_position	; compute row/col of mouse pointer
	lda sc_merk	; row
	cmp sc_zl		; inside input gadget?
	bne +++		; no, discard
	sec 
	lda sc_merk+1	; column
	sbc sc_sp		; in current text?
	cmp ls_len
	bcc ++		; yes, store position
	ldy ls_len	; no, text fills gadget?
	cpy sc_br
	bcc +		; no, set cursor behind last letter
	dey 		; yes, set cursor on last letter
+	tya 
++	sta in_cpos	; set position of cursor
+++	inc ls_len
	rts 

; --------------------------- Init Input

initinp	lda ls_flen	; save length of current filename (no matter if any)
	sta in_mlen
	ldy sc_br		; clear out_buffer
	jsr gd_blank
	lda #0		; set cursor to position 0
	sta ls_len	; input length: 0
	sta in_cpos
	ldy sc_br
	sta sc_movetab,y	; delimit (with $00)
	dey 
-	lda (sc_vekt20),y	; write template text into buffer
	ldx ls_len
	bne +
	cmp #32
	beq +
	sty ls_len	; when no more blanks: this is length of text
	inc ls_len
	inc ls_len
+	sta ls_nambuf,y
	sta in_buff2,y
	dey 
	bpl -
	ldy ls_len	; any text?
	bne +
	iny 		; no, set text length to 1
	sty ls_len
+	jsr gd_cnvasc	; convert to petscii
	jsr in_setcur	; mark cursor

; --------------------------- Input from keyboard

in_input	ldx #0		; init key buffer
	stx sy_tbuffer
	jsr in_cursor	; mark cursor
-	lda sy_tbuffer	; --- wait for
	beq -		; key strokes ---
	
	ldx #9		; check: control char?
-	cmp in_sond1,x
	beq ++		; yes, execute
	dex 
	bpl -
	ldy sc_iflag	; input delimited to digits (or else)?
	beq +		; no
-	cmp (ls_vekta8),y	; check delimiter_list
	beq ++		; yes, key is in
	dey 
	bpl -
	bmi in_input	; no, not in, wait for next input
	
+	cmp #32		; check key, permitted?
	bcc in_input
	cmp #219
	bcs in_input	; no, wait
	ldx #6		; umlaut?
-	cmp in_sond2,x
	beq ++		; yes, permitted
	dex 
	bpl -
	cmp #91		; 91 to 192: not permitted
	bcc ++
	cmp #193
	bcc in_input
++	cmp #147		; key "clear"?
	beq c2end
	cmp #19		; key "home"?
	beq c2strt
	cmp #148		; key "inst"?
	beq in_insert
	cmp #20		; key "del"?
	beq in_delete
	cmp #3		; key "stop"?
	beq in_jcan
	cmp #13		; key "return"?
	beq in_ready
	cmp #189		; key "c=x"?
	beq in_clear
	cmp #29		; key "cursor right"?
	beq in_right
	cmp #157		; key "cursor left"?
	beq in_left
	cmp #160		; key "shift space"?
	beq setcur
	cmp #64
	bne +
	lda #128

+	jmp in_string	; everything else: permitted

c2end	jmp in_sclr
c2strt	jmp in_shome
in_insert	jmp in_sinsrt
in_delete	jmp in_sdel
in_clear	jmp in_sclear
in_right	jmp in_sright
in_left	jmp in_sleft
setcur	jsr in_setcur
	jmp in_input
in_jcan	jmp in_cancel

; --------------------------- <RETURN>

in_ready	ldy sc_br		; blank out_buffer
	jsr gd_blank
-	ldy ls_len	; length = 0 : Cancel
	cpy sc_br
	bne +
	inc ls_len
	bne -
+	dey 
	dey 
	bmi in_jcan
	tya 
	tax 
-	lda ls_nambuf,y	; in_buffer to out_buffer
	sta sc_movetab,y
	dey 
	bpl -
-	lda sc_movetab,x	; ignore trailing spaces
	cmp #32
	bne +
	dec ls_len
	dex 
	bne -
+	jsr gd_cnvbc	; convert to screen code
	ldy in_mlen	
	ldx sc_zl		; input from filerequester? (row 20)
	cpx #20
	bne ++		; no, "lastname" remains unaffected
	ldx ls_dirmask	; yes, input is name for a graphics file?
	beq +		; yes, change "lastname" completely to this one
	ldx #11		; no, add prefix (4 chars)
	ldy #15
-	lda ls_nambuf,x
	sta ls_nambuf,y
	dey 
	dex 
	bpl -
-	inx 
	lda $073c,x	; prefix is in mode gadget on screen
	sta ls_nambuf,x	; copy from there
	cpx #3
	bne -
	clc 
	ldy ls_len
	dey 
	sty ls_flen2	; adjust length of input
	tya 
	adc #5
	sta ls_len
+	ldy #15		; write out_buffer to lastname
-	lda sc_movetab,y
	sta ls_lastname,y
	dey 
	bpl -
	ldy ls_len
	dey 
++	sty ls_flen	; store new length of filename

	clc 		; set mouse pointer to Load Gadget on screen
	lda #8
	adc sy_soffx
	sta $d002
	sta $d000
	clc 
	lda #114
	adc sy_soffy
	sta $d003
	tay 
	dey 
	dey 
	sty $d001
	ldy #3
	sty $d010
-	jsr gd_xtxout3	; write input to input gadget again (sets cursor off)
	clc 		; input done
	rts 

; --------------------------- <STOP>

in_cancel	ldy sc_br		; blank out_buffer
	jsr gd_blank
	ldy in_mlen	; restore old length of filename (if any)
	beq +
	dey 		; there is one: restore 
--	lda in_buff2,y
	sta sc_movetab,y
	sta ls_nambuf,y
	dey 
	bpl --
+	jsr gd_cnvasc	; convert to petscii
	ldy in_mlen	; correct length
	sty ls_flen
	bcc -		; finish

; --------------------------- Input (insert mode)

in_string	ldx in_inst	; insert mode?
	bne in_over	; no, overwrite mode
	
	pha 		; yes, save current char 
	ldx sc_br
	ldy sc_br
	dey 
-	lda ls_nambuf-1,y	; shift rest to the right
	sta ls_nambuf-1,x
	dex 
	dey 
	cpx in_cpos
	bne -
	pla 		; write current char into gap
	sta ls_nambuf,x
	inx 
	cpx sc_br		; max width reached?
	beq s0		; yes, don't increase cursor position...
	stx in_cpos
	ldx ls_len	; ...or length
	dex 
	cpx sc_br
	beq s0
	inc ls_len	; if not, do
	bne s0

; --------------------------- Input (overwrite mode)

in_over	ldx in_cpos	; write current char to in_buffer
	sta ls_nambuf,x
	inx 		; cursor to next position
	cpx sc_br		; max width?
	beq s0
	stx in_cpos
	ldx ls_len	; max length?
	cpx in_cpos
	bne s0
	inc ls_len	; no, set 
	bne s0


; ---------------------------
; --------------------------- Input Commands
; ---------------------------

; --------------------------- <HOME>

in_shome	ldx #0		; position to 0
	stx in_cpos
	beq s0

; --------------------------- <CLR> (Shift Home)

in_sclr	ldx ls_len	; position after (or on) last char
	dex 
	cpx sc_br
	beq +
	cpx #1
	bne ++
+	dex 
++	stx in_cpos
s0	jmp in_input

; --------------------------- <CURSOR LEFT>

in_sleft	ldx in_cpos	; position minus 1 (until 0)
	dex 
	bmi s0
	stx in_cpos
	bpl s0

; --------------------------- <CURSOR RIGHT>

in_sright	ldx in_cpos	; position plus 1 (until last char (+1))
	inx 
	cpx ls_len
	bcs s0
	cpx sc_br
	beq s0
	stx in_cpos
	bcc s0

; --------------------------- <C=X> (Discard Input)

in_sclear	ldy ls_len	; is empty anyway?
	dey 
	dey 
	bmi s0		; yes, don't change
	lda #32
-	sta ls_nambuf,y	; erase in_buffer
	dey 
	bpl -
	iny 		; position to 0
	sty in_cpos
	iny 
	sty ls_len
	bne s0

; --------------------------- <INSERT> (Toggle insert mode)

in_sinsrt	lda in_inst	; toggle flag (starts with insert mode)
	eor #1
	sta in_inst
	jmp in_input

; --------------------------- <DEL> (Delete char)

in_sdel	lda in_cpos	; is empty anyway?
	tax 
	tay 
	inx 
	cpx sc_br
	beq +
	cpx ls_len
	beq s0
	dex 		; no, shift chars right from cursor 1 left
	iny 
-	lda ls_nambuf,y
	sta ls_nambuf,x
	inx 
	iny 
	cpy sc_br
	bne -
	dey 		; and blank to end of gadget
+	lda #32
	sta ls_nambuf,y
	lda ls_len
	cmp #2		; until gadget empty
	beq s0
	dec ls_len
	bpl s0

; --------------------------- tables & flags

in_inst	!by 0
in_buff2	!fill 40,32
in_sond1	!by 147,148,157,29,189,3,13	; Clr, Inst, l-Arr, r-Arr, C=x, Stop, Return
	!by 20,19,160		; Del, Home, Sh-Space
in_sond2	!by 93,91,176,185,186,92,64	; ä, ö, Ä, Ö, Ü, ß, ü


