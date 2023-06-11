const redis = require('redis');
const { promisify } = require('util');

// Create Redis client and promisify the del function
const client = redis.createClient({
    host: process.env.REDIS_HOST,
    port: process.env.REDIS_PORT,
    password: process.env.REDIS_PASSWORD
});

const sremAsync = promisify(client.srem).bind(client);

exports.handler = async (event, context) => {
    console.log("Received disconnect event: ", event);
    const connectionId = event.requestContext.connectionId;

    // Delete the connection ID from Redis
    await sremAsync('connectionIds', connectionId);

    return {};
};
