#include <stdio.h>
#include "open62541/build/open62541.h"

void DeleteSubscriptionCallback(UA_Client *client, UA_UInt32 subId, void *subContext);

void StatusChangeNotificationCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
     UA_StatusChangeNotification *notification);

/* Callback for the deletion of a MonitoredItem */
void DeleteMonitoredItemCallback
    (UA_Client *client, UA_UInt32 subId, void *subContext,
     UA_UInt32 monId, void *monContext);

/* Callback for DataChange notifications */
void DataChangeNotificationCallback
    (UA_Client *client, UA_UInt32 subId, void *subContext,
     UA_UInt32 monId, void *monContext,
     UA_DataValue *value);

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

    /** subscription */
    UA_CreateSubscriptionRequest subReq = UA_CreateSubscriptionRequest_default();

    UA_CreateSubscriptionResponse subResp = UA_Client_Subscriptions_create(client, subReq, NULL, 
        StatusChangeNotificationCallback, DeleteSubscriptionCallback);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Subscription id: %d", subResp.subscriptionId);

    /** node id to monitor */
    UA_NodeId nid = UA_NODEID_NUMERIC(1, 4006);

    /** monitored items */
    UA_MonitoredItemCreateRequest monReq = UA_MonitoredItemCreateRequest_default(nid);
    monReq.requestedParameters.queueSize = 100;

    UA_MonitoredItemCreateResult monRes = UA_Client_MonitoredItems_createDataChange(
        client, subResp.subscriptionId,
        UA_TIMESTAMPSTORETURN_NEITHER, monReq,
        NULL, DataChangeNotificationCallback,
        DeleteMonitoredItemCallback);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create monitored item status: %s. id: %d", 
        UA_StatusCode_name(monRes.statusCode), monRes.monitoredItemId);

    if(monRes.statusCode != UA_STATUSCODE_GOOD) {
        return monRes.statusCode;
    }

    while(1) {
        UA_Client_runAsync(client, 0);
    }

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Client status before exit: %s", UA_StatusCode_name(status));
    return status;
}

void DeleteSubscriptionCallback(UA_Client *client, UA_UInt32 subId, void *subContext) 
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Delete subscription callback: %d", subId);
}

void StatusChangeNotificationCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
    UA_StatusChangeNotification *notification)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Subscription status change");
}

/* Callback for the deletion of a MonitoredItem */
void DeleteMonitoredItemCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
    UA_UInt32 monId, void *monContext)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Delete monitored item: %d", monId);
}

/* Callback for DataChange notifications */
void DataChangeNotificationCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
    UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
    UA_NodeId inDataType = value->value.type->typeId;
    if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_BOOLEAN].typeId))) {
        UA_Boolean* inValue = (UA_Boolean*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %d. id: %d", *inValue, monId);
    } else if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_STRING].typeId))) {
        UA_String* inValue = (UA_String*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %s. id: %d", inValue->data, monId);
    } else if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_DOUBLE].typeId))) {
        UA_Double* inValue = (UA_Double*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %g. id: %d", *inValue, monId);
    } else if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_FLOAT].typeId))) {
        UA_Float* inValue = (UA_Float*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %g. id: %d", *inValue, monId);
    } else if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_UINT32].typeId))) {
        UA_UInt32* inValue = (UA_Double*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %d. id: %d", *inValue, monId);
    } else if (UA_NodeId_equal(&inDataType, &(UA_TYPES[UA_TYPES_INT32].typeId))) {
        UA_Int32* inValue = (UA_Double*)(value->value.data);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification: %d. id: %d", *inValue, monId);
    } else {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Data change notification unsupported data type: %d. id: %d", inDataType.identifier.numeric, monId);
    }
}
