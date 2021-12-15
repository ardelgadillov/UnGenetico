#ifndef __GENREAL_CPP
#define __GENREAL_CPP

#include "genreal.h"

/************************************************************
                      GENREAL (Implementaci�n)
************************************************************/

/*!Inicializa todos los miembros de la clase. Da al gen un valor por defecto 
promediando los valores \a Min y \a Max.
\param Min Valor m�nimo que puede tomar el gen, Por defecto es 0.0.
\param Max Valor m�ximo que puede tomar el gen, Por defecto es 1.0.*/
GenReal::GenReal(double Min, double Max)
{
	m_Minimo=Min;
	if(Max < Min) Max = Min;
	m_Maximo=Max;
	m_Valor=(Min+Max)/2.0;
}

/*!Inicializa todos los miembros de la clase con valores suministrados 
por el usuario. 
\param Min Valor m�nimo que puede tomar el gen,
\param Max Valor m�ximo que puede tomar el gen, Si es menor que \a Min,
se hace igual a \a Min.
\param ValorInicial Valor inicial del gen. Se restrige al rango <i>[Min, Max]</i>*/
GenReal::GenReal(double Min, double Max, double ValorInicial)
{
	m_Minimo = Min;
	if(Max < Min) Max = Min;
	m_Maximo = Max;	
	m_Valor = restringir(ValorInicial, Min, Max); 
}


/*!Crea un nuevo objeto de la clase OperadorMutacionRealUniforme,
correspondiente al operador de mutaci�n establecido por defecto para genes reales.
\return Apuntador al nuevo objeto de la clase OperadorMutacionRealUniforme*/
inline OperadorMutacion *GenReal::operadorMutacionDefecto() const
{
	return (new OperadorMutacionRealUniforme);
}


/*!Crea un nuevo objeto de la clase OperadorCruceRealBLX,
correspondiente al operador de cruce establecido por defecto para genes reales.
\return Apuntador al nuevo objeto de la clase OperadorCruceRealBLX*/
inline OperadorCruce *GenReal::operadorCruceDefecto() const
{
	return (new OperadorCruceRealBLX() );
}

/******************************************************************
             Operadores de Mutaci�n Real (Implementaci�n)
******************************************************************/

/*!
\param pGen Apuntador al objeto de la clase GenReal que ser� sometido a mutaci�n.
El nuevo valor del gen es un n�mero real aleatorio 
restringido al rango definido por los l�mites establecidos en el gen.*/
void OperadorMutacionRealUniforme::mutarGen(Gen *pGen)
{
	GenReal *gr=(GenReal*)pGen;
	double azar=(double)((double)rand()/(double)RAND_MAX);
	*gr = ( gr->getMin() + azar*(gr->getMax()-gr->getMin()) );
}

/*!
\param pGen Apuntador al objeto de la clase GenReal que ser� sometido a mutaci�n.

Al aplicar la mutaci�n no uniforme a un gen \f$c_i\f$ que debe pertenecer al intervalo 
\f$[a_i, b_i]\f$ en la generaci�n \a t, con un n�mero m�ximo
de generaciones \f$g_{max}\f$, el nuevo valor del gen es:
\f[c'_i = \left\{ \begin{array}{ll}
c_i + \Delta(t,b_i-c_i) & \textrm{si $\tau > 0$}\\
c_i - \Delta(t,b_i-c_i) & \textrm{si $\tau \le 0$}
\end{array} \right. \f]
Donde \f[ \Delta(t,y) =  y \left( 1 - r ^ {\left(1 - \frac{t}{g_{max}}  \right) ^ b }  \right) \f]
\a b es un parametro seleccionable por el usuario, por defecto es
0.5; \a r es un n�mero aleatorio en el intervalo \f$[0, 1]\f$, \f$\tau\f$
es un n�mero aleatorio que puede valer 0 � 1.*/
void OperadorMutacionRealNoUniforme::mutarGen(Gen *g)
{
	GenReal *gr = (GenReal*)g;
	double t = m_pAG->m_Generacion,  T = m_pAG->m_GeneracionMaxima;
	double r = (double)((double)rand()/(double)RAND_MAX);
	double tau=(double)((double)rand()/(double)RAND_MAX);
	double y, delta;
    if(tau > 0.5)
    {
		y = gr->getMax() - (*gr);
		delta = y * ( 1.0 - pow(r, pow((1.0-t/T),m_b) ) );
		*gr = ( (double)*gr + delta );
    }else
    {
        y = (*gr) - gr->getMin();
        delta = y * ( 1.0 - pow(r, pow((1.0-t/T),m_b) ) );
        *gr = ( (double)*gr - delta ); 
    }
}

/*!
\param pGen Apuntador al objeto de la clase GenReal que ser� sometido a mutaci�n.

Al aplicar la mutaci�n de Muhlenbein a un gen \f$c_i\f$ que debe pertenecer al intervalo 
\f$[a_i, b_i]\f$, el nuevo valor del gen es:
\f[c'_i=c_i \pm \gamma rango \f]
\f[\gamma = \sum_{i=0}^{15} \alpha_i 2^{-i} \f]
donde \a rango define el rango de mutaci�n y se emplea como \f$Factor(b_i-a_i)\f$
El signo + � - se escoge aleatoriamente con igual probabilidad, y
\f$\alpha _i\f$ puede ser 0 � 1, con probabilidad de ser 1 igual a:
\f$P(\alpha _i =1)= \frac{1}{16}\f$*/
void OperadorMutacionRealMuhlenbein::mutarGen(Gen *pGen)
{
	GenReal *gr=(GenReal*)pGen;
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
		*gr = ( (double)*gr + rango*gamma );
	else
		*gr = ( (double)*gr - rango*gamma );		
}

/***********************************************************
               Operadores de Cruce 
************************************************************/

#pragma warning(push, 3)	//Para evitar  "warning(level 4) C4100" ocasionada por par�metros de funciones no referenciados

/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.
Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales aleatorios que pertenecen al intervalo \f$[pMejor,pPeor]\f$
Los valores resultantes se agregan al arreglo de genes \a pHijos.*/
void OperadorCruceRealPlano::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
	double azar, valor;
	for(int i=0;i<numHijos;i++)
	{
		azar = (double)((double)rand()/(double)RAND_MAX);
		valor = *pMadre + azar*( *pPadre - *pMadre );
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
 		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales que se obtienen as�:
\f[h_1=\lambda pMejor+(1-\lambda)pPeor\f]
\f[h_2=\lambda pPeor+(1-\lambda)pMejor\f]
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealAritmetico::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
    double alfa, valor;
    for(int i=0;i<numHijos;i++)
    {
        if(i%2 == 0){alfa=m_Lambda;}else{alfa=1.0-m_Lambda;}
		valor = alfa * (*pMadre) + (1.0-alfa) * (*pPadre);
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
        pHijos->Adicionar(gr);
    }
}

/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales aleatorios del intervalo
\f$[c_{min} - I \alpha , c_{max} + I \alpha]\f$ donde
\f$c_{min}=min[pMejor,pPeor]\f$, \f$c_{max}=max[pMejor,pPeor]\f$
e \f$I=c_{max}-c_{min}\f$.
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealBLX::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;    
    double cMin = _MINIMO( *pMadre, *pPadre);
	double cMax = _MAXIMO( *pMadre, *pPadre);
    double I = cMax - cMin;
	double minimo = cMin - I*m_Alfa;
    double maximo = cMax + I*m_Alfa;
	for(int i=0;i<numHijos;i++)
	{
        double azar = (double)rand()/(double)RAND_MAX;
		double valor = minimo + azar*(maximo-minimo);
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
        pHijos->Adicionar(gr);
    }
}


/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos valores que corresponden a 
n�meros reales que se obtienen as�:
\f[ h_1= \frac{1}{2} pMejor + \frac{1}{2} pPeor \f]
\f[ h_2= \frac{3}{2} pMejor - \frac{1}{2} pPeor \f]
\f[ h_3= - \frac{1}{2} pMejor + \frac{3}{2} pPeor \f]
La selecci�n de los valores que se almacenan en los genes depende del 
valor de la variable \a numHijos. 
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealLineal::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
	double valor=0;
	for(int i=0; i<numHijos; i++)
	{		
		switch (i%3)
		{
		case 0:
			valor =   (*pMadre)*0.5 + (*pPadre)*0.5;	break;
		case 1:
			valor = (*pMadre)*1.5 - (*pPadre)*0.5;	break;
		case 2:
			valor =  -(*pMadre)*0.5 + (*pPadre)*1.5;	break;
		}
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores son tomados aleatoriamente 
del valor del gen padre o el del gen madre.
Los genes resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealDiscreto::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
	double azar, valor;
	for(int i=0;i<numHijos;i++)
	{		
		azar = (double)rand()/(double)RAND_MAX;
		azar>0.5 ?  valor= *pMadre : valor= *pPadre;
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales que se obtienen as�:
\f[ h_1= pMejor + \alpha _i(pPeor - pMejor)\f]
\f$\alpha _i\f$ se escoge aleatoriamente en el intervalo \f$[-0.25,1.25]\f$
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealIntermedioExtendido::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;        
    for(int i=0; i<numHijos; i++)
	{		
		double azar=(double)((double)rand()/(double)RAND_MAX);
		double Alfa = -0.25 + 1.5*azar;
		double valor = *pPadre + Alfa*( *pMadre - *pPadre );
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}


/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Es ignorado. Existe por compatibilidad.

Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales que se obtienen as�:
\f[ h = r (pMejor - pPeor) + pMejor \f]
\a r es un n�mero aleatorio perteneciente al intervalo [0, 1] y \a pMejor corresponde al
gen padre que tiene la mejor funci�n de evaluaci�n.
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealHeuristico::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
	for(int i=0;i<numHijos;i++)
	{
		double r = (double)rand()/(double)RAND_MAX;
		double valor = *pMadre + r*( *pMadre - *pPadre );		
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

#pragma warning(pop)


/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Valor que identifica al individuo de cuyos genes se realiza el cruce.

Crea \a numHijos genes cuyos valores corresponden a 
n�meros reales que se obtienen as�:

\f[ h_1= pMejor + rango_i \gamma \Delta \f]

Donde: 
\f[ \Delta= frac{pPeor - pMejor}{\left|Fevaluacion_{pMejor}-Fevaluacion_{pPeor}\right|} \f]

\f[ \Gamma= \sum_{k=0}^{15} \alpha_k 2^{-k} \f]
\f[rango_i = 0.5 (M�ximo - M�nimo)\f]  

\a pMejor corresponde al gen padre que tiene la mejor funci�n de evaluaci�n,
\a M�ximo y \a M�nimo corresponden a los l�mites establecidos en el gen y
\f$\alpha_k\f$ puede ser 0 � 1, con probabilidad de ser 1 de 0.0625,
Los valores resultantes se agregan al arreglo de genes \a pHijos*/
void OperadorCruceRealLinealBGA::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenReal *pMadre=(GenReal*)pMejor, *pPadre=(GenReal*)pPeor;
	double dif, delta, gamma, azar, alfa, rango, valor;
	for(int i=0; i<numHijos; i++)
	{
		dif=fabs( m_pAG->m_pPoblacionActual->getIndividuo(indice).objetivo() - 
			m_pAG->m_pPoblacionActual->getIndividuo(indice).getPareja()->objetivo());
		if(dif!=0)
			delta = (*pPadre - *pMadre)/(dif);
		else
			delta = 1.0;
		gamma=0.0;
		for(int j=0;j<16;j++)
		{
			azar=(double)rand()/(double)RAND_MAX;
			azar<0.0625?  alfa=1.0  : alfa=0.0;
			gamma+=alfa*pow(2.0,-(double)(j));
		}
		rango = 0.5*(pMadre->getMax() - pMadre->getMin());		
		valor = *pMadre  +  rango * gamma * delta;
		
		GenReal *gr=new GenReal(*pMadre);
		*gr = valor;
		pHijos->Adicionar(gr);
	}
}

#endif	//__GENREAL_CPP
