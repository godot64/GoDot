
; ------------------------------------------------ 
; ------------------------------------------------ common: graphics display
; ------------------------------------------------ 
;
; switch on:  save gr_redisp in dispmk, set gr_redisp to !0, then JSR redisplay
; switch off: JSR dp2, restore gr_redisp from dispmk
;
; ------------------------------------------------ Necessary Declarations
;
;	pport		= $01
;
;	slot		= $45	; /$46, these addrs can move --
;	v1buf		= $47
;	v2buf		= $48
;	cbuf		= $49	; --
;
;	vram2		= $ce0b	; color buffers (standard addrs)
;	vram1		= $de0b
;	cstart		= $ee0b
;
;	grafreg		= $d011	; vic regs
;	multreg		= $d016
;	charreg		= $d018
;	border		= $d020
;	backgr		= $d021
;
; ------------------------------------------------ Necessary Preparation Code
:
;; -------------------------------------- Main
;
;start	ldx sc_screenvek	; save vector to previous list (if not mainlist)
;	stx list
;	ldx sc_screenvek+1
;	stx list+1
;	jsr checkcols	; where is which module? (for re-display)
;	[...]
;
; ------------------------------------------------ 

; ------------------------------------------------ Check Color Buffers

checkcols	sei 		; GoDot off
	ldx #4
	lda #$fe		; preset values (in case of execute it from REU)
-	sta slot,x
	dex
	bne -
	stx pport

	ldy #$03		; set lobyte of...
	sty slot

	lda #$f0		; ...Slot4, check for types of module there
	sta slot+1	; (may be cram when input is active)
	jsr sethicol	; (otherwise unchanged)

	lda #$c0		; ...Slot1 and check
	sta slot+1
	jsr sethicol

	lda #$d0		; ...Slot2 and check
	sta slot+1
	jsr sethicol

	lda #$e0		; ...Slot3 and check
	sta slot+1
	jsr sethicol

	lda #$36		; switch GoDot on again
	sta pport
	cli 
	rts 

; ------------------------------------------------ 

sethicol	lda (slot,x)	; get type of module (at $c003): $20, $40, $80
	tay 
	and #$40		; is saver there? (contains vram1)
	beq +		; no, next type

	jsr c147		; yes, set color buffer 
	sta v1buf
	bne ++

+	tya 		; loader? ($80, contains cram)
	bpl +		; no, next type

	jsr c147		; yes, set accordingly
	sta cbuf
	bne ++

+	jsr c147		; so, it's a modifier (contains vram2)
	sta v2buf

++	rts 

c147 	lda slot+1	; get hibyte...
	ora #$0e		; ...and OR %00001110 to it (ie $ce)
	rts 		; thus: get correct hi address of color buffers

; ------------------------------------------------ 

redisplay	ldx gr_palette	; set border to color 0 (normally black)
	stx border
	jsr setcolors	; get colors from their buffers

	lda gr_cmode	; which mode?
	beq dp4
	lda #$18		; multicolor
	sta multreg
dp4	lda #$1b		; hires
	sta charreg
	lda #$3b
	sta grafreg
	lda gr_redisp	; Flag: display graphics set?
	bne dp3		; yes, branch and leave

; ------------------------------------------------ 
; any activity to perform in graphics screen: add here, probably as a JSR call
; leading into dp2 after finishing (dp2..dp3 switches graphics off and ends redisplay)
; herein could be:
; ----------------------------------------- Wait for Clicks

wait	lda joy		; wait for click or STOP
	eor #$ff
	and #$10
	ora sc_keyprs
	ora sc_stop
	beq wait
	rts

; ------------------------------------------------ 

; ------------------------------------------------ Switch textmode on

tmode	ldx #$13
	lda #$1b
	stx charreg
	sta grafreg
	lda #$08
	sta multreg
	lda sc_maincolor
	sta border
	sta backgr
	rts

; ------------------------------------------ 

; ------------------------------------------------ 
; ------------------------------------------------ 
; Routines to handle GoDot's graphics color buffers
; for comments see sources for god.upmem
; ------------------------------------------------ 
; ------------------------------------------------ 

; ------------------------------------------ 

setcveks	sei
	lda #$35
	sta pport
	lda cbuf 		; color ram
	ldx #$d8
	bne scv0

; ------------------------------------------ 

setbveks	lda v1buf 	; vram1
	ldx #4
	dec pport
scv0	stx ls_adrmisc+1
	ldy #0
	sty sc_merk
	sty ls_adrmisc
	dey

; ------------------------------------------ 

setlast	sty gr_bkcol
	ldy #<cstart
	sty sc_vekt20
	sta sc_vekt20+1
	lda #<500
	sta ls_vekta8
	lda #>500
	sta ls_vekta8+1
	ldy #0
	rts

; ------------------------------------------ 

getcolors	jsr setcveks
stco	lda (ls_adrmisc),y
	sta sc_merk
	inc ls_adrmisc
	bne stc0
	inc ls_adrmisc+1
stc0	lda (ls_adrmisc),y
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	lda sc_merk
	sta (sc_vekt20),y
	jsr count
	bne stco
	jsr setbveks
stc1	lda (ls_adrmisc),y
	sta (sc_vekt20),y
	jsr count
	bne stc1
	ldy gr_bkcol
	bpl scv1
	ldy #0
	lda v2buf 	; vram2
	jsr setlast
	beq stc1
scv1	lda #$36
	sta pport
	cli
	lda backgr
	and #15
	sta gr_bkcol
	rts

; ------------------------------------------ 

setcolors	lda gr_bkcol
	sta backgr
	pha
	jsr setcveks
stc2	sty sc_merk
	lda (sc_vekt20),y
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	lsr
	rol sc_merk
	sta (ls_adrmisc),y
	inc ls_adrmisc
	bne stc3
	inc ls_adrmisc+1
stc3	lda sc_merk
	sta (ls_adrmisc),y
	jsr count
	bne stc2
	jsr setbveks
stc4	lda (sc_vekt20),y
	sta (ls_adrmisc),y
	jsr count
	bne stc4
	ldy gr_bkcol
	bpl scv1
	pla
	tay
	lda v2buf 	; vram2
	jsr setlast
	beq stc4

; ------------------------------------------------ Counters

count	inc sc_vekt20
	bne cou5
	inc sc_vekt20+1
cou5	inc ls_adrmisc
	bne cou6
	inc ls_adrmisc+1
cou6	lda ls_vekta8
	bne cou7
	dec ls_vekta8+1
cou7	dec ls_vekta8
	lda ls_vekta8
	ora ls_vekta8+1
	rts

; ------------------------------------------------ 

