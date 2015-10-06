#include "symbol.h"

symbol_t newSymbol() {
	symbol *new;
	new->simple_command = NULL;
	new->next = NULL;
	return new;
}
