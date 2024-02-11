var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    initUI();
    init_sbus_table();
}

function getReadings(){
    websocket.send("getReadings");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    //websocket.onclose = onClose;
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
    var message = JSON.parse(event.data);

    if (message.hasOwnProperty('sbus_data')) 
    {
      var sbus_data = message["sbus_data"];
      for (var i = 0; i < sbus_data.length; i++)
      {
        var cell = document.getElementById("sbus_table_row" + i.toString() + "_value");
        cell.innerHTML = sbus_data[i];
      }
    }

    // for (var i = 0; i < keys.length; i++){
    //     var key = keys[i];
    //     document.getElementById(key).innerHTML = message[key];
    // }
}

function init_sbus_table() 
{
  var table = document.getElementById('sbus_table');

  // Clear existing table
  while (table.firstChild) {
    table.removeChild(table.firstChild);
  }
  
  for (var i = 0; i < 16; i++) 
  {
    let newRow = document.createElement("tr");
    let numberCell = document.createElement("td");
    let valueCell = document.createElement("td");
    let progressCell = document.createElement("td");

    valueCell.setAttribute("id", "sbus_table_row" + i.toString() + "_value");
    progressCell.setAttribute("id", "sbus_table_row" + i.toString() + "_progress");

    numberCell.innerText = (i+1);
    valueCell.innerText = 0;
    progressCell.innerText = 0;

    newRow.appendChild(numberCell);
    newRow.appendChild(valueCell);
    newRow.appendChild(progressCell);

    table.appendChild(newRow);
  }
}

function initUI() {
  // document
  //   .getElementById('button')
  //   .addEventListener('click', toggle);
}

var red = document.getElementById("red");

// red.addEventListener("change",() => 
// {
//   console.log('Red :: ' + red.checked);
//   if(red.checked){
//     sendMessage("RED_ON"); 
//   }else{
//     sendMessage("RED_OFF"); 
//   }
// })

// var blue = document.getElementById("blue");

// blue.addEventListener("change",() => 
// {
//   console.log('Blue :: ' + blue.checked);
//   if(blue.checked){
//     sendMessage("BLUE_ON"); 
//   }else{
//     sendMessage("BLUE_OFF"); 
//   }
// })

// var green = document.getElementById("green");

// green.addEventListener("change",() => 
// {
//   console.log('Green :: ' + green.checked);
//   if(green.checked){
//     sendMessage("GREEN_ON"); 
//   }else{
//     sendMessage("GREEN_OFF"); 
//   }
// })