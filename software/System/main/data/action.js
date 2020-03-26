var deg = 0;

function blockFrame(){
  var iframe = document.getElementById("myFrame").style.display = "block";
}
function capturePhoto() {
  var xhr = new XMLHttpRequest();
  xhr.open('GET', "/capture", true);
  xhr.send();
}
function scale() {
  var xhr = new XMLHttpRequest();
  xhr.open('GET', "/peso", true);
  xhr.send();
  blockFrame();
}
function rotatePhoto() {
  var img = document.getElementById("photo");
  deg += 90;
  if(isOdd(deg/90)){ document.getElementById("container").className = "vert"; }
  else{ document.getElementById("container").className = "hori"; }
  img.style.transform = "rotate(" + deg + "deg)";
}
function isOdd(n) { return Math.abs(n % 2) == 1; }


/*function getWeight() {
  var iframe = document.getElementById("myFrame");
  var weight = iframe.contentWindow.document.getElementsByTagName("p");
  
  document.getElementById("div.rectangle").appendChild(weight);
}
function getWeight(){
  var iFrame =  document.getElementById('myFrame');
  var iFrameBody;
  if ( iFrame.contentDocument ) 
  { // FF
    iFrameBody = iFrame.contentDocument.getElementsByTagName('body')[0];
  }
  else if ( iFrame.contentWindow ) 
  { // IE
    iFrameBody = iFrame.contentWindow.document.getElementsByTagName('body')[0];
  }
   console.log(iFrameBody.innerHTML);
}*/