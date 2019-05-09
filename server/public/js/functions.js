function getJSONFile(fileName) {

  // Use a promise to make sure that the file is recieved before continuing
  return new Promise(function(resolve, reject) {
    $.get('/get-json?file=' + fileName, function(json_data) {
      if (json_data != undefined) {
        resolve(json_data);
      } else {
        reject(json_data);
      }
    });
  });

}

function length(JSONObj) {
  return Object.keys(JSONObj).length;
}

function test() {
  getJSONFile('devices.json').then(function(response) {
    console.log(response, length(response));
  }, function(error) {
    console.log('fail!');
  });
}

function setColour() {

  var hexColour = document.getElementById("color").value;

  var red   = parseInt("0x" + hexColour.substring(1,3));
  var green = parseInt("0x" + hexColour.substring(3,5));
  var blue  = parseInt("0x" + hexColour.substring(5,7));

  var index = document.getElementById("index").value;

  var colour = red + ',' + green + ',' + blue + ',' + index + ",1";

  $.get('nanoleaf_1/setColour?params=' + colour, function(json_data) {
    console.log(json_data.return_value);
  });
}

function toggleAll() {
  $.get('nanoleaf_1/toggleAll', function(json_data) {
    //console.log(json_data.return_value);
  });
}

var slider = document.getElementById("brightnessRange");

slider.oninput = function() {
  console.log(this.value.toString());
  $.get('/nanoleaf_1/setBrightness?params=' + this.value.toString(), function(json_data) {
    console.log(json_data.return_value);
  });
}
