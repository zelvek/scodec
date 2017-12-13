module.exports = () => {
    const express = require('express');
    const app = express();
    const server = require('http').Server(app);

    const { dialog } = require('electron');

    const path = require('path');
    const favicon = require('serve-favicon');
    const logger = require('morgan');
    const fs = require('fs');
    const exec = require('child_process').exec;
    const crypto = require('crypto');
    const play = require('play');
    const tools = require('./modules/tools.js');

    const index = require('./routes/index');

    const config = JSON.parse(fs.readFileSync(__dirname+'/config/config.json'));
    const io = require('socket.io')(server);

    let locals = {};
    locals.key = {};
    locals.file = {};

    // view engine setup
    app.set('views', path.join(__dirname, 'views'));
    app.set('view engine', 'ejs');

    // uncomment after placing your favicon in /public
    //app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));

    app.use(express.static(path.join(__dirname, 'public')));

    app.use('/', index);

    io.on('connection', (socket) => {
        //play.sound(__dirname+'/public/sound/music.wav');

        socket.on('openFileDialog', (mode) => {

            let pathArray = dialog.showOpenDialog({
                properties: ['openFile']
            });

            if (typeof pathArray === 'undefined') {
              return;
            }

            let path = pathArray[0];

            if (mode === config.const.keyMode) {
                if ( (tools.checkKey(path)) ) {
                    tools.registerData(socket, path, mode, locals);
                    socket.emit('validKey');
                }else {
                    socket.emit('invalidKey');
                }
            }else {
                tools.registerData(socket, path, mode, locals);
            }
        });

        socket.on('fileDropped', (path, mode) => {
            if (mode === config.const.keyMode) {
                if ( (tools.checkKey(path)) ) {
                    tools.registerData(socket, path, mode, locals);
                    socket.emit('validKey');
                }else {
                    socket.emit('invalidKey');
                }
            }else {
                tools.registerData(socket, path, mode, locals);
            }
        });

        socket.on('callModule', (mode) => {
          if (locals.key.path === undefined || locals.file.path === undefined) {
            let errorMsg = "Missing path";
            socket.emit("error",errorMsg, locals.key.path, locals.key.path);

            return;
          }else if(mode === undefined){
            let errorMsg = "Mode is not defined";
            socket.emit("error", errorMsg, mode);

            return;
          }

          threadsQty = 8;

          console.log("Running : \n>> codec/scodec -k "+locals.key.path+" -f "+locals.file.path+" -"+mode+" -t "+threadsQty);

          exec(__dirname+"/codec/scodec -k "+locals.key.path+" -f "+locals.file.path+" -"+mode+" -t "+threadsQty, (error, stdout, stderr) => {
            let stdoutArray = stdout.split('__');

            console.log("stdout", stdoutArray);
            if (parseInt(stdoutArray[stdoutArray.length-1]) === 1) {
                socket.emit('success');
                /*locals.key = {};
                locals.file = {};*/
            }else {
                socket.emit('fail', stdoutArray[stdoutArray.length-1], stdoutArray[stdoutArray.length-2]);
            }
          });
        });
    });

    // catch 404 and forward to error handler
    app.use(function(req, res, next) {
      const err = new Error('Not Found');
      err.status = 404;
      next(err);
    });

    // error handler
    app.use(function(err, req, res, next) {
      // set locals, only providing error in development
      res.locals.message = err.message;
      res.locals.error = req.app.get('env') === 'development' ? err : {};

      // render the error page
      res.status(err.status || 500);
      res.render('error');
    });

    server.listen(config.server.port) // 8080
}
