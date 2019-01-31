// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    
    function validateCreateGedcom(){
    var header  = document.getElementById('fname').value;
    
    if(header.length < 10){
        alert('enter something');
        return false;
    }
    return true;
}
    
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {

            console.log(data); 
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
   });
   
   
   
   $.ajax({
        type: 'get',           
        dataType: 'json',      
        url: '/uploadsList',   
        success: function (data) {
           
           for(var i = 0; i < data.length; i++){
				
				//uploadFunction(data[i]);
				var tb  =  "<tbody><tr>"
				+	"<td><a href = \"" + data[i].fileName + "\">" + data[i].fileName.slice(10) + "</a></td>"
				+	"<td>" + data[i].source + "</td>"
				+	"<td>" + data[i].version + "</td>"
				+	"<td>" + data[i].encoding +  "</td>"
				+	"<td>" + data[i].subName +  "</td>"
				+	"<td>" + data[i].subAddr +  "</td>"
				+	"<td>" + data[i].numIndi +  "</td>"
				+	"<td>" + data[i].numFam +  "</td>"
				+ 	"</tr></tbody>"
				
			   $("#customers").append(tb);
			}
			

           //We write the object to the console to show that the request was successful
            console.log(data); 
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
   });
   
   
   $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/view',   //The server endpoint we are connecting to
        success: function (data) {
            
            console.log(data); 
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
   });
   
   
   
   $('#myId').submit(function(event){
	var firstname = $('#fnameDesc').val();
	console.log('In here\n');
	var surname = $('#lnameDesc').val();
	var maxGen = $('#maxGenDesc').val();
	var filename = $('#filename_indi11').val();
	$('#customers_1').empty();
	
	  $.ajax({
        type: 'get',          
        dataType: 'json',       
        url: '/getDesc1', 
        data:{firstname_desc: firstname, lastname_desc: surname, filename_indi11:filename, maxGen_desc:maxGen}, //The server endpoint we are connecting to
        success: function (data) {
			//console.log(('The data is ' + data);
			//let obj = JSON.parse(data);
			let obj = data;;
			var i = 1; 
			
			for(j in obj){
				
					var gen = "";
					var list = obj[j];
					for(index in list){
							gen += list[index].givenName + " " +  list[index].surname + ", ";
							var tb = "<tbody><tr>"
							+ "<td>" + gen + "</td>"
							+ "<td>" + i + "</td>"
							
							+ 	"</tr></tbody>"
							
							
					}
					//$('#customers_1').after('<tr>' + '<td>' + i + '<td>' + '</td>' + gen)
					$('#customers_1').append(tb);
					i++;
			}		
			
			
            
        },
        fail: function(error) {
           // console.log('Failed!' + data);
      
        }
    });
    event.preventDefault();
	
 });
 
 
 
 $('#myId2').submit(function(event){
	var firstname = $('#fnameAnc').val();
	console.log(firstname);
	console.log('In here\n');
	var surname = $('#lnameAnc').val();
	var maxGen = $('#maxGenAnc').val();
	var filename = $('#filename_indi2').val();
	console.log(filename);
	//$('#customers_2').empty();

	  $.ajax({
        type: 'get',          
        dataType: 'json',       
        url: '/getAnc', 
        data:{firstname_anc: firstname, lastname_anc: surname, filenameAnc:filename, maxGen_anc:maxGen}, //The server endpoint we are connecting to
        success: function (data) {
			
			
			let obj = data;
			var i = 1; 
			
			for(j in obj){
				
					var gen = "";
					var list = obj[j];
					for(index in list){
							gen += list[index].givenName + " " +  list[index].surname + ", ";
							var tb = "<tbody><tr>"
							+ "<td>" + gen + "</td>"
							+ "<td>" + i + "</td>"
							
							+ 	"</tr></tbody>"
							
							
					}
					//$('#customers_1').after('<tr>' + '<td>' + i + '<td>' + '</td>' + gen)
					$('#customers_2').append(tb);
					i++;
			}		
			
			
            
        },
        fail: function(error) {
           // console.log('Failed!' + data);
      
        }
    });
    event.preventDefault();
	
 });
   
   // testing the glist
     $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/glist',   //The server endpoint we are connecting to
        success: function (data) {
            
            //console.log(data); 
        },
        fail: function(error) {
            // Non-200 return, do something with error
            //console.log(error); 
        }
   });
   
   
   $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/uploadsList',   //The server endpoint we are connecting to
        success: function (data) {
			
			for (var i = 0; i < data.length; i++) {
	
				//console.log(data[i]);
			//($('#customers')).append('<tr align = \'center\'><th align = \'center\'>'+data[i].fileName.link('/upload/' + data[i].fileName)+'</th><th></th></tr>');
				//uploadFunction(data[i]);
				
				
			}
           // console.log(data);
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
    
  
    
    
    
    
     $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/files',   //The server endpoint we are connecting to
        success: function (data) {
			var emp = "";
			for (var i = 0; i < data.length; i++) {

                emp += "<option>" + data[i]  + "</option>"
								
			}
            //console.log(data);
            var drop = document.getElementById('filename_indi');
            var drop1 = document.getElementById('filename_indi11');
            var drop2 = document.getElementById('filename_indi2');
            drop.innerHTML = emp;
            drop1.innerHTML = emp;
            drop2.innerHTML = emp;
            
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });


    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        e.preventDefault();
        $.ajax({});
    });
    function myFunction() {
    var x = document.getElementById("myFile");
    x.disabled = true;
}
});


 function myFunction() {
    var table = document.getElementById("customers");
    var row = table.insertRow(table.length);
     var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    var cell4 = row.insertCell(3);
    var cell5 = row.insertCell(4);
    var cell6 = row.insertCell(5);
    var cell7  = row.insertCell(6);
    var cell8 = row.insertCell(7);
    cell1.innerHTML = "Hello world";
}

 
function validateCreateGedcom(){
    
    var header = document.forms['form1']['firstname'];
    var Header_error = document.getElementById('Header_error');
    
    var version = document.forms['form1']['Hversion'];
    var version_error = document.getElementById('version_error');
    
    var subName = document.forms['form1']['sLastName'];
    var subNameError = document.getElementById('sub_name_error');
    
    var subAddr = document.forms['form1']['subAddr'];
    var subAddrError = document.getElementById('sub_add_error');
    
    var filename = document.forms['form1']['filename_Gedcom'];
    var fnameError = document.getElementById('fname_add_error');
    
    var test = 0;
    
    test  = 0;
    
    if (header.value == "") {
    header.style.border = "2px solid red";
    document.getElementById('Header_error').style.color = "red";
    Header_error.textContent = "Username is required!";
    header.focus();
        test = test + 1;
    //return false;
  }
else{
    Header_error.textContent = "";
    header.style.border = "0px";
}
    if(version.value == ""){
        version.style.border = "2px solid red";
        document.getElementById('version_error').style.color = "red";
        version_error.textContent = "Version is Required!";
        version.focus();
        test = test + 1;
    }
    else{
        version.style.border = "0px";
        version_error.textContent = "";
    }
    
    if(subName.value == ""){
        subName.style.border = "2px  solid red";
        document.getElementById('sub_name_error').style.color = "red";
        subNameError.textContent = "Submitter Name Required!";
        subName.focus();
        test = test + 1;
    }
    else{
        subNameError.textContent = "";
        subName.style.border = "0px";
    }
    
    if(subAddr.value == ""){
        subAddr.style.border = "2px solid red";
        document.getElementById('sub_add_error').style.color = "red";
        subAddrError.textContent = "Submitter Address Required!";
        subAddr.focus();
        test = test + 1;
    }
    else{
        subAddrError.textContent = "";
        subAddr.style.border = "0px";
    }
    
    if(filename.value == ""){
        filename.style.border = "2px solid red";
        document.getElementById('fname_add_error').style.color = "red";
        fnameError.textContent = "Enter a Filename!";
        filename.focus();
        test = test + 1;
    }
    else{
        fnameError.textContent = "";
        filename.style.border = "0px";
    }
    
    if(filename.value !=  ""){
            var n = filename.value.includes(".ged", filename.value.length-5);
            var cmp = ".ged";
            
            if(n == false){
                filename.style.border = "2px solid red";
                document.getElementById('fname_add_error').style.color = "red";
                fnameError.textContent = "Invalid Filename!";
                filename.focus();
                test = test + 1;
            }
            
        
    }
    
    
    
    if(test == 0){
        return true;
    }
    else{
        return false;
    }
    
}

function validateSubmitIndi(){
    
    var firstName = document.forms['form2']['firstname_indi'];
    var fNameError = document.getElementById('fname_indi_error');
    
    var  lastName = document.forms['form2']['lastname_indi'];
    var lNameError = document.getElementById('lname_indi_error');
    
    var test = 0;
    
    if(firstName.value == ""){
        firstName.style.border = "2px solid red";
        document.getElementById('fname_indi_error').style.color = "red";
        fNameError.textContent = "First Name Required!";
        firstName.focus();
        test = test + 1;
    }
    else{
        firstName.style.border = "0px";
        fNameError.textContent = "";
    }
    
    if(lastName.value == ""){
        lastName.style.border = "2px solid red";
        document.getElementById('lname_indi_error').style.color = "red";
        lNameError.textContent =  "Last Name Required!";
        lastName.focus();
        test++;
    
    }
    else{
        lastName.style.border = "0px";
        lNameError.textContent = "";    
    }
    
    if(test == 0){
        return true;
    }
    else{
        return false;
    }
}

function validateGetDescendants(){
    
    var firstName = document.forms['form3']['firstname_desc'];  
    var fnameError = document.getElementById('first_name_desc_error');
    
    var lastName = document.forms['form3']['lastname_desc'];
    var lnameError = document.getElementById('last_name_desc_error');
    
    var maxGen = document.forms['form3']['maxGen_desc'];
    var maxGenError = document.getElementById('maxGen_desc_error');
    
    var filename = document.getElementById('filename_indi11');
    
    
    var test = 0;
    
    if(firstName.value == ""){
        firstName.style.border = "2px solid red";
        document.getElementById('first_name_desc_error').style.color = "red";
        fnameError.textContent =  "First Name Required";
        firstName.focus();
        test = test + 1;
        
    }
    else{
        firstName.style.border = "0px";
        fnameError.textContent = "";
    }
    
    if(lastName.value == ""){
        lastName.style.border = "2px solid red";
        document.getElementById('last_name_desc_error').style.color = "red";
        lnameError.textContent = "Last Name Required!";
        lastName.focus();
        test = test + 1;
    }
    else{
        lastName.style.border = "0px";
        lnameError.textContent = "";
    }
    
    if(maxGen.value == ""){
        maxGen.style.border = "2px solid red";
        document.getElementById('maxGen_desc_error').style.color = "red";
        maxGenError.textContent = "Max Gen Required.. >= 0";
        maxGen.focus();
        test = test + 1;
    }
    else{
        maxGen.style.border = "0px";
        maxGenError.textContent = "";
        
        var num = Number(maxGen.value);
        if(num < 0){
            maxGen.style.border = "2px solid red";
            document.getElementById('maxGen_desc_error').style.color = "red";
            maxGenError.textContent = "Max Gen Required.. >= 0";
            maxGen.focus();
            test = test + 1;
            
        }
    }
    
    if(test == 0){
		//ajax call here
		//use data: { params : params },
		//$.ajax({
		//	type: 'get',            //Request type
		//	dataType: 'json',       //Data type - we will use JSON for almost everything 
		//	url: '/getDesc', 
		//	data: { filename: filename.value, firstname: firstName.value, surname: lastName.value, maxGen: maxGen.value },  //The server endpoint we are connecting to
		//	success: function (data) {
		//		console.log('In the Ajax ****************************' + data); 
		//	},
		//	fail: function(error) {
				// Non-200 return, do something with error
			//	console.log(error); 
		//	}
	   
        return true;
    }
    else{
        return false;
    }
}


		


function validateGetAncestors(){
    
        var firstName = document.forms['form4']['firstname_anc'];
    var fnameError = document.getElementById('first_name_anc_error');
    
    var lastName = document.forms['form4']['lastname_anc'];
    var lnameError = document.getElementById('last_name_anc_error');
    
    var maxGen = document.forms['form4']['maxGen_anc'];
    var maxGenError = document.getElementById('maxGen_anc_error');
    
    
    var test = 0;
    
    if(firstName.value == ""){
        firstName.style.border = "2px solid red";
        document.getElementById('first_name_anc_error').style.color = "red";
        fnameError.textContent =  "First Name Required";
        firstName.focus();
        test = test + 1;
        
    }
    else{
        firstName.style.border = "0px";
        fnameError.textContent = "";
    }
    
    if(lastName.value == ""){
        lastName.style.border = "2px solid red";
        document.getElementById('last_name_anc_error').style.color = "red";
        lnameError.textContent = "Last Name Required!";
        lastName.focus();
        test = test + 1;
    }
    else{
        lastName.style.border = "0px";
        lnameError.textContent = "";
    }
    
    if(maxGen.value == ""){
        maxGen.style.border = "2px solid red";
        document.getElementById('maxGen_anc_error').style.color = "red";
        maxGenError.textContent = "Max Gen Required.. >= 0";
        maxGen.focus();
        test = test + 1;
    }
    else{
        maxGen.style.border = "0px";
        maxGenError.textContent = "";
        
        var num = Number(maxGen.value);
        if(num < 0){
            
            maxGen.style.border = "2px solid red";
            document.getElementById('maxGen_anc_error').style.color = "red";
            maxGenError.textContent = "Max Gen Required.. >= 0";
            maxGen.focus();
            test = test + 1;
            
        }
    }
    
    if(test == 0){
        return true;
    }
    else{
        return false;
    }
    
}

function uploadFunction(fullPath){
	
	var table = document.getElementById("customers");
    
    var drop = document.getElementById('filename_indi');
	
	
	if (fullPath) {
    var startIndex = (fullPath.indexOf('\\') >= 0 ? fullPath.lastIndexOf('\\') : fullPath.lastIndexOf('/'));
    var filename = fullPath.substring(startIndex);
    if (filename.indexOf('\\') === 0 || filename.indexOf('/') === 0) {
        filename = filename.substring(1);
    }
    //alert(filename);
    var row = table.insertRow(table.length);
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    var cell4 = row.insertCell(3);
    var cell5 = row.insertCell(4);
    var cell6 = row.insertCell(5);
    var cell7 = row.insertCell(6);
    var cell8 = row.insertCell(7);
        
    var e = "";
    var n = e += "<option>" +filename + "</option>"
        
    
    cell1.innerHTML  = filename.link('/uploads/' + fullPath);
    
    
    
    document.getElementById('statPanel').value += filename + " uploaded Sucecssfully\n";
    document.getElementById('statPanel').style.color = "black";
    
   
	}
	
}
  
  


function validateFilename(){
		var file = document.getElementById('myFile');
		document.getElementById('statPanel').value += file.value  + "Downloaded Successfully";
		
		return true;
}




