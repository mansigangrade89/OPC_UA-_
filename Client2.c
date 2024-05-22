#include "open62541.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to connect to an OPC UA server
UA_Client* connectToServer(char* endpointUrl) {
    // Create a new UA_Client instance
    UA_Client* client = UA_Client_new();

    // Set default client configuration
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    // Connect to the server using the provided endpoint URL
    UA_StatusCode status = UA_Client_connect(client, endpointUrl);
    if (status != UA_STATUSCODE_GOOD) {
        printf("Failed to connect to server at %s\n", endpointUrl);
        UA_Client_delete(client); // Clean up if connection fails
        return NULL;
    }

    // Connection successful
    printf("Connected to server at %s\n", endpointUrl);
    return client;
}

// Function to disconnect from an OPC UA server
void disconnectFromServer(UA_Client* client) {
    // Disconnect the client from the server
    UA_Client_disconnect(client);

    // Delete the client instance and release resources
    UA_Client_delete(client);
}

int main() {
    // Connect to the Machine Server
    UA_Client* machineClient = connectToServer("opc.tcp://localhost:4840");
    if (!machineClient) return EXIT_FAILURE;

    // Connect to the Tending Device Server
    UA_Client* tendingDeviceClient = connectToServer("opc.tcp://localhost:8000");
    if (!tendingDeviceClient) {
        disconnectFromServer(machineClient); // Clean up if connection fails
        return EXIT_FAILURE;
    }

    // Connect to Client1 (Replace CLIENT1_PORT with the actual port number)
    UA_Client* myClient = connectToServer("opc.tcp://localhost:4840");
    if (!myClient) {
        disconnectFromServer(machineClient);
        disconnectFromServer(tendingDeviceClient);
        return EXIT_FAILURE;
    }

    // Print a message indicating that Client2 is running
    printf("Client2 is running...\n");

    // Continuously monitor state changes and update the Unified Automation command line
    while (1) {
        // Check for state changes in the machine, tending device, and Client1
        // Subscribe to relevant variables or nodes to receive updates

        // If a significant event occurs, update the command line interface
        // For example, if production starts, print a message indicating the start of production
        printf("Production process has started.\n");

        // Sleep for a short duration before checking for updates again
        // Adjust the sleep duration based on the desired update frequency
        usleep(500000); // 500 ms
    }

    // Disconnect and clean up resources when Client2 exits
    disconnectFromServer(machineClient);
    disconnectFromServer(tendingDeviceClient);
    disconnectFromServer(myClient);

    return EXIT_SUCCESS;
}

