
//This is the code for the list container with which we can link our command trees together,
//put the command list related functions in here:

#ifndef list
#define list

//command_stream node initalize
void node_init(command_stream_t stream, command_t command);

//get tail of list
command_stream_t getTail(command_stream_t head);

//add to list 
void append(command_stream_t tail, command_stream_t newNode);

#endif
