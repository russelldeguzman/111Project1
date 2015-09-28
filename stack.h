
/*This implementation is of a GENERIC stack using void 
pointers to access memory regardless of the data type */

typedef struct{
	void *data; 
	int dataSize; 
	int length;
	int allocLength;
} stack;

void newStack(stack *st, int dataSize); //create stack
void destroyStack(stack *st); //free stack
bool StackisEmpty(const stack *st); //isempty
void pushStack(stack *st, const void* add); //push
void popStack(stack *st, void* top); //pop
void topStack(stack *st, void* top); //view top element (doesnt affect stack)

