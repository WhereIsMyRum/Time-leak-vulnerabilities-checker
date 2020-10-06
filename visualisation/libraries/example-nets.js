const net1 = {
    places: ["p1", "p2", "p3", "p4"],
    transitions: ["t1", "t2", "t3", "t4", "t5"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2", "t3", "t4"],
            p3: ["t5"]
        },
        transitions: {
            t1: ["p2"],
            t2: ["p3"],
            t3: ["p3"],
            t4: ["p3"],
            t5: ["p4"]
        }
    },
};
const net2 = {
    places: ["p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8"],
    transitions: ["t1", "t2", "t3", "t4", "t5", "t6"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2"],
            p3: ["t3"],
            p4: ["t4"],
            p5: ["t5"],
            p6: ["t6"],
            p7: ["t6"]
        },
        transitions: {
            t1: ["p2", "p3"],
            t2: ["p4", "p5"],
            t3: ["p7"],
            t4: ["p6"],
            t5: ["p6"],
            t6: ["p8"]
        }
    },
};
const net3 = {
    places: ["p1", "p2", "p3", "p4", "p5"],
    transitions: ["t1", "t2"],
    flows: {
        places: {
            p1: ["t1"],
            p2: ["t2"],
            p3: ["t2"],
            p4: ["t2"]
        },
        transitions: {
            t1: ["p2", "p3", "p4"],
            t2: ["p5"]
        }
    }
};

const net4 = {
    "places": ["p1", "p2", "p3", "p4"],
    "transitions": ["t1", "t2", "t3", "t4", "t5"],
    "flows": {
        "places": {
            "p1": ["t1"],
            "p2": ["t2", "t3", "t4"],
            "p3": ["t5"]
        },
        "transitions": {
            "t1": ["p2"],
            "t2": ["p3"],
            "t3": ["p3"],
            "t4": ["p3"],
            "t5": ["p4", "p2"]
        }
    }
};

const basicTL = {
    "places": ["p1", "p2", "p3", "p4"],
    "transitions": ["H1", "L1", "L2"],
    "flows": {
        "places": {
            "p1": ["L1"],
            "p2": ["H1"],
            "p3": ["L2"]
        },
        "transitions": {
            "H1": ["p3"],
            "L1": ["p2"],
            "L2": ["p4"]
        }
    }
};

const parallelTL = {
    "places": ["p1", "p2", "p3", "p4", "p5", "p6"],
    "transitions": ["L1", "L2", "H1", "L3"],
    "flows": {
        "places": {
            "p1": ["L1"],
            "p2": ["H1"],
            "p3": ["L3"],
            "p4": ["L2"],
            "p5": ["L3"]
        },
        "transitions": {
            "L1": ["p2", "p4"],
            "L2": ["p5"],
            "H1": ["p3"],
            "L3": ["p6"]
        }
    }
}

const obfuscatedEnd = {
    "places": ["p1", "p2", "p3", "p4"],
    "transitions": ["L1", "H1", "H2"],
    "flows": {
        "places": {
            "p1": ["L1"],
            "p2": ["H1"],
            "p3": ["H2"]
        },
        "transitions": {
            "H1": ["p3"],
            "L1": ["p2"],
            "H2": ["p4"]
        }
    }
}

const obfuscatedStart = {
    "places": ["p1", "p2", "p3", "p4"],
    "transitions": ["L1", "H1", "H2"],
    "flows": {
        "places": {
            "p1": ["H1"],
            "p2": ["H2"],
            "p3": ["L1"]
        },
        "transitions": {
            "H1": ["p2"],
            "H2": ["p3"],
            "L1": ["p4"]
        }
    }
}

const combinedTL = {
    "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7"],
    "transitions": ["L1", "L2", "L3", "L4", "H1", "H2"],
    flows: {
        "places": {
            "p1": ["L1", "L2"],
            "p2": ["H1"],
            "p3": ["H2"],
            "p4": ["H2"],
            "p5": ["L3"],
            "p6": ["L4"]
        },
        "transitions": {
            "L1": ["p2"],
            "L2": ["p3"],
            "L3": ["p7"],
            "L4": ["p7"],
            "H1": ["p4", "p5"],
            "H2": ["p6"]
        }
    }
}