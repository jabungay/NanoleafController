//This file contains all functions that are used on the nanoleaf control page.

// TODO: change depending on which leaf is selected
var currentLeaf = "nanoleaf_1";

var length = 50;
var height = length * 0.86602540378

var selected = [];

var svg = document.getElementById("triHolder");
svg.setAttribute("width",  (4 * length));
svg.setAttribute("height", (8 * height));


setDisplayColours(7);

for(var i = 0; i < 8; i++) {
  selected.push(false);

  tri = document.getElementById("tri" + i);
  tri.selected = false;
  tri.number = i;

  tri.addEventListener("click", function() {

    // Go through all elements and unselected selected one
    for(var i = 0; i < selected.length; i++) {
      if (selected[i]) {
        var selTri = document.getElementById("tri" + i);
        selTri.className.baseVal = "leafBuilderUnclicked";
        selected[i] = false;
      }
    }

    if (this.className.baseVal == "leafBuilderClicked") {
      this.className.baseVal = "leafBuilderUnclicked";
      selected[this.number] = false;
    } else {
      this.className.baseVal = "leafBuilderClicked";
      selected[this.number] = true;
    }
  });
}

function setLeafColour() {

  var hexColour = document.getElementById("color").value;

  var red   = parseInt("0x" + hexColour.substring(1,3));
  var green = parseInt("0x" + hexColour.substring(3,5));
  var blue  = parseInt("0x" + hexColour.substring(5,7));

  for (var i = 0; i < selected.length; i++) {
    if (selected[i]) {
      var colour = red + ',' + green + ',' + blue;

      $.get(currentLeaf + '/setColour?params=' + (colour  + ',' + i + ",1"), function(json_data) {
        console.log(json_data.return_value);
      });
    }
  }
}

function toggleAllLeaves() {
  $.get('nanoleaf_1/toggleAll', function(json_data) {
    //console.log(json_data.return_value);
  });
}


function setDisplayColours(panel) {
  var red, green, blue;
  if (panel >= 0) {
    getColour('Red', panel).then(function(response) {
      red = response;
      getColour('Green', panel).then(function(response) {
        green = response;
        getColour('Blue', panel).then(function(response) {
          blue = response;
          console.log(red, green, blue);
          var colour = red + ',' + green + ',' + blue;
          document.getElementById("tri" + panel).setAttribute("style", "fill:rgba(" + colour + ',1');
          setDisplayColours(panel - 1);
        }, function(error) {
          console.log('fail! b');
        });
      }, function(error) {
        console.log('fail! g');
      });
    }, function(error) {
      console.log('fail! r');
    });
  }
}

// Uses promises to get a colour from the nanoleaf panel
function getColour(colour, index) {
  return new Promise(function(resolve, reject) {
    $.get('/' + currentLeaf +'/get' + colour + '?params=' + index, function(json_data) {
      if (json_data != undefined) {
        resolve(json_data.return_value);
      } else {
        reject(0);
      }
    });
  });
}
