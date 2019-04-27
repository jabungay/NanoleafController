function getTest(params) {
  console.log(params);
  $.get('/neoleaf_1/led?params=' + params, function(json_data) {
    console.log(json_data.return_value);
  });
}

function setColour() {
  var red   = document.getElementById("RVal").value;
  var green = document.getElementById("GVal").value;
  var blue  = document.getElementById("BVal").value;

  var colour = red + ',' + green + ',' + blue;

  console.log(colour);

  $.get('neoleaf_1/setColour?params=' + colour, function(json_data) {
    console.log(json_data.return_value);
  });
}

var slider = document.getElementById("brightnessRange");

slider.oninput = function() {
  console.log(this.value.toString());
  $.get('/neoleaf_1/brightness?params=' + this.value.toString(), function(json_data) {
    console.log(json_data.return_value);
  });
}
