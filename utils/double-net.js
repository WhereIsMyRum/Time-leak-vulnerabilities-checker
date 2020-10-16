const _ = require('lodash');
const fs = require('fs');
const nets = require('../visualisation/libraries/example-nets').nets
const netName = '21\.Massive-x16';

const net = _.cloneDeep(nets[netName]);
const pPlus = Number(net.places[net.places.length - 1].substr(1)) - 1;

let first = true;
net.places.forEach(place => {
    if (!first) net.places.push(`p${Number(place.substring(1))+pPlus}`);
    first = false;
});

const pLowPlus = Number(net.transitions.low[net.transitions.low.length -1].substring(1));
net.transitions.low.forEach(transition => {
    net.transitions.low.push(`L${Number(transition.substring(1)) + pLowPlus}`);
});

const pHighPlus = Number(net.transitions.high[net.transitions.high.length -1].substring(1));
net.transitions.high.forEach(transition => {
    net.transitions.high.push(`H${Number(transition.substring(1)) + pHighPlus}`)
})

Object.entries(nets[netName].flows.places).forEach(([key, val]) => {
    val = val.map(v => {
        if (v.charAt(0) == 'L') return `L${Number(v.substring(1)) + pLowPlus}`;
        return `H${Number(v.substring(1)) + pHighPlus}`;
    })
    net.flows.places[`p${Number(key.substring(1)) + pPlus}`] = val;
})

Object.entries(nets[netName].flows.transitions).forEach(([key, val]) => {
    val = val.map(v => `p${Number(v.substring(1)) + pPlus}`);

    const plus = key.charAt(0) == "L" ? pLowPlus : pHighPlus;
    net.flows.transitions[`${key.charAt(0)}${Number(key.substring(1)) + plus}`] = val;
})

fs.writeFile('./output.json', JSON.stringify(net), err => {})