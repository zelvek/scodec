const fs = require('fs');
const crypto = require('crypto');
const config = JSON.parse(fs.readFileSync(__dirname+'/../config/config.json'));

function registerData(socket, path, mode, locals) {
  if (typeof path === 'undefined') {
    return;
  }

  let splitPath;
  let data = {};

  switch (mode) {
    case config.const.keyMode:
      locals.key.path = path;

      splitPath = path.split('/');
      locals.key.name = splitPath[splitPath.length-1];

      locals.key.size = fs.lstatSync(locals.key.path).size;
      hashFile(socket, path, locals.key.size);
      data.type = "key";
      data.filesize = locals.key.size;
      data.filename = locals.key.name;

      break;
    case config.const.fileMode:
      locals.file.path = path;

      splitPath = path.split('/');
      locals.file.name = splitPath[splitPath.length-1];

      locals.file.size = fs.lstatSync(locals.file.path).size;
      hashFile(socket, path, locals.file.size);

      data.type = "file";
      data.filesize = locals.file.size;
      data.filename = locals.file.name;

      break;
  }


  socket.emit('fileChosen', data, mode);
}
function hashFile(socket, path, size) {
  // If larger than 1 GB
  if (size > 1000000000) {
      let errorMsg = "File is too large"
      socket.emit('hashDone', errorMsg);
  }else {
    fs.readFile(path, (err, data) => {
        socket.emit('hashDone', crypto.createHash('md5').update(data).digest("hex"));
    });
  }
}
function checkKey(path) {
    let keyText = fs.readFileSync(path).toString('utf-8');

    let keyLength = parseInt(keyText.slice(1, 2));

    if (isNaN(keyLength)) {
      console.log("Failed to fetch keyLength");
      return 0;
    }

    let rgx = /\[(.*)\]/;
    let rawKey = keyText.match(rgx);
    let splitKey = [];
    let lineLength = 0;

    if (typeof rawKey[1] === null) {
      console.log("Failed to fetch the key string");
      return 0;
    }else {
      let i = 0;

      splitKey = rawKey[1].split(' ');
      lineLength = splitKey[0].length;

      // Check equal columns sizes
      if (splitKey.length != keyLength) {
          return 0;
      }

      // Check line length
      for (i = 0; i < splitKey.length; i++) {
        if (splitKey[i].length !== lineLength) {
          return 0;
        }
      }
    }

    return 1;
}


module.exports = {
  registerData,
  hashFile,
  checkKey
}
