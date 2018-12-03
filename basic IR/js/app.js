let serial2;
let oldNote = 0;
let synth = new Tone.Synth().toMaster();
let fromSerial0 = 0, fromSerial1 = 0, fromSerial2 = 0, fromSerial3 = 0;

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
		
		fromSerial0 = Number(distanceValues[0]);
		fromSerial1 = Number(distanceValues[1]);
		fromSerial2 = Number(distanceValues[2]);
		fromSerial3 = Number(distanceValues[3]);

		
		if( fromSerial1 < 2500 ) {
			synth.volume.value = map(fromSerial1, 1000, 1600, -30, 100);
		}

		if( fromSerial2 < 2500 ) {
			synth.triggerAttackRelease( fromSerial2, '8n');
		}		

		
		/*
		if( fromSerial1 < 2500 ) {
			console.log(fromSerial1);
			synth.triggerAttackRelease( fromSerial1, '8n');
		}

		
		if( fromSerial3 < 2500 ) {
			console.log(fromSerial3);
			synth.triggerAttackRelease( fromSerial3, '8n');
		}		
		*/


	}

}

function draw() {


}
