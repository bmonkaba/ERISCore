

const regex = new RegExp(/\r?\n|\r/, 'g');

$(document).ready(function(){

var note_name = ["C0", "C#0/Db0", "D0", "D#0/Eb0", "E0", "F0", "F#0/Gb0", "G0", "G#0/Ab0", "A0", "A#0/Bb0", "B0", "C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1", "C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2", "C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3", "C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4", "C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5", "C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6", "C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7", "C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8"]; 
var display_note = 0;
var received = $('#received');
var scroll = $('#scroll_me');
var kbs = $('#kbs');
var socket = new WebSocket("ws://ryzen:8080/ws");
var osi;
var si = 0;
var rx_bytes = 0;
var frame_count = 0;

var t = new Date();
var start_time = Date.now();
var com_state = "";
var sdFiles = [];
var fileStreamContainer = [];





$( "#freq_slider" ).slider({
  change: function( event, ui ) {}
});


setInterval(sweepFreq, 100);


$('#file_explorer').jstree();
$('#fancy_explorer').fancytree({
    autoScroll: true,
    lazyLoad:function(event, data){
        console.log(data); 
        data.result = [];
        parent = data.node.parent;
        path = "";
        while(parent.title != "root"){
            path = "/" + parent.title + path;
            parent = parent.parent;
        }
        sendMessage({'data' : "LS "+ path + "/" + data.node.title.replaceAll(' ',':')}); //replace any spaces in the path
        data.node.setActive();
    },
    
    dblclick:function(event, data){
        console.log(event);
        console.log(data);
        if(data.targetType=="title"){
            parent = data.node.parent;
            path = "";
            while(parent.title != "root"){
                path = "/" + parent.title + path;
                parent = parent.parent;
            }
            sendMessage({"data":"GET " + path.replaceAll(' ','*') + ' ' + data.node.title.replaceAll(' ','*') });
            console.log(path + '/' + data.node.title);
        }
    }
});
    
var tree = $.ui.fancytree.getTree("#fancy_explorer");

socket.onopen = function(){  
  console.log("connected"); 
  sendMessage({data:"LS "});
}; 

socket.onmessage = function (message) {
  var canvas = $('#stage')[0];
  var ctx = canvas.getContext('2d');
  var res = message.data.trim().split(" ");
  var now = Date.now();
  
  
  rx_bytes += message.data.length;
  
  if (com_state != "") {
    switch (com_state){
          case "LS":
              if (res[0] == "DIR_EOF"){
                   var tree = $.ui.fancytree.getTree("#fancy_explorer");
                   if (tree.getActiveNode() != null){
                       //tree.getActiveNode().data.result += sdFiles;
                       tree.getActiveNode().addChildren(sdFiles);
                       sdFiles = [];
                    }else{
                        tree.reload(sdFiles);
                   }
                   com_state = "";
              } else{
                  if (message.data.search('/') > 0) sdFiles.push({"title":message.data.replace('/','').replace(regex,''),"folder": true, lazy: true});
                  else sdFiles.push({"title":message.data.replaceAll(regex,'')});
              }
              break;      
    }
  
   } else switch(res[0]){
      case "DIR":
          com_state = "LS";
          sdFiles = [];
          break;
      case "DIR_EOF":
          // handled in the ls capture state
          break;
      case "CQT_EOF":
            osi = 0;
            frame_count = frame_count + 1;
            var imageData = ctx.getImageData(1, 1, canvas.width,canvas.height-1);
            var data = imageData.data;
            
            for (var j = 1; j < canvas.height; j += 5) {        
                for (var i = 0; i < canvas.width; i+= 1) {
                    offset = (j * canvas.width * 4) + (i * 4.0);
                    data[offset]     = data[offset] * 0.96;     // red
                    data[offset + 1] = data[offset + 1] * 0.96; // green
                    data[offset + 2] = data[offset + 2] * 0.96; // blue
                    data[offset + 3] = data[offset + 3] * 1; // alpha
                }
             }
 
            ctx.putImageData(imageData, 4, 0);
            ctx.fillStyle = "#1b0b0b";
            ctx.fillRect (0, canvas.height-1, canvas.width, canvas.height);
 
            if (display_note == 20){display_note = 0;}
 
            if(0==display_note){
                kbs.empty();
                kbs.append(((rx_bytes/((now - start_time)/1000.0))).toFixed(0));
                kbs.append('Bytes/Sec<br>DL&#11015;');
                kbs.append(fileStreamContainer.length);
                //reset the bandwidth monitor variables
                rx_bytes = 0;
                start_time = now;
                
                //received.append($('<br/>'));
                
                ctx.strokeStyle = "#9A0000";
                ctx.moveTo(10, canvas.height/2.0);
                ctx.lineTo(10, canvas.height);
                ctx.stroke();
                ctx.strokeStyle = "#5A0000";
                ctx.moveTo(10, canvas.height);
                ctx.lineTo(canvas.width, canvas.height);
                ctx.stroke();
            } else{
                ctx.strokeStyle = "#9A7030";
                ctx.moveTo(8, canvas.height/2.0);
                //ctx.lineTo(8, canvas.height-30);
                //ctx.stroke();
                //ctx.strokeStyle = "#FF2050";
            }
            display_note += 1;
            break;
      case "CQT_L":
            osi += 1;
            //console.log(message.data);
            //received.append(message.data.replaceAll(",","\t"));
           // received.append($('<br/>'));
            cqt = res[1].split(",");
            
            g = 90;       
            if(0==display_note%2) g= 30;

            //cqt view
            ctx.fillStyle = "rgba(" + (parseInt(cqt[0]) * 5).toString() + ","+ g +"," + (parseInt(cqt[4]) * 1.25).toString() + ", 1)";    
            ctx.fillRect(parseInt(cqt[0]) * 8, canvas.height - (1+parseInt(cqt[4])), 4, canvas.height);
            
            //osc view
            //ctx.fillStyle = "rgba(" + (osi * 20) + ","+ cqt[4] +"," + (255 - osi * 20) + ", 1)";    
            ctx.fillStyle = "rgba(" + cqt[4] + ","+ (osi * 20) +"," + (255 - cqt[4]) + ", 1)";  
            ctx.fillRect(-3 + (3 * parseInt(cqt[0])), -1 + (canvas.height - (osi * (canvas.height/20.0))), 6, 3);
            
            
            if (frame_count%20) break; 
            ctx.fillStyle = "#e740FF";
            ctx.font = "9px Arial";
            ctx.strokeStyle = "#000000";
            ctx.strokeText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[4])) - 14);
            ctx.fillText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[4])) - 16);
            //ctx.putImageData(imageData, 0, 1);
            break;
      case "CQT_H":
            osi += 1;
            //console.log(message.data);
            //received.append(message.data.replaceAll(",","\t\t\t\t"));
            //received.append($('<br/>'));
            cqt = res[1].split(",");
            
            g = 70;       
            if(0==display_note%2) g= 10;
            
            //cqt view
            ctx.fillStyle = "rgba( "+ g +"," + (parseInt(cqt[0]) * 2).toString() + "," + (parseInt(cqt[4]) * 1.25).toString() + ", 1)";    
            ctx.fillRect(parseInt(cqt[0]) * 8, canvas.height - (1+parseFloat(cqt[4])), 4, canvas.height);

            //osc view
            ctx.fillStyle = "rgba(" + (osi * 20) + ","+ cqt[4] +"," + (255 - osi * 20) + ", 1)";   
            //ctx.fillRect(3, -1 + (canvas.height - ((parseInt(cqt[0])) * 3.0)), 3 + parseInt(cqt[4])/3.0, 2);
             ctx.fillRect(-3 + (3* parseInt(cqt[0])), -1 + (canvas.height - (osi * (canvas.height/20.0))), 6, 3);
            
            
            if (frame_count%20) break;     
            ctx.fillStyle = "#E740FF";
            ctx.font = "9px Arial";
            ctx.strokeStyle = "#000000";
            ctx.strokeText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[4])) - 14);
            ctx.fillText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[4])) - 16);
            break;
            
      case "S":
          //received.append(message.data);
          //received.append($('<br/>'));
          s = res[1].split(",");
          canvas = document.getElementById('oscope');
          //canvas = $('#oscope')[0];
          ctx = canvas.getContext('2d');
          
          ctx.fillStyle = "rgba(255,"+  (Math.floor(128 * 255)).toString() +",80,1)";//Math.random()
          ctx.strokeStyle = "rgba(80,"+ (Math.floor(0 * 255)).toString() +",200,0.25)";
          ctx.beginPath();
          ctx.moveTo(si,canvas.height/2);
          ctx.lineTo(si,canvas.height/2);
          for (var i = 1; i <= parseInt(s[0]);i++){
              ctx.lineTo(si, canvas.height/2 - (parseInt(s[i])-128)/2 + 2);
              ctx.stroke();
              ctx.fillRect(si, canvas.height/2 - (parseInt(s[i])-128)/2, 3, 2);
              si += 1;
              if (si > canvas.width){
                  ctx.lineTo(si,canvas.height/2);
                  //ctx.closePath();
                  ctx.stroke();
                  ctx.moveTo(0,canvas.height/2);
                  ctx.beginPath();
                  si = 0; 
                  ctx.fillStyle = "rgba(0,0,0,1)";
                  //ctx.fillRect (0, 0, canvas.width, canvas.height);
                  //ctx.clearRect(0, 0, canvas.width, canvas.height);
                    
                  var imageData = ctx.getImageData(1, 1, canvas.width,canvas.height);
                    var data = imageData.data;
                
                    for (var j = 1; j < canvas.height; j += 1) {        
                        for (var i = 0; i < canvas.width; i+= 1) {
                            offset = (j * canvas.width * 4) + (i * 4.0);
                            data[offset]     = data[offset] * 0.3908;     // red
                            data[offset + 1] = data[offset + 1] * 0.599; // green
                            data[offset + 2] = data[offset + 2] * 0.39065; // blue
                            data[offset + 3] = data[offset + 3] * 1; // alpha
                        }
                     }
     
                    ctx.putImageData(imageData, 1, 1);
                    ctx.fillRect(0,0,1, canvas.height);
                    ctx.fillRect(0,canvas.height,canvas.width, 1);
              }
          }
          //ctx.closePath();
          ctx.lineWidth = 1;
          ctx.stroke();
          //draw capture divider
          ctx.fillStyle = "rgba(128,60,128,1)";
          ctx.fillRect(si, 0,1,canvas.height);
          break


      case "FS_START":
          fileStreamContainer = [];
          break;
          
      case "FS":
          s = res[1].split(",");
          fileStreamContainer = fileStreamContainer.concat(s);
          break;
          
      case "FS_END":
        received.append(message.data.trim());
        received.append($('<br/>'));   
          //do something with the received data
        //shift out the file size
        for (i=0; i <16;i++) fileStreamContainer.shift();
        canvas = $('#ileview')[0];
        ctx = canvas.getContext('2d');
        ctx.fillStyle = "rgba(5,0,40,1)";
        ctx.fillRect (0, 0, canvas.width, canvas.height);
        var imageData = ctx.getImageData(0, 0, canvas.width,canvas.height);
        var data = imageData.data;
        var color;
        for(i=0; i< fileStreamContainer.length-1;i=i+2)
        {
            color = parseInt(Number("0x" + fileStreamContainer[i] + fileStreamContainer[i+1]), 10);
            data[(i*2)] = (color >> 8) & 0x00F8;
            data[(i*2)+1] = (color >> 3) & 0x00FC;
            data[(i*2)+2] = (color << 3) & 0x00F8;
            data[(i*2)+3] = 0x00FF;
        } 
        ctx.putImageData(imageData, 0, 0);
          break;

      default:
        received.append(message.data.trim());
        received.append($('<br/>'));
        scroll.animate({scrollTop: received.scrollHeight}, "slow");            
  };
  //$('#messages').animate({$('#messages').scrollTop: 0}, "fast");
  $('.messages').scrollTop(10000000000);
};



socket.onclose = function(){
  console.log("disconnected"); 
};

var sendMessage = function(message) {
  //console.log("sending:" + message.data);
  socket.send(message.data + String.fromCharCode(10));
};





// send a command to the serial port
$("#cmd_send").click(function(ev){
  ev.preventDefault();
  var cmd = $('#cmd_value').val();
  sendMessage({ 'data' : cmd});
  $('#cmd_value').val("");
});

$('#clear').click(function(){
  received.empty();
});


$("#freq_slider").on("change", function(event, ui) {
  sendMessage({ 'data' : 'AA ' + $('#freq_slider')[0].value});
});
    

draw();

});

function draw(){
  var canvas = document.getElementById('stage');
  if (canvas.getContext){
    var ctx = canvas.getContext('2d');

    ctx.fillStyle = "#D74022";
    ctx.fillStyle = "#000000";
    ctx.fillRect (0, 0, canvas.width, canvas.height);
    ctx.fillStyle = "rgba(0,0,0,1)";
    canvas.style.webkitFilter = "blur(0px)";

  }
}

function sweepFreq(){
    if ($(sweep_checkbox).is(":checked")){
        $('#freq_slider')[0].stepUp(3);
        $('#freq_slider').trigger("change");
    }
}
