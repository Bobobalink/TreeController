const runButton = document.getElementById('doCalibrate');

runButton.onclick = runCalibration;

async function runCalibration() {
    // first, capture a picture with all of the LEDs off
    await fetch('CORSTrampoline/off', {
        method: 'POST'
    });
    await capture('allOff');

    // then loop through all the LEDS, taking a picture between each picture
    for(let i = 1; i < 401; i++) {
        await fetch('CORSTrampoline/calibrate', {
            method: 'POST',
            body: i.toString()
        });
        await capture('calib' + i);
    }
}
