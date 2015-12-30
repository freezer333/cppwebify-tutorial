# Getting your C++ to the Web with Node.js

This repository is the code for a blog series I wrote [here](http://blog.scottfrees.com/getting-your-c-to-the-web-with-node-js).
It's designed to outline a few ways to take legacy C++ (or new C++) and integrate it with Node.js.

## Build instructions
Until I get around to creating a better set of build scripts, each of the examples need to be built on their own (sorry!).

### Step 1:  Install node-gyp

```
> npm install -g node-gyp
```

### Step 2:  Build all the C++ examples
You need to got into each directory under /cpp that has a binding.gyp file - which are the following:

```
/cpp/lib4ffi
/cpp/nodeprime  *
/cpp/nodeprime_sync  *
/cpp/standalone_flex_file
/cpp/standalone_stdio
/cpp/standalone_usr
```

And type `node-gyp configure build` inside each directory.  **For the * entries** - 'nodeprime' and 'nodeprime_sync' you must also install NAN **before** issuing the `node-gyp` command.  Just type `npm install` and NAN will be installed.

### Step 3:  Install web dependencies
To run the web app, go to `/web` and type `npm install`.  

You can then launch the web app, that lets you test each example, by typing `node index`.
