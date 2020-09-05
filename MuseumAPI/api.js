var express = require('express');
var app = express();
var ttn = require('ttn');
const { DataClient } = require('ttn/dist/data');
var awsIot = require('aws-iot-device-sdk');
const MongoClient = require('mongodb').MongoClient;
const assert = require('assert');


//TTN params  https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html for doc
//https://www.thethingsnetwork.org/docs/applications/nodejs/api.html
var appID = "env_stat";
var accessKey = "ttn-account-v2.-YKpgPfGoBOeCIURbXDcCA_0nGg_DMwFKveFfiUx2bs";
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
const uri = "mongodb+srv://dbUser:dbUser@cluster0-sa1g1.mongodb.net/SmartMuseum?retryWrites=true&w=majority";//"mongodb+srv://dbUser:dbUser@cluster0-sa1g1.mongodb.net/test?retryWrites=true&w=majority";

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

app.get('/testDB/', async  (req, res) => {
    
    try{  
        let r =await getStats("1","2020-09-04T15:42:58.328454247Z"); //getStats(room, start date, end date)
        console.log(r);
        res.type('application/json');
        res.status(200);
        res.send(JSON.stringify(r));
    }
    catch{
        res.status(404).send({});
    }
    
});


//THIS MUST BE THE LAST ROUTE
app.use(function(req, res, next) {
    res.status(404).send('Sorry cant find that!');
});

app.listen(3000, function () {
    console.log('Museum Middleware listening on port 3000!');
});


/*

CODE NOT BELONGING TO THE API i.e. ttn/mqtt routines

*/

ttn.data(appID, accessKey)
  .then(function (client) {
    console.log("Created client");
    //console.log(client);  
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      //Simulation payload: 31 3A 31 2C 32 3A 34 34 2E 37 30 2C 33 3A 34 34 2E 38 34 2C 34 3A 34 34 2E 38 34 2C 35 3A 2C 36 3A 31 33 30 33 2C 37 3A 31 33 30 33
      let msg = msg2json(payload);
      msg2db(msg);
    });
    console.log("registered uplink callback");
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })


//transform lora space saving messages into json format
var msg2json =  (payload) =>{

  let data = payload.payload_raw.toString();
  let new_msg={};

  data = data.split(',');
  data.forEach(el=>{
    e = el.split(":");
    e[1]===""?e[1]="0":e[1]=e[1];

    switch (e[0]) {
      case "1":
        new_msg['room']=e[1]
        break;
      case "2":
        new_msg['Tem_avg']=e[1]
        break;
      case "3":
        new_msg['Tem_min']=e[1]
        break;
      case "4":
        new_msg['Tem_max']=e[1]
        break;
      case "5":
        new_msg['Hum_avg']=e[1]
        break;
      case "6":
        new_msg['Hum_min']=e[1]
        break;
      case "7":
        new_msg['Hum_max']=e[1]
        break;
      default:
        console.error(`Error, code:${e[0]}\tvalue:${e[1]}` );
    }
  });
  new_msg['datetime']=new Date(payload.metadata.time);
  new_msg['realtime']=false;
  console.log(new_msg);
  return new_msg;
}

//insert one room data into the db
var msg2db = async (room)=>{
  try{
    
    const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });

    // Use connect method to connect to the Server
    await client.connect() 
    console.log("Connected successfully to server");
    const db = client.db("SmartMuseum");
    const col =db.collection("Rooms"); 
    await col.insertOne(room);

    await client.close();
  } 
  catch(e){
    console.error(e);
  }
}

//insert one room data into the db
var getStats = async (room,start_date,end_date)=>{
    let result;
    try{
        const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });

        // Use connect method to connect to the Server
        await client.connect(); 
        console.log("Connected successfully to server");
        const db = client.db("SmartMuseum");
        const col = db.collection("Rooms");
        result = await col.find({room:room,datetime:{ $gte:new Date(start_date)}}).toArray();
        console.log(result);
        await client.close();
        console.log(result.length);
        return result;
    } 
    catch(e){
        console.error(e);
    }
}