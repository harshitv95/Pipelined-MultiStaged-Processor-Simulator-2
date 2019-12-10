#include <stdlib.h>
#include "issue_queue.h"



int is_iq_full(){
    
    int i =7;
    while (i>=0)

    {
        if (issueQueueList[i] != NULL)//check the issueQueue is empty or not
            i--;
        else
            break;      //break as soon as it encounters the empty entry in issueQueue
    }
    return i;
}

int insert_to_iq(APEX_Instruction* instruction){
    int idx;
    if ((idx = is_iq_full())==-1)
        return 0;
    else
    {
        issueQueueList[idx]=instruction;
    }
}



APEX_Instruction* pop_from_iq(int index){

    
    APEX_Instruction instruction = issueQueueList[index];
    issueQueueList[index]=NULL;     
    return instruction;



}