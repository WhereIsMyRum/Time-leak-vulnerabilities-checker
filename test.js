const assert = require('assert');
const nets = require('./visualisation/libraries/example-nets').nets;
const childProcess = require('child_process');
const _ = require('lodash');

describe('Main', () => {
    Object.entries(nets).forEach(([key,value]) => {
        if (value.results.length != 0) {
            it(`${key} should return correct answer`, () => {
                let result = childProcess.execFileSync('./time-leak-detector/main.exe', [JSON.stringify(value)]).toString().split('\n');
                result = result.map(result => result.replace('\r', ''));
                result.pop();
                assert.ok(_.isEqual(result, value.results));
            })
        }
    })
})