var url = "ws://192.168.4.1:1337/";
var output;
var buttonlu;
var buttonru;
var buttonstop;
var buttonall;
var canvaslu;
var canvasru;
var contextlu;
var contextru;

// This is called when the page finishes loading
function init() {

    // Assign page elements to variables
    buttonlu = document.getElementById("toggleLUButton");
    output = document.getElementById("output");
    canvaslu = document.getElementById("LU_stepper_state");

    buttonru = document.getElementById("toggleRUButton");
    canvasru = document.getElementById("RU_stepper_state");

    buttonstop = document.getElementById("toggleStopButton");
    
    buttonall = document.getElementById("toggleAllbutton");
    
    // Draw circle in canvas
    contextlu = canvaslu.getContext("2d");
    contextlu.arc(25, 25, 15, 0, Math.PI * 2, false);
    contextlu.lineWidth = 3;
    contextlu.strokeStyle = "black";
    contextlu.stroke();
    contextlu.fillStyle = "black";
    contextlu.fill();

    contextru = canvasru.getContext("2d");
    contextru.arc(25, 25, 15, 0, Math.PI * 2, false);
    contextru.lineWidth = 3;
    contextru.strokeStyle = "black";
    contextru.stroke();
    contextru.fillStyle = "black";
    contextru.fill();
    
    // Connect to WebSocket server
    wsConnect(url);
}

// Call this to connect to the WebSocket server
function wsConnect(url) {
    
    // Connect to WebSocket server
    websocket = new WebSocket(url);
    
    // Assign callbacks
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}

// Called when a WebSocket connection is established with the server
function onOpen(evt) {

    // Log connection state
    console.log("Connected");
    
    // Enable button
    buttonlu.disabled = false;
    buttonru.disabled = false;
    buttonall.disabled = false;
    buttonstop.disabled = false;
    
    // Get the current state of the LED
    doSend("getState")
}

// Called when the WebSocket connection is closed
function onClose(evt) {

    // Log disconnection state
    console.log("Disconnected");
    
    // Try to reconnect after a few seconds
    setTimeout(function() { wsConnect(url) }, 2000);
}

// Called when a message is received from the server
function onMessage(evt) {

    // Print out our received message
    console.log("Received: " + evt.data);
    console.log("First state: " + String(evt.data)[0] + "... Second state: " + String(evt.data)[1]);
    
    // Update circle graphic with LED state
    /*switch(evt.data) {
        case String(evt.data)[0] == "0":
            console.log("Upper Left Stepper is off");
            contextlu.fillStyle = "black";
            contextlu.fill();
            break;
        case String(evt.data)[0] == "1":
            console.log("Upper Left Stepper is on");
            contextlu.fillStyle = "red";
            contextlu.fill();
            break;
        case String(evt.data)[1] == "0":
            console.log("Upper Right Stepper is off");
            contextru.fillStyle = "black";
            contextru.fill();
            break;
        case String(evt.data)[1] == "1":
            console.log("Upper Right Stepper is on");
            contextru.fillStyle = "red";
            contextru.fill();
            break;
        default:
            break;
    }*/

    if(String(evt.data)[0] == "1") {
        console.log("Upper Left Stepper is on");
            contextlu.fillStyle = "red";
            contextlu.fill();
    } else {
        console.log("Upper Left Stepper is off");
            contextlu.fillStyle = "black";
            contextlu.fill();
    }

    if(String(evt.data)[1] == "1") {
        console.log("Upper Right Stepper is on");
            contextru.fillStyle = "red";
            contextru.fill();
    } else {
        console.log("Upper Right Stepper is off");
            contextru.fillStyle = "black";
            contextru.fill();
    }
}

// Called when a WebSocket error occurs
function onError(evt) {
    console.log("ERROR: " + evt.data);
}

// Sends a message to the server (and prints it to the console)
function doSend(message) {
    console.log("Sending: " + message);
    websocket.send(message);
    websocket.send("getState")
}

// Called whenever the HTML button is pressed
function onPressLU() {
    doSend("1000");
}

function onPressRU() {
    doSend("0100");
}

function onPressStop() {
    doSend("0000");
}

function onPressAll() {
    doSend("1100");
}

// Call the init function as soon as the page loads
window.addEventListener("load", init, false);