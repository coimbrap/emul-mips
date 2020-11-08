all:
	echo "make rendu1 || make rendu2"

rendu1:
	rm -rf partie1 Rendu1_Barnel_Coimbra.tgz && mkdir partie1 && cp -r main/* partie1/ && cd partie1/ && find . | grep ".git" | xargs rm -f && rm -f test && rm -f *.o && tar -zcf ../Rendu1_Barnel_Coimbra.tgz * && cd - && rm -rf partie1

clean:
	rm -rf partie1 Rendu1_Barnel_Coimbra.tgz
