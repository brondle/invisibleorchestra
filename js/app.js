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
    serial = new p5.SerialPort(); // make a new instance of  serialport librar
  // serial.on('list', printList); // callback function for serialport list event
  serial.on('data', serialEvent); // callback for new data coming in

  serial.list(); // list the serial ports
  serial.open("/dev/cu.usbmodem1411"); // open a port
  //SET UP IR RECEIVER ON SEPARATE ARDUINO
}




function serialEvent() {
  // hayk code
  	let rawData = trim(serial.readLine());
	if (rawData.length > 0) {
    //TODO: set limiter so that note is only played if rawData is N different from previous value or N (500?) milliseconds have passed
		if( rawData < 2500 ) {
      console.log('rawData: ', rawData);
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
}

function convertNote(num, in_min, in_max, out_min, out_max) {
  let convertedNote = (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return Math.round(convertedNote);
  // convert to note along full octave scale (C)  return Math.ceil(convertedNote/12) * 12;
}


