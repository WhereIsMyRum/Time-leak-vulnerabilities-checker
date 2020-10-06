let _places, _transitions, _arrows, _start, _end, _mouseStartX, _mouseStartY, _draggedElement, _computed = false;


function setup() {
    createCanvas(windowWidth, windowHeight);
    ellipseMode(CENTER);
    textAlign(CENTER, CENTER);
    textFont('Georgia');
    noLoop();
};

function draw() {
    background(255);

    if (!_computed) {
        const net = obfuscatedStart;
        [_places, _end] = createPlaces(net);
        [_transitions, _start] = createTransitions(net);
        _arrows = {};

        _places[_start].tokens = 1;
        _places[_start].compute({ x: BASE_X, y: BASE_Y });
        _computed = true;
    }

    drawAll();
};


class Shape {
    constructor(id, dimension, tokens = 0) {
        this.x = 0;
        this.y = 0;
        this.tokens = 0;
        this.id = id;
        this.computed = false;
        this.dimension = dimension;
        this.colorChange = false;
    };

    checkIfNotOvelapping() {
        while (elementsOverlap({ x: this.x, y: this.y }, this.id)) {
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
            if (this.x >= windowWidth || this.y >= windowHeight) adjustCanvas(this.x, this.y);
        } else {
            this.x = coordinates.x;
            this.y = coordinates.y;
        }

        this.checkIfNotOvelapping();
    };

    isMouseOver() {
        if (mouseX >= this.x - HALF_SIDE && mouseX <= this.x + HALF_SIDE) {
            if (mouseY >= this.y - HALF_SIDE && mouseY <= this.y + HALF_SIDE) {
                return true;
            }
        }
        return false;
    }

    set_colorChange(value) {
        if (this.colorChange !== value) {
            this.colorChange = value;
            redraw();
        }
    }

    checkForColorChange() {
        if (this.isMouseOver()) {
            this.set_colorChange(true);
        } else if (this.colorChange) {
            this.set_colorChange(false);
            cursor('default');
        }
    }

    draw(callback, args, argsText) {
        if (this.colorChange) {
            push()
            stroke('blue');
            callback(...args);
            cursor('pointer')
            pop();
        } else {
            callback(...args);
        }
        text(...argsText)
    }
};

class Place extends Shape {
    constructor(id, transitions, tokens = 0, dimension = RADIUS) {
        super(id, dimension, tokens);
        transitions ? this.transitions = transitions : this.transition = [];
    };

    compute(coordinates) {
        super.compute(coordinates);
        let plusY = 0;
        if (this.transitions) {
            this.transitions.forEach(transitionId => {
                if (!_transitions[transitionId].computed) {
                    _transitions[transitionId].computed = true;
                    _transitions[transitionId].compute({ x: this.x + PLUS_X, y: this.y - plusY });
                }
                this.addArcBetweenElements(transitionId);

                plusY = plusY > 0 ? plusY * (-1) : (plusY - PLUS_Y) * (-1);
            })
        }
    };
};

class Transitions extends Shape {
    constructor(id, places, tokens = 0, dimension = SIDE) {
        super(id, dimension, tokens);
        places ? this.places = places : this.places = [];
    };

    compute(coordinates) {
        super.compute(coordinates);
        let plusY = 0;
        if (this.places) {
            this.places.forEach(placeId => {
                if (!_places[placeId].computed) {
                    _places[placeId].computed = true;
                    _places[placeId].compute({ x: this.x + PLUS_X, y: this.y + plusY });
                }
                this.addArcBetweenElements(placeId);

                plusY = plusY > 0 ? plusY * (-1) : (plusY - PLUS_Y) * (-1);
            })
        }
    };
};

class Arc {
    constructor(baseId, targetId) {
        this.startModifier = baseId[0].match('/t|l|h|T|L|H') ? HALF_SIDE : HALF_RADIUS;
        this.endModifier = baseId[0].match('/t|l|h|T|L|H') ? HALF_RADIUS : HALF_SIDE;
        this.base = _transitions[baseId] ? _transitions[baseId] : _places[baseId];
        this.target = _transitions[targetId] ? _transitions[targetId] : _places[targetId];
    };

    draw() {
        this.calculateSnapPoints();
        line(this.basePoint.x, this.basePoint.y, this.targetPoint.x, this.targetPoint.y);
        this.drawArcHead();
    };

    drawArcHead() {
        push();
        fill('black');
        const points = this.getArrowHeadPoints();
        triangle(points.p1.x, points.p1.y, points.p2.x, points.p2.y, points.p3.x, points.p3.y);
        pop();
    };

    isPlaceBased() {
        return this.base.id[0] === 'p';
    };

    calculateSnapPoints() {
        const possibleBasePoints = this.getPossiblePoints(this.base, this.startModifier);
        const possibleEndPoints = this.getPossiblePoints(this.target, this.endModifier);

        [this.basePoint, this.targetPoint] = this.getClosestPoints(possibleBasePoints, possibleEndPoints);
    };

    getPossiblePoints(element, modifier) {
        return [
            { x: element.x - modifier, y: element.y },
            { x: element.x + modifier, y: element.y },
            { x: element.x, y: element.y - modifier },
            { x: element.x, y: element.y + modifier }
        ]
    };

    getClosestPoints(basePoints, targetPoints) {
        let points, distance = Infinity;

        basePoints.forEach(base => {
            targetPoints.forEach(target => {
                const tmpDistance = sqrt(pow(base.x - target.x, 2) + pow(base.y - target.y, 2));
                if (tmpDistance < distance) {
                    distance = tmpDistance;
                    points = { base: base, target: target };
                }
            })
        })
        return [points.base, points.target]
    };

    getArrowHeadPoints() {
        let p1, p2, p3;
        p1 = { x: this.targetPoint.x, y: this.targetPoint.y };
        p2 = this.getPointAtDistanceAndAngle(30, ARROW_SIZE)
        p3 = this.getPointAtDistanceAndAngle(-30, ARROW_SIZE)
        return { p1: p1, p2: p2, p3: p3 };
    }

    getPointAtDistanceAndAngle(angle, distance) {
        return {
            x: this.targetPoint.x - ((this.targetPoint.x - this.basePoint.x) * cos(angle * PI / 180) - (this.targetPoint.y - this.basePoint.y) * sin(angle * PI / 180)) * distance / sqrt(pow(this.targetPoint.x - this.basePoint.x, 2) + pow(this.targetPoint.y - this.basePoint.y, 2)),
            y: this.targetPoint.y - ((this.targetPoint.y - this.basePoint.y) * cos(angle * PI / 180) + (this.targetPoint.x - this.basePoint.x) * sin(angle * PI / 180)) * distance / sqrt(pow(this.targetPoint.x - this.basePoint.x, 2) + pow(this.targetPoint.y - this.basePoint.y, 2))
        }
    }
};

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
    };

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
};

function drawAll() {
    drawPlaces();
    drawTransitions();
    drawArcs();
};

function drawArcs() {
    Object.values(_arrows).forEach(arrow => {
        arrow.draw();
    });
};

function drawTransitions() {
    Object.values(_transitions).forEach(transition => {
        const args = [transition.x - transition.dimension / 2, transition.y - transition.dimension / 2, transition.dimension];
        const argsText = [transition.id, transition.x, transition.y];
        transition.draw(square, args, argsText);
    });
};

function drawPlaces() {
    Object.values(_places).forEach(place => {
        const args = [ place.x, place.y, place.dimension ];
        const argsText = [ place.id, place.x + HALF_RADIUS, place.y - HALF_RADIUS ]
        place.draw(circle, args, argsText);
    });
};

function adjustCanvas(x, y) {
    const newWidth = x >= windowWidth ? x + PLUS_X : windowWidth;
    const newHeight = y >= windowHeight ? y + PLUS_Y : windowHeight;
    resizeCanvas(newWidth, newHeight);
};

function mousePressed() {
    _mouseStartX = mouseX;
    _mouseStartY = mouseY;
    _draggedElement = getElementClicked();
}

function mouseReleased() {
    _mouseStartY = undefined;
    _mouseStartY = undefined;
    _draggedElement = undefined;
}

function mouseDragged() {
    if (_draggedElement) {
        _draggedElement.x += mouseX - _mouseStartX;
        _draggedElement.y += mouseY - _mouseStartY;
        _mouseStartX = mouseX;
        _mouseStartY = mouseY;
    }
    redraw();
}

function mouseMoved() {
    Object.values(_places).forEach(place => {
        place.checkForColorChange();
    });

    Object.values(_transitions).forEach(transition => {
        transition.checkForColorChange();
    })
}

function getElementClicked() {
    const element = getPlaceClicked();
    return element ? element : getTransitionClicked();
}

function getPlaceClicked() {
    return Object.values(_places).find(place => place.isMouseOver());
}

function getTransitionClicked() {
    return Object.values(_transitions).find(transition => transition.isMouseOver());
}
