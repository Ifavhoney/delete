// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    //Search the list of files of uploads

    //Put that file into the c function






    let _Home = [{ name: "File Log Pannel", href: "#fileLog" },
    { name: "SVG View Pannel", href: "" },
    { name: "Additional functionality", href: "/fileLog" }];

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
            //Use only for send
            success: function (data) {


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
                    //do something if clicked remove state of screen ... or not cuz they might refresh
                    //look at the href if
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

            let table_tr = null;

            let table = document.getElementById("fileLogTable");


            for (let i = 0; i < data.listFiles.length; i++) {
                const element = data.listFiles[i];

                $.ajax({
                    type: 'get',            //Request type
                    dataType: 'json',       //Data type - we will use JSON for almost everything 
                    url: '/getSVGParser',   //The server endpoint we are connecting to
                    //sends back to app.js 
                    data: {
                        fileName: element,


                    },
                    //recieves from app.js44
                    success: function (data) {
                        //Turns into an object
                        let snapshot = JSON.parse(data.jsonData);

                        if (i == 0) {
                            table_tr = document.createElement("tr");
                            table.appendChild(table_tr);
                        }
                        let table_td1 = document.createElement("td");
                        let table_td1_doc = table_tr.appendChild(table_td1);

                        let table_img = document.createElement("img");
                        table_img.src = "../uploads/" + element;

                        let table_td1_href = document.createElement("a");
                        table_td1_href.href = element;
                        table_td1_href.download = element;

                        table_img_doc = table_td1_doc.
                            appendChild(table_td1_href).
                            appendChild(table_img);

                        let table_td2 = document.createElement("td");
                        let table_td2_href = document.createElement("a");
                        table_td2_href.href = element;
                        table_td2_href.download = element
                        table_td2_href.innerHTML = element;
                        let table_td2_doc = table_tr.appendChild(table_td2);
                        table_td2_doc.appendChild(table_td2_href);




                        //     let temp = table.createElement("td");


                    },
                    fail: function (data) {
                    }

                });



            }

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
});


