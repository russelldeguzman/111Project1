#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//TODO: when you finish this, rememeber to COPY THIS OVER TO read-command.c!!
void parseSimpCommand(char * parserOutput, char *input, char *output, char *word){
  //TODO: TEST THIS CRAZY FUNCTION
  int pos = 0;
  int hasInput = -1; //represents the position of '<'
  int hasOutput = -1; //represents the position of '>'
  while(parserOutput[pos] != '\0'){ //TODO: remember to end strings with '\0'
    if(parserOutput[pos] == '<') hasInput = pos;
    if(parserOutput[pos] == '>') hasOutput = pos; 
    pos = pos + 1;
  }

  if(hasOutput == -1 && hasInput == -1){ //CASE 1: NO I/O REDIRECTION
    *input = 0;
    *output = 0; 
    word = parserOutput;
    return;
  }
  if(hasOutput != -1 && hasInput == -1){//CASE 2: Output but no Input
    char *result_word = (char *) malloc(sizeof(char) * pos);
    char *output_word = (char *) malloc(sizeof(char) * pos);
    int i;

    for(i = 0; i < hasOutput; i++){
      result_word[i] = parserOutput[i]; //get result word
    }

    if(isspace(result_word[hasOutput-1])){ //elim whitespace before > if there is one
      result_word[hasOutput - 1] = '\0';
    }
    else{
      result_word[hasOutput] = '\0';
    }

    if(isspace(parserOutput[hasOutput + 1])){
      i = hasOutput + 2; //elim whitespace after > if there is any
    }
    else{
      i = hasOutput + 1;
    }
    int offset = i;
    for(i = offset; i < pos; i++){
      output_word[i - offset] = parserOutput[i]; //get output 
    }
    output_word[pos - offset] = '\0';

    *input = 0;
    output = output_word;
    word = result_word;
    return;
  } 
  if(hasOutput == -1 && hasInput != -1){//CASE 3: Input but no Output
    char *result_word = (char *) malloc(sizeof(char) * pos);
    char *input_word = (char *) malloc(sizeof(char) * pos);
    int i;

    for(i = 0; i<hasInput; i++){
      result_word[i] = parserOutput[i]; //get result word 
    }

    if(isspace(result_word[hasInput-1])){ //elim whitespace before < if there is one
      result_word[hasInput - 1] = '\0';
    }
    else{
      result_word[hasInput] = '\0';
    }

    if(isspace(parserOutput[hasInput + 1])){
      i = hasInput + 2; //elim whitespace after < if there is any
    }
    else{
      i = hasInput + 1;
    }
    int offset = i;

    for(i = offset; i < pos; i++){
      input_word[i - offset] = parserOutput[i]; //get output 
    }
    input_word[pos - offset] = '\0';

    *output = 0; 
    word =  result_word;
    input = input_word;
    return;
  }
  if(hasInput != -1 && hasOutput != -1){//CASE 4: has both I/O
    //In test cases, Input always comes before Output, so I'm going to assume that's 
    //the correct syntax. 
    char *result_word = (char *) malloc(sizeof(char) * pos);
    char *input_word = (char *) malloc(sizeof(char) * pos);
    char *output_word = (char *) malloc(sizeof(char) * pos);
    int i;
    for(int i = 0; i < hasInput; i++){
      result_word[i] = parserOutput[i]; //get result file word
    }
    if(isspace(result_word[hasInput-1])){ //elim whitespace before < if there is one
      result_word[hasInput - 1] = '\0';
    }
    else{
      result_word[hasInput] = '\0';
    }

    if(isspace(parserOutput[hasInput + 1])){
      i = hasInput + 2; //elim whitespace after < if there is any
    }
    else{
      i = hasInput + 1;
    }
    int offset = i;

    for(i = offset; i < hasOutput; i++){
      input_word[i - offset] = parserOutput[i]; //get output 
    }

    if(isspace(result_word[hasOutput-1])){ //elim whitespace before > if there is one
      input_word[hasOutput - offset -1] = '\0';
    }
    else{
      input_word[hasOutput - offset] = '\0';
    }

    if(isspace(parserOutput[hasOutput + 1])){
      i = hasOutput + 2; //elim whitespace after > if there is any
    }
    else{
      i = hasOutput + 1;
    }
    offset = i;
    for(i = offset; i < pos; i++){
      output_word[i - offset] = parserOutput[i]; //get output 
    }
    output_word[pos - offset] = '\0';

    output = output_word; 
    word =  result_word;
    input = input_word;
    return;
  }
  printf("No events triggered! Error!\n");
}

int main(int argc, char **argv){
  char parserOutput[] = "g++ -c foo.c"; 
  char *input; char *output; char *word;
  parseSimpCommand(parserOutput, input, output, word);
  printf("%s\n", word);
  printf("%s\n", input);
  printf("%s\n", output);
}

