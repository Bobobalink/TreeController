const remoteURL = 'CORSTrampoline/';
const buttons = document.getElementsByClassName('putReqs');
for (let i=0, len=buttons.length|0; i<len; i=i+1|0) {
    let button = buttons[i];
    button.addEventListener('click', async _ => {
        let xhttp = new XMLHttpRequest();
        xhttp.open('PUT', remoteURL + button.id, true);
        xhttp.send();
    });
}

const buttonSend = document.getElementById('setBut');
buttonSend.addEventListener('click', async _ => {
    let xhttp = new XMLHttpRequest();
    xhttp.open('PUT', remoteURL + 'calibrate', true);
    xhttp.send(document.getElementById('setInd').value);
});

const buttonTheater = document.getElementById('theater');
buttonTheater.addEventListener('click', async _ => {
    let xhttp = new XMLHttpRequest();
    xhttp.open('PUT', 'calibChase', true);
    xhttp.send();
})
