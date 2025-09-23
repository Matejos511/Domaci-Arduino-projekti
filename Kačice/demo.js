var canvas, context;
var x = 0;

function main() {
  // Zacetek programa

  // Najdemo canvas in si ga shranimo v globalno spremenljivko
  canvas = document.getElementById("canvas");
  context = canvas.getContext("2d");

  // Povezemo nase funkcije na dogodke tipkovnice in miske
  document.addEventListener("keydown", handleKeyDown);
  document.addEventListener("keyup", handleKeyUp);

  document.addEventListener("mousedown", handleMouseDown);
  document.addEventListener("mousemove", handleMouseMove);
  document.addEventListener("mouseup", handleMouseUp);

  tick();
}

function handleKeyDown(e) {
  // Tipka je bila pritisnena
  console.log("Tipka:", e.key);
  if (e.key === " ") x = x + 500000;
}

function handleKeyUp(e) {
  // Tipka je bila spuscena
}

function handleMouseDown(e) {
  // Tipka na miski je bila pritisnjena
  console.log("Klik:", e.clientX, e.clientY);
}
function handleMouseMove(e) {
  // Miska se je premaknila
  //console.log("mouseMove", e.clientX, e.clientY);
}
function handleMouseUp(e) {
  // Tipka na miski je bila spuscena
  //console.log("mouseUp", e.clientX, e.clientY);
}

function tick() {
  update();
  draw();
  requestAnimationFrame(tick);
}

function update() {
  x = x +5;
  if (x > canvas.width) x = x - canvas.width;
}

function draw() {
  // Pobrisemo ozadje
  context.fillStyle = "white";      // barvilo ozadja
  context.fillRect(0,0,canvas.width,canvas.height); // pobrisemo celoten canvas

  // Pobarvamo pravokotnik
  context.fillStyle = "red";        // barvilo (#red/green/blue)
  context.fillRect(1500,50,150,100); // x, y, width, height

  // Zarisemo nekaj crt
  context.beginPath();              // Zacnemo nacrtovati pot
  context.strokeStyle = "black";    // Barva crte
  context.lineWidth = 3;            // Debelina crte
  context.moveTo(50, 250);          // Prestavimo navidezno tocko na x=0, y=0
  context.lineTo(300, 300);         // Crta od prejsnje tocke, do x=300, y=300
  context.lineTo(350, 350);         // Crta od prejsnje tocke, do x=350, y=350
  context.lineTo(400, 350);         // Crta od prejsnje tocke, do x=400, y=350
  context.stroke();                 // Narisemo zacrtano pot

  // Narisemo elipso
  context.beginPath();              // Zacnemo nacrtovati novo pot
  context.strokeStyle = "green";    // Barva crte
  // Elipsa, x, y, radij1, radij2, rotacija, zacetniKot, koncniKot, smerUrinegaKazalca
  context.ellipse(400, 200, 30, 60, 0, 0, 2*Math.PI);
  context.stroke();                 // Narisemo zacrtano pot

  // Potujoc kvadratek
  context.fillStyle = "#3366CC";
  context.fillRect(x,200, 50, 50);
}