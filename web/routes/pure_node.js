

var express = require('express');
var path = require('path')
var router = express.Router();

var type = path.basename(__filename).slice(0, -3)

router.get('/', function(req, res) {
  res.render('primes', {target:type});
});


router.post('/', function(req, res) {
    var under = parseInt(req.body.under);
    var primes = find_primes(under);

    res.setHeader('Content-Type', 'application/json');
    res.end(JSON.stringify({
      results: primes
    }));

    console.log("Primes generated using " + type);
});

var find_primes = function(limit) {
    var is_prime = [true, true];
    for (var k = 2; k < limit; k++ ) is_prime.push(true);

    var primes = [];
    for (var n = 2; n < limit; n++) {
        if (is_prime[n]) {
            primes.push(n);
            for (var i = n*n; i < limit; i+= n) {
                is_prime[i] = false;
            }
        }
    }
    return primes;
}

module.exports = router;
