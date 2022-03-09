const regex = new RegExp(/\r?\n|\r/, "g");
var data_dict = {};
var file_dict = {};
var osc_array = [];
var watch = {};
var symbols = [];
var ram = {
    "RAM1": {},
    "RAM2": {}
};
var bulk_update_buffer = [];
var pixel;
var pixel_color;
var dataflow_render_by = "flow";
var file_type;

// setup the db that will be used to map ram to the symbol table
//https://dexie.org/docs/API-Reference#quick-reference
var db = new Dexie("symbols_database");
db.version(1).stores({
    symbols: "++id,&addr,bind,name,type,section,file",
    data_chunk: "++id,&addr"
});

var lgraph;
var lgcanvas;

//node constructor class
function ErisAudioNode() {
    //this.addInput("1","number");
    //this.addInput("2","number");
    //this.addOutput("0","number");
    this.properties = {
        precision: 1
    };
}

//name to show
ErisAudioNode.title = "ErisNode";

//function to call when the node is executed
ErisAudioNode.prototype.onExecute = function () {
    //default node execution.. do something
    a = 0;
    b = 1;
    a = b;

}

$(document).ready(function () {

    var note_name = ["C0", "C#0/Db0", "D0", "D#0/Eb0", "E0", "F0", "F#0/Gb0", "G0", "G#0/Ab0", "A0", "A#0/Bb0", "B0", "C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1", "C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2", "C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3", "C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4", "C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5", "C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6", "C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7", "C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8"];
    var display_note = 0;
    var received = $("#received");
    var sidebar = $("#sidebar");
    var scroll = $("#scroll_me");
    var kbs = $("#kbs");
    var socket = new WebSocket("ws://ryzen:8088/ws");
    var osi;
    var si = 0;
    var rx_bytes = 0;
    var error_bytes = 0;
    var frame_count = 0;
    var t = new Date();
    var start_time = Date.now();
    var com_state = "";
    var sdFiles = [];
    var fileStreamContainer = [];
    var sparks = {};


    //ace editor
    var editor = ace.edit("code_editor");
    editor.setTheme("ace/theme/twilight");
    editor.session.setMode("ace/mode/javascript");
    editor.setReadOnly(false);
    editor.setOptions({
        theme: "ace/theme/twilight",
        useWorker: false,
    });
    /*https://ace.c9.io/#nav=howto
    */

    editor.setValue("System.print(\"Hello World.\")");
    
    const pickr = Pickr.create({
        el: ".pickr",
        theme: "nano", // or "monolith", or "nano"
        lockOpacity: true,
        swatches: [
            "rgba(244, 67, 54, 1)",
            "rgba(233, 30, 99, 1)",
            "rgba(156, 39, 176, 1)",
            "rgba(103, 58, 183, 1)",
            "rgba(63, 81, 181, 1)",
            "rgba(33, 150, 243, 1)",
            "rgba(3, 169, 244, 1)",
            "rgba(0, 188, 212, 1)",
            "rgba(0, 150, 136, 1)",
            "rgba(76, 175, 80, 1)",
            "rgba(139, 195, 74, 1)",
            "rgba(205, 220, 57, 1)",
            "rgba(255, 235, 59, 1)",
            "rgba(255, 193, 7, 1)"
        ],

        components: {

            // Main components
            preview: true,
            opacity: false,
            hue: true,

            // Input / output Options
            interaction: {
                hex: true,
                rgba: true,
                hsla: false,
                hsva: false,
                cmyk: false,
                input: true,
                clear: false,
                save: true
            }
        }
    });


    pickr.on("change", (color, source, instance) => {
        c = color.toRGBA();
        r = (c[0].toFixed() << 8) & 0xF800;
        g = (c[1].toFixed() << 3) & 0x07E0;
        b = (c[2].toFixed() >> 3) & 0x001F;
        c = r + g + b;
        //console.log("Event: "change"", c);
        $("#write_dd").val(c);

    })


    symbols_data_table = $("#symbols_table").DataTable({
        columns: [{
                data: "name"
            },
            {
                data: "addr"
            },
            {
                data: "bind"
            },
            {
                data: "type"
            },
            {
                data: "size"
            }
        ]
    });


    $("#flow_container").hide();
    $("#graph_container").hide();
    $("#memory_container").hide();
    $("#monitor_container").hide();
    $("#symbols_container").hide();
    $("#editor").show();




    //draw the crt filter
    var crt = $("#crt")[0]
    var ctx = crt.getContext("2d");
    ctx.strokeStyle = "rgba(0, 64, 128, 0.3)";

    for (y = 0; y < crt.height; y += 3) {
        ctx.beginPath();
        ctx.moveTo(0, y);
        ctx.lineTo(crt.width, y);
        ctx.stroke();
    }



    pixel = document.getElementById("monitor_canvas").getContext("2d").createImageData(1, 1);
    pixel_color = pixel.data;

    //draw the monitor background
    var c = document.getElementById("monitor_canvas");
    if (c.getContext) {
        var ctx = c.getContext("2d");
        ctx.fillStyle = "#D74022";
        ctx.fillStyle = "#000000";
        ctx.fillRect(0, 0, c.width, c.height);
        ctx.fillStyle = "rgba(0,0,0,1)";
        c.style.webkitFilter = "blur(0px)";
    }


    lgraph = new LGraph();
    lgcanvas = new LGraphCanvas("#connections", lgraph);

    //register in the system
    LiteGraph.registerNodeType("basic/ErisAudio", ErisAudioNode);
    //lgraph.start();
    lgcanvas.setZoom(0.5);

    $("html").on("dragover", function (event) {
        event.preventDefault();
        event.stopPropagation();
        //$(this).addClass("watchbar");
    });

    $("html").on("dragleave", function (event) {
        event.preventDefault();
        event.stopPropagation();
        //$(this).removeClass("watchbar");
    });

    $("html").on("drop", function (event) {
        event.preventDefault();
        event.stopPropagation();
        let dt = event.originalEvent.dataTransfer;
        let files = dt.files;
        handleFileList(files);
        //alert("Dropped!");
    });

    $("#freq_slider").slider({
        change: function (event, ui) {}
    });

    setInterval(sweepFreq, 100);
    setInterval(function () {
        var now = Date.now();
        kbs.empty();
        kbs.append(((rx_bytes / ((now - start_time) / 1.0))).toFixed(0));
        kbs.append(" KBytes/Sec&emsp;");
        kbs.append(parseFloat((1.0 - (error_bytes / rx_bytes)) * 100.0).toFixed(2) + "%");
        kbs.append(" Quality&emsp;");
        kbs.append("DL&#11015;");
        kbs.append(fileStreamContainer.length);
        //reset the bandwidth monitor variables
        rx_bytes = 0;
        error_bytes = 0;
        start_time = now;

        //sendMessage({ "data" : "STATS"});
    }, 1000);

    setInterval(function () {
        document.querySelectorAll(".sparkline").forEach(function (svg) {
            //var w = $("#sidebar").width();
            try {
                //svg.width(w);
                sparkline.sparkline(svg, sparks[svg.id]);
                //svg.innerHTML += "<text x=\"15\" y=\"15\" class=\"svg_text\">"+svg.id+": "+sparks[svg.id][0]+"</text>";
            } catch (e) {

            }
        });
    }, 250);

    $("#file_explorer").jstree();
    $("#fancy_explorer").fancytree({
        autoScroll: true,
        lazyLoad: function (event, data) {
            console.log(data);
            data.result = [];
            parent = data.node.parent;
            path = "";
            while (parent.title != "root") {
                path = "/" + parent.title + path;
                parent = parent.parent;
            }
            sendMessage({
                "data": "LS " + path + "/" + data.node.title.replaceAll(" ", ":")
            }); //replace any spaces in the path
            data.node.setActive();
        },

        dblclick: function (event, data) {
            console.log(event);
            console.log(data);
            if (data.targetType == "title") {
                parent = data.node.parent;
                path = "";
                while (parent.title != "root") {
                    path = "/" + parent.title + path;
                    parent = parent.parent;
                }
                sendMessage({
                    "data": "GET " + path.replaceAll(" ", "*") + " " + data.node.title.replaceAll(" ", "*")
                });
                console.log(path + "/" + data.node.title);
            }
        }
    });

    var tree = $.ui.fancytree.getTree("#fancy_explorer");

    socket.onopen = function () {
        console.log("connected");
        sendMessage({
            "data": "HELO "
        });
        sendMessage({
            data: "LS "
        });
    };

    socket.onmessage = function (message_block) {
        if (message_block.data.length == 0){ return;}
        var message_list = message_block.data.replaceAll("\r", "").split("\n");
        /* remove any empty messages */
        message_list = message_list.filter(e => e);
        for (message of message_list) {
            //console.log(message)
            var res = message.split(" ");
            var now = Date.now();
            rx_bytes += message.length;
            if (com_state != "") {
                switch (com_state) {
                case "captureWrenScript":
                    if (res[0] == "#WREN_EOF!") {
                        //write the container to the editor
                        editor.setValue(fileStreamContainer.join("\n"));
                        editor.resize();
                        editor.navigateFileStart();
                        com_state="";
                    }else{
                        fileStreamContainer = fileStreamContainer.concat(message);
                    }
                    break;
                case "LS":
                    if (res[0] == "DIR_EOF") {
                        var tree = $.ui.fancytree.getTree("#fancy_explorer");
                        if (tree.getActiveNode() != null) {
                            //tree.getActiveNode().data.result += sdFiles;
                            tree.getActiveNode().addChildren(sdFiles);
                            sdFiles = [];
                        } else {
                            tree.reload(sdFiles);
                        }
                        com_state = "";
                    } else {
                        res[2] = res.slice(2).join(" ");
                        listing = JSON.parse(res[2]);
                        for (item of listing) {
                            if (item.length > 0) {
                                if (item.search("/") > 0) sdFiles.push({
                                    "title": item.replace("/", "").replace(regex, ""),
                                    "folder": true,
                                    lazy: true
                                });
                                else sdFiles.push({
                                    "title": item.replaceAll(regex, "")
                                });
                            }
                        }
                    }
                    $("#fancy_explorer").children(".fancytree-container").css({
                        "background-color": "transparent"
                    });
                    $("#fancy_explorer").children(".fancytree-container").css({
                        "font-size": "1.0em"
                    });

                    break;
                }

            } else switch (res[0]) {
            case "M":
                received.append(res.slice(1).join(" "));
                received.append($("<br/>"));
                //scroll.animate({scrollTop: received.scrollHeight}, "slow"); 
                received.scrollTop(10000000000);
                break;
            case "DIR":
                com_state = "LS";
                sdFiles = [];
                break;
            case "DIR_EOF":
                // handled in the ls capture state
                break;
            case "CQT_EOF":
                var canvas = $("#stage")[0];
                var ctx = canvas.getContext("2d");
                osi = 0;
                add_str = ""
                for (osc in osc_array) {
                    add_str += (osc + ": " + osc_array[osc] + "<br>");
                }
                $("#cqtosc").empty().append(add_str);
                osc_array = [];
                frame_count = frame_count + 1;
                var imageData = ctx.getImageData(1, 1, canvas.width, canvas.height - 1);
                var data = imageData.data;

                for (var j = 1; j < canvas.height; j += 5) {
                    for (var i = 0; i < canvas.width; i += 1) {
                        offset = (j * canvas.width * 4) + (i * 4.0);
                        data[offset] = data[offset] * 0.96; // red
                        data[offset + 1] = data[offset + 1] * 0.96; // green
                        data[offset + 2] = data[offset + 2] * 0.96; // blue
                        data[offset + 3] = data[offset + 3] * 1; // alpha
                    }
                }

                ctx.putImageData(imageData, 4, 0);
                ctx.fillStyle = "#1b0b0b";
                ctx.fillRect(0, canvas.height - 1, canvas.width, canvas.height);

                if (display_note == 20) {
                    display_note = 0;
                }

                if (0 == display_note) {
                    ctx.strokeStyle = "#9A0000";
                    ctx.moveTo(10, canvas.height / 2.0);
                    ctx.lineTo(10, canvas.height);
                    ctx.stroke();
                    ctx.strokeStyle = "#5A0000";
                    ctx.moveTo(10, canvas.height);
                    ctx.lineTo(canvas.width, canvas.height);
                    ctx.stroke();
                } else {
                    ctx.strokeStyle = "#9A7030";
                    ctx.moveTo(8, canvas.height / 2.0);
                    //ctx.lineTo(8, canvas.height-30);
                    //ctx.stroke();
                    //ctx.strokeStyle = "#FF2050";
                }
                display_note += 1;
                break;
            case "CQT_L":
                var canvas = $("#stage")[0];
                var ctx = canvas.getContext("2d");
                osi += 1;
                //console.log(message);
                //received.append(message.replaceAll(",","\t"));
                // received.append($("<br/>"));
                cqt = res[1].split(",");
                osc_array.push(res[1]);
                g = 90;
                if (0 == display_note % 2) g = 30;

                //cqt view
                ctx.fillStyle = "rgba(" + (parseInt(cqt[0]) * 5).toString() + "," + g + "," + (parseFloat(cqt[5]) * 200).toString() + ", 1)";
                ctx.fillRect(parseInt(cqt[0]) * 8, canvas.height - (canvas.height * parseFloat(cqt[5])), 4, canvas.height);

                //osc view
                //ctx.fillStyle = "rgba(" + (osi * 20) + ","+ cqt[4] +"," + (255 - osi * 20) + ", 1)";    
                ctx.fillStyle = "rgba(" + 10 + cqt[5] * 2 + "," + cqt[4] * 10 + "," + cqt[5] * 200 + ", 1)";
                ctx.fillRect(-3 + (parseInt(cqt[0])), -1 + (canvas.height - (osi * (canvas.height / 20.0))), 6, 3);


                if (frame_count % 20) break;
                ctx.fillStyle = "#e740FF";
                ctx.font = "8px Arial";
                ctx.strokeStyle = "#000000";
                ctx.strokeText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseFloat(cqt[5])) - 14);
                ctx.fillText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseFloat(cqt[5])) - 16);
                //ctx.putImageData(imageData, 0, 1);
                break;
            case "CQT_H":
                var canvas = $("#stage")[0];
                var ctx = canvas.getContext("2d");
                osi += 1;
                //console.log(message);
                //received.append(message.replaceAll(",","\t\t\t\t"));
                //received.append($("<br/>"));
                cqt = res[1].split(",");
                osc_array.push(res[1]);

                g = 70;
                if (0 == display_note % 2) g = 10;

                //cqt view
                ctx.fillStyle = "rgba( " + g + "," + (parseInt(cqt[0]) * 2).toString() + "," + (parseFloat(cqt[6]) * 20).toString() + ", 1)";
                ctx.fillRect(parseInt(cqt[0]) * 8, canvas.height - (canvas.height * parseFloat(cqt[5])), 4, canvas.height);

                //osc view
                ctx.fillStyle = "rgba( " + (parseFloat(cqt[5]) * 4000).toString() + "," + (parseInt(cqt[0]) * 2).toString() + "," + (parseFloat(cqt[4]) * 8000).toString() + ", 1)";
                //ctx.fillRect(3, -1 + (canvas.height - ((parseInt(cqt[0])) * 3.0)), 3 + parseInt(cqt[4])/3.0, 2);
                ctx.fillRect(-3 + (parseInt(cqt[0])), -1 + (canvas.height - (osi * (canvas.height / 20.0))), 6, 3);


                if (frame_count % 20) break;
                ctx.fillStyle = "#E740FF";
                ctx.font = "14px Arial";
                ctx.strokeStyle = "#000000";
                ctx.strokeText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[5])) - 14);
                ctx.fillText(note_name[parseInt(cqt[0])], -4 + (parseInt(cqt[0]) * 8), canvas.height - (parseInt(cqt[5])) - 16);
                break;

            case "S":
                s = res[1].split(",");
                canvas = document.getElementById("oscope_hidden");
                ctx = canvas.getContext("2d");
                ctx.fillStyle = "#207020"; //Math.random()
                ctx.strokeStyle = "#C0F0F0";
                ctx.beginPath();
                ctx.moveTo(si, canvas.height);
                ctx.lineTo(si, canvas.height);
                if (s[0] == "FIN") {
                    //nothing else to draw
                    //$("oscope").show();
                    destinationCanvas = document.getElementById("oscope");
                    destinationCtx = destinationCanvas.getContext("2d");
                    imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
                    destinationCtx.putImageData(imageData, 0, 0);
                } else {
                    for (var i = 1; i <= parseInt(s[0]); i++) {
                        ctx.lineTo(si, canvas.height - (parseInt(s[i]) - 2) / 10);
                        ctx.stroke();
                        ctx.fillRect(si, canvas.height - (parseInt(s[i])) / 10, 1, parseInt(s[i]));
                        si += 1;
                    }
                }

                if (si == canvas.width || s[0] == "FIN") {
                    ctx.lineTo(si, canvas.height);
                    //ctx.closePath();
                    ctx.stroke();
                    ctx.moveTo(0, canvas.height);
                    ctx.beginPath();
                    si = 0;

                    //ctx.fillRect (0, 0, canvas.width, canvas.height);
                    //ctx.clearRect(0, 0, canvas.width, canvas.height);

                    var imageData = ctx.getImageData(-2, 2, canvas.width - 2, canvas.height + 2);
                    var data = imageData.data;


                    for (var j = 0; j < canvas.height; j += 1) {
                        for (var i = 0; i < canvas.width; i += 1) {
                            offset = (j * canvas.width * 4) + (i * 4.0);
                            data[offset] = data[offset] * 0.98; // red
                            data[offset + 1] = data[offset + 1] - 1; // green
                            data[offset + 2] = data[offset + 2] * 0.98; // blue
                            data[offset + 3] = data[offset + 3] - 1; // alpha
                        }
                    }

                    ctx.closePath();
                    ctx.putImageData(imageData, 0, 0);
                    ctx.fillRect(0, 0, 1, canvas.height);
                    ctx.fillRect(0, canvas.height, canvas.width, 1);
                }
                ctx.lineWidth = 1;
                ctx.stroke();
                //draw capture divider
                ctx.fillStyle = "rgba(80,80,100,1)";
                ctx.fillRect(si, 0, 5, canvas.height);
                break

            case "RAM":
                if (res[1] == "END") {

                    //update the db
                    db.data_chunk.clear().then(function (res) {
                        db.data_chunk.bulkAdd(bulk_update_buffer).then(function (lastkey) {
                            console.log(lastkey);
                            //clear the buffer after the db load;
                            //bulk_update_buffer = [];


                            //create the datatable
                            //$("#memory_chunk_table").empty();
                            $("#memory_chunk_table").DataTable({

                                data: bulk_update_buffer,
                                columns: [{
                                        data: "addr",
                                        "width": "1.5em",
                                        "font-size": "1em"
                                    },
                                    {
                                        data: "chunk",
                                        "font-size": "0.68em"
                                    },
                                    {
                                        data: "decode",
                                        "width": "20em",
                                        "font-size": "0.8em"
                                    }
                                ]

                            });


                        }).catch(Dexie.BulkError, function (e) {
                            // by explicitely catching the bulkAdd() operation 
                            // successfull additions are committed despite rejected records.
                            console.error("Warning Dexie.BulkError completed with errors:");
                            console.error(e);
                        })
                    });
                    break;
                } else {
                    try {
                        k = res.slice(1).join(" ");
                        d = JSON.parse(k);
                        if (d.RAM1) bulk_update_buffer.push(d.RAM1);
                        if (d.RAM2) bulk_update_buffer.push(d.RAM2);
                        $.extend(true, ram, d);
                    } catch (e) {
                        //bad message throw it out
                        break;
                    }
                }
                break;
            case "STATS":
                try {
                    data_dict = JSON.parse(res.slice(1).join(" "));
                } catch (e) {
                    //bad message throw it out
                    break;
                }
                //$.extend(watch, data_dict);
                _.merge(watch, data_dict);
                try {
                    $("#watch_received").jsonViewer(watch);
                } catch (e) {
                    //do nothing - bad data?
                }
                if (Math.random() > 0.05) {
                    //don"t need to redraw every time data is received" 
                    if (!$("#flow_container").is(":hidden")) renderAudioBlocks();
                }
                renderMonitor();
                break;
            case "DD":
                try {
                    data_dict = JSON.parse(res[1]);
                } catch (e) {
                    //bad message throw it out
                    break;
                }
                //sidebar.empty();
                var width = 1024;
                for (var key in data_dict) {
                    //sidebar.append(data_dict[key] + "<br>");
                    if ($("#" + key).length < 1) {
                        sidebar.append("<br><br>" + key + ": <b id=" + key + "_VAL>val</b><br>");
                        sidebar.append("<svg class=\"sparkline\" id=" + key + " width=\""+ width +"\" height=\"15\" stroke-width=\"1\"></svg>");
                        sparks[key] = new Array(1024);
                        sparks[key].fill(0.01);
                    }
                    d = parseFloat(data_dict[key]);
                    if (isNaN(d)) {
                        //console.log(key);
                        data_dict[key] = 0;
                        sparks[key].unshift(0);
                    } else {
                        sparks[key].unshift(d);
                    }
                    if (sparks[key].length > 1024) {
                        sparks[key].pop();
                    }
                    $("#" + key + "_VAL").text(sparks[key][0]);
                }
                break;
            case "#WREN_START!":
                fileStreamContainer = [];
                com_state = "captureWrenScript";
                break;
                  
            case "FS_START":
                fileStreamContainer = [];
                break;

            case "FS":
                s = res[1].split(",");
                /*
                packet_checksum = parseInt(s.slice(-1));
                checksum = 0;
                ar = [res[0], s.slice(0, -1).join()].join();
                for (var i = 0; i < ar.length; i++) {
                    checksum += ar[i].charCodeAt(0);
                }
                checksum = checksum - 12; //(remove form feed from calc)
                if (checksum != packet_checksum) {
                    console.log("checksum error");
                }
                */
                if (fileStreamContainer.length == 0) {
                    file_type = s.slice(0, 6).map(num => String.fromCharCode(parseInt(num, 16))).join("");
                    console.log(file_type);
                    if (file_type == "ILE565") {
                        fileStreamContainer = [];
                        end = 7 + s.slice(7, 32).findIndex(rank => rank === "0A"); //split on newline
                        split = 7 + s.slice(7, end).findIndex(rank => rank === "20"); //split on comma
                        width = s.slice(7, split); //split on comma
                        width = width.map(num => String.fromCharCode(parseInt(num, 16))).join("");
                        height = s.slice(split + 1, end); //split on comma
                        height = height.map(num => String.fromCharCode(parseInt(num, 16))).join("");
                        console.log(width);
                        console.log(height);
                        //resize the canvas
                        canvas = $("#ileview")[0];
                        ctx = canvas.getContext("2d");
                        ctx.canvas.width = parseInt(width, 10);
                        ctx.canvas.height = parseInt(height, 10);
                        fileStreamContainer = fileStreamContainer.concat(s.slice(end+2));
                        //console.log(s.slice(end+1,-1));
                    } else{
                       fileStreamContainer = s.map(num => String.fromCharCode(parseInt(num, 16))).join("");
                       //console.log(s.map(num => String.fromCharCode(parseInt(num, 16))).join(""));
                       editor.setValue("");
                    }

                } else {
                    if (file_type == "ILE565") {
                        fileStreamContainer = fileStreamContainer.concat(s);
                    }else{
                    
                        fileStreamContainer += s.map(num => String.fromCharCode(parseInt(num, 16))).join("");
                        //console.log(s.map(num => String.fromCharCode(parseInt(num, 16))).join(""));
                        //editor.insert(s.map(num => String.fromCharCode(parseInt(num, 16))).join(""));
                    }
                }
                break;

            case "FS_END":
                received.append(message.trim());
                received.append($("<br/>"));
                //do something with the received data
                //shift out the file size
                //for (i=0; i <16;i++) fileStreamContainer.shift();
                if (file_type == "ILE565") {
                    canvas = $("#ileview")[0];
                    ctx = canvas.getContext("2d");
                    ctx.fillStyle = "rgba(255,0,0,1)";
                    ctx.fillRect(0, 0, canvas.width, canvas.height);
                    var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
                    var data = imageData.data;
                    var color;
                    for (i = 0; i < fileStreamContainer.length - 1; i = i + 2) {
                        color = parseInt(Number("0x" + fileStreamContainer[i] + fileStreamContainer[i + 1]), 10);
                        data[(i * 2)] = (color >> 8) & 0x00F8;
                        data[(i * 2) + 1] = (color >> 3) & 0x00FC;
                        data[(i * 2) + 2] = (color << 3) & 0x00F8;
                        data[(i * 2) + 3] = 0x00FF;
                    }
                    ctx.putImageData(imageData, 0, 0);
                } else{
                     editor.setValue(fileStreamContainer);
                     editor.resize();
                }
                break;
            case "VM":
                message = message.replaceAll("VM","");

                var total = $("#vm_terminal")[0].value.split("\n");
                if (total.length > 1000) 
                   total = total.slice(total.length - 1000);              
                $("#vm_terminal")[0].value = total.join("\n");
                $("#vm_terminal")[0].value += message + "\n";
                //$("#vm_terminal").append("\n");
                $("#vm_terminal").scrollTop(10000000000);
                break;
            case "CLS":
                $("#received").empty();
                break;
            default:
                error_bytes += message.length;
                console.log(message);
                //received.append("RX ERROR (BYTES LOST): ");
                //received.append(message.length);
                //received.append($("<br/>"));
                //$(".messages").scrollTop(10000000000);
            }
        };
    };

    socket.onclose = function () {
        console.log("disconnected");
    };

    var sendMessage = function (message) {
        //console.log("sending:" + message.data);
        socket.send(message.data + String.fromCharCode(10));
    };


    // send a command to the serial port
    $("#cmd_send").click(function (ev) {
        ev.preventDefault();
        var cmd = $("#cmd_value").val();
        sendMessage({
            "data": cmd
        });
        $("#cmd_value").val("");
        //$("#received").empty();
    });

    $("#clear").click(function () {
        $("#cmd_value").empty();
    });

    $("#ACON").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "ACON"
        });
        $("#received").empty();
    });

    $("#STATS").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "STATS"
        });
        $("#received").empty();

    });

    $("#CQT_CFG").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "CQT_CFG"
        });
        $("#received").empty();
    });

    $("#PINK_NOISE").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "DISCONNECT amp:2 0\nCONNECT pink:1 0 amp:2 0\n"
        });
        $("#received").empty();
    });

    $("#TONE").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "DISCONNECT amp:2 0\nCONNECT tonesweep:1 0 amp:2 0\nAPC erisAudioSynthToneSweep 1 play 0.15,20,2100,10.0\n"
        });
        $("#received").empty();
    });

    $("#MUTE").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "DISCONNECT i2s-out:1 0"
        });
        $("#received").empty();
    });

    $("#DD").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "GET_DD"
        });
        $("#received").empty();
    });

    $("#write_dd_button").click(function (ev) {
        ev.preventDefault();
        var key = $("#listboxDataDict option:selected").val();
        var value = $("#write_dd").val();
        sendMessage({
            "data": "UPDATE_DD " + key + " " + value
        });
        $("#received").empty();
    });

    $("#df_orderby_type").click(function (ev) {
        ev.preventDefault();
        dataflow_render_by = "type";
    });

    $("#df_orderby_flow").click(function (ev) {
        ev.preventDefault();
        dataflow_render_by = "flow";
    });


    $("#cmd_showFlow").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").show(0.35);
        $("#graph_container").hide(0.15);
        $("#memory_container").hide(0.15);
        $("#monitor_container").hide(0.15);
        $("#editor_container").hide(0.15);
        $("#symbols_container").hide(0.15);
    });

    $("#cmd_showAudio").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").hide(0.15);
        $("#graph_container").show(0.35);
        $("#memory_container").hide(0.15);
        $("#monitor_container").hide(0.15);
        $("#editor_container").hide(0.15);
        $("#symbols_container").hide(0.15);
    });

    $("#cmd_showMemory").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").hide(0.15);
        $("#graph_container").hide(0.15);
        $("#memory_container").show(0.35);
        $("#monitor_container").hide(0.15);
        $("#editor_container").hide(0.15);
        $("#symbols_container").hide(0.15);
        $("#listboxDataDict").empty();
        for (spark in sparks) {
            $("#listboxDataDict").append($("<option>").text(spark).val(spark));
        };
    });

    $("#cmd_showMonitor").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").hide(0.15);
        $("#graph_container").hide(0.15);
        $("#memory_container").hide(0.15);
        $("#monitor_container").show(0.35);
        $("#editor_container").hide(0.15);
        $("#symbols_container").hide(0.15);
    });

    $("#cmd_showSymbols").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").hide(0.15);
        $("#graph_container").hide(0.15);
        $("#memory_container").hide(0.15);
        $("#monitor_container").hide(0.15);
        $("#editor_container").hide(0.15);
        $("#symbols_container").show(0.35);
    });
    
    $("#cmd_showEditor").click(function (ev) {
        ev.preventDefault();
        $("#flow_container").hide(0);
        $("#graph_container").hide(0);
        $("#memory_container").hide(0);
        $("#monitor_container").hide(0);
        $("#symbols_container").hide(0);
        $("#editor_container").show(0);
        editor.resize();
    });


    $("#symbols_table tbody").on("click", "tr", function () {
        var data = symbols_data_table.row(this).data();
        console.log(symbols_data_table.row(this).data());
        sendMessage({
            "data": "GET_RAM " + data.addr + " " + data.size
        });
    });

    $("#getRam1").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "GET_RAM1"
        });
        $("#received").empty();
        $("#memory_chunk_table tbody").empty();
    });
    
    $("#getRam2").click(function (ev) {
        ev.preventDefault();
        sendMessage({
            "data": "GET_RAM2"
        });
        $("#received").empty();
        $("#memory_chunk_table tbody").empty();
    });

    $("#get_script_button").click(function (ev) {
         ev.preventDefault();
         sendMessage({
             "data": "GET_WREN_SCRIPT"
         });
    });

    $("#compile_script_button").click(function (ev) {
        ev.preventDefault();
        //send the script
        var code = editor.getValue();
        
        sendMessage({
            "data": "WREN_SCRIPT_START"
        });
        
        sendMessage({
            "data": code
        });
        
        sendMessage({
            "data": "WREN_SCRIPT_END"
        });
        
        sendMessage({
            "data": "WREN_SCRIPT_COMPILE"
        });
        
    });
    
    $("#run_script_button").click(function (ev) {
        ev.preventDefault();
        //send the script
        var code = editor.getValue();
        
        sendMessage({
            "data": "WREN_SCRIPT_START"
        });
        
        sendMessage({
            "data": code
        });
        
        sendMessage({
            "data": "WREN_SCRIPT_END"
        });
        
        sendMessage({
            "data": "WREN_SCRIPT_EXECUTE"
        });
        
    });
    
    $("#save_script_button").click(function (ev) {
        ev.preventDefault();
        //send the script
        var code = editor.getValue();
        
        sendMessage({
            "data": "WREN_SCRIPT_START"
        });
        
        sendMessage({
            "data": code
        });
        
        sendMessage({
            "data": "WREN_SCRIPT_END"
        });
        
        var mname = "WREN_SCRIPT_SAVE " + $("#module_name")[0].value
        sendMessage({
            "data": mname
        });
        
    });

    $("#freq_slider").on("change", function (event, ui) {
        sendMessage({
            "data": "AA " + $("#freq_slider")[0].value
        });
    });

    draw();
});

function draw() {
    var canvas = document.getElementById("stage");
    if (canvas.getContext) {
        var ctx = canvas.getContext("2d");
        ctx.fillStyle = "#D74022";
        ctx.fillStyle = "#000000";
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        ctx.fillStyle = "rgba(0,0,0,1)";
        canvas.style.webkitFilter = "blur(0px)";

    }
}

function sweepFreq() {
    if ($(sweep_checkbox).is(":checked")) {
        $("#freq_slider")[0].stepUp(3);
        $("#freq_slider").trigger("change");
    }
}

function preventDefaults(e) {
    e.preventDefault();
    e.stopPropagation();
}

function handleDrop(e) {
    e.preventDefault();
    e.stopPropagation();
    let dt = e.dataTransfer;
    let files = dt.files;
    handleFileList(files);
}

function handleFileList(files) {
    ([...files]).forEach(handleFile);
}

function handleFile(file) {
    console.log(`File added: ${file.name}`);
    reader = new FileReader();
    reader.onload = function (event) {
        file_dict = JSON.parse(event.target.result);
        r = (file_dict.memory.files[0]).symbols;
        symbols = [];
        symbols_data_table.clear();
        for (file in file_dict.memory.files) {
            $.extend(symbols, file_dict.memory.files[file].symbols);
            for (symbol in file_dict.memory.files[file].symbols) {
                symbols_data_table.row.add(file_dict.memory.files[file].symbols[symbol]);
            }
        }
        symbols_data_table.draw();
    };
    reader.readAsText(file);
}


function renderMonitor() {
    var c = document.getElementById("monitor_canvas");
    if (c.getContext) {
        var ctx = c.getContext("2d");
        //ctx.fillStyle = "#D740F2";
        ctx.strokeStyle = "#3d85de60";

        for (const [key, value] of Object.entries(watch.APPS)) {
            if (key != "root") {
                /*
                pixel_color[0]   = 255;
                pixel_color[1]   = 0;
                pixel_color[2]   = 255;
                pixel_color[3]   = 80;
                x = watch.APPS[key].update_loop_time_max/10;
                ctx.putImageData(pixel, x, c.height-2);
                */

                x = watch.APPS[key].update_period_max / 500.0;
                ctx.beginPath();
                ctx.moveTo(x, c.height - 1);
                pixel_color[0] = 0;
                pixel_color[1] = 255;
                pixel_color[2] = 255;
                pixel_color[3] = 80;
                x2 = x + watch.APPS[key].update_cpu_time_max / 20;
                ctx.lineTo(x2, c.height - 1);
                ctx.stroke();
                ctx.putImageData(pixel, x, c.height - 2);
            }
        }
        //slide the image up

        if (1) { //Math.random() > 0.0001
            ctx.fillStyle = "#000000";
            ctx.strokeStyle = "#000000";

            var imageData = ctx.getImageData(0, 0, c.width, c.height);
            ctx.putImageData(imageData, 0, -1);
            //draw the new row
            ctx.beginPath();
            ctx.moveTo(0, c.height - 1);
            ctx.lineTo(c.width, c.height - 1);
            ctx.stroke();
        }

    }
}


function renderAudioBlocks() {
    //clear any existing nodes
    lgraph.clear();
    if (dataflow_render_by == "type") {
        renderAudioBlocksByType()
    } else if (dataflow_render_by == "flow") renderAudioBlocksByFlow();
    //draw the connections
    for (c in watch.AudioDirector.AudioConnectionPool) { //for each connection in the pool
        //node_const.connect(0, node_watch, 0 );
        if (watch.AudioDirector.AudioConnectionPool[(c).toString()].inUse == 1) {
            source_name = watch.AudioDirector.AudioConnectionPool[(c).toString()].srcType;
            dest_name = watch.AudioDirector.AudioConnectionPool[(c).toString()].destType;

            source_name += ":" + watch.AudioDirector.AudioConnectionPool[(c).toString()].srcInstance;
            dest_name += ":" + watch.AudioDirector.AudioConnectionPool[(c).toString()].destInstance;

            watch.AudioDirector.AudioStreams[source_name].graph_node.
            connect(watch.AudioDirector.AudioConnectionPool[(c).toString()].srcPort,
                watch.AudioDirector.AudioStreams[dest_name].graph_node,
                watch.AudioDirector.AudioConnectionPool[(c).toString()].destPort);
        }
    }
}


function renderAudioBlocksByType() {
    var x = 0;
    var y = 0;
    var zonex = [0, 20, 620, 620, 1000, 1400];
    var zonex_start = [0, 20, 620, 620, 1000, 1400];
    var zone_width = [400, 400, 400, 820, 1400, 200];
    var zonex_gap = [90, 90, 90, 90, 90, 90];
    var zoney = [0, 300, 50, 300, 300, 0];
    var zoney_gap = [75, 125, 125, 200, 125, 125];

    var column_color = ["#213A55", "#2F2A34", "##18181F", "#40474E", "#551234", "#371B32"];
    var zi = 0;
    var erist_test;

    //draw the blocks - by type
    for (c in watch.AudioDirector.AudioStreams) { //for each component in the pool 
        erist_test = LiteGraph.createNode("basic/ErisAudio");
        inport = watch.AudioDirector.AudioStreams[c].inputs;
        for (i = 0; i < inport; i += 1) {
            erist_test.addInput(i.toString(), "number");
        };
        outport = watch.AudioDirector.AudioStreams[c].outputs;
        for (i = 0; i < outport; i += 1) {
            erist_test.addOutput(i.toString(), "number");
        };
        erist_test.title = c;

        switch (watch.AudioDirector.AudioStreams[c].category) {
        case "input-function":
            zi = 0;
            break;
        case "output-function":
            zi = 5;
            break;
        case "synth-function":
            zi = 1;
            break;
        case "effect-function":
            zi = 2;
            break;
        case "mixer-function":
            zi = 3;
            break;
        default:
            zi = 4;
        }
        erist_test.bgcolor = column_color[zi];

        erist_test.pos = [zonex[zi], zoney[zi]];
        zonex[zi] += zonex_gap[zi];

        if (zonex[zi] > zone_width[zi]) {
            zonex[zi] = zonex_start[zi];
            zoney[zi] += zoney_gap[zi];
        } else zonex[zi] += zonex_gap[zi];

        lgraph.add(erist_test);
        watch.AudioDirector.AudioStreams[c].graph_node = erist_test;
    };

}


function renderAudioBlocksByFlow() {
    var x = 0;
    var y = 0;
    var zonex = [0, 20, 620, 620, 1000, 1400];
    var zonex_start = [0, 20, 620, 620, 1000, 1400];
    var zone_width = [400, 400, 400, 820, 1400, 200];
    var zonex_gap = [90, 90, 90, 90, 90, 90];
    var zoney = [0, 300, 50, 300, 300, 0];
    var zoney_gap = [75, 125, 125, 200, 125, 125];

    var column_color = ["#213A55", "#2F2A34", "##18181F", "#40474E", "#551234", "#371B32"];
    var zi = 0;
    var erist_test;
    var rank = [];
    var next_rank = [];

    //draw the blocks - by type
    //first pass find the output (sink) blocks
    for (obj in watch.AudioDirector.AudioStreams) {
        //for each component in the pool 
        zi = -1;
        switch (watch.AudioDirector.AudioStreams[obj].outputs) {
        case 0:
            erist_test = LiteGraph.createNode("basic/ErisAudio");
            inport = watch.AudioDirector.AudioStreams[obj].inputs;
            for (i = 0; i < inport; i += 1) {
                erist_test.addInput(i.toString(), "number");
            };
            outport = watch.AudioDirector.AudioStreams[obj].outputs;
            for (i = 0; i < outport; i += 1) {
                erist_test.addOutput(i.toString(), "number");
            };
            erist_test.title = obj;
            zi = 5;
            rank.push(obj);
            break;
        default:
            delete watch.AudioDirector.AudioStreams[obj].graph_node
            delete watch.AudioDirector.AudioStreams[obj].placed
        }
        if (zi >= 0) {
            erist_test.bgcolor = column_color[zi];
            erist_test.pos = [zonex[zi], zoney[zi]];
            zonex[zi] += zonex_gap[zi];

            if (zonex[zi] > zone_width[zi]) {
                zonex[zi] = zonex_start[zi];
                zoney[zi] += zoney_gap[zi];
            } else zonex[zi] += zonex_gap[zi];

            lgraph.add(erist_test);
            watch.AudioDirector.AudioStreams[obj].graph_node = erist_test;
            watch.AudioDirector.AudioStreams[obj].placed = 1;
        }

    }

    x = 1400;
    while (rank.length > 0) {
        //search all connections for destination blocks pointing to the current rank
        y = 0;
        x -= 400;
        for (c in watch.AudioDirector.AudioConnectionPool) {
            //for each connection in the pool
            i = watch.AudioDirector.AudioConnectionPool[(c).toString()].destType + ":" + watch.AudioDirector.AudioConnectionPool[(c).toString()].destInstance;
            if (rank.includes(i)) {
                s = watch.AudioDirector.AudioConnectionPool[(c).toString()].srcType + ":" + watch.AudioDirector.AudioConnectionPool[(c).toString()].srcInstance;
                next_rank.push(s);
            }
        }

        found = 0;
        for (name of next_rank) {
            if (typeof watch.AudioDirector.AudioStreams[name].graph_node == "undefined") {
                found = 1;
                erist_test = LiteGraph.createNode("basic/ErisAudio");
                inport = watch.AudioDirector.AudioStreams[name].inputs;
                for (i = 0; i < inport; i += 1) {
                    erist_test.addInput(i.toString(), "number");
                };
                outport = watch.AudioDirector.AudioStreams[name].outputs;
                for (i = 0; i < outport; i += 1) {
                    erist_test.addOutput(i.toString(), "number");
                };
                erist_test.title = name;
                erist_test.pos = [x, y];
                y += 50 + ((inport + outport) * 20);
                lgraph.add(erist_test);
                watch.AudioDirector.AudioStreams[name].graph_node = erist_test;
            }
        }

        rank = next_rank.map((x) => x);
        if (found == 0) rank.length = 0;
        next_rank.length = 0;
    }
    
    //TODO: plot seperatly unconnected or unterminated to a sink block connection chains
    
    
    
    
    
    
    
    
    
    
}