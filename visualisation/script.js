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
    const valid = validateData(data.custom);
    if (valid.result) {
        fetch('/analyze', {
            method: "POST",
            body: JSON.stringify(data),
            headers: {
                'Content-Type': 'application/json'
            }
        })
            .then(res => res.json())
            .then(data => reRender(data));
    } else {
        console.log(valid);
        alert(`The net you\'re trying to submit is incorrect:\n ${valid.issues.map(issue => issue).join('\n')}`);
    }
}

function validateData(data) {
    const valid = {};
    let transitionsValues = [];
    let placesValues = [];
    let placesNoInput = [];
    const placesNoOutput = [];

    valid.result = true;
    valid.issues = [];
    Object.entries(data.flows.transitions).forEach(([key, value]) => {
        transitionValues = transitionsValues.concat(value);
    });
    Object.entries(data.flows.transitions).forEach(([key, value]) => {
        transitionsValues = transitionsValues.concat(value);
    });

    Object.entries(data.flows.places).forEach(([key, value]) => {
        placesValues = placesValues.concat(value);
    });

    data.places.forEach(place => {
        if (!data.flows.places[place] || data.flows.places[place].length == 0) {
            placesNoOutput.push(place);
        }
        if (!transitionsValues.includes(place)) {
            placesNoInput.push(place);
        }
    });

    if (placesNoInput.length > 1) {
        valid.result = false;
        valid.issues = valid.issues.concat([`more than 1 place has no input: ${placesNoInput}`]);
    }

    if (placesNoOutput.length > 1) {
        valid.result = false;
        valid.issues = valid.issues.concat([`more than 1 place has no output: ${placesNoOutput}`]);
    }

    if (placesNoInput.length == 0) {
        valid.result = false;
        valid.issues = valid.issues.concat(['No start place']);
    }

    if (placesNoOutput.length == 0) {
        valid.result = false;
        valid.issues = valid.issues.concat(['No end place']);
    }

    placesNoInput.forEach(place => {
        if (placesNoOutput.includes(place)) {
            valid.result = false;
            valid.issues = valid.issues.concat([`${place} has no start and no end transitions`]);
        }
    });

    data.transitions.low.forEach(transition => {
        if (!data.flows.transitions[transition] || data.flows.transitions[transition].length == 0) {
            valid.result = false;
            valid.issues = valid.issues.concat([`${transition} has no output places`]);
        }
        if (!placesValues.includes(transition)) {
            valid.result = false;
            valid.issues = valid.issues.concat([`${transition} has no input places`]);
        }
    });

    data.transitions.high.forEach(transition => {
        if (!data.flows.transitions[transition] || data.flows.transitions[transition].length == 0) {
            valid.result = false;
            valid.issues = valid.issues.concat([`${transition} has no output places`]);
        }
        if (!placesValues.includes(transition)) {
            valid.result = false;
            valid.issues = valid.issues.concat([`${transition} has no input places`]);
        }
    });

    console.log(valid.issues)
    return valid;
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