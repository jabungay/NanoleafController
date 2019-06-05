const express                   = require('express');
const { body,validationResult } = require('express-validator/check');
const { sanitizeBody }          = require('express-validator/filter');
const fs = require('fs')

const app  = express();

app.set('view engine', 'pug');
app.use(express.static(__dirname + '/public'));
app.use(express.urlencoded());

// node-aREST
var rest = require("arest")(app);

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
  console.log(`Express running → PORT ${server.address().port}`);
});
