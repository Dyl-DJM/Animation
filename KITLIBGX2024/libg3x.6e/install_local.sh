#!/bin/bash
printf '#---------------------------------------------------------------------------------\n# ' cat >> $HOME/.bashrc
date | cat >> $HOME/.bashrc
echo "# Variables d'environnement pour la lib. graphique <g3x> - Version 6e - 2022
#---------------------------------------------------------------------------------
# libG3X - installée comme une lib. locale ($PWD)
export G3X_VERSION='6e-23'
export G3X_PATH=$PWD
export GL_PATH='/usr/lib/x86_64-linux-gnu/'
export libG3X='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(G3X_PATH)/bin -lg3x'
export incG3X='-I\$(G3X_PATH)/include'
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$G3X_PATH/bin
#---------------------------------------------------------------------------------
"| cat >> $HOME/.bashrc;
make cleanall;
make clean; export GDB=1; make g3x; # compilation 'C'   en mode debug    -> libg3x.6e.gdb.{so,a}
make clean; export CPP=1; make g3x; # compilation 'C++' en mode debug    -> libg3x.6e++.gdb.{so,a}
make clean; export GDB=0; make g3x; # compilation 'C++' en mode optimisé -> libg3x.6e++.{so,a}
make clean; export CPP=0; make g3x; # compilation 'C'   en mode optimisé -> libg3x.6e.{so,a}  -- VERSION PAR DEFAUT
make clean;
bash;
