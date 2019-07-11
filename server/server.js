const express                   = require('express');
const { body,validationResult } = require('express-validator/check');
const { sanitizeBody }          = require('express-validator/filter');
const fs = require('fs')

// Use jQuery
var jsdom = require("jsdom");
const { JSDOM } = jsdom;
const { window } = new JSDOM();
const { document } = (new JSDOM('')).window;
global.document = document;
var $ = jQuery = require('jquery')(window);

const app = express();

app.set('view engine', 'pug');
app.use(express.static(__dirname + '/public'));

// node-aREST
var rest = require("arest")(app);

// Hue bridge location and API key
const hueBridgeIP = "192.168.2.73";
const hueAPIKey   = "ZuyFnRxHZz28cLcFcpzwWjPJ2Uy4utJzaIYarWcO";

// Add leaves specified in devices.json
var fileName = './devices.json';
var file = fs.readFileSync(fileName, 'utf8');
leafData = JSON.parse(file);

var leafQuantity = leafData.leaf_quantity;

for (var i = 0; i < leafQuantity; i++) {
  var leaf = "leaf" + i;
  var ip = leafData[leaf].location;
  rest.addDevice('http',ip);
}

app.get('/', (req, res) => {
  res.render('index', {
    title: 'Nanoleaf Control Panel',
    newDevice: 'hidden'
  });
});

app.get('/add', (req, res) => {
  res.render('addDevice', {
    title: 'Add a New Device'
  });
});

app.get('/get-json', (req, res) => {
  var fileName = './' + req.query.file;

  var file = fs.readFileSync(fileName, 'utf8');
  jsonData = JSON.parse(file);
  res.send(jsonData);

  console.log(jsonData.leaf_quantity);
  res.end();
});

app.get('/nanoleaf1', (req, res) => {
  res.render('nanoleaf', {
    title: 'Colour Settings',
    device: '1'
  });
});

app.post('/submit-device', (req, res) => {
  var fileName = './devices.json';
  var file = require(fileName);

  const ipaddress = req.body.ipaddress;
  const name      = req.body.name;
  const devices   = (parseInt(req.body.devices) + 1).toString();

  var newDevice = ' {\"ipaddress\" : \"' + ipaddress + '\", \"name\" : \"' + name + ' \" }';

  file['device' + devices] = JSON.parse(newDevice);

  fs.writeFile(fileName, JSON.stringify(file, null, 2), function (err) {
    if (err) return console.log(err);
    console.log(JSON.stringify(file));
    console.log('writing to ' + fileName);
  });

  res.render('index', {
    title: 'Device Added Successfully',
    newDevice: 'visible'
  });

  res.end();
});

const server = app.listen(7000, () => {
  console.log(`Nanoleaf server running → PORT ${server.address().port}`);
});

// Use to control Panel with Hue dimmer switch
// TODO: make the IP not constant
var lastState = undefined;

window.setInterval(hueToNanoleaf,1000);

// Monitor Hue API to toggle panel with dimmer switch
function hueToNanoleaf() {
  pingSwitch().then(function(currentState) {
    // Only toggle if the state actually changed
     if (lastState != undefined && (currentState.state.lastupdated != lastState.state.lastupdated)) {
       // Event 1001 -> ON button is held
       if (currentState.state.buttonevent == 1001) {
         $.get('http://192.168.2.83/changeState?params=on', function(json_data) {
         });
         // Event 4001 -> OFF button is held
       } else if (currentState.state.buttonevent == 4001) {
         $.get('http://192.168.2.83/changeState?params=off', function(json_data) {
         });
       }
      }
     lastState = currentState;
  });
}

// Recieve switch information from the Hue bridge
function pingSwitch() {
  return new Promise(function(resolve,reject) {
    $.get("http://" + hueBridgeIP + "/api/" + hueAPIKey + "/sensors/12", function(json_data) {
      if (json_data != undefined) {
        resolve(json_data);
      } else {
        reject(json_data);
      }
    });
  });
}
