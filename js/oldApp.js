let serial;
let oldNote = 0;
let synth = new Tone.Synth().toMaster()

var video;

// A variable for the color we are searching for.
var trackColor;
function setup() {
  //set up color tracking
  createCanvas(320, 240);
  pixelDensity(1);
  video = createCapture(VIDEO);
  video.size(width,height);
  // The above function actually makes a separate video
  // element on the page.  The line below hides it since we are
  // drawing the video to the canvas
  video.hide();

  // Start off tracking for red
  trackColor = [255, 0, 0];

  // serial = new p5.SerialPort(); // make a new instance of  serialport librar
  //  // serial.on('list', printList); // callback function for serialport list event
  //  serial.on('data', serialEvent); // callback for new data coming in
  //  serial.list(); // list the serial ports
  //  serial.open("/dev/cu.usbmodem1411"); // open a port
  //check available cameras
// List cameras and microphones. Select USB camera if available
if (!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
  console.log("enumerateDevices() not supported.");
  return;
}

// navigator.mediaDevices.enumerateDevices()
// .then(function(devices) {
//   devices.forEach(function(device) {
//     console.log(device.kind + ": " + device.label +
//                 " id = " + device.deviceId);
//   });
// })
// .catch(function(err) {
//   console.log(err.name + ": " + err.message);
// });
  // var options = {
  //   video: {
  //     optional: [{
  //       sourceId: 'put_desired_source_id_here'
  //     }]
  //   }
// };    // The camera can be initialized directly using an
    // element from the array returned by list():
}

function serialEvent() {
  // this is called when data is recieved
  //  let rawData = trim(serial.readLine());
  //  console.log('rawData', rawData);
  //  if (rawData.length > 0) {             // is the something there ?
  //    //ACCELEROMETER CODE
  //        let fromSerial = split(rawData, ",").map(x => {
  //      console.log('x: ', x);
  //          let returnInt = parseInt(x);
  //          console.log('return int: ', returnInt)
  //          return Math.ceil(returnInt/15)*15;
  //        });
  //    //   round cms to nearest 15 to account for 10 led sections (each is 15cm long)
  //    //    let fromSerial = Math.ceil(parseInt(rawData)/15)*15;
  //    console.log("from serial: ", fromSerial);
  //    //    //play X note for Y duration
  //    // filter for noise
  //    // if "player" is playing same note as "Conductor", play it, otherwise remain silent
  //    if (fromSerial[1] < (oldNote +100) && fromSerial[1] > (oldNote - 100) && fromSerial[0] == fromSerial[1]) {
  //      console.log('playing')
  //      synth.triggerAttackRelease(fromSerial[1]*10, '8n');
  //      oldNote = fromSerial[1];
  //    }
  //  }
}

function draw() {


  // Draw the video
  image(video,0,0);

  // We are going to look at the video's pixels
  video.loadPixels();

  // Before we begin searching, the "world record" for closest color is set to a high number that is easy for the first pixel to beat.
  var worldRecord = 500;

    // XY coordinate of closest color
    var closestX = 0;
    var closestY = 0;

    for (var y = 0; y < video.height; y++ ) {
      for (var x = 0; x < video.width; x++ ) {
        var loc = (x + y * video.width) * 4;
        // The functions red(), green(), and blue() pull out the three color components from a pixel.
        var r1 = video.pixels[loc];
        var g1 = video.pixels[loc + 1];
        var b1 = video.pixels[loc + 2];

        var r2 = trackColor[0];
        var g2 = trackColor[1];
        var b2 = trackColor[2];
        console.log([r1, g1, b1, r2, g2, b2]);

        // Using euclidean distance to compare colors
                var d = dist(r1, g1, b1, r2, g2, b2); // We are using the dist( ) function to compare the current color with the color we are tracking.
        //
        //        // If current color is more similar to tracked color than
        //        // closest color, save current location and current difference
                if (d < worldRecord) {
                  worldRecord = d;
                  closestX = x;
                  closestY = y;
                  serial.write(closestX + ",");
                  serial.write(closestY + ",");
                  serial.write("/n");
                }
              }
            }
  //
  // We only consider the color found if its color distance is less than 10.
  // This threshold of 10 is arbitrary and you can adjust this number depending on how accurate you require the tracking to be.
  if (worldRecord < 50) {
    // Draw a circle at the tracked pixel
    fill(trackColor);
    strokeWeight(4.0);
    stroke(0);
    ellipse(closestX, closestY, 16, 16);
  }
}


function mousePressed() {
  // Save color where the mouse is clicked in trackColor variable
  trackColor = video.get(mouseX,mouseY);
  console.log(trackColor);
}

