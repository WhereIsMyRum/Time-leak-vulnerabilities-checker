const _ = require('lodash');
const fs = require('fs');
const generateNet = require('./net-generator');
//const nets = require('../visualisation/libraries/example-nets').nets;
//const netNames = ['3\.Basic-TL', '8\.Combined-TL', '2\.Example-net-2', '10\.Combined-Multiple-Layers'];


const firstNet = JSON.parse(require(`${process.argv[2]}`));

const netsToJoin = generateNet();//process.argv.slice(3, process.argv.length).map(net => JSON.parse(require(`${net}`)));
const numberOfElements = firstNet.places.length + netsToJoin[0].places.length + firstNet.transitions.high.length + firstNet.transitions.low.length + netsToJoin[0].transitions.high.length + netsToJoin[0].transitions.low.length - 1;

const pPlus = [];
const tLowPlus = [];
const tHighPlus = [];
pPlus[0] = Number(firstNet.places[firstNet.places.length - 1].substr(1)) - 1;
tLowPlus[0] = firstNet.transitions.low.length > 0 ? Number(firstNet.transitions.low[firstNet.transitions.low.length - 1].substring(1)) : 0
tHighPlus[0] = firstNet.transitions.high.length > 0 ? Number(firstNet.transitions.high[firstNet.transitions.high.length - 1].substring(1)) : 0;

netsToJoin.forEach(net => {
    pPlus.push(Number(net.places[net.places.length - 1].substr(1)) - 1);
    tLowPlus.push(net.transitions.low.length > 0 ? Number(net.transitions.low[net.transitions.low.length - 1].substring(1)) : 0);
    tHighPlus.push(net.transitions.high.length > 0 ? Number(net.transitions.high[net.transitions.high.length - 1].substring(1)) : 0);
})


let first = true;
let i = 0;
let Pplus = pPlus[0]
let Lplus = tLowPlus[0];
let Hplus = tHighPlus[0];

netsToJoin.forEach(net => {
    net.places.forEach(place => {
        if (!first) firstNet.places.push(`p${Number(place.substring(1)) + Pplus}`);
        first = false;
    });

    net.transitions.low.forEach(transition => {
        firstNet.transitions.low.push(`L${Number(transition.substring(1)) + Lplus}`);
    });


    net.transitions.high.forEach(transition => {
         firstNet.transitions.high.push(`H${Number(transition.substring(1)) + Hplus}`);
    });


    Object.entries(net.flows.places).forEach(([key, val]) => {
        val = val.map(v => {
            if (v.charAt(0) == 'L') return `L${Number(v.substring(1)) + Lplus}`;
            return `H${Number(v.substring(1)) + Hplus}`;
        });
        firstNet.flows.places[`p${Number(key.substring(1)) + Pplus}`] = val;
    });

    Object.entries(net.flows.transitions).forEach(([key, val]) => {
        val = val.map(v => `p${Number(v.substring(1)) + Pplus}`);

        const plus = key.charAt(0) == "L" ? Lplus : Hplus;
        firstNet.flows.transitions[`${key.charAt(0)}${Number(key.substring(1)) + plus}`] = val;
    })

    ++i;
    Pplus += pPlus[i];
    Lplus += tLowPlus[i];
    Hplus += tHighPlus[i];
    first = true;
});

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

    return valid
}
const res = validateData(firstNet)
if (!res.result) console.log(res.issues)

fs.writeFile('./output-run.json', JSON.stringify(firstNet), err => { })
fs.writeFile('./output.json', JSON.stringify(JSON.stringify(firstNet)), err => { });
//console.log(numberOfElements);
console.log(numberOfElements)




