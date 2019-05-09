getJSONFile('devices.json').then(function(response) {
  console.log(response);
  //document.getElementById('totalDevices').value = length(response);
}, function(error) {
  console.log('fail!');
});

// Change the class of a clicked triangle
function changeColor(id) {

}

// Add all the triangles to the svg
var cols = 10;
var rows = 10;

var length = 50;
var height = length * 0.86602540378

var svg = document.getElementById("triHolder");
svg.setAttribute("width",  (cols * length + length / 2));
svg.setAttribute("height", (rows * height));

for (var i = 0; i < rows; i++) {

  for (var j = 0; j < cols; j++) {
    var p1x = (j * length) + (i % 2) * length / 2;
    var p1y = i * height;
    var p2x = (length + j * length) + (i % 2) * length / 2;
    var p2y = i * height;
    var p3x = (length / 2 + j * length) + (i % 2) * length / 2;
    var p3y = height + i * height;

    var points = p1x + ',' + p1y + ' ' + p2x + ',' + p2y + ' ' + p3x + ',' + p3y;

    var tri = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
    tri.setAttribute("class", "leafBuilderUnclicked");
    tri.setAttribute("id", "tri" + i * j);
    tri.setAttribute("points", points);
    tri.addEventListener("click", function() {
      if (this.className.baseVal == "leafBuilderClicked") {
        this.className.baseVal = "leafBuilderUnclicked";
      } else {
        this.className.baseVal = "leafBuilderClicked";
      }
    });
    svg.appendChild(tri);
  }
}
