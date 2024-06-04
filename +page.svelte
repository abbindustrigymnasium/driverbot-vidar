<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>MQTT Button</title>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.2/mqttws31.min.js"></script>
</head>
<body>
  <button id="connectButton">Connect to MQTT</button>
  <button id="sendMessageButton">Send Message</button><br>
  <table>
//Buttons
	<tr>
		<td></td>
		<td><button id="W">W</button></td>
		<td></td>
	</tr>
	<tr>
		<td><button id="A">A</button></td>
		<td><button id="S">S</button></td>
		<td><button id="D">D</button><br></td>
	</tr>
  </table>
  
  
  
  
  <button id="STOP">STOP</button>

 
  <script>
    // MQTT broker connection options
    const brokerUrl = 'maqiatto.com';
    const port = 8883; // MQTT over WebSockets port
    const clientId = 'clientId_' + Math.random().toString(16).substr(2, 8); // Generate a random client ID
    const username = 'vidar.morin@hitachigymnasiet.se'; // Your Maqiatto username
    const password = '1234'; // Your Maqiatto password
 
    // MQTT client instance
    let client = null;
	//let gamepadIndex = null;
    //let intervalId = null;
 
    // Function to connect to the MQTT broker
    function connectToBroker() {
      // Initialize MQTT client
      client = new Paho.MQTT.Client(brokerUrl, port, clientId);
     
      // Set callback handlers
      client.onConnectionLost = onConnectionLost;
      client.onMessageArrived = onMessageArrived;
 
      // Connect to MQTT broker
      client.connect({
        onSuccess: onConnect,
        userName: username,
        password: password,
        useSSL: false // Maqiatto does not support SSL
      });
    }


 
    // Event listener for button click to connect
    document.getElementById('connectButton').addEventListener('click', () => {
      if (!client || !client.isConnected()) {
        connectToBroker();
      } else {
        console.log('Already connected to MQTT broker.');
      }
    });

	//Connect gamepad (joystick)
	window.addEventListener("gamepadconnected", function(e) {
  	console.log("Gamepad connected, id: " + e.gamepad.id);
  
  //Sense coordinates for right turn
  setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[0];
        if (axis == 1) {
          console.log("right");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'right'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);
});

//Sense coordinates for a left turn
setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[0];
        if (axis == -1) {
          console.log("left");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'left'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);

  //Sense coordinates for forward movement
  setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[1];
        if (axis == -1) {
          console.log("forward");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'forward'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);

  //Sense coordinates for backwards movement
  setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[1];
        if (axis == 1) {
          console.log("backwards");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'backwards'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);

  //Sense if in dead zone (Stop)
  setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[1];
        if (axis >= 0.00005 && axis <= 0.00007) {
          console.log("STOP");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'STOP'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);

  //Sense if in dead zone (Center steering)
  setInterval(function() {
    var gamepads = navigator.getGamepads();
    for (var i = 0; i < gamepads.length; i++) {
      var gp = gamepads[i];
      if (gp) {
        var axis = gp.axes[0];
        if (axis >= -0.9 && axis <= 0.9) {
          console.log("center");
		  const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'center'; // Change to your desired message
      sendMessage(topic, message);
        }
      }
    }
  }, 100);

// Called when a message arrives
function onMessageArrived(message) {
  console.log('Message received:', message.payloadString);
}

// Function to send a message to the broker
function sendMessage(topic, message) {
  if (client && client.isConnected()) {
    const messageObject = new Paho.MQTT.Message(message);
    messageObject.destinationName = topic;
    client.send(messageObject);
    console.log('Message sent:', message);
  } else {
    console.error('Not connected to MQTT broker.');
  }
}
 
 
    // Called when the client connects
    function onConnect() {
      console.log('Connected to MQTT broker.');
    }
 
    // Called when the client loses its connection
    function onConnectionLost(responseObject) {
      if (responseObject.errorCode !== 0) {
        console.error('Connection lost:', responseObject.errorMessage);
      }
    }
 
    // Called when a message arrives
    function onMessageArrived(message) {
      console.log('Message received:', message.payloadString);
    }
 
    // Function to send a message to the broker
    function sendMessage(topic, message) {
      if (client && client.isConnected()) {
        const messageObject = new Paho.MQTT.Message(message);
        messageObject.destinationName = topic;
        client.send(messageObject);
        console.log('Message sent:', message);
      } else {
        console.error('Not connected to MQTT broker.');
      }
    }

	//MQTT for button input forward
	document.getElementById('W').addEventListener('click', () => {
      const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'forward'; // Change to your desired message
      sendMessage(topic, message);
    });

	//MQTT for button input backwards
	document.getElementById('S').addEventListener('click', () => {
      const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'backwards'; // Change to your desired message
      sendMessage(topic, message);
    });

	//MQTT for button input left
	document.getElementById('A').addEventListener('click', () => {
      const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'left'; // Change to your desired message
      sendMessage(topic, message);
    });

	//MQTT for button input right
	document.getElementById('D').addEventListener('click', () => {
      const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'right'; // Change to your desired message
      sendMessage(topic, message);
    });

	//MQTT for button input stop
	document.getElementById('STOP').addEventListener('click', () => {
      const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'STOP'; // Change to your desired message
      sendMessage(topic, message);
    });

	//MQTT for arrow key steering inputs
	document.addEventListener('keydown', function(event) {
    if(event.keyCode == 37 || event.keyCode == 65) {
        console.log('Left was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'left'; // Change to your desired message
      sendMessage(topic, message);
    }
    else if(event.keyCode == 39 || event.keyCode == 68) {
        console.log('Right was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'right'; // Change to your desired message
      sendMessage(topic, message);
    }
	else if(event.keyCode == 38 || event.keyCode == 87) {
        console.log('Up was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'forward'; // Change to your desired message
      sendMessage(topic, message);
    }
	else if(event.keyCode == 40 || event.keyCode == 83) {
        console.log('Down was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'backwards'; // Change to your desired message
      sendMessage(topic, message);
    }
	else if(event.keyCode == 32) {
        console.log('Space was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'STOP'; // Change to your desired message
      sendMessage(topic, message);
	}
	//MQTT for blinkers
	else if(event.keyCode == 81) {
        console.log('Q was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'blinkersV'; // Change to your desired message
      sendMessage(topic, message);
	}
	else if(event.keyCode == 69) {
        console.log('E was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'blinkersH'; // Change to your desired message
      sendMessage(topic, message);
	}

	//MQTT to center the steering
	else if(event.keyCode == 16) {
        console.log('Shift was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'center'; // Change to your desired message
      sendMessage(topic, message);
	}

	//MQTT for headlights
	else if(event.keyCode == 76) {
        console.log('L was pressed');
		const topic = 'vidar.morin@hitachigymnasiet.se/broker_driverbot'; // Change to your desired topic
      const message = 'Headlights'; // Change to your desired message
      sendMessage(topic, message);
	}	
});
  </script>
</body>
</html>
