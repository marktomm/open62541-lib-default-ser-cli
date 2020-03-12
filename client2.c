#include <stdio.h>
#include <time.h>   
#include "open62541/build/open62541.h"

const int _gIdArray[5] = {
    54286,
    54300,
    54314,
    54272,
    54258
};

const int _gArrLen = 5;

int main(int argc, char* argv) 
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Client main start");
    /** client init */
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://localhost:48020");
    if(status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Client failed on connect: %s", UA_StatusCode_name(status));
        return status;
    }

    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);

    for(int i = 0; i < _gArrLen ; i++) {
        UA_NodeId nid = UA_NODEID_NUMERIC(0, _gIdArray[i]);

        char string[20];
        sprintf(string, "%02d:%02d:%02d" , ptm->tm_hour - 2, ptm->tm_min + 1, ptm->tm_sec);

        UA_String avts = UA_STRING((char*)(string));
        UA_Variant avtvar;
        UA_Variant_init(&avtvar);
        UA_Variant_setScalarCopy(&avtvar, &avts, &UA_TYPES[UA_TYPES_STRING]);

        status = UA_Client_writeValueAttribute(client, nid, &avtvar); 
    }

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Client status before exit: %s", UA_StatusCode_name(status));
    return status;
}
