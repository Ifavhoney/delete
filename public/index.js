// Put all onload AJAX calls here, and event listeners
//Called anytime there's a reload

//Create a file

//Get data from user

//then 





function getClick(obj) {

    alert(obj.download);
    $.ajax({
        type: 'post',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/trackDownloads',   //The server endpoint we are connecting to
        data: { fileName: obj.download },
        success: function (data) {

        },
        fail: function (data) {

        }
    });


}


$("#editCircx").on('click', (function (e) {
    let x = document.getElementById("cx").value;
    let y = document.getElementById("cy").value;
    let r = document.getElementById("r").value;
    let u = document.getElementById("c_units").value;
    let fileName = document.getElementById("imagePanel").getAttribute("alt");
    let className = document.getElementById("imagePanel").className;
    //regex -1 for index
    let index = (className.match(/\d+/g)[0]) - 1;


    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/editCirc',   //The server endpoint we are connecting to
        data: {
            cx: x,
            cy: y,
            r: r,
            units: u,
            fileName: fileName,
            index: index

        },
        success: function (data) {
            alert(data.message);

            //   alert("hi");
            /*
            if (title.length < 1 || description.length < 1) {
                alert("Too short!")
            }
            else {
                alert("Success!")
            }
            */




        },
        fail: function (data) {

        }
    });



    e.preventDefault();

})

);

$("#editRectx").on('click', (function (e) {
    let x = document.getElementById("x").value;
    let y = document.getElementById("y").value;

    let width = document.getElementById("width").value;
    let height = document.getElementById("height").value;

    let u = document.getElementById("r_units").value;
    let fileName = document.getElementById("imagePanel").getAttribute("alt");
    let className = document.getElementById("imagePanel").className;
    //regex -1 for index
    let index = (className.match(/\d+/g)[0]) - 1;

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/editRect',   //The server endpoint we are connecting to
        data: {
            x: x,
            y: y,
            width: width,
            height: height,
            units: u,
            fileName: fileName,
            index: index



        },
        success: function (data) {
            //   alert("hi");
            /*
            if (title.length < 1 || description.length < 1) {
                alert("Too short!")
            }
            else {
                alert("Success!")
            }
            */




        },
        fail: function (data) {

        }
    });



    e.preventDefault();

})

);



$('#createSVG').submit(function (e) {
    console.log("createSVG")

    $.ajax(

        {
            type: 'post',
            dataType: 'json',
            url: '/downloadFile',
            data: {
            },
            //Use only for send
            success: function (data) {
                //   console.log("Gets here")


            },
            fail: (error) => {

                //    console.log("error!!!");
            }

        });
    // e.preventDefault();

});


$('#formLogin').submit(function (e) {

    $.ajax(

        {
            type: 'get',
            dataType: 'json',
            url: '/storeFiles',
            data: {
            },
            //Use only for send
            success: function (data) {
                console.log("Gets here")

                alert(data.message);
                console.log(data);


                if (data.text != null) {
                    console.log(text);
                }

            },
            fail: (error) => {

                console.log("error!!!");
            }

        });
    // e.preventDefault();

});



$(document).ready(function () {
    //Search the list of files of uploads

    //Put that file into the c function




    $.ajax(

        {
            type: 'get',
            dataType: 'html',
            url: '/storeFiles',
            data: {
            },
            //Use only for send
            success: function (data) {

            },
            fail: function (data) {

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

                let body = document.getElementById("body");
                body.style.display = "block";

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






    let _Home = [{ name: "File Log Pannel", href: "#fileLog" },
    { name: "SVG View Pannel", href: "#fileDropDown" },
    { name: "Additional functionality", href: "/fileLog" }];


    //

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
            //Gets the Dropdown to show files, knows through due our call with query 
            let dropDown = document.getElementById("panelDropDown");
            if (data.files != null) {
                for (let i = 0; i < data.files.length; i++) {
                    let a = document.createElement("a");
                    a.href = "#";
                    a.className = "dropdown-item file";

                    a.text = data.files[i];
                    dropDown.appendChild(a);

                }

                let item = " ";
                $('.file').click(function (e) {

                    item = $(this).text();
                    e.preventDefault();

                    document.getElementById("editFileName").style.display = "none";
                    document.getElementById("editRect").style.display = "none";
                    document.getElementById("editCirc").style.display = "none";
                    document.getElementById("editPath").style.display = "none";

                    document.getElementById("editGroup").style.display = "none";
                    //Returns selected item on the dropdown
                    $.ajax({
                        type: 'get',            //Request type
                        dataType: 'json',       //Data type - we will use JSON for almost everything 
                        url: '/getViewParser',   //The server endpoint we are connecting to
                        data: {
                            name: item,
                        },
                        success: function (data) {


                            /*
                            <rect>
                        <circle>
                            <path>
                                <g>
*/
                            let jsonTitle = data.jsonTitle;
                            let jsonDesc = data.jsonDesc;

                            let jsonRect = JSON.parse(data.jsonRectValue);
                            let jsonCirc = JSON.parse(data.jsonCircValue);
                            let jsonPath = JSON.parse(data.jsonPathValue);
                            let jsonGroup = JSON.parse(data.jsonGroupValue);
                            let jsonRectAttrValue = JSON.parse(data.jsonRectAttrValue);
                            let jsonPathAttrValue = JSON.parse(data.jsonPathAttrValue);
                            let jsonGroupAttrValue = JSON.parse(data.jsonGroupAttrValue);
                            let jsonCircAttrValue = JSON.parse(data.jsonCircAttrValue);


                            let titlePanel = document.getElementById("titlePanel")
                            titlePanel.innerHTML = jsonTitle + "</br> ";
                            let descPanel = document.getElementById("descPanel")
                            descPanel.innerHTML = jsonDesc + "</br> ";

                            let editTitleDesc = document.createElement("button");
                            editTitleDesc.id = "showFileName"
                            editTitleDesc.type = "button";
                            editTitleDesc.className = "showFileName btn btn-info";
                            editTitleDesc.innerHTML = "Edit Title/Description (A4)";
                            titlePanel.appendChild(editTitleDesc);

                            descPanel.appendChild(editTitleDesc);


                            let imagePanel = document.getElementById("imagePanel")
                            imagePanel.src = "../uploads/" + item;
                            imagePanel.width = "800"
                            imagePanel.height = "800"
                            imagePanel.alt = item;
                            //  imagePanel.height = "800"


                            let table = document.getElementById("panelDetails");
                            //Returns a type nodelist

                            let nodes = table.getElementsByTagName("tbody");
                            if (nodes.length > 0) {
                                for (var i = 0, len = nodes.length; i != len; ++i) {
                                    nodes[0].parentNode.removeChild(nodes[0]);
                                }
                            }

                            //Iterate through to dynamically add to the list
                            console.log(table.getElementsByTagName("tbody"));

                            for (let index = 0; index < jsonRect.length; index++) {
                                let tBody = document.createElement("tbody");
                                let table_td1 = document.createElement("td");
                                table_td1.innerHTML = "Rectangle " + (index + 1);
                                let table_td2 = document.createElement("td");
                                let units = jsonRect[index]["units"];
                                if (units.length == 0) {
                                    units = "";
                                }
                                table_td2.innerHTML = "Upper left corner: " + "x = " + jsonRect[index]["x"] + units + ", "
                                    + "y = " + jsonRect[index]["y"] + units + "</br>" + "Width: " + jsonRect[index]["w"] + units + ", "
                                    + "Height: " + jsonRect[index]["h"] + units

                                let table_td3 = document.createElement("td");
                                table_td3.innerHTML = jsonRect[index]["numAttr"] + "</br>"
                                let showButton = document.createElement("button");
                                showButton.type = "button";
                                showButton.className = "rectElement btn btn-primary";
                                showButton.innerHTML = "Show Elements";



                                let tbody_doc = table.appendChild(tBody);

                                tbody_doc.appendChild(table_td1);
                                tbody_doc.appendChild(table_td2);
                                let table_td3_doc = tbody_doc.appendChild(table_td3);


                                if (jsonRect[index]["numAttr"] > 0) {
                                    showButton.id = "Rectangle" + (index + 1);
                                    table_td3.appendChild(showButton);

                                }
                                let editButton = document.createElement("button");
                                editButton.id = "Rectangle" + (index + 1);
                                editButton.type = "button";
                                editButton.className = "editRectElement btn btn-info";
                                editButton.innerHTML = "Edit Elements (A4)";
                                table_td3.appendChild(editButton);

                            }

                            for (let index = 0; index < jsonCirc.length; index++) {
                                let tBody = document.createElement("tbody");
                                let table_td1 = document.createElement("td");
                                table_td1.innerHTML = "Circle " + (index + 1);
                                let table_td2 = document.createElement("td");
                                let units = jsonCirc[index]["units"];
                                if (units.length < 2) {
                                    units = "";
                                }
                                table_td2.innerHTML = "Centre: " + "x = " + jsonCirc[index]["cx"] + units + ", "
                                    + "y = " + jsonCirc[index]["cy"] + units + ", " + "radius = " + jsonCirc[index]["r"] + units

                                let table_td3 = document.createElement("td");
                                table_td3.innerHTML = jsonCirc[index]["numAttr"] + "</br>";
                                let showButton = document.createElement("button");
                                showButton.type = "button";
                                showButton.className = "circElement btn btn-primary";
                                showButton.innerHTML = "Show Elements";
                                let tbody_doc = table.appendChild(tBody);
                                tbody_doc.appendChild(table_td1);
                                tbody_doc.appendChild(table_td2);
                                tbody_doc.appendChild(table_td3);
                                if (jsonCirc[index]["numAttr"] > 0) {
                                    showButton.id = "Circle" + (index + 1);
                                    table_td3.appendChild(showButton);

                                }
                                let editButton = document.createElement("button");
                                editButton.id = "Circle" + (index + 1);
                                editButton.type = "button";
                                editButton.className = "editCircElement btn btn-info";
                                editButton.innerHTML = "Edit Elements (A4)";

                                table_td3.appendChild(editButton);

                            }



                            for (let index = 0; index < jsonPath.length; index++) {
                                let tBody = document.createElement("tbody");
                                let table_td1 = document.createElement("td");
                                table_td1.innerHTML = "Path " + (index + 1);
                                let table_td2 = document.createElement("td");
                                table_td2.innerHTML = "path data = " + jsonPath[index]["d"];
                                let table_td3 = document.createElement("td");
                                table_td3.innerHTML = jsonPath[index]["numAttr"] + "</br>"
                                let tbody_doc = table.appendChild(tBody);
                                let showButton = document.createElement("button");
                                showButton.type = "button";
                                showButton.className = "pathElement btn btn-primary";
                                showButton.innerHTML = "Show Elements";

                                tbody_doc.appendChild(table_td1);
                                tbody_doc.appendChild(table_td2);
                                tbody_doc.appendChild(table_td3);
                                if (jsonPath[index]["numAttr"] > 0) {
                                    showButton.id = "Path" + (index + 1);
                                    table_td3.appendChild(showButton);

                                }
                                let editButton = document.createElement("button");
                                editButton.id = "Path" + (index + 1);
                                editButton.type = "button";
                                editButton.className = "editPathElement btn btn-warning";
                                editButton.innerHTML = "Edit Elements";
                                table_td3.appendChild(editButton);


                            }





                            for (let index = 0; index < jsonGroup.length; index++) {
                                let tBody = document.createElement("tbody");
                                let table_td1 = document.createElement("td");
                                table_td1.innerHTML = "Group " + (index + 1);
                                let table_td2 = document.createElement("td");

                                table_td2.innerHTML = (jsonGroup[index]["children"]) + (jsonGroup[index]["children"] == 0 ? " child element" : " child elements")
                                let table_td3 = document.createElement("td");
                                table_td3.innerHTML = jsonGroup[index]["numAttr"] + "</br>"
                                let tbody_doc = table.appendChild(tBody);
                                let showButton = document.createElement("button");
                                showButton.type = "submit";
                                showButton.className = "groupElement btn btn-primary";
                                showButton.innerHTML = "Show Elements";
                                tbody_doc.appendChild(table_td1);
                                tbody_doc.appendChild(table_td2);
                                tbody_doc.appendChild(table_td3);
                                if (jsonGroup[index]["numAttr"] > 0) {
                                    showButton.id = "Group" + (index + 1);
                                    table_td3.appendChild(showButton);

                                }
                                let editButton = document.createElement("button");
                                editButton.id = "Group" + (index + 1);
                                editButton.type = "button";
                                editButton.className = "editGroupElement btn btn-warning";
                                editButton.innerHTML = "Edit Elements";
                                table_td3.appendChild(editButton);





                            }

                            $(".rectElement").click(function (e) {

                                let button = document.getElementById(this.id);
                                for (let index in jsonRectAttrValue[this.id]) {
                                    let p = document.createElement("p");
                                    p.innerHTML = "Name: " + jsonRectAttrValue[this.id][index]["name"] + "</br>" + "Value: " + jsonRectAttrValue[this.id][index]["value"];
                                    button.appendChild(p);
                                }
                                alert("Success!");

                                $(this).attr("disabled", "disabled");

                                e.preventDefault();
                            });



                            $(".pathElement").click(function (e) {

                                let button = document.getElementById(this.id);

                                for (let index in jsonPathAttrValue[this.id]) {
                                    let p = document.createElement("p");
                                    p.innerHTML = "Name: " + jsonPathAttrValue[this.id][index]["name"] + "</br>" + "Value: " + jsonPathAttrValue[this.id][index]["value"];
                                    button.appendChild(p);
                                }
                                alert("Success!");

                                $(this).attr("disabled", "disabled");

                                e.preventDefault();
                            });


                            $(".circElement").click(function (e) {

                                let button = document.getElementById(this.id);
                                for (let index in jsonCircAttrValue[this.id]) {
                                    let p = document.createElement("p");
                                    p.innerHTML = "Name: " + jsonCircAttrValue[this.id][index]["name"] + "</br>" + "Value: " + jsonCircAttrValue[this.id][index]["value"];
                                    button.appendChild(p);
                                }

                                alert("Success!");

                                $(this).attr("disabled", "disabled");

                                e.preventDefault();
                            });



                            $(".groupElement").click(function (e) {

                                let button = document.getElementById(this.id);
                                for (let index in jsonGroupAttrValue[this.id]) {
                                    let p = document.createElement("p");
                                    p.innerHTML = "Name: " + jsonGroupAttrValue[this.id][index]["name"] + "</br>" + "Value: " + jsonGroupAttrValue[this.id][index]["value"];
                                    button.appendChild(p);
                                }
                                alert("Success!");

                                $(this).attr("disabled", "disabled");

                                e.preventDefault();
                            });


                            $(".editGroupElement").click(function (e) {

                                console.log(this.id);

                                e.preventDefault();
                            });

                            $("#showFileName").click(function (e) {
                                let showFile = document.getElementById("editFileName");
                                if (showFile.style.display == "block") {
                                    document.getElementById("editFileName").style.display = "none";

                                }
                                else {
                                    document.getElementById("editFileName").style.display = "block";
                                }

                            });
                            $(".editRectElement").click(function (e) {
                                let select = document.getElementById("imagePanel");
                                select.className = this.id;

                                let showFile = document.getElementById("editRect");
                                if (showFile.style.display == "none") {
                                    document.getElementById("editRect").style.display = "block";
                                }
                                else {
                                    document.getElementById("editRect").style.display = "none";
                                }
                            });
                            //when this is pressed
                            $(".editCircElement").click(function (e) {
                                let select = document.getElementById("imagePanel");
                                select.className = this.id;

                                //dynamically show form
                                let showFile = document.getElementById("editCirc");
                                if (showFile.style.display == "none") {
                                    document.getElementById("editCirc").style.display = "block";

                                }
                                else {
                                    document.getElementById("editCirc").style.display = "none";
                                }

                            });

                            //when this is pressed
                            $(".editPathElement").click(function (e) {
                                //dynamically add form
                                console.log("hi");
                                let showFile = document.getElementById("editPath");
                                if (showFile.style.display == "none") {
                                    document.getElementById("editPath").style.display = "block";
                                }
                                else {
                                    document.getElementById("editPath").style.display = "none";
                                }

                            });
                            //when this is pressed
                            $(".editGroupElement").click(function (e) {
                                //dynamically add form
                                console.log("hi");
                                let showFile = document.getElementById("editGroup");
                                if (showFile.style.display == "none") {
                                    document.getElementById("editGroup").style.display = "block";
                                }
                                else {
                                    document.getElementById("editGroup").style.display = "none";
                                }

                            });


                            $("#editFileNameTitlex").on('click', (function (e) {
                                let title = document.getElementById("title").value;

                                $.ajax({
                                    type: 'GET',            //Request type
                                    dataType: 'json',       //Data type - we will use JSON for almost everything 
                                    url: '/editFileNameTitle',   //The server endpoint we are connecting to
                                    data: {
                                        fname: item,
                                        fileTitle: title,

                                    },
                                    success: function (data) {
                                        alert(data.message);


                                    },
                                    fail: function (data) {

                                    }
                                });
                                e.preventDefault();


                            })

                            )

                            $("#editFileNameDescx").on('click', (function (e) {

                                let description = document.getElementById("description").value;

                                $.ajax({
                                    type: 'GET',            //Request type
                                    dataType: 'json',       //Data type - we will use JSON for almost everything 
                                    url: '/editFileNameDesc',   //The server endpoint we are connecting to
                                    data: {
                                        fname: item,
                                        fileDescription: description

                                    },
                                    success: function (data) {

                                        alert(data.message);


                                    },
                                    fail: function (data) {

                                    }
                                });
                                e.preventDefault();


                            })

                            )









                            /*
                                                        console.log(jsonRect);
                                                        console.log(jsonCirc);
                                                        console.log(jsonGroup);
                            
                            */
                        },
                        fail: function (data) {

                        }
                    })
                });



                //hide
                ;

            }
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
                            //  table_td1_href.innerHTML = element;

                            table_td1_href.setAttribute("onclick", "getClick(this)");

                            table_img_doc = table_td1_doc.
                                appendChild(table_td1_href).
                                appendChild(table_img);

                            let table_td2 = document.createElement("td");
                            let table_td2_href = document.createElement("a");
                            table_td2_href.href = element;

                            table_td2_href.download = element
                            table_td2_href.innerHTML = element;
                            table_td2_href.setAttribute("onclick", "getClick(this)");

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



