var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    initUI();
}

function getReadings(){
    websocket.send("getReadings");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);

    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        document.getElementById(key).innerHTML = myObj[key];
    }
}

function initUI() {
    document
        .getElementById('button')
        .addEventListener('click', toggle);
  }

var red = document.getElementById("red");

red.addEventListener("change",() => 
{
  console.log('Red :: ' + red.checked);
  if(red.checked){
    sendMessage("RED_ON"); 
  }else{
    sendMessage("RED_OFF"); 
  }
})

var blue = document.getElementById("blue");

blue.addEventListener("change",() => 
{
  console.log('Blue :: ' + blue.checked);
  if(blue.checked){
    sendMessage("BLUE_ON"); 
  }else{
    sendMessage("BLUE_OFF"); 
  }
})

var green = document.getElementById("green");

green.addEventListener("change",() => 
{
  console.log('Green :: ' + green.checked);
  if(green.checked){
    sendMessage("GREEN_ON"); 
  }else{
    sendMessage("GREEN_OFF"); 
  }
})