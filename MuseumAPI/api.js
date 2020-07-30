var express = require('express');
var app = express();
var ttn = require('ttn');
var awsIot = require('aws-iot-device-sdk');
const MongoClient = require('mongodb').MongoClient;


//TTN params  https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html for doc
//https://www.thethingsnetwork.org/docs/applications/nodejs/api.html
var appID = "smart_museum";
var accessKey = "ttn-account-v2.UwQYpQqxdt1Lz5jaNZyk94qLMy0hLZb4XzCDvRqjYD4";
//AWS params
var device = awsIot.device({
    keyPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/station.private.key',
    certPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/station.cert.pem',
    caPath: '/home/nicods/Scrivania/SmartMuseum/MuseumAPI/AWSCerts/root-CA.crt',
    clientId: 'client',
    host: 'a1czszdg9cjrm-ats.iot.us-east-1.amazonaws.com'
});
  
// https://github.com/aws/aws-iot-device-sdk-js  see test/aws_iot_test.js for working example


//MongoDB dbUser dbUser
// to connect to ATLAS without error configure the whitelist: https://docs.atlas.mongodb.com/tutorial/whitelist-connection-ip-address/
const uri = "mongodb+srv://dbUser:dbUser@cluster0-sa1g1.mongodb.net/test?retryWrites=true&w=majority";
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

//get last hour stats of all the rooms (MAIN PAGE STATS)
app.get('/stats/', function (req, res) {
    if(  Number(req.params.clientId) > 0 ){
        let room_nr = Number(req.params.clientId);
        /*TODO
        query the db and send the stats of the given room (historical and eventually live if available)
        */
        res.type('application/json');
        res.status(200);
        res.send(JSON.stringify(message));
    }
    else{
        res.status(404).send({});
    }
    
});

app.get('/stats/:roomId', function (req, res) {
    if(  Number(req.params.clientId) > 0 ){
        let room_nr = Number(req.params.clientId);
        /*TODO
        query the db and send the stats of the given room (historical and eventually live if available)
        */
        res.type('application/json');
        res.status(200);
        res.send(JSON.stringify(message));
    }
    else{
        res.status(404).send({});
    }
    
});

app.post('/thresholds/:roomId', function (req, res) {
    try {

        if(  Number(req.params.clientId) > 0 ){
            let room_nr = Number(req.params.clientId);
            
            /*TODO
            - get body response
            - update thresholds
            - notify actuators
            */
            res.type('application/json');
            res.status(200);
            res.send(JSON.stringify(message));
        }
        else{
            res.status(404).send({});
        }
        
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
