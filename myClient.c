#include "open62541.h"
#include <stdlib.h>
#include <unistd.h>

static UA_StatusCode toggle_value(UA_Client *client, char* variable, UA_Boolean val)
{
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    UA_StatusCode retval;
    UA_Variant_setScalar(&value,&val,&UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_writeValueAttribute(client,UA_NODEID_STRING(2, variable),&value); 
    return UA_STATUSCODE_GOOD;  
}
static UA_StatusCode read_machine_values(UA_Client *client, UA_Boolean* machineVar)
{
    //UA_Boolean machineVar[4];
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    UA_StatusCode retval;
    //Read variables for machine in machine server
    // R1_MA_STA_PRODUCTIONRDY
    UA_Boolean R1_MA_STA_PRODUCTIONRDY;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R1_MA_STA_PRODUCTIONRDY"),&value);
    if(retval == UA_STATUSCODE_GOOD 
    &&  UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])
        ) {
            R1_MA_STA_PRODUCTIONRDY=*(UA_Boolean *)value.data;
            machineVar[0]=R1_MA_STA_PRODUCTIONRDY;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Machine ready for production: %i", R1_MA_STA_PRODUCTIONRDY);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R1_MA_STA_PRODUCTIONRDY");
    }
    //R1_MA_ENA_InIntfrArea
    UA_Boolean R1_MA_ENA_InIntfrArea;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R1_MA_ENA_InIntfrArea"),&value);
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R1_MA_ENA_InIntfrArea=*(UA_Boolean *)value.data;
            machineVar[1]=R1_MA_ENA_InIntfrArea;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Machine asks Tending device to enter interference area: %i", R1_MA_ENA_InIntfrArea);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R1_MA_ENA_InIntfrArea");
    }

    //R1_MA_REQ_UnldPart01
    UA_Boolean R1_MA_REQ_UnldPart01;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R1_MA_REQ_UnldPart01"),&value);
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R1_MA_REQ_UnldPart01=*(UA_Boolean *)value.data;
            machineVar[2]=R1_MA_REQ_UnldPart01;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Machine requests unload: %i", R1_MA_REQ_UnldPart01);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R1_MA_REQ_UnldPart01");
    }

    //R1_MA_REQ_LodPart01
    UA_Variant value1;
    UA_Variant_init(&value1);
    UA_Boolean R1_MA_REQ_LodPart01;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R1_MA_REQ_LodPart01"),&value1);
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value1, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R1_MA_REQ_LodPart01=*(UA_Boolean *)value1.data;
            machineVar[3]=R1_MA_REQ_LodPart01;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Machine requests load: %i", R1_MA_REQ_LodPart01);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R1_MA_REQ_LodPart01");
    }
    return UA_STATUSCODE_GOOD;
}
static UA_StatusCode read_tendingdevice_values(UA_Client *client, UA_Boolean* tendingDeviceVar)
{
    //UA_Boolean tendingDeviceVar[5];
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);
    UA_StatusCode retval;
    
    UA_Boolean R2_AM_STA_PRODUCTIONRDY;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R2_AM_STA_PRODUCTIONRDY"),&value);
    
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R2_AM_STA_PRODUCTIONRDY=*(UA_Boolean *)value.data;
            tendingDeviceVar[0]=R2_AM_STA_PRODUCTIONRDY;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Tending Device ready for production: %i", R2_AM_STA_PRODUCTIONRDY);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R2_AM_STA_PRODUCTIONRDY");
        
    }
    
    // R2_AM_STA_OutIntfrArea
    UA_Boolean R2_AM_STA_OutIntfrArea;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R2_AM_STA_OutIntfrArea"),&value);
    
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R2_AM_STA_OutIntfrArea=*(UA_Boolean *)value.data;
            tendingDeviceVar[1]=R2_AM_STA_OutIntfrArea;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Tending Device out of interference area: %i", R2_AM_STA_OutIntfrArea);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R2_AM_STA_OutIntfrArea");
        
    }
    
    // R2_AM_ACK_UnldPartCmpl01
    UA_Boolean R2_AM_ACK_UnldPartCmpl01;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R2_AM_ACK_UnldPartCmpl01"),&value);
    
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R2_AM_ACK_UnldPartCmpl01=*(UA_Boolean *)value.data;
            tendingDeviceVar[2]=R2_AM_ACK_UnldPartCmpl01;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Tending Device completed Unloading: %i", R2_AM_ACK_UnldPartCmpl01);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R2_AM_ACK_UnldPartCmpl01");
        
    }

    // R2_AM_ACK_LodPartCmpl01
    UA_Boolean R2_AM_ACK_LodPartCmpl01;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R2_AM_ACK_LodPartCmpl01"),&value);
    
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R2_AM_ACK_LodPartCmpl01=*(UA_Boolean *)value.data;
            tendingDeviceVar[3]=R2_AM_ACK_LodPartCmpl01;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Tending Device completed Loading: %i", R2_AM_ACK_LodPartCmpl01);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R2_AM_ACK_LodPartCmpl01");
        
    }
    // R2_AM_ACK_ReqLodUnldNeg
    UA_Boolean R2_AM_ACK_ReqLodUnldNeg;
    retval=UA_Client_readValueAttribute(client,UA_NODEID_STRING(2,"R2_AM_ACK_ReqLodUnldNeg"),&value);
    
    if(retval == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            R2_AM_ACK_ReqLodUnldNeg=*(UA_Boolean *)value.data;
            tendingDeviceVar[4]=R2_AM_ACK_ReqLodUnldNeg;
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Tending Device reqlodunldneg: %i", R2_AM_ACK_ReqLodUnldNeg);
        }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "could not read R2_AM_ACK_ReqLodUnldNeg");
        
    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode copyVariables(UA_Client *client1, UA_Client *client2)
{
    UA_StatusCode retval;
    char* machineVariables[4]={"R2_MA_STA_PRODUCTIONRDY","R2_MA_ENA_InIntfrArea","R2_MA_REQ_UnldPart01","R2_MA_REQ_LodPart01"};
    UA_Boolean machineVarPrev[4];
    UA_Boolean machineVarCurr[4];    
    char* tendingDeviceVariables[5]={"R1_AM_STA_PRODUCTIONRDY","R1_AM_STA_OutIntfrArea","R1_AM_ACK_UnldPartCmpl01","R1_AM_ACK_LodPartCmpl01","R1_AM_ACK_ReqLodUnldNeg"};
    UA_Boolean tendingDeviceVarPrev[5];
    UA_Boolean tendingDeviceVarCurr[5];
    retval=read_machine_values(client1,machineVarPrev);
    retval=read_tendingdevice_values(client2,tendingDeviceVarPrev);
    sleep(5);
    // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "%s", "calling");
    // retval=UA_Client_call(client1,UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),UA_NODEID_NUMERIC(1,62541),0,NULL,NULL,NULL);
    while(true)
    {
        sleep(3);
        retval=read_machine_values(client1,machineVarCurr);
        retval=read_tendingdevice_values(client2,tendingDeviceVarCurr);
        if(machineVarPrev != machineVarCurr)
        {
            getchar();
            for(int i=0;i<4;i++)
            {
                toggle_value(client2,machineVariables[i],machineVarCurr[i]);
            }
            // for(int i=0;i<4;i++)
            // {
            //     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "%s: %i", machineVariables[i],machineVarCurr[i]);
            // }
            for(int i=0;i<4;i++)
            machineVarPrev[i]=machineVarCurr[i];
        }
        if(tendingDeviceVarPrev!=tendingDeviceVarCurr)
        {
            getchar();
            for(int i=0;i<5;i++)
            {
                toggle_value(client1,tendingDeviceVariables[i],tendingDeviceVarCurr[i]);
            }
            // for(int i=0;i<5;i++)
            // {
            //     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "%s: %i", tendingDeviceVariables[i],tendingDeviceVarCurr[i]);
            // }
            for(int i=0;i<5;i++)
            tendingDeviceVarPrev[i]=tendingDeviceVarCurr[i];
        }
        
    }
    return UA_STATUSCODE_GOOD;
}

int main(void) {

    //Machine server connection
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "server on 4840 connected");
    }
    
    //retval=read_machine_values(client);

    //Connect tending device server
    UA_Client *client1 = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client1));
    UA_StatusCode retval1 = UA_Client_connect(client1, "opc.tcp://localhost:8000");
    if(retval1 != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client1);
        return (int)retval1;
    }
    else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "server on 8000 connected");
    }
    
    //sleep(60);
    //retval=UA_Client_call(client1,UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),UA_NODEID_NUMERIC(1,62541),0,NULL,NULL,NULL);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "start monitoring for tending");
    

    // Create a subscription
    // UA_CreateSubscriptionRequest request;
    // UA_Client_Subscriptions_create(client, request,NULL, )

    // UA_MonitoredItemCreateRequest monRequest_Machine[4];
    
    // monRequest_Machine[0]=UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(2, (char* const)"R1_MA_STA_PRODUCTIONRDY"));
    // monRequest_Machine[1]=UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(2, (char* const)"R1_MA_ENA_InIntfrArea"));
    // monRequest_Machine[2]=UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(2, (char* const)"R1_MA_REQ_UnldPart01"));
    // monRequest_Machine[3]=UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(2, (char* const)"R1_MA_REQ_LodPart01"));
    // monRequest_Machine[0].requestedParameters.samplingInterval=1000.0;
    // monRequest_Machine[1].requestedParameters.samplingInterval=1000.0;
    // monRequest_Machine[2].requestedParameters.samplingInterval=1000.0;
    // monRequest_Machine[3].requestedParameters.samplingInterval=1000.0;

    // UA_Client_MonitoredItems_createDataChange(client,4,monRequest_Machine,copyVariables,NULL,NULL)
    
    
    
    
    
    
    
    retval=copyVariables(client,client1);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "check production value in tending device");
    
    
    //retval=read_tendingdevice_values(client1);
    /* Clean up */
    //UA_Variant_clear(&value);
    //UA_Client_delete(client); /* Disconnects the client internally */
    return EXIT_SUCCESS;
}

