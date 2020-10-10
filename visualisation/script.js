let myp5 = new p5(s);
const netHolder = document.getElementById('net-input');
const invalidJson = document.getElementById('invalid-json-notification');
const defaultNet = { "places": [], "transitions": { "high": [], "low": [] }, "flows": { "places": {}, "transitions": {} } };
let net = defaultNet;
netHolder.innerHTML = JSON.stringify(net, undefined, 4);


window.onload = () => {
    fetchNetsData();
    netHolder.addEventListener('keyup', update);
};


function update() {
    try {
        const tmpNet = JSON.parse(netHolder.value);
        invalidJson.style.visibility = "hidden";
        if (netHolder.classList.contains('red-focus')) netHolder.classList.remove('red-focus');
        net = tmpNet;
        myp5 = new p5(s);
    } catch (e) {
        invalidJson.style.visibility = "visible";
        netHolder.classList.add('red-focus');
    }
}

function clearForm() {
    if (window.confirm('You sure you want to clear?')) {
        netHolder.value = JSON.stringify(defaultNet, undefined, 4);
        update();
    } else {
        update();
    }
}

function fetchNetsData() {
    fetch('/nets', {
        method: "GET"
    })
        .then(res => res.json())
        .then(data => addToSelect(data));
}

function analyze() {
    const data = {
        "custom": JSON.parse(netHolder.value)
    }
    fetch('/analyze', {
        method: "POST",
        body: JSON.stringify(data),
        headers: {
            'Content-Type': 'application/json'
        }
    }).then(res => {
    })
}

function addToSelect(data) {
    const select = document.getElementById('nets-select');
    data.forEach( net => {
        const option = document.createElement('option');
        option.value = net.value;
        option.text = net.text;
        select.appendChild(option);
    });
}

function selectChange(element) {
    fetch(`/nets/${element.value}`)
    .then(res => res.json())
    .then(data => netHolder.value = JSON.stringify(data, undefined, 4))
    .then(() => update());
}