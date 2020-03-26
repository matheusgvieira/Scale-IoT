var btnElement = document.querySelector('div.container button');


function getphoto(){
    var img = document.createElement('img');;
    img.src = "https://www.w3.org/2008/site/images/logo-w3c-screen-lg";
    document.getElementById('img').appendChild(img);
}
btnElement.onclick = getphoto;