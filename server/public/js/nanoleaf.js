//This file contains all functions that are used on the nanoleaf control page.

// TODO: change depending on which leaf is selected
var currentLeaf = "nanoleaf_1";

var hueBridgeIP = "192.168.2.73";
var hueAPIKey   = "ZuyFnRxHZz28cLcFcpzwWjPJ2Uy4utJzaIYarWcO";

var length = 100;

// Height of an equilateral triangle is length * sqrt(3) / 2
var height = length * 0.86602540378

// Array to ensure only one panel is selected at a time
var selected = [];

// Resize canvas to fit all panels
var svg = document.getElementById("triHolder");
svg.setAttribute("width",  (3 * length));
svg.setAttribute("height", (4 * height));

// Determine current colours from client
setDisplayColours(7);

// Setup brightness slider
var brightnessSlider = document.getElementById("brightness");
brightnessSlider.oninput = function() {
  $.get(currentLeaf + '/setBrightness?params=' + this.value, function(json_data) {
  });
}

// Create panel from devices json file
// TODO: make it support multiple clients
getJSONFile("devices.json").then(function(data) {
  var pointSet = data.leaf0.panel_points;

  for(var i = 0; i < pointSet.length; i++) {
    selected.push(false);

  var tri = document.createElementNS("http://www.w3.org/2000/svg", "polygon");
    tri.setAttribute("class", "leafBuilderUnclicked");
    tri.setAttribute("id", "tri" + i);

    var point = pointSet[i];

    // The coordinates of the triangle points depends on
    // whether the x and y coordinates are even or odd
    if (point[0] % 2 == 0) {
      if (point[1] % 2 == 0) {
        // (even, even)
        var x1 = point[0]         * length;
        var y1 = point[1]         * height;
        var x2 = (point[0] + 1)   * length;
        var y2 = y1;
        var x3 = (point[0] + 0.5) * length;
        var y3 = (point[1] + 1)   * height;
      } else {
        // (even, odd)
        var x1 = (point[0] + 0.5) * length;
        var y1 = point[1]         * height;
        var x2 = point[0]         * length;
        var y2 = (point[1] + 1)   * height;
        var x3 = (point[0] + 1)   * length;
        var y3 = y2;
      }
    } else {
      if (point[1] % 2 == 0) {
        // (odd, even)
        var x1 = point[0] * length;
        var y1 = point[1]         * height;
        var x2 = (point[0] - 0.5) * length;
        var y2 = (point[1] + 1)   * height;
        var x3 = (point[0] + 0.5) * length;
        var y3 = y2;
      } else {
        // (odd, odd)
        var x1 = (point[0] - 0.5) * length;
        var y1 = point[1]         * height;
        var x2 = (point[0] + 0.5) * length;
        var y2 = y1;
        var x3 = point[0]         * length;
        var y3 = (point[1] + 1)   * height;
      }
    }

    // TODO: fix offset issue
    if (point[0] >= 2) {
      x1 -= length;
      x2 -= length;
      x3 -= length;
    }

    var pointString = x1 + "," + y1 + " " + x2 + "," + y2 + " " + x3 + "," + y3;

    tri.setAttribute("points", pointString);

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

    svg.appendChild(tri);
  }
});

// Change actual panel colour
function setLeafColour() {

  var hexColour = document.getElementById("color").value;

  var red   = parseInt("0x" + hexColour.substring(1,3));
  var green = parseInt("0x" + hexColour.substring(3,5));
  var blue  = parseInt("0x" + hexColour.substring(5,7));

  for (var i = 0; i < selected.length; i++) {
    if (selected[i]) {
      var colour = red + ',' + green + ',' + blue;

      $.get(currentLeaf + '/setColour?params=' + (colour  + ',' + i + ",1"), function(json_data) {
      });

      // Update the colour of the panel on the webpage
      var tri = document.getElementById("tri" + i);
      tri.style.fill = "rgba(" + red + "," + green + "," + blue + ",1";
    }
  }
}

// Directly calls toggleAll on client
function toggleAllLeaves(action = "toggle") {
  $.get('nanoleaf_1/changeState?params=' + action, function(json_data) {
  });
}

// Recursive function to get all colours from the client
// param panel represents the max number of panels - 1
function setDisplayColours(panel) {
  var red, green, blue;
  if (panel >= 0) {
    getColour('Red', panel).then(function(response) {
      red = response;
      getColour('Green', panel).then(function(response) {
        green = response;
        getColour('Blue', panel).then(function(response) {
          blue = response;
          var colour = red + ',' + green + ',' + blue;
          document.getElementById("tri" + panel).style.fill = "rgba(" + colour + ',1';
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
