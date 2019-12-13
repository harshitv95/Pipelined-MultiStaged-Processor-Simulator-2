#include <stdlib.h>
#include "load_store_queue.h"
#include "cpu_commons.h"


int is_lsq_full(){
int i = LOAD_STORE_QUEUE_CAPACITY-1;
    while (i>=0) {
        if (loadStoreQueueList[i] != NULL)//check the loadStoreQueue is empty or not
            i--;
        else
            break;      //break as soon as it encounters the empty entry in issueQueue
    }
    return i;

}


int insert_to_lsq(CPU_Stage* instruction){
    
    int idx;
    if ((idx = is_lsq_full())==-1)
        return 0;
    else{
           loadStoreQueueList[idx]=instruction;
           return 1;
    }


}

CPU_Stage* get_lsq_available_instructions(int address){

    CPU_Stage* inst_array[LOAD_STORE_QUEUE_CAPACITY];
    for (int i = 0; i < LOAD_STORE_QUEUE_CAPACITY; i++)
    {
        switch (loadStoreQueueList[i]->opcode){
            case LOAD:
            case LDR:
            case STR:
            case STORE:{
                if (loadStoreQueueList[i]->rs1_valid){
                    loadStoreQueueList[i]->mem_address=address;
                    inst_array[i]=loadStoreQueueList[i];
                } 
                else
                    inst_array[i]=NULL;   
                break;
            }
       }

        inst_array[i] =loadStoreQueueList[i];
    }
return inst_array;
}

CPU_Stage* pop_from_lsq(int index){
    
    CPU_Stage* instruction = loadStoreQueueList[index];
    loadStoreQueueList[index]=NULL;     
    return instruction;


}