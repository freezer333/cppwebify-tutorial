var express = require('express');
var path = require('path')
var router = express.Router();
var temp = require('temp');
var fs = require('fs');

var type = path.basename(__filename).slice(0, -3)

router.get('/', function(req, res) {
  res.render('primes', {target:type});
});

router.post('/', function(req, res) {
    var exec = require('child_process').exec;
    var execFile = require('child_process').execFile
    var program = "../cpp/standalone_flex_file/build/Release/standalone_flex_file";
    var under = parseInt(req.body.under);

    temp.mkdir('node_example', function(err, dirPath) {
      var inputPath = path.join(dirPath, 'input.txt');
      var outputPath = path.join(dirPath, 'output.txt');

      fs.writeFile(inputPath, under, function(err) {
        if (err) throw err;
        var primes = execFile(program, [inputPath, outputPath], function(error) {
            if (error ) throw error;
            fs.readFile(outputPath, function(err, data) {
              if (err) throw err;
              var primes = data.toString().split('\n').slice(0, -3)
                              .map(function (line) {
                                  return parseInt(line);
                              });
              res.setHeader('Content-Type', 'application/json');
              res.end(JSON.stringify({
                results: primes
              }));

              exec('rm -r ' + dirPath, function(error) {
                if (error) throw error;
                console.log("Removed " + dirPath);
              })
          });
        });
      });
    });
});


module.exports = router;
