
; --------------------------------------- Locate Cursor to a gadget
; 
; precalculate from gadget parameters:
;
; locating is taking the x-tile value of the gadget where
; you clicked, f.i.: SC_SP=14, SC_BR=6
;                  : SC_ZL=8 , SC_HO=3
;                  : 24 (Sprite x-Offset), 50 (Sprite y-Offset)
; formula for X: (SC_SP*8+(SC_BR-1)*8 - 4) + 24
;            so: 112 + 40 - 4 + 24 = 172
; formula for Y: (SC_ZL*8+(SC_HO-1)*8 - 4) + 50
;            so: 64 + 16 - 4 + 50 = 126
; and then executing LOCATE to set the cursor to the 
; lower right edge of this gadget.
;

locate	ldy #0		; locate GoDot cursor (2 sprites), 
	sty spritehi	; here: to gadget position at 14,8 w/ 6,3
	ldy #172	; x-value of 172
	sty spritex
	sty spritex-2
	ldy #126	; y-value of 126
	sty spritey
	iny
	iny
	sty spritey-2
	rts

