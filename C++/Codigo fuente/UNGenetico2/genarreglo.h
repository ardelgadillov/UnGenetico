#ifndef __GENARREGLO_H
#define __GENARREGLO_H

#include "genbool.h"
#include "genentero.h"
#include "genreal.h"

#pragma warning(push, 3)	//Evita  "warning(level 4) C4100" ocasionada por parámetros de funciones no referenciados


/************************************************************
                      GEN ARREGLO
************************************************************/

 
//!Clase derivada de la clase Gen, especializada en un gen de tipo arreglo de tamaño variable
/*!Se utiliza para representar y almacenar la información genética de variables 
cuya codificación se realiza con un arreglo de datos que pueden se enteros, 
reales o booleanos.
G puede ser GenBool, GenEntero o GenReal. T puede ser bool, long o double.*/
template<class G, class T>
class GenArreglo:public Gen
{
public:

	//!Constructor por copia	
	GenArreglo(const GenArreglo& origen);

    //!Constructor por defecto.
    GenArreglo(int TamanoMinimo=1, int TamanoMaximo=10, T ValorMinimo=0 , T ValorMaximo=1);

	//!Constructor con ValorInicial
    GenArreglo(int TamanoMinimo, int TamanoMaximo, T ValorMinimo, T ValorMaximo, T ValorInicial);
	
	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenArreglo<G,T> idéntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenArreglo<G,T>(*this));
	}
	
	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const GenArreglo<G,T>*)(&origen)));
	}

	//!Operador de asignación a partir de otro objeto GenArreglo<G,T>	
	const GenArreglo<G,T>& operator = ( const GenArreglo<G,T>& origen);

	//!Operador de asignación a partir de un objeto Arreglo<T>
	const GenArreglo<G,T>& operator = ( const Arreglo<T>& origen );

	//!Convierte el objeto GenArreglo<G,T> en un objeto Arreglo<T>
	void convertArreglo(Arreglo<T> &destino) const;

    //!Destructor.
    ~GenArreglo();
	
	//!Cambia el tamaño del arreglo	
	int setTam(int nuevoTam, bool crearAleatorios=false);
	
	//!Genera un arreglo de genes de tamaño y valores aleatorios	
    void generarAleatorio();

    //!Retorna un objeto correspondiente al operador de mutación establecido por defecto para genes de tipo arreglo
    OperadorMutacion *operadorMutacionDefecto() const;
	
	//!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes de tipo arreglo
    OperadorCruce *operadorCruceDefecto() const;
	
	//!Retorna el apuntador al gen indicado por \a pos en el arreglo de genes
	/*!
	\param pos Posición dentro del arreglo de genes
	\return Apuntador al Gen en la posición \a pos*/
	G& getGen(int pos) const
	{
		return m_pArregloGen->getObj(pos);
	}

	//!Retorna el valor actual del gen en la posición \a pos del arreglo de genes
	/*!
	\param pos Posición dentro del arreglo de genes
	\return Valor actual del gen en la posición \a pos*/
	T getVal(int pos) const
	{
		return m_pArregloGen->getObj(pos).getVal();
	}

	//!Asigna un nuevo valor al gen ubicado en la posición \a pos
	bool setVal(int pos, T valor );

	//!Retorna el GenEntero que almacena el tamaño del arreglo variable.	
	const GenEntero* getGenItems() const
	{
		return m_pGenItems;
	}

	//!Retorna el tamaño actual del arreglo de genes
	int getTam() const
	{
		return m_pArregloGen->getSize();
	}

	//!Retorna el tamaño mínimo del arreglo variable
	int getMinTam() const
	{
		return  m_pGenItems->getMin();
	}

	//!Retorna el tamaño máximo del arreglo variable
	int getMaxTam() const 
	{
		return  m_pGenItems->getMax();
	}

	//!Retorna el valor mínimo válido para los datos almacenados en el arreglo de genes
	T getMinVal() const
	{
		return  m_valMin;
	}

	//!Retorna el valor máximo válido para los datos almacenados en el arreglo de genes
	T getMaxVal() const
	{
		return  m_valMax;
	}
	
protected:

	//!Crea un nuevo objeto del tipo de genes contenidos en el arreglo y retorna su apuntador
	G* crearGen() const;

	//!Apuntador al GenEntero que almacena el tamaño actual del arreglo
	GenEntero* m_pGenItems;

    //!Arreglo que contiene la informacion genética, con genes del tipo especificado por \a G
    Arreglo<G> *m_pArregloGen;
    
	//!Mínimo valor válido para los datos contenidos en el arreglo    
    T m_valMin;

    //!Máximo valor válido para los datos contenidos en el arreglo    
    T m_valMax;

	//!Valor inicial para los datos contenidos en el arreglo    
    T m_valInicial;

	/*!Indica si se debe usar el valor inicial de cada gen al crearlo*/
    bool m_usarValInicial;
};


/******************************************************************
                     Operadores de Mutación
******************************************************************/

//!Clase derivada de la clase OperadorMutacion empleada en genes de tipo arreglo. G puede ser GenBool, GenEntero o GenReal. T puede ser bool, long o double
/*!Efectúa una mutación en el gen de tipo arreglo.
Cambia su tamaño dependiendo de la probabiilidad de mutación. El nuevo tamaño 
del arreglo se genera aleatoriamente entre los límites de tamaño establecidos.
Los valores de los elementos del arreglo mutan con la misma probabilidad, usando 
el operador de mutación por defecto para cada tipo de gen,*/
template< class G, class T >
class OperadorMutacionArreglo : public OperadorMutacion
{
public:

	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación..
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.01.	*/
    OperadorMutacionArreglo(double ProbabilidadMutacion=0.01):OperadorMutacion(ProbabilidadMutacion)
	{
		G gen;
		m_pOperadorMutacionGenes = gen.operadorMutacionDefecto();		
		m_pOperadorMutacionGenes->AsignarProbabilidadMutacion(ProbabilidadMutacion);
	}
	
	//!Destructor
	/*!Destruye objetos creados*/
    ~OperadorMutacionArreglo()
	{
		delete m_pOperadorMutacionGenes;
	}
	

	//!Asigna un nuevo valor a la probabilidad de mutación, tanto del tamaño, como de cada gen del arreglo de genes.
	/*!
	\param Probabilidad Nuevo valor de probabilidad de mutación. Se restringe al rango [0,1]
	\return Valor de probablidad de mutacion asignado.*/
	virtual double AsignarProbabilidadMutacion(double Probabilidad)
	{
		Probabilidad = restringir(Probabilidad, 0.0, 1.0);
		m_pOperadorMutacionGenes->AsignarProbabilidadMutacion(Probabilidad);
		return (m_ProbabilidadMutacion = Probabilidad);
	}

	//!Ejecuta una mutación sobre un objeto de la clase GenArreglo<G,T>
	void mutar(Gen *pGen);
	
protected:

    //!Existe por compatibilidad con la clase base. La mutación la realiza la funcion \a mutar().
    void mutarGen(Gen *g){};

	//!Apuntador al operador de mutacion de los genes que hacen parte del arreglo
	OperadorMutacion* m_pOperadorMutacionGenes;
};

/*******************************************************************
                        Operadores de Cruce
*******************************************************************/

//!Clase derivada de la clase OperadorCruce usada en genes de tipo arreglo. G puede ser GenBool, GenEntero o GenReal. T puede ser bool, long o double
/*!Efectúa un cruce entre dos genes de tipo arreglo, creando nuevos genes hijos. 
El tamaño de los nuevos genes es definido por el cruce de los tamaños de los padres.
Cada uno de los genes que hacen parte del nuevo arreglo, se obtiene cruzando 
los genes de la misma posición en el arreglo de los padres,
usando el operador de cruce por defecto para genes de tipo G.
Si el tamaño del nuevo gen es mayor que el tamaño del padre de menor tamaño, 
los elementos restantes se copian exactamente del padre de mayor tamaño.
Si el tamaño del nuevo gen es aún mayor que el tamaño de ambos padres, 
los genes restantes se crean aleatoriamente. */
template< class G, class T >
class OperadorCruceArreglo : public OperadorCruce
{
public:
  //!Constructor de la clase OperadorCruceArreglo
    OperadorCruceArreglo()
	{
		G gen;
		m_pOperadorCruceGenes = gen.operadorCruceDefecto();
		GenEntero g;
		m_pOperadorCruceTamanos = g.operadorCruceDefecto();
	}
  //!Destructor de la clase OperadorCruceArreglo
    ~OperadorCruceArreglo()
	{
		delete m_pOperadorCruceGenes;
		delete m_pOperadorCruceTamanos;
	}
  //!Define el proceso de cruce de los genes de tipo arreglo
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
protected:
	//!Apuntador al operador de cruce de los genes que hacen parte del arreglo
	OperadorCruce* m_pOperadorCruceGenes;
	//!Apuntador al operador de cruce para el tamaño del arreglo
	OperadorCruce* m_pOperadorCruceTamanos;
};




/************************************************************
                  GEN ARREGLO (Implementación)
************************************************************/


/*!Inicializa todos los miembros de la clase con valores válidos. Asigna a cada gen 
su valor por defecto. El tamaño inicial del arreglo es el promedio de los límites
de tamaño establecidos.
\param TamanoMinimo Tamaño mínimo que puede tomar el arreglo, Por defecto es 1.
\param TamanoMaximo Tamaño máximo que puede tomar el arreglo, Por defecto es 10.
\param ValorMinimo Valor mínimo que puede tomar cada gen, Por defecto es 0.
\param ValorMaximo Valor máximo que puede tomar cada gen, Por defecto es 1. */
template<class G, class T>
GenArreglo<G, T>::GenArreglo(int TamanoMinimo, int TamanoMaximo, T ValorMinimo , T ValorMaximo)
{
	if(TamanoMinimo < 1) TamanoMinimo = 1;
	if(TamanoMaximo < TamanoMinimo) TamanoMaximo = TamanoMinimo;
	if( (double)ValorMaximo < (double)ValorMinimo )  ValorMaximo = ValorMinimo;

	m_valMax = ValorMaximo;
	m_valMin = ValorMinimo;
	m_usarValInicial = false;
	m_pArregloGen = new Arreglo<G>;
	m_pGenItems = new GenEntero(TamanoMinimo, TamanoMaximo);

	setTam( (TamanoMinimo+TamanoMaximo)/2, false);
}


/*!Inicializa todos los miembros de la clase con valores válidos. Da a cada gen 
el valor inicial especificado, restringiéndolo a los valores límites. 
El tamaño inicial del arreglo es el promedio de los límites
de tamaño establecidos.
\param TamanoMinimo Tamaño mínimo que puede tomar el arreglo,
\param TamanoMaximo Tamaño máximo que puede tomar el arreglo,
\param ValorMinimo Valor mínimo que puede tomar cada gen,
\param ValorMaximo Valor máximo que puede tomar cada gen, 
\param ValorInicial Valor que se asigna a cada gen,*/
template<class G, class T>
GenArreglo<G, T>::GenArreglo(int TamanoMinimo, int TamanoMaximo, T ValorMinimo , T ValorMaximo, T ValorInicial)
{
	if(TamanoMinimo < 1) TamanoMinimo = 1;
	if(TamanoMaximo < TamanoMinimo) TamanoMaximo = TamanoMinimo;
	if(ValorMaximo < ValorMinimo )  ValorMaximo = ValorMinimo;

	m_pArregloGen = new Arreglo<G>;	
	m_valMax = ValorMaximo;
	m_valMin = ValorMinimo;
	m_valInicial = restringir(ValorInicial, ValorMinimo, ValorMaximo);
	m_usarValInicial = true;
	m_pGenItems = new GenEntero(TamanoMinimo, TamanoMaximo);
	setTam( (TamanoMinimo+TamanoMaximo)/2, false);
}


/*!Construye el nuevo objeto copiando idénticamente las propiedades 
de otro objeto de la clase GenArreglo.
\param origen Objeto del que se hace copia.*/
template<class G, class T>
GenArreglo<G, T>::GenArreglo(const GenArreglo<G,T>& origen)
{
	m_valMin = origen.m_valMin;
	m_valMax = origen.m_valMax;
	m_valInicial = origen.m_valInicial;
	m_usarValInicial = origen.m_usarValInicial;
	m_pGenItems = new GenEntero(*origen.m_pGenItems);
	m_pArregloGen = new Arreglo<G>(*origen.m_pArregloGen);
}




/*!Asigna nuevos valores al gen copiando las propiedades de otro gen del mismo tipo
\param origen Objeto del que se hace copia
\return Referencia al objeto actual.*/
template<class G, class T>
const GenArreglo<G,T>& GenArreglo<G, T>::operator = ( const GenArreglo<G,T>& origen )
{
	m_valMin = origen.m_valMin;
	m_valMax = origen.m_valMax;
	m_valInicial = origen.m_valInicial;
	m_usarValInicial = origen.m_usarValInicial;
	*m_pGenItems = *origen.m_pGenItems;
	*m_pArregloGen = *origen.m_pArregloGen;
	return *this;
}


/*!Asigna nuevos valores al gen copiando los valores de un arreglo de datos de tipo T
\param origen Arreglo del que se toman los valores
\return Referencia al objeto actual.*/
template<class G, class T>
const GenArreglo<G,T>& GenArreglo<G, T>::operator = ( const Arreglo<T>& origen )
{
	int tam = origen.getSize();
	setTam(tam);
	for(int i=0; i<tam; i++)
		setVal(i, origen[i]  );
	return *this;
}


/*!Destruye todos los objetos contenidos en el arreglo*/
template<class G, class T>
GenArreglo<G, T>::~GenArreglo()
{
	delete m_pGenItems;
	delete m_pArregloGen;
}


/*!Asigna al gen un nuevo tamaño y valores aleatorios dentro de sus respectivos rangos.*/
template<class G, class T>
void GenArreglo<G, T>::generarAleatorio()
{
	int old = m_pGenItems->getVal();
	//el nuevo tamaño es generado aleatoriamente entre los limites
	double azar = (double)rand()/(double)RAND_MAX ;
	int nuevo = redondear(m_pGenItems->getMin() + azar*(m_pGenItems->getMax() - m_pGenItems->getMin()));
	for(int i=0; i<old && i<nuevo; i++)
	{
		m_pArregloGen->getObj(i).generarAleatorio();
	}
	setTam(nuevo, true);
}


/*!Si el objeto GenArreglo fue creado utilizando un velor inicial establecido, 
el nuevo gen se crea usando este mismo valor. De lo contrario se crea con el valor 
por defecto del tipo G.
\return Apuntador a un objeto del tipo de genes del arreglo,*/
template<class G, class T>
G* GenArreglo<G, T>::crearGen() const
{
	if(m_usarValInicial)
		return ( new G(m_valMin, m_valMax, m_valInicial) );
	else
		return ( new G(m_valMin, m_valMax) );
}



/*!Si el nuevo tamaño es menor que el anterior, se eliminan los genes restantes. Si 
es mayor, los nuevos genes son creados aleatoriamente dependiendo del valor de 
\a crearAleatorios
\param nuevoTam Nuevo tamaño del arreglo. 
\param crearAleatorios Si su valor es true se crean genes aleatorios. Si es false se crean genes con valores iniciales preestablecidos
\return Nuevo tamaño del arreglo. */
template<class G, class T>
int GenArreglo<G,T>::setTam(int nuevoTam, bool crearAleatorios)
{	
	int old=m_pArregloGen->getSize();
	if( nuevoTam==old ) return old;	//no hay nada que cambiar
	nuevoTam = restringir(nuevoTam, (int)m_pGenItems->getMin(), (int)m_pGenItems->getMax() );

	m_pArregloGen->asignarMemoria(nuevoTam);
	*m_pGenItems = nuevoTam;
	for(int i=old; i<nuevoTam; i++)
	{
		G *pNuevo = crearGen();
		if(crearAleatorios) pNuevo->generarAleatorio();
		m_pArregloGen->Adicionar(pNuevo);
	}
	m_pArregloGen->Truncar(nuevoTam, true, false); //eliminar objetos pero no liberar memoria.
	return nuevoTam;
}


/*!Si \a pos no es una posición válida en al arreglo, no se asigna 
el valor especificado.
\param pos Posición dentro del arreglo de genes
\param valor Valor a asignar al gen en la posición \a pos
\return true si el valor especificado fue asignado, false en caso contrario.*/
template<class G, class T>
bool GenArreglo<G,T>::setVal(int pos, T valor )
{
	if(pos<0 || pos >= m_pArregloGen->getSize()) return false;
	m_pArregloGen->getObj(pos) = valor;
	return true;
}


/*!El arreglo destino toma el mismo tamaño del GenArreglo. Los valores del arreglo 
destino toman los mismos valores de los genes contenidos en el GenArreglo, en sus
respectivas posiciones.
\param destino Referencia al arreglo destino */
template<class G, class T>
void GenArreglo<G,T>::convertArreglo(Arreglo<T> &destino) const
{
	int tamg=m_pArregloGen->getSize();

	destino.asignarMemoria(tamg);
	int j, tam=destino.getSize();
	for(j=0; j<tam && j<tamg; j++)
		destino[j] = m_pArregloGen->getObj(j).getVal();
	for(j=tam; j<tamg; j++)
	{
		T* nuevoDato = new T( m_pArregloGen->getObj(j).getVal() );
		destino.Adicionar(nuevoDato);
	}
	destino.Truncar(tamg);
}


/*!
\return Apuntador al nuevo objeto de la clase OperadorMutacionArreglo<G,T>*/
template<class G, class T>
OperadorMutacion* GenArreglo<G,T>::operadorMutacionDefecto() const
{
	return (new OperadorMutacionArreglo<G,T>);
}


/*!
\return Apuntador al nuevo objeto de la clase OperadorCruceArreglo<G,T>*/
template<class G, class T>
OperadorCruce* GenArreglo<G,T>::operadorCruceDefecto() const
{	
	return (new OperadorCruceArreglo<G,T>);
}


/******************************************************************
                   OperadorMutacionArreglo (Implementacion)
******************************************************************/



/*!Método sobrecargado de la clase operador mutacion. 
\param pGen Apuntador al objeto de la clase GenArreglo<G,T> que será sometido a mutación.
*/
template< class G, class T >
void OperadorMutacionArreglo<G, T>::mutar(Gen *pGen)
{
	GenArreglo<G,T> *gr=(GenArreglo<G,T> *)pGen;

	//decidimos si debe mutar el tamaño
	double azar= (double)rand()/(double)RAND_MAX;
	if(azar<m_ProbabilidadMutacion)
	{		
		//obtener aleatoriamente el nuevo tamaño	
		azar = (double)rand()/(double)RAND_MAX ;	
		int nuevo = redondear(gr->getMinTam() + azar*(gr->getMaxTam() - gr->getMinTam() ) );		
		//cambiar el tamaño creando aleatorios si es necesario
		gr->setTam(nuevo, true);
	}

	//se hace mutar cada gen dependiendo de su probabilidad de mutación
	int tam=gr->getTam();
	for(int i=0; i<tam; i++)
	{
		m_pOperadorMutacionGenes->mutar( &gr->getGen(i) );
	}
}


/*******************************************************************
                OperadorCruceArreglo (Inplementación)
*******************************************************************/



/*!
\param pMejor Gen del individuo padre con mejor función de evaluación.
\param pPeor Gen del individuo padre con peor función de evaluación.
\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
\param numHijos Numero de genes hijos a crear en el cruce.
\param indice Valor que identifica al individuo de cuyos genes se realiza el cruce.
*/
template< class G, class T >
void OperadorCruceArreglo<G,T>::cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice)
{
	GenArreglo<G,T> *pMadre = (GenArreglo<G,T> *)pMejor;
	GenArreglo<G,T> *pPadre = (GenArreglo<G,T> *)pPeor;
	int j;
	for(int i=0;i<numHijos;i++)
	{
		//gr será el nuevo hijo
		GenArreglo<G,T> *gr = new GenArreglo<G,T>( pMadre->getMinTam(), pMadre->getMaxTam(), 
			pMadre->getMinVal() , pMadre->getMaxVal() );

		//pNuevoTam: arreglo donde estará el GenEntero del nuevo tamaño
		Arreglo<GenEntero>* pNuevoTam = new Arreglo<GenEntero>;	
		
		//definir y ajustar el tamaño del hijo cruzando los tamaños de los padres
		m_pOperadorCruceTamanos->cruzarGenes( (Gen*)pMadre->getGenItems(), (Gen*)pPadre->getGenItems(), (Arreglo<Gen> *)pNuevoTam, 1, -1);	
		int tnuevo = pNuevoTam->getObj(0);
		delete pNuevoTam;		
		tnuevo = gr->setTam( tnuevo , true); 

		//encontrar cual de los padres tiene mayor tamaño y asignarlo a pMayor
		int tmenor= _MINIMO( pPadre->getTam(), pMadre->getTam() );
		int tmayor= _MAXIMO( pPadre->getTam(), pMadre->getTam() );
		GenArreglo<G,T> *pMayor;
		if(tmayor==pPadre->getTam() )
			pMayor=pPadre;
		else
			pMayor=pMadre;
		
		//cruzar los genes uno a uno
		for(j=0; j<tmenor && j<tnuevo; j++)
		{			
			Arreglo<G> *Hijo = new Arreglo<G>; 
			m_pOperadorCruceGenes->cruzarGenes( &pMadre->getGen(j), &pPadre->getGen(j), (Arreglo<Gen>*)Hijo , 1, -1);
			gr->setVal(j, Hijo->getObj(0).getVal() );			
			delete Hijo;
		}
		//asignar el valor de los genes del padre de mayor tamaño
		for(j=tmenor; j<tmayor && j<tnuevo; j++)
		{
			gr->setVal(j, pMayor->getVal(j) );
		}
		//una vez obtenido el hijo se adiciona al arreglo recibido como parametro
 		pHijos->Adicionar(gr);
	}
}





/****************************************************************************/
/*				DEFINICION DE TIPOS PARA USO DE GENARREGLO					*/
/****************************************************************************/

//!Define un arreglo de datos de tipo booleano \a (bool)
typedef Arreglo<bool>							ArregloBool;
//!Define un arreglo de datos de tipo entero \a (long)
typedef Arreglo<long>							ArregloEntero;
//!Define un arreglo de datos de tipo real \a (double)
typedef Arreglo<double>							ArregloReal;
//!Define un gen de tipo arreglo con datos de tipo booleano \a (bool)
typedef GenArreglo<GenBool,bool>				GenArregloBool;
//!Define un gen de tipo arreglo con datos de tipo entero \a (long)
typedef GenArreglo<GenEntero,long>				GenArregloEntero;
//!Define un gen de tipo arreglo con datos de tipo real \a (double)
typedef GenArreglo<GenReal,double>				GenArregloReal;
//!Define un operador de mutación para genes de tipo arreglo con datos de tipo booleano \a (bool)
typedef OperadorMutacionArreglo<GenBool,bool>	OperadorMutacionArregloBool;
//!Define un operador de mutación para genes de tipo arreglo con datos de tipo entero \a (long)
typedef OperadorMutacionArreglo<GenEntero,long> OperadorMutacionArregloEntero;
//!Define un operador de mutación para genes de tipo arreglo con datos de tipo real \a (double)
typedef OperadorMutacionArreglo<GenReal,double> OperadorMutacionArregloReal;
//!Define un operador de cruce para genes de tipo arreglo con datos de tipo booleano \a (bool)
typedef OperadorCruceArreglo<GenBool,bool>		OperadorCruceArregloBool;
//!Define un operador de cruce para genes de tipo arreglo con datos de tipo entero \a (long)
typedef OperadorCruceArreglo<GenEntero,long>	OperadorCruceArregloEntero;
//!Define un operador de cruce para genes de tipo arreglo con datos de tipo real \a (double)
typedef OperadorCruceArreglo<GenReal,double>	OperadorCruceArregloReal;


/****************************************************************************/
/*				DEFINICION DE MACROS PARA USO DE GENARREGLO					*/
/****************************************************************************/


#ifndef DOXYGEN_SHOULD_SKIP_THIS		//Esto es para no incluir en la ayuda.
/*Adiciona un \a tipoGenArreglo en la posición \a pos del genoma 
del individuo apuntado por \a pIndividuo y realiza la actualización de la variable \a var 
(codificación o decodificación) con la información del Gen. Al crear el GenArreglo
utiliza el \a constructor, el cual debe ser de la forma:  (param1, param2, ...)	*/
#define __ADICIONAR_GENARREGLO(pIndividuo, pos, var, tipoGenArreglo,  constructor)	\
	{																		\
		if(estado==ESTADO_CREAR)											\
		{																	\
			Gen *pGen=new tipoGenArreglo constructor;						\
			pIndividuo->adicionarGen(pGen);									\
		}																	\
		else if(estado==ESTADO_CODIFICAR)									\
		{																	\
			tipoGenArreglo &_gen = (tipoGenArreglo&)(pIndividuo->getGen(pos));	\
			_gen = var;													\
		}																	\
		else if(estado==ESTADO_DECODIFICAR)									\
		{																	\
			tipoGenArreglo &_gen = (tipoGenArreglo&)(pIndividuo->getGen(pos));	\
			_gen.convertArreglo(var);										\
		}																	\
	}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


/*Las siguientes tres MACROS son versiones especializadas de __ADICIONAR_GENARREGLO
para facilitar su uso*/


//!Macro que adiciona un objeto GenArregloBool a un individuo
/*!
Se debe utilizar solamente en la función \a codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su propósito particular es insertar un objeto GenArregloBool en una posición determinada
del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tamaño mínimo
y tamaño máximo. 

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloBool, 
\param tamMin Tamaño mínimo que puede tomar el gen.
\param tamMax Tamaño máximo que puede tomar el gen.
*/
#define ADICIONAR_GENARREGLO_BOOL(pIndividuo, pos, var, tamMin, tamMax)				\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloBool,  (tamMin, tamMax) )


//!Macro que adiciona un objeto GenArregloEntero a un individuo
/*!
Se debe utilizar solamente en la función codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su propósito particular es insertar un objeto GenArregloEntero en una posición determinada
del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tamaño mínimo, 
tamaño máximo y los valores mínimo, máximo e inicial.


\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloEntero, 
\param tamMin Tamaño mínimo que puede tomar el gen.
\param tamMax Tamaño máximo que puede tomar el gen.
\param valMin Valor mínimo que puede tomar cada gen del arreglo.
\param valMax Valor máximo que puede tomar cada gen del arreglo.
\param valInicial Valor inicial que toma cada gen del arreglo.*/
#define ADICIONAR_GENARREGLO_ENTERO(pIndividuo, pos, var, tamMin, tamMax, valMin, valMax, valInicial)	\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloEntero, (tamMin, tamMax, valMin, valMax, valInicial) )


//!Macro que adiciona un objeto GenArregloReal a un individuo
/*!
Se debe utilizar solamente en la función \a codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su propósito particular es insertar un objeto GenArregloReal en una posición determinada
del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tamaño mínimo, 
tamaño máximo y los valores mínimo, máximo e inicial.


\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloReal, 
\param tamMin Tamaño mínimo que puede tomar el gen.
\param tamMax Tamaño máximo que puede tomar el gen.
\param valMin Valor mínimo que puede tomar cada gen del arreglo.
\param valMax Valor máximo que puede tomar cada gen del arreglo.
\param valInicial Valor inicial que toma cada gen del arreglo.*/
#define ADICIONAR_GENARREGLO_REAL(pIndividuo, pos, var, tamMin, tamMax, valMin, valMax, valInicial)	\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloReal, (tamMin, tamMax, valMin, valMax, valInicial) )



#pragma warning(pop)



#endif	//__GENARREGLO_H
