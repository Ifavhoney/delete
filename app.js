

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

function getTime() {
  let date = new Date();
  let cur_date = date.toISOString().slice(0, 10);
  let time = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds()
  let creation_time = cur_date + " " + time;
  return creation_time;
}
function getSize(file) {
  let value = path.join(__dirname + "/uploads/" + file);

  let size = Math.round((fs.statSync(value).size / 1024));
  return size;
}
async function getSVG_ID(connection, fileName) {
  let svg_id = -1;
  try {
    //could use limit function too - optimize last
    const [vRow, vCol] = await connection.execute("select * from FILE where file_name = " + '\'' + fileName + '\'');
    for (const item of vRow) {
      svg_id = item["svg_id"];
      let d_descr = item["file_description"];
    }
  } catch (e) {
    return -1;
  }
  return svg_id;

}

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
    "updateCirc": ["bool", ["string", "float", "float", "float", "string", "int"]],
    "updateRect": ["bool", ["string", "float", "float", "float", "float", "string", "int"]],



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

app.get('/clearData', async function (req, res) {
  let message = null;
  let connection;
  try {


    connection = await mysql.createConnection(credentials);

    //Check for duplicates

    await connection.execute("DELETE FROM IMG_CHANGE");
    await connection.execute("DELETE FROM DOWNLOAD");
    await connection.execute("DELETE FROM FILE");

    await connection.execute("ALTER TABLE FILE AUTO_INCREMENT=1")
    await connection.execute("ALTER TABLE DOWNLOAD AUTO_INCREMENT=1;")
    await connection.execute("ALTER TABLE IMG_CHANGE AUTO_INCREMENT=1;")

    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message });
});

//#region execute queries
app.get('/query1', async function (req, res) {
  let message = null;
  let connection;
  let query1;
  let sortByName;
  let sortBySize;

  try {


    connection = await mysql.createConnection(credentials);
    let [vRow, vCol] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE;");
    let [vRow1, vCol1] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE ORDER BY file_name;");
    let [vRow2, vCol2] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE ORDER BY file_size;");
    message = "success";
    query1 = vRow;
    sortByName = vRow1;
    sortBySize = vRow2;

  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query1: query1, sortByName: sortByName, sortBySize: sortBySize });
});


app.get('/query2', async function (req, res) {
  let message = null;
  let connection;
  let creation_time = req.query.creation_time;
  let query2;
  let sortByName;
  let sortBySize;
  let sortByDate;
  try {


    connection = await mysql.createConnection(credentials);
    console.log("gets here");
    let [vRow, vCol] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE  WHERE creation_time BETWEEN '2020-04-01 00:00:00' AND " + '\'' + creation_time + '\'');
    let [vRow1, vCol1] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE  WHERE creation_time BETWEEN '2020-04-01 00:00:00' AND " + '\'' + creation_time + '\'' + " ORDER BY file_name");
    let [vRow2, vCol2] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE  WHERE creation_time BETWEEN '2020-04-01 00:00:00' AND " + '\'' + creation_time + '\'' + " ORDER BY file_size");
    let [vRow3, vCol3] = await connection.execute("SELECT file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size from FILE  WHERE creation_time BETWEEN '2020-04-01 00:00:00' AND " + '\'' + creation_time + '\'' + " ORDER BY creation_time");
    query2 = vRow;
    sortByName = vRow1;
    sortBySize = vRow2;
    sortByDate = vRow3
    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query2: query2, sortByName: sortByName, sortBySize: sortBySize, sortByDate: sortByDate });
});




app.get('/query3', async function (req, res) {
  let message = null;
  let query3;
  let sortByName;
  let sortBySize;
  let sortByDate;
  let creation_time = req.query.creation_time;

  let connection;

  try {


    connection = await mysql.createConnection(credentials);


    let [vRow, vCol] = await connection.execute("SELECT FILE.file_name, FILE.file_size, IMG_CHANGE.change_time, COUNT(*) AS count FROM FILE, IMG_CHANGE WHERE FILE.svg_id = IMG_CHANGE.svg_id and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " GROUP BY IMG_CHANGE.svg_id");
    let [vRow1, vCol1] = await connection.execute("SELECT FILE.file_name, FILE.file_size, IMG_CHANGE.change_time, COUNT(*) AS count FROM FILE, IMG_CHANGE WHERE FILE.svg_id = IMG_CHANGE.svg_id and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " GROUP BY IMG_CHANGE.svg_id ORDER BY FILE.file_name");
    let [vRow2, vCol2] = await connection.execute("SELECT FILE.file_name, FILE.file_size, IMG_CHANGE.change_time, COUNT(*) AS count FROM FILE, IMG_CHANGE WHERE FILE.svg_id = IMG_CHANGE.svg_id and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " GROUP BY IMG_CHANGE.svg_id ORDER BY FILE.file_size");
    let [vRow3, vCol3] = await connection.execute("SELECT FILE.file_name, FILE.file_size, IMG_CHANGE.change_time, COUNT(*) AS count FROM FILE, IMG_CHANGE WHERE FILE.svg_id = IMG_CHANGE.svg_id and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " GROUP BY IMG_CHANGE.svg_id ORDER BY IMG_CHANGE.change_time");

    query3 = vRow;
    sortByName = vRow1;
    sortBySize = vRow2;
    sortByDate = vRow3


    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query3: query3, sortByName: sortByName, sortBySize: sortBySize, sortByDate: sortByDate });
});


app.get('/query4', async function (req, res) {

  let message = null;
  let query4;
  let shape = req.query.shape;
  let begin = req.query.begin;

  let end = req.query.end;

  let sortByName;
  let sortBySize;
  let sortByShape;
  let connection;

  try {

    connection = await mysql.createConnection(credentials);


    let [vRow, vCol] = await connection.execute("SELECT file_name, file_size, " + shape + " FROM FILE where " + shape + " between " + begin + " and " + end);
    let [vRow1, vCol1] = await connection.execute("SELECT file_name, file_size, " + shape + " FROM FILE where " + shape + " between " + begin + " and " + end + " ORDER BY file_name;");
    let [vRow2, vCol2] = await connection.execute("SELECT file_name, file_size, " + shape + " FROM FILE where " + shape + " between " + begin + " and " + end + " ORDER BY file_size;");
    let [vRow3, vCol3] = await connection.execute("SELECT file_name, file_size, " + shape + " FROM FILE where " + shape + " between " + begin + " and " + end + " ORDER BY " + shape + ";");

    query4 = vRow;
    sortByName = vRow1;
    sortBySize = vRow2;
    sortByShape = vRow3;
    message = "success";

  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query4: query4, sortByName: sortByName, sortBySize: sortBySize, sortByShape: sortByShape });
});



app.get('/query5', async function (req, res) {

  let message = null;
  let query5;
  let sortByName;
  let sortByDate;
  let sortByCount;
  let connection;

  try {


    connection = await mysql.createConnection(credentials);


    let [vRow, vCol] = await connection.execute("SELECT COUNT(*) as count, DOWNLOAD.svg_id, FILE.file_name, DOWNLOAD.d_descr FROM DOWNLOAD, FILE where FILE.svg_id = DOWNLOAD.svg_id GROUP BY FILE.svg_id; ");
    let [vRow1, vCol1] = await connection.execute("SELECT COUNT(*) as count, DOWNLOAD.svg_id, FILE.file_name, DOWNLOAD.d_descr FROM DOWNLOAD, FILE where FILE.svg_id = DOWNLOAD.svg_id GROUP BY FILE.svg_id ORDER BY FILE.svg_id ; ");
    let [vRow2, vCol2] = await connection.execute("SELECT COUNT(*) as count, DOWNLOAD.svg_id, FILE.file_name, DOWNLOAD.d_descr FROM DOWNLOAD, FILE where FILE.svg_id = DOWNLOAD.svg_id GROUP BY FILE.svg_id ORDER BY count;");
    let [vRow3, vCol3] = await connection.execute("SELECT COUNT(*) as count, DOWNLOAD.svg_id, FILE.file_name, DOWNLOAD.d_descr FROM DOWNLOAD, FILE where FILE.svg_id = DOWNLOAD.svg_id GROUP BY FILE.svg_id ORDER BY DOWNLOAD.download_id DESC;");
    query5 = vRow;
    sortByName = vRow1;
    sortByCount = vRow2;
    sortByDate = vRow3;
    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query5: query5, sortByName: sortByName, sortByCount: sortByCount, sortByDate: sortByDate });
});


app.get('/query6', async function (req, res) {

  let message = null;
  let query6;
  let file_name = req.query.file_name;
  let creation_time = req.query.creation_time;
  let sortByType;
  let sortByDesc;
  let sortByAsc;
  let connection;

  try {


    connection = await mysql.createConnection(credentials);
    let [vRowId, vColId] = await connection.execute("SELECT * FROM FILE where file_name = " + '\'' + file_name + '\'');
    let id = 0;
    for (const item of vRowId) {
      id = item["svg_id"];
    }


    let [vRow, vCol] = await connection.execute(" SELECT FILE.file_name, IMG_CHANGE.change_type, IMG_CHANGE.change_summary, IMG_CHANGE.change_time FROM FILE, IMG_CHANGE WHERE FILE.svg_id = " + id + " and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'');
    let [vRow1, vCol1] = await connection.execute(" SELECT FILE.file_name, IMG_CHANGE.change_type, IMG_CHANGE.change_summary, IMG_CHANGE.change_time FROM FILE, IMG_CHANGE WHERE FILE.svg_id = " + id + " and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " ORDER BY IMG_CHANGE.change_type");
    let [vRow2, vCol2] = await connection.execute(" SELECT FILE.file_name, IMG_CHANGE.change_type, IMG_CHANGE.change_summary, IMG_CHANGE.change_time FROM FILE, IMG_CHANGE WHERE FILE.svg_id = " + id + " and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " ORDER BY IMG_CHANGE.change_time DESC");
    let [vRow3, vCol3] = await connection.execute(" SELECT FILE.file_name, IMG_CHANGE.change_type, IMG_CHANGE.change_summary, IMG_CHANGE.change_time FROM FILE, IMG_CHANGE WHERE FILE.svg_id = " + id + " and IMG_CHANGE.change_time between '2020-04-01 00:00:00' and " + '\'' + creation_time + '\'' + " ORDER BY IMG_CHANGE.change_time ASC");

    query6 = vRow;
    sortByType = vRow1;
    sortByDesc = vRow2;
    sortByAsc = vRow3;


    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, query6: query6, sortByType: sortByType, sortByDesc: sortByDesc, sortByAsc: sortByAsc });
});




app.get('/displayStatus', async function (req, res) {
  let message = null;
  let connection;
  let fileCount = 0;
  let downloadCount = 0;
  let changeCount = 0;
  try {


    connection = await mysql.createConnection(credentials);
    let [vRow, vCol] = await connection.execute("SELECT COUNT(*) FROM FILE")
    let [vRow1, vCol1] = await connection.execute("SELECT COUNT(*) FROM DOWNLOAD")
    let [vRow2, vCol2] = await connection.execute("SELECT COUNT(*) FROM IMG_CHANGE")

    for (const item of vRow) {
      fileCount = item["COUNT(*)"];
    }
    for (const item of vRow1) {
      downloadCount = item["COUNT(*)"];
    }
    for (const item of vRow2) {
      changeCount = item["COUNT(*)"];
    }



    message = "success";
  }
  catch (e) {
    message = "fail";
  }
  finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message, fileCount: fileCount, downloadCount: downloadCount, changeCount: changeCount });
});

app.get("/login", async function (req, res, next) {

  let username = req.query.username;
  let password = req.query.password;
  let database = req.query.database;
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

    }
    catch (e) {
      message = "Incorrect Account";
      console.log("There has been an error:" + message);
    }

  }
  res.send({ message: message });

})

//every time the user clicks on the download link
// for a file using the A3 interface, insert a recordintotheDOWNLOADtable.
// Makesurethatthenewrecord'ssvg_idcorrectlyreferencestherelevantin the FILE table.

//unable to call alert atm
app.get("/storeFiles", async function (req, res, next) {

  let message = null;

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
        message = "success";

      }
      catch (e) {
        message = "fail";
        console.log("Query file error: " + e);

      }
      finally {
        if (connection && connection.end) connection.end();

      }
    }
    res.send({ message: message });




  }




  );




});

app.get("/trackDownloads", async function (req, res, next) {


  let message = null;
  let fileName = req.query.fileName;
  let d_descr = req.query.d_descr;
  let connection;
  try {


    connection = await mysql.createConnection(credentials);
    const [vRow, vField] = await connection.execute("select * from FILE where file_name = " + '\'' + fileName + '\'');
    let svg_id = 1;
    for (const item of vRow) {
      svg_id = item["svg_id"];
    }
    let date = new Date();
    let cur_date = date.toISOString().slice(0, 10);
    let time = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds()
    let creation_time = cur_date + " " + time;

    //+ '\'' + creation_time + '\'' + 
    await connection.execute("INSERT INTO DOWNLOAD (d_descr, svg_id) VALUES(" + '\'' + d_descr + " at " + getTime() + '\', ' + svg_id + ");");


    message = "success";
  } catch (e) {
    message = "fail, either you didnt store files or incorrect credentials";

    console.log("Query file error: " + e);

  } finally {
    if (connection && connection.end) connection.end();

  }
  res.send({ message: message })


});

app.get("/downloadFile", async function (req, res) {


  let file = req.query.fileName;
  let title = req.query.title;
  let desc = req.query.description;
  file = file.trim();
  title = title.trim();
  desc = desc.trim();
  let message;
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
        res.send({ message: "Fail due to being empty or incorrect ending with .svg" })

      }
      else {


        console.log(title.length);
        if (title.length == 0) {
          title = " ";
        }
        if (desc.length == 0) {
          desc = " ";
        }

        let jsonValue = sharedLibrary.svgDownloadFile(file + ".svg", json, title, desc);
        if (jsonValue == true) {

          fs.rename(file + ".svg", "uploads/" + file + ".svg", function (err) {
            if (err) throw "unable to add " + file + ".svg" + " due to wrong format"
            console.log('successly moved file to uploads folder');
          });


          let connection;
          try {


            /*console.log("size: " + size);
            console.log("(" + '\'' + files[i] + '\',\'' + jsonTitle + '\',\'' + jsonDesc + '\',' + jsonNums.numRect + ',' + jsonNums.numCirc + ',' + jsonNums.numPaths + ','
            + jsonNums.numGroups + ");")
            */


            connection = await mysql.createConnection(credentials);
            await connection.execute("INSERT INTO FILE(file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size) " +
              "VALUES" + "(" + '\'' + file + ".svg" + '\',\'' + title + '\',\'' + desc + '\',' + 0 + ',' + 0 + ',' + 0 + ','
              + 0 + ',\'' + getTime() + '\',' + getSize(file + ".svg") + ");");

            console.log('successfully added to database');
            message = "success";
          } catch (e) {
            message = "fail";

            console.log("Query file error: " + e);

          } finally {
            if (connection && connection.end) connection.end();

          }
          res.send({ message: message })

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
  //res.send({ jsonValue });

}

);

app.get('/editFileNameTitle', async function (req, res) {


  let editTitle = req.query.fileTitle.trim();

  /*let editDescription = req.query.fileDescription.trim();
  if (editDescription.length == 0) {
    editDescription = " ";
  }
  */
  let fileName = req.query.fname;
  if (editTitle.length == 0) {
    editTitle = " ";
  }

  // console.log(editTitle + " " + fileName);

  //console.log(fileName);
  let jsonValue = sharedLibrary.updateTilteDesc(path.join(__dirname + '/uploads/' + fileName), editTitle, " ");
  let message = null;


  if (jsonValue == true) {
    message = "success please refresh";
    let connection;

    try {

      // INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) VALUES('Emoji_poo.svg', 'emoji', '2020-01-01 10:10:10', 1);


      connection = await mysql.createConnection(credentials);
      const svg_id = await getSVG_ID(connection, fileName)


      await connection.execute("INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) " +
        "VALUES" + "(" + '\'' + "Edit Title" + '\',\'' + "Change Title to " + editTitle + ' \',\'' + getTime() + '\',' + svg_id + ");");

      await connection.execute("update FILE SET file_size =" + getSize(fileName) + " where svg_id = " + svg_id + ";");

      await connection.execute("update FILE SET file_title = " + '\'' + editTitle + '\'' + " where svg_id = " + svg_id + ";");

    } catch (e) {
      console.log("Query file error: " + e);
      message = "fail"

    } finally {
      if (connection && connection.end) connection.end();

    }

  }
  else {
    message = "fail";
  }
  res.send({ message: message, title: editTitle });


})


app.get('/editFileNameDesc', async function (req, res) {



  let editDescription = req.query.fileDescription.trim();
  if (editDescription.length == 0) {
    editDescription = " ";
  }

  // console.log(editTitle + " " + fileName);

  //console.log(fileName);
  let fileName = req.query.fname;

  let jsonValue = sharedLibrary.updateTilteDesc(path.join(__dirname + '/uploads/' + fileName), " ", editDescription);
  let message;


  if (jsonValue == true) {
    message = "success";
    let connection;

    try {

      // INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) VALUES('Emoji_poo.svg', 'emoji', '2020-01-01 10:10:10', 1);


      console.log(credentials);
      connection = await mysql.createConnection(credentials);
      const svg_id = await getSVG_ID(connection, fileName)


      await connection.execute("INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) " +
        "VALUES" + "(" + '\'' + "Edit Description " + '\',\'' + "Change Description to " + editDescription + '\',\'' + getTime() + '\',' + svg_id + ");");

      await connection.execute("update FILE SET file_size =" + getSize(fileName) + " where svg_id = " + svg_id + ";");

      await connection.execute("update FILE SET file_description = " + '\'' + editDescription + '\'' + " where svg_id = " + svg_id + ";");


    } catch (e) {
      console.log("Query file error: " + e);
      message = "fail";
    } finally {
      if (connection && connection.end) connection.end();

    }

  }
  else {
    message = "fail";

  }
  res.send({ message: message, description: editDescription });


})


//Refresh to see changes
app.get('/editCirc', async function (req, res) {
  let fileName = req.query.fileName;

  let cy = parseFloat(req.query.cy);
  let cx = parseFloat(req.query.cx);
  let units = req.query.units.trim();
  let r = parseFloat(req.query.r);
  let index = req.query.index;
  //bool updateCirc(char *fileName, float cx, float cy, float r, char *units, int index);
  console.log(cy + "\n" + cy + "\n" + units + "\n" + r + "\n" + index);
  if (units.length == 0) {
    units = " ";
  }
  let jsonValue = sharedLibrary.updateCirc(path.join(__dirname + '/uploads/' + fileName), cx, cy, r, units, index);

  let message;


  if (jsonValue == true) {
    let connection;

    try {

      // INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) VALUES('Emoji_poo.svg', 'emoji', '2020-01-01 10:10:10', 1);


      console.log(credentials);
      connection = await mysql.createConnection(credentials);
      const svg_id = await getSVG_ID(connection, fileName)



      index++;
      await connection.execute("INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) " +
        "VALUES" + "(" + '\'' + "Edit Circle " + '\',\'' + "Edit Circle Attribute # " + index.toString() + '\',\'' + getTime() + '\',' + svg_id + ");");

      console.log('successfully added to database');

      message = "success please refresh";


    } catch (e) {
      console.log("Query file error: " + e);
      message = "fail";
    } finally {
      if (connection && connection.end) connection.end();

    }

  }
  else {
    message = "fail"

  }
  res.send({ message: message });



})
app.get('/editRect', async function (req, res) {

  let fileName = req.query.fileName;

  let x = parseFloat(req.query.x);
  let y = parseFloat(req.query.y);
  let width = parseFloat(req.query.width);
  let height = parseFloat(req.query.height);
  let units = req.query.units;
  let index = req.query.index;

  console.log(x + y + width + height + units);

  if (units.length == 0) {
    units = " ";
  }
  let jsonValue = sharedLibrary.updateRect(path.join(__dirname + '/uploads/' + fileName), x, y, width, height, units, index);

  let message;


  if (jsonValue == true) {
    let connection;

    try {

      // INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) VALUES('Emoji_poo.svg', 'emoji', '2020-01-01 10:10:10', 1);


      console.log(credentials);
      connection = await mysql.createConnection(credentials);
      const svg_id = await getSVG_ID(connection, fileName)

      index++;
      await connection.execute("INSERT INTO IMG_CHANGE(change_type, change_summary, change_time, svg_id) " +
        "VALUES" + "(" + '\'' + "Edit Rectangle" + '\',\'' + "Edit Rectangle Atttribute # " + index.toString() + '\',\'' + getTime() + '\',' + svg_id + ");");

      console.log('successfully added to database');

      message = "success please refresh";


    } catch (e) {
      console.log("Query file error: " + e);
      message = "fail";
    } finally {
      if (connection && connection.end) connection.end();

    }

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
  fs.readdir(path.join(__dirname), (err, files) => {
    //callback function
    const kb = 1024;
    let arr = [];
    files.forEach(file => {
      if (file == "my.svg") {
        fs.unlinkSync("my.svg");
      }
    });

  }

  );

  //gets list of files
  fs.readdir(path.join(__dirname + '/uploads'), (err, files) => {
    //callback function
    const kb = 1024;
    let arr = [];
    files.forEach(file => {

      let svgPath = path.join(__dirname + "/uploads/" + file);
      let size = (fs.statSync(svgPath).size / kb).toFixed(2)

      arr.push(size + " KB");
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
