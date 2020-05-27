var awsIot = require('aws-iot-device-sdk');
//
// Replace the values of '<YourUniqueClientIdentifier>' and '<YourCustomEndpoint>'
// with a unique client identifier and custom host endpoint provided in AWS IoT.
// NOTE: client identifiers must be unique within your AWS account; if a client attempts 
// to connect with a client identifier which is already in use, the existing 
// connection will be terminated.
//
var device = awsIot.device({
  keyPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/station.private.key',
  certPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/station.cert.pem',
    caPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/root-CA.crt',
  clientId: 'client',
      host: 'a1czszdg9cjrm-ats.iot.us-east-1.amazonaws.com'
});

//
// Device is an instance returned by mqtt.Client(), see mqtt.js for full
// documentation.
//
device
  .on('connect', function() {
    console.log('connect');
    device.subscribe('topic_1');
    device.publish('topic_1', JSON.stringify({ test_data: "working"}));
  });

device
  .on('message', function(topic, payload) {
    console.log('message', topic, payload.toString());
  });