// Put all onload AJAX calls here, and event listeners
//Called anytime there's a reload


$(document).ready(function () {
    //Search the list of files of uploads

    //Put that file into the c function






    let _Home = [{ name: "File Log Pannel", href: "#fileLog" },
    { name: "SVG View Pannel", href: "#fileDropDown" },
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
                if (data.text != null) {
                    console.log(text);
                }
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
    /*
    $('#upload').submit(function (e) {
        // e.preventDefault(); //Prevents from refreshing after the submit so i can show the information!
        $('#blah').html("Form has data: " + $('#entryBox').val());
        //Pass data to the Ajax call, so it gets passed to the server

        console.log("??");



        $.ajax({
            type: 'get',
            dataType: 'html',
            url: '/',   //The server endpoint we are connecting to
            data: {
            },
            success: function (data) {
                


            },
            fail: function (data) {

            }
        });
    });
    */


    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/fileDropDown',   //The server endpoint we are connecting to
        data: {
        },
        success: function (data) {
            let dropDown = document.getElementById("panelDropDown");
            if (data.files != null) {
                for (let i = 0; i < data.files.length; i++) {
                    let a = document.createElement("a");
                    a.href = "#";
                    a.className = "dropdown-item";
                    a.text = data.files[i];
                    dropDown.appendChild(a);

                }


            }
        },
        fail: function (data) {

        }
    })


    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getViewParser:name',   //The server endpoint we are connecting to
        data: {

        },
        success: function (data) {
            $('.dropdown-item').click(function () {
                var selText = $(this).text();
                console.log(selText);
            });
        },
        fail: function (data) {

        }
    })






    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getListOfFiles',   //The server endpoint we are connecting to
        data: {
        },
        success: function (data) {
            //  console.log("you should see this in inspect");
            let table_tr = null;
            console.log(data.cons);
            let table = document.getElementById("fileLogTable");

            if (data.length == 0) {

                table_tr = document.createElement("tr");
                table.appendChild(table_tr);

                let table_td1 = document.createElement("td");
                table_td1.innerHTML = "NO FILES"
                table_tr.appendChild(table_td1);
                console.log("no files")

            }
            else {


                for (let i = 0; i < data.files.length; i++) {
                    const element = data.files[i];
                    const elementSize = data.sizes[i];


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
                            if (data == null) {
                                console.log("invalid file")

                            }
                            //Turns into a javascript object
                            let snapshot = JSON.parse(data.jsonData);
                            //console.log(snapshot);
                            table_tr = document.createElement("tr");
                            table.appendChild(table_tr);

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
                            table_td2_doc = table_tr.appendChild(table_td2).appendChild(table_td2_href);

                            let table_td3 = document.createElement("td");
                            table_td3.innerHTML = elementSize;
                            table_tr.appendChild(table_td3);

                            let table_td4 = document.createElement("td");
                            table_td4.innerHTML = snapshot["numRect"];
                            table_tr.appendChild(table_td4);

                            let table_td5 = document.createElement("td");
                            table_td5.innerHTML = snapshot["numCirc"];
                            table_tr.appendChild(table_td5);

                            let table_td6 = document.createElement("td");
                            table_td6.innerHTML = snapshot["numPaths"];
                            table_tr.appendChild(table_td6);

                            let table_td7 = document.createElement("td");
                            table_td7.innerHTML = snapshot["numGroups"];
                            table_tr.appendChild(table_td7);


                            //rects


                        },
                        fail: function (data) {
                        }

                    });



                }
            }
        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
});



