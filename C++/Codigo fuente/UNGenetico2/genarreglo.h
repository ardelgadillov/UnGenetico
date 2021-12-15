#ifndef __GENARREGLO_H
#define __GENARREGLO_H

#include "genbool.h"
#include "genentero.h"
#include "genreal.h"

#pragma warning(push, 3)	//Evita  "warning(level 4) C4100" ocasionada por par�metros de funciones no referenciados


/************************************************************
                      GEN ARREGLO
************************************************************/

 
//!Clase derivada de la clase Gen, especializada en un gen de tipo arreglo de tama�o variable
/*!Se utiliza para representar y almacenar la informaci�n gen�tica de variables 
cuya codificaci�n se realiza con un arreglo de datos que pueden se enteros, 
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
	\return Apuntador a un nuevo objeto GenArreglo<G,T> id�ntico al actual.*/
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

	//!Operador de asignaci�n a partir de otro objeto GenArreglo<G,T>	
	const GenArreglo<G,T>& operator = ( const GenArreglo<G,T>& origen);

	//!Operador de asignaci�n a partir de un objeto Arreglo<T>
	const GenArreglo<G,T>& operator = ( const Arreglo<T>& origen );

	//!Convierte el objeto GenArreglo<G,T> en un objeto Arreglo<T>
	void convertArreglo(Arreglo<T> &destino) const;

    //!Destructor.
    ~GenArreglo();
	
	//!Cambia el tama�o del arreglo	
	int setTam(int nuevoTam, bool crearAleatorios=false);
	
	//!Genera un arreglo de genes de tama�o y valores aleatorios	
    void generarAleatorio();

    //!Retorna un objeto correspondiente al operador de mutaci�n establecido por defecto para genes de tipo arreglo
    OperadorMutacion *operadorMutacionDefecto() const;
	
	//!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes de tipo arreglo
    OperadorCruce *operadorCruceDefecto() const;
	
	//!Retorna el apuntador al gen indicado por \a pos en el arreglo de genes
	/*!
	\param pos Posici�n dentro del arreglo de genes
	\return Apuntador al Gen en la posici�n \a pos*/
	G& getGen(int pos) const
	{
		return m_pArregloGen->getObj(pos);
	}

	//!Retorna el valor actual del gen en la posici�n \a pos del arreglo de genes
	/*!
	\param pos Posici�n dentro del arreglo de genes
	\return Valor actual del gen en la posici�n \a pos*/
	T getVal(int pos) const
	{
		return m_pArregloGen->getObj(pos).getVal();
	}

	//!Asigna un nuevo valor al gen ubicado en la posici�n \a pos
	bool setVal(int pos, T valor );

	//!Retorna el GenEntero que almacena el tama�o del arreglo variable.	
	const GenEntero* getGenItems() const
	{
		return m_pGenItems;
	}

	//!Retorna el tama�o actual del arreglo de genes
	int getTam() const
	{
		return m_pArregloGen->getSize();
	}

	//!Retorna el tama�o m�nimo del arreglo variable
	int getMinTam() const
	{
		return  m_pGenItems->getMin();
	}

	//!Retorna el tama�o m�ximo del arreglo variable
	int getMaxTam() const 
	{
		return  m_pGenItems->getMax();
	}

	//!Retorna el valor m�nimo v�lido para los datos almacenados en el arreglo de genes
	T getMinVal() const
	{
		return  m_valMin;
	}

	//!Retorna el valor m�ximo v�lido para los datos almacenados en el arreglo de genes
	T getMaxVal() const
	{
		return  m_valMax;
	}
	
protected:

	//!Crea un nuevo objeto del tipo de genes contenidos en el arreglo y retorna su apuntador
	G* crearGen() const;

	//!Apuntador al GenEntero que almacena el tama�o actual del arreglo
	GenEntero* m_pGenItems;

    //!Arreglo que contiene la informacion gen�tica, con genes del tipo especificado por \a G
    Arreglo<G> *m_pArregloGen;
    
	//!M�nimo valor v�lido para los datos contenidos en el arreglo    
    T m_valMin;

    //!M�ximo valor v�lido para los datos contenidos en el arreglo    
    T m_valMax;

	//!Valor inicial para los datos contenidos en el arreglo    
    T m_valInicial;

	/*!Indica si se debe usar el valor inicial de cada gen al crearlo*/
    bool m_usarValInicial;
};


/******************************************************************
                     Operadores de Mutaci�n
******************************************************************/

//!Clase derivada de la clase OperadorMutacion empleada en genes de tipo arreglo. G puede ser GenBool, GenEntero o GenReal. T puede ser bool, long o double
/*!Efect�a una mutaci�n en el gen de tipo arreglo.
Cambia su tama�o dependiendo de la probabiilidad de mutaci�n. El nuevo tama�o 
del arreglo se genera aleatoriamente entre los l�mites de tama�o establecidos.
Los valores de los elementos del arreglo mutan con la misma probabilidad, usando 
el operador de mutaci�n por defecto para cada tipo de gen,*/
template< class G, class T >
class OperadorMutacionArreglo : public OperadorMutacion
{
public:

	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n..
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.01.	*/
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
	

	//!Asigna un nuevo valor a la probabilidad de mutaci�n, tanto del tama�o, como de cada gen del arreglo de genes.
	/*!
	\param Probabilidad Nuevo valor de probabilidad de mutaci�n. Se restringe al rango [0,1]
	\return Valor de probablidad de mutacion asignado.*/
	virtual double AsignarProbabilidadMutacion(double Probabilidad)
	{
		Probabilidad = restringir(Probabilidad, 0.0, 1.0);
		m_pOperadorMutacionGenes->AsignarProbabilidadMutacion(Probabilidad);
		return (m_ProbabilidadMutacion = Probabilidad);
	}

	//!Ejecuta una mutaci�n sobre un objeto de la clase GenArreglo<G,T>
	void mutar(Gen *pGen);
	
protected:

    //!Existe por compatibilidad con la clase base. La mutaci�n la realiza la funcion \a mutar().
    void mutarGen(Gen *g){};

	//!Apuntador al operador de mutacion de los genes que hacen parte del arreglo
	OperadorMutacion* m_pOperadorMutacionGenes;
};

/*******************************************************************
                        Operadores de Cruce
*******************************************************************/

//!Clase derivada de la clase OperadorCruce usada en genes de tipo arreglo. G puede ser GenBool, GenEntero o GenReal. T puede ser bool, long o double
/*!Efect�a un cruce entre dos genes de tipo arreglo, creando nuevos genes hijos. 
El tama�o de los nuevos genes es definido por el cruce de los tama�os de los padres.
Cada uno de los genes que hacen parte del nuevo arreglo, se obtiene cruzando 
los genes de la misma posici�n en el arreglo de los padres,
usando el operador de cruce por defecto para genes de tipo G.
Si el tama�o del nuevo gen es mayor que el tama�o del padre de menor tama�o, 
los elementos restantes se copian exactamente del padre de mayor tama�o.
Si el tama�o del nuevo gen es a�n mayor que el tama�o de ambos padres, 
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
	//!Apuntador al operador de cruce para el tama�o del arreglo
	OperadorCruce* m_pOperadorCruceTamanos;
};




/************************************************************
                  GEN ARREGLO (Implementaci�n)
************************************************************/


/*!Inicializa todos los miembros de la clase con valores v�lidos. Asigna a cada gen 
su valor por defecto. El tama�o inicial del arreglo es el promedio de los l�mites
de tama�o establecidos.
\param TamanoMinimo Tama�o m�nimo que puede tomar el arreglo, Por defecto es 1.
\param TamanoMaximo Tama�o m�ximo que puede tomar el arreglo, Por defecto es 10.
\param ValorMinimo Valor m�nimo que puede tomar cada gen, Por defecto es 0.
\param ValorMaximo Valor m�ximo que puede tomar cada gen, Por defecto es 1. */
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


/*!Inicializa todos los miembros de la clase con valores v�lidos. Da a cada gen 
el valor inicial especificado, restringi�ndolo a los valores l�mites. 
El tama�o inicial del arreglo es el promedio de los l�mites
de tama�o establecidos.
\param TamanoMinimo Tama�o m�nimo que puede tomar el arreglo,
\param TamanoMaximo Tama�o m�ximo que puede tomar el arreglo,
\param ValorMinimo Valor m�nimo que puede tomar cada gen,
\param ValorMaximo Valor m�ximo que puede tomar cada gen, 
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


/*!Construye el nuevo objeto copiando id�nticamente las propiedades 
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


/*!Asigna al gen un nuevo tama�o y valores aleatorios dentro de sus respectivos rangos.*/
template<class G, class T>
void GenArreglo<G, T>::generarAleatorio()
{
	int old = m_pGenItems->getVal();
	//el nuevo tama�o es generado aleatoriamente entre los limites
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



/*!Si el nuevo tama�o es menor que el anterior, se eliminan los genes restantes. Si 
es mayor, los nuevos genes son creados aleatoriamente dependiendo del valor de 
\a crearAleatorios
\param nuevoTam Nuevo tama�o del arreglo. 
\param crearAleatorios Si su valor es true se crean genes aleatorios. Si es false se crean genes con valores iniciales preestablecidos
\return Nuevo tama�o del arreglo. */
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


/*!Si \a pos no es una posici�n v�lida en al arreglo, no se asigna 
el valor especificado.
\param pos Posici�n dentro del arreglo de genes
\param valor Valor a asignar al gen en la posici�n \a pos
\return true si el valor especificado fue asignado, false en caso contrario.*/
template<class G, class T>
bool GenArreglo<G,T>::setVal(int pos, T valor )
{
	if(pos<0 || pos >= m_pArregloGen->getSize()) return false;
	m_pArregloGen->getObj(pos) = valor;
	return true;
}


/*!El arreglo destino toma el mismo tama�o del GenArreglo. Los valores del arreglo 
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



/*!M�todo sobrecargado de la clase operador mutacion. 
\param pGen Apuntador al objeto de la clase GenArreglo<G,T> que ser� sometido a mutaci�n.
*/
template< class G, class T >
void OperadorMutacionArreglo<G, T>::mutar(Gen *pGen)
{
	GenArreglo<G,T> *gr=(GenArreglo<G,T> *)pGen;

	//decidimos si debe mutar el tama�o
	double azar= (double)rand()/(double)RAND_MAX;
	if(azar<m_ProbabilidadMutacion)
	{		
		//obtener aleatoriamente el nuevo tama�o	
		azar = (double)rand()/(double)RAND_MAX ;	
		int nuevo = redondear(gr->getMinTam() + azar*(gr->getMaxTam() - gr->getMinTam() ) );		
		//cambiar el tama�o creando aleatorios si es necesario
		gr->setTam(nuevo, true);
	}

	//se hace mutar cada gen dependiendo de su probabilidad de mutaci�n
	int tam=gr->getTam();
	for(int i=0; i<tam; i++)
	{
		m_pOperadorMutacionGenes->mutar( &gr->getGen(i) );
	}
}


/*******************************************************************
                OperadorCruceArreglo (Inplementaci�n)
*******************************************************************/



/*!
\param pMejor Gen del individuo padre con mejor funci�n de evaluaci�n.
\param pPeor Gen del individuo padre con peor funci�n de evaluaci�n.
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
		//gr ser� el nuevo hijo
		GenArreglo<G,T> *gr = new GenArreglo<G,T>( pMadre->getMinTam(), pMadre->getMaxTam(), 
			pMadre->getMinVal() , pMadre->getMaxVal() );

		//pNuevoTam: arreglo donde estar� el GenEntero del nuevo tama�o
		Arreglo<GenEntero>* pNuevoTam = new Arreglo<GenEntero>;	
		
		//definir y ajustar el tama�o del hijo cruzando los tama�os de los padres
		m_pOperadorCruceTamanos->cruzarGenes( (Gen*)pMadre->getGenItems(), (Gen*)pPadre->getGenItems(), (Arreglo<Gen> *)pNuevoTam, 1, -1);	
		int tnuevo = pNuevoTam->getObj(0);
		delete pNuevoTam;		
		tnuevo = gr->setTam( tnuevo , true); 

		//encontrar cual de los padres tiene mayor tama�o y asignarlo a pMayor
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
		//asignar el valor de los genes del padre de mayor tama�o
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
//!Define un operador de mutaci�n para genes de tipo arreglo con datos de tipo booleano \a (bool)
typedef OperadorMutacionArreglo<GenBool,bool>	OperadorMutacionArregloBool;
//!Define un operador de mutaci�n para genes de tipo arreglo con datos de tipo entero \a (long)
typedef OperadorMutacionArreglo<GenEntero,long> OperadorMutacionArregloEntero;
//!Define un operador de mutaci�n para genes de tipo arreglo con datos de tipo real \a (double)
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
/*Adiciona un \a tipoGenArreglo en la posici�n \a pos del genoma 
del individuo apuntado por \a pIndividuo y realiza la actualizaci�n de la variable \a var 
(codificaci�n o decodificaci�n) con la informaci�n del Gen. Al crear el GenArreglo
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
Se debe utilizar solamente en la funci�n \a codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su prop�sito particular es insertar un objeto GenArregloBool en una posici�n determinada
del individuo apuntado por \a pIndividuo.
Este objeto estar� asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tama�o m�nimo
y tama�o m�ximo. 

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posici�n en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloBool, 
\param tamMin Tama�o m�nimo que puede tomar el gen.
\param tamMax Tama�o m�ximo que puede tomar el gen.
*/
#define ADICIONAR_GENARREGLO_BOOL(pIndividuo, pos, var, tamMin, tamMax)				\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloBool,  (tamMin, tamMax) )


//!Macro que adiciona un objeto GenArregloEntero a un individuo
/*!
Se debe utilizar solamente en la funci�n codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su prop�sito particular es insertar un objeto GenArregloEntero en una posici�n determinada
del individuo apuntado por \a pIndividuo.
Este objeto estar� asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tama�o m�nimo, 
tama�o m�ximo y los valores m�nimo, m�ximo e inicial.


\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posici�n en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloEntero, 
\param tamMin Tama�o m�nimo que puede tomar el gen.
\param tamMax Tama�o m�ximo que puede tomar el gen.
\param valMin Valor m�nimo que puede tomar cada gen del arreglo.
\param valMax Valor m�ximo que puede tomar cada gen del arreglo.
\param valInicial Valor inicial que toma cada gen del arreglo.*/
#define ADICIONAR_GENARREGLO_ENTERO(pIndividuo, pos, var, tamMin, tamMax, valMin, valMax, valInicial)	\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloEntero, (tamMin, tamMax, valMin, valMax, valInicial) )


//!Macro que adiciona un objeto GenArregloReal a un individuo
/*!
Se debe utilizar solamente en la funci�n \a codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su prop�sito particular es insertar un objeto GenArregloReal en una posici�n determinada
del individuo apuntado por \a pIndividuo.
Este objeto estar� asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico, para la cual se establecen el tama�o m�nimo, 
tama�o m�ximo y los valores m�nimo, m�ximo e inicial.


\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posici�n en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado. Debe ser de tipo ArregloReal, 
\param tamMin Tama�o m�nimo que puede tomar el gen.
\param tamMax Tama�o m�ximo que puede tomar el gen.
\param valMin Valor m�nimo que puede tomar cada gen del arreglo.
\param valMax Valor m�ximo que puede tomar cada gen del arreglo.
\param valInicial Valor inicial que toma cada gen del arreglo.*/
#define ADICIONAR_GENARREGLO_REAL(pIndividuo, pos, var, tamMin, tamMax, valMin, valMax, valInicial)	\
	__ADICIONAR_GENARREGLO(pIndividuo, pos, var, GenArregloReal, (tamMin, tamMax, valMin, valMax, valInicial) )



#pragma warning(pop)



#endif	//__GENARREGLO_H
