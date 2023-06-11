const AWS = require('aws-sdk');
const redis = require('redis');
const { promisify } = require('util');

const apigwManagementApi = new AWS.ApiGatewayManagementApi({
    apiVersion: '2018-11-29',
    endpoint: process.env.APIG_ENDPOINT
});

// Create Redis client and promisify the get function
const client = redis.createClient({
    host: process.env.REDIS_HOST, // Redis hostname
    port: process.env.REDIS_PORT, // Redis port
    password: process.env.REDIS_PASSWORD // Redis password
});
const getAsync = promisify(client.get).bind(client);

exports.handler = async (event, context) => {
    console.log("Received event: ", event);

    const payload = event;
    console.log("Parsed payload: ", payload);

    if (typeof payload.button !== 'boolean') {
        console.log('Received message does not contain "button" attribute with boolean value');
        return { statusCode: 400, body: 'Invalid message format' };
    }

    // Get the connection ID from Redis
    const connectionId = await getAsync('connectionId');
	console.log("Retreived connectionId from Redis: ", connectionId);

    // This is the part where we send a message to the WebSocket connection
    await apigwManagementApi.postToConnection({
        ConnectionId: connectionId,
        Data: JSON.stringify(payload)
    }).promise();

    return {};
};

