var ttn = require('ttn');
const { DataClient } = require('ttn/dist/data');

//TTN params  https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html for doc
//https://www.thethingsnetwork.org/docs/applications/nodejs/api.html
var appID = "env_stat";
var accessKey = "ttn-account-v2.-YKpgPfGoBOeCIURbXDcCA_0nGg_DMwFKveFfiUx2bs";
console.log("Started")

ttn.data(appID, accessKey)
  .then(function (client) {
    console.log("Created client");
    //console.log(client);  
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      //Simulation payload: 31 3A 31 2C 32 3A 34 34 2E 37 30 2C 33 3A 34 34 2E 38 34 2C 34 3A 34 34 2E 38 34 2C 35 3A 2C 36 3A 31 33 30 33 2C 37 3A 31 33 30 33
      let msg = msg2json(payload);
      
      
      msg2db(msg);

      console.log("trying uplink to: "+ devID);
      client.send(devID,new Buffer(JSON.stringify({realtime:true}),"hex"),1,false,"replace");

    });
    console.log("registered uplink callback");
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })



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
  new_msg['datetime']=payload.metadata.time;
  new_msg['realtime']=false;
  console.log(new_msg);
  return new_msg;
}

var msg2db = async (msg)=>{
  try{
    //let urii ="mongodb://localhost:27017";
    //let urii= "mongodb+srv://dbUser:dbUser@cluster0-sa1g1.mongodb.net/SmartMuseum?retryWrites=true&w=majority"


    // Use connect method to connect to the Server
    await client.connect( async err => {
      assert.equal(null, err);
      console.log("Connected successfully to server");
      const db = client.db("SmartMuseum");
      //collection.insertOne(room);
      await db.collection("Rooms").insertOne(room, (err, r) => {
        assert.equal(null, err);
        assert.equal(1, r.insertedCount);
      });
        
    });

    client.close();
  } 
  catch(e){
    console.error(e);
  }
}