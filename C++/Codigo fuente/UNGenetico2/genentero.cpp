    #ifndef __GENENTERO_CPP
#define __GENENTERO_CPP


#include "genentero.h"


/*****************************************************
               GEN ENTERO (Implementación)
*****************************************************/


/*!Inicializa todos los miembros de la clase. Da al gen un valor por defecto 
promediando los valores \a Min y \a Max.
\param Min Valor mínimo que puede tomar el gen, Por defecto es -10.
\param Max Valor máximo que puede tomar el gen, Por defecto es 10. */
GenEntero::GenEntero(long Min, long Max)
{
	m_Minimo = Min;
	if(Max < Min) Max = Min;
	m_Maximo = Max;
	m_Valor = (Min+Max)/2;
}


/*!Inicializa todos los miembros de la clase con valores suministrados 
por el usuario. 
\param Min Valor mínimo que puede tomar el gen,
\param Max Valor máximo que puede tomar el gen, Si es menor que \a Min,
se hace igual a \a Min.
\param ValorInicial Valor inicial del gen. Se restrige al rango <i>[Min, Max]</i>*/
GenEntero::GenEntero(long Min, long Max, long ValorInicial)
{
	m_Minimo = Min;
	if(Max < Min) Max = Min;
	m_Maximo = Max;	
	m_Valor = restringir(ValorInicial, Min, Max); 
}


/*!Crea un nuevo objeto de la clase OperadorMutacionEnteroUniforme
correspondiente al operador de mutación establecido por defecto para genes enteros.
\return Apuntador al nuevo objeto de la clase OperadorMutacionEnteroUniforme*/
inline OperadorMutacion *GenEntero::operadorMutacionDefecto() const
{
	return (new OperadorMutacionEnteroUniforme);
}

/*!Crea un nuevo objeto de la clase OperadorCruceEnteroBLX,
correspondiente al operador de cruce establecido por defecto para genes enteros.
\return Apuntador al nuevo objeto de la clase OperadorCruceEnteroBLX*/
inline OperadorCruce *GenEntero::operadorCruceDefecto() const
{
	return (new OperadorCruceEnteroBLX());
}


/*****************************************************
                Operadores de Mutación
*****************************************************/

/*!
\param pGen Apuntador al objeto de la clase GenEntero que será sometido a mutación.
El nuevo valor del gen es un número entero aleatorio 
restringido al rango definido por los límites establecidos en el gen.*/
void OperadorMutacionEnteroUniforme::mutarGen(Gen *pGen)
{
	GenEntero *g=(GenEntero*)pGen;
	double azar = (double)rand()/(double)RAND_MAX;
	*g = redondear(g->getMin() + azar*(g->getMax() - g->getMin()) );
}

/*!
\param pGen Apuntador al objeto de la clase GenEntero que será sometido a mutación.

Al aplicar la mutación no uniforme a un gen \f$c_i\f$ que debe pertenecer al intervalo 
\f$[a_i, b_i]\f$ en la generación \a t, con un número máximo
de generaciones \f$g_{max}\f$, el nuevo valor del gen es:
\f[c'_i = \left\{ \begin{array}{ll}
c_i + \Delta(t,b_i-c_i) & \textrm{si $\tau > 0$}\\
c_i - \Delta(t,b_i-c_i) & \textrm{si $\tau \le 0$}
\end{array} \right. \f]
Donde \f[ \Delta(t,y) =  y \left( 1 - r ^ {\left(1 - \frac{t}{g_{max}}  \right) ^ b }  \right) \f]
\a b es un parametro seleccionable por el usuario, por defecto es
0.5; \a r es un número aleatorio en el intervalo \f$[0, 1]\f$, \f$\tau\f$
es un número aleatorio que puede valer 0 ó 1.*/
void OperadorMutacionEnteroNoUniforme::mutarGen(Gen *pGen)
{
	GenEntero *gr = (GenEntero*)pGen;
	double t = m_pAG->m_Generacion,  T = m_pAG->m_GeneracionMaxima;
	double r = (double)rand()/(double)RAND_MAX;
	double tau=(double)rand()/(double)RAND_MAX;
	double y, delta;
    if(tau > 0.5)
    {
		y = gr->getMax() - *gr;
		delta = y * ( 1.0 - pow(r, pow((1.0-t/T),m_b) ) );
		*gr = ( (long)*gr + redondear(delta) );
    }else
    {
        y = *gr - gr->getMin();
        delta = y * ( 1.0 - pow(r, pow((1.0-t/T),m_b) ) );
		*gr = ( (long)*gr - redondear(delta) );        
    }
}

/*!
\param pGen Apuntador al objeto de la clase GenEntero que será sometido a mutación.

Al aplicar la mutación de Muhlenbein a un gen \f$c_i\f$ que debe pertenecer al intervalo 
\f$[a_i, b_i]\f$, el nuevo valor del gen es:
\f[c'_i=c_i \pm \gamma rango \f]
\f[\gamma = \sum_{i=0}^{15} \alpha_i 2^{-i} \f]
donde \a rango define el rango de mutación y se emplea como \f$Factor(b_i-a_i)\f$
El signo + ó - se escoge aleatoriamente con igual probabilidad, y
\f$\alpha _i\f$ puede ser 0 ó 1, con probabilidad de ser 1 igual a:
\f$P(\alpha _i =1)= \frac{1}{16}\f$*/
void OperadorMutacionEnteroMuhlenbein::mutarGen(Gen *pGen)
{
	GenEntero *gr=(GenEntero*)pGen;
	double azar, alfa, rango=m_Factor*(gr->getMax()-gr->getMin());
	double gamma=0.0;
	for(int i=0;i<16;i++)
	{
		azar=(double)rand()/(double)RAND_MAX;
		azar<0.0625? alfa=1.0 : alfa=0.0;
		gamma+=alfa*pow(2.0,-(double)(i));
	}
	azar=(double)rand()/(double)RAND_MAX;
	if(azar<0.5)
		*gr = ( (long)*gr + redondear(rango*gamma) );		
	else
		*gr = ( (long)*gr - redondear(rango*gamma) );				
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
\param indice Es ignorado. Existe por compatibilidad.
Crea \a numHijos genes cuyos valores corresponden a 
números enteros aleatorios que pertenecen al intervalo \f$[pMejor,pPeor]\f$
Los valores resultantes se agregan al arreglo de genes \a pHijos.*/
void OperadorCruceEnteroPlano::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;	
	for(int i=0;i<numHijos;i++)
	{		
		double azar = (double)rand()/(double)RAND_MAX;
		long valor = redondear( *pMadre + azar*( *pPadre - *pMadre ) );
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
números enteros que se obtienen así:
\f[h_1=\lambda pMejor+(1-\lambda)pPeor\f]
\f[h_2=\lambda pPeor+(1-\lambda)pMejor\f]
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroAritmetico::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;
    double alfa;
    for(int i=0;i<numHijos;i++)
    {
        if(i%2 == 0){alfa=m_Lambda;}else{alfa=1.0-m_Lambda;}
		long valor = redondear(alfa * (*pMadre) + (1.0-alfa) * (*pPadre) );
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
        pHijos->Adicionar(gr);
    }
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
números enteros aleatorios del intervalo
\f$[c_{min} - I \alpha , c_{max} + I \alpha]\f$ donde
\f$c_{min}=min[pMejor,pPeor]\f$, \f$c_{max}=max[pMejor,pPeor]\f$
e \f$I=c_{max}-c_{min}\f$.
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroBLX::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;    
    long cMin = _MINIMO( *pMadre, *pPadre);
	long cMax = _MAXIMO( *pMadre, *pPadre);
    long I = cMax - cMin;
	double minimo = cMin - I*m_Alfa;
    double maximo = cMax + I*m_Alfa;
	for(int i=0;i<numHijos;i++)
	{		
        double azar = (double)rand()/(double)RAND_MAX;
        long valor = redondear(minimo + azar*(maximo-minimo));
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
        pHijos->Adicionar(gr);
    }
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos valores que corresponden a 
números enteros que se obtienen así:
\f[ h_1= \frac{1}{2} pMejor + \frac{1}{2} pPeor \f]
\f[ h_2= \frac{3}{2} pMejor - \frac{1}{2} pPeor \f]
\f[ h_3= - \frac{1}{2} pMejor + \frac{3}{2} pPeor \f]
La selección de los valores que se almacenan en los genes depende del 
valor de la variable \a numHijos. 
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroLineal::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;
	long valor=0;
	for(int i=0; i<numHijos; i++)
	{		
		switch (i%3)
		{
		case 0:
			valor= redondear( (*pMadre)*1.5 - (*pPadre)*0.5) ; break;			
		case 1:
			valor= redondear( (*pMadre)*0.5 + (*pPadre)*0.5) ; break;
		case 2:
			valor= redondear(-(*pMadre)*0.5 + (*pPadre)*1.5) ; break;
		}
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores son tomados aleatoriamente 
del valor del gen padre o el del gen madre.
Los genes resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroDiscreto::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;
	long valor;
	for(int i=0;i<numHijos;i++)
	{		
		double azar = (double)rand()/(double)RAND_MAX;
		azar>0.5 ?  valor=(*pMadre) : valor=(*pPadre);
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
números enteros que se obtienen así:
\f[ h= pMejor + \alpha _i(pPeor - pMejor)\f]
\f$\alpha _i\f$ se escoge aleatoriamente en el intervalo \f$[-0.25,1.25]\f$
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroIntermedioExtendido::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;        
    for(int i=0; i<numHijos; i++)
	{		
		double azar=(double)rand()/(double)RAND_MAX;
		double Alfa = -0.25 + 1.5*azar;
		long valor = redondear( *pPadre + Alfa*( *pMadre - *pPadre ));
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
números enteros que se obtienen así:
\f[ h = r (pMejor - pPeor) + pMejor \f]
\a r es un número aleatorio perteneciente al intervalo [0, 1] y \a pMejor corresponde al
gen padre que tiene la mejor función de evaluación.
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroHeuristico::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;
	for(int i=0;i<numHijos;i++)
	{
		double r = (double)rand()/(double)RAND_MAX;
		long valor = redondear( *pMadre + r*( *pMadre - *pPadre));
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

#pragma warning(pop)


/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Valor que identifica al individuo de cuyos genes se realiza el cruce.

Crea \a numHijos genes cuyos valores corresponden a 
números enteros que se obtienen así:

\f[ h_1= pMejor + rango_i \gamma \Delta \f]

Donde: 
\f[ \Delta= frac{pPeor - pMejor}{\left|Fevaluacion_{pMejor}-Fevaluacion_{pPeor}\right|} \f]

\f[ \Gamma= \sum_{k=0}^{15} \alpha_k 2^{-k} \f]
\f[rango_i = 0.5 (Máximo - Mínimo)\f]  

\a pMejor corresponde al gen padre que tiene la mejor función de evaluación,
\a Máximo y \a Mínimo corresponden a los límites establecidos en el gen y
\f$\alpha_k\f$ puede ser 0 ó 1, con probabilidad de ser 1 de 0.0625,
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceEnteroLinealBGA::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenEntero *pMadre=(GenEntero*)pMejor, *pPadre=(GenEntero*)pPeor;
	double dif, delta, gamma, azar, alfa, rango;
	for(int i=0; i<numHijos; i++)
	{
		dif=fabs( m_pAG->m_pPoblacionActual->getIndividuo(indice).objetivo() - 
			m_pAG->m_pPoblacionActual->getIndividuo(indice).getPareja()->objetivo());		
		if(dif!=0)
			delta = ( *pPadre - *pMadre)/dif;
		else
			delta = 1.0;
		gamma=0.0;
		for(int j=0;j<16;j++)
		{
			azar = (double)rand()/(double)RAND_MAX;
			azar<0.0625?  alfa=1.0  : alfa=0.0;
			gamma += alfa*pow(2.0,-(double)(j));
		}
		rango = 0.5*(pMadre->getMax() - pMadre->getMin());
		
		long valor = redondear( *pMadre +  rango * gamma * delta);
		GenEntero *gr=new GenEntero(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}


#endif		//__GENENTERO_CPP