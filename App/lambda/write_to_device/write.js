const AWS = require('aws-sdk');
const iotdata = new AWS.IotData({endpoint: 'a3ibupythmyonz-ats.iot.us-east-1.amazonaws.com'});

exports.handler = async (event) => {
    if (event.httpMethod === 'OPTIONS') {
        return {
            statusCode: 200,
            body: JSON.stringify('OK'),
        };
    }
    let body = JSON.parse(event.body);
    const params = {
        topic: body.topic,
        payload: JSON.stringify({ message: body.message }),
        qos: 0
    };

    try {
        await iotdata.publish(params).promise();
        const response = {
            statusCode: 200,
            body: JSON.stringify('Message sent:' + body.message),
        };
        return response;
    } catch (err) {
        console.log(err);
        const response = {
            statusCode: 500,
            body: JSON.stringify('An error occurred'),
        };
        return response;
    }
};
