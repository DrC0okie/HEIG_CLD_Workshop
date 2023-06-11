const redis = require('redis');
const { promisify } = require('util');

// Create Redis client and promisify the set and get functions
const client = redis.createClient({
    host: process.env.REDIS_HOST, // Redis hostname
    port: process.env.REDIS_PORT, // Redis port
    password: process.env.REDIS_PASSWORD // Redis password
});
const setAsync = promisify(client.set).bind(client);
const getAsync = promisify(client.get).bind(client); // Promisify the get function

exports.handler = async (event, context) => {
    console.log("Received event: ", event);
    const connectionId = event.requestContext.connectionId;
    console.log("Stored connection ID: ", connectionId);

    // Store the connection ID in Redis
    await setAsync('connectionId', connectionId);

    // Get the stored value from Redis to verify
    const storedValue = await getAsync('connectionId');
    console.log("Retrieved stored connection ID from Redis: ", storedValue);

    return {};
};
