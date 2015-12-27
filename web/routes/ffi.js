var express = require('express');
var path = require('path')
var router = express.Router();

var type = path.basename(__filename).slice(0, -3)

router.get('/', function(req, res) {
  res.render('primes', {target:type});
});


router.post('/', function(req, res) {
    var ffi = require('ffi')
    var ref = require('ref')
    var ArrayType = require('ref-array')
    var int = ref.types.int
    var IntArray = ArrayType(int)

    var under = parseInt(req.body.under);
    var a = new IntArray(under);


    var libprime = ffi.Library('../cpp/lib4ffi/build/Release/prime', {
      'getPrimes': [ int, [ int, IntArray] ]
    })

    var count = libprime.getPrimes(under, a);
    var primes = a.toArray().slice(0, count);

    res.setHeader('Content-Type', 'application/json');
    res.end(JSON.stringify({
      results: primes
    }));

    console.log("Primes generated using " + type);
});
module.exports = router;
