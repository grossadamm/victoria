#ifndef Command_h
#define Command_h

struct Command; /* Forward declaration */

typedef struct Command
{
  char command;
  char* data;
} Command;

#endif