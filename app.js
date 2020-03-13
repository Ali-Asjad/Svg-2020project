'use strict'

// C library API
const ffi = require('ffi-napi');
const ref = require('ref-napi');
const StructType = require('ref-struct-di')(ref)


// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  const files = req.files
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      console.log('File Upload Failed' + err);
      return res.status(500).send(err);
    }
    console.log('File Uploaded');
    res.setHeader('Content-Type', 'image/svg+xml');
    res.sendFile(__dirname + '/uploads/' + uploadFile.name);

  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});


/******************** My code goes here ********************/

/****************C function parsing functions *****************/
var ElementType = {
  SVG_IMAGE : 0,
  CIRC: 1,
  RECT: 2,
  PATH: 3,
  GROUP: 4
};

var VoidPtr = 'void*'

 var SVG = ffi.Library('/root/Dev/jsh/svg/bin/libsvgparse', {
   'getRects' : [VoidPtr, [VoidPtr]],
   'getCircles' : [VoidPtr, [VoidPtr]],
   'getGroups' : [VoidPtr, [VoidPtr]],
   'getPaths' : [VoidPtr, [VoidPtr]],
   'getLength':['int',[VoidPtr]],
   'circListToJSON' : ['string',[VoidPtr]],
   'rectListToJSON' : ['string',[VoidPtr]],
   'pathListToJSON' : ['string',[VoidPtr]],
   'groupListToJSON' : ['string',[VoidPtr]],
   'attrListToJSON' : ['string',[VoidPtr]],
   'createSVGimage' : [VoidPtr, ['string']],
   'SVGimageToString': ['string', [VoidPtr]],
   'writeSVGimage': ['bool',[VoidPtr, 'string']],
   'deleteSVGimage':['void', [VoidPtr]],
   'countRectAttr': ['int', [VoidPtr]],
   'countCircleAttr':['int', [VoidPtr]],
   'countPathAttr':['int', [VoidPtr]],
   'countGroupAttr':['int', [VoidPtr]],
   'createValidSVGimage':[VoidPtr, ['string','string']],
   'validateSVGimage':['bool', [VoidPtr, 'string']],
   'SVGimageToString':['string', [VoidPtr]],
   'SVGtoJSONInfo':['string',[VoidPtr]],
   'numAttr':['int',[VoidPtr]],
   'METAtoJSON':['string',[VoidPtr]],
   'JSONtoSVG': [VoidPtr,['string']],
   'JSONtoRect':[VoidPtr,['string']],
   'JSONtoCircle':[VoidPtr,['string']],
   'addComponent':['void',[VoidPtr,'int',VoidPtr]],
   'setAttribute':['void',[VoidPtr,'int','int',VoidPtr]]
 })

//Respond to GET requests for image files in the uploads/ directory
app.get('/images', function(req , res){
  var logs = [];
  var arrSVG =[];
  var xsdPath = "./parser/src/svg.xsd";
  var svginfoArray = new Array();
  var path = "./uploads"
  fs.readdir(path, function(err, items) {
    logs.push(items);
    var realpath = "./uploads/";
    for (var i = 0 ; i < logs[0].length ; i ++) {
      
      var jsonArg= new Object();
      arrSVG =[];

      var img = SVG.createSVGimage(realpath  + logs[0][i]);
      var svgBool = SVG.validateSVGimage(img, xsdPath);
      if (svgBool == false){
        arrSVG.push(logs[0][i]);
        arrSVG.push(0);
        arrSVG.push(0);
        arrSVG.push(0);
        arrSVG.push(0);
        arrSVG.push(svgBool);
        jsonArg.name = 'files';
        jsonArg.value = arrSVG;
        svginfoArray.push(jsonArg);
        SVG.deleteSVGimage(img);

      }else{

        var countRects = SVG.getRects(img);
        var countCirc = SVG.getCircles(img);
        var countPath = SVG.getPaths(img);
        var countGroup = SVG.getGroups(img);
        arrSVG.push(logs[0][i]);
        arrSVG.push(SVG.getLength(countRects));
        arrSVG.push(SVG.getLength(countCirc));
        arrSVG.push(SVG.getLength(countPath));
        arrSVG.push(SVG.getLength(countGroup));
        arrSVG.push(svgBool);
        jsonArg.name = 'files';
        jsonArg.value = arrSVG;
        svginfoArray.push(jsonArg);
        SVG.deleteSVGimage(img);
      }
      
    }
    var myJsonString = JSON.stringify(svginfoArray);
    res.send(myJsonString);
  });
});

//svg panel file params getting module
app.get('/someendpoint', function(req , res){
  let reqfname = req.query.fname;
  var realPath = "./uploads/";
  var xsdPath = "./parser/src/svg.xsd";
  var img = SVG.createValidSVGimage(realPath  + reqfname, xsdPath);
  
  var boolsvg = SVG.validateSVGimage(img, xsdPath);
  var metainfo = SVG.METAtoJSON(img);

  var lstRect = SVG.getRects(img);
  var jsonRect = SVG.rectListToJSON(lstRect);

  var lstCircle = SVG.getCircles(img);
  var jsonCirc = SVG.circListToJSON(lstCircle);

  var lstPath = SVG.getPaths(img);
  var jsonPath = SVG.pathListToJSON(lstPath);

  var lstGroup = SVG.getGroups(img);
  var jsonGroup = SVG.groupListToJSON(lstGroup);
  
  var arrayall = [];
  arrayall.push(jsonRect);
  arrayall.push(jsonCirc);
  arrayall.push(jsonPath);
  arrayall.push(jsonGroup);

  var arrayStr = String(metainfo) + "+++{\"Rectangles\":" + jsonRect + "," + "\"Circles\":" + jsonCirc + "," + "\"Paths\":" + jsonPath + "," + "\"Groups\":" + jsonGroup + "}";
  SVG.deleteSVGimage(img);
  if (boolsvg === null) {
    res.send({
      foo: "validate failed!" 
    });
  }
  else{
    var myJsonString = JSON.stringify(arrayStr);
    res.send(myJsonString);
  }
});

app.get('/svgValidate', function(req , res){
  let reqfname = req.query.fname;
  var realPath = "./uploads/";
  var xsdPath = "./parser/src/svg.xsd";
  var img = SVG.createValidSVGimage(realPath  + reqfname, xsdPath);
  var svgBool = SVG.validateSVGimage(img, xsdPath);
  SVG.deleteSVGimage(img);
  res.send(svgBool);
});

// create a simple SVG file into the server
app.get('/createsvg', function(req , res){
  let reqfname = req.query.fname;
  var realPath = "./uploads/";
  var xsdPath = "./parser/src/svg.xsd"
  var strNewInfo = "{\"title\":\"A New created SVG\",\"descr\":\"Created by myself for testing\"}";
  //create new svg file with JSON string
  var testimg = SVG.JSONtoSVG(strNewInfo);
  // validate the created svgfile
  var svgBool = SVG.validateSVGimage(testimg, xsdPath);
  if (svgBool == true){
    var boolSVG = SVG.writeSVGimage(testimg, reqfname + ".svg");
    if (boolSVG == true){
      moveFile('./' + reqfname + '.svg', realPath);
      SVG.deleteSVGimage(testimg);
      res.send(boolSVG);
    }else{
      console.log("file writing failed!")
      res.send(boosSVG);
    }
    
  }
  
});

// create a rectangle with the supported json string of rectangle itmes
app.get('/addrect', function(req , res){
  
  let reqfname = String(req.query.fname);
  let reqstrname = req.query.addstr;
  var realPath = "./uploads/";
  var rectitem = SVG.JSONtoRect(reqstrname);
  var rectimg = SVG.createSVGimage(realPath + reqfname);
  SVG.addComponent(rectimg, ElementType.RECT, rectitem);

  var boolimg = SVG.writeSVGimage(rectimg, realPath + reqfname);
  if (boolimg == true){
    console.log("succeessfully added!")
    res.send(boolimg);
  }else{
    console.log("file writing failed!")
    res.send(boolimg);
  }
    
 
});
// create a circle with the supported json string of circle itmes
app.get('/addcirc', function(req , res){
  
  let reqfname = String(req.query.fname);
  let reqstrname = req.query.addstr;
  reqstrname = reqstrname.replace(/\uFFFD/g, '');
  var realPath = "./uploads/";
  console.log(reqstrname);
  var circitem = SVG.JSONtoCircle(reqstrname);
  var circimg = SVG.createSVGimage(realPath + reqfname);
  SVG.addComponent(circimg, ElementType.CIRC, circitem);
  var boolimg = SVG.writeSVGimage(circimg, realPath + reqfname);
  if (boolimg == true){
    console.log("succeessfully added!")
    res.send(boolimg);
  }else{
    console.log("file writing failed!")
    res.send(boolimg);
  }
    
 
});
// set the scale of rectangle into the existing server SVG file
app.get('/rectscale', function(req , res){

  let reqfname = String(req.query.fname);
  let factorval = req.query.factor;
  var realPath = "./uploads/";

  // define the attribute types
  var attrname = ref.types.CString
  var attrvalue = ref.types.CString
  // define the "attrval" struct type
  var attrstruct = StructType({
    name: attrname,
    value: attrvalue
})

  var img = SVG.createSVGimage(realPath  + reqfname);
  var lstRect = SVG.getRects(img);
  var jsonRect = SVG.rectListToJSON(lstRect);
  jsonRect = "{\"Rectangles\":" +jsonRect + "}";
  var myJsonString = JSON.parse(String(jsonRect));
  if (myJsonString.Rectangles.length > 0)
  {
      for (var i = 0 ; i < myJsonString.Rectangles.length ; i ++) {
          var structAttri = new attrstruct
          structAttri.name = "width";
          structAttri.value = String(parseFloat(myJsonString.Rectangles[i].w) * parseFloat(factorval)) + "cm";
          SVG.setAttribute(img, ElementType.RECT, i, structAttri.ref());
          var structAttri = new attrstruct
          structAttri.name = "height";
          structAttri.value = String(parseFloat(myJsonString.Rectangles[i].h) * parseFloat(factorval)) + "cm";
          SVG.setAttribute(img, ElementType.RECT, i, structAttri.ref());
      }
      var boolimg = SVG.writeSVGimage(img, realPath + reqfname);
  }
  SVG.deleteSVGimage(img);
  if (boolimg == true){
    console.log("succeessfully added!")
    res.send(boolimg);
  }else{
    console.log("file writing failed!")
    res.send(boolimg);
  }
    
 
});
// set the scale of Circle into the existing server SVG file
app.get('/circscale', function(req , res){

  let reqfname = String(req.query.fname);
  let factorval = req.query.factor;
  var realPath = "./uploads/";

  // define the attribute types
  var attrname = ref.types.CString
  var attrvalue = ref.types.CString
  // define the "attrval" struct type
  var attrstruct = StructType({
    name: attrname,
    value: attrvalue
})

  var img = SVG.createSVGimage(realPath  + reqfname);
  var lstCirc = SVG.getCircles(img);
  var jsonCirc = SVG.circListToJSON(lstCirc);
  jsonCirc = "{\"Circles\":" +jsonCirc + "}";
  console.log(jsonCirc);
  var myJsonString = JSON.parse(String(jsonCirc));
  if (myJsonString.Circles.length > 0)
  {
      for (var i = 0 ; i < myJsonString.Circles.length ; i ++) {
          var structAttri = new attrstruct
          structAttri.name = "r";
          structAttri.value = String(parseFloat(myJsonString.Circles[i].r) * parseFloat(factorval)) + "cm";
          SVG.setAttribute(img, ElementType.CIRC, i, structAttri.ref());
      }
      var boolimg = SVG.writeSVGimage(img, realPath + reqfname);
  }
  SVG.deleteSVGimage(img);
  if (boolimg == true){
    console.log("succeessfully added!")
    res.send(boolimg);
  }else{
    console.log("file writing failed!")
    res.send(boolimg);
  }
    
 
});

// update properties of server uploaded svg file
app.get('/updateprop', function(req , res){
  
  let arrupdate = req.query.paramjson;
  var testfile = "testupdate.svg";
  var realPath = "./uploads/";
  var boolimg
  // define the attribute types
  var attrname = ref.types.CString
  var attrvalue = ref.types.CString
  // define the "attrval" struct type
  var attrstruct = StructType({
    name: attrname,
    value: attrvalue
  })
  var img = SVG.createSVGimage(realPath + arrupdate[0]);

  if (String(arrupdate[3]) == "title"){

      var structAttri = new attrstruct
      structAttri.name = String(arrupdate[3]);
      structAttri.value = String(arrupdate[4]);
      SVG.setAttribute(img, ElementType.SVG_IMAGE, 0, structAttri.ref());

  }else if(String(arrupdate[3]) == "description"){
    
      var structAttri = new attrstruct
      structAttri.name = String(arrupdate[3]);
      structAttri.value = String(arrupdate[4]);
      SVG.setAttribute(img, ElementType.SVG_IMAGE, 0, structAttri.ref());

  }else{
        if (arrupdate[1] == "Rectangle"){
          
          var structAttri = new attrstruct
          structAttri.name = String(arrupdate[3]);
          structAttri.value = String(arrupdate[4]);
          SVG.setAttribute(img, ElementType.RECT, Number(arrupdate[2]-1), structAttri.ref());
      
        }else if(arrupdate[1] == "Circle"){
          
          var structAttri = new attrstruct
          structAttri.name = String(arrupdate[3]);
          structAttri.value = String(arrupdate[4]);
          SVG.setAttribute(img, ElementType.CIRC, Number(arrupdate[2]-1), structAttri.ref());
      
        }else if(arrupdate[1] == "Path"){
          
          var structAttri = new attrstruct
          structAttri.name = String(arrupdate[3]);
          structAttri.value = String(arrupdate[4]);
          SVG.setAttribute(img, ElementType.PATH, Number(arrupdate[2]-1), structAttri.ref());
      
        }else if(arrupdate[1] == "Group"){
          
          var structAttri = new attrstruct
          structAttri.name = String(arrupdate[3]);
          structAttri.value = String(arrupdate[4]);
          SVG.setAttribute(img, ElementType.GROUP, Number(arrupdate[2]-1), structAttri.ref());
      
        }

  }
  
  //var boolimg = SVG.writeSVGimage(img, testfile);
  var boolimg = SVG.writeSVGimage(img, realPath + arrupdate[0]);
  SVG.deleteSVGimage(img);

  if (boolimg == true){
    console.log("succeessfully Updated!")
    res.send(boolimg);
  }else{
    console.log("file writing failed!")
    res.send(boolimg);
  }
 
});

// file moving function
var moveFile = (file, dir2)=>{
  //include the fs, path modules
  var fs = require('fs');
  var path = require('path');

  //gets file name and adds it to dir2
  var f = path.basename(file);
  var dest = path.resolve(dir2, f);

  fs.rename(file, dest, (err)=>{
    if(err) throw err;
    else console.log('Successfully moved');
  });
};

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);