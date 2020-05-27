const MongoClient = require('mongodb').MongoClient;


const uri = "mongodb+srv://dbUser:dbUser@cluster0-sa1g1.mongodb.net/test?retryWrites=true&w=majority";
const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
client.connect(err => {
  const collection = client.db("test").collection("devices");
  // perform actions on the collection object
  
  client.close();
});

