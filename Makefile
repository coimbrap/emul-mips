all:
	echo "make rendu1 || make rendu2"

rendu1:
	rm -rf rendu_1 Rendu1_Barnel_Coimbra.tgz && mkdir rendu_1 && cp -r main/* rendu_1/ && cd rendu_1/ && find . | grep ".git" | xargs rm -f && rm -f test && rm -f *.o && tar -zcf ../Rendu1_Barnel_Coimbra.tgz * && cd - && rm -rf rendu_1

clean:
	rm -rf rendu_1 Rendu1_Barnel_Coimbra.tgz
