const videoElement = document.querySelector('video');
const imgElement = document.getElementById("imageCap");
const audioSelect = document.querySelector('select#audioSource');
const videoSelect = document.querySelector('select#videoSource');
const captureButton = document.getElementById("capPic");
const settingsDiv = document.getElementById("cameraSettings");

const settingsBlacklist = new Set();
settingsBlacklist.add('aspectRatio');
settingsBlacklist.add('width');
settingsBlacklist.add('height');

let mediaStreamTrack;
let imageCapture;

audioSelect.onchange = getStream;
videoSelect.onchange = getStream;

getStream().then(getDevices).then(gotDevices);

function getDevices() {
  // AFAICT in Safari this only gets default devices until gUM is called :/
  return navigator.mediaDevices.enumerateDevices();
}

function gotDevices(deviceInfos) {
  window.deviceInfos = deviceInfos; // make available to console
  console.log('Available input and output devices:', deviceInfos);
  for (const deviceInfo of deviceInfos) {
    const option = document.createElement('option');
    option.value = deviceInfo.deviceId;
    if (deviceInfo.kind === 'audioinput') {
      option.text = deviceInfo.label || `Microphone ${audioSelect.length + 1}`;
      audioSelect.appendChild(option);
    } else if (deviceInfo.kind === 'videoinput') {
      option.text = deviceInfo.label || `Camera ${videoSelect.length + 1}`;
      videoSelect.appendChild(option);
    }
  }
}

function getStream() {
  if (window.stream) {
    window.stream.getTracks().forEach(track => {
      track.stop();
    });
  }
  const audioSource = audioSelect.value;
  const videoSource = videoSelect.value;
  const constraints = {
    audio: {deviceId: audioSource ? {exact: audioSource} : undefined},
    video: {deviceId: videoSource ? {exact: videoSource} : undefined}
  };
  return navigator.mediaDevices.getUserMedia(constraints).
    then(gotStream).catch(handleError);
}

function gotStream(stream) {
  window.stream = stream; // make stream available to console
  audioSelect.selectedIndex = [...audioSelect.options].
    findIndex(option => option.text === stream.getAudioTracks()[0].label);
  videoSelect.selectedIndex = [...videoSelect.options].
    findIndex(option => option.text === stream.getVideoTracks()[0].label);
  videoElement.srcObject = stream;
  mediaStreamTrack = stream.getVideoTracks()[0];
  imageCapture = new ImageCapture(mediaStreamTrack);
  console.log(imageCapture);
  makeSettings(mediaStreamTrack.getCapabilities(), mediaStreamTrack.getSettings());
}

function handleError(error) {
  console.error('Error: ', error);
}

captureButton.addEventListener('click', () => {capture('manualCap');});

async function capture(fname) {
    console.log('capturing image');
    let tryCapture = async () => {
        await imageCapture.takePhoto()
            .then(blob => {
                imgElement.src = URL.createObjectURL(blob);
                imgElement.onload = () => {
                    URL.revokeObjectURL(this.src);
                }
                let xhttp = new XMLHttpRequest();
                xhttp.open('POST', 'saveImage/' + fname, false);
                xhttp.send(blob);
            })
            .catch(async error => {
                console.error('takePhoto() error:', error);
                await sleep(100);
                await tryCapture();
            });
    };
    await tryCapture();
}

function putSettings() {
    console.log('putting image settings');
    let out = {};
    out.vidCap = mediaStreamTrack.getCapabilities();
    out.vidSet = mediaStreamTrack.getSettings();
    imageCapture.getPhotoCapabilities().then((val) => {out.photoCap = val}).then(() => {
    imageCapture.getPhotoSettings().then((val) => {out.photoSet = val}).then(() => {
        let xhttp = new XMLHttpRequest();
        xhttp.open('POST', 'putBlob', true);
        xhttp.setRequestHeader("Content-type", "application/json")
        xhttp.send(JSON.stringify(out));
    })});
}

function makeSettings(capabilities, settings) {
    settingsDiv.innerText = ''; // clear all the old settings
    for(const name in capabilities) {
        if(!capabilities.hasOwnProperty(name))
            continue;
        if(settingsBlacklist.has(name)) // skip dangerous settings so I don't accidentally touch them
            continue;
        const limits = capabilities[name];
        if(typeof limits === 'string') {
            // skip it because it's some weird config id thingy
        } else if(Array.isArray(limits)) {
            // arrays give options, so make a dropdown
            let nameLabel = document.createElement('label');
            nameLabel.for = 'select_' + name;
            nameLabel.innerText = name;

            let selectItem = document.createElement("select");
            selectItem.id = 'select_' + name;
            if(limits.length === 1) // dont display options with only one choice
                continue;
            for(const i in limits) {
                if(!limits.hasOwnProperty(i))
                    continue;
                let option = document.createElement("option");
                option.value = limits[i];
                option.innerText = limits[i];
                if(limits[i] === settings[name]) {
                    option.selected = true;
                }
                selectItem.appendChild(option);
            }

            selectItem.onchange = () => {
                let constraint = {};
                constraint[name] = selectItem.options[selectItem.selectedIndex].value;
                mediaStreamTrack.applyConstraints({ advanced: [constraint]}).catch(error => console.error('applyConstraints() error:', error));
            };

            settingsDiv.appendChild(nameLabel);
            settingsDiv.appendChild(selectItem);
            settingsDiv.appendChild(document.createElement('br'));
        } else {
            // it's probably an object with a min and max for a numerical value
            if(limits.hasOwnProperty('max') && limits.hasOwnProperty('min')) {
                //create a slider doodad for a numerical value
                let contDiv = document.createElement('div');
                contDiv.className = 'sliderContainer';
                let nameLabel = document.createElement('label');
                nameLabel.innerText = name;
                nameLabel.for = 'select_' + name;
                nameLabel.className = 'nameLabel';

                let lowLabel = document.createElement('span');
                lowLabel.innerText = (+limits['min'].toFixed(2)).toString();
                lowLabel.className = 'rangeLabel';

                let slider = document.createElement('input');
                slider.id = 'select_' + name;
                slider.type = 'range';
                slider.min = limits['min'];
                slider.max = limits['max'];
                slider.value = settings[name];
                slider.className = 'slider';
                if(limits.hasOwnProperty('step')) {
                    slider.step = limits['step'];
                } else { // default to a step of 1
                    slider.step = '1';
                }

                let highLabel = document.createElement('span');
                highLabel.innerText = (+limits['max'].toFixed(2)).toString();
                highLabel.className = 'rangeLabel';

                let curLabel = document.createElement('span');
                curLabel.innerText = (+settings[name].toFixed(2)).toString();
                curLabel.id = 'valueDisplay_' + name;
                curLabel.className = 'valueDisplay';


                slider.oninput = (event) => {
                    curLabel.innerText = (+slider.valueAsNumber.toFixed(2)).toString();
                    let constraint = {};
                    constraint[name] = slider.valueAsNumber;
                    mediaStreamTrack.applyConstraints({ advanced: [constraint]}).catch(error => console.error('applyConstraints() error:', error));
                };

                contDiv.appendChild(nameLabel);
                contDiv.appendChild(lowLabel);
                contDiv.appendChild(slider);
                contDiv.appendChild(highLabel);
                contDiv.appendChild(curLabel);
                //contDiv.appendChild(document.createElement('br'));
                settingsDiv.appendChild(contDiv);
            }
        }
    }
}
