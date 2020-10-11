var colors = generateRandomColors(6);

var squares = document.querySelectorAll(".square");
var pickedColor = pickColor();
var colorDisplay= document.getElementById("colorDisplay"); 
var messageDisplay=document.querySelector("#message");
var resetButton = document.querySelector("#reset");

resetButton.addEventListener("click",function(){
    colors=generateRandomColors(6);
    pickedColor=pickColor();
    colorDisplay.textContent=pickedColor;
    for(var i=0;i<squares.length;i++){
        squares[i].style.background=colors[i];
    }
})

colorDisplay.textContent=pickedColor;

for(var i = 0;i<squares.length; i++)
{
    //add initial colors to squares
    squares[i].style.background=colors[i];

    //add click listner to square
    squares[i].addEventListener("click",function()
    {
        var clickedColor = this.style.background;
        if(clickedColor === pickedColor)
        {
            messageDisplay.textContent="Correct";
            resetButton.textContent="Play Again ?"
            changeColors(clickedColor);
        }
        else 
        {
            this.style.background="#232323";
            messageDisplay.textContent="Try Again";
        }
    });
}

function changeColors(color){
    for(var i=0;i<squares.length;i++)
    {
        squares[i].style.background=color;
    }
}

function pickColor(){
    var random=Math.floor(Math.random()*colors.length);
    return colors[random];
}

function generateRandomColors(num){
    var arr=[];
    for(var i=0;i<num;i++)
    {
        arr.push(randomColor()) 
    }


    return arr;
}

function randomColor(){
    var r=Math.floor(Math.random()*256);
    var g=Math.floor(Math.random()*256);
    var b=Math.floor(Math.random()*256);
    return "rgb(" + r + ", " + g + ", " + b + ")";

}