$(document).ready( () => {
    // CONST
    const keyMode = 1;
    const fileMode = 2;

    //SOCKETS
    socket.on("fileChosen", (data, mode) => {

        if (mode == keyMode) {
          unlockFileSelect();
        }else if (mode == fileMode) {
          unlockSplitTile();
        }

        updateButton(data);
    });

    socket.on("hashDone", (hash) => {
        $("#md5-string").html(hash);
    })

    socket.on("success", () => {
        $("#success-caption").html("Success");
        successReset();
    });

    socket.on("fail", (errid, errstr) => {
        $("#success-caption").html("ERR"+errid+" -- "+errstr);
        successReset();
    });

    socket.on('invalidKey', () => {
        markKeyAsInvalid();
        resetAll();
    });

    socket.on('validKey', () => {
        markKeyAsValid();
    });
    // TOP UTILITY BUTTONS
    $("#minimize").click(function () {
        window.minimize();
    });

    $("#quit").click(function () {
        window.close();
    });

    // DRAG AND DROP


    // Since hovering child elements triggers dragleave,
    // we store as a global variable when we're hovering the child elements
    // in order to manage the classes properly
    let iconDragHover = false;
    $(window).on({
        dragover: (e) => {
            let target = $(e.target);

            if ( !(target.hasClass("dropzone")) || (target.hasClass("border-disabled")) ) {
                e.preventDefault;
                return false;
            }
        },
        drop: (e) => {
            let target = $(e.target);

            if ( !(target.hasClass("dropzone")) || (target.hasClass("border-disabled")) ) {
                e.preventDefault;
                return false;
            }
        }
    });

    $(".material-icons").on("dragenter", function(e) {
        if ( (isDisabled(this, "icon")) ) {
          return;
        }

        iconDragHover = true;
    });
    $(".material-icons").on("dragleave", function(e) {
        if ( (isDisabled(this, "icon")) ) {
          return;
        }

        iconDragHover = false;
    });

    $(".dropzone").on("dragenter", function(e) {
        if ( (isDisabled(this, "border")) ) {
          return;
        }

        e.preventDefault();
        e.stopPropagation();

        if ( !(iconDragHover) ) {
            $(this).addClass("dragover-border");
            $(this).find(".material-icons").addClass("dragover-color");
        }

        return false;
    });

    $(".dropzone").on("dragover", (e) => {
        e.preventDefault();
        e.stopPropagation();
        return false;
    });

    $(".dropzone").on("dragleave", function(e) {
        if ( (isDisabled(this, "border")) ) {
          return;
        }

        e.preventDefault();
        e.stopPropagation();

        if ( !(iconDragHover) ) {
            $(this).removeClass("dragover-border");
            $(this).find(".material-icons").removeClass("dragover-color");
        }

        return false;
    });

    $(".dropzone").on("drop", function(e) {
        if ( (isDisabled(this, "border")) ) {
          return;
        }

        let filesData = e.originalEvent.dataTransfer.files;
        let mode = 0;

        $(this).removeClass("dragover-border");
        $(this).find(".material-icons").removeClass("dragover-color");
        e.preventDefault();

        if ($(this).attr("id") === "key-dropzone") {
          mode = keyMode;
        }else if ($(this).attr("id") === "file-dropzone") {
          mode = fileMode;
        }else {
          console.log("Button type not found");
          return;
        }
        socket.emit('fileDropped', filesData[0].path, mode);
    });

    // FILE SELECTION
    $(".button").click( function(e) {
        if ( (isDisabled(this, "background")) ){
          return;
        }

        let that = $(this);

        let mode = 0;

        if (that.attr("id") === "key-button") {
          mode = keyMode;
        }else if (that.attr("id") === "file-button") {
          mode = fileMode;
        }else {
          console.log("Button type not found");
          return;
        }

        socket.emit("openFileDialog", mode);
    });

    /*$(".dropzone").dblclick( () => {
      if ( (isDisabled(this, "border")) ){
        return;
      }

      let that = $(this);

      let mode = 0;

      // TODO
      // CAN'T FIND BUTTON
        console.log(that.find(".button"));
      if (that.closest(".button").attr("id") === "key-button") {
        mode = keyMode;
      }else if (that.closest(".button").attr("id") === "file-button") {
        mode = fileMode;
      }else {
        console.log("Button type not found");
        return;
      }

      socket.emit("openFileDialog", mode);
    });*/

    // C MODULE CALL
    $("#lock-icon").click( function() {
        console.log("lock-icon clicked");
      if ( (isDisabled(this, "icon")) ) {
        return;
      }

      let cypherMode = 'c';

      socket.emit("callModule", cypherMode);

      toggleLoading(cypherMode);
    });

    $("body").click( function(e) {
        console.log("target", e.target);
        console.log("this", this);
    });

    $("#lock-open-icon").click( function() {
      if ( (isDisabled(this, "icon")) ) {
        return;
      }

      let decypherMode = 'd';

      socket.emit("callModule", decypherMode);

      toggleLoading(decypherMode);
    });
});
