; ----------------------------------------- re-Use Module
; Control recursion:
; USED is preset with $ff
; check at start of Module:
;

	lda used		; previous function cancelled?
	cmp #3
	bcs st12

	jsr requester	; no, do whatever you want

st12	jsr cancel	; yes:
	jsr gd_spron	; mouse on
	sec		; set leave flag
	rts		; finished

; ----------------------------------------- Startup requester

requ 	inc used		; set to: requester was displayed (0)
	ldx #<sgiflst	; open (sample) requester
	ldy #>sgiflst
	jsr gd_xmloop	; wait for clicks
			; after leaving, check USED status:

; ----------------------------------------- Requester management

requester	ldx used		; is negative? (first start?)
	bmi requ		; yes, open requester
	rts		; otherwise: leave module (via st12)

; ----------------------------------------- Leave Module

cn2 	lda #3		; set (after completing your function): all things done
	!by $2c
cancel 	lda #$ff		; preset for next module call
	sta used
	sec		; leave module (.c is set)
	rts


