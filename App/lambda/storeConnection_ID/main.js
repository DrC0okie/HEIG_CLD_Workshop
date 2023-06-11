const redis = require('redis');
const { promisify } = require('util');

// Create Redis client and promisify the set and get functions
const client = redis.createClient({
    host: process.env.REDIS_HOST, // Redis hostname
    port: process.env.REDIS_PORT, // Redis port
    password: process.env.REDIS_PASSWORD // Redis password
});

const saddAsync = promisify(client.sadd).bind(client);

exports.handler = async (event, context) => {
    console.log("Received event: ", event);
    const connectionId = event.requestContext.connectionId;
    console.log("Stored connection ID: ", connectionId);

    // Store the connection ID in Redis
    await saddAsync('connectionIds', connectionId);

    return {};
};
