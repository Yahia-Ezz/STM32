
#ifndef SCH_H
#define SCH_H

void SCH_Init(void);
void SCH_StartExecution(void);
void SCH_AppendTaskToQueue(void (*PtrToFunc)());

#endif