var express = require('express');
var path = require('path')
var router = express.Router();

var type = path.basename(__filename).slice(0, -3)

router.get('/', function(req, res) {
  res.render('primes', {target:type});
});

router.post('/', function(req, res) {

    // I'm using exec, but if you don't want to
    // buffer all the output, but use it incrementally,
    // you'd use the other methods (ie. streaming).
    // That would be more effective if you were
    // streaming the results to the browser, but here
    // we need all the primes to render the response anyway...
    var execFile = require('child_process').execFile
    var program = "../cpp/standalone_stdio/build/Release/standalone";


    var under = parseInt(req.body.under);
    var child = execFile(program, [under],
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



});


module.exports = router;
