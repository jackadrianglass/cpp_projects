project('calculator', 'cpp')
compiler_flags = [
    '-std=c++2a',
    '-Wall',
    '-Wextra',
    '-Wshadow',
    '-Wnon-virtual-dtor',
    '-Wold-style-cast',
    '-Wcast-align',
    '-Wunused',
    '-Woverloaded-virtual',
    '-Wpedantic',
    '-Wconversion',
    '-Wsign-conversion',
    '-Wnull-dereference',
    '-Wdouble-promotion',
    '-Wformat=2',
    '-Wmisleading-indentation',
    '-Wduplicated-cond',
    '-Wduplicated-branches',
    '-Wlogical-op',
    '-Wuseless-cast'
]

fmt = dependency('fmt')

executable('calc', 'main.cc', dependencies: fmt, cpp_args: compiler_flags)
