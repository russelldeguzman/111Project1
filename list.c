#include "list.h"

//command_stream node initalize
void node_init(command_stream_t stream, command_t command){
	stream->head=command;
	stream->next=NULL;
}
//get tail of list
command_stream_t getTail(command_stream_t head){
	command_stream_t tail=head;
	while(tail->next != NULL) tail=tail->next;
	return tail;
}

//add to list 
void append(command_stream_t tail, command_stream_t newNode){
	tail->next=newNode;
	newNode->next=NULL;
	tail=tail->next;	
}
