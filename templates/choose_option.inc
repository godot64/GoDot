; --------------------------------------
;
; 1. Toggle between two options
;
; -------------------------------------- Sample Event: Set Dither

evdith	ldx dither	; .x= 0: ordered - 1: pattern
	beq +
	ldx #$ff
+	inx		; toggle (EOR #1 in .a would work too)

dth1	stx dither
	lda #<dithtx
	sta sc_texttab	; set target address...
	lda #>dithtx
	sta sc_texttab+1
	txa
	asl
	tay
	lda dithtxadr,y	; set source address of new text
	tax
	lda dithtxadr+1,y
	tay
	jmp gd_xtxtggl	; exchange text in screenlist (no immediate display!)

; -------------------------------------- Related Data

dither	!by 0
dithtxadr	!wo dthord
	!wo dthpat
dthord	!scr "Ordered@"
dthpat	!scr "Pattern@"

sdith	!by 12,18,9,3,$ce	; part of screenlist
	!wo sevdith
dithtx	!scr "Ordered@"


; --------------------------------------
;
; 2. Toggle between 3 (or more) options
;
; -------------------------------------- Sample Event: Set # of grays

evgrays	ldy ncols		; Index auf Farben (5, 9 und 16)
	cpy #2
	bne +
	ldy #$ff
+	iny		; toggle
	sty ncols

	lda colors,y
	sta cols		; neue Farbanzahl publizieren
	jsr gd_xcnvdez	; in Ziffern wandeln
	ldy #1
	sta coltx,y	; in Screenlist eintragen
	txa
	and #15
	bne co1
	ldx #32		; statt führender Null ein Space
co1	txa
	dey
	sta coltx,y

	ldy #6		; und neue Zahl sofort anzeigen
	jmp gd_xtxout1

; -------------------------------------- Related Data

ncols	!by 0
cols	!by 5
colors	!by 5,9,16

sgray	!by 10,23,4,3,$cf	; part of screenlist
	!wo sevgrays
coltx	!scr " 5@"

; --------------------------------------
;
; 3. Toggle between 9 options using tabigad
;
; ------------------------------------ Sample Event: Choose Palette

evpalette	lda palflag
	cmp #8
	bne dp0
	lda #$ff
	sta palflag
dp0	inc palflag	; toggle
	jsr setpalette	; (change to new palette)

	ldx palflag	; set new gadget caption
	lda gadoffs,x
	tax
	ldy #9
dpi1	lda paltxts,x
	sta paltx,y
	dex
	dey
	bpl dpi1

	ldy #0		; Offset to gadget descriptor to change (get from tabigad)
dpi2	jsr settab	; compute screen address
	ldy #6		; Offset to caption in gadget descriptor
	jmp gd_xtxout1	; and display new caption

; ------------------------------------ Output gadget text routines

tabigad	!wo palgad
	!wo areagad

settab	lda #<tabigad	; address gadget descriptor
	sta ls_vekta8
	lda #>tabigad
	sta ls_vekta8+1
; ---	rts		; RTS could be useful

gettab	lda (ls_vekta8),y	; set working vector
	sta sc_screentab
	iny
	lda (ls_vekta8),y
	sta sc_screentab+1

	lda #0		; transfer gadget data (to compute them)
	tax
	tay
gt0	jsr gd_setpos
	cpx #3
	bne gt0
	stx sc_ho
	jsr gd_trim	; trim to inner area of gadget
	jmp gd_initmove	; compute screen address for display

; -------------------------------------- Related Data

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

palgad	!by 9,11,18,3,$ca	; part of screenlist
	!wo evpalette
paltx	!scr "  GoDot   @"


