#include "open62541.h"

#include <signal.h>
#include <stdlib.h>

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}
//not being used currently but might be needed
static UA_StatusCode simpleUnload(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{
    UA_Boolean now = false;
    UA_Variant value;
    UA_Variant_init(&value);
    UA_StatusCode retval;
    UA_Boolean ENA_IntfrArea=false;
    UA_Boolean REQ_Unld=false;
    retval=UA_Server_readValue(server,UA_NODEID_STRING(2, "R2_MA_ENA_InIntfrArea"),&value);
    ENA_IntfrArea=*(UA_Boolean *)value.data;
    if(ENA_IntfrArea && REQ_Unld)
    {
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_OutIntfrArea"), value);
        sleep(10);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload complete");
        now=true;
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_ACK_UnldPartCmpl01"), value);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload acknowledgement set");
        sleep(5);
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_OutIntfrArea"), value);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Out of interference area set");
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload Complete");
    }
    return UA_STATUSCODE_GOOD;
}
//function to be triggered when machine variable req_unload is called
static void unload(UA_Server *server, UA_UInt32 monitoredItemId,
                               void *monitoredItemContext, const UA_NodeId *nodeId,
                               void *nodeContext, UA_UInt32 attributeId,
                               const UA_DataValue *value1) {
UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload called");
    UA_Boolean now = false;
    UA_Variant value;
    UA_Variant_init(&value);
    UA_StatusCode retval;
    UA_Boolean ENA_IntfrArea=false;
    UA_Boolean REQ_Unld=false;
    retval=UA_Server_readValue(server,UA_NODEID_STRING(2, "R2_MA_ENA_InIntfrArea"),&value);
    ENA_IntfrArea=*(UA_Boolean *)value.data;
    UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"R2_MA_ENA_InIntfrArea: %i",ENA_IntfrArea);

    retval=UA_Server_readValue(server,UA_NODEID_STRING(2, "R2_MA_REQ_UnldPart01"),&value);
    REQ_Unld=*(UA_Boolean *)value.data;
    UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"R2_MA_REQ_UnldPart01: %i",REQ_Unld);
    if(ENA_IntfrArea && REQ_Unld)
    {
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        //Setting out of interference area variable to false
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_OutIntfrArea"), value);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Out of interference area unset");
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unloading");
        sleep(5);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload complete");
        now=true;
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        //setting unload acknowledgement to true
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_ACK_UnldPartCmpl01"), value);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload acknowledgement set");
        sleep(3);
        
        //UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Unload Complete");
    }
    
    }
static void change_outIntfrnceArea(UA_Server *server, UA_UInt32 monitoredItemId,
                            void *monitoredItemContext, const UA_NodeId *nodeId,
                            void *nodeContext, UA_UInt32 attributeId,
                            const UA_DataValue *value1) {
    UA_Boolean now = true;
    UA_Variant value;
    UA_Variant_init(&value);
    UA_StatusCode retval;
    UA_Boolean Unld_Ack=false;
    retval=UA_Server_readValue(server,UA_NODEID_STRING(2, "R2_AM_ACK_UnldPartCmpl01"),&value);
    Unld_Ack=*(UA_Boolean *)value.data;
    if(Unld_Ack)
    {
        UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_Server_writeValue(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_OutIntfrArea"), value);
        UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Out of interference area set");  
    }                     
    }
static UA_StatusCode AM_ACK_UnldPartCmpl01_ValueCallback(UA_Server *server,UA_NodeId currentNodeId)
{   
    
    UA_MonitoredItemCreateRequest monRequest =
        UA_MonitoredItemCreateRequest_default(currentNodeId);
    monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, change_outIntfrnceArea);
}
static UA_StatusCode myValueCallback(UA_Server *server,UA_NodeId currentNodeId)
{   
    
    UA_MonitoredItemCreateRequest monRequest =
        UA_MonitoredItemCreateRequest_default(currentNodeId);
    monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
    //calling unload function
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, unload);
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server=UA_Server_new();
    UA_ServerConfig_setMinimal(UA_Server_getConfig(server),8000,NULL);
    //Add namespace
    UA_Int16 ns_room2=UA_Server_addNamespace(server,"Room2");
    UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"Added namespace with no. %d", ns_room2);
    
    //Add Machine Object
    UA_NodeId machineId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes machineAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, (char* const)"R2_Machine"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, (char* const)"Machine"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            machineAttr, NULL, &machineId);
    //Add Production Ready attribute
    UA_VariableAttributes Production_Ready_Attr = UA_VariableAttributes_default;
    UA_Boolean MA_STA_ProductionRdy = true;
    UA_Variant_setScalar(&Production_Ready_Attr.value, &MA_STA_ProductionRdy, &UA_TYPES[UA_TYPES_BOOLEAN]);
    Production_Ready_Attr.accessLevel= UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_MA_STA_PRODUCTIONRDY"), machineId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"MA_STA_ProductionRdy"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), Production_Ready_Attr, NULL, NULL);
    
    //Add In Interference Area attribute
    UA_VariableAttributes IN_Interfr_Area_Attr = UA_VariableAttributes_default;
    UA_Boolean MA_ENA_InIntfrArea = false;
    UA_Variant_setScalar(&IN_Interfr_Area_Attr.value, &MA_ENA_InIntfrArea, &UA_TYPES[UA_TYPES_BOOLEAN]);
    IN_Interfr_Area_Attr.accessLevel= UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_MA_ENA_InIntfrArea"), machineId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"MA_ENA_InIntfrArea"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), IN_Interfr_Area_Attr, NULL, NULL);
    
    //Add Unload Part 01 attribute
    UA_VariableAttributes Unload_Part_01_Attr = UA_VariableAttributes_default;
    UA_Boolean MA_REQ_UnldPart01 = false;
    UA_Variant_setScalar(&Unload_Part_01_Attr.value, &MA_REQ_UnldPart01, &UA_TYPES[UA_TYPES_BOOLEAN]);
    Unload_Part_01_Attr.accessLevel= UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    Unload_Part_01_Attr.valueRank=UA_VALUERANK_SCALAR;
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_MA_REQ_UnldPart01"), machineId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"MA_REQ_UnldPart01"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), Unload_Part_01_Attr, NULL, NULL);

    //Add Load Part 01 attribute
    UA_VariableAttributes Load_Part_01_Attr = UA_VariableAttributes_default;
    UA_Boolean MA_REQ_LodPart01 = false;
    UA_Variant_setScalar(&Load_Part_01_Attr.value, &MA_REQ_LodPart01, &UA_TYPES[UA_TYPES_BOOLEAN]);
    Load_Part_01_Attr.accessLevel= UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_MA_REQ_LodPart01"), machineId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"MA_REQ_LodPart01"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), Load_Part_01_Attr, NULL, NULL);
    //Add TendingDevice Object
    UA_NodeId tendingDeviceId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes tendingDeviceAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, (char* const)"R2_TendingDevice"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, (char* const)"TendingDevice"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            tendingDeviceAttr, NULL, &tendingDeviceId);
    //Add Production Ready attribute
    UA_VariableAttributes td_Production_Ready_Attr = UA_VariableAttributes_default;
    UA_Boolean AM_STA_ProductionRdy = true;
    UA_Variant_setScalar(&td_Production_Ready_Attr.value, &AM_STA_ProductionRdy, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_PRODUCTIONRDY"), tendingDeviceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"AM_STA_ProductionRdy"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), td_Production_Ready_Attr, NULL, NULL);
    //Add Out of Interference Area attribute
    UA_VariableAttributes Out_Interference_Area_Attr = UA_VariableAttributes_default;
    UA_Boolean AM_STA_OutIntfrArea = true;
    UA_Variant_setScalar(&Out_Interference_Area_Attr.value, &AM_STA_OutIntfrArea, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_AM_STA_OutIntfrArea"), tendingDeviceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"AM_STA_OutIntfrArea"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), Out_Interference_Area_Attr, NULL, NULL);
    //Add Unload Complete attribute
    UA_VariableAttributes td_Unload_Complete_Attr = UA_VariableAttributes_default;
    UA_Boolean AM_ACK_UnldPartCmpl01 = false;
    UA_Variant_setScalar(&td_Unload_Complete_Attr.value, &AM_ACK_UnldPartCmpl01, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_AM_ACK_UnldPartCmpl01"), tendingDeviceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"AM_ACK_UnldPartCmpl01"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), td_Unload_Complete_Attr, NULL, NULL);
    //Add Load Complete attribute
    UA_VariableAttributes td_Load_Complete_Attr = UA_VariableAttributes_default;
    UA_Boolean AM_ACK_LodPartCmpl01 = false;
    UA_Variant_setScalar(&td_Load_Complete_Attr.value, &AM_ACK_LodPartCmpl01, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_AM_ACK_LodPartCmpl01"), tendingDeviceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"AM_ACK_LodPartCmpl01"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), td_Load_Complete_Attr, NULL, NULL);
    //Add Load Unload Negative attribute
    UA_VariableAttributes td_Load_Unload_Negative_Attr = UA_VariableAttributes_default;
    UA_Boolean AM_ACK_ReqLodUnldNeg = false;
    UA_Variant_setScalar(&td_Load_Unload_Negative_Attr.value, &AM_ACK_ReqLodUnldNeg, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, (char* const)"R2_AM_ACK_ReqLodUnldNeg"), tendingDeviceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, (char* const)"AM_ACK_ReqLodUnldNeg"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), td_Load_Unload_Negative_Attr, NULL, NULL);
    
    //check for change in machine variable req_unload
    myValueCallback(server,UA_NODEID_STRING(2, (char* const)"R2_MA_REQ_UnldPart01"));
    AM_ACK_UnldPartCmpl01_ValueCallback(server,UA_NODEID_STRING(2, (char* const)"AM_ACK_UnldPartCmpl01"));
    UA_LOG_INFO(UA_Log_Stdout,UA_LOGCATEGORY_USERLAND,"starting server .....");
    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

