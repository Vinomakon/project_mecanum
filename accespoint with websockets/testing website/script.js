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

    switch (String(evt.data)[0]) {
        case "1":
            document.getElementById("lu_motor_state").src = "up.png";
            console.log("changed lu motor state to up");
            break;
        case "2":
            document.getElementById("lu_motor_state").src = "down.png";
            break;
        case "0":
            document.getElementById("lu_motor_state").src = "neutral.png";
            console.log("changed lu motor state to neutral");
            break;
        default:
            break; 
    }

    switch (String(evt.data)[1]) {
        case "1":
            document.getElementById("ru_motor_state").src = "up.png";
            break;
        case "2":
            document.getElementById("ru_motor_state").src = "down.png";
            break;
        case "0":
            document.getElementById("ru_motor_state").src = "neutral.png";
            break;
        default:
            break; 
    }

    switch (String(evt.data)[2]) {
        case "1":
            document.getElementById("ld_motor_state").src = "up.png";
            break;
        case "2":
            document.getElementById("ld_motor_state").src = "down.png";
            break;
        case "0":
            document.getElementById("ld_motor_state").src = "neutral.png";
            break;
        default:
            break; 
    }

    switch (String(evt.data)[3]) {
        case "1":
            document.getElementById("rd_motor_state").src = "up.png";
            break;
        case "2":
            document.getElementById("rd_motor_state").src = "down.png";
            break;
        case "0":
            document.getElementById("rd_motor_state").src = "neutral.png";
            break;
        default:
            break; 
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
    doSend("1111");
}

// Call the init function as soon as the page loads
window.addEventListener("load", init, false);