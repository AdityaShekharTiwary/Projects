var counter = document.querySelector(".counter");
var counter = document.querySelector(".followers");

let count = 1;
setInterval( () => {
    if(count <= 1000)
    count++;
    counter.innerText=count;
} ,5)