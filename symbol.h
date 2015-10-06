typedef struct symbol *symbol_t;
typedef struct symbol_header *symbol_header_t;

enum symbol_type {
	COMMAND_SYMBOL,	// Regular commands
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
	char *simple_command;
	struct symbol *next;
};

// Create a new symbol
symbol_t newSymbol();
