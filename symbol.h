enum symbol_type {
	COMMAND_SYMBOL,	// Regular commands
	LT_SYMBOL,	// <
	GT_SYMBOL,	// >
	OR_SYMBOL,	// ||
	AND_SYMBOL,	// &&
	PIPE_SYMBOL,	// |
	LBRACKET_SYMBOL	// (
	RBRACKET_SYMBOL	// )
	SEQUENCE_SYMBOL // ;
	NEWLINE_SYMBOL  // \n
};

struct symbol {
	enum symbol_type type;

	struct symbol *next;
}	
	
