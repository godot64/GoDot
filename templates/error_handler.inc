; ----------------------------------------- Error Handler

; --------------------------- Error Messages (32 bytes wide)

err1 	!scr "    ERROR: Not a BASIC file!    "	; 0	(to: .x)
err2	!scr "ERROR: No PRINT or missing quote"	; 32
err3	!scr "ERROR: Missing colon? Line >40? "	; 64
err4	!scr " ERROR: REM-line# out of range! "	; 96
err5	!scr "     ERROR: File corrupt!       "	; 128
err6	!scr "  ERROR: File can't be a Font!  "	; 160

; --------------------------- Message Output

faila	ldx #0
	!by $2c
failb	ldx #32
	!by $2c
failc	ldx #64
	!by $2c
faild	ldx #96
	!by $2c
faile	ldx #128
	!by $2c
failf	ldx #160

	ldy #0
fl0	lda err1,x
	sta message,y	; - see activity_gauge.inc
	inx 
	iny 
	cpy #32
	bne fl0

; ...	jsr errnf		; rebuild requesters if necessary

	jsr gd_clrms
	jsr messout	; - see activity_gauge.inc

	pla 		; abandon last routine
	pla 		; (fill with PHA before if neccessary)

	clc 		; flag: stay in requester
	rts


