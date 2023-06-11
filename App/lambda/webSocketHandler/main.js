// Load AWS SDK and create new ApiGatewayManagementApi object
const AWS = require('aws-sdk');
const apigwManagementApi = new AWS.ApiGatewayManagementApi({
    apiVersion: '2018-11-29',
    endpoint: process.env.APIG_ENDPOINT
});

// Load Redis module and create new Redis client
const redis = require('redis');
const client = redis.createClient({
    host: process.env.REDIS_HOST, // Redis hostname
    port: process.env.REDIS_PORT, // Redis port
    password: process.env.REDIS_PASSWORD // Redis password
});

// Convert some Redis client methods to promises
const { promisify } = require('util');
const smembersAsync = promisify(client.smembers).bind(client);
const sremAsync = promisify(client.srem).bind(client);

// Lambda function to broadcast payload to all WebSocket connections
exports.handler = async (event, context) => {
    console.log("Received event: ", event);

    // Parse the payload
    const payload = event;
    console.log("Parsed payload: ", payload);

    // Check if the payload contains the "button" attribute
    if (typeof payload.button !== 'boolean') {
        console.log('Received message does not contain "button" attribute with boolean value');
        return { statusCode: 400, body: 'Invalid message format' };
    }

    // Get all the WebSocket connection IDs from the Redis set
    const connectionIds = await smembersAsync('connectionIds');
    console.log("Retrieved connection IDs from Redis: ", connectionIds);

    // Loop through each connection ID and post the payload
    for (let connectionId of connectionIds) {
        try {
            // Attempt to post the payload to the WebSocket connection
            await apigwManagementApi.postToConnection({
                ConnectionId: connectionId,
                Data: JSON.stringify(payload)
            }).promise();
        } catch (e) {
            console.log(`Failed to post to connection ${connectionId}: ${e.message}`);
            // If the connection no longer exists (status code 410), remove the connection ID from Redis
            if (e.statusCode === 410) {
                console.log(`Stale connection ${connectionId}. Removing...`);
                await sremAsync('connectionIds', connectionId);
            }
        }
    }

    return {};
};
