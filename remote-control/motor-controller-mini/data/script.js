var gateway = `ws://${location.hostname}:${location.port}/ws`;
var websocket;

$(function () {
    initWebSocket();
    initUI();
    init_sbus_table();
});

// Init web socket when the page loads
//window.addEventListener('load', onload);

//function onload(event) {
//    initWebSocket();
//    initUI();
//    init_sbus_table();
//}

function getReadings() {
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

    if (message.hasOwnProperty('sbus_data')) {
        var sbus_data = message["sbus_data"];
        for (var i = 0; i < sbus_data.length; i++) {
            $("#sbus_table_row_" + i + "_value").html(sbus_data[i]);
            $("#sbus_table_row_" + i + "_progress").val(sbus_data[i]);
        }
    }

    // for (var i = 0; i < keys.length; i++){
    //     var key = keys[i];
    //     document.getElementById(key).innerHTML = message[key];
    // }
}

function init_sbus_table() {
    for (var i = 0; i < 16; i++) {
        $("#sbus_table").append(createRow(i));
    }
}

function createRow(i) {
    return `
        <tr>
            <td><span class="available"></span></td>
            <td>${(i+1)}</td>
            <td id="sbus_table_row_${i}_value"></td>
            <td><meter id="sbus_table_row_${i}_progress" min="200" max="1800"></meter></td>
        </tr>`;
}

function initUI() {
    // document
    //   .getElementById('button')
    //   .addEventListener('click', toggle);
}

//var red = document.getElementById("red");

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