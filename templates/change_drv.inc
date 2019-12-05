; --------------------------- Bootdrive sicherstellen

chgdrv	lda ls_sysdrive
	ldx ls_bootdrive
	sta ls_bootdrive
	stx ls_sysdrive
	rts
	

