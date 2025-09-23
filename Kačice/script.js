var canvas, context;
var igralec1X = 525;
var igralec1Y = 450; 

var igralec2X = 1375;
var igralec2Y = 450;

var powerupX = -50;
var powerupY = -50;
var speed1 = 0;
var speed2 = 0;

  
function main() {
  // Zacetek programa
  // Najdemo canvas in si ga shranimo v globalno spremenljivko
  canvas = document.getElementById("canvas");
  context = canvas.getContext("2d");

 // Povezemo nase funkcije na dogodke tipkovnice in miske
 document.addEventListener("keydown", handleKeyDown);
 document.addEventListener("keyup", handleKeyUp);

  context.fillStyle = "white";      // barvilo ozadja
  context.fillRect(0,0,canvas.width,canvas.height); // pobrisemo celoten canvas

  spawnPowerup();

  tick();
}

var igralec1Smer = "";
var igralec2Smer = "";
function handleKeyDown(e) { 
  // Tipka je bila pritisnena
  console.log("Tipka:", e.key);       
  if (e.key === "s") igralec1Smer = "dol"; 
  if (e.key === "a") igralec1Smer = "levo"; 
  if (e.key === "w") igralec1Smer = "gor";  
  if (e.key === "d") igralec1Smer = "desno";
  if (e.key === "ArrowDown") igralec2Smer = "down";
  if (e.key === "ArrowLeft") igralec2Smer = "left"; 
  if (e.key === "ArrowUp") igralec2Smer = "up";  
  if (e.key === "ArrowRight") igralec2Smer = "right";
}

function handleKeyUp(e) {
  // Tipka je bila spuscena
}

function tick() {
  // Funkcija tick se poklice veckrat na sekundo
  update(); // najprej izracunajmo novo stanje igre
  draw(); // potem pa stanje izrisimo na canvas
  requestAnimationFrame(tick);
}

function update() {
  // Logiko sprogramirajte tukaj
  var xp1=igralec1X+12, yp1=igralec1Y+12; //točka tik pred glavo playerja 1
  var xp2=igralec2X+12, yp2=igralec2Y+12; //točka tik pred glavo playerja 2

  if (igralec1Smer == "dol") {
    igralec1Y = igralec1Y + 5+speed1;
    yp1+=14;
  }

  if (igralec1Smer == "levo") {
    igralec1X = igralec1X - 5-speed1;
    xp1-=14;
  }

  if (igralec1Smer == "gor") {
    igralec1Y = igralec1Y - 5-speed1;
    yp1-=14;
  }

  if (igralec1Smer == "desno") {
    igralec1X = igralec1X + 5+speed1;  
    xp1+=14;
  }

  if (igralec2Smer == "down") {
    igralec2Y = igralec2Y + 5+speed2;
    yp2+=14;
  }

  if (igralec2Smer == "left") {
    igralec2X = igralec2X - 5-speed2;
    xp2-=14;
  }

  if (igralec2Smer == "up") {
    igralec2Y = igralec2Y - 5-speed2;
    yp2-=14;
  }

  if (igralec2Smer == "right") {
    igralec2X = igralec2X + 5+speed2;
    xp2+=14;   
  }


  var pixel1 = context.getImageData(xp1, yp1, 1, 1).data;
  if (pixel1[0] != 255 && pixel1[0] != 200 && pixel1[0] != 60 && igralec1Smer != ""){
    console.log(pixel1);
    alert("Zmagal je igralec 2! (to play again spam refresh)")
  }

  var pixel2 = context.getImageData(xp2, yp2, 1, 1).data;
  if (pixel2[0] != 255 && pixel2[0] != 200 && pixel2[0] != 65 && igralec2Smer != "") {
    console.log(pixel2);
    alert("Zmagal je igralec 1! (to play again spam refresh)")
  }

  if(xp1 > powerupX && xp1 < powerupX + 25 && yp1 > powerupY && yp1 < powerupY + 25){
    powerupX = -50;
    powerupY = -50;
    context.fillStyle = "white";      // barvilo ozadja
    context.fillRect(0,0,canvas.width,canvas.height); // pobrisemo celoten canva
    speed2 = speed2 + 0.25;
    spawnPowerup();
  }
 
  if(xp2 > powerupX && xp2 < powerupX + 25 && yp2 > powerupY && yp2 < powerupY + 25){
    powerupX = -50;
    powerupY = -50;
    context.fillStyle = "white";      // barvilo ozadja
    context.fillRect(0,0,canvas.width,canvas.height); // pobrisemo celoten canvas
    speed1 = speed1 + 0.25;
    spawnPowerup();
  }

 if (igralec1X < 250) {
   igralec1X = 250;
 }

 if (igralec1X > 1650) {
   igralec1X = 1650;
 }

 if (igralec1Y < 100) {
   igralec1Y = 100;
 }

 if (igralec1Y > 850) {
   igralec1Y = 850;
 }

 if (igralec2X < 250) {
  igralec2X = 250;
 }

 if (igralec2X > 1650) {
  igralec2X = 1650;
 } 

 if (igralec2Y < 100) {
  igralec2Y = 100;
 } 

 if (igralec2Y > 850) {
  igralec2Y = 850;
 } 

}

function draw() {
  // Risanje sprogramirajte tukaj
  // Potujoc kvadratek
  //context.fillStyle = "white";      // barvilo ozadja
  //context.fillRect(0,0,canvas.width,canvas.height); // pobrisemo celoten canvas

  context.beginPath();              // Zacnemo nacrtovati pot
  context.strokeStyle = "black";    // Barva crte
  context.lineWidth = 2;            // Debelina crte
  context.moveTo(249, 99);          // Prestavimo navidezno tocko na x=0, y=0
  context.lineTo(1676, 99);         // Crta od prejsnje tocke, do x=300, y=300
  context.lineTo(1676, 876);         // Crta od prejsnje tocke, do x=350, y=350
  context.lineTo(249, 876);         // Crta od prejsnje tocke, do x=400, y=350
  context.lineTo(249, 99)
  context.stroke();                 // Narisemo zacrtano pot


  context.fillStyle = "#3cff00";
  context.fillRect( Math.round(igralec1X), Math.round(igralec1Y), 25, 25);

  context.fillStyle = "#4169e1";
  context.fillRect( Math.round(igralec2X), Math.round(igralec2Y), 25, 25);


}


function spawnPowerup() {
  powerupX = Math.random()*1375 +274;
  powerupY = Math.random()*755 +99;

  context.fillStyle = "#c8981e";
  context.fillRect(Math.round(powerupX), Math.round(powerupY), 25, 25);
}
