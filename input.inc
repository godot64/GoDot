; ---------------------------
;
;   godotinput.inc
;   : GoDot Input System
;
;   1,00, first release
;   1.01, 20.08.93, bugfix: what if input length is 1?
;
;
;   This Include File must be set next to the process section of
;   each saver. Without this include there's no keyboard input.
;   First entry is always "initinp".
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

	mlen		= $fd
	cpos		= $fe
	
; ---------------------------
; --------------------------- Input Routines
; ---------------------------
;
; --------------------------- Output keystrokes on screen & mark cursor
;
cursor	ldy sc_br		; move in_buffer to out_buffer
	dey 
c0	lda ls_nambuf,y
	sta sc_movetab,y
	dey 
	bpl c0
	jsr gd_cnvbc	; convert to screencode
	sta sc_loop	; care for color mode (.a is >0)
	jsr gd_xtxout3	; set text colors
	stx sc_loop	; care for text mode (.x is 0)
	jsr gd_initmove	; compute position in text screen ram
	ldy cpos		; get cursor position
	lda (sc_vekt20),y	; invert this position
	eor #$80
	sta (sc_vekt20),y
	rts 
;
; --------------------------- Compute text cursor position
;
setcursor	dec ls_len
	jsr gd_position	; compute row/col of mouse pointer
	lda sc_merk	; row
	cmp sc_zl		; inside input gadget?
	bne sc0		; no, discard
	sec 
	lda sc_merk+1	; column
	sbc sc_sp		; in current text?
	cmp ls_len
	bcc sc1		; yes, store position
	ldy ls_len	; no, text fills gadget?
	cpy sc_br
	bcc sc2		; no, set cursor behind last letter
	dey 		; yes, set cursor on last letter
sc2	tya 
sc1	sta cpos		; set position of cursor
sc0	inc ls_len
	rts 
;
; --------------------------- Init Input
;
initinp	lda ls_flen	; save length of current filename (no matter if any)
	sta mlen
	ldy sc_br		; clear out_buffer
	jsr gd_blank
	lda #0		; set cursor to position 0
	sta ls_len	; input length: 0
	sta cpos
	ldy sc_br
	sta sc_movetab,y	; delimit (with $00)
	dey 
ii0	lda (sc_vekt20),y	; write template text into buffer
	ldx ls_len
	bne ii1
	cmp #32
	beq ii1
	sty ls_len	; when no more blanks: this is length of text
	inc ls_len
	inc ls_len
ii1	sta ls_nambuf,y
	sta buff2,y
	dey 
	bpl ii0
	ldy ls_len	; any text?
	bne ii3
	iny 		; no, set text length to 1
	sty ls_len
ii3	jsr gd_cnvasc	; convert to petscii
	jsr setcursor	; mark cursor
;
; --------------------------- Input from keyboard
;
input	ldx #0		; init key buffer
	stx sy_tbuffer
	jsr cursor	; mark cursor
iloop	lda sy_tbuffer	; --- wait for 
	beq iloop		; key strokes ---
	
	ldx #9		; check: control char?
in0	cmp sonder1,x
	beq in1		; yes, execute
	dex 
	bpl in0
	ldy sc_iflag	; input delimited to digits (or else)?
	beq in2		; no
in3	cmp (ls_vekta8),y	; check delimiter_list
	beq in1		; yes, key is in
	dey 
	bpl in3
	bmi input		; no, not in, wait for next input
	
in2	cmp #32		; check key, permitted?
	bcc input
	cmp #219
	bcs input		; no, wait
	ldx #5		; umlaut?
in4	cmp sonder2,x
	beq in1		; yes, permitted
	dex 
	bpl in4
	cmp #91		; 91 to 193: not permitted
	bcc in1
	cmp #193
	bcc input
in1	cmp #147		; key "clear"?
	beq c2end
	cmp #19		; key "home"?
	beq c2strt
	cmp #148		; key "inst"?
	beq insert
	cmp #20		; key "del"?
	beq delete
	cmp #3		; key "stop"?
	beq jcancel
	cmp #13		; key "return"?
	beq ready
	cmp #189		; key "c=x"?
	beq clear
	cmp #29		; key "cursor right"?
	beq right
	cmp #157		; key "cursor left"?
	beq left
	cmp #160		; key "shift space"?
	beq setcur
	cmp #64		; key "ü"?
	bne in5
	lda #186		; yes, change to "Ü"
	
in5	jmp string	; everything else: permitted
;
c2end	jmp sclr
c2strt	jmp shome
insert	jmp sinsert
delete	jmp sdelete
clear	jmp sclear
right	jmp sright
left	jmp sleft
setcur	jsr setcursor
	jmp input
jcancel	jmp cancel
;
; --------------------------- <RETURN>
;
ready	ldy sc_br		; blank out_buffer
	jsr gd_blank
r01	ldy ls_len	; length = 0 : Cancel
	cpy sc_br
	bne r00
	inc ls_len
	bne r01
r00	dey 
	dey 
	bmi jcancel
	tya 
	tax 
r0	lda ls_nambuf,y	; in_buffer to out_buffer
	sta sc_movetab,y
	dey 
	bpl r0
r02	lda sc_movetab,x	; ignore trailing spaces
	cmp #32
	bne r03
	dec ls_len
	dex 
	bne r02
r03	jsr gd_cnvbc	; convert to screen code
	ldy mlen	
	ldx sc_zl		; input from filerequester? (row 20)
	cpx #20
	bne r1		; no, "lastname" remains unaffected
	ldx ls_dirmask	; yes, input is name for a graphics file?
	beq r4		; yes, change "lastname" completely to this one
	ldx #11		; no, add prefix (4 chars)
	ldy #15
r5	lda ls_nambuf,x
	sta ls_nambuf,y
	dey 
	dex 
	bpl r5
r6	inx 
	lda $073c,x	; prefix is in mode gadget on screen
	sta ls_nambuf,x	; copy from there
	cpx #3
	bne r6
	clc 
	ldy ls_len
	dey 
	sty ls_flen2		; adjust length of input
	tya 
	adc #5
	sta ls_len
r4	ldy #15		; write out_buffer to lastname
r3	lda sc_movetab,y
	sta ls_lastname,y
	dey 
	bpl r3
	ldy ls_len
	dey 
r1	sty ls_flen	; store new length of filename

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
r2	jsr gd_xtxout3	; write input to input gadget again (sets cursor off)
	clc 		; input done
	rts 
;
; --------------------------- <STOP>
;
cancel	ldy sc_br		; blank out_buffer
	jsr gd_blank
	ldy mlen		; restore old length of filename (if any)
	beq cc1
	dey 		; there is one: restore 
cc0	lda buff2,y
	sta sc_movetab,y
	sta ls_nambuf,y
	dey 
	bpl cc0
cc1	jsr gd_cnvasc	; convert to petscii
	ldy mlen		; correct length
	sty ls_flen
	bcc r2		; finish
;
; --------------------------- Input (insert mode)
;
string	ldx inst		; insert mode?
	bne over		; no, overwrite mode
	
	pha 		; yes, save current char 
	ldx sc_br
	ldy sc_br
	dey 
str0	lda ls_nambuf-1,y	; shift rest to the right
	sta ls_nambuf-1,x
	dex 
	dey 
	cpx cpos
	bne str0
	pla 		; write current char into gap
	sta ls_nambuf,x
str1	inx 
	cpx sc_br		; max width reached?
	beq s0		; yes, don't increase cursor position...
	stx cpos
	ldx ls_len	; ...or length
	dex 
	cpx sc_br
	beq s0
	inc ls_len	; if not, do
	bne s0
;
; --------------------------- Input (overwrite mode)
;
over	ldx cpos		; write current char to in_buffer
	sta ls_nambuf,x
	inx 		; cursor to next position
	cpx sc_br		; max width?
	beq s0
	stx cpos
	ldx ls_len	; max length?
	cpx cpos
	bne s0
	inc ls_len	; no, set 
	bne s0
;
;
; ---------------------------
; --------------------------- Input Commands
; ---------------------------
;
; --------------------------- <HOME>
;
shome	ldx #0		; position to 0
	stx cpos
	beq s0
;
; --------------------------- <CLR> (Shift Home)
;
sclr	ldx ls_len	; position after (or on) last char
	dex 
	cpx sc_br
	beq s2
	cpx #1
	bne s1
s2	dex 
s1	stx cpos
s0	jmp input
;
; --------------------------- <CURSOR LEFT>
;
sleft	ldx cpos		; position minus 1 (until 0)
	dex 
	bmi s0
	stx cpos
	bpl s0
;
; --------------------------- CURSOR RIGHT
;
sright	ldx cpos		; position plus 1 (until last char (+1))
	inx 
	cpx ls_len
	bcs s0
	cpx sc_br
	beq s0
	stx cpos
	bcc s0
;
; --------------------------- <C=X> (Discard Input)
;
sclear	ldy ls_len	; is empty anyway?
	dey 
	dey 
	bmi s0		; yes, don't change
	lda #32
scl1	sta ls_nambuf,y	; erase in_buffer
	dey 
	bpl scl1
	iny 		; position to 0
	sty cpos
	iny 
	sty ls_len
	bne s0
;
; --------------------------- <INSERT> (Toggle insert mode)
;
sinsert	lda inst		; toggle flag (starts with insert mode)
	eor #1
	sta inst
	jmp input
;
; --------------------------- <DEL> (Delete char)
;
sdelete	lda cpos		; is empty anyway?
	tax 
	tay 
	inx 
	cpx sc_br
	beq sd0
	cpx ls_len
	beq s0
	dex 		; no, shift chars right from cursor 1 left
	iny 
sd1	lda ls_nambuf,y
	sta ls_nambuf,x
	inx 
	iny 
	cpy sc_br
	bne sd1
	dey 		; and blank to end of gadget
sd0	lda #32
	sta ls_nambuf,y
	lda ls_len
	cmp #2		; until gadget empty
	beq s0
	dec ls_len
	bpl s0
;
; --------------------------- tables & flags
;
inst	!by 0
buff2	!fill 40,32
sonder1	!by 147,148,157,29,189,3,13
	!by 20,19,160
sonder2	!by 93,91,176,185,186,92
;

