function isDisabled(el, type) {
  let rtn = false;
  console.log($(el));
  console.log(type+"-disabled");
  if ($(el).hasClass(type+"-disabled")) {

    rtn = true;
  }
  console.log(rtn);
  return rtn;
}
