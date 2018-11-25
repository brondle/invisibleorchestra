let serial;
let oldNote = 0;
let synth = new Tone.Synth().toMaster()

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
		if( rawData < 2500 ) {
			console.log(rawData);
			synth.triggerAttackRelease( rawData, '8n');
		}
		
	}	
}

function draw() {

}
