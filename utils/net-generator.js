const { json } = require("body-parser");

let currentMaxPlace = 2
let currentMaxHigh = 0
let currentMaxLow = 1

function generateNet(net = { "places": ["p1", "p2"], "transitions": { "high": [], "low": ["L1"] }, "flows": { "places": {"p1":["L1"]}, "transitions": {"L1":["p2"]} } }, recursionDepth = 0, distributions = [20, 20, 0, 0, 60]) {
    switch (getType(distributions)) {
        case 1:
            returnNet = getBaseLow(net);
            break;
        case 2:
            returnNet = getBaseHigh(net);
            break;
        case 3:
            returnNet = getSequentialComposition(net, recursionDepth + 1, distributions);
            break;
        case 4:
            returnNet = getBranchingComposition(net, recursionDepth + 1, distributions);
            break;
        case 5:
            returnNet = getParallelComposition(net, recursionDepth + 1, distributions);
            break;
    }

    returnNet.places = returnNet.places.filter((item, i, ar) => ar.indexOf(item) === i)
    returnNet.transitions.high = returnNet.transitions.high.filter((item, i, ar) => ar.indexOf(item) === i)
    returnNet.transitions.low = returnNet.transitions.low.filter((item, i, ar) => ar.indexOf(item) === i)

    if (recursionDepth === 0) {
        console.log(JSON.stringify(returnNet));
        return renumberElements([addEndPlace(returnNet)]);
    }
    return returnNet;
}

function getType(distributions) {
    const ranges = getRanges(distributions);
    const random = getRandom();
    for (let i = 0; i < ranges.length; ++i) {
        if (random >= ranges[i][0] && random <= ranges[i][1]) {
            return i + 1;
        }
    }
}

function getRandom() {
    return Math.floor(Math.random() * 100)
}

function getRanges(distributions) {
    const ranges = [];
    for (let i = 0; i < distributions.length; ++i) {
        if (i === 0) {
            ranges.push([0, distributions[i] - 1])
        } else {
            ranges.push([ranges[i - 1][1] + 1, ranges[i - 1][1] + distributions[i]])
        }
    }

    return ranges;
}

function getBaseHigh(net) {
    const netMaxs = getNetMaxs();
    let maxPlace = netMaxs.maxPlace > 0 ? netMaxs.maxPlace - 1 : 0;

    net.places.push(`p${maxPlace + 2}`);
    net.transitions.high.push(`H${netMaxs.maxHigh + 1}`);

    net.flows.places[`p${maxPlace + 1}`] = [`H${netMaxs.maxHigh + 1}`];
    net.flows.transitions[`H${netMaxs.maxHigh + 1}`] = [`p${maxPlace + 2}`];

    currentMaxHigh += 1;
    currentMaxPlace += 1;

    return net
}

function getBaseLow(net) {
    const netMaxs = getNetMaxs()
    let maxPlace = netMaxs.maxPlace > 0 ? netMaxs.maxPlace - 1 : 0;

    net.places.push(`p${maxPlace + 2}`);
    net.transitions.low.push(`L${netMaxs.maxLow + 1}`);

    net.flows.places[`p${maxPlace + 1}`] = [`L${netMaxs.maxLow + 1}`];
    net.flows.transitions[`L${netMaxs.maxLow + 1}`] = [`p${maxPlace + 2}`];


    currentMaxLow += 1;
    currentMaxPlace += 1;

    return net
}

function getSequentialComposition(net, recursionDepth, distribution) {
    if (recursionDepth > 10) {
        distribution = [50, 50, 0, 0, 0];
    }
    let netOutput = generateNet(net, recursionDepth, distribution);
    netOutput = generateNet(netOutput, recursionDepth, distribution);

    return netOutput;
}


function getBranchingComposition(baseNet, recursionDepth, distributions) {
    if (recursionDepth > 5) {
        distributions = [50, 50, 0, 0, 0]
    }

    const branchNet1 = generateNetBasedOnLimits(getNetMaxs(), recursionDepth, distributions);
    const branchNet2 = generateNetBasedOnLimits(getNetMaxs(), recursionDepth, distributions);

    return joinBranches(baseNet, branchNet1, branchNet2);
}

function getParallelComposition(baseNet, recursionDepth, distributions) {
    if (recursionDepth > 2) {
        distributions = [50, 50, 0, 0, 0]
    }

    const parallelNet1 = generateNetBasedOnLimits(getNetMaxs(), recursionDepth, distributions);
    const parallelNet2 = generateNetBasedOnLimits(getNetMaxs(), recursionDepth, distributions);

    return joinParallel(baseNet, parallelNet1, parallelNet2);
}

function getNetMaxs() {
    const netMaxs = {};

    netMaxs.maxPlace = currentMaxPlace;
    netMaxs.maxHigh = currentMaxHigh;
    netMaxs.maxLow = currentMaxLow;

    return netMaxs;
}

function generateNetBasedOnLimits(netMaxs, recursionDepth, distributions) {

    const highTransitionsMax = netMaxs.maxHigh != 0 ? [`H${netMaxs.maxHigh}`] : []
    const lowTransitionsMax = netMaxs.maxLow != 0 ? [`L${netMaxs.maxLow}`] : []
    let baseNet = { "places": [`p${netMaxs.maxPlace}`], "transitions": { "high": highTransitionsMax, "low": lowTransitionsMax }, "flows": { "places": {}, "transitions": {} } };

    baseNet = generateNet(baseNet, recursionDepth, distributions);

    if (netMaxs.maxHigh  > 0) baseNet.transitions.high.shift()
    if (netMaxs.maxLow > 0) baseNet.transitions.low.shift();

    return baseNet;
}

function printNet(net) {
    console.log(net.places);
    console.log(net.transitions)
    console.log(net.flows.places)
    console.log(net.flows.transitions)
}

function joinBranches(baseNet, branch1, branch2) {
    const startPlace = branch1.places.shift();
    const secondBranchStart = branch2.places.shift();

    [branch1, branch2] = makeCommonEndPlace(branch1, branch2, baseNet)
    branch1.flows.places[startPlace] = branch1.flows.places[startPlace].concat(branch2.flows.places[secondBranchStart]);
    delete branch2.flows.places[secondBranchStart];

    baseNet.places = baseNet.places.concat(branch1.places, branch2.places)
    baseNet.transitions.high = baseNet.transitions.high.concat(branch1.transitions.high, branch2.transitions.high)
    baseNet.transitions.low = baseNet.transitions.low.concat(branch1.transitions.low, branch2.transitions.low)

    Object.keys(branch1.flows.places).forEach(key => {
        if (!baseNet.flows.places[key]) baseNet.flows.places[key] = [];
        baseNet.flows.places[key] = baseNet.flows.places[key].concat(branch1.flows.places[key]);
    })

    Object.keys(branch2.flows.places).forEach(key => {
        if (!baseNet.flows.places[key]) baseNet.flows.places[key] = [];
        baseNet.flows.places[key] = baseNet.flows.places[key].concat(branch2.flows.places[key]);
    })

    Object.keys(branch1.flows.transitions).forEach(key => {
        if (!baseNet.flows.transitions[key]) baseNet.flows.transitions[key] = []
        baseNet.flows.transitions[key] = baseNet.flows.transitions[key].concat(branch1.flows.transitions[key])
    })

    Object.keys(branch2.flows.transitions).forEach(key => {
        if (!baseNet.flows.transitions[key]) baseNet.flows.transitions[key] = []
        baseNet.flows.transitions[key] = baseNet.flows.transitions[key].concat(branch2.flows.transitions[key])
    })

    return baseNet;
}

function joinParallel(baseNet, parallel1, parallel2) {
    let netMaxs = getNetMaxs();
    const parallel1StartPlace = parallel1.places.shift();
    const parallel2StartPlace = parallel2.places.shift();
    let newStartTransition;

    if (getType([50,50,0,0,0] === 1)) {
        newStartTransition = `L${netMaxs.maxLow + 1}`;
        parallel1.transitions.low.push(newStartTransition);
        currentMaxLow += 1;
    } else {
        newStartTransition = `H${netMaxs.maxHigh + 1}`;
        parallel1.transitions.high.push(newStartTransition);
        currentMaxHigh =+ 1;
    }

    let newPlace = `p${netMaxs.maxPlace + 1}`;
    currentMaxPlace += 1
    parallel1.places.push(newPlace);

    parallel1.flows.places[newPlace] = parallel1.flows.places[parallel1StartPlace];
    parallel1.flows.places[parallel1StartPlace] = [newStartTransition];
    parallel1.flows.transitions[newStartTransition] = [newPlace];

    newPlace = `p${netMaxs.maxPlace + 2}`;
    currentMaxPlace += 1
    parallel2.places.push(newPlace);
    parallel2.flows.places[newPlace] = parallel2.flows.places[parallel2StartPlace]
    delete parallel2.flows.places[parallel2StartPlace];
    parallel2.flows.transitions[newStartTransition] = [newPlace];

    const parallel1EndPlace = findEndPlace(parallel1);
    const parallel2EndPlace = findEndPlace(parallel2);

    netMaxs = getNetMaxs();
    let newTransition;
    if (getType([50, 50, 0, 0, 0]) == 1) {
        newTransition = `L${netMaxs.maxLow + 1}`;
        parallel1.transitions.low.push(newTransition);
        currentMaxLow += 1;
    } {
        newTransition = `H${netMaxs.maxHigh + 1}`;
        parallel1.transitions.high.push(newTransition);
        currentMaxHigh += 1;
    }
    parallel1.flows.places[parallel1EndPlace] = [newTransition];
    parallel2.flows.places[parallel2EndPlace] = [newTransition];
    parallel1.places.push(`p${netMaxs.maxPlace + 1}`);
    currentMaxPlace += 1;
    parallel1.flows.transitions[newTransition] = [`p${netMaxs.maxPlace + 1}`];

    baseNet.places = baseNet.places.concat(parallel1.places, parallel2.places)
    baseNet.transitions.high = baseNet.transitions.high.concat(parallel1.transitions.high, parallel2.transitions.high)
    baseNet.transitions.low = baseNet.transitions.low.concat(parallel1.transitions.low, parallel2.transitions.low)

    Object.keys(parallel1.flows.places).forEach(key => {
        if (!baseNet.flows.places[key]) baseNet.flows.places[key] = [];
        baseNet.flows.places[key] = baseNet.flows.places[key].concat(parallel1.flows.places[key]);
    })

    Object.keys(parallel2.flows.places).forEach(key => {
        if (!baseNet.flows.places[key]) baseNet.flows.places[key] = [];
        baseNet.flows.places[key] = baseNet.flows.places[key].concat(parallel2.flows.places[key]);
    })

    Object.keys(parallel1.flows.transitions).forEach(key => {
        if (!baseNet.flows.transitions[key]) baseNet.flows.transitions[key] = []
        baseNet.flows.transitions[key] = baseNet.flows.transitions[key].concat(parallel1.flows.transitions[key])
    })

    Object.keys(parallel2.flows.transitions).forEach(key => {
        if (!baseNet.flows.transitions[key]) baseNet.flows.transitions[key] = []
        baseNet.flows.transitions[key] = baseNet.flows.transitions[key].concat(parallel2.flows.transitions[key])
    })

    return baseNet;
}

function makeCommonEndPlace(branch1, branch2) {
    const b1EndPlace = findEndPlace(branch1)
    const b2EndPlace = findEndPlace(branch2)
    let maxNets = getNetMaxs();
    
    if (getType([50,50,0,0,0]) === 1) {
        branch1.transitions.low.push(`L${maxNets.maxLow + 1}`)
        branch1.flows.places[b1EndPlace] = [`L${maxNets.maxLow + 1}`];
        branch1.flows.transitions[`L${maxNets.maxLow + 1}`] = [`p${maxNets.maxPlace + 1}`]
        currentMaxLow += 1;
    } else {
        branch1.transitions.high.push(`H${maxNets.maxHigh + 1}`)
        branch1.flows.places[b1EndPlace] = [`H${maxNets.maxHigh + 1}`];
        branch1.flows.transitions[`H${maxNets.maxHigh + 1}`] = [`p${maxNets.maxPlace + 1}`]
        currentMaxHigh += 1;
    }

    branch1.places.push(`p${maxNets.maxPlace + 1}`);
    currentMaxPlace += 1;
    maxNets = getNetMaxs();

    if (getType([50,50,0,0,0]) === 1) {
        branch2.transitions.low.push(`L${maxNets.maxLow + 1}`)
        branch2.flows.places[b2EndPlace] = [`L${maxNets.maxLow + 1}`];
        branch2.flows.transitions[`L${maxNets.maxLow + 1}`] = [`p${maxNets.maxPlace}`]
        currentMaxLow += 1;
    } else {
        branch2.transitions.high.push(`H${maxNets.maxHigh + 1}`)
        branch2.flows.places[b2EndPlace] = [`H${maxNets.maxHigh + 1}`];
        branch2.flows.transitions[`H${maxNets.maxHigh + 1}`] = [`p${maxNets.maxPlace}`]
        currentMaxHigh += 1;
    }

    return [branch1, branch2];
}

function findEndPlace(net) {
    let pId;
    net.places.forEach(place => {
        if (!net.flows.places[place]) {
            pId = place;
        }
    })

    return pId;
}

function addEndPlace(net) {
    const endPlace = findEndPlace(net);
    const netMaxs = getNetMaxs();

    if (getType([50,50,0,0,0]) === 1) {
        net.transitions.low.push(`L${netMaxs.maxLow + 1}`)
        net.flows.places[endPlace] = [`L${netMaxs.maxLow + 1}`]
        net.flows.transitions[`L${netMaxs.maxLow + 1}`] = [`p${netMaxs.maxPlace + 1}`];
    } else {
        net.transitions.high.push(`H${netMaxs.maxHigh + 1}`)
        net.flows.places[endPlace] = [`H${netMaxs.maxHigh + 1}`]
        net.flows.transitions[`H${netMaxs.maxHigh + 1}`] = [`p${netMaxs.maxPlace + 1}`];
    }

    net.places.push(`p${netMaxs.maxPlace + 1}`);

    return net;
}


module.exports = generateNet;