module.exports = function(grunt) {

  grunt.initConfig({
    auto_install: {
      local: {},
      lib4ffi: {
        options: {
          cwd: '../cpp/lib4ffi',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },
      nodeprime: {
        options: {
          cwd: '../cpp/nodeprime',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },
      nodeprime_sync: {
        options: {
          cwd: '../cpp/nodeprime_sync',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },
      standalone_flex_file: {
        options: {
          cwd: '../cpp/standalone_flex_file',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },
      standalone_stdio: {
        options: {
          cwd: '../cpp/standalone_stdio',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },
      standalone_usr: {
        options: {
          cwd: '../cpp/standalone_usr',
          stdout: true,
          stderr: true,
          failOnError: true
        }
      }
    },
    clean: {
      cpp: {
        src: ['../cpp/standalone_usr/build/', 
              '../cpp/standalone_flex_file/build',
              '../cpp/standalone_usr/build', 
              '../cpp/nodeprime/build', 
              '../cpp/nodeprime/node_modules', 
              '../cpp/nodeprime_sync/build', 
              '../cpp/nodeprime_sync/node_modules',
              '../cpp/lib4ffi/build'],
        options: {
          'force':true
        }
      }
    }

  });

  grunt.loadNpmTasks('grunt-auto-install');
  grunt.loadNpmTasks('grunt-contrib-clean');

  grunt.registerTask('default', ['auto_install']);
  grunt.registerTask('cleanup', ['clean:cpp']);
  
};