// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    var filename ="";
    var booladded = false;
    var reqelement  = "";
    var reqnum = "";
    $('#ninput').val("");
    $('#vinput').val("");

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/images',
        success: function (data) {
            var imgArray = data;
            if (imgArray.length < 1){
                alert("No given uploaded files!!!");
            } else
            {
                for (var i = 0 ; i < imgArray.length ; i ++) {
                    var arrValue= [];
                    arrValue = imgArray[i].value;
                    if(String(arrValue[5]) == "true"){
                        var imgUrl = window.location.href + "uploads/" + arrValue[0];
                        var fileSize = getFileSize(imgUrl)/1000;
                        var newRowContent = "<tr><td><div id=\"image_holder\"><a href=\"" + imgUrl + "\" download><img src=\"" + imgUrl + "\"></a></div></td><td><a href=\"" + arrValue[0] + "\" download>" + arrValue[0] + "</a></td><td>" + Math.round(fileSize) + "KB</td><td>" + arrValue[1] + "</td><td>" + arrValue[2] + "</td><td>" +arrValue[3] +"</td><td>" + arrValue[4] + "</td></tr>";
                        $("#rtbody").append(newRowContent);
                        var newdownlist = "<option>" + arrValue[0] + "</option>";
                        $("#svgs").append(newdownlist);
                        $("#scale-svgs").append(newdownlist);
                        $("#show-svgs").append(newdownlist);
                        
                    }else
                    {
                        console.log("Invalid SVG file detected!!!");
                    }
                    
                }

            }
            document.getElementById("show-svgs").selectedIndex = -1;
        },
        fail: function(error) {
            // Non-200 return, do something with error
            alert("On page load, received error from server");
            console.log(error);
        }
    });
    //click on dropdown list
    //$('#rtable').on('click', 'tbody tr', function(){
    $("#show-svgs").click(function(){
        //get row contents into an array
        var tableData = $(this).children("td").map(function(){
            return $(this).text();
        }).get();

        filename = $('#show-svgs').val();  //the filename of server
        // SVG Info display Ajax Request
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/someendpoint',   //The server endpoint we are connecting to
            data: {
                fname: filename
            },
            success: function (data) {

                var strMeta = String(data).split("+++");

                var strTitle = String(strMeta[0]).split(":")[0];
                var strDesc = String(strMeta[0]).split(":")[1];
                var strValue = JSON.parse(String(strMeta[1]));
                var imgUrl = window.location.href + "uploads/" + filename;
                var svgimg = "<tr><th colspan = \"5\"><img src=\"" + imgUrl + "\"></th></tr>";
                $("#svgimg").empty();
                $("#svgimg").append(svgimg);

                var svgtitle = "<tr><th colspan = \"2\">"+ strTitle +"</th><th colspan = \"3\">"+ strDesc + "</th></tr>";
                $("#headinfo").empty();
                $("#headinfo").append(svgtitle);
                $("#bottominfo").html("");
                if (strValue.Rectangles.length > 0)
                {
                    for (var i = 0 ; i < strValue.Rectangles.length ; i ++) {
                        var attributes = "<tr><td colspan = \"1\">Rectangle "+ (i + 1) +"</td><td colspan = \"3\">Rectangle H = " + strValue.Rectangles[i].h + "Rectangle W = " + strValue.Rectangles[i].w + " Rectangle X = " +strValue.Rectangles[i].x + " Rectangle Y = " +strValue.Rectangles[i].y + "</td><td colspan = \"1\">" + strValue.Rectangles[i].numAttr + "</td></tr>";
                        $("#bottominfo").append(attributes);
                    }
                }
                if (strValue.Circles.length > 0)
                {
                    for (var i = 0 ; i < strValue.Circles.length ; i ++) {
                        var attributes = "<tr><td colspan = '1'>Circle "+ (i + 1) +"</td><td colspan = '3'> Circle r = " + strValue.Circles[i].r + " Circle CX = " +strValue.Circles[i].cx + " Circle CY = " +strValue.Circles[i].cy + "</td><td colspan = '1'>" + strValue.Circles[i].numAttr + "</td></tr>";
                        $("#bottominfo").append(attributes);
                    }
                }
                if (strValue.Paths.length > 0)
                {
                    for (var i = 0 ; i < strValue.Paths.length ; i ++) {
                        var attributes = "<tr><td colspan = \"1\">Path "+ (i + 1) +"</td><td colspan = \"3\"> Path d = " + strValue.Paths[i].d + "</td><td colspan = \"1\">" + strValue.Paths[i].numAttr + "</td></tr>";
                        $("#bottominfo").append(attributes);
                    }
                }
                if (strValue.Groups.length > 0)
                {
                    for (var i = 0 ; i < strValue.Groups.length ; i ++) {
                        var attributes = "<tr><td colspan = \"1\">Group "+ (i + 1) +"</td><td colspan = \"3\"> Group Children = " + strValue.Groups[i].children + "</td><td colspan = \"1\">" + strValue.Groups[i].numAttr + "</td></tr>";
                        $("#bottominfo").append(attributes);
                    }
                }      

            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });
        
        // moving to the head of SVG View Panel
        $('html, body').animate({
            scrollTop: $("#scrolltop").offset().top
        }, 1000);
    });

    //click on view-table body
    $('#viewtable').on('click', 'tbody tr', function(){
        //get row contents into an array
        var tableData = $(this).children("td").map(function(){
            return $(this).text();
        }).get();

        $('#ninput').val("");
        $('#vinput').val("");  

        // SVG Info display Ajax Request
        $.ajax({
            type: 'get',            
            dataType: 'json',       
            url: '/someendpoint',   
            data: {
                fname: filename
            },
            success: function (data) {

                var strMeta = String(data).split("+++");
               console.log(data);
                var strTitle = String(strMeta[0]).split(":")[0];
                strTitle = strTitle.replace(/\"/gi,"");
                var strDesc = String(strMeta[0]).split(":")[1];
                strDesc = strDesc.replace(/\"/gi,"");
                var strValue = JSON.parse(String(strMeta[1]));
                var strElements = String(tableData[0]).split(" ");
                var strElement = String(strElements[0]);
                reqelement = strElement;
                strNum = String(strElements[1]);
                reqnum = strNum;
                booladded = true;

                var svgimg = "<tr id = \"tr_value\"><td>title</td><td >" + strTitle + "</td></tr><tr id = \"tr_value\"><td>description</td><td >" + strDesc + "</td></tr>";
                $("#tr_element").empty();
                $("#tr_element").append(svgimg);

                if (strElement == "Rectangle"){
                    var attributes = "<tr id = \"tr_value\"><td>height</td><td >" + strValue.Rectangles[strNum-1].h + "</td></tr><tr id = \"tr_value\"><td>width</td><td >" + strValue.Rectangles[strNum-1].w + "</td></tr>";
                    attributes = attributes + "<tr id = \"tr_value\"><td>x</td><td >" + strValue.Rectangles[strNum-1].x + "</td></tr><tr id = \"tr_value\"><td>y</td><td >" + strValue.Rectangles[strNum-1].y + "</td></tr>";
                    attributes = attributes + "<tr id = \"tr_value\"><td>units</td><td >" + strValue.Rectangles[strNum-1].units + "</td></tr>";
                    $("#tr_element").append(attributes);
                    console.log(strValue.Rectangles[strNum-1].numAttr);
                    if(Number(strValue.Rectangles[strNum-1].numAttr) > 0)
                    {
                        for(var i = 0; i < Number(strValue.Rectangles[strNum-1].numAttr); i++){
                            var recAttr = "<tr id = \"tr_value\"><td>" + strValue.Rectangles[strNum-1].getAttr[i].name + "</td><td >" + strValue.Rectangles[strNum-1].getAttr[i].value + "</td></tr>";
                            $("#tr_element").append(recAttr);
                        }
                    }

                }else if(strElement == "Circle")
                {
                    var attributes = "<tr id = \"tr_value\"><td>cx</td><td >" + strValue.Circles[strNum-1].cx + "</td></tr><tr id = \"tr_value\"><td>cy</td><td >" + strValue.Circles[strNum-1].cy + "</td></tr>";
                    attributes = attributes + "<tr id = \"tr_value\"><td>r</td><td >" + strValue.Circles[strNum-1].r + "</td></tr><tr id = \"tr_value\"><td> units </td><td >" + strValue.Circles[strNum-1].units + "</td></tr>";
                    $("#tr_element").append(attributes);
                    console.log(strValue.Circles[strNum-1].numAttr);
                    if(Number(strValue.Circles[strNum-1].numAttr) > 0)
                    {
                        for(var i = 0; i < Number(strValue.Circles[strNum-1].numAttr); i++){
                            var circAttr = "<tr id = \"tr_value\"><td>" + strValue.Circles[strNum-1].getAttr[i].name + "</td><td >" + strValue.Circles[strNum-1].getAttr[i].value + "</td></tr>";
                            $("#tr_element").append(circAttr);
                        }
                    }
                }else if(strElement == "Path")
                {
                    var attributes = "<tr id = \"tr_value\"><td>d</td><td >" + strValue.Paths[strNum-1].d + "</td></tr><tr id = \"tr_value\"><td> units </td><td >" + strValue.Paths[strNum-1].units + "</td></tr>";
                    $("#tr_element").append(attributes);
                    console.log(strValue.Paths[strNum-1].numAttr);
                    if(Number(strValue.Paths[strNum-1].numAttr) > 0)
                    {
                        for(var i = 0; i < Number(strValue.Paths[strNum-1].numAttr); i++){
                            var pathAttr = "<tr id = \"tr_value\"><td>" + strValue.Paths[strNum-1].getAttr[i].name + "</td><td >" + strValue.Paths[strNum-1].getAttr[i].value + "</td></tr>";
                            $("#tr_element").append(pathAttr);
                        }
                    }
                }else if(strElement == "Group")
                {
                    var attributes = "<tr id = \"tr_value\"><td> children </td><td>" + strValue.Groups[strNum-1].children + "</td></tr>";
                    $("#tr_element").append(attributes);
                    console.log(strValue.Groups[strNum-1].numAttr);
                    if(Number(strValue.Groups[strNum-1].numAttr) > 0)
                    {
                        for(var i = 0; i < Number(strValue.Groups[strNum-1].numAttr); i++){
                            var pathAttr = "<tr id = \"tr_value\"><td>" + strValue.Groups[strNum-1].getAttr[i].name + "</td><td >" + strValue.Groups[strNum-1].getAttr[i].value + "</td></tr>";
                            $("#tr_element").append(pathAttr);
                        }
                    }
                }  

            },
            fail: function(error) {
                // Non-200 return, do something with error
                alert("On page load, received error from server");
                console.log(error); 
            }
        });
        
        // moving to the head of SVG View Panel
        $('html, body').animate({
            scrollTop: $("#updatehead").offset().top
        }, 1000);
    });
    // when double click an item of updatetable it shows the items below inputbox
    $('#updatetable').on('dblclick', 'tbody tr', function(){
        //get row contents into an array
        var tData = $(this).children("td").map(function(){
            return $(this).text();
        }).get();
        var td = tData[0]+'*'+tData[1];
        $('#ninput').val(tData[0]);
        $('#vinput').val(tData[1]);
    });

    //Add a new attribute into table
    $("#refreshbtn").click(function(){
        if (booladded == false){
            alert("Select an item of SVG element!");
        }else
        {
            $('#ninput').val("");
            $('#vinput').val("");
        }

    });

    

    //create a new simple svg file
    $("#btn-create").click(function(){
        if ($('#fname').val() == ""){
            alert("Input has no value!");
        }else
        {
            $.ajax({
                type: 'get',            
                dataType: 'json',      
                url: '/createsvg',   
                data: {
                    fname: $('#fname').val()
                },
                success: function (data) {
                    if (data == true){
                        alert("A New SVG file successfully created!");
                        window.location.replace(window.location.href);
                    }else{
                        alert("A New SVG file creation failed!")
                        window.location.replace(window.location.href);
                    }
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    alert("On page load, received error from server");
                    console.log(error); 
                }
            });
        }

    });
    // adding rectangle shape into server-svgfile
    $("#btn-rect").click(function(){
        if ($('#rectval').val() == ""){
            alert("Input has no value!");
        }else
        {
            $.ajax({
                type: 'get',            
                dataType: 'json',      
                url: '/addrect',   
                data: {
                    addstr: $('#rectval').val(),
                    fname: $('#svgs').val()
                },
                success: function (data) {
                    if (data == true){
                        alert("A New rectangle value successfully set!");
                        window.location.replace(window.location.href);
                    }else{
                        alert("A New rectangle value set failed!")
                        window.location.replace(window.location.href);
                    }
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    alert("On page load, received error from server");
                    console.log(error); 
                }
            });
        }
        

    });
    // adding circle shape into server-svgfile
    $("#btn-circ").click(function(){
        if ($('#circval').val() == ""){
            alert("Input has no value!");
        }else
        {
            
            $.ajax({
                type: 'get',            
                dataType: 'json',     
                url: '/addcirc', 
                data: {
                    addstr: $('#circval').val(),
                    fname: $('#svgs').val()
                },
                success: function (data) {
                    if (data == true){
                        alert("A New Circle value successfully set!");
                        window.location.replace(window.location.href);
                    }else{
                        alert("A New Circle value set failed!")
                        window.location.replace(window.location.href);
                    }
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    alert("On page load, received error from server");
                    console.log(error); 
                }
            });
        }
        

    });
    // adding Rectangle scale shape into server-svgfile
    $("#btn_rect").click(function(){
        
        var recvalue = document.getElementById("rectscale");
            if ($('#rectscale').val() == "") {
                console.log("abc");
                alert("Input scale value please!")
            }else
            {
                sum = Number(recvalue.value)
                if(Number(sum)) {
                    $.ajax({
                        type: 'get',          
                        dataType: 'json',       
                        url: '/rectscale', 
                        data: {
                            factor: $('#rectscale').val(),
                            fname: $('#scale-svgs').val()
                        },
                        success: function (data) {
                            alert(data);
                            if (data == true){
                                alert("A New Rectangle factor value successfully set!");
                                window.location.replace(window.location.href);
                            }else{
                                alert("A New Rectangle factor value set failed!")
                                window.location.replace(window.location.href);
                            }
                        },
                        fail: function(error) {
                            // Non-200 return, do something with error
                            alert("On page load, received error from server");
                            console.log(error); 
                        }
                    });
                }else {
                    alert("Numbers only, please!");
                };
            }   
    });

    // adding Circle scale shape into server-svgfile
    $("#btn_circ").click(function(){
        
        var circvalue = document.getElementById("circscale");
            if (($('#circscale').val() == "")) {
                alert("Input scale value please!")
            }else
            {
                sum = Number(circvalue.value)
                if(Number(sum)) {
                    $.ajax({
                        type: 'get',          
                        dataType: 'json',     
                        url: '/circscale',  
                        data: {
                            factor: $('#circscale').val(),
                            fname: $('#scale-svgs').val()
                        },
                        success: function (data) {
                            console.log(data);
                            if (data == true){
                                alert("A New Circle factor value successfully set!");
                                window.location.replace(window.location.href);
                            }else{
                                alert("A New Circle factor value set failed!")
                                window.location.replace(window.location.href);
                            }
                        },
                        fail: function(error) {
                            // Non-200 return, do something with error
                            alert("On page load, received error from server");
                            console.log(error); 
                        }
                    });
                }else {
                    alert("Numbers only, please!");
                };
            }   
    });
    //Event listener form example , we can use this instead explicitly listening for events
    //No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});

//get the size of a file
function getFileSize(url)
{
    var fileSize = '';
    var http = new XMLHttpRequest();
    http.open('HEAD', url, false); // false = Synchronous

    http.send(null); // it will stop here until this http request is complete

    // when we are here, we already have a response, b/c we used Synchronous XHR

    if (http.status === 200) {
        fileSize = http.getResponseHeader('content-length');
        //console.log('fileSize = ' + fileSize);
    }

    return fileSize;
}
//Rectangle scale control input value validate
function recValidate() {
    var recvalue = document.getElementById("rectscale");
    sum = Number(recvalue.value)
        if(Number(sum)) {
            
            } else {
                alert("Numbers only, please!");
            };
};
//Circle scale control input value validate
function circValidate() {
    var circvalue = document.getElementById("circscale");
    sum = Number(circvalue.value)
        if(Number(sum)) {
            
            } else {
                alert("Numbers only, please!");
            };
};