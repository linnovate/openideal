/* $Id: img_assist_popup.js,v 1.1.2.4 2009/02/01 09:59:57 sun Exp $ */

var currentMode;

function onChangeBrowseBy(el) {
  frames['img_assist_main'].window.location.href = Drupal.settings.basePath + 'index.php?q=img_assist/thumbs/' + el.value;
}

function onClickUpload() {
  frames['img_assist_main'].window.location.href = Drupal.settings.basePath + 'index.php?q=img_assist/upload';
}

function onClickStartOver() {
  frames['img_assist_main'].window.location.href = Drupal.settings.basePath + 'index.php?q=img_assist/thumbs/img_assist_browser';
}

function updateCaption() {
  var caption = frames['img_assist_main'].document.getElementById('caption');
  var title = frames['img_assist_main'].document.img_assist['edit-title'].value;
  var desc = frames['img_assist_main'].document.img_assist['edit-desc'].value;
  if (desc != '') {
    title = title + ': ';
  }
  caption.innerHTML = '<strong>' + title + '</strong>' + desc;
}

function onChangeHeight() {
  var formObj = frames['img_assist_main'].document.forms[0];
  var aspect = formObj['edit-aspect'].value;
  var height = formObj['edit-height'].value;
  formObj['edit-width'].value = Math.round(height * aspect);
}

function onChangeWidth() {
  var formObj = frames['img_assist_main'].document.forms[0];
  var aspect = formObj['edit-aspect'].value;
  var width = formObj['edit-width'].value;
  formObj['edit-height'].value = Math.round(width / aspect);
}

function onChangeLink() {
  var formObj = frames['img_assist_main'].document.forms[0];
  if (formObj['edit-link-options-visible'].value == 1) {
    if (formObj['edit-link'].value == 'url') {
      showElement('edit-url', 'inline');
    }
    else {
      hideElement('edit-url');
    }
  }
}

function onChangeSizeLabel() {
  var formObj = frames['img_assist_main'].document.forms[0];
  if (formObj['edit-size-label'].value == 'other') {
    showElement('size-other', 'inline');
  }
  else {
    hideElement('size-other');
    // get the new width and height
    var size = formObj['edit-size-label'].value.split('x');
    // this array is probably a bounding box size, not an actual image
    // size, so now we use the known aspect ratio to find the actual size
    var aspect = formObj['edit-aspect'].value;
    var width = size[0];
    var height = size[1];
    if (Math.round(width / aspect) <= height) {
      // width is controlling factor
      height = Math.round(width / aspect);
    }
    else {
      // height is controlling factor
      width = Math.round(height * aspect);
    }
    // fill the hidden width and height textboxes with these values
    formObj['edit-width'].value = width;
    formObj['edit-height'].value = height;
  }
}

function setHeader(mode) {
  if (currentMode != mode) {
    frames['img_assist_header'].window.location.href = Drupal.settings.basePath + 'index.php?q=img_assist/header/' + mode;
  }
  currentMode = mode;
}

function showElement(id, format) {
  var docObj = frames['img_assist_main'].document;
  format = (format) ? format : 'block';
  if (docObj.layers) {
    docObj.layers[id].display = format;
  }
  else if (docObj.all) {
    docObj.all[id].style.display = format;
  }
  else if (docObj.getElementById) {
    docObj.getElementById(id).style.display = format;
  }
}

function hideElement(id) {
  var docObj = frames['img_assist_main'].document;
  if (docObj.layers) {
    docObj.layers[id].display = 'none';
  }
  else if (docObj.all) {
    docObj.all[id].style.display = 'none';
  }
  else if (docObj.getElementById) {
    docObj.getElementById(id).style.display = 'none';
  }
}

function insertImage() {
  if (window.opener || (tinyMCEPopup.editor && tinyMCEPopup.editor.id)) {
    // Get variables from the fields on the properties frame
    var formObj = frames['img_assist_main'].document.forms[0];
    // Get mode  (see img_assist.module for detailed comments)
    if (formObj['edit-insertmode'].value == 'html') {
      // return so the page can submit normally and generate the HTML code
      return true;
    }
    else if (formObj['edit-insertmode'].value == 'html2') {
      // HTML step 2 (processed code, ready to be inserted)
      var content = getHTML(formObj);
    }
    else {
      // formObj['edit-insertmode'].value == 'filtertag'
      var content = getFilterTag(formObj);
    }
    return insertToEditor(content);
  }
  else {
    alert('The image cannot be inserted because the parent window cannot be found.');
    return false;
  }
}

function getHTML(formObj) {
  return frames['img_assist_main'].document.getElementById('finalhtmlcode').value;
}
