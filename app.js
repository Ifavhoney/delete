

'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');
app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads'), { dotfiles: 'allow' }));

// Minimization
const fs = require('fs');

const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];
let text = "";

// Send HTML at root, do not change
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/public/index.html'));
});


// Send Style, do not change
app.get('/style.css', function (req, res) {
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
  fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
  if (!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.fileInput;
  //From ajax client
  //console.log(value);

  // Use the mv() method to place the file somewhere on your server

  if (uploadFile != null) {
    //do not proceed if it's a broken svg
    let exists = false;
    fs.readdir(path.join(__dirname + '/uploads'), (err, files) => {
      files.forEach((file) => {
        if (uploadFile.name == file) {
          exists = true

        }
      }

      )
      if (exists == false) {

        uploadFile.mv('uploads/' + uploadFile.name, function (err) {
          if (err) {
            return res.status(500).send(err);
          }
          text = "Success: Valid SVG and Does not overwrite";
          res.redirect("/");

        });
      }
      else {

        text = "Fail: file exists, cannot overwrite";
        res.redirect("/");

      }

    }

    );
  }

  else {
    text = "empty file";

    res.redirect("/");

  }

});



//Respond to GET requests for files in the uploads/ directory

app.get('/uploads/:name', function (req, res) {
  fs.stat('uploads/' + req.params.name, function (err, stat) {
    if (err == null) {
      res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: ' + err);
    }
  });
});

//******************** Your code goes here ******************** 
//Shared library is how you communicate with the backend
//CreateSVGChar Returns num rects, paths, circles, groups

const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));

let sharedLibrary = ffi.Library("libsvgparse.so",
  {
    "createSVGChar": ["string", ["string", "string"]],
    "rectViewPanelToJSON": ["string", ["string", "string"]],
    "circViewPanelToJSON": ["string", ["string", "string"]],
    "pathViewPanelToJSON": ["string", ["string", "string"]],
    "groupViewPanelToJSON": ["string", ["string", "string"]],
    "titleViewPanelToString": ["string", ["string", "string"]],
    "descViewPanelToString": ["string", ["string", "string"]],
    "rectViewPanelAttrToJSON": ["string", ["string", "string"]],
    "circViewPanelAttrToJSON": ["string", ["string", "string"]],
    "pathViewPanelAttrToJSON": ["string", ["string", "string"]],
    "groupViewPanelAttrToJSON": ["string", ["string", "string"]],
    "updateTilteDesc": ["bool", ["string", "string", "string"]],


    "svgDownloadFile": ["bool", ["string", "string", "string", "string"]],

  });


const mysql = require('mysql2/promise');

//Database
let credentials = {
  host: 'dursley.socs.uoguelph.ca',
  user: "jnguessa",
  password: "1079936",
  database: "jnguessa"

};
app.get("/login", async function (req, res, next) {
  console.log("again");
  let username = req.query.username;
  let password = req.query.password;
  let database = req.query.database;
  console.log(username);
  let message = null;

  if (username != null && password != null & database != null) {
    try {
      message = await testConnection(username, password, database);
      credentials = {
        host: 'dursley.socs.uoguelph.ca',
        user: username,
        password: password,
        database: database

      }
      console.log("test" + message);


    }
    catch (e) {
      message = "fail";
      console.log("There has been an error:" + message);
    }

  }
  console.log(message);
  res.redirect("/");
  // res.redirect("/login:" + message);
  //res.send(JSON.stringify({ message: message }));

})

//every time the user clicks on the download link
// for a file using the A3 interface, insert a recordintotheDOWNLOADtable.
// Makesurethatthenewrecord'ssvg_idcorrectlyreferencestherelevantin the FILE table.

app.get("/storeFiles", async function (req, res, next) {



  fs.readdir(path.join(__dirname + '/uploads'), async (err, files) => {
    for (let i = 0; i < files.length; i++) {

      let connection;
      try {


        connection = await mysql.createConnection(credentials);
        const [vRow, vField] = await connection.execute("select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'FILE' ");
        if (vRow.length != 0) {




          let value = path.join(__dirname + "/uploads/" + files[i]);
          let jsonTitle = sharedLibrary.titleViewPanelToString(value, "null");
          let jsonDesc = sharedLibrary.descViewPanelToString(value, "null");
          let jsonNums = JSON.parse(sharedLibrary.createSVGChar(value, "null"));
          let size = Math.round((fs.statSync(value).size / 1024));

          /*console.log("size: " + size);
          console.log("(" + '\'' + files[i] + '\',\'' + jsonTitle + '\',\'' + jsonDesc + '\',' + jsonNums.numRect + ',' + jsonNums.numCirc + ',' + jsonNums.numPaths + ','
          + jsonNums.numGroups + ");")
          */
          let date = new Date();
          let cur_date = date.toISOString().slice(0, 10);
          let time = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds()
          let creation_time = cur_date + " " + time;

          //Check for duplicates

          const [rows1, fields1] = await connection.execute("SELECT * FROM FILE where file_name = " + '\'' + files[i] + '\'');
          let length = 0;
          for (let row of rows1) {
            length++;
          }
          if (length == 0) {
            await connection.execute("INSERT INTO FILE(file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size) " +
              "VALUES" + "(" + '\'' + files[i] + '\',\'' + jsonTitle + '\',\'' + jsonDesc + '\',' + jsonNums.numRect + ',' + jsonNums.numCirc + ',' + jsonNums.numPaths + ','
              + jsonNums.numGroups + ',\'' + creation_time + '\',' + size + ");");
          }

        }
      }
      catch (e) {
        console.log("Query file error: " + e);
        return false;
      }
      finally {
        if (connection && connection.end) connection.end();

      }
    }


  }


  );




});

app.post("/trackDownloads", async function (req, res, next) {
  let fileName = req.body.fileName;
  let connection;
  try {


    connection = await mysql.createConnection(credentials);
    const [vRow, vField] = await connection.execute("select * from FILE where file_name = " + '\'' + fileName + '\'');
    let svg_id = 1;
    let d_descr = " ";
    for (const item of vRow) {
      svg_id = item["svg_id"];
      d_descr = item["file_description"];
      console.log(d_descr);
    }
    await connection.execute
      ("INSERT INTO DOWNLOAD (d_descr, svg_id) VALUES(" + '\'' + d_descr + '\', ' + svg_id + ");");



  } catch (e) {
    console.log("Query file error: " + e);

  } finally {
    if (connection && connection.end) connection.end();

  }

});

app.post("/downloadFile", async function (req, res) {


  let file = req.body.fileName;
  let title = req.body.title;
  let desc = req.body.description;
  file = file.trim();
  title = title.trim();
  desc = desc.trim();
  fs.readdir(path.join(__dirname + '/uploads'), async (err, files) => {
    let exists = false;

    for (let i = 0; i < files.length; i++) {
      if (files[i] == file + ".svg") {
        exists = true;
        break;

      }

    }
    if (exists == false) {

      let json = JSON.stringify({ "title": file, "desc": file });

      if ((file + ".svg" == ".svg") || file.indexOf(".svg") >= 0) {
        console.log("not a valid svg");
      }
      else {


        console.log(title.length);


        let jsonValue = sharedLibrary.svgDownloadFile(file + ".svg", json, title.length == 0 ? " " : title, desc.length == 0 ? " " : desc);
        if (jsonValue == true) {

          fs.rename(file + ".svg", "uploads/" + file + ".svg", function (err) {
            if (err) throw "unable to add " + file + ".svg" + " due to wrong format"
            console.log('successly moved file to uploads folder');
          });


          // let connection;
          try {
            // console.log(credentials);
            /*
            connection = await mysql.createConnection(credentials);
            await connection.execute("INSERT INTO FILE(file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size) " +
              "VALUES" + "(" + '\'' + files[i] + '\',\'' + jsonTitle + '\',\'' + jsonDesc + '\',' + jsonNums.numRect + ',' + jsonNums.numCirc + ',' + jsonNums.numPaths + ','
              + jsonNums.numGroups + ',\'' + creation_time + '\',' + size + ");");
              */

          } catch (e) {
            //  console.log("Query file error: " + e);

          } finally {
            //    if (connection && connection.end) connection.end();

          }
        }


        else {
          console.log("File not added");
        }
      }

    }
    else {
      console.log("File already exists");
    }
  });

  /*
   
   
  */
  res.redirect("/")

  //res.send({ jsonValue });
});

app.get('/editFileName', function (req, res) {


  let editTitle = req.query.fileTitle;
  let editDescription = req.query.fileDescription;
  let fileName = req.query.fname;

  console.log(editTitle + " " + fileName);

  console.log(fileName);
  let jsonValue = sharedLibrary.updateTilteDesc(path.join(__dirname + '/uploads/' + fileName), editTitle, editDescription);
  let message;
  if (jsonValue == true) {
    message = "success";
  }
  else {
    message = "fail"

  }
  res.send({ message: message });


})


app.get('/someendpoint', function (req, res) {
  let retStr = req.query.name1 + " " + req.query.name2;
  res.send({
    foo: retStr
  });
});


//constantly waiting for a change in the uploads folder

app.get('/getListOfFiles', function (req, res) {

  //gets list of files
  fs.readdir(path.join(__dirname + '/uploads'), (err, files) => {
    //callback function
    const kb = 1024;
    let arr = [];
    files.forEach(file => {

      let svgPath = path.join(__dirname + "/uploads/" + file);
      let size = Math.round((fs.statSync(svgPath).size / kb)) + ' KB';
      arr.push(size);
    });
    res.send({
      length: files.length,
      cons: text,
      files: files,
      sizes: arr
    });
  }

  );


});



app.get("/getSVGParser", function (req, res) {
  //From ajax client
  let value = path.join(__dirname + "/uploads/" + req.query.fileName);
  //console.log(value);

  //Returns the number for rects, circles, paths, and groups, 
  let jsonValue = sharedLibrary.createSVGChar(value, "null");

  if (jsonValue == null) {
    fs.unlinkSync(value)
  }

  //Sends back that information
  else {

    res.send({ jsonData: jsonValue });
  }
})



app.get("/fileDropDown", function (req, res) {
  //Returns the number for rects, circles, paths, and groups, 

  fs.readdir(path.join(__dirname + '/uploads'), (err, files) => {
    let i = 0;
    files.forEach(file => {

      let jsonValue = sharedLibrary.createSVGChar(path.join(__dirname + '/uploads/' + file), "null");

      if (jsonValue == null) {
        files.splice(i, 1);

      }
      i++;
    });
    // console.log()
    res.send({ files: files });
  });

})

//gets the data
app.get("/getViewParser", function (req, res) {

  //From ajax client
  let value = path.join(__dirname + "/uploads/" + req.query.name);
  //Returns the number for rects, circles, paths, and groups, 
  let jsonCircValue = sharedLibrary.circViewPanelToJSON(value, "null");
  let jsonPathValue = sharedLibrary.pathViewPanelToJSON(value, "null");
  let jsonRectValue = sharedLibrary.rectViewPanelToJSON(value, "null");
  let jsonRectAttrValue = sharedLibrary.rectViewPanelAttrToJSON(value, "null");
  let jsonCircAttrValue = sharedLibrary.circViewPanelAttrToJSON(value, "null");
  let jsonPathAttrValue = sharedLibrary.pathViewPanelAttrToJSON(value, "null");
  let jsonGroupAttrValue = sharedLibrary.groupViewPanelAttrToJSON(value, "null");
  let jsonGroupValue = sharedLibrary.groupViewPanelToJSON(value, "null");
  let jsonTitle = sharedLibrary.titleViewPanelToString(value, "null");
  let jsonDesc = sharedLibrary.descViewPanelToString(value, "null");

  //Sends back that information

  res.send({
    jsonCircValue: jsonCircValue,
    jsonPathValue: jsonPathValue,
    jsonRectValue: jsonRectValue,
    jsonGroupValue: jsonGroupValue,
    jsonTitle: jsonTitle,
    jsonDesc: jsonDesc,
    jsonRectAttrValue: jsonRectAttrValue,
    jsonCircAttrValue: jsonCircAttrValue,
    jsonGroupAttrValue: jsonGroupAttrValue,
    jsonPathAttrValue: jsonPathAttrValue

  });

})


let testConnection = async function (username, password, database) {
  // '1079936',
  let dbConf =
  {
    host: 'dursley.socs.uoguelph.ca', user: username,
    password: password,
    database: database
  };
  let createTableFile = "CREATE TABLE IF NOT EXISTS FILE ( svg_id INT AUTO_INCREMENT, file_name VARCHAR(60) NOT NULL, file_title VARCHAR(256),file_description VARCHAR(256), n_rect INT NOT NULL, n_circ INT NOT NULL, n_path INT NOT NULL,      n_group INT NOT NULL,       creation_time DATETIME NOT NULL,       file_size INT NOT NULL,       CONSTRAINT svg_id PRIMARY KEY(svg_id));";
  let createTableIMG_CHANGE = "CREATE TABLE IF NOT EXISTS IMG_CHANGE ( change_id INT AUTO_INCREMENT,change_type VARCHAR(256) NOT NULL, change_summary VARCHAR(256) NOT NULL, change_time DATETIME NOT NULL,svg_id INT NOT NULL,CONSTRAINT change_id PRIMARY KEY(change_id),      FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE);";
  let createTableDownload = "CREATE TABLE IF NOT EXISTS DOWNLOAD (download_id INT AUTO_INCREMENT, d_descr VARCHAR(256),   svg_id INT NOT NULL,CONSTRAINT download_id PRIMARY KEY(download_id),  FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE);"
  let connection;
  let result = "fail";
  try {
    connection = await mysql.createConnection(dbConf);
    await connection.execute(createTableFile);
    await connection.execute(createTableIMG_CHANGE);
    await connection.execute(createTableDownload);


    //Run select query, wait for results
    const [rows1, fields1] = await connection.execute("select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'FILE' ");

    /*
    console.log("\nSorted by last name:");
    for (let row of rows1) {
      console.log("ID: " + row.id + " Last name: " + row.last_name + " First name: " + row.first_name + " mark: " + row.mark);
    }
  */
    result = "success";
  } catch (e) {
    result = "fail"
    console.log("Query error: " + e);
  }
  finally {
    connection.end();
  }
  return result;
}


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
