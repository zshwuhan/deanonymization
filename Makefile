OVERLAP=5000
THREAD=8
ITERFLAG=1
ALGO=0
ENCRYPT=1
CODE=./source/
OBJ=./bin/
DATA=./data/

$(OBJ)baseAlgo : $(CODE)baseAlgo.cpp $(CODE)init.cpp $(CODE)output.cpp $(CODE)compute.cpp $(OBJ)graphSplit $(OBJ)graphAnonymity $(DATA)graph1.txt $(DATA)graphTarget.txt
	@g++ -O2 -o $(OBJ)baseAlgo $(CODE)baseAlgo.cpp $(CODE)init.cpp $(CODE)output.cpp $(CODE)compute.cpp -pthread
	@echo "main program built"

$(OBJ)graphSplit : $(CODE)graphSplit.cpp $(DATA)smallG.txt
	@g++ -O2 -o $(OBJ)graphSplit $(CODE)graphSplit.cpp
	@echo "graphSplit compiled"
	@$(OBJ)graphSplit $(OVERLAP)
	@echo "smallG has been splited to two graphs"

$(OBJ)graphAnonymity : $(CODE)graphAnonymity.cpp $(DATA)graph2.txt
	@g++ -O2 -o $(OBJ)graphAnonymity $(CODE)graphAnonymity.cpp 
	@$(OBJ)graphAnonymity $(ENCRYPT)
	@echo "anonymity done"


.PHONY: main
main:
	@g++ -O2 -o $(OBJ)baseAlgo $(CODE)baseAlgo.cpp $(CODE)init.cpp $(CODE)output.cpp $(CODE)compute.cpp -pthread
	@echo build success

.PHONY: run-full
run-full:
	@$(OBJ)baseAlgo $(OVERLAP) $(THREAD) 1 $(ALGO)

.PHONY: run
run:
	@$(OBJ)baseAlgo $(OVERLAP) $(THREAD) 0 $(ALGO)


.PHONY: split
split:
	@$(OBJ)graphSplit $(OVERLAP)
	@echo "split"

.PHONY: encrypt
encrypt:
	@$(OBJ)graphAnonymity $(ENCRYPT)
	@echo "anonimated"

.PHONY: clean
clean:
	@-rm $(OBJ)baseAlgo
	@echo "bin clean"

.PHONY: clean-data
clean-data:
	@-rm $(DATA)g*.txt
	@-rm $(DATA)pair.txt
	@echo "data clean"

.PHONY: clean-result
clean-result:
	@-rm ./result/*
	@echo "result clean"

.PHONY: reset
reset:
	@-rm $(OBJ)g*
	@-rm $(OBJ)baseAlgo
