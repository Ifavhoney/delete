// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {

    /*
    let x = {
        "stylist_1": [{
            "Address": "1 Stn Main",
            "Phone": "403-990-9033"
        }, {
            "Address": "474 Cirrus Rd",
            "Phone": "403-995-3243"
        }, {
            "Address": "1591 Stn St",
            "Phone": "403-982-8893"
        }],
        "stylist_2": [{
            "Address": "219 Welch Blvd",
            "Phone": "587-436-3171"
        }, {
            "Address": "374 Main Rd",
            "Phone": "587-315-9431"
        }, {
            "Address": "564 Main Rd",
            "Phone": "403-938-9983"
        }]
    }
    */

    let _Home = ["File Log Pannel", "SVG View Pannel", "Additional functionality"];
    let _FileLog;

    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '" + data.foo + "' from server");
            //We write the object to the console to show that the request was successful
            console.log("first ajax calls " + data);

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
    $.ajax(

        {
            type: 'get',
            dataType: 'html',
            url: '/',
            data: {
            },
            success: function (data) {
                //Type object of <Key, object>
                /*
                let addresses = [];
                for (let i = 0; i < data.stylist_1.length; i++) {
                    //Adding to array
                    addresses.push(data.stylist_1[i]);
                    let temp = document.createElement("div");
                    temp.innerHTML = addresses[i].Address;
                    document.getElementById("home").appendChild(temp);
                }
                console.log("addresses")
                */




            },
            fail: (error) => {
                console.log("error!!!");
            }

        });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function (e) {
        $('#blah').html("Form has data: " + $('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getListOfFiles',   //The server endpoint we are connecting to
        data: {
        },
        success: function (data) {
            //  console.log("you should see this in inspect");
            // console.log(data.listFiles);
        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
});


