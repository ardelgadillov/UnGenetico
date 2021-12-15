
/*EJEMPLO SIMPLE DE APLICACIÓN DE LA LIBRERIA UNGenetico 2.0.

  Este ejemplo ilustra lo mínimo que debe contener una aplicación 
  de UNGenetico 2.0.
  Encuentra el mínimo de la funcion   
			f(x)=sin(x)+sin(4x) en el intervalo [0,10].
  
  La salida del programa debe ser la siguiente:
			F(Xmin)= -1.92821 Xmin= 4.34233	

*/

#include "UNGenetico.h"


DECLARAR_ALGORITMO(MiAG)
	double x;
FIN_DECLARAR_ALGORITMO


/*funciones requeridas*/

double MiAG::objetivo()
{
	return sin(x) + sin(4*x);	
}

void MiAG::codificacion(Individuo* pInd, int estado)
{
	ADICIONAR_GENREAL(pInd, 0, x, 0, 10, 0);
}

int main()
{
	cout << "EJEMPLO \"MINIMO\" DE APLICACION DE UNGenetico 2.0\n\n";
	cout << "Encuentra el minimo de la funcion  f(x)=sin(x)+sin(4x) en el intervalo [0,10].\n\nOptimizando...\n\n\n\n";
	cout.flush();

	MiAG AG;
	AG.optimizar();
	cout << "Solucion:\n\nF(Xmin)= " << AG.m_pMejorEnLaHistoria->objetivo(true);	//objetivo() se invoca con tru para obligar actualizacion de variables en el AG.
	cout << " Xmin= "<< AG.x << "\n\n\n\nPulse una tecla y Enter para salir.";
	char a;
	cin >> a;
	return 0;
}
