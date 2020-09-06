var data;
var flagRealTime = true;
var minT1
var myIpAddr =
    $(document).ready(async function () {



        var idRoom = sessionStorage.getItem('idRoom');
        $('#idRoom').text("Room " + idRoom);



        console.log(idRoom);


        if (idRoom != 0 && idRoom != null) {
            //LOAD DATA FROM API





            //REAL TIME 
            interval = setInterval(() => {
                APICall(url = `http://127.0.0.1:3000/stats/${idRoom}`, method = 0, data = {}) // 1: POST, 0: GET
                    .then((response) => {

                        try { //update the status iff it is available online
                            console.log(response)

                            $('#Tem_max').text(response[0]["Tem_max"]);
                            $('#Tem_min').text(response[0]["Tem_min"]);
                            $('#Tem_avg').text(response[0]["Tem_avg"]);
                            $('#Hum_min').text(response[0]["Hum_min"]);
                            $('#Hum_max').text(response[0]["Hum_max"]);
                            $('#Hum_avg').text(response[0]["Hum_avg"]);







                        } catch (error) {
                            console.log(error);
                        }

                    })
                    .catch(function (e) { console.log(`error ${e}`); }); // r={}

            }, 5000);

        } else {

            // GET ALL DATA TO PUT IN THE  MAIN ROOM
            // FIRST TIME NO DELAY
            APICall(url = `http://127.0.0.1:3000/stats`, method = 0, data = {})


                .then((response) => {

                    try {
                        // SET DATA TO HTML 
                        console.log(response.room1[0]["room"]);
                       
                       

                        $('#Tem_max').text(response.room1[0]["Tem_max"]);
                        $('#Tem_min').text(response.room1[0]["Tem_min"]);
                        $('#Tem_avg').text(response.room1[0]["Tem_avg"]);
                        $('#Hum_min').text(response.room1[0]["Hum_min"]);
                        $('#Hum_max').text(response.room1[0]["Hum_max"]);
                        $('#Hum_avg').text(response.room1[0]["Hum_avg"]);

                        $('#Tem_max2').text(response.room2[0]["Tem_max"]);
                        $('#Tem_min2').text(response.room2[0]["Tem_min"]);
                        $('#Tem_avg2').text(response.room2[0]["Tem_avg"]);
                        $('#Hum_min2').text(response.room2[0]["Hum_min"]);
                        $('#Hum_max2').text(response.room2[0]["Hum_max"]);
                        $('#Hum_avg2').text(response.room2[0]["Hum_avg"]);

                        $('#Tem_max3').text(response.room3[0]["Tem_max"]);
                        $('#Tem_min3').text(response.room3[0]["Tem_min"]);
                        $('#Tem_avg3').text(response.room3[0]["Tem_avg"]);
                        $('#Hum_min3').text(response.room3[0]["Hum_min"]);
                        $('#Hum_max3').text(response.room3[0]["Hum_max"]);
                        $('#Hum_avg3').text(response.room3[0]["Hum_avg"]);


                    } catch (error) {
                        console.log(error);
                    }

                }) // r={}
                .catch(function (e) { console.log(`error ${e}`); });
                
            interval = setInterval(() => {

                APICall(url = `http://127.0.0.1:3000/stats`, method = 0, data = {})


                    .then((response) => {

                        try {
                            // SET DATA TO HTML 
                            console.log(response);
                            console.log(response[0]["Room"]);
                            console.log(response[1]["Tem_max"].toString());

                            $('#Tem_max').text(response[2]["Tem_max"]);
                            $('#Tem_min').text(response[2]["Tem_min"]);
                            $('#Tem_avg').text(response[2]["Tem_avg"]);
                            $('#Hum_min').text(response[2]["Hum_min"]);
                            $('#Hum_max').text(response[2]["Hum_max"]);
                            $('#Hum_avg').text(response[2]["Hum_avg"]);

                            $('#Tem_max2').text(response[1]["Tem_max"]);
                            $('#Tem_min2').text(response[1]["Tem_min"]);
                            $('#Tem_avg2').text(response[1]["Tem_avg"]);
                            $('#Hum_min2').text(response[1]["Hum_min"]);
                            $('#Hum_max2').text(response[1]["Hum_max"]);
                            $('#Hum_avg2').text(response[1]["Hum_avg"]);

                            $('#Tem_max3').text(response[0]["Tem_max"]);
                            $('#Tem_min3').text(response[0]["Tem_min"]);
                            $('#Tem_avg3').text(response[0]["Tem_avg"]);
                            $('#Hum_min3').text(response[0]["Hum_min"]);
                            $('#Hum_max3').text(response[0]["Hum_max"]);
                            $('#Hum_avg3').text(response[0]["Hum_avg"]);


                        } catch (error) {
                            console.log(error);
                        }

                    }) // r={}
                    .catch(function (e) { console.log(`error ${e}`); });
            }, 3600000);



        }
    });
$("#roomOne").click(function () {

    sessionStorage.setItem('idRoom', '1');
    //var data = sessionStorage.getItem('idRoom');
    //console.log(data);
    // window.location.replace("room.html");
    window.location.href = "room.html";
});
$("#roomTwo").click(function () {

    sessionStorage.setItem('idRoom', '2');
    //var data = sessionStorage.getItem('idRoom');
    //console.log(data);
    // window.location.replace("room.html");
    window.location.href = "room.html";
});
$("#roomThree").click(function () {

    sessionStorage.setItem('idRoom', '3');
    //var data = sessionStorage.getItem('idRoom');
    //console.log(data);
    // window.location.replace("room.html");
    window.location.href = "room.html";
});
$("#mainRoom").click(function () {

    sessionStorage.setItem('idRoom', '0');
    //var data = sessionStorage.getItem('idRoom');
    //console.log(data);
    // window.location.replace("room.html");
    window.location.href = "/";
});




function addMT(id) {
    var newCount = parseInt($(id).text()) + 1;

    var idRoom = sessionStorage.getItem('idRoom');
    console.log(idRoom);

    msgText = {

        Room: idRoom,
        v: newCount,
        mt: "Tem_max"
    }

    APICall(url = `http://127.0.0.1:3000/SETthresholds/${idRoom}`, method = 1, data = msgText) // 1: POST, 0: GET
        .then((r) => { console.log(r); }) // r={}
        .catch(function (e) { console.log(`error ${e}`); });

    $(id).text(newCount);
}


function substract(id) {
    var newCount = parseInt($(id).text()) - 1;
    $(id).text(newCount);
}


async function APICall(url = '', method = 0, data = {}) {
    var response;
    if (method == 1) {
        // Default options are marked with *
        response = await fetch(url, {
            method: 'POST',
            mode: 'cors',
            cache: 'no-cache',
            credentials: 'same-origin',
            headers: {
                'Content-Type': 'application/json'
            },
            redirect: 'follow',
            referrerPolicy: 'no-referrer',
            body: JSON.stringify(data) // body data type must match "Content-Type" header
        });
    }
    if (method == 0) {
        // Default options are marked with *
        response = await fetch(url, {
            method: 'GET',
            mode: 'cors',
            cache: 'no-cache',
            credentials: 'same-origin',
            headers: {
                'Content-Type': 'application/json'
            },
            redirect: 'follow',
            referrerPolicy: 'no-referrer',

        });
    }
    return response.json(); // parses JSON response into native JavaScript objects
}