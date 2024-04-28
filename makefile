default:
	gcc -o passwordSolver.exe falloutPassword.c
	./passwordSolver.exe data/senhas.txt

# test:
# 	gcc -o passwordSolver.exe falloutPassword.c
# 	cat ./tests/testCase.txt | ./passwordSolver.exe ./tests/senhasTest.txt