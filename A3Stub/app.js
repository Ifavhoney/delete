'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');
app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

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

let sharedLibrary = ffi.Library("./parser/bin/libsvgparse.so",
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


    "svgDownloadFile": ["bool", ["string", "string"]],

  });

app.get("/downloadFile", function (req, res) {

  let jsonValue = sharedLibrary.svgDownloadFile(path.join(__dirname + '/uploads/' + req.query.title), req.query.json);
  res.send({ jsonValue });
});

app.post('/editFileName', function (req, res) {


  let editTitle = req.body.titl;
  let editDescription = req.body.desc;
  let fileName = req.query.fname;

  console.log(editTitle + " " + editDescription);
  /*
  // 
  */
  console.log(fileName);
  let jsonValue = sharedLibrary.updateTilteDesc(path.join(__dirname + '/uploads/' + "rects.svg"), "editTitle", "editDescription");
  let message;
  if (jsonValue == true) {
    message = "success";
  }
  else {
    message = "fail"
    console.log("?????" + message);

  }
  //res.send({ title: editTitle, description: editDescription, message: message });



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


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);