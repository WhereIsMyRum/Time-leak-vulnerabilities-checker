const nets = {
    "1\.Example-net-1": {
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
        },
        "results": [
            "H1-lowEnd",
            "H2-lowStart"
        ]
    },
    "2\.Example-net-2": {
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
        },
        "results": [
            "H1-low",
            "H2-maxDuration",
            "H3-maxDuration"
        ]
    },
    "3\.Basic-TL": {
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
        },
        "results": ["H1-low"]
    },
    "4\.Obfuscated-End": {
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
        },
        "results": ["H1-lowStart", "H2-high"]
    },

    "5\.Obfuscated-Start": {
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
        },
        "results": ["H1-high", "H2-lowEnd"]
    },

    "6\.Parallel-TL": {
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
        },
        "results": ["H1-maxDuration"]
    },

    "7\.Parallel-TL-By-Place": {
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
        },
        "results": ["H1-lowStart", "H2-high"]
    },

    "8\.Combined-TL": {
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
        },
        "results": ["H1-low", "H2-low"]
    },
    "9\.Test-Low-Start": {
        "places": ["p1", "p2", "p3", "p4"],
        "transitions": {
            "high": ["H1", "H2", "H3"],
            "low": ["L1", "L2"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1", "H2", "L2"],
                "p3": ["H3"],
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p3"],
                "H1": ["p3"],
                "H2": ["p3"],
                "H3": ["p4"]
            }
        },
        "results": ["H1-knowIfNotExecuted"]
    },
    "10\.Combined-Multiple-Layers": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "p10", "p11"],
        "transitions": {
            "high": ["H1", "H2", "H3", "H4"],
            "low": ["L1", "L2", "L3", "L4", "L5"]
        },
        "flows": {
            "places": {
                "p1": ["L1", "L2"],
                "p2": ["H4"],
                "p3": ["H1"],
                "p4": ["H3"],
                "p5": ["H2"],
                "p6": ["H4"],
                "p7": ["L3"],
                "p8": ["H3"],
                "p9": ["L4"],
                "p10": ["L5"],
            },
            "transitions": {
                "L1": ["p4"],
                "L2": ["p2", "p3"],
                "L3": ["p11"],
                "L4": ["p11"],
                "L5": ["p11"],
                "H1": ["p5", "p6"],
                "H2": ["p7", "p8"],
                "H3": ["p10"],
                "H4": ["p9"]
            }
        },
        "results": ["H1-low", "H2-low", "H3-low", "H4-low"]
    },
    "11\.Split-Start-No-Split-Places": {
        "places": ["p1", "p2", "p3", "p4", "p5"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["L2", "H1"],
                "p3": ["H2"],
                "p4": ["L3"]
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p3"],
                "H1": ["p3"],
                "H2": ["p4"],
                "L3": ["p5"]
            }
        },
        "results": ["H1-lowStart", "H2-lowEnd"]
    },
    "12\.Split-Start-Split-Places": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["L2", "H1"],
                "p3": ["H2"],
                "p4": ["H2"],
                "p5": ["L3"]
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p3"],
                "H1": ["p4"],
                "H2": ["p5"],
                "L3": ["p6"]
            }
        },
        "results": ["H1-lowStart", "H2-lowEnd"]
    },
    "13\.Split-Start-Split-Places-Both-Sides": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["L2"],
                "p3": ["H2"],
                "p4": ["H2"],
                "p5": ["L3"],
                "p7": ["H1"]
            },
            "transitions": {
                "L1": ["p2", "p7"],
                "L2": ["p3"],
                "H1": ["p4"],
                "H2": ["p5"],
                "L3": ["p6"]
            }
        },
        "results": ["H1-lowStart", "H2-lowEnd"]
    },
    "14\.Split-End-No-Split-Places": {
        "places": ["p1", "p2", "p3", "p4", "p5"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["H2", "L2"],
                "p4": ["L3"]
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p4"],
                "H1": ["p3"],
                "H2": ["p4"],
                "L3": ["p5"]
            }
        },
        "results": ["H1-lowStart", "H2-lowEnd"]
    },
    "15\.Split-End-Split-Places": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["H2"],
                "p4": ["L3"],
                "p6": ["L2"]
            },
            "transitions": {
                "L1": ["p2"],
                "L2": ["p4"],
                "H1": ["p3", "p6"],
                "H2": ["p4"],
                "L3": ["p5"]
            }
        },
        "results": ["H1-low", "H2-low"]
    },
    "16\.Split-End-Split-Places-Both-Sides": {
        "places": ["p1", "p2", "p3", "p4", "p5", "p6", "p7"],
        "transitions": {
            "high": ["H1", "H2"],
            "low": ["L1", "L2", "L3"]
        },
        "flows": {
            "places": {
                "p1": ["L1"],
                "p2": ["H1"],
                "p3": ["H2"],
                "p4": ["L3"],
                "p6": ["L2"],

                "p7": ["L3"],
            },

            "transitions": {
                "L1": ["p2"],
                "L2": ["p7"],
                "H1": ["p3", "p6"],
                "H2": ["p4"],
                "L3": ["p5"]
            }
        },
        "results": ["H1-low", "H2-maxDuration"]
    }
}

module.exports = { nets } 
