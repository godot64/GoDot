; --------------------------------------
;
; 1. Toggle between two options
;
; -------------------------------------- Event: Set Dither

evdith	ldx dither	; 0: ordered - 1: pattern
	beq +
	ldx #$ff
+	inx		; or increment

dth1	stx dither	; offset auf Dither merken
	lda #<dithtx
	sta sc_texttab
	lda #>dithtx
	sta sc_texttab+1
	txa
	asl
	tay
	lda dithtxadr,y
	tax
	lda dithtxadr+1,y
	tay
	jmp gd_xtxtggl

; -------------------------------------- Accordimg Data

dither	!by 0
dithtxadr	!wo dthord
	!wo dthpat
dthord	!scr "Ordered@"
dthpat	!scr "Pattern@"

sdith	!by 12,18,9,3,$ce
	!wo sevdith
dithtx	!scr "Ordered@"


; --------------------------------------
;
; 2. Toggle between 3 (or more) options
;
; -------------------------------------- Event: Set # of grays

evgrays	ldy ncols		; Index auf Farben (5, 9 und 16)
	cpy #2
	bne +
	ldy #$ff
+	iny
	sty ncols

	lda colors,y	; Farbnummer nach Ziffern
	sta cols		; Farbe publizieren
	jsr gd_xcnvdez	; in Ziffern wandeln
	ldy #1
	sta coltx,y	; in Screenlist eintragen
	txa
	and #15
	bne co1
	ldx #32
co1	txa
	dey
	sta coltx,y

	ldy #6		; und anzeigen
	jmp gd_xtxout1

; -------------------------------------- Accordimg Data

ncols	!by 0
cols	!by 5
colors	!by 5,9,16

sgray	!by 10,23,4,3,$cf
	!wo sevgrays
coltx	!scr " 5@"

; --------------------------------------
;
; 3. Toggle between 9 options w/ tabigad
;
; ------------------------------------ Event: Choose Palette

evpalette	lda palflag
	cmp #8
	bne dp0
	lda #$ff
	sta palflag
dp0	inc palflag
	jsr setpalette	; change to new palette
	ldx palflag
	lda gadoffs,x
	tax
	ldy #9
dpi1	lda paltxts,x
	sta paltx,y
	dex
	dey
	bpl dpi1
	ldy #0
dpi2	jsr settab
	ldy #6
	jmp gd_xtxout1

; ------------------------------------ Output gadget text routines

tabigad	!wo palgad
	!wo areagad

settab	lda #<tabigad
	sta ls_vekta8
	lda #>tabigad
	sta ls_vekta8+1
; ---	rts		; could be useful

gettab	lda (ls_vekta8),y
	sta sc_screentab
	iny
	lda (ls_vekta8),y
	sta sc_screentab+1
	lda #0
	tax
	tay
gt0	jsr gd_setpos
	cpx #3
	bne gt0
	stx sc_ho
	jsr gd_trim
	jmp gd_initmove

; -------------------------------------- Accordimg Data

palflag	!by 0
gadoffs	!by 9,19,29,39,49,59,69,79,89
paltxts	!scr "  GoDot   "	; 0
	!scr "   CoCo   "	; 10
	!scr " Colodore "	; 20
	!scr "  Pepto   "	; 30
	!scr "  Deekay  "	; 40
	!scr "   Brix   "	; 50
	!scr "   VICE   "	; 60
	!scr "  Wheels  "	; 70
	!scr "  Gray16  "	; 80

palgad	!by 9,11,18,3,$ca
	!wo evpalette
paltx	!scr "  GoDot   @"


