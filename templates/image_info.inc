; ----------------------------------------- Image Information Handler

getname	ldx #0
si0	lda ls_lastname,x	; name of loaded picture
	beq si1
	sta nbuf,x
	inx
	cpx #16
	bcc si0
si1	rts

setttext	ldx #6		; choose mode "Text"
	!by $2c
sethtext	ldx #13		; or "Hires"
	!by $2c
setmtext	ldx #20		; or "Multi"
	ldy #6
chi1	lda htext,x
	sta modetx,x
	dex
	dey
	bpl chi1
	rts

getdatac	ldx #4		; Datatype: Color
	!by $2c
getdatag	ldx #9		; Datatype: Gray
	ldy #4
sinfo0	lda dtextc,x
	sta datatype,y
	dex
	dey
	bpl sinfo0
	rts

setinfo	jsr getdatac	; this one: Color
	jsr setname
	jsr setloader
	jsr setmode
	jmp setdata

setname	lda #0
	ldx #<ls_picname
	ldy #>ls_picname
	bne si3
setloader	lda #17
	ldx #<ls_iloader
	ldy #>ls_iloader
	bne si3
setmode	lda #25
	ldx #<ls_imode
	ldy #>ls_imode
	bne si3
setdata	lda #33
	ldx #<ls_idrive
	ldy #>ls_idrive
si3	stx sc_texttab
	sty sc_texttab+1
	tax
	ldy #0
si4 	lda nbuf,x
	beq si5
	sta (sc_texttab),y
	inx
	iny
	bne si4
si5	rts

nbuf	!fill 16,32	; 16 bytes buffer for filename
	!by 0

modname	!scr "xxxxxxx@"	; loader "xxxxxxxx" (hardcoded here)

modetx	!scr "320x200@"	; resolution
datatype	!scr "Color@"	; datatype

; ----------------------------------------- Presets

ttext	!scr "Text   "	; modes
htext	!scr "320x200"
mtext	!scr "160x200"

dtextc	!scr "Color"	; data types
dtextg	!scr "Gray "

