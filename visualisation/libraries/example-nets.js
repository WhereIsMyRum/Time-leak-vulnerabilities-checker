const nets = {
    "net1": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["H1"],
                "p2": ["L1", "L2", "L3"],
                "p3": ["H2"]
            },
            "transitions": {
                "H1": ["p2"],
                "L1": ["p3"],
                "L2": ["p3"],
                "L3": ["p3"],
                "H2": ["p4"]
            }
        }
    },
    "net2": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8"],
        "transitions": {
            "high": ["H1", "H2", "H3"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["H2"],
                "p4": ["L2"],
                "p5": ["H3"],
                "p6": ["L3"],
                "p7": ["L3"]
            },
            "transitions": {
                "L1": ["p2", "p3"],
                "H1": ["p4", "p5"],
                "H2": ["p7"],
                "L2": ["p6"],
                "H3": ["p6"],
                "L3": ["p8"]
            }
        }
    },
    "net3": {
        "places": ["p1", "p2", "p3", "p4", "p5"],
        "transitions": {
            "high": [],
            "low": ["L1", "L2"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["L2"],
                "p3": ["L2"],
                "p4": ["L2"]
            },
            "transitions": {
                "L1": ["p2", "p3", "p4"],
                "L2": ["p5"]
            }
        }
    },

    "basicTL": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1"],
            "low": ["L1", "L2"]
        },
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
    },

    "parallelTL": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6"],
        "transitions": {
            "low": ["L1", "L2", "L3"],
            "high": ["H1"]
        },
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
    },

    "parallelTLByPlace": {
        "places": ["p1", "p2", "p3", "p4", "p5"],
        "transitions": {
            "low": ["L1", "L2"],
            "high": ["H1", "H2"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["L2"],
                "p4": ["H2"]
            },
            "transitions": {
                "L1": ["p2", "p3"],
                "L2": ["p4"],
                "H1": ["p4"],
                "H2": ["p5"]
            }
        }
    },

    "obfuscatedEnd": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["H2"]
            },
            "transitions": {
                "L1": ["p2"],
                "H1": ["p3"],
                "H2": ["p4"]
            }
        }
    },

    "obfuscatedStart": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L2"]
        },
        "flows": {
            "places": {
                "p1": ["H1"],
                "p2": ["H2"],
                "p3": ["L2"]
            },
            "transitions": {
                "H1": ["p2"],
                "H2": ["p3"],
                "L2": ["p4"]
            }
        }
    },

    "combinedTL": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3", "L4"]
        },
        "flows": {
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
    },
    "testLowStart": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1", "H2", "H3"],
            "low": ["L1", "L2"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1", "H2","L2"],
                "p3": ["H3"],
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p3"],
                "H1": ["p3"],
                "H2": ["p3"],
                "H3": ["p4"]
            }
        }
    }
}

module.exports = { nets } 
