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

function pingLeaf(leaf) {
  return new Promise(function(resolve, reject) {
    $.get(leaf + '/ping', function(json_data) {
      if (json_data != undefined) {
        resolve(json_data.return_value);
      } else {
        reject(-1);
      }
    });
  });
}
