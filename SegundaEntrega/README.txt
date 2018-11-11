Andreia Sofia Suzano Pereira, 89414
Tiago Pereira Gonçalves Lé, 89550

1) Structure of the directories
.
├── CircuitRouter-ParSolver
├── CircuitRouter-SeqSolver
├── CircuitRouter-SimpleShell
├── inputs
├── lib
└── results
6 directories

2) How to compile and execute
    a) Compiling and executing only CircuitRouter-ParSolver
       	cd CircuitRouter-ParSolver/ && make
	./CircuitRouter-ParSolver -t NUMTHREADS INPUTFILE

    b) Compiling and executing doTest.sh
	cd CircuitRouter-SeqSolver/ && make
	cd ../CircuitRouter-ParSolver/ && make
	chmod +x doTest.sh
	./doTest.sh NUMTHREADS INPUTFILE


3) Description of the characteristics of the processors and operating    system used

	cpu cores	: 4

	clock rate      : 1.80GHz

	model		: 142
	model name	: Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
	
	Linux andreia-Lenovo-ideapad-720S-14IKB 4.15.0-36-generic #39-Ubuntu SMP Mon Sep 24 16:19:09 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
