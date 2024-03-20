# ESP32 Websocket basic template

Websocket is a communication protocaol that allows bi-directional communication between a client and a server in real time. The client side usually is the browser displaying the web page or portal, whereas the server is the backend processing data and serving the web pages.It's designed to be event-driven and optimized for low-latency. 

## Infrastructure
- HTML files (under data folder)
- javascript (in html or separate files under data folder)
- Arduino C/C++

## client-to-server cases
- Users submit a form on web portal
- Users click a button on web portal

## server-to-client cases
- Certain readings from a sensor to be displayed on the web portal
- Current state of a actuator to be displayed on the web portal

### html snippet
```html
<!--placeholder to show data coming from server side-->
<span id="data_from_server">loading...</span>

<!--form allows users to submit data to server-->
<form>
    <label>Text Input</label><br>
    <input type="text" id="text_input"><br>

    
    <input type="checkbox" id="checkbox_input" checked>
    <label>Please check here</label><br>

    <label>Select an item</label><br>
        <select id="select_input">
            <option value="1">Item 1</option>
            <option value="2">Item 2</option>
        </select>
    
</form>

<!--input button needs to be outside the for and use onclick attribute to trigger javascript function-->
<button onclick="submit_btn()">Submit</button>

<!--placeholder to show alert message after the button is clicked-->
<div id="alert_msg"></div>

```

### javascript snippet
```js
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event){
    console.log(event.data);
    // process data sent from server
    // assuming data in json format
    var myObjs = JSON.parse(event.data);
    

}

function submit_btn(){
    // read and process input data from the form
    var textmsg = document.getElementById("text_input").value;
    var isCheck = document.getElementById("checkbox_input").checked; // return true if checked, false if unchecked
    var selected = document.getElementById("select_input").value;

    var msg_to_server = "text:"textmsg + " selected value:" + selected;

    websocket.send(msg_to_server);

    // display alert info 
    document.getElementById("alert_msg").innerHTML = "Your message successfully sent!";

}
```

### ESP32 code snippet
```

```