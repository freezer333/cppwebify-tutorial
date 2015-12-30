var express = require('express');
var app = express();
var bodyParser = require('body-parser');

app.use(express.static('public'));
app.set('view engine', 'jade');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

var types = [
  {
    title: "pure_node",
    description: "Execute a really primitive implementation of prime sieve in Node.js"
  },
  {
    title: "standalone_args",
    description: "Execute C++ executable as a child process, using command line args and stdout.  Based on /cpp/standalone_stdio"
  },
  {
    title: "standalone_usr",
    description: "Execute C++ executable as a child process, using direct user input.  Based on /cpp/standalone_usr"
  },
  {
    title: "standalone_file",
    description: "Execute C++ executable as a child process, using an input and output file.  Based on /cpp/standalone_flex_file"
  },
  {
    title: "ffi",
    description: "Using Node Foreign Function Interface (ffi) to call C++ code.  Based on /cpp/lib4ffi"
  },
  {
    title: "addonsync",
    description: "Creating a Synchronous Node Addon that can be called like any other module.  Based on /cpp/nodeprime_sync"
  },
  {
    title: "addon",
    description: "Creating a Asynchronous Node Addon that can be called like any other module.  Based on /cpp/nodeprime"
  }
  ];


types.forEach(function (type) {
    app.use('/'+type.title, require('./routes/' + type.title));
});

app.get('/', function (req, res) {
  res.render('index', { routes: types});
});

var server = app.listen(3000, function () {
  console.log('Web server listing at http://localhost:%s', server.address().port);
});
