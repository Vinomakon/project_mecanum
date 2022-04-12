var url = "ws://192.168.4.1:1337/";
var output;

// This is called when the page finishes loading
function init() {
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
function diagonal_lu() {
    doSend("0110");
}

function forward() {
    doSend("1111");
}

function diagonal_ru() {
    doSend("1001");
}

function left() {
    doSend("2112");
}

function right() {
    doSend("1221");
}

function diagonal_ld() {
    doSend("2002");
}

function backward() {
    doSend("2222");
}

function diagonal_rd() {
    doSend("0220");
}

function rotate_ccw() {
    doSend("2121");
}

function stop() {
    doSend("0000");
}

function rotate_cw() {
    doSend("1212");
}

// Call the init function as soon as the page loads
window.addEventListener("load", init, false);