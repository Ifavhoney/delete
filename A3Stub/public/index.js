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

    let _Home = [{ name: "File Log Pannel", href: "/fileLog" },
    { name: "SVG View Pannel", href: "/fileLog" },
    { name: "Additional functionality", href: "/fileLog" }];
    let _FileLog = {
        thead: [{ image: "image", width: "20%" }, { fileName: "File name", width: "20%" }
            , { fileSize: "File size", width: "10%" }, { numRect: "Number of rectangles", width: "10%" },
        { numRect: "Number of circles", width: "10%" },
        { numRect: "Number of paths", width: "10%" },
        { numRect: "Number of groups", width: "10%" },
        ],
        tBody: [{ image: "../uploads" }, { fileName: "File name" }
            , { fileSize: "File size" }, { numRect: "Number of rectangles" },
        { numRect: "Number of circles" },
        { numRect: "Number of paths" },
        { numRect: "Number of groups" },
        ],
    };

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
            url: '/fileLog',
            data: {
            },
            //Use only for send
            success: function (data) {
                var link = document.getElementById('home');
                link.style.display = 'none'; //or
                let tHeadDoc = null;
                //Type object of <Key, object>
                for (let i = 0; i < _FileLog.thead.length; i++) {
                    const element = _FileLog[i];
                    let head = document.getElementById("fileLog");
                    if (i == 0) {
                        let temp = document.createElement("div");
                        temp.className = "center-screen"
                        let tempDoc = head.appendChild(temp);

                        let table = document.createElement("table");
                        table.className = "table table-bordered table-lg bg-light";
                        let tableDoc = tempDoc.appendChild(table);

                        let tHead = document.createElement("thead");
                        tHeadDoc = tableDoc.appendChild(tHead);

                    }
                    //get tab
                    let tr = document.createElement("tr");
                    let trDoc = tHeadDoc.appendChild(tr);
                    let th = document.createElement("th");
                    th.style = element


                }

                console.log("Hello");
            },
            fail: (error) => {

                console.log("error!!!");
            }

        });


    $.ajax(

        {
            type: 'get',
            dataType: 'html',
            url: '/',
            data: {
            },
            //Use only for send
            success: function (data) {

                var link = document.getElementById('home');
                let tBodyDoc = null;
                //Type object of <Key, object>
                for (let i = 0; i < _Home.length; i++) {
                    const element = _Home[i];
                    let head = document.getElementById("home");
                    if (i == 0) {
                        let temp = document.createElement("div");
                        temp.className = "center-screen"
                        let tempDoc = head.appendChild(temp);

                        let table = document.createElement("table");
                        table.className = "table table-striped table-dark";
                        let tableDoc = tempDoc.appendChild(table);

                        let tbody = document.createElement("tbody");
                        tBodyDoc = tableDoc.appendChild(tbody);

                    }
                    //get tab
                    let tr = document.createElement("tr");
                    tr.setAttribute("class", "d-flex justify-content-center p-5");
                    let trDoc = tBodyDoc.appendChild(tr);
                    let td = document.createElement("td");
                    let tdDoc = trDoc.appendChild(td);
                    let a = document.createElement("a");
                    a.href = element.href;
                    //do something if clicked remove state of screen
                    a.innerHTML = element.name;

                    tdDoc.appendChild(a);

                }


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


