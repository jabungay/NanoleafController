const express  = require('express');
const app = express();

app.set('view engine', 'pug');

app.use(express.static(__dirname + '/public'));

// node-aREST
var rest = require("arest")(app);
rest.addDevice('http','192.168.2.83');

app.get('/', (req, res) => {
  res.render('index', {
    title: 'Homepage'
  });
});

const server = app.listen(7000, () => {
  console.log(`Express running → PORT ${server.address().port}`);
});
