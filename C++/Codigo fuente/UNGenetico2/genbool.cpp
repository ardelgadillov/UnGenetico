#ifndef __GENBOOL_CPP
#define __GENBOOL_CPP


#include"genbool.h"


/*****************************************************
                GEN BOOL (Implementación)
*****************************************************/

/*!Crea un nuevo objeto de la clase OperadorMutacionBoolUniforme,
correspondiente al operador de mutación establecido por defecto para genes de tipo bool.
\return Apuntador al nuevo objeto de la clase OperadorMutacionBoolUniforme*/
inline OperadorMutacion *GenBool::operadorMutacionDefecto() const
{
    return (new OperadorMutacionBoolUniforme);
}

/*!Crea un nuevo objeto de la clase OperadorCruceBoolDiscreto,
correspondiente al operador de cruce establecido por defecto para genes de tipo bool.
\return Apuntador al nuevo objeto de la clase OperadorCruceBoolPlano*/
inline OperadorCruce *GenBool::operadorCruceDefecto() const
{
	return (new OperadorCruceBoolDiscreto);
}


/*****************************************************
                Operadores de Mutación 
*****************************************************/


/*!
\param pGen Apuntador al objeto de la clase GenBool que será sometido a mutación.*/
void OperadorMutacionBoolUniforme::mutarGen(Gen *pGen)
{
	GenBool *pGB=(GenBool*)pGen;
	*pGB = !((bool)(*pGB));
}


/*****************************************************
                Operadores de Cruce
*****************************************************/
#pragma warning(push, 3)	//Para evitar  "warning(level 4) C4100" ocasionada por parámetros de funciones no referenciados
/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.*/
void OperadorCruceBoolDiscreto::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
    GenBool *pMadre=(GenBool*)pMejor, *pPadre=(GenBool*)pPeor;
    for(int i=0;i<numHijos;i++)
    {
		GenBool *gr;
        double azar = (double)rand()/(double)RAND_MAX;
		azar<0.5? gr=new GenBool(*pMadre) : gr=new GenBool(*pPadre);
        pHijos->Adicionar(gr);
    }
}

#pragma warning(pop)


#endif  //__GENBOOL_CPP