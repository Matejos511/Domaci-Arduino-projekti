document.addEventListener("DOMContentLoaded", function() {
  // Ta del kode se zazene na zacetku

  // Poskrbimo za pravilno raztegovanje okna
  var doResize = function() {
    var canvas = document.getElementById("canvas");
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
  };
  window.addEventListener("resize", doResize);
  doResize();

  // Zazenemo glavni del programa (ki ga sprogramirate vi)
  main();
});
