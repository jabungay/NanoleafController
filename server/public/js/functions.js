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


// Make sure the client is still available
// TODO: regularly ping the client to make sure it's still alive
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
