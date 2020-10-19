const _ = require('lodash');
const fs = require('fs');
const nets = require('../visualisation/libraries/example-nets').nets;
const netNames = ['3\.Basic-TL', '8\.Combined-TL', '2\.Example-net-2', '10\.Combined-Multiple-Layers'];

const firstNet = _.cloneDeep(nets[netNames[0]]);

netNames.shift();
const netsToJoin = netNames.map(netName => _.cloneDeep(nets[netName]));

const pPlus = [];
const tLowPlus = [];
const tHighPlus = [];
pPlus[0] = Number(firstNet.places[firstNet.places.length - 1].substr(1)) - 1;
tLowPlus[0] = Number(firstNet.transitions.low[firstNet.transitions.low.length - 1].substring(1));
tHighPlus[0] = Number(firstNet.transitions.high[firstNet.transitions.high.length - 1].substring(1));

netsToJoin.forEach(net => {
    pPlus.push(Number(net.places[net.places.length - 1].substr(1)) - 1);
    tLowPlus.push(Number(net.transitions.low[net.transitions.low.length - 1].substring(1)));
    tHighPlus.push(Number(net.transitions.high[net.transitions.high.length - 1].substring(1)));''
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

fs.writeFile('./output.json', JSON.stringify(firstNet, undefined, 4), err => {});




