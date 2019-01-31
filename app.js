'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

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
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});
//  a trial




//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
      
    } else {
      res.send('');
    }
  });
});

let sharedLib = ffi.Library('./sharedLib', {
  'random': [ 'string', [] ],
  'returnIndiList': ['string', [] ],
  'createSimpleGedcom': [ 'void' ,['string', 'float', 'string', 'string', 'string', 'string'] ],
  'addAnIndividual': ['void', ['string', 'string', 'string'] ],
  'returnDescList': ['string', ['string', 'string', 'string', 'string'] ],
  'ObjectToJSON': ['string', ['string'] ],
  'returnAncList': ['string', ['string', 'string', 'string', 'string'] ],
	
});

let descJSON = sharedLib.returnIndiList();
console.log("ffiTest.js: sharedLib.getDesc() returned "+descJSON+"\n");


app.get('/view', function(req, res){
	var filename = req.query.filename;
	var array;
	
	array = sharedLib.random();
	res.send(array);
	
});

app.get('/Gedcom', function(req, res){
		
		var source = req.query.fname_src;
		console.log(source);
		var version = req.query.lname_src;
		var encoding = req.query.country;
		var subName = req.query.subLastName;
		var subAddr = req.query.subAddr;
		var filename1 = req.query.Gfilename;
		
		
		sharedLib.createSimpleGedcom(source, version, encoding, filename1, subName, subAddr);
		res.redirect('/');
		
});



app.get('/getDesc1', function(req, res){
	
	var firstname = req.query.firstname_desc;
	console.log('ygcyedcvyefgv ' + firstname + "\n\n\n\n\n\n\n\n");
	var surname = req.query.lastname_desc;
	console.log(surname);
	var filename = req.query.filename_indi11;
	console.log(filename);
	var maxGen = req.query.maxGen_desc;
	var  str;
	
	str =  JSON.parse(sharedLib.returnDescList(filename, firstname, surname, maxGen)) ;
	console.log('In the desc ' + str);
	res.send(str);	
	
});

app.get('/getAnc', function(req, res){
	
	var filename = req.query.filenameAnc;
	console.log(filename);
	var firstname = req.query.firstname_anc;
	console.log(firstname);
	var surname = req.query.lastname_anc;
	var maxGen = req.query.maxGen_anc;
	
	
	var str = JSON.parse(sharedLib.returnAncList(filename, firstname, surname, maxGen));
	console.log(str);
	res.send(str);
	
	
});

app.get('/createIndiv', function(req, res){
	
	var firstName = req.query.firstname_indi;
	console.log('the name is ' + firstName);
	var surname = req.query.lnameIndi;
	//var filename = "minValid.ged";
	var filename = req.query.filename_indi;
	console.log('passed filename is :' + filename + "\n");
	
	sharedLib.addAnIndividual(firstName, surname, filename);
	res.redirect('/');
	
});

app.get('/glist', function(req, res){
	var filename = req.query.filename;
	var array = new Array();
	//array = JSON.parse(sharedLib.returnIndiList()) ;
	res.send(array);
});




//******************** Your code goes here ******************** 

//Sample endpoint
app.get('/someendpoint', function(req , res){
  res.send({
    foo: "bar"
  });
});



app.get('/tr4.jpg',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/tr4.jpg'));
});

app.get('/bar.jpg',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/bar.jpg'));
});

app.get('/files',function(req,res){
	var files = fs.readdirSync('./uploads/');
	
	console.log(files.length);	 
	console.log(files);
	res.send(files);
});

app.get('/uploadsList', function(req, res){
	
	var files = fs.readdirSync('./uploads');
	var fileData  = new Array();
	for(var i = 0; i < files.length; i++){
			console.log(files[i]);
			var temp = JSON.parse(sharedLib.ObjectToJSON(files[i]) );
			fileData.push(temp);
	}
	res.send(fileData);
	
});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
