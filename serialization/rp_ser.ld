	SECTION_DATA_PROLOGUE(rp_ser_decoder,,SUBALIGN(4))
	{
		_rp_ser_decoder_start = .;
		KEEP(*(SORT_BY_NAME(".rp_ser_decoder*")))
		_rp_ser_decoder_end = .;
	} GROUP_LINK_IN(ROMABLE_REGION)
