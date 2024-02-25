var gateway = `ws://${location.hostname}:${location.port}/ws`;
var websocket;

$(function () {
    initWebSocket();
    initUI();
});

// Init web socket when the page loads
//window.addEventListener('load', onload);

//function onload(event) {
//    initWebSocket();
//    initUI();
//    init_sbus_table();
//}

// function getReadings() {
//     websocket.send("getReadings");
// }

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
   // getReadings();
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
            $("#sbus_table_row_" + i + "_status").removeClass("offline").addClass(sbus_data[i] < 172 ? "offline" : "available");
        }
    }

    if (message.hasOwnProperty('adc_data')) {
        var adc_data = message["adc_data"];
        for (var i = 0; i < adc_data.length; i++) {
            $("#adc_in_table_row_" + i + "_value").html(adc_data[i]);
            $("#adc_in_table_row_" + i + "_progress").val(adc_data[i]);
          //  $("#adc_in_table_row_" + i + "_status").removeClass("offline").addClass(sbus_data[i] < 200 ? "offline" : "available");
        }
    }

    if (message.hasOwnProperty('pwm_data')) {
        var pwm_data = message["pwm_data"];
        for (var i = 0; i < pwm_data.length; i++) {
            $("#pwm_in_table_row_" + i + "_value").html(pwm_data[i]);
            $("#pwm_in_table_row_" + i + "_progress").val(pwm_data[i]);
            //  $("#adc_in_table_row_" + i + "_status").removeClass("offline").addClass(sbus_data[i] < 200 ? "offline" : "available");
        }
    }

    // for (var i = 0; i < keys.length; i++){
    //     var key = keys[i];
    //     document.getElementById(key).innerHTML = message[key];
    // }
}

function init_sbus_table() {
    for (var i = 0; i < 16; i++) {
        $("#sbus_table").append(createRadioRow(i, "sbus", 172, 1809));
    }
}

function init_pwm_table() {
    for (var i = 0; i < 2; i++) {
        $("#pwm_in_table").append(createRow(i, "pwm_in", 980, 2000));
    }
}

function init_adc_table() {
    for (var i = 0; i < 2; i++) {
        $("#adc_in_table").append(createRow(i, "adc_in", 0, 4048));
    }
}

function createRadioRow(i, prefix, min, max) {
    return `
        <tr>
            <td><span id="${prefix}_table_row_${i}_status" class="offline"></span></td>
            <td>${(i+1)}</td>
            <td id="${prefix}_table_row_${i}_value"></td>
            <td><meter id="${prefix}_table_row_${i}_progress" min="${min}" max="${max}"></meter></td>
        </tr>`;
}

function createRow(i, prefix, min, max) {
    return `
        <tr>
           <td>${(i + 1)}</td>
           <td id="${prefix}_table_row_${i}_pin"></td>
           <td id="${prefix}_table_row_${i}_value"></td>
           <td><meter id="${prefix}_table_row_${i}_progress" min="${min}" max="${max}"></meter></td>
        </tr>`;
}

function initUI() {
    // document
    //   .getElementById('button')
    //   .addEventListener('click', toggle);
    init_sbus_table();
    init_adc_table();
    init_pwm_table();
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