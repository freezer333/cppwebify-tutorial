
var express = require('express');
var path = require('path')
var router = express.Router();

var type = path.basename(__filename).slice(0, -3)

router.get('/', function(req, res) {
  res.render('primes', {target:type});
});

router.post('/', function(req, res) {

    var execFile = require('child_process').execFile
    var program = "../cpp/standalone_usr/build/Release/standalone_usr";

    var under = parseInt(req.body.under);
    var child = execFile(program, [],
      function (error, stdout, stderr) {
        var primes = stdout.split("\n")
                       .slice(0, -3)
                       .map(function (line) {
                           return parseInt(line);
                       });

        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({
          results: primes
        }));

        console.log("Primes generated from " + type);
    });

    child.stdin.setEncoding('utf-8');
    child.stdin.write(under + "\n");



});


module.exports = router;
