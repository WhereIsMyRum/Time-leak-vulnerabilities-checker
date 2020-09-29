let _places, _transitions, _arrows, _start, _end;
const PLUS_X = 100;
const PLUS_Y = 60;
const RADIUS  = 30;
const SIDE = 40;
const ARROW_SIZE = 9;
const BASE_Y = 200
const BASE_X = 200;

function setup() {
    createCanvas(window.outerHeight, window.outerWidth);
    ellipseMode(CENTER);
    textAlign(CENTER, CENTER);
    textFont('Georgia');
};

function draw() {
    background(220);
    coordinates = {x: BASE_X, y: BASE_Y};
    _arrows  = {};

    const net1 = {
        places: ['p1','p2', 'p3'],
        transitions: ['t1','t2','t3', 't4', 't5'],
        flows: {
            places: {
                p1: ['t1'],
                p2: ['t2', 't3', 't4']
            },
            transitions: {
                t1: ['p2'],
                t3: ['p3'],
                t4: ['p3']
            }
        }
    };

    const net2 = {
        places: ['p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8'],
        transitions: ['t1','t2','t3','t4','t5','t6'],
        flows: {
            places: {
                p1: ['t1'],
                p2: ['t2'],
                p3: ['t3'],
                p4: ['t4'],
                p5: ['t5'],
                p6: ['t6'],
                p7: ['t6']
            },
            transitions: {
                t1: ['p2', 'p3'],
                t2: ['p4','p5'],
                t3: ['p7'],
                t4: ['p6'],
                t5: ['p6'],
                t6: ['p8']
            }
        }
    };

    const net3 = {
        places: ['p1','p2','p3','p4','p5'],
        transitions: ['t1', 't2'],
        flows: {
            places: {
                p1: ['t1'],
                p2: ['t2'],
                p3: ['t2'],
                p4: ['t2'],
            },
            transitions: {
                t1: ['p2','p3','p4'],
                t2: ['p5']
            }
        }
    }

    const net = net3;
    [_places, _end] = createPlaces(net);
    [_transitions, _start] = createTransitions(net);

    _places[_start].tokes = 1;
    _places[_start].compute(coordinates);
    drawAll();
};


 class Shape {
    constructor(id, tokens = 0) {
        this.x = 0;
        this.y = 0;
        this.tokens = 0;
        this.id = id;
    };

    checkIfNotOvelapping() {
        while(elementsOverlap({x: this.x, y: this.y}, this.id)) {
            this.y = this.y + PLUS_Y;
        }
    };

    addArcBetweenElements(targetId) {
        if (!_arrows[this.id + targetId]) {
            _arrows[this.id + targetId] = new Arc(this.id, targetId)
        }
    };

    compute(coordinates) {
        if (this.id === _end) {
            this.x = coordinates.x += 2 * PLUS_X
            this.y = BASE_Y;
        } else {
            this.x = coordinates.x;
            this.y = coordinates.y;
        }
        

        this.checkIfNotOvelapping();
    }
};

class Place extends Shape {
    constructor(id, transitions, tokens = 0, radius = RADIUS) {
        super(id, tokens);
        this.radius = radius;
        this.drawn = false;
        transitions ? this.transitions = transitions : this.transition = [];
    };

    compute(coordinates) {
        super.compute(coordinates);

        let plusY = 0;
        if (this.transitions) {
            this.transitions.forEach(transitionId => {
                _transitions[transitionId].compute({x: this.x + PLUS_X, y: this.y - plusY});
                this.addArcBetweenElements(transitionId);

                plusY = plusY > 0 ? plusY * (-1) : (plusY - PLUS_Y) * (-1);
            })
        }
    };

    draw() {
        circle(this.x, this.y, this.radius);
        text(this.id, this.x + RADIUS / 2, this.y - RADIUS / 2);
            /*stroke(255,255,255,1);
            fill(0,0,0,100);
            circle(this.x, this.y, this.radius / 2)
            init()*/

    }
};

class Transitions extends Shape {
    constructor(id, places, tokens = 0, side = SIDE) {
        super(id, tokens);
        this.side = side;
        this.drawn = false;
        places ? this.places = places : this.places = [];
    }

    compute(coordinates) {
        super.compute(coordinates);

        let plusY = 0;
        if (this.places) {
            this.places.forEach(placeId => {
                _places[placeId].compute({x: this.x + PLUS_X, y: this.y + plusY});
                this.addArcBetweenElements(placeId);

                plusY = plusY > 0 ? plusY * (-1) : (plusY - PLUS_Y) * (-1);
            })
        }
    }

    draw() {
        square(this.x - this.side / 2, this.y - this.side / 2, this.side);
        text(this.id, this.x, this.y);
    }
};

class Arc {
    constructor(baseId, targetId) {
        this.startModifier = baseId[0] === 't' ? SIDE / 2 : RADIUS / 2;
        this.endModifier = baseId[0] === 't' ? RADIUS / 2 : SIDE / 2;
        this.base = _transitions[baseId] ? _transitions[baseId] : _places[baseId];
        this.target = _transitions[targetId] ? _transitions[targetId] : _places[targetId];
    }

    draw() {
        push();
        //line = calculateLinePoints();
        line(this.base.x + this.startModifier, this.base.y, this.target.x - this.endModifier, this.target.y);
        const y_translation = this.target.y - BASE_Y;
        translate(this.target.x - ARROW_SIZE, BASE_Y + y_translation);
        this.drawArcHead();
        pop();
    }

    drawArcHead() {
        const angle = this.calculateAngle();
        this.alignArchHeadToLine(angle);
        triangle(0, ARROW_SIZE / 2, 0, -ARROW_SIZE / 2, ARROW_SIZE, 0);
    }

    alignArchHeadToLine(angle) {
        if (this.baseBelowTarget()) {
            translate(-ARROW_SIZE * 3 / (PLUS_Y / 30), ARROW_SIZE * 1.2 / (PLUS_Y / 30));
            rotate(-angle);
        } else if (this.baseAboveTarget()) {
            translate(-ARROW_SIZE * 3 / (PLUS_Y / 30), -ARROW_SIZE * 1.2 / (PLUS_Y / 30));
            rotate(-angle);
        } else {
            if (this.isPlaceBased()) {
                translate(-ARROW_SIZE * 2.2, 0);
            } else {
                translate(-ARROW_SIZE * 1.7, 0);
            }
        }
    }

    calculateAngle() {
        const refVec = createVector(10, 0);
        const primalVec = createVector(this.target.x - this.base.x, this.target.y - this.base.y);
        return primalVec.angleBetween(refVec);
    }

    baseBelowTarget() {
        return this.base.y > this.target.y;
    }

    baseAboveTarget() {
        return this.base.y < this.target.y;
    }

    isPlaceBased() {
        return this.base.id[0] === 'p';
    }

    calculateLinePoints() {
        const line = {};
        if (this.base.y === this.target.y) {

        } else if (this.base.x === this.target.x) {
            if (this.base.y < this.target.y) {

            } else {

            }
        }
    }
}

function createPlaces(net) {
    const places = {};
    let end;

    net.places.forEach(placeId => {
        const place = new Place(placeId, net.flows.places[placeId]);
        if (!net.flows.places[placeId]) end = placeId;

        places[placeId] = place;
    });

    return [places, end];
};

function createTransitions(net) {
    const transitions = {};
    let places = [...net.places];

    net.transitions.forEach(transitionId => {
        const transition = new Transitions(transitionId, net.flows.transitions[transitionId]);
        transitions[transitionId] = transition;

        places = filterPlaces(net, transitionId, places);
    });

    return [transitions, places[0]];
};

function filterPlaces(net, transitionId, places) {
    if (net.flows.transitions[transitionId]) {
        places = places.filter(place => {
            return net.flows.transitions[transitionId].indexOf(place) < 0;
        })
    }

    return places;
};

function elementsOverlap(coordinates, id) {
    let result = false;
    Object.values(_places).forEach(place => {
        if (id != place.id && coordinates.x === place.x && coordinates.y === place.y) {
            result = true;
        }
    });

    Object.values(_transitions).forEach(transition => {
        if (id != transition.id && coordinates.x === transition.x && coordinates.y === transition.y) {
            result = true; 
        }
    });

    return result;
}

function drawAll() {
    drawPlaces();
    drawTransitions();
    drawArcs();
};

function drawArcs() {
    Object.values(_arrows).forEach(arrow => {
        arrow.draw();
    });
}

function drawTransitions() {
    Object.values(_transitions).forEach(transition => {
        transition.draw();
    });
}

function drawPlaces() {
    Object.values(_places).forEach(place => {
        place.draw();
    });
}

