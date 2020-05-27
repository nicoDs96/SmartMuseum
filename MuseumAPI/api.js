var express = require('express');
var app = express();
var ttn = require('ttn');
var awsIot = require('aws-iot-device-sdk');
const MongoClient = require('mongodb').MongoClient;


//TTN params  https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html for doc
var appID = "smart_museum"
var accessKey = "ttn-account-v2.UwQYpQqxdt1Lz5jaNZyk94qLMy0hLZb4XzCDvRqjYD4"
ttn.data(appID, accessKey)
  .then(function (client) {
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      console.log(payload)
    })
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })


//AWS params
var device = awsIot.device({
    keyPath: '/home/pi/deviceSDK/certs/private.pem.key',
    certPath: '/home/pi/deviceSDK/certs/certificate.pem.crt',
    caPath: '/home/pi/deviceSDK/certs/caCert.crt',
    clientId: '<YOUR THING NAME>',
    host: 'end point '
});
/*
https://github.com/aws/aws-iot-device-sdk-js

device.on(‘connect’, function() {    device.subscribe(‘LED’)   device.publish('topic_2', JSON.stringify({ test_data: 1})); });

device.on(‘message’, function(topic, payload) {    
    var payload = JSON.parse(payload.toString());
    console.log();    
    if(topic == ''){   } else {    }
}
*/

//MongoDB dbUser dbUser
// to connect to ATLAS without error configure the whitelist: https://docs.atlas.mongodb.com/tutorial/whitelist-connection-ip-address/
const uri = "mongodb+srv://dbUser:<password>@cluster0-sa1g1.mongodb.net/test?retryWrites=true&w=majority";
const client = new MongoClient(uri, { useNewUrlParser: true });
client.connect(err => {
  const collection = client.db("test").collection("devices");
  // perform actions on the collection object
  client.close();
});





app.use(express.json()); // for parsing application/json

app.get('/test', function (req, res) {
    testRes = {Hello:'World'};
    res.send(JSON.stringify(testRes));
});

app.get('/state/:clientId', function (req, res) {
    if( clientsActivity.has( req.params.clientId) ){
        message = {
            activity: clientsActivity.get(req.params.clientId)
        }
        res.type('application/json');
        res.status(200);
        res.send(JSON.stringify(message));
    }
    else{
        res.status(404).send({});
    }
    
});


app.post('/readings', function (req, res) {
    try {
        
    }
    catch (e) {
        console.error("Error : " + e);
        res.status(500).send("500 - Internal Error");
    }

});


//THIS MUST BE THE LAST ROUTE
app.use(function(req, res, next) {
    res.status(404).send('Sorry cant find that!');
});

app.listen(3000, function () {
    console.log('Museum Middleware listening on port 3000!');
});