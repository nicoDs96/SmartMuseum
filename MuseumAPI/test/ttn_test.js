var ttn = require('ttn');

//TTN params  https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html for doc
//https://www.thethingsnetwork.org/docs/applications/nodejs/api.html
var appID = "smart_museum";
var accessKey = "ttn-account-v2.UwQYpQqxdt1Lz5jaNZyk94qLMy0hLZb4XzCDvRqjYD4";
console.log("Started")

ttn.data(appID, accessKey)
  .then(function (client) {
    console.log("Created client");
    console.log(client);  
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      console.log(payload)
    });
    console.log("registered uplink callback");
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })