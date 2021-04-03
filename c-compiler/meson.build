project('C- Compiler', 'c')
# bison part
bison = find_program('bison', required: true)
bisonargs = [
  '@INPUT@',
  '--color',
  '--warning=all',
  '--defines',
  '--output=@OUTPUT@',
]

parsegen = generator(bison,
  output : '@PLAINNAME@.c',
  arguments : bisonargs)

parsefiles = parsegen.process('parser.y')

# flex part
flex = find_program('flex', required: true)
flexlib = dependency('libfl', required: true)

lexgen = generator(flex,
  output : '@PLAINNAME@.c',
  arguments : ['-o', '@OUTPUT@', '@INPUT@'])

lexfiles = lexgen.process('scan.l')

# putting it all together
files = [
  'globals.h',
  'main.c',
  'parse.h',
  'scan.h',
  'util.c', 'util.h',
  'analyze.c', 'analyze.h',
  'symtab.c', 'symtab.h',
  'cgen.c', 'cgen.h',
  'code.c', 'code.h'
]

exe = executable('cm', lexfiles, parsefiles, files, dependencies: [flexlib])
