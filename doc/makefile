over: Calc_Sever
	./Calc_Sever
Calc_Sever: SeqStack.o WriteCompound.o Calc_Sever.o
	gcc SeqStack.o WriteCompound.o Calc_Sever.o -g -o Calc_Sever
Calc_Sever.o: Calc_Sever.c
	gcc Calc_Sever.c -g -c -o Calc_Sever.o
SeqStack.o: SeqStack.c
	gcc SeqStack.c -g -c -o SeqStack.o
WriteCompound.o: WriteCompound.c
	gcc WriteCompound.c -g -c -o WriteCompound.o
clean:
	rm Calc_Sever Calc_Sever.o SeqStack.o WriteCompound.o