let myp5;
const netHolder = document.getElementById('net-input');
const invalidJson = document.getElementById('invalid-json-notification');
const defaultNet = { "places": [], "transitions": { "high": [], "low": [] }, "flows": { "places": {}, "transitions": {} } };
let net = defaultNet;
setNetToDefault();


window.onload = () => {
    fetchNetsData();
    populateSavedNets();
    netHolder.addEventListener('keyup', update);
};


function update() {
    try {
        const tmpNet = JSON.parse(netHolder.value);
        invalidJson.style.visibility = "hidden";
        if (netHolder.classList.contains('red-focus')) netHolder.classList.remove('red-focus');
        reRender(tmpNet);
    } catch (e) {
        console.log(e);
        invalidJson.style.visibility = "visible";
        netHolder.classList.add('red-focus');
    }
}

function reRender(tmpNet) {
    net = tmpNet;
    if (myp5) myp5.remove();
    myp5 = new p5(s);
}

function clearForm() {
    if (window.confirm('You sure you want to clear?')) {
        setNetToDefault();
        resetSelectById('nets-select');
        resetSelectById('nets-custom-select');
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

function populateSavedNets() {
    const customSelect = document.getElementById('nets-custom-select');
    Object.entries(localStorage).sort().forEach(([key, value]) => {
        const option = document.createElement('option');
        option.value = key;
        option.text = key;
        customSelect.append(option);
    })
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
    })
        .then(res => res.json())
        .then(data => reRender(data));
}

function addToSelect(data) {
    const select = document.getElementById('nets-select');
    data.forEach(net => {
        const option = document.createElement('option');
        option.value = net.value;
        option.text = net.text;
        select.appendChild(option);
    });
}

function selectPredefinedChange(element) {
    fetch(`/nets/${element.value}`)
        .then(res => res.json())
        .then(data => netHolder.value = JSON.stringify(data, undefined, 4))
        .then(() => update());
    resetSelectById('nets-custom-select');
}

function selectCustomChange(element) {
    netHolder.value = localStorage.getItem(element.value);
    update();
    resetSelectById('nets-select');
}

function resetSelectById(id) {
    const select = document.getElementById(id);
    if (select) select.value = '';
}

function saveNetToLocalStorage() {
    try {
        JSON.parse(netHolder.value);
    } catch (e) {
        alert("The net you want is invalid!");
        return;
    }

    const netName = prompt("Enter your net name:", '');
    if (netName !== null && netName !== "") {
        if (localStorage.getItem(netName)) {
            alert("A net with that name exists! Select a different name.");
            return;
        }
        localStorage.setItem(netName, netHolder.value);
        appendToSavedNets(netName);
    }
}

function appendToSavedNets(netName) {
    const customSelect = document.getElementById('nets-custom-select');
    const option = document.createElement('option');
    option.value = netName;
    option.key = netName;
    option.innerHTML = netName;
    customSelect.append(option);
}

function deleteNetFromLocalStorage() {
    const customSelect = document.getElementById('nets-custom-select');
    if (customSelect.value != '') {
        localStorage.removeItem(customSelect.value);
        customSelect.childNodes.forEach(option => {
            if (option.value == customSelect.value) {
                option.remove();
            }
        })
    }
    setNetToDefault();
    resetSelectById('nets-custom-select');
}

function setNetToDefault() {
    netHolder.value = JSON.stringify(defaultNet, undefined, 4);
    update();
}