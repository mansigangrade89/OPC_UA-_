#include "open62541.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Global variables to hold machine, tending device, and client values
UA_Boolean machineVar[4];
UA_Boolean tendingDeviceVar[5];
UA_Boolean clientVar;

// Function prototypes
static UA_StatusCode read_machine_values(UA_Client *client);
static UA_StatusCode read_tendingdevice_values(UA_Client *client);
static UA_StatusCode read_client_values(UA_Client *client);
static void inform_process_status(void);

int main(void) {
    UA_Client *machineClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(machineClient));
    // Connect to the machine server
    UA_StatusCode machineClientStatus = UA_Client_connect(machineClient, "opc.tcp://localhost:4840");
    if (machineClientStatus != UA_STATUSCODE_GOOD) {
        UA_Client_delete(machineClient);
        return (int)machineClientStatus;
    }

    UA_Client *tendingDeviceClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(tendingDeviceClient));
    // Connect to the tending device server
    UA_StatusCode tendingDeviceClientStatus = UA_Client_connect(tendingDeviceClient, "opc.tcp://localhost:8000");
    if (tendingDeviceClientStatus != UA_STATUSCODE_GOOD) {
        UA_Client_delete(machineClient);
        UA_Client_delete(tendingDeviceClient);
        return (int)tendingDeviceClientStatus;
    }

    UA_Client *client1 = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client1));
    // Connect to the client1 server
    UA_StatusCode client1Status = UA_Client_connect(client1, "opc.tcp://localhost:4840");
    if (client1Status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(machineClient);
        UA_Client_delete(tendingDeviceClient);
        UA_Client_delete(client1);
        return (int)client1Status;
    }

    printf("Machine, Tending Device, and Client1 clients connected\n");
    printf("Client2 is running\n");

    while (1) {
        // Read values from the machine server
        UA_StatusCode machineStatus = read_machine_values(machineClient);
        if (machineStatus != UA_STATUSCODE_GOOD) {
            printf("Error reading machine values\n");
        }

        // Read values from the tending device server
        UA_StatusCode tendingDeviceStatus = read_tendingdevice_values(tendingDeviceClient);
        if (tendingDeviceStatus != UA_STATUSCODE_GOOD) {
            printf("Error reading tending device values\n");
        }

        // Read values from the client1 server
        UA_StatusCode clientStatus = read_client_values(client1);
        if (clientStatus != UA_STATUSCODE_GOOD) {
            printf("Error reading client1 values\n");
        }

        // Inform about the ongoing process based on the values read
        inform_process_status();

        sleep(1); // Adjust sleep interval as needed
    }

    UA_Client_disconnect(machineClient);
    UA_Client_delete(machineClient);
    UA_Client_disconnect(tendingDeviceClient);
    UA_Client_delete(tendingDeviceClient);
    UA_Client_disconnect(client1);
    UA_Client_delete(client1);

    return 0;
}

// Function definitions
static UA_StatusCode read_machine_values(UA_Client *client) {
    // Implement reading machine values from the machine server
    UA_StatusCode retval;
    UA_Variant value;
    UA_Variant_init(&value);

    // Read machine values from the server
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(2, "R1_MA_STA_PRODUCTIONRDY"), &value);
    if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
        machineVar[0] = *(UA_Boolean *)value.data;
    } else {
        return UA_STATUSCODE_BAD;
    }

    // Repeat the process for other machine values...

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode read_tendingdevice_values(UA_Client *client) {
    // Implement reading values from the tending device server
    UA_StatusCode retval;
    UA_Variant value;
    UA_Variant_init(&value);

    // Read tending device values from the server
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(2, "R2_AM_STA_PRODUCTIONRDY"), &value);
    if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
        tendingDeviceVar[0] = *(UA_Boolean *)value.data;
    } else {
        return UA_STATUSCODE_BAD;
    }

    // Repeat the process for other tending device values...

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode read_client_values(UA_Client *client) {
    // Implement reading values from the client server
    UA_StatusCode retval;
    UA_Variant value;
    UA_Variant_init(&value);

    // Read client value from the server
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(2, "ClientVariable"), &value);
    if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
        clientVar = *(UA_Boolean *)value.data;
    } else {
        return UA_STATUSCODE_BAD;
    }

    // Repeat the process for other client values...

    return UA_STATUSCODE_GOOD;
}

static void inform_process_status(void) {
    printf("Process Status:\n");
    printf("---------------\n");
    printf("Machine Status: ...\n"); // Replace '...' with the actual status based on the values read from the machine server
    printf("Tending Device Status: ...\n"); // Replace '...' with the actual status based on the values read from the tending device server
    printf("Client1 Status: ...\n"); // Replace '...' with the actual status based on the values read from the client1 server
    printf("\n");
}

