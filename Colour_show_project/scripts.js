const red = document.querySelector(".red");
const cyan = document.querySelector(".cyan");
const violet = document.querySelector(".violet");
const orange = document.querySelector(".orange");
const pink = document.querySelector(".pink");

const center = document.querySelector(".center");

// consol.log(window.getComputedStyle(red).backgroundColor);

const getBGcolor = (selectedElement) => {
    return window.getComputedStyle(selectedElement).backgroundColor;
};

var orangeElementColor = getBGcolor(orange);
orange.addEventListener('mouseenter', () => {
    center.style.background = orangeElementColor;
});