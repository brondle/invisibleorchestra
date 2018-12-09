let serial;
let synth = new Tone.Synth().toMaster();
let synth2 = new Tone.Synth().toMaster();
let fromSerial0 = 0, fromSerial1 = 0, fromSerial2 = 0, fromSerial3 = 0;
let oldNote = 0;
let oldNote2 = 0;
let keyMap = {
  1: "C", 2: "D", 3: "E", 4: "F", 5: "G", 6: "A", 7: "B"}
let notes = ["A", "B", "C", "D", "E", "F", "G"];
let octaves =[1, 2, 3, 4, 5, 6];

function setup() {

 serial = new p5.SerialPort(); // make a new instance of  serialport librar
  // serial.on('list', printList); // callback function for serialport list event
  serial.on('data', serialEvent); // callback for new data coming in
  serial.list(); // list the serial ports
  serial.open("COM3"); // open a port
}

function serialEvent() {

	let rawData = trim(serial.readLine());




	if (rawData.length > 0) {
		let distanceValues = split(rawData, ",");
    console.log('distance values: ', distanceValues);

		fromSerial0 = Number(distanceValues[0]);
		fromSerial1 = Number(distanceValues[1]);
		fromSerial2 = Number(distanceValues[2]);
		fromSerial3 = Number(distanceValues[3]);

		  if (fromSerial0 < 2000) {
			  let vol1 = map(fromSerial0, 1000, 1400, -30, 20);
			//      synth2.volume.value = 20;
			if (vol1 < 20) {
			  synth.volume.value = vol1;
			} else {
			  synth.volume.value = 20;
			}

		  }

		if( fromSerial1 < 2000 ) {
      let vol2 = map(fromSerial1, 1000, 3000, -30, 20);
      if (vol2 < 20) {
        synth2.volume.value = vol2;
      } else {
        synth2.volume.value = 20;
      }
      console.log('volume: ', synth.volume.value);
      //synth2.volume.value = 20;
		}

		if( fromSerial2 < 2000 ) {
      let note = convertNote(fromSerial2, 1500, 2200, 1, 7);
        if (oldNote != note) {
          synth.triggerRelease();
          oldNote = note;
          //check out latency hints/performance stuff
          synth.triggerAttack( keyMap[note] + "3");
        }
      }
    serial.clear();



		if( fromSerial3 < 2000 ) {
      // synth2.volume.value = map (fromSerial0, 1000, 2500, 10, 50);
      let note2 = convertNote(fromSerial3, 1000, 1500, 1, 7);
    if (oldNote2 != note2) {
        synth2.triggerRelease();
      oldNote2 = note2;
      synth2.triggerAttack( keyMap[note2] + "4");
    }
    serial.clear();
    }


	}

}

function draw() {


}
function convertNote(num, in_min, in_max, out_min, out_max) {
  let convertedNote = (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return Math.round(convertedNote);
  // convert to note along full octave scale (C)  return Math.ceil(convertedNote/12) * 12;
}
