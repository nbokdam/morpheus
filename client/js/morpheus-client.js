/**
 * Created by nbokdam on 20-5-17.
 */
var rSlider, gSlider, bSlider, tSlider;

var fadeLights = function() {
  const fa = {
    r: rSlider[0].value,
    g: gSlider[0].value,
    b: bSlider[0].value,
    t: tSlider[0].value
  };
  console.log(fa);
  $.post("/api/Beds/fade", fa);
  return fa;
}

$(function() {
  rSlider = $("#redslider");
  gSlider = $("#greenslider");
  bSlider = $("#blueslider");
  tSlider = $("#timeslider");
})

