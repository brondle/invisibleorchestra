// Learning Processing
// Daniel Shiffman
// http://www.learningprocessing.com

// Example 16-11: Simple color tracking

var video;

// A variable for the color we are searching for.
var trackColor;
var distance = 0;
let serial;

let oldNote = 0;
let keyMap = {
  1: "C2", 2: "D2", 3: "E2", 4: "F2", 5: "G2", 6: "A2", 7: "B2"}
let notes = ["A", "B", "C", "D", "E", "F", "G"];
let octaves =[1, 2, 3, 4, 5, 6];
//var synth = new Tone.MonoSynth({
//  "oscillator" : {
//    "type" : "square8"
//  },
//  "envelope" : {
//    "attack" : 0.05,
//    "decay" : 0.3,
//    "sustain" : 0.4,
//    "release" : 0.8,
//  },
//  "filterEnvelope" : {
//    "attack" : 0.001,
//    "decay" : 0.7,
//    "sustain" : 0.1,
//    "release" : 0.8,
//    "baseFrequency" : 300,
//    "octaves" : 4
//  }
//}).toMaster();
//
//NOTE/FIXME: pianoFiles only works if you set up a server with http-server in this folder due to tonejs automatically requesting samples with an xml request
let pianoFiles = {};
notes.forEach(function(a1){
  octaves.forEach(function(a2){
    pianoFiles[(a1 + a2)] = `http://127.0.0.1:8080/lib/samples/piano/${a1 + a2}.mp3`;
  });
});
console.log(pianoFiles);

let piano = new Tone.Sampler(pianoFiles, {onload: this.onInstrumentReady}).toMaster();

function setup() {
  createCanvas(320, 240);
  pixelDensity(1);
  video = createCapture({
    video: {
      options: {
        sourceId: 'e65e25daa87b9eda5f3ee305dfc56256342b2e03dae49e28f1d8d9bb279bc620'
      }
    }
  });
  video.size(width,height);
  // The above function actually makes a separate video
  // element on the page.  The line below hides it since we are
  // drawing the video to the canvas
  video.hide();
// List cameras and microphones. Select USB camera if available
if (!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
  console.log("enumerateDevices() not supported.");
  return;
}

 navigator.mediaDevices.enumerateDevices()
 .then(function(devices) {
   devices.forEach(function(device) {
     console.log(device.kind + ": " + device.label +
                 " id = " + device.deviceId);
   });
 })
 .catch(function(err) {
   console.log(err.name + ": " + err.message);
 });
// var options = {
//   video: {
//     optional: [{
//       sourceId: 'put_desired_source_id_here'
//     }]
//   }
// };

  // Start off tracking for red
  trackColor = [255, 0, 0];
  serial = new p5.SerialPort(); // make a new instance of  serialport librar
  // serial.on('list', printList); // callback function for serialport list event
  serial.on('data', serialEvent); // callback for new data coming in

  serial.on('list', printList); // callback function for serialport list event
  serial.list(); // list the serial ports
  serial.open("/dev/cu.usbmodem1411"); // open a port
  //SET UP IR RECEIVER ON SEPARATE ARDUINO
}
function printList(portList) {
  for (var i = 0; i < portList.length; i++) {
    // Display the list the console:
    console.log(i + " " + portList[i]);
  }
}

function roundData(data) {

}

function serialEvent() {
  // hayk code
  	let rawData = trim(serial.readLine());
	if (rawData.length > 0) {
    //TODO: set limiter so that note is only played if rawData is N different from previous value or N (500?) milliseconds have passed
		if( rawData < 2500 ) {
			console.log("raw data: ", rawData);
      let note = convertNote(rawData, 1000, 2500, 1, 7);
      console.log("note: ", keyMap[note]);
			piano.triggerAttackRelease(keyMap[note], '8n');
      serial.clear(); //flush buffer in a potentially misguided attempt to reduce latency
		}

	}

  //brent code
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
      var r1 = video.pixels[loc   ];
      var g1 = video.pixels[loc + 1];
      var b1 = video.pixels[loc + 2];

      var r2 = trackColor[0];
      var g2 = trackColor[1];
      var b2 = trackColor[2];

      // Using euclidean distance to compare colors
      var d = dist(r1, g1, b1, r2, g2, b2); // We are using the dist( ) function to compare the current color with the color we are tracking.

      // If current color is more similar to tracked color than
      // closest color, save current location and current difference
      if (d < worldRecord) {
        distance = d;
        worldRecord = d;
        closestX = x;
        closestY = y;
        //      console.log("rgb:", [r1, g1, b1]);
        //console.log("x " +closestX + "y " + closestY);
      }
    }

  }

  // We only consider the color found if its color distance is less than 10.
  // This threshold of 10 is arbitrary and you can adjust this number depending on how accurate you require the tracking to be.
  if (worldRecord < 10) {
    //TODO: also play sounds in different sectors (i.e. horns top right or whatever)
    serial.write(closestX + ",");
    serial.write(closestY + ",");
    serial.write("\n");
    fill(trackColor);
    strokeWeight(4.0);
    stroke(0);
    ellipse(closestX, closestY, 16, 16);
  } else {
    serial.write("\n");
  }
}

function convertNote(num, in_min, in_max, out_min, out_max) {
  let convertedNote = (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return Math.round(convertedNote);
  // convert to note along full octave scale (C)  return Math.ceil(convertedNote/12) * 12;
}


function mousePressed() {
  // Save color where the mouse is clicked in trackColor variable
  trackColor = video.get(mouseX,mouseY);
  console.log(trackColor);
}

