const AWS = require('aws-sdk');
const iotdata = new AWS.IotData({endpoint: 'a3ibupythmyonz-ats.iot.us-east-1.amazonaws.com'});

exports.handler = async (event) => {
    if (event.httpMethod === 'OPTIONS') {
        return {
            statusCode: 200,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Credentials': true,
                'Access-Control-Allow-Methods': 'POST',
                'Access-Control-Allow-Headers': 'Content-Type',
            },
            body: JSON.stringify('OK'),
        };
    }

    const params = {
        topic: 'esp8266/sub',
        payload: JSON.stringify(event),
        qos: 0
    };

    try {
        await iotdata.publish(params).promise();
        const response = {
            statusCode: 200,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Credentials': true,
            },
            body: JSON.stringify('Message sent'),
        };
        return response;
    } catch (err) {
        console.log(err);
        const response = {
            statusCode: 500,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Credentials': true,
            },
            body: JSON.stringify('An error occurred'),
        };
        return response;
    }
};
