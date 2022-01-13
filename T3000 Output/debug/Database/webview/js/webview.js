
var canvas=document.getElementById("canvas");
var ctx=canvas.getContext("2d");
var $canvas=$("#canvas");
var canvasOffset=$canvas.offset();
var offsetX=canvasOffset.left;
var offsetY=canvasOffset.top;
var selectedGraphicName = "";
var loadedGraphicName ="";
var translatePos = {
    x: $canvas.width / 2,
    y: $canvas.height / 2
};

var scale = 1.0;
var scaleMultiplier = 0.1;
var startDragOffset = {};
var lastScale = 1.0;

var $inputElement=$("#inputElement");
var $outputElement=$("#outputElement");
var $variableElement=$("#variableElement");
var $schedualElement=$("#schedualElement");
var $holidayElement=$("#holidayElement");
var $programElement=$("#programElement");
var $graphicElement =$("#graphicElement");


var $box_3=$("#box_3");
var $circleBtn=$("#circleBtn");
 
var $selectBtn=$("#selectBtn");
var $doBtn=$("#doBtn");
var $unDoBtn=$("#unDoBtn");
var $deleteBtn=$("#deleteBtn");


var $menu=$('#contextMenu');
var isMouseDown = false; 
var selectedDragableObject;

var Images=[];
var DoElements=[];
var SelectedImage;

var BackGroundImage ="";
var BackGroundImageWidth;
var BackGroundImageHeight;

$inputElement.draggable({
    helper:'clone',
});
$outputElement.draggable({
    helper:'clone',
});
$variableElement.draggable({
    helper:'clone',
});
$schedualElement.draggable({
    helper:'clone',
});
$holidayElement.draggable({
    helper:'clone',
});
$programElement.draggable({
    helper:'clone',
});
$graphicElement.draggable({
    helper:'clone',
});
var rec_toggled = false;
var circle_toggled = false;
function removelistner()
{
}
function resetButtonToggles()
{
	if(rec_toggled)
		$box_3.click();
		
	if(circle_toggled)
		$circleBtn.click();
}
$selectBtn.click(function(){
    SetdefaultCanvasEvents(true);
});
$deleteBtn.click(function(){
   
    if (selectedDragableObject > -1) {
        DoElements.push(Images[selectedDragableObject]);
        Images.splice(selectedDragableObject, 1);
        selectedDragableObject = -1;
    }
	
	drawRectangleOnCanvas.drawAll(scale, translatePos);
});
$doBtn.click(function(){
    if(DoElements.length > 0){
        Images.push(DoElements.pop());
        drawRectangleOnCanvas.drawAll(scale, translatePos);
    }
});

$unDoBtn.click(function(){
    if(Images.length > 0){    
        DoElements.push(Images.pop());
        drawRectangleOnCanvas.drawAll(scale, translatePos);
    }
});
$box_3.click(function(){
		if(!rec_toggled)
		{
			//alert ("enabled")
			resetButtonToggles();
			rec_toggled = true;
			$box_3.attr('style',' box-shadow: 0 0 5px -1px rgba(0,0,0,0.6);');
			canvas.addEventListener("mousemove", drawRectangleOnCanvas.handleMouseMove, false);
			canvas.addEventListener("mousedown", drawRectangleOnCanvas.handleMouseDown, false);
			canvas.addEventListener("mouseup", drawRectangleOnCanvas.handleMouseUp, false);
			canvas.addEventListener("mouseout", drawRectangleOnCanvas.handleMouseOut, false);
            
            SetdefaultCanvasEvents(false);
		}
		else
		{
			rec_toggled = false;
			$box_3.attr('style',' box-shadow: 0 0 0 0;');
			//alert("called");
			
			canvas.removeEventListener('mousemove', drawRectangleOnCanvas.handleMouseMove, false);
			canvas.removeEventListener('mousedown', drawRectangleOnCanvas.handleMouseDown, false);
			canvas.removeEventListener('mouseup', drawRectangleOnCanvas.handleMouseUp, false);
			canvas.removeEventListener('mouseout', drawRectangleOnCanvas.handleMouseOut, false);
            SetdefaultCanvasEvents(true);
			
		}
});
$circleBtn.click(function(){
		if(!circle_toggled)
		{
			//alert ("enabled")
			resetButtonToggles();
			circle_toggled = true;
			$circleBtn.attr('style',' box-shadow: 0 0 5px -1px rgba(0,0,0,0.6);');
			canvas.addEventListener("mousemove", drawCircleOnCanvas.handleMouseMove, false);
			canvas.addEventListener("mousedown", drawCircleOnCanvas.handleMouseDown, false);
			canvas.addEventListener("mouseup", drawCircleOnCanvas.handleMouseUp, false);
			canvas.addEventListener("mouseout", drawCircleOnCanvas.handleMouseOut, false);
            SetdefaultCanvasEvents(false);
		}
		else
		{
			circle_toggled = false;
			$circleBtn.attr('style',' box-shadow: 0 0 0 0;');
			//alert("called");
			
			canvas.removeEventListener('mousemove', drawCircleOnCanvas.handleMouseMove, false);
			canvas.removeEventListener('mousedown', drawCircleOnCanvas.handleMouseDown, false);
			canvas.removeEventListener('mouseup', drawCircleOnCanvas.handleMouseUp, false);
			canvas.removeEventListener('mouseout', drawCircleOnCanvas.handleMouseOut, false);
            
            SetdefaultCanvasEvents(true);
			
		}
});
// set the data payload

containsPoint = function (rect, x, y) {
    return !(x < rect.x ||
             x > rect.x + rect.width ||
             y < rect.y ||
             y > rect.y + rect.height);
  };
function IsElementSelected(x,y)
{
    for (var i = 0; i < Images.length; i++) {
        var rect = {
            x: Images[i].x,
            y: Images[i].y,
            width: Images[i].width,
            height: Images[i].height
        }
    if (containsPoint(rect, x, y)) {
        selectedDragableObject = i;
        return true;
      }
    }
    return false
}


var defaultCanvasEvents = {
	handleMouseDown: function(e) {
		var rect = canvas.getBoundingClientRect();
			x1 = e.clientX - rect.left;
			y1 = e.clientY - rect.top;
            
            selectedDragableObject = -1;
            IsElementSelected(x1,y1)
            
            isMouseDown = true;

           
            if(selectedDragableObject == -1){
                
                showContextMenu();
                startDragOffset.x = e.clientX - translatePos.x;
                startDragOffset.y = e.clientY - translatePos.y;
                console.log("trnslate Pos:",translatePos);
            }
            
            if(selectedDragableObject && LockEnabled ){
                if(Images[selectedDragableObject].object_type == 6)
                {
                    var request = {
                        'action':7,
                        'graphic_name' : Images[selectedDragableObject].object_name
                    };
                    loadedGraphicName = Images[selectedDragableObject].object_name;
                    window.chrome.webview.postMessage(request);	 
                    selectedDragableObject = -1;
                }
               else if(Images[selectedDragableObject].object_type > 2){
                    var request = {
                        'action':8,
                        'object_type' :Images[selectedDragableObject].object_type,
                        'index' :Images[selectedDragableObject].object_variable
                    };
                    window.chrome.webview.postMessage(request);
                    selectedDragableObject = -1;
                }else{
                    canvas.addEventListener("mousemove", defaultCanvasEvents.handleMouseMove, false);
                    canvas.addEventListener("mouseup", defaultCanvasEvents.handleMouseUp, false);
                   
                }  
            }else{
                canvas.addEventListener("mousemove", defaultCanvasEvents.handleMouseMove, false);
                canvas.addEventListener("mouseup", defaultCanvasEvents.handleMouseUp, false);
               
            }   
             drawRectangleOnCanvas.drawAll(scale, translatePos);
		},
	handleMouseUp: function(e) {
		
        isMouseDown = false;
        canvas.removeEventListener("mousemove", defaultCanvasEvents.handleMouseMove, false);
        canvas.removeEventListener("mouseup", defaultCanvasEvents.handleMouseUp, false);
        //selectedDragableObject = -1;
        
	},
	handleMouseMove: function(e) {
		if (!isMouseDown) return;

		var rect = canvas.getBoundingClientRect();

        if(selectedDragableObject == -1){
            translatePos.x = e.clientX - startDragOffset.x;
            translatePos.y = e.clientY - startDragOffset.y;
            console.log("event:",e);
            console.log("offset Pos:",startDragOffset);
            console.log("trnslate Pos:",translatePos);
        }else{
            startX = parseInt(e.clientX - recOffsetX);
            startY = parseInt(e.clientY - recOffsetY);
            Images[selectedDragableObject].x = startX;
            Images[selectedDragableObject].y = startY;
        }
        drawRectangleOnCanvas.drawAll(scale, translatePos);
		
	},
	handleMouseOut: function(e) {
		// tell the browser we're handling this event
		e.preventDefault();
		e.stopPropagation();


		// Put your mouseOut stuff here
	  },
      handlecContextmenu: function(e) {
		// tell the browser we're handling this event
		e.preventDefault();
		e.stopPropagation();

        var rect = canvas.getBoundingClientRect(),
        
        startX = parseInt(e.clientX - recOffsetX);
        startY = parseInt(e.clientY - recOffsetY);
        if(IsElementSelected(startX,startY))
            showContextMenu(rect,startX+30,startY);
        else
            showContextMenu();
	  }
}
function SetdefaultCanvasEvents(Flag)
{
    if(Flag)
    {

        canvas.addEventListener("mousedown", defaultCanvasEvents.handleMouseDown, false);
        canvas.addEventListener("contextmenu", defaultCanvasEvents.handlecContextmenu, false);
    }else{

        canvas.removeEventListener("mousedown", defaultCanvasEvents.handleMouseDown, false);
        canvas.removeEventListener("contextmenu", defaultCanvasEvents.handlecContextmenu, false);
    }

}
$canvas.droppable({
    drop:dragDrop,
});
SetdefaultCanvasEvents(true);

function dragDrop(e,ui){
    SelectedImage ="";

    var element=ui.draggable;
    var x=parseInt(ui.offset.left-offsetX);
    var y=parseInt(ui.offset.top-offsetY);
    var imagePath = element.prop('src');
    var ImageName = imagePath.replace(/^.*[\\\/]/, '');
    var objectType = element.attr("object-type");
    
	console.log("Src: images/"+ImageName);
    var image = {
        x: x,
        y: y,
        width: 50,
        height: 50,
        image: "images/"+ImageName,
        type : "image",
        color: "red",
        fontType:1,
        fontSize:"11px",
        iconSize:1,
        textPos:1,
        object_display_type : 0,
        object_type: objectType
      }
      SelectedImage = image;
      if(objectType == 6)
      {
        image["type"]="graphic";
        var request = {
            'action':6
        };
        window.chrome.webview.postMessage(request);	 
        $("#selectGraphicSelect").show();
        $("#selectGraphicLoad").hide();
      }else{
        $('#object_details').hide();        
        $("#object_type").val(objectType);
          ReadVariablesFromDevice(parseInt(objectType));
        $('#selectVariableModel').modal('show');
      }
      
}




function reOffset() {
  var BB = canvas.getBoundingClientRect();
  recOffsetX = BB.left;
  recOffsetY = BB.top;
}
var recOffsetX, recOffsetY;
reOffset();
window.onscroll = function(e) {
  reOffset();
}
window.onresize = function(e) {
  reOffset();
}

var isRecDown = false;
var startX, startY;
var strokeWidth = 4;
drawCount = 1;
var rects = [];
var newRect;
var drawRectangleOnCanvas = {
  handleMouseDown: function(e) {
    // tell the browser we're handling this event
    e.preventDefault();
    e.stopPropagation();

    startX = parseInt(e.clientX - recOffsetX);
    startY = parseInt(e.clientY - recOffsetY);

    // Put your mousedown stuff here
    isRecDown = true;
  },

  handleMouseUp: function(e) {
    // tell the browser we're handling this event
    e.preventDefault();
    e.stopPropagation();

    mouseX = parseInt(e.clientX - recOffsetX);
    mouseY = parseInt(e.clientY - recOffsetY);

    // Put your mouseup stuff here
    isRecDown = false;

    //if(!willOverlap(newRect)){
    Images.push(newRect);
    //}
    drawRectangleOnCanvas.drawAll(scale, translatePos);
  },

  drawAll: function(scale,translatePos) {
  console.log("Draw Called");
  console.log("scale",scale);
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.save();
    console.log(translatePos)
   // ctx.translate(translatePos.x, translatePos.y);
   if(scale != lastScale){
    ctx.scale(scale, scale);
    lastScale = scale
   }

    //ctx.drawImage(base_image, 0, 0);
    ctx.lineWidth = strokeWidth;
    /*if(BackGroundImage){
        var tempImage=new Image();
        tempImage.src = BackGroundImage;
        tempImage.width =BackGroundImageWidth;
        tempImage.height = BackGroundImageHeight;
        //alert("background")
		ctx.drawImage( tempImage ,0,0,BackGroundImageWidth,BackGroundImageHeight);
    }*/
    var text = loadedGraphicName;
           ctx.fillStyle = "#63B1EC";
		  var font = "bold 12px serif";
		  ctx.font = font;
		  var width = ctx.measureText(text).width;
		  var height = ctx.measureText("w").width; //
		  ctx.fillText(text, 50 , 18 );
    for (var i = 0; i < Images.length; i++) {
        if(Images[i].type=="BGIMAGE"){
            if(selectedDragableObject == i)
            {
                ctx.strokeStyle =  "#63B1EC";
                ctx.globalAlpha = 1;
                ctx.strokeRect(Images[i].x, Images[i].y, Images[i].width , Images[i].height );
                    
                ctx.beginPath();
                ctx.arc(Images[i].x, Images[i].y, 15, 0, Math.PI * 2, true);
                ctx.closePath();
            }
            var tempImage=new Image();
            tempImage.src = Images[i].src;
            tempImage.width =Images[i].width;
            tempImage.height = Images[i].height;
            //alert("background")
            ctx.drawImage( tempImage ,Images[i].x,Images[i].y,Images[i].width,Images[i].height);
        }
		else if(Images[i].type == "rectangle"){
		  var r = Images[i];
          if(selectedDragableObject == i)
          ctx.strokeStyle = "#63B1EC";
          else
		  ctx.strokeStyle = r.color;
		  ctx.globalAlpha = 1;
		  ctx.strokeRect(r.x, r.y, r.width , r.height );
            
		  ctx.beginPath();
		  ctx.arc(r.x, r.y, 15, 0, Math.PI * 2, true);
		  ctx.closePath();
		  //ctx.fillStyle = r.color;
		 // ctx.fill();

		  /*var text = drawCount;
		  ctx.fillStyle = "blue";
		  var font = "bold " + 20 + "px serif";
		  ctx.font = font;
		  var width = ctx.measureText(text).width;
		  var height = ctx.measureText("w").width; // this is a GUESS of height
		  ctx.fillText(text, r.x , r.y -2 );*/
		  
		}else if(Images[i].type === "circle"){
			var r = Images[i];
            var color = r.color
            if(selectedDragableObject == i)
                color = "#63B1EC";
			drawEllipse(r.x, r.y, (r.x+(r.width*2)), (r.y+(r.height*2)),color);
		}
		else if(Images[i].type === "image"){
            if(selectedDragableObject == i)
            {
                ctx.strokeStyle =  "#63B1EC";
                ctx.globalAlpha = 1;
                ctx.strokeRect(Images[i].x, Images[i].y, Images[i].width , Images[i].height );
                    
                ctx.beginPath();
                ctx.arc(Images[i].x, Images[i].y, 15, 0, Math.PI * 2, true);
                ctx.closePath();
            }
            var tempImage=new Image();
            tempImage.src = Images[i].image;
            tempImage.width =60;
            tempImage.height = 60;

			ctx.drawImage( tempImage ,Images[i].x,Images[i].y,Images[i].width, Images[i].height);
            var text ="";
            if(Images[i].object_display_type > 0)
            {
			     text = Images[i].value+" | "+Images[i].unit;//drawCount;
                if(Images[i].object_type == 5) // for programs
                {
                    if(Images[i].status)
                        text = "OFF";
                    else 
                        text = "ON";
                }
            }
            if((Images[i].object_type ==3 || Images[i].object_type == 4)&& Images[i].object_display_type < 3) // for scedule and calenders
            {
                text = Images[i].object_name
            }
            else if(Images[i].object_display_type == 2)
            {
                text += " | "+Images[i].object_name
            }
            if(Images[i].object_display_type == 2){
                text += " | "+Images[i].object_label
            }
		  ctx.fillStyle = Images[i].color;
		  var font = "bold " + Images[i].fontSize + "px serif";
		  ctx.font = font;
		  var width = ctx.measureText(text).width;
		  var height = ctx.measureText("w").width; // this is a GUESS of height
            var posFactorX = 0;
            var posFactorY = 0;
          switch(Images[i].textPos)
        {
            case 1:
                posFactorY = -5;
                break;
            case 2:
                posFactorY = Images[i].height + 15;
                break;
            case 3:
                posFactorX = Images[i].width + 15;
                posFactorY = Images[i].height/2;
                break;
            case 4:
                posFactorX =  -width;
                posFactorY = Images[i].height /2;
                break;
        }
		  ctx.fillText(text, Images[i].x + posFactorX , Images[i].y + posFactorY );
		 
		}
        else if(Images[i].type=="graphic"){
            if(selectedDragableObject == i)
            {
                ctx.strokeStyle =  "#63B1EC";
                ctx.globalAlpha = 1;
                ctx.strokeRect(Images[i].x, Images[i].y, Images[i].width , Images[i].height );
                    
                ctx.beginPath();
                ctx.arc(Images[i].x, Images[i].y, 15, 0, Math.PI * 2, true);
                ctx.closePath();
            }
            var tempImage=new Image();
            tempImage.src = Images[i].image;
            tempImage.width =60;
            tempImage.height = 60;

			ctx.drawImage( tempImage ,Images[i].x,Images[i].y,Images[i].width, Images[i].height);
           var text = Images[i].object_name;
           ctx.fillStyle = "#63B1EC";
		  var font = "bold 12px serif";
		  ctx.font = font;
		  var width = ctx.measureText(text).width;
		  var height = ctx.measureText("w").width; //
		  ctx.fillText(text, Images[i].x + posFactorX , Images[i].y + Images[i].height + 15 );
        }
    }
  },

  handleMouseOut: function(e) {
    // tell the browser we're handling this event
    e.preventDefault();
    e.stopPropagation();

    mouseX = parseInt(e.clientX - recOffsetX);
    mouseY = parseInt(e.clientY - recOffsetY);

    // Put your mouseOut stuff here
    isRecDown = false;
  },

  handleMouseMove: function(e) {
    if (!isRecDown) {
      return;
    }
    // tell the browser we're handling this event
    e.preventDefault();
    e.stopPropagation();

    mouseX = parseInt(e.clientX - recOffsetX);
    mouseY = parseInt(e.clientY - recOffsetY);
    newRect = {
      x: Math.min(startX, mouseX),
      right: Math.max(startX, mouseX),
      y: Math.min(startY, mouseY),
      bottom: Math.max(startY, mouseY),
      color: "#000000",
	  type:"rectangle",
      width:mouseX - startX,
      height:mouseY - startY
    }
    drawRectangleOnCanvas.drawAll(scale, translatePos);
    ctx.strokeStyle = "#000000";
    ctx.lineWidth = strokeWidth;
    ctx.globalAlpha = 1;
    ctx.strokeRect(startX, startY, mouseX - startX, mouseY - startY);
  }
}
var isCircleDown = false;
var x1,y1;              /// start points
var newcircle;
var drawCircleOnCanvas = {
	handleMouseDown: function(e) {
		var rect = canvas.getBoundingClientRect();
			x1 = e.clientX - rect.left;
			y1 = e.clientY - rect.top;
			isCircleDown = true;
		},
	handleMouseUp: function(e) {
		
		isCircleDown = false;
		Images.push(newcircle);
		drawRectangleOnCanvas.drawAll(scale, translatePos);
	},
	handleMouseMove: function(e) {
		if (!isCircleDown) return;
    
		var rect = canvas.getBoundingClientRect(),
        x2 = e.clientX - rect.left,
        y2 = e.clientY - rect.top;
    
		newcircle = {
		  x: x1,
		  y: y1,
		  top: x2,
		  bottom: y2,
		  color: "#000000",
		  type:"circle",
          width : (x2 - x1) * 0.5,
          height :(y2 - y1) * 0.5,
          
		}
		drawRectangleOnCanvas.drawAll(scale, translatePos);

		/// draw ellipse
		drawEllipse(x1, y1, (x1+(newcircle.width*2)), (y1+(newcircle.height*2)),newcircle.color);
	},
	handleMouseOut: function(e) {
		// tell the browser we're handling this event
		e.preventDefault();
		e.stopPropagation();


		// Put your mouseOut stuff here
		isCircleDown = false;
	  }
}
function drawEllipse(x1, y1, x2, y2,color) {

    var radiusX = (x2 - x1) * 0.5,   /// radius for x based on input
        radiusY = (y2 - y1) * 0.5,   /// radius for y based on input
        centerX = x1 + radiusX,      /// calc center
        centerY = y1 + radiusY,
        step = 0.01,                 /// resolution of ellipse
        a = step,                    /// counter
        pi2 = Math.PI * 2 - step;    /// end angle
    
    /// start a new path
    ctx.beginPath();

    /// set start point at angle 0
    ctx.moveTo(centerX + radiusX * Math.cos(0),
               centerY + radiusY * Math.sin(0));

    /// create the ellipse    
    for(; a < pi2; a += step) {
        ctx.lineTo(centerX + radiusX * Math.cos(a),
                   centerY + radiusY * Math.sin(a));
    }
    
    /// close it and stroke it for demo
    ctx.closePath();
    ctx.strokeStyle = color;
    ctx.stroke();
}

function MessageReceived(Message)
{
   //alert('HTML Page Popup :'+Message);
   // alert('Updated value: '+Message["variable"]["value"]);
   if(Message["variable"]["name"] == "IDT")
   {
		//alert(Message["variable"]["value"]);
		if(Images.length > 0)
			Images[0].value = Message["variable"]["value"];
		
   }else if(Message["variable"]["name"] == "ODT")
   {
		//alert(Message["variable"]["value"]);
		if(Images.length > 1)
			Images[1].value = Message["variable"]["value"];
   }
  drawRectangleOnCanvas.drawAll(scale, translatePos);
}
 $("#object_type").change(function () {
		ReadVariablesFromDevice(parseInt(this.value));
    });
	
$("#selectVariableSave").click(function () {
        var isUpdate = false;
		if(selectedDragableObject>-1 && selectedDragableObject < Images.length)
        {
            SelectedImage = Images[selectedDragableObject];
            isUpdate = true;
        }
		SelectedImage["object_type"] = parseInt($("#object_type").find(":selected").val());
		SelectedImage["object_variable"] = parseInt($("#object_variables").find(":selected").val());
		SelectedImage["value"] = parseFloat($("#object_value").val());
		SelectedImage["auto_manual"] = parseInt($("#object_auto_manual").find(":selected").val());
		SelectedImage["unit"] = 0;
        SelectedImage["color"]= $("#object_display_color").val();
		SelectedImage["object_name"] = $("#object_variables").find(":selected").text();
       // SelectedImage["fontType"] = parseInt($("#object_font_type").find(":selected").val());
        SelectedImage["fontSize"] = $("#object_display_Size").find(":selected").val();
        SelectedImage["iconSize"] = parseInt($("#object_icon_size").find(":selected").val());
        SelectedImage["textPos"] = parseInt($("#object_display_pos").find(":selected").val());
        SelectedImage["object_display_type"] = parseInt($("#object_display_type").find(":selected").val());
		SelectedImage["object_label"] = "";
        if(SelectedImage["object_type"] == 5)
            SelectedImage["status"] = parseInt($("#object_status").find(":selected").val());

       if(isUpdate)
       {
        Images[selectedDragableObject] = SelectedImage;
        var request = {
            'action':4,
            'object_type' :Images[selectedDragableObject].object_type,
            'index' :Images[selectedDragableObject].object_variable,
            'value':Images[selectedDragableObject].value,
            'auto_manual':Images[selectedDragableObject].auto_manual,
            'status':Images[selectedDragableObject].status,
        };
        window.chrome.webview.postMessage(request);	
         
       }else{
		Images.push(SelectedImage);
        var request = {
            'action':9,
            'Value' :$("#object_name").val(),
            'xpos' :SelectedImage.x,
            'ypos' :SelectedImage.y
        };
       // alert(request)
        window.chrome.webview.postMessage(request);	
        console.log(request)
       }
       
		SelectedImage ="";
		ReadElementsDataFromDevice();
		$("#object_type").val(0);
		$("#object_variables").val(0);
		$('#selectVariableModel').modal('hide');
		drawRectangleOnCanvas.drawAll(scale, translatePos);
        
    });
	
	
$("#selectVariableClose").click(function () {
		$("#object_type").val(0);
		$("#object_variables").val(0);
		$('#selectVariableModel').modal('hide');
    });
		
function ReadVariablesFromDevice(type)
{
		var request = {
					'action':0,
					'sub_action':type,
                };
		 window.chrome.webview.postMessage(request);	 
}
function PopulateVariableList(response)
{
	//alert(response);
	var $dropdown = $("#object_variables");
	//if(response["action"] >= 0 && response["action"] <= 5 )
	{
		$dropdown.empty(); 
		$.each(response["Data"], function() {
            var value = this.desc
            if(this.desc == "")
                 value = this.id
                 
			$dropdown.append($("<option />").val(this.index).text(value));
		});
		
	}
}
function ReadElementsDataFromDevice()
{
		for (var i = 0; i < Images.length; i++) {
			if(Images[i].type ==="image")
			{
				var request = {
					'action':3,
					'object_type' :Images[i].object_type,
					'object_variable' :Images[i].object_variable,
					'element_id' :i
                };
				window.chrome.webview.postMessage(request);	 
			 }
		}
	drawRectangleOnCanvas.drawAll(scale, translatePos);
}
function UpdateElementsDataFromDevice(response)
{
	//console.log("value: "+response["value"]);
	//console.log("object_name: "+response["object_name"]);
	if(response["element_id"] < Images.length)
	{
		Images[response["element_id"]].value = response["value"];
		Images[response["element_id"]].object_name = response["object_name"];
		Images[response["element_id"]].unit = response["unit"];
		Images[response["element_id"]].object_label = response["object_label"];
		Images[response["element_id"]].auto_manual = response["auto_manual"];
        if(Images[response["element_id"]].object_type == 5)
		    Images[response["status"]].object_label = response["status"];
	}else{
		console.log("invalid Data from T3000: "+response);
	}
	drawRectangleOnCanvas.drawAll(scale, translatePos);
}
ReadVariablesFromDevice(parseInt($('#object_type').val()));
setInterval(ReadElementsDataFromDevice, 3000)

$("#NewBtn").click(function(){
    loadedGraphicName = "";
    Images= [];
    
   drawRectangleOnCanvas.drawAll(scale, translatePos);
});

$("#NewGraphicClose").click(function(){   
    
    $('#NewGraphicNameFileModel').modal('hide');

});
$("#NewGraphicSave").click(function(){
   
    var filename = $("#NewGraphicName").val();
    if(filename!="")
    {
        loadedGraphicName = filename.toUpperCase();
        var request = {
            'action':5,
            'graphic_name' :loadedGraphicName,
            'Data' :JSON.stringify(Images)
        };
        window.chrome.webview.postMessage(request);	 
        $('#NewGraphicNameFileModel').modal('hide');
    }else{
        alert("Name should not be empty")
    }
});
$("#saveBtn").click(function(){
   
    if(loadedGraphicName=="")
    {
        $('#NewGraphicNameFileModel').modal('show');
    }
    else{

        var request = {
            'action':5,
            'graphic_name' :loadedGraphicName,
            'Data' :JSON.stringify(Images)
        };
        window.chrome.webview.postMessage(request);	 
    }
    
    //downloadObjectAsJson(Images,"graphic1");
});
$("#loadBtn").click(function(){
   
    var request = {
        'action':6
    };
    window.chrome.webview.postMessage(request);	 
    $("#selectGraphicSelect").hide();
    $("#selectGraphicLoad").show();
    //downloadObjectAsJson(Images,"graphic1");
});
function downloadObjectAsJson(exportObj, exportName){
    var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(exportObj));
    var downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href",     dataStr);
    downloadAnchorNode.setAttribute("download", exportName + ".json");
    document.body.appendChild(downloadAnchorNode); // required for firefox
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
}
function GraphicFilesList(filesList)
{
    $("#graphicFileTable tbody").html("");

    $.each(filesList["files"], function() {
        var newRowContent = "<tr><td class='graphicFile' >"+ this +"</td></tr>";
        $("#graphicFileTable tbody").append(newRowContent);
    });
    
    $('#selectGraphicFileModel').modal('show');
}

$("#selectGraphicLoad").click(function(){
    loadedGraphicName = selectedGraphicName;
    var request = {
        'action':7,
        'graphic_name' : loadedGraphicName
    };
    window.chrome.webview.postMessage(request);	 
    selectedGraphicName = "";
});
$("#selectGraphicSelect").click(function(){
   
    SelectedImage['object_name'] =selectedGraphicName;
    Images.push(SelectedImage);
    SelectedImage ="";
	drawRectangleOnCanvas.drawAll(scale, translatePos);
    $('#selectGraphicFileModel').modal('hide');
    selectedGraphicName ="";

});
$("#selectGraphicClose").click(function(){   
    
    $('#selectGraphicFileModel').modal('hide');

});
$("#graphicFileTable").on('click', 'tr', function() {
    $('.selected').removeClass('selected');
    $(this).addClass("selected");
    var file = $('.graphicFile',this).html();
    //alert(file );
    selectedGraphicName = file;
});
//requestAnimationFrame(animate);
function LoadGraphicFile(response)
{
    //alert(response);
    console.log(response);
   Images = response;
   drawRectangleOnCanvas.drawAll(scale, translatePos);
   $('#selectGraphicFileModel').modal('hide');
   
}
function showContextMenu(r,x,y){
    if(!r || selectedDragableObject < -1 ){$menu.hide(); return;}
    if(Images[selectedDragableObject].object_type>2)
    {
        $("#menuView").show();
    }else{
        $("#menuView").hide();
    }
    if(Images[selectedDragableObject].type=="BGIMAGE" )
    {
        $("#menuView").hide();
        $("#menuEdit").hide();
    }
    if(Images[selectedDragableObject].type=="graphic")
    {
        $("#menuEdit").hide();
    }
    $menu.show();
    var m=r.contextMenu;
    //$menu.empty();
    $menu.css({left:x,top:y});
    
  }

  $('#contextMenu').on('click','li',function(e){
    // hide the context menu
    showContextMenu();
    //alert('Context selection: '+$(this).attr('id'));
    if($(this).attr('id')=="menuView"){
        if(selectedDragableObject > -1 && selectedDragableObject < Images.length)
        { 
            if(Images[selectedDragableObject].object_type == 6)
            {
                var request = {
                    'action':7,
                    'graphic_name' : Images[selectedDragableObject].object_name
                };
                window.chrome.webview.postMessage(request);	 
            }else {
                var request = {
                    'action':8,
                    'object_type' :Images[selectedDragableObject].object_type,
                    'index' :Images[selectedDragableObject].object_variable
                };
                window.chrome.webview.postMessage(request);	 
            }
        
        }
    }
    else if($(this).attr('id')=="menuEdit"){
        if(selectedDragableObject > -1 && selectedDragableObject < Images.length)
        {    
            $("#object_type").val( Images[selectedDragableObject].object_type );
            $("#object_variables").val( Images[selectedDragableObject].object_variable );
            $("#object_display_color").val( Images[selectedDragableObject].color );
            //$("#object_variables").val( Images[selectedDragableObject].object_name );
            $("#object_display_Size").val( Images[selectedDragableObject].fontSize );
            $("#object_icon_size").val( Images[selectedDragableObject].iconSize );
            $("#object_display_pos").val( Images[selectedDragableObject].textPos );
            $("#object_display_type").val( Images[selectedDragableObject].object_display_type );
            $('#object_details').show();
            
            $("#object_auto_manual").val( Images[selectedDragableObject].auto_manual );
            if(Images[selectedDragableObject].object_type==5)
            {
                $('.obj_value').hide();  
                $('.obj_status').show();  
                alert(Images[selectedDragableObject].status)
                $("#object_status").val( Images[selectedDragableObject].status );              
            }
            else if(Images[selectedDragableObject].object_type > 2 && Images[selectedDragableObject].object_type < 5 ){
                $('.obj_status').hide();  
                $('.obj_value').hide(); 
            }else
            {
                $('.obj_status').hide();  
                $('.obj_value').show(); 
                $("#object_value").val( Images[selectedDragableObject].value );
            }
            
            $('#selectVariableModel').modal('show');
        }
    }else if($(this).attr('id')=="menuDelete"){
        if (selectedDragableObject > -1) {
            DoElements.push(Images[selectedDragableObject]);
            Images.splice(selectedDragableObject, 1);
            selectedDragableObject = -1;
        }
        drawRectangleOnCanvas.drawAll(scale, translatePos);
    }else if($(this).attr('id')=="menuRotate"){

    }
  });
  var LockEnabled = false;
  $("#lockAction").click(function(){
   
    if(LockEnabled){
        $("#lockIcon").attr("src", "images/unlock.ico")
        LockEnabled = false;
        //$("#accordionExample").attr('disabled','disabled');
        $("#accordionExample .accordion-button").attr("aria-expanded","true");
        $("#accordionExample .accordion-button").removeAttr("disabled");   
        $("#accordionExample .accordion-button").click();  
        
        $("#accordionExample").show();
    }
    else{
        $("#lockIcon").attr("src", "images/lock.ico")
        LockEnabled = true;
        selectedDragableObject = -1;

        drawRectangleOnCanvas.drawAll(scale, translatePos);
        isMouseDown = false;
        canvas.removeEventListener("mousemove", defaultCanvasEvents.handleMouseMove, false);
        canvas.removeEventListener("mouseup", defaultCanvasEvents.handleMouseUp, false);
        SetdefaultCanvasEvents(true);
        if($("#accordionExample #tab1").attr("aria-expanded") == "true")
                $("#accordionExample #tab1").click();
        
        if($("#accordionExample #tab2").attr("aria-expanded") == "true")
                $("#accordionExample #tab2").click();

        if($("#accordionExample #tab3").attr("aria-expanded") == "true")
                $("#accordionExample #tab3 ").click();
        
        
        $("#accordionExample").hide();
        
    }
});
  showContextMenu();

  //=================================== RESIZE of CANVAS =======================================
  $(function(){
	resizeCanvas();
});

$(window).on('resize', function(){
    resizeCanvas();
});

function resizeCanvas()
{
    var canvas = $('#canvas');
    canvas.attr("width" , window.innerWidth);
    canvas.attr("height", window.innerHeight);
    //canvas.css("width", $(window).width());
    //canvas.css("height", $(window).height());
    $("#canvasDiv").css("height", $(window).height());
     canvasOffset=canvas.offset();
     offsetX=canvasOffset.left;
     offsetY=canvasOffset.top;     
     canvas.width
     translatePos = {
        x: window.innerWidth / 2,
        y: window.innerHeight / 2
    };
    console.log("resize translate",translatePos)
    SetdefaultCanvasEvents(true);
	drawRectangleOnCanvas.drawAll(scale, translatePos);
}
$("#zoomin").click(function(){
    if(scale < 1.5){
        scale =scale + scaleMultiplier;
    drawRectangleOnCanvas.drawAll(scale, translatePos);
    }
});
$("#zoomout").click(function(){
    if(scale > 1){
        scale =scale - scaleMultiplier;
        drawRectangleOnCanvas.drawAll(scale, translatePos);
    }
});
  //=================================== END RESIZE of CANVAS =======================================
  $("#backgroundImgBtn").click(function(){
    var request = {
        'action':10
    };
    window.chrome.webview.postMessage(request);	 
});

function UpdateBackgroundImage(response)
{
    BackGroundImage = response.file;
    BackGroundImageWidth = response.width;
    BackGroundImageHeight = response.height;
    var newBGImage = {
        type:"BGIMAGE",
        x: 0,
        y: 0,
        src: response.file,
        width : response.width,
        height :response.height,
        
      }
    Images.unshift(newBGImage)
    drawRectangleOnCanvas.drawAll(scale, translatePos);
}
$('.dropdown-menu li').on('click', function() {
    //alert('clicked');
    var val = $(this).find('a').html();
    var img = $(this).find('img').attr('src');
    if(img=="")
        return;
    //alert(img);
    var selVal = $(this).attr('value');
    //alert(selVal);
    if(selectedDragableObject>-1 && selectedDragableObject < Images.length)
    {
     Images[selectedDragableObject].image=img;
    }else{
        SelectedImage.image=img;
    }
    $('#dropdownMenuButton').html("<img src="+img+" width='20'height='15' > " + val+" &nbsp");
});