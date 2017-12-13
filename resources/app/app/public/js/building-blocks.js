function unlockFileSelect() {
  $("#first-linker").removeClass("linker-disabled");
  $("#file-dropzone").removeClass("border-disabled");
  $("#file-icon").removeClass("icon-disabled");
  $("#file-button").removeClass("background-disabled");
}
function unlockSplitTile() {
  $("#last-linker, .split-line").removeClass("linker-disabled");

  $(".dot").removeClass("background-disabled");
  $("#lock-icon").removeClass("icon-disabled");
  $("#lock-open-icon").removeClass("icon-disabled");
}
function updateButton(data) {
  let thisButton = $("#"+data.type+"-button");

  thisButton.html('');

  thisButton.addClass("button-w-info");

  let parsedSize = parseSize(data.filesize);

  let newButton = "<p class=\"button-filename\">"+data.filename+"</p>\
  <p class=\"button-filesize\">"+parsedSize+"</p>\
  <p class=\"button-change\" id=\""+data.type+"\">Change</p>";

  thisButton.append(newButton);
}
function parseSize(size) {
  let sizeStr = size.toString();
  let strLen = sizeStr.length;
  let rtn;

  if (strLen > 3) {
    rtn = (size/1000).toFixed(2);
    rtn += " Ko";
    if (strLen > 6) {
      rtn = (size/1000000).toFixed(2);
      rtn += " Mo";
      if (strLen > 9) {
        rtn = (size/1000000000).toFixed(2);
        rtn += " Go";
        if (strLen > 12) {
          rtn = (size/1000000000000).toFixed(2);
          rtn += " To";
        }
      }

    }
  }else {
    rtn = size.toFixed(2);
    rtn += " o";
  }

  console.log("return ", rtn);
  return rtn;
}
function revertFileButton(disabled) {
  let disabledClass = '';
  if ( (disabled) ) {
    disabledClass = "background-disabled";
  }
  let originalButton = '<div class="button '+disabledClass+'" id="file-button">\
                          Choisir un fichier\
                        </div>'

  let fileButton = $("#file-button").removeClass("button-w-info");
  fileButton.html('');
  fileButton.append(originalButton);
}
function disableFileButton() {
  $("#file-button").addClass("background-disabled");
}
function revertKeyButton() {
  let originalButton = '<div class="button" id="key-button">\
                          Choisir une clef\
                        </div>'

  let fileButton = $("#key-button").removeClass("button-w-info");
  fileButton.html('');
  fileButton.append(originalButton);
}
function toggleLoading(mode) {
  let loadingCaption = '';
  if (mode === 'c') {
    loadingCaption = 'Encrypting file...'
  }else if (mode === 'd'){
    loadingCaption = 'Decrypting file...'
  }

  $("#loading-caption").html(loadingCaption);
  $("#loading-screen-container").toggleClass("display-loading-screen");
  $("#loading-caption").toggleClass("display-loading-animation");
}
function successReset() {
  displaySuccess();
  disableFile(false);
}
function resetAll() {
  disableFile(true);
  disableKey();
  revertKeyButton();
}
function displaySuccess() {
  $("#success-caption").toggleClass("success-animation");
  $("#loading-caption").toggleClass("display-loading-animation remove-animation");

  setTimeout( () => {
    $("#success-caption").toggleClass("success-animation");
    setTimeout( () => {
      $("#loading-screen-container").toggleClass("display-loading-screen");
    }, 600);
  }, 1200);

  setTimeout( () => {
    $("#loading-caption").toggleClass("remove-animation");
  });
}
function disableFile(disableFileButton) {
  $(".dual-tile-icon").addClass("icon-disabled");
  $("#last-linker, .split-line").addClass("linker-disabled");
  $(".dot").addClass("background-disabled");

  revertFileButton(disableFileButton);
}
function disableKey() {
  $("#first-linker").addClass("linker-disabled");
  $("#file-dropzone").addClass("border-disabled");
  revertFileButton(true);
}
function markKeyAsInvalid() {
  resetAll();
  clearKeyBackgroundState();
  keyBackgroundToWhite();
  $("#key-dropzone").addClass("invalid-border");
  $("#key-background").addClass("invalid-background");
}
function markKeyAsValid() {
  clearKeyBackgroundState();
  keyBackgroundToWhite();
  $("#key-dropzone").addClass("valid-border");
  $("#key-background").addClass("valid-background");
}
function clearKeyBackgroundState() {
  $("#key-background").css("color", "#535761");
  $("#key-dropzone").removeClass("invalid-border valid-border");
  $("#key-background").removeClass("valid-background invalid-background");
}
function keyBackgroundToWhite() {
  $("#key-background").css("color", "#eee");
}
