import os
import ConfigParser

env = Environment().Clone()
config = ConfigParser.RawConfigParser()
config.read('scons.cfg')

# env['CC'] = 'clang'
# env['CXX'] = 'clang++'
env['CXX'] = 'g++'
print "CXX is:", env['CXX']

env.Append(
        CPPPATH = [
                '#',
                '#libraries',
                '#libraries/SpecReader/src',
                '#libraries/BasicElement/src',
                '#libraries/FileReader/src',
                '#libraries/ReportGenerator/src',
                '#libraries/Comparator/src',
                config.get('PYTHON', 'inc'),
                config.get('BOOST', 'inc'),
                ]
        )
if env['CXX'] == 'clang++' :
        env.Append(
                CXXFLAGS = [
                        '-std=c++11',
                        '-fcolor-diagnostics',
                        '-fPIC',
                        #'-Wall', '-small' , '-fcompact', '-O', '-modern'
                ]
        )
else :
        env.Append(
                CXXFLAGS = [
                        '-std=gnu++0x',
                        # '-fcolor-diagnostics',
                        # '-fPIC',
                        #'-Wall', '-small' , '-fcompact', '-O', '-modern'
                ]
        )


# env.SharedLibrary(
#         'fileReader',
#         Glob( 'libraries/FileReader/src/FileReader/*.cpp' ),
#         )

env.SharedLibrary(
        'specReader',
        Glob( 'libraries/SpecReader/src/SpecReader/*.cpp' ),
        LIBPATH=config.get('BOOST', 'libdir'),
        LIBS=['boost_filesystem'],
        )

env.SharedLibrary(
        'basicElement',
        [ Glob( 'libraries/BasicElement/src/BasicElement/*.cpp' ), 
          Glob( 'libraries/BasicElement/src/BasicElement/SubElements/*.cpp' ),
        ],
        LIBPATH='.',
        LIBS=['specReader'],
        )

env.SharedLibrary(
        'comparator',
        Glob( 'libraries/Comparator/src/Comparator/*.cpp' ),
        LIBPATH='.',
        LIBS=['basicElement'],
        )

env.SharedLibrary(
        'reportGenerator',
        Glob( 'libraries/ReportGenerator/src/ReportGenerator/*.cpp' ),
        LIBPATH='.',
        LIBS=['basicElement'],
        )

env.Program(
        'mikqc',
        # Glob( 'app/*.cpp' ),
        Glob( 'libraries/main.cpp' ),
        LIBPATH=[ '.', 
                  config.get('BOOST', 'libdir'), 
                ],
        LIBS=['fileReader',
              'specReader',
              'basicElement',
              'comparator',
              'reportGenerator',
              'boost_system',
              'boost_filesystem',
             ],
        )

